# CMakeList.txt : CMake project for multithreding-low-latency, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("multithreding-low-latency")

# Add source to this project's executable.
add_executable (multithreding-low-latency "multithreding-low-latency.cpp" "multithreding-low-latency.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET multithreding-low-latency PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
