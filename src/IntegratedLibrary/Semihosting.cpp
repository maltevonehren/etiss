

#include "etiss/IntegratedLibrary/Semihosting.h"
#include "etiss/CPUCore.h"

using namespace etiss::plugin;

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

etiss_uint64 Semihosting::SemihostingCall(etiss_uint64 operationNumber, etiss_uint64 parameter)
{
    std::stringstream msg;
    msg << "Semihosting: Execute call " << std::dec << operationNumber << " with parameter ";
    msg << "0x" << std::hex << std::setfill('0') << std::setw(16) << parameter;
    etiss::log(etiss::INFO, msg.str());

    switch (operationNumber)
    {
    case 5: // write
    {
        etiss_uint64 fd, addr, count;
        system_->dbg_read(system_->handle, parameter, (etiss_uint8 *)&fd, 8);
        system_->dbg_read(system_->handle, parameter + 8, (etiss_uint8 *)&addr, 8);
        system_->dbg_read(system_->handle, parameter + 16, (etiss_uint8 *)&count, 8);

        std::stringstream msg;
        msg << "Semihosting: Write to fd " << std::dec << fd << ":";
        etiss::log(etiss::INFO, msg.str());

        std::vector<etiss_uint8> buffer;
        buffer.resize(count);
        system_->dbg_read(system_->handle, addr, buffer.data(), count);
        std::string str(buffer.begin(), buffer.end());

        std::cout << str;

        // assume there was no error so return 0
        return 0;
    }
    default:
        std::stringstream msg;
        msg << "Semihosting: unknown operation number: " << operationNumber;
        etiss::log(etiss::WARNING, msg.str());
        break;
    }
    return 0;
}

int Semihosting::MaybeSemihostingCall()
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
            etiss_uint64 retval =
                SemihostingCall(operationNumberRegister.get()->read(), parameterRegister.get()->read());

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
        return ((etiss::plugin::Semihosting *)plugin)->MaybeSemihostingCall();
    }
}
