#include <mlir/IR/BuiltinOps.h>
#include <mlir/IR/MLIRContext.h>
#include <mlir/IR/Builders.h>  // 新增的头文件
#include <llvm/Support/raw_ostream.h>
#include "toydialect.h"

using namespace mlir;

int main() {
  // 初始化MLIR上下文
  MLIRContext context;
  
  // 创建模块
  OwningOpRef<ModuleOp> module = ModuleOp::create(UnknownLoc::get(&context));
  
  // 创建Builder
  OpBuilder builder(&context);
  
  // 在模块中插入一个空函数（更实际的示例）
  builder.setInsertionPointToStart(module->getBody());
  
  // 打印模块
  llvm::outs() << "\n=== MLIR Hello World ===\n";
  module->print(llvm::outs());
  llvm::outs() << "\n========================\n";
  
  return 0;
}
