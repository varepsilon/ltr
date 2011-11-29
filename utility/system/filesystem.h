/*
Copyright (c) 2009-2010, Yandex Data Analysis School
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Yandex Data Analysis School nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef YABRA_UTILITY_FILESYSTEM_FILESYSTEM_H_
#define YABRA_UTILITY_FILESYSTEM_FILESYSTEM_H_

#include <cstdio>
#include <cstdlib>
#include <string>

using std::string;

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <io.h>
#include <windows.h>
# define LOCAL_SLASH_CHAR     '\\'
# define LOCAL_SLASH_STRING   "\\"
#elif defined(__linux__) || defined(__FreeBSD__)
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
# define LOCAL_SLASH_CHAR     '/'
# define LOCAL_SLASH_STRING   "/"
#endif

bool IsExist(const char* path);

/**
  * Return current working directory. Cross-platform version.
  * Returns empty string in case of error.
  */
static inline string GetCurrentWorkingDirectory() {
  char buffer[512];
#if defined(_WIN32)
  int r = GetCurrentDirectory(512, buffer);
  if (r == 0) {
    return string();
  }
  return string(buffer, r);
#else // Linux etc.
  char* r = getcwd(buffer, 512);
  if (r == 0) {
    return string();
  }
  return string(buffer);
#endif
}

#if defined(__linux__) || defined(__FreeBSD__)
static inline string ReadLink(const string& path) {
    for (;;) {
        char buffer[1024];
        ssize_t r = readlink(path.c_str(), buffer, 1024);
        if (r < 0)
            abort();
        if (r < (ssize_t)1024) {
            return string(buffer, r);
        }
    }
}
#endif // defined(__linux__) || defined(__FreeBSD__)

#if defined(__FreeBSD__)
static inline string TryFastInaccurateExecNameImpl(const char* pwd) {
    if (!pwd) {
        return string();
    }

    extern const char* __progname;
    const string s = string(pwd) + "/" + __progname;

    if (IsExist(s.c_str())) {
        return s;
    }

    return string();
}

static inline string TryFastInaccurateExecName1() {
    return TryFastInaccurateExecNameImpl(getenv("PWD"));
}

static inline string TryFastInaccurateExecName2() {
    char buffer[1024];
    return TryFastInaccurateExecNameImpl(getcwd(buffer, 1024));
}

static inline bool GoodPath(const string& path) {
    return path.find('/') != string::npos;
}
#endif // defined(__FreeBSD__)

//static inline string GetCurrentExecutablePath() {
//    char buffer[512];
//#if defined(_WIN32)
//    int r = GetModuleFileName(NULL, buffer, 512);
//    if (r == 0) {
//        return string();
//    }
//    string path = string(buffer, r);
//#elif defined(__linux__)
//    sprintf(buffer, "%d", getpid());
//    string pidString(buffer);
//    string path = ReadLink("/proc/" + pidString + "/exe");
//#elif defined(__FreeBSD__)
//    sprintf(buffer, "%d", getpid());
//    string pidString(buffer);
//    string path = ReadLink("/proc/" + pidString + "/file");
//
//    if (!GoodPath(path)) {
//        path = TryFastInaccurateExecName1();
//    }
//
//    if (!GoodPath(path)) {
//        path = TryFastInaccurateExecName2();
//    }
//#endif
//    size_t pos = path.find_last_of(LOCAL_SLASH_STRING);
//    return path.substr(0, pos);
//}

static inline string ConstructPath(const string& s1, const string& s2) {
    return string(s1 + LOCAL_SLASH_STRING + s2);
}

static inline string ConstructPath(const string& s1, const string& s2, const string& s3) {
    return string(s1 + LOCAL_SLASH_STRING + s2 + LOCAL_SLASH_STRING + s3);
}

static inline string ConstructPath(const string& s1, const string& s2, const string& s3, const std::string & s4) {
    return string(s1 + LOCAL_SLASH_STRING + s2 + LOCAL_SLASH_STRING + s3 + LOCAL_SLASH_STRING + s4);
}

static inline string GetConfigDirectoryPath() {
#if defined(_WIN32)
  string configpath = ConstructPath(getenv("APPDATA"), "yabracadabra");
  return configpath;
#else
  char* xdg_config_home = getenv("XDG_CONFIG_HOME");
  string configpath;
  if (xdg_config_home != NULL) {
    configpath = ConstructPath(xdg_config_home, "yabracadabra");
  } else {
    configpath = ConstructPath(getenv("HOME"), ".config", "yabracadabra");
  }
  return configpath;
#endif // _WIN32
}

static inline string GetVarDirectoryPath() {
#if defined(_WIN32)
  string configpath = ConstructPath(getenv("APPDATA"), "yabracadabra", "var");
  return configpath;
#else
  char* xdg_data_home = getenv("XDG_DATA_HOME");
  string varpath;
  if (xdg_data_home != NULL) {
    varpath = ConstructPath(xdg_data_home, "yabracadabra");
  } else {
    varpath = ConstructPath(getenv("HOME"), ".local", "share" ,"yabracadabra");
  }
  return varpath;
#endif // _WIN32
}

#endif // YABRA_UTILITY_FILESYSTEM_FILESYSTEM_H_
