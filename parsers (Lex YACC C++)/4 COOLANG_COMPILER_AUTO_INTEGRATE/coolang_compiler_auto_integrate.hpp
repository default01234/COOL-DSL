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
#ifndef COOLANG_COMPILER_AUTO_INTEGRATE_HPP_
#define COOLANG_COMPILER_AUTO_INTEGRATE_HPP_

#include "coolang_compiler_define.h"

#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<string>
#include <sstream>
#include<fstream>
#include"rule_token_number.h"
#include<map>
#include<vector>
#include<deque>
#include<stack>
#include<list>

#define F_ false
#define T_ true

#define Intg int
#define Addr int
#define Strg std::string

#define default_organization 100
#define __line__ "#line"
#ifndef fileno
#define fileno(__F) ((__F)->_file)
#endif

using namespace std;
namespace COOLANG {

extern bool isNumber(const string &str);
extern Intg toIntg(Strg s);
extern Strg toStrg(const Intg &i);

class Arg {
public:
    Strg arg_s = "";
    Intg argFlag = 0;

    Intg changeable = F_;
    Arg():arg_s(""), argFlag(0) {

    }

    Arg(Intg flag, const Strg &arg_s):arg_s(""), argFlag(0) {
        argFlag = flag;
        this->arg_s = arg_s;
    }

    Strg toStrg() const {
        stringstream ss;
        ss << "argFlag:[" << argFlag << "]\t" << "arg_s:[" << arg_s << "]\t"
                << endl;

        return ss.str();
    }
    Arg& operator=(const Arg& other){
    this->argFlag = other.argFlag;
    this->arg_s = other.arg_s;
    return *this;
    }

};
class AssemblyCode {
public:
    Intg type = 0;

    Intg arg1Flag = 0;

    Intg arg2Flag = 0;

    Intg operatorFlag = 0;

    Intg resultFlag = 0;

    Strg arg1_s = "";

    Strg arg2_s = "";

    Strg operator_s = "";

    Strg result_s = "";

    Arg *arg1 = NULL;
    Arg *arg2 = NULL;
    Arg *argoperator = NULL;
    Arg *argresult = NULL;

    AssemblyCode(const Arg &arg1, const Arg &arg2, const Arg &argoperator,
            const Arg &argresult) {
        arg1Flag = arg1.argFlag;
        arg2Flag = arg2.argFlag;
        operatorFlag = argoperator.argFlag;
        resultFlag = argresult.argFlag;
        arg1_s = arg1.arg_s;
        arg2_s = arg2.arg_s;
        operator_s = argoperator.arg_s;
        result_s = argresult.arg_s;

    }
    AssemblyCode():type(0),arg1Flag(0),arg2Flag(0),operatorFlag(0),resultFlag(0),arg1_s(""),arg2_s(""),operator_s(""),result_s("") {
    }
    void setArg(const Intg argPosition, const Arg arg) {
        switch (argPosition) {
        case 0:
            arg1Flag = arg.argFlag;
            arg1_s = arg.arg_s;
            break;
        case 1:
            arg2Flag = arg.argFlag;
            arg2_s = arg.arg_s;
            break;
        case 2:
            operatorFlag = arg.argFlag;
            operator_s = arg.arg_s;
            break;
        case 3:
            resultFlag = arg.argFlag;
            result_s = arg.arg_s;
            break;
        default:
            break;
        }
    }
    Strg toStrg() const {
        stringstream ss;

        ss << type << COC_DELIMITER << arg1_s << COC_DELIMITER << arg2_s
                << COC_DELIMITER << operator_s << COC_DELIMITER << result_s
                << COC_DELIMITER << arg1Flag << COC_DELIMITER << arg2Flag
                << COC_DELIMITER << operatorFlag << COC_DELIMITER << resultFlag;

        return ss.str();
    }

    bool operator==(const AssemblyCode &asc) const {
        return type == asc.type && arg1Flag == asc.arg1Flag &&

        arg2Flag == asc.arg2Flag &&

        operatorFlag == asc.operatorFlag &&

        resultFlag == asc.resultFlag &&

        arg1_s == asc.arg1_s &&

        arg2_s == asc.arg2_s &&

        operator_s == asc.operator_s &&

        result_s == asc.result_s;

    }
    AssemblyCode & operator=(const AssemblyCode &asc) {
           type = asc.type ;

           arg1Flag = asc.arg1Flag ;

           arg2Flag = asc.arg2Flag ;

           operatorFlag = asc.operatorFlag ;

           resultFlag = asc.resultFlag ;

           arg1_s = asc.arg1_s ;

           arg2_s = asc.arg2_s ;

           operator_s = asc.operator_s ;

           result_s = asc.result_s;

           return *this;


       }

};
class Compiler {
public:
    Intg lastCodeType = 0;
    vector<Intg> heap;
    ofstream outputfstream;
    Intg lineNumber = 0;
    Intg tokenNumber = 0;
    map<Intg, Arg> token_arg_map;
    deque<AssemblyCode> output_code_buffer;
    Compiler() {

    }
    Compiler(const Strg &filePath) {
        outputfstream.open(filePath.c_str(), ios::trunc);
    }

