cmake_minimum_required(VERSION 3.15)
project(qProfiler VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(
  qprofiler 
  src/main.c
  src/args.c
  src/print.c
  src/monitor.c
  src/show.c
)

target_compile_options(qprofiler PRIVATE -O3)
target_include_directories(qprofiler PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(MSVC)
  target_compile_options(qprofiler PRIVATE /W4)
else()
  target_compile_options(qprofiler PRIVATE -Wall -Wextra -Wpedantic)
  target_compile_options(qprofiler PRIVATE -Werror)
endif()
