#include "stdafx.h"
#include <string>
#include "Compiler.h"
#include "boost/algorithm/string.hpp"

using namespace std;

const string Compiler::nullObjectId = "null";

Compiler::Compiler(void)
{
}

Compiler::~Compiler(void)
{
}

void Compiler::parseHeader(TiXmlElement* rootElement)
{
    auto headerElement= rootElement->FirstChildElement("adventure");

    if (headerElement)
    {
        auto startElem = headerElement->FirstChildElement("start");
        m_startRoomStr = startElem==nullptr ? "" : startElem->GetText();
    }
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
                    addToStringTable(i->Attribute("id"),i->GetText()); // TODO: Remove this offset, not needed now
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
        // Add "room 0" : this is where all the objects dropped go[2/7/2014 dave.footitt]
        Room r;
        r.description = nullObjectId;
        std::fill(r.exits.begin(),r.exits.end(),Compiler::nullObjectId);
        m_rooms.push_back(std::make_pair(nullObjectId,r));

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

            for (auto& e : newRoom.exits)
                e = nullObjectId;

            TiXmlElement* e = i->FirstChildElement("exits");
            if (e != nullptr)
            {
                std::array<std::string,4> d = {"north","south","east","west"};

                for (e=e->FirstChildElement(); e != nullptr; e=e->NextSiblingElement())
                {
                    uint32_t exitNo = 0;
                    for (auto& exitStr : d)
                    {
                        if (boost::iequals(e->Value(),exitStr))
                        {
                            newRoom.exits[exitNo]=e->GetText();
                            break;
                        }
                        ++exitNo;
                    }
                }
            }

            m_rooms.push_back(std::make_pair(i->Attribute("id"),newRoom));
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

        parseHeader(rootElement);
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
    m_rooms.clear();
    m_generatedObjectID = 0;
}

void Compiler::addToStringTable( const idType& id, const std::string& s )
{
    m_messages.push_back(std::make_pair(id,s));
}

