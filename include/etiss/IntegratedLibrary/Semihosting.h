#ifndef ETISS_INCLUDE_SEMIHOSTING_H_
#define ETISS_INCLUDE_SEMIHOSTING_H_

#include "etiss/CPUArch.h"

#include "etiss/Plugin.h"

#include <memory>

namespace etiss
{

namespace plugin
{

class Semihosting : public etiss::TranslationPlugin
{
  public:
    // Plugin
    void init(ETISS_CPU *cpu, ETISS_System *system, CPUArch *arch) override;
    void cleanup() override;

    // TranslationPlugin
    void finalizeInstrSet(etiss::instr::ModedInstructionSet &) const override;
    void finalizeCodeBlock(etiss::CodeBlock &) const override;
    void *getPluginHandle() override;

    // Functionality
    int MaybeSemihostingCall();

  protected:
    // Plugin
    std::string _getPluginName() const override;

    void finalizeInstr(etiss::instr::Instruction &) const;
    etiss_uint64 SemihostingCall(etiss_uint64, etiss_uint64);

    CPUArch *arch_;
    ETISS_CPU *cpu_;
    ETISS_System *system_;
};
} // namespace plugin

} // namespace etiss

#endif
