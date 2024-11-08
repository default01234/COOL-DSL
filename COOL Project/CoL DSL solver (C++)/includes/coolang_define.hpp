/*
 * coolang_define.h include #define and some global variables
 *
 *  Created on: 2022年8月16日
 *      Author: -
 */

#ifndef COOLANG_DEFINE_H_
#define COOLANG_DEFINE_H_

#include<set>
#include<map>
#include<string>
#include <cassert>
#define debug /*true*/ false
#define filelog /*true*/ true
#define WIN_DEBUG_OUTPUT /*true*/ false
inline bool debugflag = false;
inline int debug_min_addr = 999999;
#include <fstream>
#include <iostream>

#define hdot "\n.   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .\n" //can be used in toStrg member function
#define hline "\n-------------------------------------------------------------------------------------------------------------\n" //can be used in toStrg member function
#define bhline "\n================================================================================================================================================================================================\n" //more attractive, can not be used in toStrg member function

#define bbhline "\n################################################################################################################################################################################################\n" //more attractive, can not be used in toStrg member function

#define Intg int
#define Intl int64_t
#define Numb double
#define PRECISION 16 //type double precision

inline bool debugMode = false;

#define safe_delete(p)  { if(p) { delete (p);     (p)=nullptr; } }
#define FILEKEYPRIMARY "_prim_"
#define FILEKEYEND "_end_"
#define FILEKEYBUILTIN "built-in"
#define COOLANG_PROJECT_PATH "COOLANG_PROJECT_PATH"
#define COOLANG_HOME "COOLANG_HOME"
#define COOLANG_SOURCE_CODE_SUFFIX ".cos"
#define COOLANG_CHARACTER_CODE_SUFFIX ".coc"
#define COOLANG_BINARY_CODE_SUFFIX ".cob"
#define COC_DELIMITER "\\."
#define IO_KEYBOARD "KEYBOARD"
#define IO_SCREEN "SCREEN"

#define to_json_FUNN_ "funcName"
#define to_json_STRG_ "string"
#define to_json_NUMB_ "number"
#define to_json_ID_   "identifier"
#define to_json_CLS_  "class"
#define to_json_OPR_OTHER_  "other"
#define to_json_OPR_CALL_  "call"
#define to_json_OPR_COMMA_  "comma"
#define to_json_TRUE_ 1
#define to_json_FALSE_ 0

#define SETTING_BASE_ALG_BF 2
#define SETTING_BASE_ALG_DF 1

#define T_  true
#define F_ false
#define A_ 3 //adaptive (can be true or false, not determined now)

#define S_S_JIA -1
#define S_S_JIAN -2
#define S_S_CHENG -3
#define S_S_CHU -4
#define S_S_FUZHI -5
#define S_S_DENGYU -6
#define S_S_MI -7
#define CALL -8
#define SHUCHU -9
#define S_S_DAYU -10
#define S_S_XIAOYU -11
#define COMMA -12
#define TUIDAO -13
#define LST_ACCESS -14
#define LENGTH -15
#define ERASE -16
#define CLEAR -17
#define TYPENAME -18

#define S_S_AND -19
#define S_S_OR -20
#define S_S_NOT -21
#define S_S_MODULO -22

#define S_S_JIADENG -23
#define S_S_JIANDENG -24
#define S_S_CHENGDENG -25
#define S_S_CHUDENG -26
#define S_S_MODENG -27
#define S_S_MIDENG -28
#define S_SI -29 //operand on the left
#define S_SD -30

#define S_MAP -31
#define S_MULTIMAP -32
#define S_SET -33
#define S_MULTISET -34

#define S_TONUM -35
#define S_TOSTRG -36
#define S_TOINT -37
#define S_NONBLOCKEXECUTE -38
#define S_BLOCKEXECUTE -39
#define S_SLEEP -40
#define S_FIND -41
#define S_COUNT -42
#define S_INSERT -43
#define S_PUSHBACK -44
#define S_PUSHFRONT -45
#define S_POPBACK -46
#define S_POPFRONT -47
#define S_BACK -48
#define SI_S -49 //operand on the right
#define SD_S -50
#define S_S_BUDENG -51
#define S_S_BUDAYU -52
#define S_S_BUXIAOYU -53
#define S_S_EXIST_SUBEXPR -54
#define S_S_FIND_SUBEXPR -55
#define S_RESET -56
#define S_LOGICJUMP -57

