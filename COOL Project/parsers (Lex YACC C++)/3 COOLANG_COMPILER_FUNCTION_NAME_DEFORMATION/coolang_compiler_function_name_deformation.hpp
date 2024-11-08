/*
 * han_compiler.h
 *
 *  Created on: Oct 19, 2021
 *      Author: han
 */

#include"coolang_compiler_define.h"

#ifndef COOLANG_FUNCTION_NAME_DEFORMATION_HPP
#define COOLANG_FUNCTION_NAME_DEFORMATION_HPP
#define safe_delete(p) { if(p) { delete (p);     (p)=NULL; } }

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

#define F_ false
#define T_ true

#define Intg int
#define Addr int

#define INSERTSTRG "_ARG_"

#define __line__ "#line"
#ifndef fileno
#define fileno(__F) ((__F)->_file)
#endif
using namespace std;
namespace COOLANG {
class Strg: public std::string {
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
class FunctionName;
class Block {
public:
    bool isFN = false;
    Strg s;
    FunctionName *funn = NULL;
    Strg toStrg() const;
    Block(Intg isFN, const char *s) {
        this->isFN = isFN;
        this->s = Strg(s);
    }
    Block(Intg isFN, const Strg &s) {
        this->isFN = isFN;
        this->s = Strg(s);
    }
    Block(Intg isFN, FunctionName *funcn) {
        this->isFN = isFN;
        this->funn = funcn;
    }
    Block() {

    }
};
/**
 * @attention ���ݰ�����������,����ֻ�Ǻ�������,
 * @attention һ��
 *
 */
class FunctionName {
public:
#define typeFuncNameOrExpr 0 //�������ƻ���ʽƬ��
#define typeScur 1 //С����,�����б�,ƥ��"("
#define typeMcur 2 //С����,�����б�,ƥ��"("
#define typeBcur 3  //������,������,ƥ��"{"
    Intg type = 0; //0 ���溯�����ƻ���ʽ, 1 ������ 2 ���ַ�
#define switch_identity 1 //������������
#define switch_arg 2 //������,����ֱֹ����addBlockʱȷ��
#define switch_prefix 3 //һ��λ�ں���/���ʽǰ,��ǰ��һ����ֱ������ʽ����(һԪ�����,ֻ���Ҳ�����)
#define switch_suffix 4 //һ��λ�ں���/���ʽ��,�������������û�б��ʽ(һԪ�����,ֻ���������,��,��Ԫ�����)
#define switch_sep 5  //��������,������prefixҲ������suffix,���ڸ���ͬ��ı��ʽ,��������comma
#define switch_lscur 6  //������
#define switch_rscur 7  //������
#define switch_comma 8  //����
#define switch_lmcur 9  //������������,
#define switch_rmcur 10 //��������
#define switch_lbcur 11  //�������,
#define switch_rbcur 12 //�Ҵ�����
//valid when type==0
    Intg lastFNSwitch = 0; //switcher of the last block input FunctionName
    Intg switcher = 0;
    //parentFN:������ʽ�ϲ�ı��ʽ
    FunctionName *parentFN = NULL;
    //appendFN:һ�����ʽ�����ͬ������ʽ
    FunctionName *appendFN = NULL;
    //*oldBrotherFN:һ�����ʽǰ���ͬ������ʽ
    FunctionName *oldBrotherFN = NULL;

//valid when type = 0
    //format : prefix identity arg suffix
    //ר�Ŵ�ź��������ַ�������ʽ�����ַ���(��ʶ��)���Դ洢�ַ���,bcurFN,mcurFN
    deque<Block> identitydq;
    //��ź�������������������һ��λ��block�е�funn��ʾ����Ϊ����Ҳ�����Ǻ�������,�����ڴ洢scurFN
    deque<Block> argdq;
    //ǰ׺�������@#$+����*/,?:�ȿ���λ�ں���/����ǰ�棬����������������/�������ַ�������
    deque<Block> prefixdq;
    //��׺�������+��*/,?.<< =>-->:�ȿ���λ�ں�������Ĳ�������һ��λ�ں���/�������棬����������������/�������ַ�������
    //���һ�����������Է���ǰ��Ҳ���Է��ں��棨��Ϊ��׺�������Ƚ�����Ϊ��׺
    deque<Block> suffixdq;
//valid when type == scur
    //format : ( scurblock )
    deque<Block> scurblockdq;
//valid when type == mcur
    //format : { mbcurblock }
    deque<Block> mcurblockdq;
//valid when type == bcur
    //format : { bcurblock }
    deque<Block> bcurblockdq;

private:
    //��ֹ����ջʵ��
    FunctionName() {

    }
public:
    static FunctionName* FunctionName_() {
        return new FunctionName();
    }
    //�ж��Ƿ�δ�洢������,��ת��Ϊ�ַ�����Ϊ��.ע��,()������,��{}��������
    bool isEmpty() {
        if (type == typeFuncNameOrExpr) {
            return identitydq.size() <= 0 && argdq.size() <= 0
                    && prefixdq.size() <= 0 && suffixdq.size() <= 0
                    && (appendFN ? appendFN->isEmpty() : 1);
        } else if (type == typeBcur || type == typeMcur) {
            return false;
        } else {
            for (auto &it : scurblockdq) {
                if (it.isFN && it.funn->isEmpty() == false) {
                    return false;
                } else if (it.isFN == false && it.s != "") {
                    return false;
                }
            }
            return true;
        }
    }
    /**
     * @brief convert FunctionName to a string
     * @attention To convert FN to string correctly,  we should discuss by case, here is the structure:
     * FN's type == s|m|bcur:
     *      parentFN...
     * FN's type == expr
     *      isn't Function(argdq size <= 0)....
     *      isFunction(argdq size >0)....
     * @return the generated string
     */
    Strg toStrg() const {
#if debug
        {
            static int ignorecount = 0;
            std::cout << "toStrg()" << " " << "\tignorecount:[" << ignorecount++
                    << "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")"
                    << std::endl;
        }
#endif
        stringstream ss;
        if (type == typeScur) {
            if (parentFN) {
#if debug
                {
                    static int ignorecount = 0;
                    std::cerr << "toStrg()"
                            << "if parentFN exist, can't convert scur directly, exit(-1) "
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << __FILE__ << ":" << __LINE__ << ":0" << ")"
                            << std::endl;
                }
#endif
                exit(-1);
            } else {
                ss << "(";
                for (const auto &blk : this->scurblockdq) {
                    ss << blk.toStrg();

                }
                ss << ")";
                return ss.str();
            }

        } else if (type == typeMcur) {
            if (parentFN) {
#if debug
                {
                    static int ignorecount = 0;
                    std::cerr << "toStrg()"
                            << "if parentFN exist, can't convert mcur directly, exit(-1) "
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << __FILE__ << ":" << __LINE__ << ":0" << ")"
                            << std::endl;
                }
#endif
                exit(-1);
            } else {
                ss << "[";
                for (const auto &blk : this->mcurblockdq) {
                    ss << blk.toStrg();

                }
                ss << "]";
                return ss.str();
            }

        } else if (type == typeBcur) {
            if (parentFN) {
#if debug
                {
                    static int ignorecount = 0;
                    std::cerr << "toStrg()"
                            << "if parentFN exist, can't convert bcur directly, exit(-1) "
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << __FILE__ << ":" << __LINE__ << ":0" << ")"
                            << std::endl;
                }
#endif
                exit(-1);
            } else {
                ss << "{";
                for (const auto &blk : this->bcurblockdq) {
                    ss << blk.toStrg();

                }
                ss << "}";

            }

        } else if (type == typeFuncNameOrExpr) {
            if (argdq.size() <= 0) {
                for (const auto &itprefix : prefixdq) {
                    ss << itprefix.toStrg();
                }
                for (const auto &itidentity : identitydq) {

                    if (itidentity.isFN) {
                        if (itidentity.funn->type == typeFuncNameOrExpr) {
                            ss << itidentity.toStrg();
                        } else if (itidentity.funn->type == typeBcur) {
                            ss << "{";
                            for (const auto &elem : itidentity.funn->bcurblockdq) {
                                ss << elem.toStrg();
                            }
                            ss << "}";
                        } else if (itidentity.funn->type == typeMcur) {
                            ss << "[";
                            for (const auto &elem : itidentity.funn->mcurblockdq) {
                                ss << elem.toStrg();
                            }
                            ss << "]";
                        } else if (itidentity.funn->type == typeScur) {
                            ss << "(";
                            for (const auto &elem : itidentity.funn->scurblockdq) {
                                ss << elem.toStrg();
                            }
                            ss << ")";
                        }

                    } else {
                        ss << itidentity.s;
                    }

                }
                for (const auto &itsuffix : suffixdq) {
                    ss << itsuffix.toStrg();
                }
                if (appendFN) {
                    ss << appendFN->toStrg();
                }

            } else {
                for (const auto &itprefix : prefixdq) {
                    ss << itprefix.toStrg();
                }
                bool isFunction = false;
                Strg functionNameStrg = "";
                for (const auto &itidentity : identitydq) {

                    if (itidentity.isFN) {
                        if (itidentity.funn->type == typeFuncNameOrExpr) {
                            ss << itidentity.toStrg();
                        } else if (itidentity.funn->type == typeBcur) {
                            ss << "{";
                            for (const auto &elem : itidentity.funn->bcurblockdq) {
                                ss << elem.toStrg();
                            }
                            ss << "}";
                        } else if (itidentity.funn->type == typeMcur) {
                            ss << "[";
                            for (const auto &elem : itidentity.funn->mcurblockdq) {
                                ss << elem.toStrg();
                            }
                            ss << "]";
                        } else if (itidentity.funn->type == typeScur) {
                            ss << "(";
                            for (const auto &elem : itidentity.funn->scurblockdq) {
                                ss << elem.toStrg();
                            }
                            ss << ")";
                        }

                    } else {
                        if (isFunction == false) {
                            if (itidentity.s != INSERTSTRG) {
                                isFunction = true;
                            }
                        }
                        functionNameStrg += itidentity.s;
                    }

                }
                if (isFunction) {
                    ss << functionNameStrg;
                }

                Strg args;
                ss << "(";
                for (const auto &itarg : argdq) {
                    Strg argList;
                    for (const auto &elem : itarg.funn->scurblockdq) {
                        argList += elem.toStrg();
                    }
                    if (argList != "") {
                        args += argList;
                        args += ",";
                    }

                }
                if (args.length() > 0) {
                    args.pop_back(); // pop out the comma at the end of args
                }
                ss << args;
                ss << ")";
                for (const auto &itsuffix : suffixdq) {
                    ss << itsuffix.toStrg();
                }
                if (appendFN) {
                    ss << appendFN->toStrg();
                }

            }

        }

