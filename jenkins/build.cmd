call "%PROGRAMFILES%\Microsoft Visual Studio 9.0\vcvarsall.bat"

@set PATH=%PATH%;%PROGRAMFILES%\Microsoft SDKs\Windows\v6.1\Bin
@set PATH=%PATH%;%PROGRAMFILES%\Microsoft Visual Studio 9.0\VC\Bin
@set PATH=%PATH%;%PROGRAMFILES%\Microsoft Visual Studio 9.0\Common7\IDE
@set INCLUDE=%PROGRAMFILES%\Microsoft SDKs\Windows\v6.1\Include;%INCLUDE%
@set INCLUDE=%PROGRAMFILES%\Microsoft Visual Studio 9.0\VC\Include;%INCLUDE%
@set LIB=%PROGRAMFILES%\Microsoft SDKs\Windows\v6.1\Lib;%LIB%
@set LIB=%PROGRAMFILES%\Microsoft Visual Studio 9.0\VC\lib;%LIB%

cmake ../ -DCMAKE_BUILD_TYPE=Release
nmake
