import platform
import glob
import os

DefaultEnvironment(tools=[])
env = Environment(ENV = os.environ, tools=['g++', 'link'])
env['SYSTEM'] = platform.system().lower()


#to modify for user
COMPILER_PATH = '"C:/Program Files (x86)/CodeBlocks/mingw64/bin"'
COMPILER = "g++"
HEADER_PATH = "C:/Boost/include/boost-1_67"
LIB_PATH = "C:/Boost/lib"
BOOST_SYSTEM = "boost_system-mgw71-mt-x64-1_67"
#end

BUILD_DIRECTORY = "build/debug/main"
release = ARGUMENTS.get('release', 0)
if int(release):
    BUILD_DIRECTORY = "build/release/main"
else:
    BUILD_DIRECTORY = "build/debug/main"

GLOBAL_FLAGS = [
    "-I" + HEADER_PATH,
    "-L" + LIB_PATH,
    "-std=c++11",
    "-lstdc++"
]
LINUX_FLAGS = [
    "-g"
]
WINDOWS_FLAGS = [
    "-Wno-deprecated",
    "-l" + BOOST_SYSTEM,
    "-lwsock32",
    "-lws2_32"
]
_LIBFLAGS = [
    "-l" + BOOST_SYSTEM,
    "-lwsock32",
    "-lws2_32"
]

env['CCFLAGS'] = []

if env['SYSTEM'] in ['linux', 'darwin']:
    env.PrependUnique( CPPFLAGS=LINUX_FLAGS )
    print("CXXFLAGS =", env['CXXFLAGS'])
    print("linux detected")

if env['SYSTEM'] == 'windows':
    env.Replace( CXX=COMPILER_PATH[:-1] + "/" + COMPILER + '"' )
    env.Replace( _LIBFLAGS=_LIBFLAGS )
    env.PrependUnique( CXXFLAGS=WINDOWS_FLAGS )
    print("CXXFLAGS =", env['CXXFLAGS'])
    print("windows detected")

env.PrependUnique(CXXFLAGS=GLOBAL_FLAGS)

SOURCE_FILES = []

for filename in glob.iglob('src/**/*.cpp', recursive=True):
    new_filename = "obj/"+filename[4:-4]+".o"
    SOURCE_FILES.append(new_filename)
    env.Object(new_filename, filename, CXX=env['CXX'], CXXFLAGS=env['CXXFLAGS'], _LIBFLAGS=env['_LIBFLAGS'])


env.Program(BUILD_DIRECTORY, SOURCE_FILES, CXX=env['CXX'], CXXFLAGS=env['CXXFLAGS'], _LIBFLAGS=env['_LIBFLAGS'])

#working compilation on windows 10 with boost 1_67 complete installation on 64bit architecture
#g++ -IC:\Boost\include\boost-1_67 -LC:/Boost/lib -lstdc++ -std=c++11 main.cpp -Wno-deprecated -lboost_system-mgw71-mt-x64-1_67 -lwsock32