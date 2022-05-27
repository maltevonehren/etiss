

#include "etiss/IntegratedLibrary/Semihosting.h"
#include "etiss/IntegratedLibrary/SemihostingCalls.h"
#include "etiss/CPUCore.h"
#include <unistd.h>
#include <errno.h>

using namespace etiss::plugin;

extern "C"
{
    etiss::plugin::Semihosting *etiss_semihosting_plugin;

    struct semihosting_return etiss_semihosting(etiss_uint32 XLEN, etiss_uint64 operation, etiss_uint64 parameter)
    {
        if (etiss_semihosting_plugin == nullptr)
        {
            etiss::log(etiss::WARNING, "semihosting called but semihosting plugin not initialized");
            return { 0, 0 };
        }
        return etiss_semihosting_plugin->semihostingCall(XLEN, operation, parameter);
    }
}

void Semihosting::finalizeCodeBlock(etiss::CodeBlock &block) const
{
    std::stringstream ss;
    ss << "extern struct semihosting_return "
          "etiss_semihosting(etiss_uint32 XLEN, etiss_uint64 operation, etiss_uint64 parameter);\n";
    ss << "struct semihosting_return { etiss_int64 should_exit; etiss_int64 value; };\n";

    block.fileglobalCode().insert(ss.str());
}

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
    etiss_semihosting_plugin = this;
}

void Semihosting::cleanup()
{
    arch_ = nullptr;
    cpu_ = nullptr;
    system_ = nullptr;
    etiss_semihosting_plugin = nullptr;
}

void *Semihosting::getPluginHandle()
{
    return (void *)this;
}