    void setOutputCodePath(const char *filePath) {
        outputfstream.close();
        outputfstream.open(filePath, ios::trunc);

        return;
    }
    void outputAssemblyCode(AssemblyCode &as) {
#if debug
        cout << "Compiler::outputAssemblyCode in Strg:[" << as.toStrg() << "\t]"
                << endl;
#endif
        #if debug
        {
               static int ignorecount = 0;
               std::cout <<"("<<__FILE__<<":"<<__LINE__<<":0"<<")\toutputAssemblyCode(AssemblyCode&)"<< " in. as"
                       << "in Strg:[" << as.toStrg() << "\t]\tignorecount:["
                       << ignorecount++ << "\t]" << std::endl;
        }
        #endif
        if (as.arg1Flag == Dz && as.arg1_s == __line__) {
#if debug
            cout << "\t1" << endl;
#endif
            if (as.arg1) {
                if (as.arg1->arg_s != __line__) {
                    as.arg1_s = as.arg1->arg_s;
                } else {
                    as.arg1->arg_s = as.arg1_s = toStrg(lineNumber);
                }
            } else {
                cerr
                        << "Compiler::outputAssemblyCode err1,can't certain lineNumber,exit(-1)"
                        << endl;

            }
#if debug
            cout << "\t1.1" << endl;
#endif
        }
        if (as.arg2Flag == Dz && as.arg2_s == __line__) {
#if debug
            cout << "\t2" << endl;
#endif
            if (as.arg2) {
                if (as.arg2->arg_s != __line__) {
                    as.arg2_s = as.arg2->arg_s;
                } else {
                    as.arg2->arg_s = as.arg2_s = toStrg(lineNumber);
                }
            } else {
                cerr
                        << "Compiler::outputAssemblyCode err2,can't certain lineNumber,exit(-1)"
                        << endl;

            }
#if debug
            cout << "\t2.1" << endl;
#endif
        }
        if (as.operatorFlag == Dz && as.operator_s == __line__) {
#if debug
            cout << "\t3" << endl;
#endif
            if (as.argoperator) {
                if (as.argoperator->arg_s != __line__) {
                    as.operator_s = as.argoperator->arg_s;
                } else {
                    as.argoperator->arg_s = as.operator_s = toStrg(lineNumber);
                }
            } else {
                cerr
                        << "Compiler::outputAssemblyCode err operator,can't certain lineNumber,exit(-1)"
                        << endl;

            }
#if debug
            cout << "\t3.1" << endl;
#endif
        }
        if (as.resultFlag == Dz && as.result_s == __line__) {
#if debug
            cout << "\t4" << endl;
#endif
            if (as.argresult) {
                if (as.argresult->arg_s != __line__) {
                    as.result_s = as.argresult->arg_s;
                } else {
                    as.argresult->arg_s = as.result_s = toStrg(lineNumber);
                }
            } else {
                cerr
                        << "Compiler::outputAssemblyCode err result,can't certain lineNumber,exit(-1)"
                        << endl;

            }
#if debug
            cout << "\t4.1" << endl;
#endif
        }
#if debug
        if (as.arg1)
            cout << "\t" << as.arg1->toStrg();
#endif
#if debug
        if (as.arg2)
            cout << "\t" << as.arg2->toStrg();
#endif
#if debug
        if (as.argoperator)
            cout << "\t" << as.argoperator->toStrg();
#endif
#if debug
        if (as.argresult)
            cout << "\t" << as.argresult->toStrg();
#endif
        Strg &&asc = as.toStrg();
#if debug
        cout << "Compiler::outputAssemblyCode Strg end Strg:[" << asc << "\t]"
                << endl;
#endif
        outputfstream << asc << endl;
    }
    /**
     * @brief ������������ڽ�ɨ�赽�ı�ʶ��ת����arg��tokennumber��ɼ�ֵ�Դ���������?
     * createNew���ھ����Ƿ񴴽��µļ�ֵ�ԣ���������֮ǰ��ֵ�ԣ����뽫��ʼ����Ϊ1
     * @param createNew=1
     * @param tokenType
     * @param argFlag
     * @param tokenString
     * @return
     */
    Intg matchToken(bool createNew, Intg tokenType, Intg argFlag,
            const char *tokenString) {
#if debug
        cout << "in Compiler::matchToken:createNew:[" << createNew
                << "\t]tokenType:[" << tokenType << "\t]"
                        "argFlag:[" << argFlag << "\t]tokenString:["
                << tokenString << "\t]" << endl;
#endif

    if(createNew == false){
        return tokenNumber;
    }
        tokenNumber-=4;
        Strg tokenStrg(tokenString);
        Arg tokenArg(argFlag, tokenStrg);
        switch (tokenType) {
        case Token::CALL:
        tokenArg.arg_s = "call";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::_FIL:
            tokenArg.arg_s = "FIL";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::_LIN:
            tokenArg.arg_s = "LIN";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::_LOAD:
            tokenArg.arg_s = "LOAD";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::CLEAR:
            tokenArg.arg_s = "CLEAR";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::ERASE:
            tokenArg.arg_s = "ERASE";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::LENGTH:
            tokenArg.arg_s = "LENGTH";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::TYPENAME:
            tokenArg.arg_s = "TYPENAME";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::LIST:
            tokenArg.arg_s = "LIST";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::MAP:
            tokenArg.arg_s = "MAP";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::MULTIMAP:
            tokenArg.arg_s = "MULTIMAP";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::SET:
            tokenArg.arg_s = "SET";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::MULTISET:
            tokenArg.arg_s = "MULTISET";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::TOINT:
            tokenArg.arg_s = "TOINT";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::TOSTRG:
            tokenArg.arg_s = "TOSTRG";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::TONUM:
            tokenArg.arg_s = "TONUM";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::NONBLOCKEXECUTE:
            tokenArg.arg_s = "NONBLOCKEXECUTE";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::BLOCKEXECUTE:
            tokenArg.arg_s = "BLOCKEXECUTE";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::SLEEP:
            tokenArg.arg_s = "SLEEP";
            token_arg_map[tokenNumber] = tokenArg;
            break;
	case Token::LOGICJUMP:
            tokenArg.arg_s = "LOGICJUMP";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::CHANGEABLE:
        tokenArg.arg_s = "$";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::PENDCHANGEABLE:
        tokenArg.arg_s = "#";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::COMPATIBLE:
        tokenArg.arg_s = "?";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::CHENG:
        tokenArg.arg_s = "*";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::CHU:
        tokenArg.arg_s = "/";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::MODULO:
        tokenArg.arg_s = "%";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::AND:
        tokenArg.arg_s = "&&";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::OR:
        tokenArg.arg_s = "||";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::NOT:
        tokenArg.arg_s = "!";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::USER_OP1:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::USER_OP2:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::USER_OP3:
            token_arg_map[tokenNumber] = tokenArg;
            break;

        case Token::COMMA:
            tokenArg.arg_s = "COMMA";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::DAYU:
        tokenArg.arg_s = ">";
            token_arg_map[tokenNumber] = tokenArg;
            break;
    case Token::BUDAYU:
        tokenArg.arg_s = "<=";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::DENGYU:
        tokenArg.arg_s = "==";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::BUDENG:
        tokenArg.arg_s = "!=";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::TUIDAO:
        tokenArg.arg_s = "=>";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::DOTDOT:
        tokenArg.arg_s = ":";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::JICHENG:
            tokenArg.arg_s = "<<";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::FUZHI:
        tokenArg.arg_s = "=";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::HIGHEST:
        tokenArg.arg_s = "";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::JIA:
        tokenArg.arg_s = "+";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::DOT:
        tokenArg.arg_s = ".";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::JIAN:
        tokenArg.arg_s = "-";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::LBCUR:
        tokenArg.arg_s = "{";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::LMCUR:
        tokenArg.arg_s = "[";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::LOWEST:
        tokenArg.arg_s = "";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::LSCUR:
        tokenArg.arg_s = "(";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::MI:
        tokenArg.arg_s = "^";
            token_arg_map[tokenNumber] = tokenArg;
            break;
    case Token::JIADENG:
        tokenArg.arg_s = "+=";
            token_arg_map[tokenNumber] = tokenArg;
            break;
    case Token::JIANDENG:
        tokenArg.arg_s = "-=";
            token_arg_map[tokenNumber] = tokenArg;
            break;
    case Token::CHENGDENG:
            tokenArg.arg_s = "*=";
            token_arg_map[tokenNumber] = tokenArg;
            break;
    case Token::CHUDENG:
            tokenArg.arg_s = "/=";
            token_arg_map[tokenNumber] = tokenArg;
            break;
    case Token::MODENG:
            tokenArg.arg_s = "%=";
            token_arg_map[tokenNumber] = tokenArg;
            break;
    case Token::MIDENG:
            tokenArg.arg_s = "^=";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::RBCUR:
        tokenArg.arg_s = "}";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::RMCUR:
        tokenArg.arg_s = "]";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::RSCUR:
        tokenArg.arg_s = ")";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::SEMICOLON:
        tokenArg.arg_s = ";";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::SHUCHU:
        tokenArg.arg_s = "-->";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::XIAOYU:
        tokenArg.arg_s = "<";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::_return_:
        tokenArg.arg_s = "return";
            token_arg_map[tokenNumber] = tokenArg;
            break;
    case Token::_abort_:
            tokenArg.arg_s = "abort";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::_return_high:
        tokenArg.arg_s = "";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::_return_low:
        tokenArg.arg_s = "";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::arg:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::attribute:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::changeable_attribute:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::function_declare_attribute:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::variable_specific_attribute:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::IF:
        tokenArg.arg_s = "if";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::ELSE:
        tokenArg.arg_s = "else";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::ELSEIF:
        tokenArg.arg_s = "elif";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::ELIF:
        tokenArg.arg_s = "elif";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::WHILE:
        tokenArg.arg_s = "while";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::CONTINUE:
        tokenArg.arg_s = "continue";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::BREAK:
        tokenArg.arg_s = "break";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::SI:
        tokenArg.arg_s = "++";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::SD:
        tokenArg.arg_s = "--";
            token_arg_map[tokenNumber] = tokenArg;
            break;

        default:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        }


#if debug
        cout << "in Compiler::matchToken: out tokenNumber:[" << tokenNumber
                << "\t]tokenType:[" << tokenType << "\t]"
                        "argFlag:[" << argFlag << "\t]tokenString:["
                << tokenString << "\t]" << endl;
#endif

        return tokenNumber;

    }
    /**
     * @brief �˺������ڴ����ַ��룬����createNew���ڱ����Ƿ�Ҫ�����µ��ַ���
     * @param createNew
     * @param ruleType
     * @param tokenNumber1  arg1
     * @param tokenNumber2  arg2
     * @param tokenNumber3  argop
     * @param tokenNumber4  res
     */
    void matchRule(bool createNew, Intg ruleType, Intg *tokenNumber1,
    Intg *tokenNumber2, Intg *tokenNumber3, Intg *tokenNumber4) {
#if debug
        cout << "in Compiler::matchRule:createNew:[" << createNew
                << "\t]ruleType:[" << ruleType << "\t]"
                        "tokenNumber1:["
                << toStrg(tokenNumber1 ? (*tokenNumber1) : -65536) << "\t]"
                << "tokenNumber2:["
                << toStrg(tokenNumber2 ? (*tokenNumber2) : -65536) << "\t]"
                << "tokenNumber3:["
                << toStrg(tokenNumber3 ? (*tokenNumber3) : -65536) << "\t]"
                << "tokenNumber4:["
                << toStrg(tokenNumber4 ? (*tokenNumber4) : -65536) << "\t]";
#endif
        #if debug
        {
               static int ignorecount = 0;
               std::cout <<"("<<__FILE__<<":"<<__LINE__<<":0"<<")\tmatchRule(bool, int, int*, int*, int*, int*) in"<< " "
                       << "\tignorecount:["
                       << ignorecount++ << "\t]" << std::endl;
        }
        #endif
        AssemblyCode as;
        Arg *&arg1 = as.arg1;
        Arg *&arg2 = as.arg2;
        Arg *&argoperator = as.argoperator;
        Arg *&argresult = as.argresult;
#if debug
        cout << "in Compiler::matchRule 1" << endl;
#endif
        if (tokenNumber1
                && (token_arg_map.find(*tokenNumber1) != token_arg_map.end())) {
#if debug
            cout << "in Compiler::matchRule arg1!=NULL" << endl;
#endif
            arg1 = &token_arg_map[*tokenNumber1];
        }
        if (tokenNumber2
                && (token_arg_map.find(*tokenNumber2) != token_arg_map.end())) {
#if debug
            cout << "in Compiler::matchRule arg2!=NULL" << endl;
#endif
            arg2 = &token_arg_map[*tokenNumber2];
        }
        if (tokenNumber3
                && (token_arg_map.find(*tokenNumber3) != token_arg_map.end())) {
#if debug
            cout << "in Compiler::matchRule argoperator!=NULL" << endl;
#endif
            argoperator = &token_arg_map[*tokenNumber3];
        }
        if (tokenNumber4
                && (token_arg_map.find(*tokenNumber4) != token_arg_map.end())) {
#if debug
            cout << "in Compiler::matchRule argresult!=NULL" << endl;
#endif
            argresult = &token_arg_map[*tokenNumber4];
        }
#if debug
        cout << "in Compiler::matchRule 2" << endl;
#endif

#if debug
        cout << "in Compiler::matchRule 3" << endl;
#endif
        switch (ruleType) {
        case Rule::_arg____CHANGEABLE_arg:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "_arg____CHANGEABLE_arg" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::_arg____PENDCHANGEABLE_arg:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "_arg____PENDCHANGEABLE_arg" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::_arg____COMPATIBLE_arg:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "_arg____COMPATIBLE_arg" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::_arg____arg:
#if debug
            cout << "in Compiler::matchRule switch:[" << "_arg____arg" << "]"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);

            break;
        case Rule::_arg____variable_specific_attribute_DOTDOT__arg_:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "_arg____variable_specific_attribute_DOTDOT__arg_" << "]"
                    << endl;
#endif

            (*tokenNumber4) = (*tokenNumber1);
            if (arg2) {

                if (arg2->arg_s == "new" || arg2->arg_s == "symbol" || arg2->arg_s == "placeholder") {
                    as.type = C;
                } else {

                    as.type = M;
                }
            } else {
#if debug
                cout << "\terr:variable_specific_attribute null,exit(-1)"
                        << endl;
#endif
                //exit(-1);
            }

            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
    case Rule::variable_specific_attribute___variable_specific_attribute_LSCUR_variable_specific_attribute_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "variable_specific_attribute___variable_specific_attribute_LSCUR_variable_specific_attribute_RSCUR" << "]"
                    << endl;
#endif

        (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "attr_call"));
            as.setArg(3, *argresult);
            break;
        case Rule::code___code_environment:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "code___code_environment" << "]" << endl;
#endif
//            flushMap();

