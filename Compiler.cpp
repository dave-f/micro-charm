#include "stdafx.h"
#include "Compiler.h"
#include <string>

using namespace std;

const string Compiler::nullObjectId = "null";

Compiler::Compiler(void) : m_generatedObjectID(0)
{
}

Compiler::~Compiler(void)
{
}

void Compiler::parseMessages( TiXmlElement* rootElement )
{
    auto messageElement = rootElement->FirstChildElement("messages");

    if (messageElement)
    {
        // System messages come first [30/6/2014 dave.footitt]
        auto systemElement = messageElement->FirstChildElement("system");

        if (systemElement)
        {
            for (auto i=systemElement->FirstChildElement("msg"); i!=nullptr; i=i->NextSiblingElement())
            {
                if (i->Attribute("id")!=nullptr)
                {
                    // Maybe need some special flag
                    addToStringTable(i->Attribute("id"),i->GetText(),80); // TODO: Remove this offset, not needed now
                    // Can possibly remove boost::tuple now too.
                }
            }
        }
        
        // Now the other messages
        for (auto i=messageElement->FirstChildElement("msg"); i!=nullptr; i=i->NextSiblingElement())
        {
            if (i->Attribute("id")!=nullptr)
            {
                addToStringTable(i->Attribute("id"),i->GetText());
            }
        }

    }
}

void Compiler::parseRooms( TiXmlElement* rootElement )
{
    auto roomElement = rootElement->FirstChildElement("rooms");

    if (roomElement)
    {
        for (auto i=roomElement->FirstChildElement("room"); i!=nullptr; i=i->NextSiblingElement())
        {
            Room newRoom;
            auto d = i->FirstChildElement("description");
            if (d!=nullptr)
            {
                string id = generateID();
                addToStringTable(id, d->GetText());
                newRoom.description = id;
            }
            // For now
            for (auto& j : newRoom.exits)
            {
                j = nullObjectId;
            }
            m_rooms.insert(std::make_pair(i->Attribute("id"),newRoom)); // todo: check id !nullptr
        }
    }

}

bool Compiler::compileFile( const std::string& sourceFile )
{
    TiXmlDocument doc(sourceFile);

    if (doc.LoadFile())
    {
        TiXmlElement* rootElement = doc.RootElement();
        reset();

        parseMessages(rootElement);
        //parseVerbs();
        //parsePreps();
        //parseObjects();
        //parseHighConditions();
        //parseLowConditions();
        parseRooms(rootElement);

        return true;
    }

    return false;
}

void Compiler::reset()
{
    m_messages.clear();
    m_stringTableOffset = 0;
}

void Compiler::addToStringTable( const idType& id, const std::string& s, int32_t offset )
{
    if (offset==-1)
    {
        m_messages.push_back(boost::make_tuple(id,m_stringTableOffset,s));
    }
    else
    {
        m_messages.push_back(boost::make_tuple(id,offset + m_stringTableOffset,s));
    }

    m_stringTableOffset++;
}

