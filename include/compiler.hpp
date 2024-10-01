#ifndef COMPILER_H
#define COMPILER_H

#include <vector>
#include <string>
#include <memory>
#include <cstdint>

using ErrorHandler = void(*)(std::string&);

struct SelenaInfo {
    int shbinSize;
    bool hasCompiled;
    std::unique_ptr<uint32_t> shbin;
    std::vector<std::string> errors;
};

/**
 * @brief Sets the error handler to use upon error
 * @param[in] handler Error handler to use
 */
void SelenaSetErrorHandler(ErrorHandler handler);

/**
 * @brief Compiles a GLSL shader to a SHBIN at runtime
 * 
 * @param[in] source Source code string to compile
 * @return A struct which holds compilation results
 */
SelenaInfo SelenaCompileShaderSource(std::string& source);

#endif /* COMPILER_H */