            break;
        case Rule::code___environment:
#if debug
            cout << "in Compiler::matchRule switch:[" << "code___environment"
                    << "]" << endl;
#endif
//            flushMap();

            break;
        case Rule::environment___functionImplement:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "environment___functionImplement" << "]" << endl;
#endif
//            flushMap();

            break;
        case Rule::environment___scope:
#if debug
            cout << "in Compiler::matchRule switch:[" << "environment___scope"
                    << "]" << endl;
#endif
//            flushMap();

            break;
        case Rule::environment___sentence:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "environment___sentence" << "]" << endl;
#endif
//            flushMap();

            break;
        case Rule::environment_null___NULL:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "environment_null___NULL" << "]" << endl;
#endif
//            flushMap();

            break;

        case Rule::expression___LSCUR_expression_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___LSCUR_expression_RSCUR" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);

            break;
        case Rule::expression____arg_:
#if debug
            cout << "in Compiler::matchRule switch:[" << "expression____arg_"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::sentence____return__SEMICOLON:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "sentence____return__SEMICOLON" << "]" << endl;
#endif

            as.type = R;
            break;
        case Rule::sentence____return__expression_SEMICOLON:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "sentence____return__expression_SEMICOLON" << "]"
                    << endl;
#endif

            as.type = R;
            as.setArg(0, *arg1);

