#pragma once

#include <string>
#include <map>
#include "tinyxml.h"

class Compiler
{
public:
    Compiler(void);
    ~Compiler(void);

public:
    bool compileFile(const std::string& sourceFile);

protected:
    void reset();
    void parseMessages(TiXmlElement*);
    void parseRooms(TiXmlElement*);

    typedef std::string idType;

    std::map<idType,std::string> m_messages;
};
