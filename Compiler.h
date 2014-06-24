#pragma once

#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include "tinyxml.h"

class Compiler
{
public:
    typedef std::string idType;

private:
    struct Room
    {
        // blah blah
        idType description;
        std::vector<std::pair<std::string,idType>> exits; // nsew->roomID
    };

public:
    Compiler(void);
    ~Compiler(void);

public:
    bool compileFile(const std::string& sourceFile);

    const std::map<idType,Room>& getRooms() const { return m_rooms; } 
    const std::map<idType,std::string>& getStringTable() const { return m_messages; }


protected:
    void reset();
    void parseMessages(TiXmlElement*);
    void parseRooms(TiXmlElement*);
    void addToStringTable(const idType& id, const std::string& s);
    std::string generateID()
    {
        ++m_generatedObjectID;
        return "Object" + std::to_string(m_generatedObjectID);
    }

    std::map<idType,std::string> m_messages;
    std::map<idType,Room> m_rooms;
    uint32_t m_generatedObjectID;
};
