#pragma once

#include <string>

class Compiler
{
public:
    Compiler(void);
    ~Compiler(void);

public:
    bool compileFile(const std::string& sourceFile);
};
