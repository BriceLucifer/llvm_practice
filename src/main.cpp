#include "evallvm.hpp"

int main() {
    std::string program = R"(
        42
    )";

    EvaLLVM vm;

    vm.exec(program);

    return 0;
}