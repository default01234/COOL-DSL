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
#include "coolang_compiler_function_name_deformation.hpp"

using namespace COOLANG;
using namespace std;

COOLANG::Compiler_FND compiler_FND;

void setOutputCodePath(const char *filePath) {
#if debug
    {
        cout << "C++版本:" << __cplusplus << endl;
        static int ignorecount = 0;
        cout << "setOutputCodePath(const char*)" << " " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "han_compiler_function_name_deformation.cpp:" << __LINE__
                << ":0" << ")" << endl;
    }
#endif
    try {
        compiler_FND.setOutputCodePath(filePath);
    } catch (exception &e) {
        cout << "setOutputCodePath err" << e.what() << endl;

    }
}
void addToken(Intg tokenType, char *tokenString) {
#if debug
    {
        static int ignorecount = 0;
        cout << "addBlock(int, char*)" << " " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "han_compiler_function_name_deformation.cpp:" << __LINE__
                << ":0" << ")" << endl;
    }
#endif
    try {

        compiler_FND.addToken(tokenType, tokenString);
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
                << __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
    }
#endif
    try {
        compiler_FND.flush();
    } catch (exception &e) {
        cout << "flushMap err" << e.what() << endl;
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
Strg Block::toStrg() const {
#if debug
    {
        static int ignorecount = 0;
        cout << "Block::toStrg()" << " " << "\tignorecount:[" << ignorecount++
                << "\t](" << "han_compiler_function_name_deformation.cpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    stringstream ss;
    if (isFN == false) {
        ss << s;
    } else {
        ss << const_cast<FunctionName*>(funn)->toStrg();

    }
    return ss.str();

}
void Compiler_FND::flush() {

#if debug
    {
        static int ignorecount = 0;
        cout << "Compiler_FND::flush()" << " " << "\tignorecount:["
                << ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
                << ":0" << ")" << endl;
    }
#endif

    outputfstream.open(filePath, ios::out | ios::app);

    while (currentFN->parentFN || currentFN->oldBrotherFN) {
        currentFN =
                currentFN->parentFN ?
                        currentFN->parentFN : currentFN->oldBrotherFN;
    }
    Strg &&flushStrg = currentFN->toStrg();

#if debug
    {
        static int ignorecount = 0;
        cout << "Compiler_FND::flush()" << " " << "\tignorecount:["
                << ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
                << ":0" << ")" << "\n\tflushStrg:[" << flushStrg << "]\t"
                << endl;
    }
#endif
    outputfstream << flushStrg;

    outputfstream.close();
    while (pdelete.size() > 0) {
        safe_delete(pdelete.back());
        pdelete.pop_back();
    }
    currentFN = NULL;
    return;
}
