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
    ~Semihosting();
    // Plugin
    void init(ETISS_CPU *cpu, ETISS_System *system, CPUArch *arch) override;
    void cleanup() override;

    // TranslationPlugin
    void finalizeInstrSet(etiss::instr::ModedInstructionSet &) const override;
    void finalizeCodeBlock(etiss::CodeBlock &) const override;
    void *getPluginHandle() override;

    // Functionality
    int maybeSemihostingCall();

  protected:
    // Plugin
    std::string _getPluginName() const override;
    void finalizeInstr(etiss::instr::Instruction &) const;

    etiss_int64 semihostingCall(etiss_uint64 operation, etiss_uint64 parameter);

    /// fieldNo starts at 0
    etiss_uint64 readFromStructUInt(etiss_uint64 address, int fieldNo);

    std::vector<etiss_uint8> readSystemMemory(etiss_uint64 address, etiss_uint64 length);
    void writeSystemMemory(etiss_uint64 address, std::vector<etiss_uint8> data);

    std::map<etiss_uint64, FILE *> openFiles;
    etiss_uint64 nextFd = 0;
    etiss_uint64 semihosting_errno = 0;

    CPUArch *arch_;
    ETISS_CPU *cpu_;
    ETISS_System *system_;
};
} // namespace plugin

} // namespace etiss

#endif