            break;
    case Rule::sentence____abort__SEMICOLON:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "sentence____abort__SEMICOLON" << "]" << endl;
#endif

            as.type = ABT;
            break;
        case Rule::sentence___BREAK_SEMICOLON:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "sentence___BREAK_SEMICOLON" << "]" << endl;
#endif

            as.type = BRK;
            break;
        case Rule::sentence___CONTINUE_SEMICOLON:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "sentence___CONTINUE_SEMICOLON" << "]" << endl;
#endif

            as.type = CTN;
            break;
        case Rule::expression___expression_CHENG_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_CHENG_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_CHU_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_CHU_expression" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_COMMA_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_COMMA_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_DENGYU_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_DENGYU_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_BUDENG_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_BUDENG_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_DAYU_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_DAYU_expression" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
    case Rule::expression___expression_BUDAYU_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_BUDAYU_expression" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_XIAOYU_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_XIAOYU_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
    case Rule::expression___expression_BUXIAOYU_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_BUXIAOYU_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_DOTDOT_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_DOTDOT_expression" << "]"
                    << endl;
#endif
#if debug
            cout
                    << "HAN_COMPILER::Compiler::matchRule::expression___expression_DOTDOT_expression"
                    << endl;
#endif

            //Ĭ����һ��Ϊ����system��ʵ�������������иı�
            (*tokenNumber4) = (*tokenNumber1);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = C;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expression___expression_FUZHI_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_FUZHI_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expression___expression_JIADENG_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_JIADENG_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expression___expression_JIANDENG_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_JIANDENG_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expression___expression_CHENGDENG_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_CHENGDENG_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expression___expression_CHUDENG_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_CHUDENG_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expression___expression_MODENG_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_MODENG_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expression___expression_MIDENG_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_MIDENG_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expression___expression_JIAN_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_JIAN_expression" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___JIAN_expression:

            if (isNumber(arg2->arg_s) == true) {
#if debug
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIAN_expression" << "],createNew == F"
                        << endl;
#endif
                createNew = false;
                (*tokenNumber4) = *tokenNumber2;
                token_arg_map[(*tokenNumber4)].arg_s =
                        token_arg_map[(*tokenNumber3)].arg_s
                                + token_arg_map[(*tokenNumber4)].arg_s;
                argresult = &token_arg_map[(*tokenNumber4)];

            } else {
#if debug
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIAN_expression" << "],createNew == T"
                        << endl;
#endif
                createNew = true;
                (*tokenNumber4) = --tokenNumber;
                token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
                argresult = &token_arg_map[(*tokenNumber4)];
                as.type = M;
                as.setArg(0, Arg(Sz, "0"));
                as.setArg(1, *arg2);
                as.setArg(2, *argoperator);
                as.setArg(3, *argresult);
            }
            break;
        case Rule::expression___JIA_expression:

            if (isNumber(arg2->arg_s) == true) {
#if debug
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIA_expression" << "],createNew == F"
                        << endl;
#endif
                createNew = false;
                (*tokenNumber4) = *tokenNumber2;
                token_arg_map[(*tokenNumber4)].arg_s =
                        token_arg_map[(*tokenNumber3)].arg_s
                                + token_arg_map[(*tokenNumber4)].arg_s;
                argresult = &token_arg_map[(*tokenNumber4)];

            } else {
#if debug
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIA_expression" << "],createNew == T"
                        << endl;
#endif
                createNew = true;
                (*tokenNumber4) = --tokenNumber;
                token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
                argresult = &token_arg_map[(*tokenNumber4)];
                as.type = M;
                as.setArg(0, Arg(Sz, "0"));
                as.setArg(1, *arg2);
                as.setArg(2, *argoperator);
                as.setArg(3, *argresult);
            }
            break;
        case Rule::expression___NOT_expression:

#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___NOT_expression" << "],createNew == T"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);

            break;
        case Rule::expression___USER_OP1_expression:

#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___USER_OP1_expression" << "],createNew == T"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);

            break;
        case Rule::expression___USER_OP2_expression:

#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___USER_OP2_expression" << "],createNew == T"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);

            break;
        case Rule::expression___SI_expression:

#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___SI_expression" << "],createNew == T"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);

            break;
        case Rule::expression___SD_expression:

#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___SD_expression" << "],createNew == T"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);

            break;
        case Rule::expression___expression_USER_OP3:

#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_USER_OP3" << "],createNew == T"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);

            break;
        case Rule::expression___expression_SI:

#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_SI" << "],createNew == T"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);

            break;
        case Rule::expression___expression_SD:

#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_SD" << "],createNew == T"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);

            break;

        case Rule::expression___expression_AND_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_AND_expression" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_OR_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_OR_expression" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_USER_OP1_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_USER_OP1_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_USER_OP2_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_USER_OP2_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;

        case Rule::expression___expression_JIA_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_JIA_expression" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_MODULO_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_MODULO_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___LIST_LSCUR_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___LIST_LSCUR_RSCUR" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___MAP_LSCUR_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___MAP_LSCUR_RSCUR" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___MULTIMAP_LSCUR_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___MULTIMAP_LSCUR_RSCUR" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___SET_LSCUR_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___SET_LSCUR_RSCUR" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___MULTISET_LSCUR_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___MULTISET_LSCUR_RSCUR" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_DOT_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_DOT_expression" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = Y;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_LSCUR_expression_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_LSCUR_expression_RSCUR" << "]"
                    << endl;
