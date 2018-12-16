# ZPRProject
Projekt na (Średnio)Zaawansowane Programowanie w C++:
Serwer z Analizą Danych
- Repozytorium Serwera

Build with:
Python 3.7.1
SCons v3.0.0
Windows 10 x64
Boost v1_67
MinGW-W64 g++ x86_64 7.1.0

Requirements:
- Installed Boost
- Installed MinGW g++

We doesn't know strictly what versions of above will work and which doesnt.

How to Build:
Open SConstruct file and fill accordingly, then save the file:
COMPILER_PATH = <Compiler_path> ; Directory may look like: '"C:/Program Files (x86)/mingw64/bin"'
COMPILER = "g++" ; For other compilers build may not work - use with caution
HEADER_PATH = "C:/Boost/include/boost-1_67" ; This is default Boost directory
LIB_PATH = "C:/Boost/lib" ; This is default Boost directory
BOOST_SYSTEM = <boost_system> ; by default it should be "boost_system", for boost complete build "boost_system-mgw71-mt-x64-1_67" worked

Use Cmd > "**/ZPRProject" > scons to build

If Build doesnt work - please let me know by mail: jakub@dmochowski.co