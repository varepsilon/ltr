#include "filesystem.h"

bool IsExist(const char* path) {
#if defined(_WIN32)
    return ::GetFileAttributesA(path) != 0xFFFFFFFF;
#else
    return access(path, F_OK) == 0;
#endif
}

