#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <vector>
#include <string>
#include <memory>
#include <cstdint>

using ErrorHandler = void(*)(std::string&);

struct SelenaInfo {
    int shbinSize;
    bool hasCompiled;
    std::vector<std::string> errors;
    std::unique_ptr<uint32_t> shbinData;
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

#endif /* COMPILER_HPP */