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
#include "coolang_compiler_auto_integrate.hpp"

using namespace COOLANG;
using namespace std;
void output() {
    return COOLANG::output();
}
void COOLANG::output() {
    cout << "COOLANG::output" << endl;
}
COOLANG::Compiler compiler;

void setOutputCodePath(const char *filePath) {
    try {
        compiler.setOutputCodePath(filePath);
    } catch (exception &e) {
        cout << "setOutputCodePath err" << e.what() << endl;
    }
}
Intg matchToken(Intg createNew, Intg tokenType, Intg argFlag,
        const char *tokenString) {
    try {
        return compiler.matchToken(createNew, tokenType, argFlag, tokenString);
    } catch (exception &e) {
        cout << "matchToken err" << e.what() << endl;
        exit(1);
    }

}
void matchRule(Intg createNew, Intg ruleType, const Intg *tokenNumber1,
        const Intg *tokenNumber2, const Intg *tokenNumber3,
        const Intg *tokenNumber4) {
    try {
        Intg * tokenNumber1_;
        Intg * tokenNumber2_;
        Intg * tokenNumber3_;
        Intg * tokenNumber4_;
        tokenNumber1_ = const_cast<Intg*>(tokenNumber1);
        tokenNumber2_ = const_cast<Intg*>(tokenNumber2);
        tokenNumber3_ = const_cast<Intg*>(tokenNumber3);
        tokenNumber4_ = const_cast<Intg*>(tokenNumber4);
        compiler.matchRule(createNew, ruleType, tokenNumber1_, tokenNumber2_,
                tokenNumber3_, tokenNumber4_);
    } catch (exception &e) {
        cout << "matchRule err" << e.what() << endl;
    }

}
void matchFinish() {
    try {
        compiler.matchFinish();
    } catch (exception &e) {
        cout << "matchFinish err" << e.what() << endl;
    }
    compiler.matchFinish();
}
void flushMap() {
    try {
        compiler.flushMap();
    } catch (exception &e) {
        cout << "flushMap err" << e.what() << endl;
    }
    compiler.flushMap();
}
Intg COOLANG::toIntg(Strg s) {
    return atoi(s.c_str());
}
Strg COOLANG::toStrg(const Intg& i) {
    stringstream ss;
    ss << i;
    return ss.str();
}
bool COOLANG::isNumber(const string &str){
    stringstream ss(str);
    double d;
    char c;
    if (!(ss >> d))
        return false;
    if (ss >> c)
        return false;
    return true;
}

