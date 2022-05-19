

#include "etiss/IntegratedLibrary/Semihosting.h"
#include "etiss/CPUCore.h"
#include <unistd.h>
#include <errno.h>

using namespace etiss::plugin;

Semihosting::~Semihosting()
{
    // TODO: close files
}

std::string Semihosting::_getPluginName() const
{
    return "semihosting";
}

void Semihosting::init(ETISS_CPU *cpu, ETISS_System *system, etiss::CPUArch *arch)
{
    arch_ = arch;
    cpu_ = cpu;
    system_ = system;
}

void Semihosting::cleanup()
{
    arch_ = nullptr;
    cpu_ = nullptr;
    system_ = nullptr;
}

void Semihosting::finalizeInstr(etiss::instr::Instruction &instr) const
{
    std::string pcode = getPointerCode();
    if (instr.name_ == "ebreak")
    {
        instr.addCallback(
            [pcode](etiss::instr::BitArray &ba, etiss::CodeSet &cs, etiss::instr::InstructionContext &context)
            {
                std::stringstream ss;

                ss << "if (semihosting_maybe_execute(" << pcode << ")) {\n";
                ss << "    cpu->instructionPointer += 4ULL;\n";
                ss << "    break;\n";
                ss << "}\n";

                cs.append(etiss::CodePart::PREINITIALDEBUGRETURNING).code() = ss.str();
                return true;
            },
            0);
    }
}

void Semihosting::finalizeInstrSet(etiss::instr::ModedInstructionSet &mis) const
{
    mis.foreach (
        [this](etiss::instr::VariableInstructionSet &vis)
        {
            vis.foreach ([this](etiss::instr::InstructionSet &is)
                         { is.foreach ([this](etiss::instr::Instruction &instr) { finalizeInstr(instr); }); });
        });
}

void Semihosting::finalizeCodeBlock(etiss::CodeBlock &block) const
{
    block.fileglobalCode().insert("extern void semihosting_maybe_execute(void *);\n\n");
}

void *Semihosting::getPluginHandle()
{
    return (void *)this;
}

etiss_uint64 Semihosting::readFromStructUInt(etiss_uint64 address, int fieldNo)
{
    int width = 8; // TODO: change depending on ISA
    etiss_uint64 field;
    system_->dbg_read(system_->handle, address + width * fieldNo, (etiss_uint8 *)&field, width); // TODO throw error
    return field;
}

std::vector<etiss_uint8> Semihosting::readSystemMemory(etiss_uint64 address, etiss_uint64 length)
{
    std::vector<etiss_uint8> buffer;
    buffer.resize(length);
    system_->dbg_read(system_->handle, address, buffer.data(), length);
    return buffer;
}

void Semihosting::writeSystemMemory(etiss_uint64 address, std::vector<etiss_uint8> data)
{
    system_->dbg_write(system_->handle, address, data.data(), data.size());
}

const char *mode_strs[] = { "r", "rb", "r+", "r+b", "w", "wb", "w+", "w+b", "a", "ab", "a+", "a+b" };

