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
