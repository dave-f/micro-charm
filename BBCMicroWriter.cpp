#include "stdafx.h"

#include <fstream>
#include <stdint.h>

#include "BBCMicroWriter.h"
#include "Compiler.h"

using namespace std;

/*   BBC Micro File Format
 *   ---------------------
 *
 *   Byte          Meaning
 *   ----          -------
 *
 *      1          Version number
 *      2          Start room
 *      3          Text table
 *    4,4          ID
 *     6+          Text itself (13 terminated)
 *
 * Followed by room data:
 *
 *      1          RoomID
 *      2          Offset to next room
 *    3,4          Text ID
 *      5          Exits
 *
 *      7+         (Repeats)
 *
 *
 */

BBCMicroWriter::BBCMicroWriter(void) : m_baseAddress(0x2100) // Maybe pass this in on cmdline?
{
}


BBCMicroWriter::~BBCMicroWriter(void)
{
}

bool BBCMicroWriter::writeFile(const Compiler& c, const std::string& fileName)
{
    ofstream f(fileName,ios::binary);

    if (!f)
    {
        return false;
    }

    // Version number
    uint8_t versionNumber = 1;
    f.write(reinterpret_cast<const char*>(&versionNumber),1); 

    // Start room
    uint8_t startRoom = 1;
    f.write(reinterpret_cast<const char*>(&startRoom),1); 

    // String table
    auto stringTable = c.getStringTable();
    
    f << uint8_t(c.getStringTable().size());
    uint16_t offset = stringTable.size()*2;

    for (auto i : stringTable)
    {
        f.write(reinterpret_cast<const char*>(&offset),2); 
        offset += i.second.length() + 1;
    }

    m_ids.clear();
    uint32_t num = 0;

    for (auto i : stringTable)
    {
        f << i.second;
        f.put(13); // cr, not sure we need this
        m_ids.push_back(std::make_pair(i.first,num));
        num++;
    }

    // Rooms
    auto rooms = c.getRooms();
    uint8_t roomID = 0;
    int32_t objOffset;

    //auto prompt = getOffsetForObjectID("obj1");

    for (auto i : rooms)
    {
        f.write(reinterpret_cast<const char*>(&roomID),1);
        objOffset = getOffsetForObjectID(i.second.description);
        f.write(reinterpret_cast<const char*>(&offset),2);
        
        for (auto j : i.second.exits) // needs to be populated with n,s,e,w
        {
            if (true)
            {
                f.put(0);
            }
            else
            {
                // no exit, write 0
            }
        }

        roomID++;
    }

    // Then add to SSD if specified maybe
    // DiskWriter.. blah blah

    return true;
}

int32_t BBCMicroWriter::getOffsetForObjectID( const Compiler::idType& objID ) const
{
    auto obj = std::find_if(m_ids.cbegin(),m_ids.cend(),[&](const std::pair<Compiler::idType,uint32_t>& i)
    {
        return i.first==objID;
    });

    return obj==m_ids.end() ? -1 : obj->second;
}