        Strg &&convertStrg = ss.str();
#if debug
        {
            static int ignorecount = 0;
            std::cout << "toStrg()" << " convert Strg:" << convertStrg
                    << "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
                    << ":" << __LINE__ << ":0" << ")" << std::endl;
        }
#endif
        return convertStrg;

    }
    Block& getLastArg() {
        return argdq.back();
    }
    Block& getLastName() {
        return identitydq.back();
    }

};
/**
 * Compiler Function Name Deformation
 * @attention
 *   currentFN : every time before or after we call addToken or addBlock,
 *        we should make that currentFN's type is {@type typeFuncNameOrExpr}
 *   addBlock  : adding block must must ensure that
 *          blocks with FunctionName of type {@type Bcur/Mcur/Rcur} 's parentFNs
 *          type is {@type typeFuncNameOrExpr}
 *          that is,   {@type Bcur/Mcur/Rcur} cannot be parent of
 *          {@type Bcur/Mcur/Rcur}
 *
 */
class Compiler_FND {
public:
    FunctionName *currentFN = NULL;
    Block *lastBlock = NULL;    //��һ�����Զ����ɵ�block
    char *currentTokenStrg = NULL;
    //the switch of last block read in FND programm
    Intg lastFNDSwitcher = 0;
    ofstream outputfstream;
    Strg filePath;
    deque<FunctionName*> pdelete;
    void flush();
    void setOutputCodePath(const char *filePath) {
        this->filePath = Strg(filePath);
        outputfstream.close();
        outputfstream.open(filePath, ios::trunc);
        outputfstream.close();

        return;
    }
    /**
     * @brief create a FN whose parentFN is predetermined , and push it to pdelete
     * @param parentFN
     * @return the point of the created FN
     */
    FunctionName* createFN(FunctionName *parentFN) {
        FunctionName *fn = FunctionName::FunctionName_();
        fn->parentFN = parentFN;
        pdelete.push_back(fn);
        return fn;
    }
    FunctionName* createFN() {
        return createFN(NULL);
    }