#define ADDR_NEG_LOWBOUND -57

static std::set<std::string> inheritedChangeableOps = { ":","+","-","*","/" ,"==" ,"^" ,"call" ,"ca" ,">" ,"<" ,"COMMA" ,"[]" ,"LENGTH" ,"ERASE" ,"&&" ,"||" , "%" ,"MAP" ,"MULTIMAP" ,"SET" ,"MULTISET" ,"!=" ,"<=" ,">=" };
static std::set<std::string> constantChangeableOps = { "$" };
static std::set<std::string> constantUnchangeableOps = { "TYPENAME","EXIST_SUBEXPR","FIND_SUBEXPR" };
static std::set<std::string> constantAdaptiveChangeableOps = { "#" };
static std::set<std::string> nonChangeableRelatedOps = { "+=" ,"-=" ,"*=" ,"%=" ,"^=" , "--", "++", "=" , "-->" ,"TUIDAO" , "TONUM" ,"TOSTRG" ,"TOINT" ,"NONBLOCKEXECUTE" ,"BLOCKEXECUTE","CLEAR" ,"SLEEP", "LOGICJUMP" ,"FIND" ,"COUNT" ,"INSERT" ,"PUSHBACK" ,"PUSHFRONT" ,"POPBACK" ,"POPFRONT" ,"BACK" ,"RESET" };
static std::set<std::string> attributeRelatedOps = { "#", ":", "$", "?" };
static std::set<std::string> groundingPhaseOps = { "EXIST_SUBEXPR", "FIND_SUBEXPR" };


static std::set<std::string> lhsConstraintInheritingOps = { "+","-","*","/" ,"^","+=" ,"-=" ,"*=" ,"%=" ,"^=" };
static std::set<std::string> lhsConstraintCloningOps = { "+=" ,"-=" ,"*=" ,"%=" ,"^=" , "--", "++" };
static std::set<std::string> rhsConstraintCloningOps = { "=" };

static std::set<std::string> constantStringOps = { "TYPENAME","TOSTRG" };
static std::set<std::string> constantNumberOps = { "==" , ">" ,"<", "LENGTH" ,"&&" ,"||" , "%","!=" ,"<=" ,">=","EXIST_SUBEXPR","FIND_SUBEXPR","TONUM","TOINT","FIND" ,"COUNT" };

static std::set<std::string> nonConstraintRelatedRhsNames = { "new", "out","symbol","placeholder" };


static std::set<std::string> nonAscRelatedArgNames = { "ans", "thisexpr" };

static std::set<std::string> valueComparators = { "eq" ,"ne" ,"gte" ,"gt" ,"lte" , "lt" };
static std::set<std::string> logicComparators = { "is" ,"not" };

static std::map<std::string, std::set<std::string>> exclusiveConstraints = {
	{"string",{"number"}},
	{"number",{"string"}},
};
static std::set<std::string> nonInheritableConstraints = { "eq" ,"ne" ,"gte" ,"gt" ,"lte" , "lt", "immediate" };
#define COOL_N 0 //什么也不做
#define COOL_QS 1
#define COOL_QE 2
#define COOL_C 3
#define COOL_M 4
#define COOL_ME 5
#define COOL_FUN 6
#define COOL_R 7  //返回, 一定条件下执行
#define COOL_B 8  //分支
#define COOL_L 9  //循环
#define COOL_TD 10 //推导
#define COOL_S 11 //系统
#define COOL_Y 12 //引用
#define COOL_LST 13 //列表
#define COOL_FIL 14 //文件 原则上，COOL_FIL、COOL_LIN不能打断表达式，但大多数情况下遇到嵌入表达式的COOL_FIL、COOL_LIN会被直接忽略
#define COOL_LIN 15 //行数
#define COOL_LOAD 16
#define COOL_CTN 17 //continue
#define COOL_BRK 18 //break
#define COOL_ABT 19 //abort
#define	EXPRQS	20	//expr scope start
#define	EXPRQE	21	//expr scope end


#define Dz 0
#define Sz 1
#define Bs 2
#define Zf 3

