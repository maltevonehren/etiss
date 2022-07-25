/**
 * Generated on Mon, 25 Jul 2022 12:49:45 +0200.
 *
 * This file contains the instruction behavior models of the tum_semihosting
 * instruction set for the RV32IMACFD core architecture.
 */

#include "RV32IMACFDArch.h"

#define ETISS_ARCH_STATIC_FN_ONLY
#include "RV32IMACFDFuncs.h"

using namespace etiss;
using namespace etiss::instr;


// EBREAK ----------------------------------------------------------------------
static InstructionDefinition ebreak_ (
	ISA32_RV32IMACFD,
	"ebreak",
	(uint32_t) 0x100073,
	(uint32_t) 0xffffffff,
	[] (BitArray & ba,etiss::CodeSet & cs,InstructionContext & ic)
	{

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

		CodePart & partInit = cs.append(CodePart::INITIALREQUIRED);

		partInit.code() = std::string("//EBREAK\n");

// -----------------------------------------------------------------------------
partInit.code() += "cpu->instructionPointer = " + std::to_string(ic.current_address_ + 4U) + ";\n";
partInit.code() += "if (etiss_semihost_enabled()) {\n";
partInit.code() += "etiss_uint32 mem_val_0;\n";
partInit.code() += "cpu->exception |= (*(system->dread))(system->handle, cpu, " + std::to_string(ic.current_address_ - 4U) + ", (etiss_uint8*)&mem_val_0, 4);\n";
partInit.code() += "if (cpu->exception) translate_exc_code(cpu, system, plugin_pointers, cpu->exception);\n";
partInit.code() += "etiss_uint32 pre = (etiss_uint32)(mem_val_0);\n";
partInit.code() += "etiss_uint32 mem_val_1;\n";
partInit.code() += "cpu->exception |= (*(system->dread))(system->handle, cpu, " + std::to_string(ic.current_address_ + 0U) + ", (etiss_uint8*)&mem_val_1, 4);\n";
partInit.code() += "if (cpu->exception) translate_exc_code(cpu, system, plugin_pointers, cpu->exception);\n";
partInit.code() += "etiss_uint32 ebreak = (etiss_uint32)(mem_val_1);\n";
partInit.code() += "etiss_uint32 mem_val_2;\n";
partInit.code() += "cpu->exception |= (*(system->dread))(system->handle, cpu, " + std::to_string(ic.current_address_ + 4U) + ", (etiss_uint8*)&mem_val_2, 4);\n";
partInit.code() += "if (cpu->exception) translate_exc_code(cpu, system, plugin_pointers, cpu->exception);\n";
partInit.code() += "etiss_uint32 post = (etiss_uint32)(mem_val_2);\n";
partInit.code() += "if (pre == 32509971U && ebreak == 1048691U && post == 1081102355U) {\n";
partInit.code() += "etiss_uint32 operation = *((RV32IMACFD*)cpu)->X[10U];\n";
partInit.code() += "etiss_uint32 parameter = *((RV32IMACFD*)cpu)->X[11U];\n";
partInit.code() += "*((RV32IMACFD*)cpu)->X[10U] = (etiss_int32)(etiss_semihost(cpu, system, plugin_pointers, " + std::to_string(32) + ", operation, parameter));\n";
partInit.code() += "}\n";
partInit.code() += " else {\n";
partInit.code() += "cpu->exception = raise(cpu, system, plugin_pointers, 0U, " + std::to_string(3) + ");\n";
partInit.code() += "}\n";
partInit.code() += "}\n";
partInit.code() += " else {\n";
partInit.code() += "cpu->exception = raise(cpu, system, plugin_pointers, 0U, " + std::to_string(3) + ");\n";
partInit.code() += "}\n";
partInit.code() += "if (cpu->return_pending | cpu->instructionPointer != " + std::to_string(ic.current_address_ + 4) + ") return cpu->exception;\n";
// -----------------------------------------------------------------------------

		partInit.getRegisterDependencies().add(reg_name[10U], 32);
		partInit.getRegisterDependencies().add(reg_name[11U], 32);
		partInit.getAffectedRegisters().add(reg_name[10U], 32);
		partInit.getAffectedRegisters().add("instructionPointer", 32);

		return true;
	},
	0,
	[] (BitArray & ba, Instruction & instr)
	{
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------

		std::stringstream ss;
// -----------------------------------------------------------------------------
ss << "ebreak" << " # " << ba << (" []");
// -----------------------------------------------------------------------------
		return ss.str();
	}
);