    /*
     * ����ǰfn����Ϊ���׷�ӵ�fn����������,��Ϊ��ǰfn׷��һ��fn��Ȼ�󽫵�ǰfn����Ϊ׷�ӵ�fn
     * ����׷�ӵ�fn����oldBrother����Ϊ���ӵ���һ��fn���丸Fn����ΪoldBrother��parentFN
     */
    void appendFN() {

        if (!currentFN->appendFN) {
#if debug
            {
                static int ignorecount = 0;
                cout << "appendFN()" << " appendFN==NULL!!!"
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "han_compiler_function_name_deformation.hpp:"
                        << __LINE__ << ":0" << ")" << endl;
            }
#endif
            currentFN->appendFN = FunctionName::FunctionName_();

            pdelete.push_back(currentFN->appendFN);

            currentFN->lastFNSwitch = switch_sep;
        }
        FunctionName *oldBrotherOfAppend = currentFN;
        currentFN = currentFN->appendFN;
        currentFN->oldBrotherFN = oldBrotherOfAppend;
        currentFN->parentFN = currentFN->oldBrotherFN->parentFN;
    }
    //@using when parentFN type is scopeFN, create a new elementFN following currentFN
    // and set it as currentFN
    //
    //
    void addScurBrotherElementFN() {
        parentFN();
        if (currentFN->isEmpty()) {
            return;
        }
        FunctionName *scpBrotherElementFn = FunctionName::FunctionName_();

        currentFN->bcurblockdq.push_back(Block(true, scpBrotherElementFn));
        scpBrotherElementFn->parentFN = currentFN;
        lastBlock = &(currentFN->argdq.back());

        pdelete.push_back(scpBrotherElementFn);

        currentFN = scpBrotherElementFn;

    }
    /*
     * @using when matchs "{"
     * �ڵ�ǰfn�Ļ��������һ���µ�fn,  ���Դ����� ,�����������һ���յ�FN��ΪԪ��
     * @attention ���currentfnΪ��,�򴴽��µ�fn
     * ���currentfn  type == expr, ����� ���µ�fn���Ϊidentity
     * ���currentfn parent  type == bcur,���½���fn��ӵ�currentfn��bcurblockdq��
     * ���currentfn parent  type == mcur,���½���fn��ӵ�currentfn��mcurblockdq��
     * ���currentfn parent  type == scur,�½�fn����ӵ�scurblockdq ,����functionName�����InsertStrgռλ
     *
     * �����Ϻ���bcurFN�����һ���յ�fn��ΪcurrentFN
     */
    void createBcurFN() {
        if (currentFN == NULL) {
            currentFN = FunctionName::FunctionName_();
            pdelete.push_back(currentFN);
            currentFN->type = typeFuncNameOrExpr;
        }
        if (currentFN->type == typeMcur || currentFN->type == typeBcur
                || currentFN->type == typeScur) {
#if debug
            {
                static int ignorecount = 0;
                std::cerr << "createBcurFN()"
                        << "  currentFN type == S|M|Bcur, exit(-1) "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << __FILE__ << ":" << __LINE__ << ":0" << ")"
                        << std::endl;
            }
#endif
            exit(-1);

        } else if (currentFN->type == typeFuncNameOrExpr) {

            //��ǰFN�޷������identity, ��bcurFN  append�����
            if (currentFN->identitydq.size() > 0 || currentFN->argdq.size() > 0
                    || currentFN->suffixdq.size() > 0) {
                appendFN();

                FunctionName *bcurFN = createFN(currentFN);
                bcurFN->type = typeBcur;
                currentFN->identitydq.push_back(Block(true, bcurFN));
                currentFN->lastFNSwitch = switch_identity;
                currentFN = bcurFN;

                FunctionName *elemFN = FunctionName::FunctionName_();
                elemFN->parentFN = currentFN;
                pdelete.push_back(elemFN);
                currentFN->bcurblockdq.push_back(Block(true, elemFN));
                currentFN = elemFN;
            } else {
                //����bcurFN���Ϊidentity
                currentFN->lastFNSwitch = switch_identity;
                FunctionName *bcurFN = FunctionName::FunctionName_();
                bcurFN->parentFN = currentFN;
                bcurFN->type = typeBcur;
                currentFN->identitydq.push_back(Block(true, bcurFN));
                currentFN = bcurFN;

                FunctionName *elemFN = FunctionName::FunctionName_();
                elemFN->parentFN = currentFN;
                pdelete.push_back(elemFN);
                currentFN->bcurblockdq.push_back(Block(true, elemFN));
                currentFN = elemFN;
            }

        }

    }
    /***
     * @using when match "}"
     * @attention leave current scope, won't append a "}" for the parentFN(type is bcur),
     * then set currentFN's parentFN(a bcurFN) 's parentFN as currentFN.if its type is
     * scur|mcur|bcur ,error.
     */
    void finishBcurFN() {
        if (currentFN->parentFN) {
            if (currentFN->parentFN->type == typeBcur) {
                parentFN();
                if (currentFN->parentFN) {
                    parentFN();
                    if (currentFN->type == typeScur
                            || currentFN->type == typeMcur
                            || currentFN->type == typeBcur) {
#if debug
                        {
                            static int ignorecount = 0;
                            std::cerr << "finishBcurFN()"
                                    << " a scur|mcur|bcur 's parentFN is a scur|mcur|bcur, exit(-1) "
                                    << "\tignorecount:[" << ignorecount++
                                    << "\t](" << __FILE__ << ":" << __LINE__
                                    << ":0" << ")" << std::endl;
                        }
#endif
                        exit(-1);
                    }
                } else {
                    flush();
                }

            } else {
#if debug
                {
                    static int ignorecount = 0;
                    std::cerr << "finishScurFN()"
                            << "elemFN's parentFN isn't scurFN, exit(-1) "
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << __FILE__ << ":" << __LINE__ << ":0" << ")"
                            << std::endl;
                }
#endif
                exit(-1);
            }

        } else {
#if debug
            {
                static int ignorecount = 0;
                std::cerr << "finishBcurFN()"
                        << " found '}' without matching '{', exit(-1) "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << __FILE__ << ":" << __LINE__ << ":0" << ")"
                        << std::endl;
            }
#endif
            exit(-1);
        }

    }
    /*
     * @using when matchs "["
     * �ڵ�ǰfn�Ļ��������һ���µ�fn,  ���Դ����� ,�����������һ���յ�FN��ΪԪ��
     * @attention ���currentfnΪ��,�򴴽��µ�fn
     * ���currentfn  type == expr, ����� ���µ�fn���Ϊidentity
     * ���currentfn parent  type == bcur,���½���fn��ӵ�currentfn��bcurblockdq��
     * ���currentfn parent  type == mcur,���½���fn��ӵ�currentfn��mcurblockdq��
     * ���currentfn parent  type == scur,�½�fn����ӵ�scurblockdq ,����functionName�����InsertStrgռλ
     *
     * �����Ϻ���mcurFN�����һ���յ�fn��ΪcurrentFN
     */
    void createMcurFN() {
        if (currentFN == NULL) {
            currentFN = FunctionName::FunctionName_();
            pdelete.push_back(currentFN);
            currentFN->type = typeFuncNameOrExpr;
        }

        if (currentFN->type == typeMcur || currentFN->type == typeBcur
                || currentFN->type == typeScur) {
#if debug
            {
                static int ignorecount = 0;
                std::cerr << "createMcurFN()"
                        << "  currentFN type == S|M|Bcur, exit(-1) "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << __FILE__ << ":" << __LINE__ << ":0" << ")"
                        << std::endl;
            }
#endif
            exit(-1);

        } else if (currentFN->type == typeFuncNameOrExpr) {

            //��ǰFN�޷������identity, ��mcurFN  append�����
            if (currentFN->identitydq.size() > 0 || currentFN->argdq.size() > 0
                    || currentFN->suffixdq.size() > 0) {
                appendFN();

                FunctionName *mcurFN = createFN(currentFN);
                mcurFN->type = typeMcur;
                currentFN->identitydq.push_back(Block(true, mcurFN));
                currentFN->lastFNSwitch = switch_identity;
                currentFN = mcurFN;

                FunctionName *elemFN = FunctionName::FunctionName_();
                elemFN->parentFN = currentFN;
                pdelete.push_back(elemFN);
                currentFN->mcurblockdq.push_back(Block(true, elemFN));
                currentFN = elemFN;
            } else {
                //����mcurFN���Ϊidentity
                currentFN->lastFNSwitch = switch_identity;
                FunctionName *mcurFN = FunctionName::FunctionName_();
                mcurFN->parentFN = currentFN;
                mcurFN->type = typeMcur;
                currentFN->identitydq.push_back(Block(true, mcurFN));
                currentFN = mcurFN;

                FunctionName *elemFN = FunctionName::FunctionName_();
                elemFN->parentFN = currentFN;
                pdelete.push_back(elemFN);
                currentFN->mcurblockdq.push_back(Block(true, elemFN));
                currentFN = elemFN;
            }

        }

    }
    /***
     * @using when match "]"
     * @attention leave current scope, won't append a "]" for the parentFN(type is mcur),
     * then set currentFN's parentFN(a mcurFN) 's parentFN as currentFN.if its type is
     * scur|mcur|bcur ,error.
     */
    void finishMcurFN() {
        if (currentFN->parentFN) {
            if (currentFN->parentFN->type == typeMcur) {
                parentFN();
                if (currentFN->parentFN) {
                    parentFN();
                    if (currentFN->type == typeScur
                            || currentFN->type == typeMcur
                            || currentFN->type == typeBcur) {
#if debug
                        {
                            static int ignorecount = 0;
                            std::cerr << "finishMcurFN()"
                                    << " a scur|mcur|bcur 's parentFN is a scur|mcur|bcur, exit(-1) "
                                    << "\tignorecount:[" << ignorecount++
                                    << "\t](" << __FILE__ << ":" << __LINE__
                                    << ":0" << ")" << std::endl;
                        }
#endif
                        exit(-1);
                    }
                } else {
                    flush();
                }

            } else {
#if debug
                {
                    static int ignorecount = 0;
                    std::cerr << "finishScurFN()"
                            << "elemFN's parentFN isn't scurFN, exit(-1) "
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << __FILE__ << ":" << __LINE__ << ":0" << ")"
                            << std::endl;
                }
#endif
                exit(-1);
            }

        } else {
#if debug
            {
                static int ignorecount = 0;
                std::cerr << "finishMcurFN()"
                        << " found '}' without matching '{', exit(-1) "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << __FILE__ << ":" << __LINE__ << ":0" << ")"
                        << std::endl;
            }
#endif
            exit(-1);
        }

    }

