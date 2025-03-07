#include <cctype>
#include <cstdio>
#include <ctype.h>
#include <map>
#include <string>
#include <vector>


#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

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

// 存储运算符优先级
static std::map<char, int>Operator_Precedence;

static void init_precedence() {
    Operator_Precedence['-'] = 1;
    Operator_Precedence['+'] = 2;
    Operator_Precedence['/'] = 3;
    Operator_Precedence['*'] = 4;
}

// 辅助返回已经定义的二元运算符的优先级
static int getBinOpPrecedence() {
    if(!isascii(Current_token))
    return -1;

    int TokPrec = Operator_Precedence[Current_token];
    if (TokPrec <= 0) return -1;
    return TokPrec;
}

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

// 函数定义的AST类
class FunctionDefnAST {
    FunctionDeclAST *Func_Decl;
    BaseAST* Body;
    public:
    FunctionDefnAST(FunctionDeclAST *proto, BaseAST *body) : Func_Decl(proto), Body(body) {};
};

// 函数定义的AST类定义
class FunctionCallAST : public BaseAST{
    std::string Function_Callee;
    std::vector<BaseAST*> Function_Arguements;
    public:
    FunctionCallAST(const std::string &callee, std::vector<BaseAST*> &args) : Function_Callee(callee), Function_Arguements(args) {};
};

// 二元运算符parser
static BaseAST* binary_op_parser(int Old_Prec, BaseAST *LHS) {
    while (1) {
        int Operator_Prec = getBinOpPrecedence();
        if(Operator_Prec < Old_Prec)
            return LHS;
        
        int BinOp = Current_token;
        next_token();

        BaseAST* RHS = Base_Parser();
        if(!RHS) return 0;

        int Next_Prec = getBinOpPrecedence();
        if (Operator_Prec < Next_Prec) {
            RHS = binary_op_parser(Operator_Prec + 1, RHS);
            if (RHS == 0) return 0;
        }
        
        LHS = new BinaryAST(std::to_string(BinOp), LHS, RHS);
    }
}

static BaseAST* numeric_parser() {
    BaseAST *Result = new NumericAST(Numeric_Val);
    next_token();
    return Result;
}

static BaseAST* expression_parser() ;

static BaseAST* paran_parser() {
    next_token();
    BaseAST* V = expression_parser();

    if (!V) return 0;

    if (Current_token != ')')
        return 0;

    return V;
}

static BaseAST* identfier_parser() {
    std::string IdName = Identifer_string;

    next_token();

    if (Current_token != '(') 
        return new VariableAST(IdName);

    next_token();

    std::vector<BaseAST*> Args;

    if (Current_token != ')') {
        while (1) {
            BaseAST* Arg = expression_parser();
            if (!Arg) return 0;

            Args.push_back(Arg);
            
            if (Current_token == ')') break;

            if (Current_token != ',')
                return 0;
            next_token();
        }
    }

    next_token();

    return new FunctionCallAST(IdName, Args);
}

static BaseAST* expression_parser() {
    BaseAST *LHS = Base_Parser();
    if (!LHS) return 0;
    return binary_op_parser(0, LHS);
}

static BaseAST* Base_Parser() {
    switch (Current_token) {
        default: return 0;
        case IDENTIFIER_TOKEN : return identfier_parser();
        case NUMERIC_TOKEN : return numeric_parser();
        case '(' : return paran_parser();
    }
}

static FunctionDeclAST * func_decl_parser() {
    if (Current_token != IDENTIFIER_TOKEN) 
        return 0;

    std::string FnName = Identifer_string;
    next_token();

    if(Current_token != '(')
        return 0;

    std::vector<std::string> Function_Argument_Names;
    while (next_token() == IDENTIFIER_TOKEN) 
        Function_Argument_Names.push_back(Identifer_string);
    if (Current_token != ')')
        return 0;

    next_token();

    return new FunctionDeclAST(FnName, Function_Argument_Names);
}

static FunctionDefnAST *func_defn_parser() {
    next_token();
    FunctionDeclAST *Decl = func_decl_parser();
    if (Decl == 0) return 0;

    if (BaseAST* Body = expression_parser())
        return new FunctionDefnAST(Decl, Body);
    return 0;
}

// 高层函数
static void HandleDefn() {
    if (FunctionDefnAST *F = func_defn_parser()) {
        if (Function* LF = F->Codegen()) {

        }
    } else {
        next_token();
    }
}

static void HandleTopExpression() {
    if (FunctionDefnAST *F = top_level_parser()) {
        if (Function *LF = F->Codegen()) {

        }
    } else {
        next_token();
    }
}

static void Driver() {
    while (1) {
        switch (Current_token) {
            case EOF_TOKEN: return;
            case ';': next_token(); break;
            case DEF_TOKEM: HandleDefn(); break;
            default: HandleTopExpression(); break;
        }
    }
}

using namespace llvm ;

int main(int argc, char* argv[]) {
    LLVMContext &Context = getGlobalContext();
    init_precedence();
    file = fopen(argv[1], "r");
    if(file == 0) {
        printf("Could not open file\n");
    }

    next_token();
    Module_Ob = new Module("my compiler", Context);
    Driver();
    Module_Ob->dump();
    return 0;
}