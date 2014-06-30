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
}

void Compiler::addToStringTable( const idType& id, const std::string& s )
{
    m_messages.insert(std::make_pair(id,s));
}