    /*
     * @using when matchs "("
     * �ڵ�ǰfn�Ļ��������һ���µ�fn,  ����С���� ,�����������һ���յ�FN��ΪԪ��
     * @attention ���currentfnΪ��,�򴴽��µ�fn
     * ���currentfn  type == expr, ����� ���µ�fn���Ϊarg
     * ���currentfn parent  type == bcur,���½���fn��ӵ�currentfn��bcurblockdq��
     * ���currentfn parent  type == mbcur,���½���fn��ӵ�currentfn��mbcurblockdq��
     * ���currentfn parent  type == scur,�½�fn����ӵ�scurblockdq ,����functionName�����InsertStrgռλ
     *
     * �����Ϻ���bcurFN�����һ���յ�fn��ΪcurrentFN
     */
    void createScurFN() {
        if (currentFN == NULL) {
            currentFN = FunctionName::FunctionName_();
            pdelete.push_back(currentFN);
            currentFN->type = typeFuncNameOrExpr;

        }
        if (currentFN->type == typeBcur || currentFN->type == typeScur
                || currentFN->type == typeMcur) {
#if debug
            {
                static int ignorecount = 0;
                std::cerr << "createScurFN()"
                        << "  currentFN type == S|M|Bcur, exit(-1) "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << __FILE__ << ":" << __LINE__ << ":0" << ")"
                        << std::endl;
            }
#endif
            exit(-1);

        } else if (currentFN->type == typeFuncNameOrExpr) {

            //��ǰFN�޷������arg, ��  appendFN ,��scurFN push as identity
            if (currentFN->suffixdq.size() > 0) {
                appendFN();

                FunctionName *scurFN = createFN(currentFN);
                scurFN->type = typeScur;
                currentFN->argdq.push_back(Block(true, scurFN));
                currentFN->lastFNSwitch = switch_arg;
                currentFN = scurFN;

                FunctionName *elemFN = FunctionName::FunctionName_();
                elemFN->parentFN = currentFN;
                pdelete.push_back(elemFN);
                currentFN->scurblockdq.push_back(Block(true, elemFN));
                currentFN = elemFN;
            } else {
                //����scurFN���Ϊarg
                currentFN->lastFNSwitch = switch_arg;
                FunctionName *scurFN = FunctionName::FunctionName_();
                scurFN->type = typeScur;
                scurFN->parentFN = currentFN;
                currentFN->argdq.push_back(Block(true, scurFN));
                currentFN = scurFN;

                FunctionName *elemFN = FunctionName::FunctionName_();
                elemFN->parentFN = currentFN;
                pdelete.push_back(elemFN);
                currentFN->scurblockdq.push_back(Block(true, elemFN));
                currentFN = elemFN;
            }

        }

    }
    /***
     * @using when match ")"
     * @attention leave current scur, won't append a ")" for the parentFN(type is scur),
     * then set currentFN's parentFN(a scurFN) 's parentFN as currentFN,
     * if its type is scur|mcur|bcur ,error ,else push INSERTSTRING as identity.
     */
    void finishScurFN() {
        if (currentFN->parentFN) {
            if (currentFN->parentFN->type == typeScur) {
                currentFN = currentFN->parentFN;
                if (currentFN->parentFN) {
                    if (currentFN->isEmpty() == false) {
                        currentFN->parentFN->identitydq.push_back(
                                Block(false, INSERTSTRG));

                    }
                    parentFN();
                    if (currentFN->type == typeScur
                            || currentFN->type == typeMcur
                            || currentFN->type == typeBcur) {
#if debug
                        {
                            static int ignorecount = 0;
                            std::cerr << "finishBcurFN()"
                                    << " a scur|mcur|bcur 's parentFN is a scur|bcur, exit(-1) "
                                    << "\tignorecount:[" << ignorecount++
                                    << "\t](" << __FILE__ << ":" << __LINE__
                                    << ":0" << ")" << std::endl;
                        }
#endif
                        exit(-1);
                    } else if (currentFN->lastFNSwitch == switch_arg) {

                    }
                } else {
                    flush();
                }

            } else {
#if debug
                {
                    static int ignorecount = 0;
                    std::cerr << "finishBcurFN()"
                            << "elemFN's parentFN isn't bcurFN, exit(-1) "
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << __FILE__ << ":" << __LINE__ << ":0" << ")"
                            << std::endl;
                }
#endif
                exit(-1);
            }

        } else {
#if debug
            {
                static int ignorecount = 0;
                std::cerr << "finishBcurFN()"
                        << " found ')' without matching '(', exit(-1) "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << __FILE__ << ":" << __LINE__ << ":0" << ")"
                        << std::endl;
            }
#endif
            exit(-1);
        }

    }
    /**
     * @brief handle the circumstance when find a ","
     * when parentFN == null , treat "," as a separator and flush;
     * when parentFN type is scur , treate  "," as a separator and insert it to parentFN's scurblockdq,
     * on this circumstance:
     *      if parentFN's parentFN is exist and type is typeExpression, push a INSERTSTRG back into identitydq.
     * when parentFN type is bcur|mcur , treate  "," as a separator and insert it in parentFN's b|mcurblockdq,
     * while parentFN type is typeExpr , set parentFN as currentFN,when exit,apply one of the above solusions
     * @attention if a block contents a "comma" ,it cannot content other identity or symbol!!!!
     * a comma can't form expr with any other symbol.
     */
    void setComma() {
        if (currentFN == NULL) {
            currentFN = FunctionName::FunctionName_();
            pdelete.push_back(currentFN);
            currentFN->identitydq.push_back(Block(false, ","));
            flush();
            return;
        } else {
            if (currentFN->parentFN == NULL) {
                appendFN();
                currentFN->identitydq.push_back(Block(false, ","));
                flush();
                return;
            } else {
                if (currentFN->parentFN->type == typeScur) {
                    currentFN->parentFN->scurblockdq.push_back(
                            Block(false, ","));
                    if (currentFN->parentFN->parentFN->type
                            == typeFuncNameOrExpr
                            && currentFN->parentFN->parentFN->lastFNSwitch
                                    == switch_arg) {
                        currentFN->parentFN->parentFN->identitydq.push_back(
                                Block(false, INSERTSTRG));
                    }
                    FunctionName *elemFN = createFN(currentFN->parentFN);
                    currentFN->parentFN->identitydq.push_back(
                            Block(true, elemFN));
                    currentFN = elemFN;

                } else if (currentFN->parentFN->type == typeBcur) {
                    currentFN->parentFN->bcurblockdq.push_back(
                            Block(false, ","));
                    FunctionName *elemFN = createFN(currentFN->parentFN);
                    currentFN->parentFN->bcurblockdq.push_back(
                            Block(true, elemFN));

                    currentFN = elemFN;

                } else if (currentFN->parentFN->type == typeMcur) {
                    currentFN->parentFN->mcurblockdq.push_back(
                            Block(false, ","));
                    FunctionName *elemFN = createFN(currentFN->parentFN);
                    currentFN->parentFN->mcurblockdq.push_back(
                            Block(true, elemFN));

                    currentFN = elemFN;

                } else if (currentFN->parentFN->type == typeFuncNameOrExpr) {
                    while (currentFN->parentFN
                            && currentFN->parentFN->type == typeFuncNameOrExpr) {

                        parentFN();

                    }
                    setComma();

                }

            }
        }

    }

