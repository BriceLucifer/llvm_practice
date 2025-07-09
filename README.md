# MLIR 配置
```
brew install llvm lldb
```
how to run lli in llvm@20
```bash
clang++ -fno-stack-check -S -emit-llvm test.cpp -o test.ll
lli test.ll
echo $?
```

## the way to compile the main.cpp in first program main.cpp in src directory
```bash
clang++ src/main.cpp \
    -I/opt/homebrew/opt/llvm/include \
    -std=c++17 \
    -stdlib=libc++ \
    -fno-exceptions \
    -funwind-tables \
    -D__STDC_CONSTANT_MACROS \
    -D__STDC_FORMAT_MACROS \
    -D__STDC_LIMIT_MACROS \
    -L/opt/homebrew/opt/llvm/lib \
    -Wl,-search_paths_first \
    -Wl,-headerpad_max_install_names \
    -lLLVM \
    -o EvaLLVM
```
