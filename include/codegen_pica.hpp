#ifndef CODEGENERATOR_PICA_HPP
#define CODEGENERATOR_PICA_HPP

//#include "ast.hpp"
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <variant>

struct AstNode;

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#define vectorized [4] = {0}

// Supported Pica Variable types
enum { Int, Double, Floating, Boolean, Vector2, Vector3, Vector4, NoReturn };

// A simple Pica Register
struct PicaRegister {
    char registerType;
    int registerValue;
    std::array<char, 4> swizzleMask;
};

// A simple Pica Constant can be one of these
using PicaConstant = std::variant<int[4], float[4], bool>;

struct PicaRegisterFile {
    int ints[4] vectorized;
    int bools[16] = {0};
    int floats[96] vectorized;
    int inputs[16] vectorized;
    int outputs[16] vectorized;
    int temporary[16] vectorized;
    int andress[3] = {0};

    // Allocate scalars in free registers
    template<int amount>
    PicaRegister Allocate(char reg);

    // Frees a scalars specified registers
    template<int amount>
    void Free(PicaRegister registers);
};

struct PicaVariable {
    char variableType;
    std::string variableName;

    PicaConstant variableConstant;
    PicaRegister variableRegister;
};

struct PicaInstruction {
    PicaVariable dst;
    PicaVariable src[3];
    int instructionType;
};

struct PicaProgram;

struct PicaFunction {
    int returnType;
    std::string name;
    std::vector<PicaVariable> variables;
    std::shared_ptr<PicaProgram> program;
    std::vector<PicaInstruction> instructions;

    PicaFunction(PicaProgram* p) : program(p) {}
};

struct PicaProgram {
    std::vector<PicaFunction> functions;
    std::vector<PicaVariable> globals;
    PicaRegisterFile registers;
};

std::unique_ptr<PicaProgram> BuildIR(AstNode* root);

class CodeGeneratorPica {
    public:
    // Generate the Assembly
    std::string Generate(void);

    // Get the IR Representation
    CodeGeneratorPica(PicaProgram* p);
    private:
    // IR Program to convert to Assembly
    std::unique_ptr<PicaProgram> program;
};
#endif /* CODEGENERATOR_HPP */