#include "stdafx.h"
#include "Compiler.h"

Compiler::Compiler(void)
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
                m_messages.insert(std::make_pair(i->Attribute("id"),i->GetText()));
        }
    }
}

void Compiler::parseRooms( TiXmlElement* )
{

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
        //parseNouns();
        //parseAdverbs();
        //parseObjects();
        //parseHighConditions();
        //parseLowConditions();
        //parseRooms(rootElement);

        return true;
    }

    return false;
}

void Compiler::reset()
{
    m_messages.clear();
}
