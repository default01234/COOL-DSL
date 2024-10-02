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

/*
 *interface
 */
#include "coolang_compiler_convert_non_ascii_character.hpp"

using namespace COOLANG;
using namespace std;

COOLANG::Compiler_CNAC compiler_CNAC;

void setOutputCodePath(const char *filePath) {
#if debug
    {
        cout << "C++版本:" << __cplusplus << endl;
        static int ignorecount = 0;
        cout << "setOutputCodePath(const char*)" << " " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << __FILE__<< ":" << __LINE__
                << ":0" << ")" << endl;
    }
#endif
    try {
        compiler_CNAC.setOutputCodePath(filePath);
    } catch (exception &e) {
        cout << "setOutputCodePath err" << e.what() << endl;

    }
}
void addToken(Intg tokenType, const char *tokenString) {

    try {

        compiler_CNAC.addToken(tokenType, tokenString);
        return;
    } catch (exception &e) {
        cout << "matchToken err" << e.what() << endl;
        exit(1);
    }

}
void flush() {
#if debug
    {
        static int ignorecount = 0;
        cout << "flush()" << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                << "COOLANG_convert_non_ascii_character.cpp:" << __LINE__
                << ":0" << ")" << endl;
    }
#endif
    try {
        compiler_CNAC.flush();
    } catch (exception &e) {
        cout << "flush err" << e.what() << endl;
    }
}
Intg COOLANG::toIntg(Strg s) {
    return atoi(s.c_str());
}
COOLANG::Strg COOLANG::toStrg(const Intg &i) {
    stringstream ss;
    ss << i;
    return ss.str();
}
bool COOLANG::isNumber(const string &str) {
    stringstream ss(str);
    double d;
    char c;
    if (!(ss >> d))
        return false;
    if (ss >> c)
        return false;
    return true;
}
void Compiler_CNAC::flush() {

#if debug
    {
        static int ignorecount = 0;
        cout << "Compiler_CNAC::flush()" << " " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "COOLANG_convert_non_ascii_character.cpp:" << __LINE__
                << ":0" << ")" << endl;
    }
#endif

    outputfstream.open(filePath, ios::out | ios::app);

    outputfstream << currentflushStrg;

    outputfstream.close();

    currentflushStrg.clear();
    return;
}
std::string COOLANG::nonASCIICodeToHex(const char *nonASCIICode, unsigned int length) {
    string hexOut;
    for (unsigned int i = 0; i < length; i++) {
        char hexChar[6] = { 0 };
        sprintf(hexChar, "_u%02X_", (unsigned char) nonASCIICode[i]);
        hexOut += hexChar;
    }
    return hexOut;
}
std::string COOLANG::nonASCIICodeToHex(const char *nonASCIICode) {
    string hexOut;
    for (unsigned int i = 0; (unsigned char) nonASCIICode[i] != '\0'; i++) {
        char hexChar[6] = { 0 };
        sprintf(hexChar, "_u%02X_", (unsigned char) nonASCIICode[i]);
        hexOut += hexChar;
    }
    return hexOut;
}
