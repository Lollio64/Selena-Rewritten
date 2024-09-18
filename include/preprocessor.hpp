#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <string>
#include <vector>
#include <memory>
#include "lexer.hpp"

struct Macro {
    std::string id;
    std::string expansion;
    std::vector<std::string> params;
};

class Preprocessor {
    std::vector<Macro> macros;

    void ResolveMacros(std::shared_ptr<Token> token);

    template<typename T>
    void AddMacro(std::string id, T value, std::vector<std::string> params);
};
#endif /* PREPROCESSOR_H */