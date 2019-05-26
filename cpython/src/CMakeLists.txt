project(SymPython)
cmake_minimum_required(VERSION 3.0)

set(STP_DIR "" CACHE PATH "Try to use particular STP install (set this to folder where STPConfig.cmake is installed)")
find_package(STP REQUIRED)

set(SRCS 
  bytecode.c
  vm.c
)

SET(HDRS
  bytecode.h
  opcode.h
  vm.h
)

SET(CMAKE_C_FLAGS "-g")

include_directories(${STP_INCLUDE_DIRS})
add_executable(SymPython main.c ${HDRS} ${SRCS})
target_link_libraries(SymPython stp)

# Tests.
add_executable(test_vm test/test_vm.c ${HDRS} ${SRCS})
target_link_libraries(test_vm stp)
add_executable(test_parse_bytecode test/test_parse_bytecode.c ${HDRS} ${SRCS})
target_link_libraries(test_parse_bytecode stp)
add_executable(test_stp test/test_stp.c ${HDRS} ${SRCS})
target_link_libraries(test_stp stp)