#define S_Dz 0  //用于arg、data均可，表明其内容为数据区的地址，可用于<dataTable>键、值（特别说明，Data中不储存Dz或标识。S_Dz说明Data为空，刚初始化或者无内容）。
#define Sz 1    //用于arg，或data，可用于<dataTable>值
#define S_Bs 2  //用于arg，表明其内容为数据区的标识，可用于<dataTable>键, 不可用于值。如果要引用另一个变量，请使用S_PTR
#define Zf 3    //用于arg、data均可，表明其内容为字符，可用于<dataTable>值
#define Z_Dz 4  //用于arg，表明其内容为作用域表的地址，在<dataTable>中作为键时被替换为S_AR，代表储存着对象
#define M_Bs 5  //用于arg，表明这是一个模板/类用到的符号+-*/:<<等
#define M_Dz 6  //用于arg，表明这是一个模板用到的符号在符号表中的地址
#define X_Bs 7  //用于arg，表明这是一个作用于变量的属性，亦或是用户定义的在系统表中的类
#define X_Dz /*8*/S_Dz  //用于arg，表明这是一个作用于变量的属性，亦或是用户定义的在系统表中的类
#define S_AR 9  //通常用于data，表明这是一个类的实例或者是单纯的一个作用域，可用于<dataTable>的键、值
#define Y_Dz 10 //用于arg，表明这是一个引用，储存在引用表，可以作为Code中四元式标志位，可用于<dataTable>键，注意，类型Y_Dz的arg其rootAR不能为空
#define S_LST COOL_LST//#define COOL_LST 13 //用于arg，用于Data，表明此Data中储存着一个列表, 不可用于<dataTable>键
#define S_PTR 14//仅用于Data，用于说明对此data进行赋值、访问(查询、比大小等)操作请使用ptr所指data，但删除此data时不删除ptr所指data；
#define EXPRZ_Dz 15 //用于Arg，表明其内容仅为一个包含表达式的作用域地址，Data的类型为其根节点的类型。不可出现在Data中。请用Data::isExpression标志位。来判断是否是Expression 在<DataTable>中作为键时被替换为S_Dz，代表储存着值
//对于类型（属性）为其他的Arg中的flag: isExpression, 用于说明此Arg是一个表达式，同时，此表达式有属性，甚至有值（体现在data中）。


#define MAP S_MAP //-31, only used for data, mean a map in the data
#define MULTIMAP S_MULTIMAP//-32, only used for data, mean a multimap in the data
#define SET S_SET //-33, only used for data, mean a set in the data
#define MULTISET S_MULTISET //-34, only used for data, mean s multiset in the data


//function flag in Scope
#define FN 1   //函数名称作用域
#define FB 2   //普通函数体
#define RFB 3  //函数体，包含return
#define BRB 4  //分支的体
#define LOB 5  //循环的体
#define SYSB 6 //系统的体
#define EXPRB 7 //表达式的体，内部直接视为表达式

#define Du 2
//#define COOL_R COOL_R //一定条件下执行,不再重复定义

#define MODE_SCANNING 0  //globalRefTable
#define MODE_GROUNDING 1 //preexe模式下有ar搭配
#define MODE_EXECUTION 2

#define BUILTIN_FUN_INFO "1" //built-in function's default organization/return value.

#define EXPRESSIONHANDLER_CDT 1 //this expressionhandler with a valid codetable as expression, mutually exclusive with EXPRESSIONHANDLER_ARG.
#define EXPRESSIONHANDLER_ARG 2 //this expressionhandler with a valid arg as expression, mutually exclusive with EXPRESSIONHANDLER_CDT.