    /**
     * @brief handle the circumstance when find a separator. the separator can't be a comma
     * and its FN only has an identity as content.
     * @param s : separator string.
     * @attention
     * when parentFN == null, flush;
     * else
     * when parentFN's type is a scur|mcur|bcur , append the block to currentFN;
     * while parentFN's type is a expr ,set parentFN as currentFN to apply the above solutions
     */
    void setSeparator(const char *s) {
        if (currentFN == NULL) {
            currentFN = createFN();
            currentFN->identitydq.push_back(Block(false, s));
            flush();
        } else {
            if (currentFN->parentFN == NULL) {
                appendFN();
                currentFN->identitydq.push_back(Block(false, s));
                flush();
            } else {
                if (currentFN->parentFN->type == typeScur
                        || currentFN->parentFN->type == typeMcur
                        || currentFN->parentFN->type == typeBcur) {
                    appendFN();
                    currentFN->identitydq.push_back(Block(false, s));
                    currentFN->lastFNSwitch = switch_sep;
                    appendFN();

                } else {
                    while (currentFN->parentFN
                            && currentFN->parentFN->type == typeFuncNameOrExpr) {
                        parentFN();
                    }
                    setSeparator(s);

                }

            }

        }

    }
    /**
     * @brief handle circumstance when match an identity
     * @param s :identity string
     * @attention
     * when currentFN's type is expr:
     *     if currentFN don't have suffix and lastFNswitch isn't a separator, push  as identity;
     *     else
     *        if parentFN isn't null ,
     *             parentFN's type = expr ,appendFN and push as identity
     *             parentFN's type = scur ,goto parentFN and push into scurblockdq
     *             parentFN's type = mcur :goto parentFN and push into mcurblockdq
     *             parentFN's type = bcur :goto parentFN and push into bcurblockdq
     *
     *        else flush and createFN then push as identity.
     */
    void setIdentity(const char *s) {
        if (currentFN == NULL) {
            currentFN = createFN();
            currentFN->identitydq.push_back(Block(false, s));
            currentFN->lastFNSwitch = switch_identity;
        } else {
            if (currentFN->suffixdq.size()
                    <= 0&&currentFN->lastFNSwitch != switch_sep) {
                currentFN->identitydq.push_back(Block(false, s));
                currentFN->lastFNSwitch = switch_identity;
            } else {
                if (currentFN->parentFN) {
                    if (currentFN->parentFN->type == typeFuncNameOrExpr) {
                        appendFN();
                        currentFN->identitydq.push_back(Block(false, s));
                        currentFN->lastFNSwitch = switch_identity;
                    } else if (currentFN->parentFN->type == typeScur) {
                        FunctionName *elemFN = createFN(currentFN->parentFN);
                        elemFN->identitydq.push_back(Block(false, s));
                        currentFN->parentFN->scurblockdq.push_back(
                                Block(true, elemFN));
                        currentFN = elemFN;

                    } else if (currentFN->parentFN->type == typeMcur) {
                        FunctionName *elemFN = createFN(currentFN->parentFN);
                        elemFN->identitydq.push_back(Block(false, s));
                        currentFN->parentFN->mcurblockdq.push_back(
                                Block(true, elemFN));
                        currentFN = elemFN;

                    } else if (currentFN->parentFN->type == typeBcur) {
                        FunctionName *elemFN = createFN(currentFN->parentFN);
                        elemFN->identitydq.push_back(Block(false, s));
                        currentFN->parentFN->bcurblockdq.push_back(
                                Block(true, elemFN));
                        currentFN = elemFN;

                    }
                } else {
                    flush();
                    currentFN = createFN();
                    currentFN->identitydq.push_back(Block(false, s));
                    currentFN->lastFNSwitch = switch_identity;
                }

            }

        }

    }
    /**
     * @brief handle the circumstance when match a prefix
     * @param s: the prefix string
     * @attention
     * when currentFN is empty or it only has prefix, push as prefix, else appendFN and push as prefix
     *
     */
    void setPrefix(const char *s) {
        if (currentFN == NULL) {
            currentFN = createFN();
            currentFN->prefixdq.push_back(Block(false, s));
            currentFN->lastFNSwitch = switch_prefix;
        } else {
            if (currentFN->lastFNSwitch == switch_prefix
                    || currentFN->isEmpty()) {
                currentFN->prefixdq.push_back(Block(false, s));
                currentFN->lastFNSwitch = switch_prefix;

            } else {
                appendFN();
                currentFN->prefixdq.push_back(Block(false, s));
                currentFN->lastFNSwitch = switch_prefix;
            }

        }

    }
    /**
     * @brief handle the circumstance when match a suffix
     * @param s :the suffix string
     * @attention
     * when currentFN's lastFNswitch is arg or identity or suffix, push as suffix,
     * else, createFN then push as suffix then flush
     *
     */
    void setSuffix(const char *s) {
        if (currentFN) {
            if (currentFN->lastFNSwitch == switch_arg
                    || currentFN->lastFNSwitch == switch_identity
                    || currentFN->lastFNSwitch == switch_suffix) {
                currentFN->suffixdq.push_back(Block(false, s));
                currentFN->lastFNSwitch = switch_suffix;
            } else {
                appendFN();
                currentFN->suffixdq.push_back(Block(false, s));
                currentFN->lastFNSwitch = switch_suffix;
            }
        } else {

            currentFN = createFN();
            currentFN->suffixdq.push_back(Block(false, s));
            currentFN->lastFNSwitch = switch_suffix;
            flush();

        }

    }
    /**
     *��Compiler�����һ���µĿ飬
     * @param switcher
     * @param s
     * @attention �κ�����£����ô˺���ʱ��currentFN���Ͳ�����Bcur��Scur��
     * similarly , a scur|bcur 's parentFn can't be a scur|bcur.
     */
    void addBlock(Intg switcher, char *s) {
#if debug
        {
            static int ignorecount = 0;
            cout << "addBlock(int, char*)" << " in" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "han_compiler_function_name_deformation.hpp:" << __LINE__
                    << ":0" << ")" << "\n\tswitcher:[" << switcher << "]\t"
                    << "blockStrg:[" << s << "]\t" << endl;

        }
#endif
        //switch_lbcur
        if (switcher == switch_lbcur) {
            createBcurFN();

        } else if (switcher == switch_rbcur) {
            //switch_rbcur
            finishBcurFN();

        } else if (switcher == switch_lmcur) {
            createMcurFN();
        } else if (switcher == switch_rmcur) {
            finishMcurFN();
        } else if (switcher == switch_lscur) {
            createScurFN();
        } else if (switcher == switch_rscur) {
            finishScurFN();
        } else if (switcher == switch_comma) {
            setComma();
        } else if (switcher == switch_sep) {
            setSeparator(s);

        } else if (switcher == switch_identity) {
            setIdentity(s);
        } else if (switcher == switch_prefix) {
            setPrefix(s);
        } else if (switcher == switch_suffix) {
            setSuffix(s);
        }
        lastFNDSwitcher = switcher;
    }
//set parentFN as currentFN
    void parentFN() {
        currentFN = currentFN->parentFN;
    }