#endif
#if debug
            {
                static int ignorecount = 0;
#if debug
                cout << "matchRule(bool, int, int*, int*, int*, int*)" << " "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "han_compiler.hpp:" << __LINE__ << ":0" << ")"
                        << endl;
#endif
            }
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "call"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_LSCUR_expression_null_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_LSCUR_expression_null_RSCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);

            as.setArg(2, Arg(Bs, "call"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_MI_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_MI_expression" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_SHUCHU_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_SHUCHU_expression" << "]"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expression_null___NULL:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression_null___NULL" << "]" << endl;
#endif

            break;

        case Rule::scopeBegin___LBCUR:
#if debug
            cout << "in Compiler::matchRule switch:[" << "scopeBegin___LBCUR"
                    << "]" << endl;
#endif

        (*tokenNumber4) = (*tokenNumber1);
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
        arg1 = NULL;
            as.type = QS;
            as.setArg(3, *argresult);//the input arg is exactly the result arg. just set it at the result position of the asc. clear the input arg.
            break;
        case Rule::scopeHalf___scopeBegin_sentence:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeBegin_sentence" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeHalf___scopeHalf_functionDeclare:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeHalf_functionDeclare" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeHalf___scopeHalf_functionImplement:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeHalf_functionImplement" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeHalf___scopeHalf_scope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeHalf_scope" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeHalf___scopeHalf_sentence:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeHalf_sentence" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scope___scopeHalf_RBCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scope___scopeHalf_RBCUR" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            arg1 = NULL;
        argresult = &token_arg_map[(*tokenNumber4)];
            as.type = QE;
            as.setArg(3, *argresult);
            break;
        case Rule::scope___scopeBegin_RBCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scope___scopeBegin_RBCUR" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            arg1 = NULL;
        argresult = &token_arg_map[(*tokenNumber4)];
            as.type = QE;
            as.setArg(3, *argresult);
            break;


        case Rule::expr_scope_begin___LBCUR:
#if debug
            cout << "in Compiler::matchRule switch:[" << "expr_scope_begin___LBCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
        arg1 = NULL;
            as.type = EXPRQS;
            as.setArg(3, *argresult);//the input arg is exactly the result arg. just set it at the result position of the asc. clear the input arg.
            break;
        case Rule::expr_scope_half___expr_scope_begin_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expr_scope_half___expr_scope_begin_expression" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::expr_scope___expr_scope_half_RBCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expr_scope___expr_scope_half_RBCUR" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            arg1 = NULL;
        argresult = &token_arg_map[(*tokenNumber4)];
            as.type = EXPRQE;
            as.setArg(3, *argresult);
            break;

            /*������������*/
        case Rule::expressionFunctionDeclareRoot___JIAN_expression:

            if (isNumber(arg2->arg_s) == true) {
#if debug
                cout << "in Compiler::matchRule switch:["
                        << "expressionFunctionDeclareRoot___JIAN_expression"
                        << "],createNew == F" << endl;
#endif
                createNew = false;
                (*tokenNumber4) = *tokenNumber2;
                token_arg_map[(*tokenNumber4)].arg_s =
                        token_arg_map[(*tokenNumber3)].arg_s
                                + token_arg_map[(*tokenNumber4)].arg_s;
                argresult = &token_arg_map[(*tokenNumber4)];

            } else {
#if debug
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIAN_expression" << "],createNew == T"
                        << endl;
#endif
                createNew = true;
                (*tokenNumber4) = --tokenNumber;
                token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
                argresult = &token_arg_map[(*tokenNumber4)];
                as.type = M;
                as.setArg(0, Arg(Sz, "0"));
                as.setArg(1, *arg2);
                as.setArg(2, *argoperator);
                as.setArg(3, *argresult);
            }
            break;
        case Rule::expressionFunctionDeclareRoot___JIA_expression:

            if (isNumber(arg2->arg_s) == true) {
#if debug
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIA_expression" << "],createNew == F"
                        << endl;
#endif
                createNew = false;
                (*tokenNumber4) = *tokenNumber2;
                token_arg_map[(*tokenNumber4)].arg_s =
                        token_arg_map[(*tokenNumber3)].arg_s
                                + token_arg_map[(*tokenNumber4)].arg_s;
                argresult = &token_arg_map[(*tokenNumber4)];

            } else {
#if debug
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIA_expression" << "],createNew == T"
                        << endl;
#endif
                createNew = true;
                (*tokenNumber4) = --tokenNumber;
                token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
                argresult = &token_arg_map[(*tokenNumber4)];
                as.type = M;
                as.setArg(0, Arg(Sz, "0"));
                as.setArg(1, *arg2);
                as.setArg(2, *argoperator);
                as.setArg(3, *argresult);
            }
            break;
        case Rule::expressionFunctionDeclareRoot___LSCUR_expressionFunctionDeclareRoot_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___LSCUR_expressionFunctionDeclareRoot_RSCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_CHENG_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_CHENG_expression"
                    << "]" << endl;
#endif

            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_CHU_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_CHU_expression"
                    << "]" << endl;
#endif

            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_COMMA_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_COMMA_expression"
                    << "]" << endl;
#endif

            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_DENGYU_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_DENGYU_expression"
                    << "]" << endl;
#endif

            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_DAYU_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_DAYU_expression"
                    << "]" << endl;
#endif

            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_XIAOYU_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_XIAOYU_expression"
                    << "]" << endl;
#endif

            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_DOTDOT_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_DOTDOT_expression"
                    << "]" << endl;
#endif

#if debug
            cout
                    << "HAN_COMPILER::Compiler::matchRule::expressionFunctionDeclareRoot___expression_DOTDOT_expression"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_FUZHI_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_FUZHI_expression"
                    << "]" << endl;
#endif
#if debug
            cout
                    << "HAN_COMPILER::Compiler::matchRule::expressionFunctionDeclareRoot___expression_FUZHI_expression"
                    << endl;
#endif

            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_JIAN_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_JIAN_expression"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_JIA_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_JIA_expression"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_LSCUR_expression_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_LSCUR_expression_RSCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "call"));
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_LSCUR_expression_null_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_LSCUR_expression_null_RSCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);

            as.setArg(2, Arg(Bs, "call"));
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_MI_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_MI_expression"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_SHUCHU_expression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_SHUCHU_expression"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;

        case Rule::scopeFunctionDeclareBegin___LBCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareBegin___LBCUR" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
        arg1 = NULL;
            as.type = QS;
            as.setArg(3, *argresult);//the input arg is exactly the result arg. just set it at the result position of the asc. clear the input arg.
            break;
        case Rule::scopeFunctionDeclareHalf___scopeFunctionDeclareBegin_sentence:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareHalf___scopeFunctionDeclareBegin_sentence"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeFunctionDeclareHalf___scopeFunctionDeclareHalf_sentence:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareHalf___scopeFunctionDeclareHalf_sentence"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot_SRMICOLON:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot_SRMICOLON"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot_SRMICOLON:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot_SRMICOLON"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeFunctionDeclare___scopeFunctionDeclareEnd_RBCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclare___scopeFunctionDeclareEnd_RBCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            arg1 = NULL;
        argresult = &token_arg_map[(*tokenNumber1)];
            as.type = QE;
            as.setArg(3, *argresult);
            break;

        case Rule::autoScopeBegin___NULL:
