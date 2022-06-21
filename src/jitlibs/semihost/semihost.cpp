#include "etiss/ETISS.h"

#include "SemihostingCalls.h"

#include <cstdio>

// constant for SYS_ELAPSED and SYS_TICKFREQ
#define TICKER_FREQ 1000 // Hz

// constants for SYS_OPEN
const char *SYS_OPEN_MODES_STRS[] = { "r", "rb", "r+", "r+b", "w", "wb", "w+", "w+b", "a", "ab", "a+", "a+b" };
#define SYS_OPEN_MODES_TOTAL 12 // total number of modes
#define SYS_OPEN_MODES_IN_LIMIT 4
#define SYS_OPEN_MODES_OUT_LIMIT 8

#define PS_PER_CS 10000000000 // 10 * 10^9

// if var is negative set semihosting errno and return -1
#define CHECK_NEGATIVE_RETURN(var) \
    if ((var) < 0)                 \
    {                              \
        semihosting_errno = errno; \
        return -1;                 \
    }

etiss_int64 semihostingCall(ETISS_CPU *const cpu, ETISS_System *const etissSystem, etiss_uint32 XLEN,
                            etiss_uint64 operationNumber, etiss_uint64 parameter);

extern "C"
{
    uint8_t etiss_semihost_enabled()
    {
        return etiss::cfg().get<bool>("arch.enable_semihosting", false);
    }

    int64_t etiss_semihost(ETISS_CPU *const cpu, ETISS_System *const etissSystem, void *const *const _, uint32_t XLEN,
                           uint64_t operation, uint64_t parameter)
    {
        return semihostingCall(cpu, etissSystem, XLEN, operation, parameter);
    }
}

etiss_uint64 semihostReadStructField(ETISS_System *etissSystem, etiss_uint32 XLEN, etiss_uint64 address, int fieldNo)
{
    int width = XLEN / 8;
    etiss_uint64 field = 0;
    etissSystem->dbg_read(etissSystem->handle, address + width * fieldNo, (etiss_uint8 *)&field,
                          width); // TODO throw error
    return field;
}

std::vector<etiss_uint8> semihostReadSystemMemory(ETISS_System *etissSystem, etiss_uint64 address, etiss_uint64 length)
{
    std::vector<etiss_uint8> buffer;
    buffer.resize(length);
    etissSystem->dbg_read(etissSystem->handle, address, buffer.data(), length);
    return buffer;
}

void semihostWriteSystemMemory(ETISS_System *etissSystem, etiss_uint64 address, std::vector<etiss_uint8> data)
{
    etissSystem->dbg_write(etissSystem->handle, address, data.data(), data.size());
}

std::string semihostReadString(ETISS_System *etissSystem, etiss_uint64 address, etiss_uint64 length)
{
    std::vector<etiss_uint8> buffer = semihostReadSystemMemory(etissSystem, address, length);
    std::string str(buffer.begin(), buffer.end());
    return str;
}

bool is_std_in_out_err(FILE *file)
{
    return file == stdin || file == stdout || file == stderr;
}

