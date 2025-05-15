cmake_minimum_required(VERSION 3.15)
project(MLIRHelloWorld)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 直接指定Homebrew安装路径
set(HOMEBREW_LLVM_PREFIX "/opt/homebrew/opt/llvm")

# 设置包搜索路径
list(APPEND CMAKE_PREFIX_PATH
    "${HOMEBREW_LLVM_PREFIX}/lib/cmake/llvm"
    "${HOMEBREW_LLVM_PREFIX}/lib/cmake/mlir"
)

find_package(LLVM REQUIRED CONFIG)
find_package(MLIR REQUIRED CONFIG)

include_directories(SYSTEM
    include
    ${LLVM_INCLUDE_DIRS}
    ${MLIR_INCLUDE_DIRS}
)

add_executable(mlir_hello src/main.cpp)

# 关键修正：直接指定MLIR库文件名
target_link_libraries(mlir_hello PRIVATE
    ${HOMEBREW_LLVM_PREFIX}/lib/libMLIRIR.a
    ${HOMEBREW_LLVM_PREFIX}/lib/libMLIRSupport.a
    ${HOMEBREW_LLVM_PREFIX}/lib/libMLIRParser.a
    LLVMCore
)

# macOS特殊设置
if(APPLE)
    target_compile_definitions(mlir_hello PRIVATE
        _LIBCPP_DISABLE_AVAILABILITY
    )
    target_link_options(mlir_hello PRIVATE
        -Wl,-undefined,dynamic_lookup
    )
endif()