etiss_uint64 Semihosting::readStructField(etiss_uint32 XLEN, etiss_uint64 address, int fieldNo)
{
    int width = XLEN / 8;
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

semihosting_return Semihosting::semihostingCall(etiss_uint32 XLEN, etiss_uint64 operationNumber, etiss_uint64 parameter)
{
    switch (operationNumber)
    {
    // all operations that have the file descriptor as their first argument
    case SYS_CLOSE:
    case SYS_WRITE:
    case SYS_READ:
    case SYS_ISTTY:
    case SYS_SEEK:
    case SYS_FLEN:
    {
        etiss_uint64 fd = readStructField(XLEN, parameter, 0);
        if (openFiles.count(fd) == 0)
        {
            std::stringstream ss;
            ss << "Semihosting: invalid file descriptor " << fd << " for semihosting call 0x" << std::hex
               << std::setfill('0') << std::setw(2) << operationNumber;
            etiss::log(etiss::INFO, ss.str());
            semihosting_errno = EBADF;
            return { 0, -1 };
        }
        FILE *file = openFiles[fd];

        switch (operationNumber)
        {
        case SYS_CLOSE:
        {
            std::stringstream ss;
            ss << "Semihosting: SYS_CLOSE fd " << fd;
            etiss::log(etiss::INFO, ss.str());

            openFiles.erase(fd);
            if (fd > 2)
            {
                // do not close stdin, stdout, stderr of host process
                if (fclose(file) < 0)
                {
                    semihosting_errno = errno;
                    return { 0, -1 };
                }
            }
            return { 0, 0 };
        }
        case SYS_WRITE:
        {
            etiss_uint64 address = readStructField(XLEN, parameter, 1);
            etiss_uint64 count = readStructField(XLEN, parameter, 2);

            std::stringstream ss;
            ss << "Semihosting: SYS_WRITE fd " << fd;
            etiss::log(etiss::INFO, ss.str());

            std::vector<etiss_uint8> buffer = readSystemMemory(address, count);
            etiss_int64 num_written = fwrite(buffer.data(), 1, count, file);

            return { 0, (etiss_int64)count - num_written };
        }
        case SYS_READ:
        {
            etiss_uint64 address = readStructField(XLEN, parameter, 1);
            etiss_uint64 count = readStructField(XLEN, parameter, 2);

            std::stringstream ss;
            ss << "Semihosting: SYS_READ fd " << fd;
            etiss::log(etiss::INFO, ss.str());

            std::vector<etiss_uint8> buffer;
            buffer.resize(count);
            etiss_int64 num_read = fread(buffer.data(), 1, count, file);
            buffer.resize(num_read);
            writeSystemMemory(address, buffer);

            return { 0, (etiss_int64)count - num_read };
        }
        case SYS_ISTTY:
        {
            std::stringstream ss;
            ss << "Semihosting: SYS_ISTTY fd " << fd;
            etiss::log(etiss::INFO, ss.str());

            return { 0, isatty(fileno(file)) };
        }
        case SYS_SEEK:
        {
            etiss_uint64 position = readStructField(XLEN, parameter, 1);

            std::stringstream ss;
            ss << "Semihosting: SYS_SEEK fd " << fd << ": " << position;
            etiss::log(etiss::INFO, ss.str());

            int retval = fseek(file, position, SEEK_SET);
            if (retval < 0)
            {
                semihosting_errno = errno;
                return { 0, -1 };
            }
            return { 0, 0 };
        }
        case SYS_FLEN:
        {
            std::stringstream ss;
            ss << "Semihosting: SYS_FLEN fd " << fd;
            etiss::log(etiss::INFO, ss.str());

            int currentPos = ftell(file);
            if (fseek(file, 0, SEEK_END) < 0)
            {
                semihosting_errno = ESPIPE;
                return { 0, -1 };
            }
            int length = ftell(file);
            fseek(file, currentPos, SEEK_SET);
            return { 0, length };
        }
        }
    }
    case SYS_OPEN:
    {
        etiss_uint64 path_str_addr = readStructField(XLEN, parameter, 0);
        etiss_uint64 mode = readStructField(XLEN, parameter, 1);
        etiss_uint64 path_str_len = readStructField(XLEN, parameter, 2);
        if (mode > 11)
        {
            // invalid mode
            semihosting_errno = EINVAL;
            return { 0, -1 };
        }

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
                return { 0, -1 };
            }
        }
        etiss_uint64 fd = nextFd++;
        openFiles[fd] = file;

        {
            std::stringstream ss;
            ss << "Semihosting: got fd " << fd;
            etiss::log(etiss::INFO, ss.str());
        }

        return { 0, (etiss_int64)fd };
    }
    case SYS_REMOVE:
    {
        etiss_uint64 path_str_addr = readStructField(XLEN, parameter, 0);
        etiss_uint64 path_str_len = readStructField(XLEN, parameter, 1);

        std::vector<etiss_uint8> buffer = readSystemMemory(path_str_addr, path_str_len);
        std::string path_str(buffer.begin(), buffer.end());

        std::stringstream ss;
        ss << "Semihosting: SYS_REMOVE \"" << path_str << "\"";
        etiss::log(etiss::INFO, ss.str());

        if (remove(path_str.c_str()) < 0)
        {
            semihosting_errno = errno;
            return { 0, -1 };
        }
        return { 0, 0 };
    }
    case SYS_TIME:
    {
        etiss::log(etiss::INFO, "Semihosting: SYS_TIME");
        etiss_int64 seconds_since_epoch = (etiss_int64)std::time(0);
        return { 0, seconds_since_epoch };
    }
    case SYS_ERRNO:
    {
        std::stringstream ss;
        ss << "Semihosting: SYS_ERRNO (" << semihosting_errno << ")";
        etiss::log(etiss::INFO, ss.str());
        return { 0, semihosting_errno };
    }
    case SYS_EXIT:
    {
        etiss::log(etiss::INFO, "Semihosting: SYS_EXIT -> exit simulator");
        return { 1, 0 };
    }
    default:
        std::stringstream msg;
        msg << "Semihosting: unknown operation number: " << operationNumber;
        etiss::log(etiss::WARNING, msg.str());
        return { 0, 0 };
    }
}
