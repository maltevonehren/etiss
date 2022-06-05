#include "etiss/ETISS.h"

#include "SemihostingCalls.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>

etiss_int64 semihostingCall(ETISS_CPU *const cpu, ETISS_System *const etissSystem, etiss_uint32 XLEN,
                            etiss_uint64 operationNumber, etiss_uint64 parameter);

extern "C"
{
    int64_t etiss_semihost(ETISS_CPU *const cpu, ETISS_System *const etissSystem, void *const *const _, uint32_t XLEN,
                           uint64_t operation, uint64_t parameter)
    {
        return semihostingCall(cpu, etissSystem, XLEN, operation, parameter);
    }
}

etiss_uint64 semihostReadStructField(ETISS_System *etissSystem, etiss_uint32 XLEN, etiss_uint64 address, int fieldNo)
{
    int width = XLEN / 8;
    etiss_uint64 field;
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

const int mode_flags[] = {
    O_RDONLY,                      // "r",
    O_RDONLY,                      // "rb"
    O_RDWR,                        // "r+"
    O_RDWR,                        // "r+b"
    O_CREAT | O_TRUNC | O_WRONLY,  // "w"
    O_CREAT | O_TRUNC | O_WRONLY,  // "wb"
    O_CREAT | O_TRUNC | O_RDWR,    // "w+"
    O_CREAT | O_TRUNC | O_RDWR,    // "w+b"
    O_CREAT | O_APPEND | O_WRONLY, // "a"
    O_CREAT | O_APPEND | O_WRONLY, // "ab"
    O_CREAT | O_APPEND | O_RDWR,   // "a+"
    O_CREAT | O_APPEND | O_RDWR    // "a+b"
};

etiss_int64 semihostingCall(ETISS_CPU *const cpu, ETISS_System *const etissSystem, etiss_uint32 XLEN,
                            etiss_uint64 operationNumber, etiss_uint64 parameter)
{

    static std::map<etiss_uint64, etiss_uint64> openFiles;
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
        etiss_uint64 file = openFiles[fd];

        switch (operationNumber)
        {
        case SYS_CLOSE:
        {
            std::stringstream ss;
            ss << "Semihosting: SYS_CLOSE fd " << fd;
            etiss::log(etiss::VERBOSE, ss.str());

            openFiles.erase(fd);
            if (fd > 2)
            {
                // do not close stdin, stdout, stderr of host process
                if (close(file) < 0)
                {
                    semihosting_errno = errno;
                    return -1;
                }
            }
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
            etiss_int64 num_written = write(file, buffer.data(), count);

            return (etiss_int64)count - num_written;
        }
        case SYS_READ:
        {
            etiss_uint64 address = semihostReadStructField(etissSystem, XLEN, parameter, 1);
            etiss_uint64 count = semihostReadStructField(etissSystem, XLEN, parameter, 2);

            std::stringstream ss;
            ss << "Semihosting: SYS_READ fd " << fd;
            etiss::log(etiss::VERBOSE, ss.str());

            std::vector<etiss_uint8> buffer;
            buffer.resize(count);
            etiss_int64 num_read = read(file, buffer.data(), count);
            buffer.resize(num_read);
            semihostWriteSystemMemory(etissSystem, address, buffer);

            return (etiss_int64)count - num_read;
        }
        case SYS_ISTTY:
        {
            std::stringstream ss;
            ss << "Semihosting: SYS_ISTTY fd " << fd;
            etiss::log(etiss::VERBOSE, ss.str());

            return isatty(file);
        }
        case SYS_SEEK:
        {
            etiss_uint64 position = semihostReadStructField(etissSystem, XLEN, parameter, 1);

            std::stringstream ss;
            ss << "Semihosting: SYS_SEEK fd " << fd << ": " << position;
            etiss::log(etiss::VERBOSE, ss.str());

            int retval = lseek(file, position, SEEK_SET);
            if (retval < 0)
            {
                semihosting_errno = errno;
                return -1;
            }
            return 0;
        }
        case SYS_FLEN:
        {
            std::stringstream ss;
            ss << "Semihosting: SYS_FLEN fd " << fd;
            etiss::log(etiss::VERBOSE, ss.str());

            int currentPos = lseek(file, 0, SEEK_CUR);
            if (currentPos < 0)
            {
                semihosting_errno = ESPIPE;
                return -1;
            }
            lseek(file, 0, SEEK_END);
            int length = lseek(file, 0, SEEK_CUR);
            lseek(file, currentPos, SEEK_SET);
            return length;
        }
        }
    }
    case SYS_OPEN:
    {
        etiss_uint64 path_str_addr = semihostReadStructField(etissSystem, XLEN, parameter, 0);
        etiss_uint64 mode = semihostReadStructField(etissSystem, XLEN, parameter, 1);
        etiss_uint64 path_str_len = semihostReadStructField(etissSystem, XLEN, parameter, 2);
        if (mode > 11)
        {
            // invalid mode
            semihosting_errno = EINVAL;
            return -1;
        }

        std::vector<etiss_uint8> buffer = semihostReadSystemMemory(etissSystem, path_str_addr, path_str_len);
        std::string path_str(buffer.begin(), buffer.end());

        std::stringstream ss1;
        ss1 << "Semihosting: SYS_OPEN \"" << path_str << "\"";
        etiss::log(etiss::VERBOSE, ss1.str());

        etiss_uint64 file = -1;
        // special file path for opening stdin, stdout and stderr
        if (path_str == ":tt")
        {
            if (mode <= 3)
                file = fileno(stdin);
            else if (mode >= 4 && mode <= 7)
                file = fileno(stdout);
            else // if (mode >= 8 && mode <= 11)
                file = fileno(stderr);
        }
        else
        {
            file = open(path_str.c_str(), mode_flags[mode]);
            if (file < 0)
            {
                semihosting_errno = errno;
                return -1;
            }
        }
        etiss_uint64 fd = nextFd++;
        openFiles[fd] = file;

        std::stringstream ss2;
        ss2 << "Semihosting: got fd " << fd;
        etiss::log(etiss::VERBOSE, ss2.str());

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

        std::vector<etiss_uint8> buffer = semihostReadSystemMemory(etissSystem, path_str_addr, path_str_len);
        std::string path_str(buffer.begin(), buffer.end());

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
    default:
        std::stringstream msg;
        msg << "Semihosting: unknown operation number: " << operationNumber;
        etiss::log(etiss::WARNING, msg.str());
        return 0;
    }
}
