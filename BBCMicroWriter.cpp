#include "stdafx.h"

#include <fstream>
#include <stdint.h>
#include "boost/lexical_cast.hpp"

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

    // ------------ Header

    // Write Version number
    uint8_t versionNumber = 1;
    f.write(reinterpret_cast<const char*>(&versionNumber),1); 

    // Write Start room
    uint8_t startRoom;
    std::string s = c.getStartRoom();
    if (s.empty())
    {
        startRoom = 1;
    }
    else
    {
        startRoom = 0;
        for (auto& i : c.getRooms())
        {
            if (i.first==s)
            {
                break;
            }
            startRoom++;
        }
    }
    f.write(reinterpret_cast<const char*>(&startRoom),1); 

    // Build string table
    std::vector<std::string> stringTable = buildStringTable(c);
    uint8_t numElems = stringTable.size(); // todo: check > 255

    // Write num strings
    f.write(reinterpret_cast<const char*>(&numElems),1);
    uint16_t stringOffset = 5 + numElems*2; // 5 is the header size (ver,start room,rooms offset(2 bytes) and num strings)

    // Write room offset
    uint16_t totalStringLength = 0;
    for (auto& i : stringTable)
        totalStringLength += (i.length()+1); // string length + CR
    uint16_t roomOffset = 5 + numElems*2 + totalStringLength; // 5 is the header size (ver,start room,rooms offset(2 bytes) and num strings)
    f.write(reinterpret_cast<const char*>(&roomOffset),2);

    // ------------ Header

    // Write string table
    for (auto i : stringTable)
    {
        f.write(reinterpret_cast<const char*>(&stringOffset),2);
        stringOffset += i.length() + 1;
    }

    // now write the strings
    for (auto i : stringTable)
    {
        f << i;
        f.put(13);
    }

    // Rooms
    auto rooms = c.getRooms();
    uint8_t roomID = 0;
    uint8_t nextRoomOffset = 0;
    uint8_t spare = 0;

    for (auto& i : rooms)
    {
        // Room ID (1 byte)
        f.write(reinterpret_cast<const char*>(&roomID),1);

        // Offset to next room (1 byte)
        nextRoomOffset = 8;
        f.write(reinterpret_cast<const char*>(&nextRoomOffset),1);
        
        // Room description ID (1 byte)
        uint8_t descriptionIndex;
        getStringID(i.second.description,descriptionIndex);
        f.write(reinterpret_cast<const char*>(&descriptionIndex),1);
        
        // Room exits (4 bytes)
        for (auto j : i.second.exits)
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

        // Spare (1 byte)
        f.write(reinterpret_cast<const char*>(&spare),1);

        roomID++;
    }

    // Then add to SSD if specified maybe
    // DiskWriter.. blah blah

    return true;
}

bool BBCMicroWriter::getStringID( const Compiler::idType& objID, uint8_t& id ) const
{
    auto it = m_stringIDs.find(objID);

    if (it !=m_stringIDs.end())
    {
        id = static_cast<uint8_t>(it->second);
        return true;
    }
    else
    {
        return false;
    }
}

std::vector<std::string> BBCMicroWriter::buildStringTable(const Compiler& c)
{
    std::vector<std::string> r;
    auto x = c.getStringTable();
    uint32_t j = 0;
    m_stringIDs.clear();

    for (const auto& i : x)
    {
        r.push_back(i.second);
        m_stringIDs[i.first]=j++;
    }

    return r; // move
}