etiss_int64 semihostingCall(ETISS_CPU *const cpu, ETISS_System *const etissSystem, etiss_uint32 XLEN,
                            etiss_uint64 operationNumber, etiss_uint64 parameter)
{

    static std::map<etiss_uint64, FILE *> openFiles;
    static etiss_uint64 nextFd;
    static etiss_int64 semihosting_errno;

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
        etiss_uint64 fd = semihostReadStructField(etissSystem, XLEN, parameter, 0);
        if (openFiles.count(fd) == 0)
        {
            std::stringstream ss;
            ss << "Semihosting: invalid file descriptor " << fd << " for semihosting call 0x" << std::hex
               << std::setfill('0') << std::setw(2) << operationNumber;
            etiss::log(etiss::VERBOSE, ss.str());
            semihosting_errno = EBADF;
            return -1;
        }
        auto file = openFiles[fd];

        switch (operationNumber)
        {
        case SYS_CLOSE:
        {
            std::stringstream ss;
            ss << "Semihosting: SYS_CLOSE fd " << fd;
            etiss::log(etiss::VERBOSE, ss.str());

            openFiles.erase(fd);
            // do not close stdin, stdout, stderr of host process
            if (!is_std_in_out_err(file))
                fclose(file);
            return 0;
        }
        case SYS_WRITE:
        {
            etiss_uint64 address = semihostReadStructField(etissSystem, XLEN, parameter, 1);
            etiss_uint64 count = semihostReadStructField(etissSystem, XLEN, parameter, 2);

            std::stringstream ss;
            ss << "Semihosting: SYS_WRITE fd " << fd;
            etiss::log(etiss::VERBOSE, ss.str());

            std::vector<etiss_uint8> buffer = semihostReadSystemMemory(etissSystem, address, count);

            size_t num_written = fwrite(buffer.data(), 1, count, file);
            return count - num_written;
        }
        case SYS_READ:
        {
            etiss_uint64 address = semihostReadStructField(etissSystem, XLEN, parameter, 1);
            etiss_uint64 count = semihostReadStructField(etissSystem, XLEN, parameter, 2);

            std::stringstream ss;
            ss << "Semihosting: SYS_READ fd " << fd << " count " << count;
            etiss::log(etiss::VERBOSE, ss.str());

            std::vector<etiss_uint8> buffer;
            buffer.resize(count);

            size_t num_read = 0;
            if (file == stdin)
            {
                // when reading from stdin: mimic behaviour from read syscall
                // and return on newline.
                while (num_read < count)
                {
                    char c = fgetc(file);
                    buffer[num_read] = c;
                    num_read++;
                    if (c == '\n')
                        break;
                }
            }
            else
            {
                num_read = fread(buffer.data(), 1, count, file);
            }

            buffer.resize(num_read);
            semihostWriteSystemMemory(etissSystem, address, buffer);

            return (etiss_int64)count - num_read;
        }
        case SYS_ISTTY:
        {
            std::stringstream ss;
            ss << "Semihosting: SYS_ISTTY fd " << fd;
            etiss::log(etiss::VERBOSE, ss.str());

            return is_std_in_out_err(file);
        }
        case SYS_SEEK:
        {
            etiss_uint64 position = semihostReadStructField(etissSystem, XLEN, parameter, 1);

            std::stringstream ss;
            ss << "Semihosting: SYS_SEEK fd " << fd << ": " << position;
            etiss::log(etiss::VERBOSE, ss.str());

            int retval = fseek(file, position, SEEK_SET);
            CHECK_NEGATIVE_RETURN(retval);
            return 0;
        }
        case SYS_FLEN:
        {
            std::stringstream ss;
            ss << "Semihosting: SYS_FLEN fd " << fd;
            etiss::log(etiss::VERBOSE, ss.str());

            size_t currentPos = ftell(file);
            CHECK_NEGATIVE_RETURN(currentPos);
            fseek(file, 0, SEEK_END);
            size_t length = ftell(file);
            fseek(file, currentPos, SEEK_SET);
            return (etiss_int64)length;
        }
        }
    }
    case SYS_OPEN:
    {
        etiss_uint64 path_str_addr = semihostReadStructField(etissSystem, XLEN, parameter, 0);
        etiss_uint64 mode = semihostReadStructField(etissSystem, XLEN, parameter, 1);
        etiss_uint64 path_str_len = semihostReadStructField(etissSystem, XLEN, parameter, 2);

        std::string path_str = semihostReadString(etissSystem, path_str_addr, path_str_len);

        std::stringstream ss;
        ss << "Semihosting: SYS_OPEN \"" << path_str << "\"";
        etiss::log(etiss::VERBOSE, ss.str());

        if (mode >= SYS_OPEN_MODES_TOTAL)
        {
            // invalid mode
            semihosting_errno = EINVAL;
            return -1;
        }

        FILE *file = nullptr;
        if (path_str == ":tt")
        {
            // special file path for opening stdin, stdout and stderr
            // open stdin, stdout or stderr depending on mode argument
            if (mode < SYS_OPEN_MODES_IN_LIMIT) // 0 <= mode <= 3
                file = stdin;
            else if (mode < SYS_OPEN_MODES_OUT_LIMIT) // 4 <= mode <= 7
                file = stdout;
            else // 8 <= mode <= 11
                file = stderr;
        }
        else
        {
            file = fopen(path_str.c_str(), SYS_OPEN_MODES_STRS[mode]);
            if (file == nullptr)
            {
                semihosting_errno = errno;
                return -1;
            }
        }
        etiss_uint64 fd = nextFd++;
        openFiles[fd] = file;

        return (etiss_int64)fd;
    }
    case SYS_WRITEC:
    {
        etiss_uint64 character = semihostReadStructField(etissSystem, XLEN, parameter, 0);
        putchar(character);
        return 0;
    }
    case SYS_READC:
    {
        return getchar();
    }
    case SYS_REMOVE:
    {
        etiss_uint64 path_str_addr = semihostReadStructField(etissSystem, XLEN, parameter, 0);
        etiss_uint64 path_str_len = semihostReadStructField(etissSystem, XLEN, parameter, 1);

        std::string path_str = semihostReadString(etissSystem, path_str_addr, path_str_len);

        std::stringstream ss;
        ss << "Semihosting: SYS_REMOVE \"" << path_str << "\"";
        etiss::log(etiss::VERBOSE, ss.str());

        if (remove(path_str.c_str()) < 0)
        {
            semihosting_errno = errno;
            return -1;
        }
        return 0;
    }
    case SYS_CLOCK:
    {
        // return centiseconds since some arbitrary start point
        return cpu->cpuTime_ps / PS_PER_CS;
    }
    case SYS_TIME:
    {
        etiss::log(etiss::VERBOSE, "Semihosting: SYS_TIME");
        etiss_int64 seconds_since_epoch = (etiss_int64)std::time(0);
        return seconds_since_epoch;
    }
    case SYS_ERRNO:
    {
        std::stringstream ss;
        ss << "Semihosting: SYS_ERRNO (" << semihosting_errno << ")";
        etiss::log(etiss::VERBOSE, ss.str());
        return semihosting_errno;
    }
    case SYS_EXIT:
    {
        etiss::log(etiss::VERBOSE, "Semihosting: SYS_EXIT -> exit simulator");
        // TODO
        etiss::shutdown();
        exit(0);
    }
    case SYS_ELAPSED:
    {
        return cpu->cpuTime_ps / TICKER_FREQ;
    }
    case SYS_TICKFREQ:
    {
        return TICKER_FREQ;
    }
    case SYS_WRITE0:
    case SYS_ISERROR:
    case SYS_TMPNAM:
    case SYS_RENAME:
    case SYS_SYSTEM:
    case SYS_GET_CMDLINE:
    case SYS_HEAPINFO:
    case SYS_EXIT_EXTENDED:
    {
        std::stringstream ss;
        ss << "Semihosting: operation not implemented: " << operationNumber;
        etiss::log(etiss::WARNING, ss.str());
        return 0;
    }
    default:
    {
        std::stringstream ss;
        ss << "Semihosting: unknown operation number: " << operationNumber;
        etiss::log(etiss::WARNING, ss.str());
        return 0;
    }
    }
}
