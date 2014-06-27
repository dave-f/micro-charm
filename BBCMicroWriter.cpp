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
    f << uint8_t(1);

    // Start room
    f << uint8_t(1);

    // String table
    auto stringTable = c.getStringTable();
    
    f << uint8_t(c.getStringTable().size());
    short offset = 0;

    for (auto i : stringTable)
    {
        f << offset; 
        f << i.second;
        f << uint8_t(13);

        offset += i.second.length() + 1;
    }

    // Rooms
    auto rooms = c.getRooms();
    uint8_t roomID = 0;

    for (auto i : rooms)
    {
        f << roomID;
        f << getOffsetForObjectID(i.second.description);
        f << uint8_t(0); // n
        f << uint8_t(0); // s
        f << uint8_t(0); // e
        f << uint8_t(0); // w
        roomID++;
    }

    // Then add to SSD?
    // DiskWriter.. blah blah

    return true;
}

uint16_t BBCMicroWriter::getOffsetForObjectID( const Compiler::idType& objID ) const
{
    // search for objID...

    return m_baseAddress+0;
}
