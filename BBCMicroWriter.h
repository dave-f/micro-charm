#pragma once

#include "writer.h"
#include "Compiler.h"

class BBCMicroWriter : public Writer
{
public:
    BBCMicroWriter(void);
    virtual ~BBCMicroWriter(void);

public:
    virtual bool writeFile(const Compiler& c, const std::string& fileName);

protected:
    uint16_t getOffsetForObjectID(const Compiler::idType& objID) const;

    uint16_t m_baseAddress;
};