    Compiler_FND() {
        currentFN = FunctionName::FunctionName_();
        pdelete.push_back(currentFN);
    }
    void addToken(Intg type, char *text) {

        currentTokenStrg = text;
        switch (type) {
        case Token::CALL:
#if debug
        {
            static int ignorecount = 0;
            cout << "addToken(int, char*)" << " CALL" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "han_compiler_function_name_deformation.hpp:" << __LINE__
                    << ":0" << ")" << endl;
        }
#endif
            exit(-1);
            break;
        case Token::CHANGEABLE:
            addBlock(switch_prefix, text);
            break;
	case Token::COMPATIBLE:
            addBlock(switch_prefix, text);
            break;
        case Token::PENDCHANGEABLE:
            addBlock(switch_prefix, text);
            break;
        case Token::CHENG:
            addBlock(switch_suffix, text);

            break;
        case Token::CHU:
            addBlock(switch_suffix, text);
            break;
        case Token::COMMA:
            addBlock(switch_comma, text);
            break;
        case Token::DAYU:
            addBlock(switch_suffix, text);
            break;
        case Token::JIADENG:
            addBlock(switch_suffix, text);
            break;
        case Token::JIANDENG:
            addBlock(switch_suffix, text);
            break;
        case Token::CHENGDENG:
            addBlock(switch_suffix, text);
            break;
        case Token::CHUDENG:
            addBlock(switch_suffix, text);
            break;
        case Token::MODENG:
            addBlock(switch_suffix, text);
            break;
        case Token::MIDENG:
            addBlock(switch_suffix, text);
            break;
        case Token::DENGYU:
            addBlock(switch_suffix, text);
            break;
        case Token::BUDENG:
            addBlock(switch_suffix, text);
            break;
        case Token::DOTDOT:
            addBlock(switch_prefix, text);
            break;
        case Token::FUZHI:
            addBlock(switch_suffix, text);
            break;
        case Token::AND:
            addBlock(switch_suffix, text);
            break;
        case Token::OR:
            addBlock(switch_suffix, text);
            break;
        case Token::MODULO:
            addBlock(switch_suffix, text);
            break;
        case Token::NOT:
            addBlock(switch_prefix, text);
            break;
        case Token::USER_OP3:
            addBlock(switch_suffix, text);
            break;
        case Token::HIGHEST:

            break;
        case Token::JIA:
            //�Ӽ�Ҳ�п�����ǰ׺
            if (lastFNDSwitcher == switch_sep || lastFNDSwitcher == switch_lscur) {
                addBlock(switch_prefix, text);
            } else {
                addBlock(switch_suffix, text);
            }

            break;
        case Token::JIAN:
            //�Ӽ�Ҳ�п�����ǰ׺
            if (lastFNDSwitcher == switch_sep || lastFNDSwitcher == switch_lscur) {
                addBlock(switch_prefix, text);
            } else {
                addBlock(switch_suffix, text);
            }

            break;
        case Token::USER_OP2:
            if (lastFNDSwitcher == switch_sep || lastFNDSwitcher == switch_lscur) {
                addBlock(switch_prefix, text);
            } else {
                addBlock(switch_suffix, text);
            }

            break;
        case Token::USER_OP1:
            if (lastFNDSwitcher == switch_sep || lastFNDSwitcher == switch_lscur) {
                addBlock(switch_prefix, text);
            } else {
                addBlock(switch_suffix, text);
            }

            break;
        case Token::SI:
            if (lastFNDSwitcher == switch_sep || lastFNDSwitcher == switch_lscur) {
                addBlock(switch_prefix, text);
            } else {
                addBlock(switch_suffix, text);
            }

            break;
        case Token::SD:
            if (lastFNDSwitcher == switch_sep || lastFNDSwitcher == switch_lscur) {
                addBlock(switch_prefix, text);
            } else {
                addBlock(switch_suffix, text);
            }

            break;

        case Token::LBCUR:
            addBlock(switch_lbcur, text);
            break;
        case Token::LMCUR:
            addBlock(switch_lmcur, text);

            break;
        case Token::LOWEST:

            break;
        case Token::LSCUR:
            addBlock(switch_lscur, text);

            break;
        case Token::MI:
            addBlock(switch_suffix, text);
            break;
        case Token::RBCUR:
            addBlock(switch_rbcur, text);
            break;
        case Token::RMCUR:
            addBlock(switch_rmcur, text);
            break;
        case Token::RSCUR:
            addBlock(switch_rscur, text);
            break;
        case Token::SEMICOLON:
            addBlock(switch_sep, text);
            break;
        case Token::SHUCHU:
            addBlock(switch_suffix, text);
            break;
        case Token::TUIDAO:
            addBlock(switch_suffix, text);
            break;
        case Token::JICHENG:
            addBlock(switch_suffix, text);
            break;
        case Token::DOT:
            addBlock(switch_suffix, text);
            break;
        case Token::CLEAR:
            addBlock(switch_suffix, text);
            break;
        case Token::ERASE:
            addBlock(switch_suffix, text);
            break;
        case Token::INSERT:
            addBlock(switch_suffix, text);
            break;
        case Token::LENGTH:
            addBlock(switch_suffix, text);
            break;
        case Token::TYPENAME:
            addBlock(switch_suffix, text);
            break;
        case Token::TOINT:
            addBlock(switch_suffix, text);
            break;
        case Token::TONUM:
            addBlock(switch_suffix, text);
            break;
        case Token::TOSTRG:
            addBlock(switch_suffix, text);
            break;
        case Token::_FIL:
            addBlock(switch_prefix, text);
            break;
        case Token::_LIN:
            addBlock(switch_prefix, text);
            break;
        case Token::_LOAD:
            addBlock(switch_prefix, text);
            break;
        case Token::LIST:
            addBlock(switch_prefix, text);
            break;
        case Token::MAP:
            addBlock(switch_prefix, text);
            break;
        case Token::MULTIMAP:
            addBlock(switch_prefix, text);
            break;
        case Token::SET:
            addBlock(switch_prefix, text);
            break;
        case Token::MULTISET:
            addBlock(switch_prefix, text);
            break;
        case Token::BACK:
            addBlock(switch_suffix, text);
            break;
        case Token::PUSHBACK:
            addBlock(switch_suffix, text);
            break;
        case Token::PUSHFRONT:
            addBlock(switch_suffix, text);
            break;
        case Token::POPBACK:
            addBlock(switch_suffix, text);
            break;
        case Token::POPFRONT:
            addBlock(switch_suffix, text);
            break;
        case Token::FIND:
            addBlock(switch_suffix, text);
            break;
        case Token::COUNT:
            addBlock(switch_suffix, text);
            break;
	case Token::EXIST_SUBEXPR:
            addBlock(switch_suffix, text);
            break;
	case Token::FIND_SUBEXPR:
            addBlock(switch_suffix, text);
            break;
	case Token::EXPR:
            addBlock(switch_identity, text);
            break;
	case Token::RESET:
            addBlock(switch_suffix, text);
            break;
        case Token::NONBLOCKEXECUTE:
            addBlock(switch_prefix, text);
            break;
        case Token::BLOCKEXECUTE:
            addBlock(switch_prefix, text);
            break;
        case Token::SLEEP:
            addBlock(switch_prefix, text);
            break;
	case Token::LOGICJUMP:
            addBlock(switch_prefix, text);
            break;

        case Token::XIAOYU:
            addBlock(switch_suffix, text);
            break;
        case Token::BUXIAOYU:
            addBlock(switch_suffix, text);
            break;
        case Token::BUDAYU:
            addBlock(switch_suffix, text);
            break;
        case Token::_return_:
            addBlock(switch_prefix, text);
            break;
	case Token::_abort_:
            addBlock(switch_prefix, text);
            break;
        case Token::_return_high:

            break;
        case Token::_return_low:

            break;
        case Token::arg:
            //��argǰ�ǡ�)��ʱ��˵����arg���ں��õ�name�������µĲ���
            if (lastFNDSwitcher == switch_rscur) {
                addBlock(switch_identity, text);
            } else {
                addBlock(switch_identity, text);
            }

            break;
        case Token::attribute:
	    if (lastFNDSwitcher == switch_rbcur){
	        char s[2]={';','\0'};
		addBlock(switch_sep,s);
	    }
            addBlock(switch_prefix, text);
            break;
        case Token::changeable_attribute:
            addBlock(switch_prefix, text);
            break;
        case Token::function_declare_attribute:
	    if (lastFNDSwitcher == switch_rbcur){
	        char s[2]={';','\0'};
		addBlock(switch_sep,s);
	    }
            addBlock(switch_prefix, text);
            break;
        case Token::variable_specific_attribute:
	    if (lastFNDSwitcher == switch_rbcur){
	        char s[2]={';','\0'};
		addBlock(switch_sep,s);
	    }
            addBlock(switch_prefix, text);
            break;
        case Token::IF:
            addBlock(switch_sep, text);
            break;
        case Token::ELSE:
            addBlock(switch_sep, text);
            break;
        case Token::ELSEIF:
            addBlock(switch_sep, text);
            break;
        case Token::ELIF:
            addBlock(switch_sep, text);
            break;
        case Token::WHILE:
            addBlock(switch_sep, text);
            break;

        default:
            break;
        }

    }

};

}
/*
 *interface
 */

extern void setOutputCodePath(const char *filePath);
extern void addToken(Intg tokenType, char *tokenString);
extern void flush();

#endif /* COOLANG_H_ */