enum class ArgCmpMode {
	constraint = 1 << 0,	//if constraint need to have intersection
	changeable = 1 << 1,	//if changeable need to be consist
	unknown = 1 << 2,	//if unknown need to be consist
	compatible = 1 << 3,	//
	asc = 1 << 4,	//if arg scope need to be compared
	name = 1 << 5,	//if arg type and value to be compared
	data = 1 << 6,	//if data need to be compared, if data1 == data2 == datanull, or_data_return false.
	placeholder = 1 << 7, //if pointer need to be transed, 
	symbol = 1 << 8,  //if symbol compared
	ref = 1 << 9,  //if reference need to be transed
	identifier = 1 << 10,
	fka = 1 << 11,
	or_data = 1 << 12,//(name, identifier, fka, symbol) or (data) must be intersect. if data1 == data2 == datanull, or_data_return false.
	immediate = 1 << 13, //whether compare immediate (enable constraint first.), if set, immediate will be conflict to var.

};
#define ArgCmpMode_COUNT 13
inline ArgCmpMode operator|(ArgCmpMode a, ArgCmpMode b) {
	return static_cast<ArgCmpMode>(static_cast<int>(a) | static_cast<int>(b));
}

inline ArgCmpMode operator&(ArgCmpMode a, ArgCmpMode b) {
	return static_cast<ArgCmpMode>(static_cast<int>(a) & static_cast<int>(b));
}

typedef ArgCmpMode acm;



/*
 * Support for non-ASCII character paths
 */
#ifndef pathStr

#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <windows.h>
typedef std::string string;
typedef std::wstring wstring;

#if __WIN32__ || _WIN32
#include <windows.h>
#define pathStr std::wstring  //win systems use ANSI(GBK)
#ifndef PATH_MAX
#define PATH_MAX 260  // Windows MAX_PATH length
#endif




/// <summary>
/// center align string.
/// </summary>
/// <param name="str"></param>
/// <param name="width"></param>
/// <returns></returns>
static std::string calign(const std::string& str, int width = 16) {
	int len = str.length();
	if (width < len) {
		return str;
	}

	int diff = width - len;
	int pad1 = diff / 2;
	int pad2 = diff - pad1;

	return std::string(pad1, ' ') + str + std::string(pad2, ' ');
}
/**
 * @brief convert char * to wstring
 * @param charp
 * @param codePage
 * @param existUnknownCharacters if there are unknown character in charp, set to 1.
 * @return
 */
static std::wstring charpToWString(const char* charp, const int codePage,
	int* existUnknownCharacters) {
	int num = MultiByteToWideChar(codePage, 0, charp, -1,
		nullptr, 0);
	wchar_t* wide = new wchar_t[num + 1];
	int writeInNum = MultiByteToWideChar(codePage, MB_ERR_INVALID_CHARS, charp,
		-1, wide, num);
	std::wstring w_str(wide);
	delete[] wide;
	if (writeInNum == 0 && num != 0) {
		*existUnknownCharacters = true;
	}
	else {
		*existUnknownCharacters = false;
	}
	return w_str;
}
static  std::wstring StringToWString(const string& str) {
	auto codepagev = { CP_UTF8, CP_ACP, CP_MACCP, CP_OEMCP, CP_SYMBOL,
	CP_THREAD_ACP, CP_UTF7 };

	int existUnknownCharacters1 = false;
	wstring ws;
	for (auto pg : codepagev) {

		ws = charpToWString(str.c_str(), pg, &existUnknownCharacters1);

		if (existUnknownCharacters1 == 0) {
			break;
		}
	}
	return ws;
}
static std::string WStringToString(const wstring& wstr, const int codePage,
	int* existUnknownCharacters) {

	int iSize;

	char* strBytes;
	char rep = '?';

	iSize = WideCharToMultiByte(codePage, WC_NO_BEST_FIT_CHARS, wstr.c_str(),
		-1, nullptr, 0, nullptr,
		nullptr);

	strBytes = new char[(iSize + 1)]/**sizeof(char)*/;

	int writeInNum = WideCharToMultiByte(codePage, WC_NO_BEST_FIT_CHARS,
		wstr.c_str(), -1, strBytes, iSize, &rep, existUnknownCharacters);

	std::string str(strBytes);
	delete[] strBytes;
	if (writeInNum == 0 && iSize != 0) {
		*existUnknownCharacters = true;
	}
	else {
		*existUnknownCharacters = false;
	}
	return str;
}
/**
 * @attention this function convert a wstring into string using CP_ACP, which
 * depends on system and can be ansi or other code. don't set it as others.
 * @brief
 * @param wstr
 * @return
 */
