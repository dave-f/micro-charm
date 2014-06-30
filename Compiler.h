#pragma once

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <array>
#include "tinyxml.h"
#include "boost/tuple/tuple.hpp"

class Compiler
{
public:
    typedef std::string idType;
    static const  std::string nullObjectId;

private:
    struct Room
    {
        enum ExitType : uint8_t
        {
            None = 0,
            North = 1,
            South = 2,
            East = 3,
            West = 4
        };
        // blah blah
        idType description;
        std::array<idType,4> exits;
    };

public:
    Compiler(void);
    ~Compiler(void);

public:
    bool compileFile(const std::string& sourceFile);

    const std::map<idType,Room>& getRooms() const { return m_rooms; } 
    const std::vector<boost::tuple<idType,uint32_t,std::string>>& getStringTable() const 
    { 
        return m_messages; 
    }


protected:
    void reset();
    void parseMessages(TiXmlElement*);
    void parseRooms(TiXmlElement*);
    void addToStringTable(const idType& id, const std::string& s, int32_t offset = -1);
    std::string generateID()
    {
        ++m_generatedObjectID;
        return "obj" + std::to_string(m_generatedObjectID);
    }

    std::vector<boost::tuple<idType,uint32_t,std::string>> m_messages;
    std::map<idType,Room> m_rooms;
    uint32_t m_generatedObjectID;
    int32_t m_stringTableOffset;
};
