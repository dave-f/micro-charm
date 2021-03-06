#pragma once

#include <string>

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
    bool getStringID(const Compiler::idType& objID, uint8_t& id) const;
    std::vector<std::string> buildStringTable(const Compiler& c);
    bool BBCMicroWriter::getRoomID( const Compiler::idType& objID, const std::vector<std::pair<Compiler::idType,Compiler::Room>>& r, uint8_t& id) const;

    uint16_t m_baseAddress;
    std::map<Compiler::idType,uint32_t> m_stringIDs;
};
