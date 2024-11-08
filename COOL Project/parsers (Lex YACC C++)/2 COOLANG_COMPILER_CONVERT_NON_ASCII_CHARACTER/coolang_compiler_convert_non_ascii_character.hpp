/*  COOLang compiler implementation
    Copyright (C) 2022,Han JiPeng,Beijing Huagui Technology Co., Ltd

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, version LGPL-3.0-or-later.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCCOOLANGTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef COOLANG_COMPILER_FUNCTION_NAME_DEFORMATION_HPP
#define COOLANG_COMPILER_FUNCTION_NAME_DEFORMATION_HPP

#include "coolang_compiler_define.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "rule_token_number.h"
#include "y.tab.h"
#include<map>
#include<vector>
#include <deque>



#define Intg int
#define Addr int


#define INSERTSTRG "_ARG_"

#define __line__ "#line"
#ifndef fileno
#define fileno(__F) ((__F)->_file)
#endif
using namespace std;
namespace COOLANG{
class Strg: public string {
public:
    Strg() :
            string("") {

    }
    Strg(const string &s) :
            string(s) {
    }
    Strg(const char *s) :
            string(s) {
    }
    Intg size() {
        return this->string::size();
    }
    Strg& operator+=(const string &s) {
        this->string::operator+=(s);
        return (*this);
    }
    Strg& operator=(const string &s) {
        this->string::operator =(s);
        return (*this);
    }
    Strg& operator=(const char *s) {
        this->string::operator =(s);
        return (*this);
    }

};

extern bool isNumber(const string &str);
extern Intg toIntg(Strg s);
extern Strg toStrg(const Intg &i);

/**
 *
 * @param nonASCIICode code to transcode;
 * @param length  the nonASCIICode's length;
 * @return the code encoded by hex;
 */
string nonASCIICodeToHex(const char *nonASCIICode, unsigned int length) ;
string nonASCIICodeToHex(const char *nonASCIICode) ;

/**
 * Compiler Function Name Deformation
 */
class Compiler_CNAC {
public:
    Strg currentflushStrg;
    ofstream outputfstream;
    Strg filePath;
    void flush();
    void setOutputCodePath(const char *filePath) {
        this->filePath = Strg(filePath);
        outputfstream.close();
        outputfstream.open(filePath, ios::trunc);
        outputfstream.close();

        return;
    }
    Compiler_CNAC() {
    }
    void addToken(Intg type, const char *text) {

        switch (type) {
        case Token::CALL:
#if debug
        {
            static int ignorecount = 0;
            cout << "addToken(int, char*)" << " CALL" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "han_compiler_convert_non_ascii_character.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
            exit(-1);
            break;
        case Token::CHANGEABLE:
            currentflushStrg = Strg(text);
            break;
        case Token::PENDCHANGEABLE:
            currentflushStrg = Strg(text);
            break;
        case Token::CHENG:
            currentflushStrg = Strg(text);

            break;
        case Token::CHU:
            currentflushStrg = Strg(text);
            break;
        case Token::COMMA:
            currentflushStrg = Strg(text);
            break;
        case Token::DAYU:
            currentflushStrg = Strg(text);
            break;
        case Token::DENGYU:
            currentflushStrg = Strg(text);
            break;
        case Token::DOTDOT:
            currentflushStrg = Strg(text);
            break;
        case Token::FUZHI:
            currentflushStrg = Strg(text);

            break;
        case Token::HIGHEST:

            break;
        case Token::JIA:
            currentflushStrg = Strg(text);

            break;
        case Token::JIAN:
            currentflushStrg = Strg(text);

            break;
        case Token::LBCUR:
            currentflushStrg = Strg(text);
            break;
        case Token::LMCUR:
            currentflushStrg = Strg(text);

            break;
        case Token::LOWEST:

            break;
        case Token::LSCUR:

            currentflushStrg = Strg(text);

            break;
        case Token::MI:
            currentflushStrg = Strg(text);
            break;
        case Token::RBCUR:
            currentflushStrg = Strg(text);
            break;
        case Token::RMCUR:
            currentflushStrg = Strg(text);
            break;
        case Token::RSCUR:
            currentflushStrg = Strg(text);
            break;
        case Token::SEMICOLON:
            currentflushStrg = Strg(text);
            break;
        case Token::SHUCHU:
            currentflushStrg = Strg(text);
            break;
        case Token::XIAOYU:
            currentflushStrg = Strg(text);
            break;
        case Token::_return_:
            currentflushStrg = Strg(text);
            break;
        case Token::_return_high:

            break;
        case Token::_return_low:

            break;
        case Token::arg:
            currentflushStrg = Strg(text);
            break;
        case Token::attribute:
            currentflushStrg = Strg(text);
            break;
        case Token::changeable_attribute:
            currentflushStrg = Strg(text);
            break;
        case Token::function_declare_attribute:
            currentflushStrg = Strg(text);
            break;
        case Token::variable_specific_attribute:
            currentflushStrg = Strg(text);
            break;
        case Token::IF:
            currentflushStrg = Strg(text);
            break;
        case Token::ELSE:
            currentflushStrg = Strg(text);
            break;
        case Token::ELSEIF:
            currentflushStrg = Strg(text);
            break;
        case Token::ELIF:
            currentflushStrg = Strg(text);
            break;
        case Token::WHILE:
            currentflushStrg = Strg(text);
            break;

        default:
            break;
        }
        flush();

    }

};

}
/*
 *interface
 */

extern void setOutputCodePath(const char *filePath);
extern void addToken(Intg tokenType, const char *tokenString);
extern void flush();

#endif /* COOLANG_COMPILER_H_ */
