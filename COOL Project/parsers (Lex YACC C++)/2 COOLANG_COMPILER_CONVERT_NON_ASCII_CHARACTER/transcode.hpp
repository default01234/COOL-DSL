/*  COOLang compiler implementation
    Copyright (C) 2022,Han JiPeng,Beijing Huagui Technology Co., Ltd

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, version LGPL-3.0-or-later.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef TRANSCODE_H_
#define TRANSCODE_H_
#include <iostream>
#include <cstdio>

using namespace std;
namespace COOLANG{
/**
 *
 * @param nonASCIICode code to transcode;
 * @param length  the nonASCIICode's length;
 * @return the code encoded by hex;
 */
string NonASCIICodeToHex(const char *nonASCIICode, unsigned int length);

/**
 *
 * @param nonASCIICode :the pointer of the nonASCIICode
 * @return the code encoded by hex;
 */
string NonASCIICodeToHex(const char *nonASCIICode);

}

std::string Chan::NonASCIICodeToHex(const char *nonASCIICode, unsigned int length) {
    string hexOut;
    for (unsigned int i = 0; i < length; i++) {
        char hexChar[6] = { 0 };
        sprintf(hexChar, "_u%02X_", (unsigned char) nonASCIICode[i]);
        hexOut += hexChar;
    }
    return hexOut;
}
std::string Chan::NonASCIICodeToHex(const char *nonASCIICode) {
    string hexOut;
    for (unsigned int i = 0; (unsigned char) nonASCIICode[i] != '\0'; i++) {
        char hexChar[6] = { 0 };
        sprintf(hexChar, "_u%02X_", (unsigned char) nonASCIICode[i]);
        hexOut += hexChar;
    }
    return hexOut;
}

#endif /* TRANSCODE_H_ */
