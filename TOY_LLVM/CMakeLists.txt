cmake_minimum_required(VERSION 3.30)
project(TOY_LLVM)

set(CMAKE_CXX_STANDARD 20)
find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs core support)

add_executable(TOY_LLVM
        toy.cpp)
target_link_libraries(TOY_LLVM ${llvm_libs})
