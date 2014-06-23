#pragma once

#include "writer.h"

class BBCMicroWriter : public Writer
{
public:
    BBCMicroWriter(void);
    virtual ~BBCMicroWriter(void);

public:
    virtual bool writeFile(const Compiler& c, const std::string& fileName);
};