#if debug
            cout << "in Compiler::matchRule switch:[" << "autoScopeBegin___NULL"
                    << "]" << endl;
#endif
#if debug
            cout << "in Compiler::matchRule::autoScopeBegin___NULL 1" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
#if debug
            cout << "in Compiler::matchRule::autoScopeBegin___NULL 2" << endl;
#endif
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
#if debug
            cout << "in Compiler::matchRule::autoScopeBegin___NULL 3" << endl;
#endif
            as.type = QS;
            as.setArg(3, token_arg_map[(*tokenNumber4)]);
#if debug
            cout << "in Compiler::matchRule::autoScopeBegin___NULL 4" << endl;
#endif
            break;
        case Rule::autoScopeEnd___NULL:
#if debug
            cout << "in Compiler::matchRule switch:[" << "autoScopeEnd___NULL"
                    << "]" << endl;
#endif
            //do nothing
            break;

        case Rule::autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot_SEMICOLON:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot_SEMICOLON"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;

        case Rule::autoScope___autoScopeHalf_autoScopeEnd:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "autoScope___autoScopeHalf_autoScopeEnd" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            arg1 = NULL;
        argresult = &token_arg_map[(*tokenNumber1)];
            as.type = QE;
            as.setArg(3, *argresult);
            break;

        case Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_autoScope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareBegin___function_declare_attribute_DOTDOT_autoScope"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = QE;
            as.setArg(0, token_arg_map[(*tokenNumber4)]);
            break;
        case Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_scopeFunctionDeclare:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareBegin___function_declare_attribute_DOTDOT_scopeFunctionDeclare"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = F;
            if (arg2->argFlag == Sz) {
                argoperator->arg_s = "@";
            }
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::functionDeclareBegin___function_declare_attribute_scopeFunctionDeclare:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareBegin___function_declare_attribute_scopeFunctionDeclare"
                    << "]" << endl;
#endif
            //todo: ��Ҫ�޸�
            (*tokenNumber4) = (*tokenNumber1);
          if (arg2 && arg2->argFlag == Bs) {
                as.type = F;
                as.setArg(0, *arg1);
                as.setArg(1, *arg2);
                as.setArg(2, Arg(Bs, "@"));
                as.setArg(3, *arg1);
            }

            break;
        case Rule::functionDeclareBegin___function_declare_attribute_autoscope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareBegin___function_declare_attribute_autoscope"
                    << "]" << endl;
#endif
#if debug
            cout
                    << "HAN_COMPILER::Compiler::matchRule::functionDeclare___function_declare_attribute_autoscope"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = F;
            as.setArg(0, *arg1);
            as.setArg(1, Arg(Sz, toStrg(default_organization)));
            as.setArg(2, Arg(Bs, "@"));
            as.setArg(3, *arg1);
            break;
        case Rule::functionDeclareBegin___function_declare_attribute_functionDeclareBegin:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareBegin___function_declare_attribute_functionDeclareBegin"
                    << "]" << endl;
#endif

            (*tokenNumber4) = (*tokenNumber1);
            as.type = F;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "@"));
            as.setArg(3, *arg1);
            break;
        case Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_functionDeclareBegin:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareBegin___function_declare_attribute_DOTDOT_functionDeclareBegin"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = F;
            if (arg2->argFlag == Sz) {
                argoperator->arg_s = "@";
            }
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;

        case Rule::functionDeclareHalf___functionDeclareBegin_JICHENG_arg:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareHalf___functionDeclareBegin_JICHENG_arg"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            argoperator->arg_s = "<<";
            as.type = S;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::functionDeclare___functionDeclareBegin:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclare___functionDeclareBegin" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::functionDeclare___functionDeclareHalf:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclare___functionDeclareHalf" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::functionImplement___functionDeclare_scope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "functionImplement___functionDeclare_scope" << "]"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = F;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(3, *arg1);
            break;
        case Rule::functionImplement___functionImplement_TUIDAO_functionDeclare:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "functionImplement___functionImplement_TUIDAO_functionDeclare"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = TD;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;

        case Rule::sentence___SEMICOLON:
#if debug
            cout << "in Compiler::matchRule switch:[" << "sentence___SEMICOLON"
                    << "]" << endl;
#endif
            as.type = ME;
            break;
        case Rule::sentence___expression_SEMICOLON:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "sentence___expression_SEMICOLON" << "]" << endl;
#endif
            as.type = ME;
            break;
        case Rule::sentence___functionDeclare_SEMICOLON:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "sentence___functionDeclare_SEMICOLON" << "]" << endl;
#endif
            as.type = ME;
            break;
        case Rule::sentence___functionImplement:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "sentence___functionImplement" << "]" << endl;
#endif
            as.type = ME;
            break;
        case Rule::sentence___systemImplement:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "sentence___systemImplement" << "]" << endl;
#endif
            as.type = ME;
            break;
        case Rule::sentence___branch:
#if debug
            cout << "in Compiler::matchRule switch:[" << "sentence___branch"
                    << "]" << endl;
#endif
            as.type = ME;
            break;
        case Rule::sentence___loop:
#if debug
            cout << "in Compiler::matchRule switch:[" << "sentence___loop"
                    << "]" << endl;
#endif
            as.type = ME;
            break;
        case Rule::sentence_null___NULL:
#if debug
            cout << "in Compiler::matchRule switch:[" << "sentence_null___NULL"
                    << "]" << endl;
#endif
            as.type = ME;
            break;

        case Rule::branchBegin___IF_LSCUR_expression_RSCUR_autoScope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "branchBegin___IF_LSCUR_expression_RSCUR_autoScope"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, Arg(Sz, "0"));
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchBegin___IF_LSCUR_expression_RSCUR_scope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "branchBegin___IF_LSCUR_expression_RSCUR_scope" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, Arg(Sz, "0"));
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_autoScope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_autoScope"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_scope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_scope"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchEnd___branchBegin_ELSE__autoScope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "branchEnd___branchBegin_ELSE__autoScope" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, *arg1);
            as.setArg(1, Arg(Sz, "1"));
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchEnd___branchBegin_ELSE__scope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "branchEnd___branchBegin_ELSE__scope" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, *arg1);
            as.setArg(1, Arg(Sz, "1"));
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchEnd___branchHalf_ELSE__autoScope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "branchEnd___branchHalf_ELSE__autoScope" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, *arg1);
            as.setArg(1, Arg(Sz, "1"));
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchEnd___branchHalf_ELSE__scope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "branchEnd___branchHalf_ELSE__scope" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, *arg1);
            as.setArg(1, Arg(Sz, "1"));
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branch___branchBegin:
#if debug
            cout << "in Compiler::matchRule switch:[" << "branch___branchBegin"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);

            break;
        case Rule::branch___branchHalf:
