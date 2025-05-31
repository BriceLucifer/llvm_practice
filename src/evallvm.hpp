#pragma once

#include <string>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <system_error>

class EvaLLVM {
    public:
        EvaLLVM() {
            moduleInit();
        }

        void exec(const std::string& program) {
            // 1. parse the program
            // auto ast = parser->parser(program);
            // 2. compile to LLVM IR
            // compile(ast);
            
            moduler->print(llvm::outs(),nullptr);

            // 3. save module IR to file
            saveModuleToFile("./out.ll");
        }
    private:
        void moduleInit() {
            ctx = std::make_unique<llvm::LLVMContext>();
            moduler = std::make_unique<llvm::Module>("EvaLLVM", *ctx);
            builder = std::make_unique<llvm::IRBuilder<>>(*ctx);
        }
        void saveModuleToFile(const std::string& filename) {
            std::error_code errorCode;
            llvm::raw_fd_ostream outLL(filename, errorCode);
            moduler->print(outLL, nullptr);
        }
        std::unique_ptr<llvm::LLVMContext> ctx;
        std::unique_ptr<llvm::Module> moduler;
        std::unique_ptr<llvm::IRBuilder<>> builder;
};