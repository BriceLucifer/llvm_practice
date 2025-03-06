#include <cctype>
#include <cstdio>
#include <string>
#include <vector>

enum Token_Type {
    EOF_TOKEN = 0,
    NUMERIC_TOKEN,
    IDENTIFIER_TOKEN,
    PARAN_TOKEN,
    DEF_TOKEM,
};

// 持有数据
static int Numeric_Val;

// 持有字符串名字
static std::string Identifer_string;

// 文件
FILE *file;

// 持有当前的token 来自词法分析器
static int Current_token;

static int get_token() {
    static int LastChar = ' ';

    while (isspace(LastChar))
    LastChar = fgetc(file);
    
    if (isalpha(LastChar)) {
        Identifer_string = LastChar;
        while (isalnum((LastChar = fgetc(file)))) 
            Identifer_string += LastChar;
        
        if (Identifer_string == "def")
        return DEF_TOKEM;
        return IDENTIFIER_TOKEN;
    }

    if (LastChar == '#') {
        do LastChar = fgetc(file);
        while(LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF) return get_token();
    }

    if (LastChar == EOF ) return EOF_TOKEN;

    int ThisChar = LastChar;
    LastChar = fgetc(file);

    return ThisChar;
}

static void next_token() {
    Current_token = get_token();
}

class BaseAST {
    public:
    virtual ~BaseAST();
};

// 变量表达式
class VariableAST : public BaseAST {
    std::string Var_name;
    // 定义string对象用作存储变量名
    public:
    VariableAST(std::string &name) : Var_name(name) {};
};

// 数值表达式
class NumericAST : public BaseAST {
    int numeric_val;
    public:
    NumericAST (int val): numeric_val(val) {};
};

// 二元运算表达式
class BinaryAST : public BaseAST {
    std::string Bin_Operator;
    // 由于LHS和RHS可以是任何类型， 因此用BaseAST对象存储
    BaseAST *LHS, *RHS;
    public:
    BinaryAST(std::string op, BaseAST *lhs, BaseAST *rhs) : Bin_Operator(op), LHS(lhs), RHS(rhs) {};
};

// 函数声明的AST
class FunctionDeclAST {
    std::string Func_name;
    std::vector<std::string> Arguments;
    public:
    FunctionDeclAST(const std::string &name, const std::vector<std::string> &args) : Func_name(name), Arguments(args) {};
};

// 函数定义的AST类定义
class FunctionCallAST : public BaseAST{
    std::string Function_Callee;
    std::vector<BaseAST*> Function_Arguements;
    public:
    FunctionCallAST(const std::string &callee, std::vector<BaseAST*> &args) : Function_Callee(callee), Function_Arguements(args) {};
};

static BaseAST* Base_Parser() {
    switch (Current_token) {
        default: return 0;
        case IDENTIFIER_TOKEN : return identfier_parser();
        case NUMERIC_TOKEN : return numeric_parser();
        case '(' : return paran_parser();
    }
}