#if debug
            cout << "in Compiler::matchRule switch:[" << "branch___branchHalf"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);

            break;
        case Rule::branch___branchEnd:
#if debug
            cout << "in Compiler::matchRule switch:[" << "branch___branchEnd"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);

            break;
        case Rule::loop___WHILE_LSCUR_expression_RSCUR_autoScope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "loop___WHILE_LSCUR_expression_RSCUR_autoScope" << "]"
                    << endl;
#endif

            (*tokenNumber1) = --tokenNumber;

            token_arg_map[(*tokenNumber1)] = Arg(Dz, __line__);
            arg1 = &token_arg_map[(*tokenNumber1)];
            argresult = arg1;
            as.type = L;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);

            break;
        case Rule::loop___WHILE_LSCUR_expression_RSCUR_scope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "loop___WHILE_LSCUR_expression_RSCUR_scope" << "]"
                    << endl;
#endif

            (*tokenNumber1) = --tokenNumber;

            token_arg_map[(*tokenNumber1)] = Arg(Dz, __line__);
            arg1 = &token_arg_map[(*tokenNumber1)];
            argresult = arg1;
            as.type = L;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);

            break;
            //matchRule(1, Rule::systemDeclareBegin___system_declare_attribute_DOTDOT_arg,&$3,&$1,&$2,&$$)
        case Rule::systemDeclareBegin___system_declare_attribute_DOTDOT_arg:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "systemDeclareBegin___system_declare_attribute_DOTDOT_arg"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = S;
            as.setArg(0, *arg1);
            as.setArg(1, Arg(Bs, "system"));
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::systemDeclareHalf___systemDeclareBegin_JICHENG_arg:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "systemDeclareHalf___systemDeclareBegin_JICHENG_arg"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            argoperator->arg_s = "<<";
            as.type = S;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::systemDeclare___systemDeclareBegin:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "systemDeclare___systemDeclareBegin" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::systemDeclare___systemDeclareHalf:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "systemDeclare___systemDeclareHalf" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::systemImplement___systemDeclare_scope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "systemImplement___systemDeclare_scope" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = S;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(3, *arg1);
            break;
        case Rule::environment___systemImplement:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "environment___systemImplement" << "]" << endl;
#endif
            flushMap();
            break;
        case Rule::sentence___systemDeclare_SEMICOLON:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "sentence___systemDeclare_SEMICOLON" << "]" << endl;
#endif
            as.type = ME;
            break;
        case Rule::scopeHalf___scopeHalf_systemImplement:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeHalf_systemImplement" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeHalf___scopeHalf_systemDeclare:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeHalf_systemDeclare" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;

        case Rule::expressionForList___strictExpression:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expressionForList___strictExpression" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;

        case Rule::expression___expression_LMCUR_listAccess_RMCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_LMCUR_listAccess_RMCUR" << "]"
                    << endl;
#endif
#if debug
            {
                static int ignorecount = 0;
#if debug
                cout << "matchRule(bool, int, int*, int*, int*, int*)" << " "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "han_compiler.hpp:" << __LINE__ << ":0" << ")"
                        << endl;
#endif
            }
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "[]"));
            as.setArg(3, *argresult);
            break;

        case Rule::listBegin___LBCUR:
#if debug
            cout << "in Compiler::matchRule switch:[" << "listBegin___LBCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
             token_arg_map[(*tokenNumber1)] = Arg(Dz, __line__);
             arg1 = argresult = &token_arg_map[(*tokenNumber1)];
             as.type = LST;
             as.setArg(0, *argresult);
             as.setArg(2, Arg(Bs, "{null}"));
             as.setArg(3, *argresult);
             break;

        case Rule::listHalf___listBegin_expressionForList:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "listHalf___listBegin_expressionForList" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = LST;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "{}"));
            as.setArg(3, *arg1);
            break;
        case Rule::list___listBegin_listEnd:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "list___listBegin_listEnd" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::list___listHalf_listEnd:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "list___listHalf_listEnd" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::listAccessBegin___NULL:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "listAccessBegin___NULL" << "]" << endl;
#endif
#if debug
            cout << "in Compiler::matchRule::listAccessBegin___NULL 1" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
#if debug
            cout << "in Compiler::matchRule::listAccessBegin___NULL 2" << endl;
#endif
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            arg1 = argresult = &token_arg_map[(*tokenNumber4)];
#if debug
            cout << "in Compiler::matchRule::listAccessBegin___NULL 3" << endl;
#endif
            as.type = LST;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "{null}"));
            as.setArg(3, *argresult);
#if debug
            cout << "in Compiler::matchRule::listAccessBegin___NULL 4" << endl;
#endif
            break;
        case Rule::listAccessHalf___listAccessBegin_expressionForListAccess:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "listAccessHalf___listAccessBegin_expressionForListAccess"
                    << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            as.type = LST;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "{}"));
            as.setArg(3, *arg1);
            break;

        case Rule::listAccess___listAccessBegin_listAccessEnd:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "listAccess___listAccessBegin_listAccessEnd" << "]"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::listAccess___listAccessHalf_listAccessEnd:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "listAccess___listAccessHalf_listAccessEnd" << "]"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);
            break;

        case Rule::_arg____list:
#if debug
            cout << "in Compiler::matchRule switch:[" << "_arg____list" << "]"
                    << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);

            break;

        case Rule::expression___expression_CLEAR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_CLEAR" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "CLEAR"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_POPBACK_LSCUR_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_POPBACK_LSCUR_RSCUR" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "POPBACK"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_POPFRONT_LSCUR_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_POPFRONT_LSCUR_RSCUR" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "POPFRONT"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_BACK_LSCUR_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_BACK_LSCUR_RSCUR" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "BACK"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_FIND_LSCUR_expression_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_FIND_LSCUR_expression_RSCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "FIND"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_COUNT_LSCUR_expression_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_COUNT_LSCUR_expression_RSCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "COUNT"));
            as.setArg(3, *argresult);
            break;


    case Rule::expression___EXPR_expr_scope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___EXPR_expr_scope" << "]" << endl;
