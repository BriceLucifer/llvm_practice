#pragma once 

#include <mlir/IR/Dialect.h>
#include <mlir/IR/MLIRContext.h>
#include <llvm/ADT/StringRef.h>
#include <mlir/IR/DialectRegistry.h>

class ToyDialect: public mlir::Dialect{
    public:
    explicit ToyDialect(mlir::MLIRContext *ctx);

    static llvm::StringRef getDialectNamespace() {
        return "toy";
    }

    void initialize();
};