static std::string WStringToString(const wstring& wstr) {

	string s;
	auto codepagev = { CP_ACP, CP_UTF8, CP_MACCP, CP_OEMCP, CP_SYMBOL,
	CP_THREAD_ACP, CP_UTF7 };

	int existUnknownCharacters1 = false;
	for (auto cp : codepagev) {

		s = WStringToString(wstr.c_str(), cp, &existUnknownCharacters1);
		if (existUnknownCharacters1 == false) {
			return s;
		}
	}
	std::cout
		<< "WStringToString error: unknow character exist in wstring. throw(const wstring &)("
		<< __FILE__ << ":" << "0" << ")[" << s << "]" << std::endl;
	throw wstr;
	//    std::cerr
	//            << "WStringToString error: unknow character exist in wstring. exit(-1)("
	//            << __FILE__ << ":" << "0" << ")[" << s << "]" << std::endl;
	//    exit(-1);

}
/**
 * @brief utf8 is the encoding of code files,
 * so if you want to write character codes to documents, convert them to utf8,
 *  same as below
 * @param wstr
 * @return
 */
static std::string WStringToUTF8(const wstring& wstr) {

	string s;

	int existUnknownCharacters1 = false;
	s = WStringToString(wstr.c_str(), CP_UTF8, &existUnknownCharacters1);

	return s;
}

/**
 * @brief convert char*(wstring) to utf8
 * @return
 */
static string charpToUtf8(const char* chrp) {
	auto codepagev = { CP_UTF8, CP_ACP, CP_MACCP, CP_OEMCP, CP_SYMBOL,
	CP_THREAD_ACP, CP_UTF7 };

	int existUnknownCharacters1 = false;
	int existUnknownCharacters2 = false;
	wstring ws;
	string s;
	for (auto pg : codepagev) {

		ws = charpToWString(chrp, pg, &existUnknownCharacters1);

		if (existUnknownCharacters1 == 0) {
			break;
		}
	}
	s = WStringToString(ws, CP_UTF8, &existUnknownCharacters2);
	if (existUnknownCharacters1 == 0 && existUnknownCharacters2 == 0) {

		return s;
	}
	std::cout << "Invalid character in path:" << s << ", throw(const char *)."
		<< std::endl;
	throw(chrp);
	//    std::cerr << "Invalid character in path:" << s << ", exit(-1)."
	//            << std::endl;
	//    exit(-1);
}
static pathStr& assign(pathStr& pstr, const char* chrp) {
	pstr = StringToWString(string(chrp));
	return pstr;
}
static pathStr& assign(pathStr& pstr, const string& str) {
	pstr = StringToWString(str);
	return pstr;
}
static string pathStrToStr(const pathStr& pstr) {
	return WStringToString(pstr);
}
static string pathStrToUTF8(const pathStr& pstr) {
	return WStringToUTF8(pstr);
}
static string pathToUTF8(const std::filesystem::path& path_) {
	return pathStrToUTF8(path_.wstring());
}
/**
 * @brief this function converts "filesystem::path" to ANSI code for win systems
 * @param pstr
 * @return
 */
static string pathToStr(const std::filesystem::path& path_) {
	return pathStrToStr(path_.wstring());
}
static pathStr pathToPathStr(const std::filesystem::path& path_) {
	return path_.wstring();
}
static pathStr charpToPathStr(const char* charp) {
	return StringToWString(string(charp));
}
static std::filesystem::path charpToPath(const char* charp) {
	return std::filesystem::path(charpToPathStr(charp));
}

#else
#define pathStr std::string  //other systems use utf8
static pathStr& assign(pathStr& pstr, const char* chrp) {
	pstr = chrp;
	return pstr;
}
static pathStr& assign(pathStr& pstr, const string& str) {
	pstr = str;
	return pstr;
}
/**
 * @brief this function convert "filesystem::path" to utf8 code for linux systems
 * @param pstr
 * @return
 */
static string pathToStr(const std::filesystem::path& path_) {
	return path_.string();
}
static string pathToPathStr(const std::filesystem::path& path_) {
	return path_.string();
}

static pathStr charpToPathStr(const char* charp) {
	return string(charp);
}
static std::filesystem::path charpToPath(const char* charp) {
	return std::filesystem::path(charpToPathStr(charp));
}
#endif

#endif

#endif /* COOLANG_DEFINE_H_ */