#endif
            (*tokenNumber4) = (*tokenNumber1);

            break;

        case Rule::expression___expression_EXIST_SUBEXPR_expr_scope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_EXIST_SUBEXPR_expr_scope" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "EXIST_SUBEXPR"));
            as.setArg(3, *argresult);
            break;
    case Rule::expression___expression_FIND_SUBEXPR_expr_scope:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_FIND_SUBEXPR_expr_scope" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "FIND_SUBEXPR"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___RESET:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___RESET" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "RESET"));
            as.setArg(3, *argresult);
            break;




        case Rule::expression___expression_PUSHBACK_LSCUR_expression_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_PUSHBACK_LSCUR_expression_RSCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "PUSHBACK"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_PUSHFRONT_LSCUR_expression_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_PUSHFRONT_LSCUR_expression_RSCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "PUSHFRONT"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_ERASE_LSCUR_listAccess_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_ERASE_LSCUR_listAccess_RSCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "ERASE"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_INSERT_LSCUR_expression_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_INSERT_LSCUR_listAccess_RSCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "INSERT"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_LENGTH:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_LENGTH" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "LENGTH"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_TYPENAME:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_TYPENAME" << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "TYPENAME"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_TOINT_LSCUR_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_TOINT_LSCUR_RSCUR" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "TOINT"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_TONUM_LSCUR_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_TONUM_LSCUR_RSCUR" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "TONUM"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_TOSTRG_LSCUR_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_TOSTRG_LSCUR_RSCUR" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "TOSTRG"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___NONBLOCKEXECUTE_LSCUR_expression_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___NONBLOCKEXECUTE_LSCUR_expression_RSCUR"
                    << "]" << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "NONBLOCKEXECUTE"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___BLOCKEXECUTE_LSCUR_expression_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___BLOCKEXECUTE_LSCUR_expression_RSCUR" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "BLOCKEXECUTE"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___SLEEP_LSCUR_expression_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___SLEEP_LSCUR_expression_RSCUR" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "SLEEP"));
            as.setArg(3, *argresult);
            break;

	    case Rule::expression___LOGICJUMP_LSCUR_expression_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "expression___LOGICJUMP_LSCUR_expression_RSCUR" << "]"
                    << endl;
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, Arg(Bs, "LOGICJUMP"));
            as.setArg(3, *argresult);
            break;

        case Rule::sentence____FIL_LSCUR_arg_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "sentence____FIL_LSCUR_arg_RSCUR" << "]" << endl;
#endif

            as.type = FIL;
            as.setArg(0, *arg1);

            break;
        case Rule::sentence____LIN_LSCUR_arg_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "sentence____LIN_LSCUR_arg_RSCUR" << "]" << endl;
#endif

            as.type = LIN;
            as.setArg(0, *arg1);

            break;
        case Rule::sentence____LOAD_LSCUR_arg_RSCUR:
#if debug
            cout << "in Compiler::matchRule switch:["
                    << "sentence____LOAD_LSCUR_arg_RSCUR" << "]" << endl;
#endif

            as.type = LOAD;
            as.setArg(0, *arg1);

            break;
        default:
#if debug
            cout << "in Compiler::matchRule switch:[" << "default" << "]"
                    << endl;
#endif
#if debug
            cout << "in Compiler::matchRule no rule found" << endl;
#endif
            break;

        }

#if debug
        {
            static int ignorecount = 0;
#if debug
            cout << "matchRule(bool, int, int*, int*, int*, int*)"
                    << " matchRule success" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/han_compiler.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
#endif
        }
#endif

        if (createNew == true) {
//            ++lineNumber;
//            outputAssemblyCode(as);
            lastCodeType = as.type;
            output_code_buffer.push_back(as);
        }
        if ((lastCodeType == M ||lastCodeType == EXPRQE)&& as.type!=M && as.type!=EXPRQS && as.type!=EXPRQE){

            while(!output_code_buffer.empty()){
                ++lineNumber;
                outputAssemblyCode(output_code_buffer.front());
                output_code_buffer.pop_front();

            }
        }

    }
    void matchFinish() {
        while(!output_code_buffer.empty()){
            ++lineNumber;
            outputAssemblyCode(output_code_buffer.front());
            output_code_buffer.pop_front();

        }
        outputfstream.close();
    }
    void put_EXPRQ_first(){
        stack<Intg> pos_EXPRQS_stack;
        stack<Intg> pos_EXPRQE_stack;

        auto it = output_code_buffer.begin();
        for(Intg i = 0;it!=output_code_buffer.end();++it,++i){
            if(it->type == EXPRQS){
                pos_EXPRQS_stack.push(i);
            }
            if(it->type == EXPRQE){
                pos_EXPRQE_stack.push(i);
            }
        }
        if(pos_EXPRQS_stack.size()!=pos_EXPRQE_stack.size()){
            #if debug
            {
                   static int ignorecount = 0;
                   std::cout <<"("<<__FILE__<<":"<<__LINE__<<":0"<<")\tput_EXPRQ_first()"<< " unpaired EXPRQS and EXPRQE, exit(-1)"
                           << "\tignorecount:["
                           << ignorecount++ << "\t]" << std::endl;
            }
            #endif

            cerr<<"unpaired EXPRQS and EXPRQE, exit(-1)"<<endl;
            exit(-1);

        }
        if(pos_EXPRQS_stack.empty()){
            return;
        }
        list<deque<AssemblyCode>> expr_buffers;
        while(!pos_EXPRQS_stack.empty()){
            Intg spos =  pos_EXPRQS_stack.top();
            Intg epos =  pos_EXPRQE_stack.top();

            auto its = output_code_buffer.begin()+spos;
            auto ite = output_code_buffer.begin()+epos+1;
            deque<AssemblyCode> expr_buffer(its,ite+1);
            expr_buffers.push_back(expr_buffer);
            output_code_buffer.erase(its,ite+1);

            pos_EXPRQS_stack.pop();
            pos_EXPRQE_stack.pop();

        }

        while(!expr_buffers.empty()){
            auto & expr_buffer = expr_buffers.front();
            output_code_buffer.insert(output_code_buffer.begin(),expr_buffer.begin(),expr_buffer.end());
            expr_buffers.pop_front();
        }
    }
    void flushMap() {
#if debug
        {
            static int ignorecount = 0;
#if debug
            cout << "flushMap()" << " " << "\tignorecount:[" << ignorecount++
                    << "\t](" << "han_compiler.hpp:" << __LINE__ << ":0" << ")"
                    << endl;
#endif
        }
#endif
//����flushmap֮ǰ��
//lex�����Ѿ�����������һ�����ʽ��token���벢�������token_arg_map��
//��˱��������½���ӵ�ʮ������(С�ģ�������������ӵ���ǰ�棬����ں���)
        Intg size = token_arg_map.size();
        auto it = token_arg_map.end();
        if (size > 0) {
            it--;
        }
        while (size > 10) {

            token_arg_map.erase(it--);
            size--;
        }
    }

};
extern void output();
}
/*
 *interface
 */

extern void output();
extern COOLANG::Compiler compiler;

extern void setOutputCodePath(const char *filePath);

extern Intg matchToken(Intg createNew, Intg tokenType, Intg argFlag,
        const char *tokenString);
extern void matchRule(const Intg createNew, const Intg ruleType,
        const Intg *tokenNumber1, const Intg *tokenNumber2,
        const Intg *tokenNumber3, const Intg *tokenNumber4);
extern void matchFinish();
extern void flushMap();

#endif /* COOLANG_COMPILER_H_ */