etiss_int64 Semihosting::semihostingCall(etiss_uint64 operationNumber, etiss_uint64 parameter)
{
    switch (operationNumber)
    {
    // all operations that have the file descriptor as their first argument
    case 0x02: // SYS_CLOSE
    case 0x05: // SYS_WRITE
    case 0x06: // SYS_READ
    case 0x09: // SYS_ISTTY
    case 0x0c: // SYS_FLEN
    {
        etiss_uint64 fd = readFromStructUInt(parameter, 0);
        if (openFiles.count(fd) == 0)
        {
            std::stringstream ss;
            ss << "Semihosting: invalid file descriptor " << fd << " for semihosting call 0x" << std::hex
               << std::setfill('0') << std::setw(2) << operationNumber;
            etiss::log(etiss::INFO, ss.str());
            semihosting_errno = EBADF;
            return -1;
        }
        FILE *file = openFiles[fd];

        switch (operationNumber)
        {
        case 0x02: // SYS_CLOSE
        {
            std::stringstream ss;
            ss << "Semihosting: SYS_CLOSE fd " << fd;
            etiss::log(etiss::INFO, ss.str());

            openFiles.erase(fd);
            if (fd > 2)
                // do not close stdin, stdout, stderr of host process
                fclose(file);
            return 0;
        }
        case 0x05: // SYS_WRITE
        {
            etiss_uint64 address = readFromStructUInt(parameter, 1);
            etiss_uint64 count = readFromStructUInt(parameter, 2);

            std::stringstream ss;
            ss << "Semihosting: SYS_WRITE fd " << fd;
            etiss::log(etiss::INFO, ss.str());

            std::vector<etiss_uint8> buffer = readSystemMemory(address, count);
            etiss_uint64 num_written = fwrite(buffer.data(), 1, count, file);

            return count - num_written;
        }
        case 0x06: // SYS_READ
        {
            etiss_uint64 address = readFromStructUInt(parameter, 1);
            etiss_uint64 count = readFromStructUInt(parameter, 2);

            std::stringstream ss;
            ss << "Semihosting: SYS_READ fd " << fd;
            etiss::log(etiss::INFO, ss.str());

            std::vector<etiss_uint8> buffer;
            buffer.resize(count);
            etiss_uint64 num_read = fread(buffer.data(), 1, count, file);
            buffer.resize(num_read);
            writeSystemMemory(address, buffer);

            return count - num_read;
        }
        case 0x09: // SYS_ISTTY
        {
            std::stringstream ss;
            ss << "Semihosting: SYS_ISTTY fd " << fd << " isntr " << cpu_->instructionPointer;
            etiss::log(etiss::INFO, ss.str());

            return isatty(fileno(file));
        }
        case 0x0c: // SYS_FLEN
        {
            std::stringstream ss;
            ss << "Semihosting: SYS_FLEN fd " << fd << " isntr " << cpu_->instructionPointer;
            etiss::log(etiss::INFO, ss.str());

            int currentPos = ftell(file);
            fseek(file, 0, SEEK_END);
            int length = ftell(file);
            fseek(file, 0, currentPos);
            return length;
        }
        }
    }
    case 0x01: // SYS_OPEN
    {
        etiss_uint64 path_str_addr = readFromStructUInt(parameter, 0);
        etiss_uint64 mode = readFromStructUInt(parameter, 1);
        etiss_uint64 path_str_len = readFromStructUInt(parameter, 2);
        if (mode > 11)
            // invalid mode
            return -1;

        std::vector<etiss_uint8> buffer = readSystemMemory(path_str_addr, path_str_len);
        std::string path_str(buffer.begin(), buffer.end());

        {
            std::stringstream ss;
            ss << "Semihosting: SYS_OPEN \"" << path_str << "\" with mode " << mode_strs[mode];
            etiss::log(etiss::INFO, ss.str());
        }

        FILE *file = NULL;
        // special file path for opening stdin, stdout and stderr
        if (path_str == ":tt")
        {
            if (mode <= 3)
                file = stdin;
            else if (mode >= 4 && mode <= 7)
                file = stdout;
            else // if (mode >= 8 && mode <= 11)
                file = stderr;
        }
        else
        {
            file = fopen(path_str.c_str(), mode_strs[mode]);
            if (file == NULL)
            {
                semihosting_errno = errno;
                return -1;
            }
        }
        etiss_uint64 fd = nextFd++;
        openFiles[fd] = file;

        {
            std::stringstream ss;
            ss << "Semihosting: got fd " << fd;
            etiss::log(etiss::INFO, ss.str());
        }

        return fd;
    }
    case 0x13: // SYS_ERRNO
    {
        etiss::log(etiss::INFO, "Semihosting: SYS_ERRNO");
        return errno;
    }
    case 0x18: // SYS_EXIT
    {
        etiss::log(etiss::WARNING, "Semihosting: SYS_EXIT -> exit simulator");
        etiss::shutdown();
        exit(0);
        return 0;
    }
    default:
        std::stringstream msg;
        msg << "Semihosting: unknown operation number: " << operationNumber;
        etiss::log(etiss::WARNING, msg.str());
        return 0;
    }
}

int Semihosting::maybeSemihostingCall()
{
    etiss_int64 pc = cpu_->instructionPointer;

    etiss_uint32 buf[3];
    system_->dbg_read(system_->handle, pc - 4, (etiss_uint8 *)&buf, 12);

    if (buf[0] == 0x01f01013 && buf[1] == 0x00100073 && buf[2] == 0x40705013)
    {
        auto operationNumberRegister = plugin_core_->getStruct()->findName("R10");
        auto parameterRegister = plugin_core_->getStruct()->findName("R11");

        if (operationNumberRegister && parameterRegister)
        {
            etiss_int64 retval =
                semihostingCall(operationNumberRegister.get()->read(), parameterRegister.get()->read());

            operationNumberRegister.get()->write(retval);
        }
        else
        {
            etiss::log(etiss::ERROR, "Semihosting could not read registers");
        }
        return 1;
    }

    return 0;
}

extern "C"
{
    int semihosting_maybe_execute(void *plugin)
    {
        return ((etiss::plugin::Semihosting *)plugin)->maybeSemihostingCall();
    }
}
