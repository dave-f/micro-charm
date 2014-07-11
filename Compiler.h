#pragma once

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <array>
#include "tinyxml.h"

class Compiler
{
public:
    typedef std::string idType;
    static const  std::string nullObjectId;

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

    const std::vector<std::pair<idType,Room>>& getRooms() const { return m_rooms; } 
    const std::vector<std::pair<idType,std::string>>& getStringTable() const 
    { 
        return m_messages; 
    }
    std::string getStartRoom() const
    {
        return m_startRoomStr;
    }

protected:
    void reset();
    void parseHeader(TiXmlElement*);
    void parseMessages(TiXmlElement*);
    void parseRooms(TiXmlElement*);
    void addToStringTable(const idType& id, const std::string& s);
    std::string generateID()
    {
        ++m_generatedObjectID;
        return "obj" + std::to_string(m_generatedObjectID);
    }

    std::vector<std::pair<idType,std::string>> m_messages;
    std::vector<std::pair<idType,Room>> m_rooms;
    std::string m_startRoomStr;
    uint32_t m_generatedObjectID;
};
