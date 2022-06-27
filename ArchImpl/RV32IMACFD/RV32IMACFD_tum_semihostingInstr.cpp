/**
 * Generated on Sun, 05 Jun 2022 16:07:34 +0200.
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
partInit.code() += "exception |= (*(system->dread))(system->handle, cpu, " + std::to_string(ic.current_address_ - 4U) + ", (etiss_uint8*)&mem_val_0, 4);\n";
partInit.code() += "etiss_uint32 mem_val_1;\n";
partInit.code() += "exception |= (*(system->dread))(system->handle, cpu, " + std::to_string(ic.current_address_ + 0U) + ", (etiss_uint8*)&mem_val_1, 4);\n";
partInit.code() += "etiss_uint32 mem_val_2;\n";
partInit.code() += "exception |= (*(system->dread))(system->handle, cpu, " + std::to_string(ic.current_address_ + 4U) + ", (etiss_uint8*)&mem_val_2, 4);\n";
partInit.code() += "if ((etiss_uint32)(mem_val_0) == 32509971U && (etiss_uint32)(mem_val_1) == 1048691U && (etiss_uint32)(mem_val_2) == 1081102355U) {\n";
partInit.code() += "etiss_uint32 operation = *((RV32IMACFD*)cpu)->X[" + std::to_string(10U) + "];\n";
partInit.code() += "etiss_uint32 parameter = *((RV32IMACFD*)cpu)->X[" + std::to_string(11U) + "];\n";
partInit.code() += "*((RV32IMACFD*)cpu)->X[" + std::to_string(10U) + "] = (etiss_int32)(etiss_semihost(cpu, system, plugin_pointers, " + std::to_string(32) + ", operation, parameter));\n";
partInit.code() += "}\n";
partInit.code() += " else {\n";
partInit.code() += "exception = ETISS_RETURNCODE_BREAKPOINT;\n";
partInit.code() += "}\n";
partInit.code() += "}\n";
partInit.code() += " else {\n";
partInit.code() += "exception = ETISS_RETURNCODE_BREAKPOINT;\n";
partInit.code() += "}\n";
partInit.code() += "return exception;\n";
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
