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
    int32_t getOffsetForObjectID(const Compiler::idType& objID) const;
    std::array<std::string,100> buildStringTable(const Compiler& c);

    uint16_t m_baseAddress;

    std::vector<std::pair<Compiler::idType,uint32_t>> m_ids;
    std::array<std::string,100> m_stringTable;
};
