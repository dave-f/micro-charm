#pragma once

#include <string>

class Compiler;

class Writer
{
public:
    Writer(void);
    virtual ~Writer(void);

public:
    virtual bool writeFile(const Compiler& c, const std::string& fileName) = 0;
};

