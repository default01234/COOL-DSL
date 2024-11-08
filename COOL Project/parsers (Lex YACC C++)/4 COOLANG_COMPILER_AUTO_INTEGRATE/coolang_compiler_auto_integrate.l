%{
/*  COOLang compiler implementation
    Copyright (C) 2022,Han JiPeng,Beijing Huagui Technology Co., Ltd

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, version LGPL-3.0-or-later.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
i
    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

	#include "coolang_compiler_define.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>


	#include "y.tab.h"
	#include "coolang_compiler_auto_integrate.hpp"

	void yyerror(const char*);
	int num_num = 0;
	int num_id = 0;
	int word_count = 0;
//	extern char*	yylval;头文件里面有



#if !debug
#define puts(XXX) 
#endif
%}


%s	SINGLE_COMMENT
%s	MULTI_COMMENT
%s	UTF8QUOTATIONSTRING
%s	ASCIIQUOTATIONSTRING
%s	VARIABLE_ATTRIBUTE_ASCIIQUOTATIONSTRING
%s	STRING_TRANSM
%s	FILENAME

%s	FUNCTION_ATTRIBUTE
%s	VARIABLE_ATTRIBUTE		

SPACE 			[ 　\n\t]+
SIGNEDNUMBER		[-+]?([0-9]+|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?)
UNSIGNEDNUMBER		([0-9]+|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?)
FUNCTIONNAME		((([^\x00-\x7F"　""	"" "]+)|([a-zA-Z_]+))[a-zA-Z_0-9]*)+
ID			((([^\x00-\x7F"　""	"" "]+)|([a-zA-Z_]+))[a-zA-Z_0-9]*)+



%%

\"提取作用域\"
<INITIAL>"\("|（				{puts("lex>>Find LSCUR");puts(yytext);yylval.argp=matchToken(1, Token::LSCUR, Bs,yytext);return LSCUR;}
<INITIAL>"\)"|"）"				{puts("lex>>Find RSCUR");puts(yytext);yylval.argp=matchToken(1, Token::RSCUR, Bs,yytext);return RSCUR;}
<INITIAL>"\["|"【"				{puts("lex>>Find LMCUR");puts(yytext);yylval.argp=matchToken(1, Token::LMCUR, Bs,yytext);return LMCUR;}
<INITIAL>"\]"|"】"				{puts("lex>>Find RMCUR");puts(yytext);yylval.argp=matchToken(1, Token::RMCUR, Bs,yytext);return RMCUR;}
<INITIAL>"\{"				{puts("lex>>Find LBCUR");puts(yytext);yylval.argp=matchToken(1, Token::LBCUR, Bs,yytext);return LBCUR;}
<INITIAL>"\}"				{puts("lex>>Find RBCUR");puts(yytext);yylval.argp=matchToken(1, Token::RBCUR, Bs,yytext);return RBCUR;}
<INITIAL>","|"，"				{puts("lex>>Find COMMA");puts(yytext);yylval.argp=matchToken(1, Token::COMMA, Bs,yytext);return COMMA;}
<INITIAL>";"|"；"				{puts("lex>>Find SEMICOLON");puts(yytext);yylval.argp=matchToken(1, Token::SEMICOLON, Bs,yytext);return SEMICOLON;}

\"返回\"
<INITIAL>("re:"|"re"|"return"|"return:")	{puts("lex>>Find _return_");puts(yytext);yylval.argp=matchToken(1, Token::_return_, Bs,yytext);return _return_;}
<INITIAL>("abort")			{puts("lex>>Find _abort_");puts(yytext);yylval.argp=matchToken(1, Token::_abort_, Bs,yytext);return _abort_;}



\"运算符\"
<INITIAL>"#file"/("("|（)			{BEGIN FILENAME;puts("lex>>Find _FIL BEGIN FILENAME");puts(yytext);yylval.argp=matchToken(1, Token::_FIL, Bs,yytext);return _FIL;}
<INITIAL>"#line"/("("|（)			{puts("lex>>Find _LIN");puts(yytext);yylval.argp=matchToken(1, Token::_LIN, Bs,yytext);return _LIN;}
<INITIAL>"#load"/("("|（)			{BEGIN FILENAME;puts("lex>>Find _LOAD BEGIN FILENAME");puts(yytext);yylval.argp=matchToken(1, Token::_LOAD, Bs,yytext);return _LOAD;}
<FILENAME>"("			{puts("lex>>Find LSCUR IN FILENAME");puts(yytext);yylval.argp=matchToken(1, Token::LSCUR, Bs,yytext);return LSCUR;}
<FILENAME>[^\(\)]*		{puts("lex>>Find FILENAME");puts(yytext);yylval.argp=matchToken(1, Token::arg,Bs,yytext);return arg;}
<FILENAME>")"			{BEGIN INITIAL;puts("lex>>END FILENAME, Find RSCUR");puts(yytext);yylval.argp=matchToken(1, Token::RSCUR, Bs,yytext);return RSCUR;}

<INITIAL>"list"/("("|（)			{puts("lex>>Find LIST");puts(yytext);yylval.argp = matchToken(1, Token::LIST, Bs, yytext);return LIST;}
<INITIAL>"map"/("("|（)			{puts("lex>>Find MAP");puts(yytext);yylval.argp=matchToken(1, Token::MAP, Bs,yytext);return MAP;}
<INITIAL>"multimap"/("("|（)		{puts("lex>>Find MULTIMAP");puts(yytext);yylval.argp=matchToken(1, Token::MULTIMAP, Bs,yytext);return MULTIMAP;}
<INITIAL>"set"/("("|（)			{puts("lex>>Find SET");puts(yytext);yylval.argp=matchToken(1, Token::SET, Bs,yytext);return SET;}
<INITIAL>"multiset"/("("|（)		{puts("lex>>Find MULTISET");puts(yytext);yylval.argp=matchToken(1, Token::MULTISET, Bs,yytext);return MULTISET;}
<INITIAL>"nonBlockExecute"/("("|（)	{puts("lex>>Find NONBLOCKEXECUTE");puts(yytext);yylval.argp=matchToken(1, Token::NONBLOCKEXECUTE, Bs,yytext);return NONBLOCKEXECUTE;}
<INITIAL>"blockExecute"/("("|（)		{puts("lex>>Find BLOCKEXECUTE");puts(yytext);yylval.argp=matchToken(1, Token::BLOCKEXECUTE, Bs,yytext);return BLOCKEXECUTE;}
<INITIAL>"sleep"/("("|（)			{puts("lex>>Find SLEEP");puts(yytext);yylval.argp=matchToken(1, Token::SLEEP, Bs,yytext);return SLEEP;}
<INITIAL>"logicjump"/("("|（)			{puts("lex>>Find LOGICJUMP");puts(yytext);yylval.argp=matchToken(1, Token::LOGICJUMP, Bs,yytext);return LOGICJUMP;}




<INITIAL>".clear"/("("|（)			{puts("lex>>Find CLEAR");puts(yytext);yylval.argp=matchToken(1, Token::CLEAR, Bs,yytext);return CLEAR;}
<INITIAL>".erase"/("("|（)			{puts("lex>>Find ERASE");puts(yytext);yylval.argp=matchToken(1, Token::ERASE, Bs,yytext);return ERASE;}
<INITIAL>".insert"/("("|（)		{puts("lex>>Find INSERT");puts(yytext);yylval.argp=matchToken(1, Token::INSERT, Bs,yytext);return INSERT;}
<INITIAL>".length"/([^"("a-zA-Z_0-9]{1})	{puts("lex>>Find LENGTH");puts(yytext);yylval.argp=matchToken(1, Token::LENGTH, Bs,yytext);return LENGTH;}
<INITIAL>".typename"/([^"("a-zA-Z_0-9]{1})	{puts("lex>>Find TYPENAME");puts(yytext);yylval.argp=matchToken(1, Token::TYPENAME, Bs,yytext);return TYPENAME;}
<INITIAL>".toInt"/("("|（)			{puts("lex>>Find TOINT");puts(yytext);yylval.argp=matchToken(1, Token::TOINT, Bs,yytext);return TOINT;}
<INITIAL>".toNum"/("("|（)			{puts("lex>>Find TONUM");puts(yytext);yylval.argp=matchToken(1, Token::TONUM, Bs,yytext);return TONUM;}
<INITIAL>".toStrg"/("("|（)		{puts("lex>>Find TOSTRG");puts(yytext);yylval.argp=matchToken(1, Token::TOSTRG, Bs,yytext);return TOSTRG;}
<INITIAL>".back"/("("|（)			{puts("lex>>Find BACK");puts(yytext);yylval.argp=matchToken(1, Token::BACK, Bs,yytext);return BACK;}
<INITIAL>".pushBack"/("("|（)		{puts("lex>>Find PUSHBACK");puts(yytext);yylval.argp=matchToken(1, Token::PUSHBACK, Bs,yytext);return PUSHBACK;}
<INITIAL>".pushFront"/("("|（)		{puts("lex>>Find PUSHFRONT");puts(yytext);yylval.argp=matchToken(1, Token::PUSHFRONT, Bs,yytext);return PUSHFRONT;}
<INITIAL>".popBack"/("("|（)		{puts("lex>>Find POPBACK");puts(yytext);yylval.argp=matchToken(1, Token::POPBACK, Bs,yytext);return POPBACK;}
<INITIAL>".popFront"/("("|（)		{puts("lex>>Find POPFRONT");puts(yytext);yylval.argp=matchToken(1, Token::POPFRONT, Bs,yytext);return POPFRONT;}
<INITIAL>".find"/("("|（)			{puts("lex>>Find FIND");puts(yytext);yylval.argp=matchToken(1, Token::FIND, Bs,yytext);return FIND;}
<INITIAL>".count"/("("|（)			{puts("lex>>Find COUNT");puts(yytext);yylval.argp=matchToken(1, Token::COUNT, Bs,yytext);return COUNT;}
<INITIAL>".existsubexpr"/("{")			{puts("lex>>Find EXIST_SUBEXPR");puts(yytext);yylval.argp=matchToken(1, Token::EXIST_SUBEXPR, Bs,yytext);return EXIST_SUBEXPR;}
<INITIAL>".findsubexpr"/("{")			{puts("lex>>Find FIND_SUBEXPR");puts(yytext);yylval.argp=matchToken(1, Token::FIND_SUBEXPR, Bs,yytext);return FIND_SUBEXPR;}
<INITIAL>("expr"|"expression")/("{")			{puts("lex>>Find EXPR");puts(yytext);const char* yt = "expression";yylval.argp=matchToken(1, Token::EXPR, Bs,yt);return EXPR;}
<INITIAL>".reset"/("(")			{puts("lex>>Find RESET");puts(yytext);yylval.argp=matchToken(1, Token::RESET, Bs,yytext);return RESET;}






<INITIAL>"~~~"([^~]{1})			{puts("lex>>Find USER_OP3");puts(yytext);yylval.argp=matchToken(1, Token::USER_OP3, Bs,yytext);return USER_OP3;}
<INITIAL>"~~"([^~]{1})			{puts("lex>>Find USER_OP2");puts(yytext);yylval.argp=matchToken(1, Token::USER_OP2, Bs,yytext);return USER_OP2;}
<INITIAL>"~"([^~]{1})			{puts("lex>>Find USER_OP1");puts(yytext);yylval.argp=matchToken(1, Token::USER_OP1, Bs,yytext);return USER_OP1;}
<INITIAL>"&&"				{puts("lex>>Find AND");puts(yytext);yylval.argp=matchToken(1, Token::AND, Bs,yytext);return AND;}
<INITIAL>"&"				{puts("lex>>Find BINARY_AND(USER_OP1)");puts(yytext);yylval.argp=matchToken(1, Token::USER_OP1, Bs,yytext);return USER_OP1;}
<INITIAL>"||"				{puts("lex>>Find OR");puts(yytext);yylval.argp=matchToken(1, Token::OR, Bs,yytext);return OR;}
<INITIAL>"|"				{puts("lex>>Find BINARY_OR(USER_OP1)");puts(yytext);yylval.argp=matchToken(1, Token::USER_OP1, Bs,yytext);return USER_OP1;}
<INITIAL>"!="|"！="				{puts("lex>>Find BUDENG");puts(yytext);yylval.argp=matchToken(1, Token::BUDENG, Bs,yytext);return BUDENG;}
<INITIAL>"!"|"！"				{puts("lex>>Find NOT");puts(yytext);yylval.argp=matchToken(1, Token::NOT, Bs,yytext);return NOT;}
<INITIAL>"=="				{puts("lex>>Find DENGYU");puts(yytext);yylval.argp=matchToken(1, Token::DENGYU, Bs,yytext);return DENGYU;}
<INITIAL>"=>"				{puts("lex>>Find TUIDAO");puts(yytext);yylval.argp=matchToken(1, Token::TUIDAO, Bs,yytext);return TUIDAO;}
<INITIAL>"<<"				{puts("lex>>Find JICHENG");puts(yytext);yylval.argp=matchToken(1, Token::JICHENG, Bs,yytext);return JICHENG;}
<INITIAL>"="				{puts("lex>>Find FUZHI");puts(yytext);yylval.argp=matchToken(1, Token::FUZHI, Bs,yytext);return FUZHI;}
<INITIAL>"-->"				{puts("lex>>Find SHUCHU");puts(yytext);yylval.argp=matchToken(1, Token::SHUCHU, Bs,yytext);return SHUCHU;}
<INITIAL>">="/([^>]+)			{puts("lex>>Find BUXIAOYU");puts(yytext);yylval.argp=matchToken(1, Token::BUXIAOYU, Bs,yytext);return BUXIAOYU;}
<INITIAL>"<="/([^<]+)			{puts("lex>>Find BUDAYU");puts(yytext);yylval.argp=matchToken(1, Token::BUDAYU, Bs,yytext);return BUDAYU;}
<INITIAL>">"/([^>=]+)			{puts("lex>>Find DAYU");puts(yytext);yylval.argp=matchToken(1, Token::DAYU, Bs,yytext);return DAYU;}
<INITIAL>"<"/([^<=]+)			{puts("lex>>Find XIAOYU");puts(yytext);yylval.argp=matchToken(1, Token::XIAOYU, Bs,yytext);return XIAOYU;}
<INITIAL>"++"				{puts("lex>>Find SI");puts(yytext);yylval.argp=matchToken(1, Token::SI, Bs,yytext);return SI;}
<INITIAL>"--"				{puts("lex>>Find SD");puts(yytext);yylval.argp=matchToken(1, Token::SD, Bs,yytext);return SD;}
<INITIAL>"+="				{puts("lex>>Find JIADENG");puts(yytext);yylval.argp=matchToken(1, Token::JIADENG, Bs,yytext);return JIADENG;}
<INITIAL>"-="				{puts("lex>>Find JIANDENG");puts(yytext);yylval.argp=matchToken(1, Token::JIANDENG, Bs,yytext);return JIANDENG;}
<INITIAL>"*="				{puts("lex>>Find CHENGDENG");puts(yytext);yylval.argp=matchToken(1, Token::CHENGDENG, Bs,yytext);return CHENGDENG;}
<INITIAL>"/="				{puts("lex>>Find CHUDENG");puts(yytext);yylval.argp=matchToken(1, Token::CHUDENG, Bs,yytext);return CHUDENG;}
<INITIAL>"%="				{puts("lex>>Find MODENG");puts(yytext);yylval.argp=matchToken(1, Token::MODENG, Bs,yytext);return MODENG;}
<INITIAL>"^="				{puts("lex>>Find MIDENG");puts(yytext);yylval.argp=matchToken(1, Token::MIDENG, Bs,yytext);return MIDENG;}
<INITIAL>"+"				{puts("lex>>Find JIA");puts(yytext);yylval.argp=matchToken(1, Token::JIA, Bs,yytext);return JIA;}
<INITIAL>"-"				{puts("lex>>Find JIAN");puts(yytext);yylval.argp=matchToken(1, Token::JIAN, Bs,yytext);return JIAN;}
<INITIAL>"*"				{puts("lex>>Find CHENG");puts(yytext);yylval.argp=matchToken(1, Token::CHENG, Bs,yytext);return CHENG;}
<INITIAL>"/"				{puts("lex>>Find CHU");puts(yytext);yylval.argp=matchToken(1, Token::CHU, Bs,yytext);return CHU;}
<INITIAL>"%"				{puts("lex>>Find MODULO");puts(yytext);yylval.argp=matchToken(1, Token::MODULO, Bs,yytext);return MODULO;}
<INITIAL>"^"				{puts("lex>>Find MI");puts(yytext);yylval.argp=matchToken(1, Token::MI, Bs,yytext);return MI;}
<INITIAL>":"|：				{puts("lex>>Find DOTDOT");puts(yytext);yylval.argp=matchToken(1, Token::DOTDOT, Bs,yytext);return DOTDOT;}
<INITIAL>"$"				{puts("lex>>Find CHANGEABLE");puts(yytext);yylval.argp=matchToken(1, Token::CHANGEABLE, Bs,yytext);return CHANGEABLE;}
<INITIAL>"?"				{puts("lex>>Find COMPATIBLE");puts(yytext);yylval.argp=matchToken(1, Token::COMPATIBLE, Bs,yytext);return COMPATIBLE;}
<INITIAL>"#"				{puts("lex>>Find PENDCHANGEABLE");puts(yytext);yylval.argp=matchToken(1, Token::PENDCHANGEABLE, Bs,yytext);return PENDCHANGEABLE;}
<INITIAL>"."				{puts("lex>>Find DOT");puts(yytext);yylval.argp=matchToken(1, Token::DOT, Bs,yytext);return DOT;}

\"函数声明属性\"
<INITIAL>("e"|"exp"|"expr"|"expression")/("("|:) 	{puts("lex>>Find function_declare_attribute e");puts(yytext);const char* yt = "expression";yylval.argp=matchToken(1, Token::function_declare_attribute, Bs,yt);return function_declare_attribute;}
<INITIAL>"@"/(":"|：)			{puts("lex>>Find function_declare_attribute @:");puts(yytext);yylval.argp=matchToken(1, Token::function_declare_attribute, Bs,yytext);return function_declare_attribute;}
<INITIAL>"@"/(((([^\x00-\x7F"　""	"" "]+)|([a-zA-Z_]+))[a-zA-Z_0-9]*)+)		{puts("lex>>Find function_declare_attribute @{FUNCTIONNAME}");puts(yytext);yylval.argp=matchToken(1, Token::function_declare_attribute, Bs,yytext);return function_declare_attribute;}
<INITIAL>"@("|"@（"				{BEGIN FUNCTION_ATTRIBUTE;puts("lex>>Find function_declare_attribute @(");puts(yytext);}
<FUNCTION_ATTRIBUTE>{SIGNEDNUMBER}(((,|，)|(;|；)){SIGNEDNUMBER})*((,)|(;))?		{puts("lex>>Find function_declare_attribute @(infovector)");puts(yytext);yylval.argp=matchToken(1, Token::function_declare_attribute, Bs,yytext);return function_declare_attribute;}
<FUNCTION_ATTRIBUTE>")"|"）"				{BEGIN INITIAL;puts("lex>>End function_declare_attribute @(number)");puts(yytext);}
<FUNCTION_ATTRIBUTE>.|\n				{puts("lex>>Unrecognized character in FUNCTION_ATTRIBUTE");puts(yytext);}

\"可变属性（弃用）\"
<INITIAL>"$"/(":"|：)			{puts("lex>>Find changeable_attribute $");puts(yytext);yylval.argp=matchToken(1, Token::changeable_attribute, Bs,yytext);return changeable_attribute;}

\"变量特有属性\"
<INITIAL>"out"/(":"|：)			{puts("lex>>Find variable_specific_attribute out");puts(yytext);yylval.argp=matchToken(1, Token::variable_specific_attribute, Bs,yytext);return variable_specific_attribute;}
<INITIAL>"new"/(":"|：)			{puts("lex>>Find variable_specific_attribute new");puts(yytext);yylval.argp=matchToken(1, Token::variable_specific_attribute, Bs,yytext);return variable_specific_attribute;}
<INITIAL>("immediate"|"imm")/(":"|：)	{puts("lex>>Find variable_specific_attribute immediate");puts(yytext);const char* yt = "immediate";yylval.argp=matchToken(1, Token::variable_specific_attribute, Bs,yt);return variable_specific_attribute;}
<INITIAL>("placeholder")/(":"|：)	{puts("lex>>Find variable_specific_attribute placeholder");puts(yytext);const char* yt = "placeholder";yylval.argp=matchToken(1, Token::variable_specific_attribute, Bs,yt);return variable_specific_attribute;}
<INITIAL>("symbol")/(":"|：)	{puts("lex>>Find variable_specific_attribute symbol");puts(yytext);const char* yt = "symbol";yylval.argp=matchToken(1, Token::variable_specific_attribute, Bs,yt);return variable_specific_attribute;}

\"变量特有复合属性\"
<INITIAL>("eq"|"ne"|"gte"|"gt"|"lte"|"lt"|"not")/"("	{BEGIN VARIABLE_ATTRIBUTE;puts("lex>>Find variable_specific_attribute attr(*) attr: ");puts(yytext);yylval.argp=matchToken(1, Token::variable_specific_attribute, Bs,yytext);return variable_specific_attribute;}
<VARIABLE_ATTRIBUTE>"("					{puts("lex>>Find variable_specific_attribute begin, LSCUR: ");puts(yytext);yylval.argp=matchToken(1, Token::LSCUR, Bs,yytext);return LSCUR;}

<VARIABLE_ATTRIBUTE>(({SIGNEDNUMBER})|({UNSIGNEDNUMBER}))	{puts("lex>>Find variable_specific_attribute attr(numb) numb: ");puts(yytext);yylval.argp=matchToken(1, Token::variable_specific_attribute, Sz,yytext);return variable_specific_attribute;}
<VARIABLE_ATTRIBUTE>{ID}				{puts("lex>>Find variable_specific_attribute attr(ID) ID:");puts(yytext);yylval.argp=matchToken(1, Token::variable_specific_attribute, Bs,yytext);return variable_specific_attribute;}

<VARIABLE_ATTRIBUTE>"\"\""					{puts("lex>>Find variable_specific_attribute attr(string) string content: null ");puts(yytext);const char* yt = "";yylval.argp=matchToken(1, Token::variable_specific_attribute,Zf,yt);return variable_specific_attribute;}
<VARIABLE_ATTRIBUTE>("\"")/([^\"])				{BEGIN VARIABLE_ATTRIBUTE_ASCIIQUOTATIONSTRING;puts("lex>>Find variable_specific_attribute attr(string) string start ");puts("[");puts(yytext);puts("]");}
<VARIABLE_ATTRIBUTE_ASCIIQUOTATIONSTRING>((([^\"\n]*)|((\\\")*))*)		{puts("lex>>Find variable_specific_attribute attr(string) content: ");puts(yytext);yylval.argp=matchToken(1, Token::variable_specific_attribute,Zf,yytext);return variable_specific_attribute;}
<VARIABLE_ATTRIBUTE_ASCIIQUOTATIONSTRING>("\"")				{BEGIN VARIABLE_ATTRIBUTE;puts("lex>>Find variable_specific_attribute string end");puts(yytext);}

<VARIABLE_ATTRIBUTE>")"|"）"				{BEGIN INITIAL;puts("lex>>End variable_specific_attribute attr(*), RSCUR: ");puts(yytext); yylval.argp=matchToken(1, Token::RSCUR, Bs,yytext);return RSCUR;}
<VARIABLE_ATTRIBUTE>.|\n				{puts("lex>>Unrecognized character in VARIABLE_ATTRIBUTE");puts(yytext);}


\"class、system属性\"
<INITIAL>"system"/(":"|：)			{puts("lex>>Find system_declare_attribute system");puts(yytext);yylval.argp=matchToken(1, Token::system_declare_attribute, Bs,yytext);return system_declare_attribute;}
<INITIAL>"class"/(":"|：)			{puts("lex>>Find system_declare_attribute class");puts(yytext);const char* yt = "system";yylval.argp=matchToken(1, Token::system_declare_attribute, Bs,yt);return system_declare_attribute;}

\"变量\"
<INITIAL>"\"\""					{puts("lex>>Find string content");puts(yytext);const char* yt = "";yylval.argp=matchToken(1, Token::arg,Zf,yt);return arg;}
<INITIAL>("\"")/([^\"])					        {BEGIN ASCIIQUOTATIONSTRING;puts("lex>>Find string start");puts("[");puts(yytext);puts("]");}
<ASCIIQUOTATIONSTRING>((([^\"\n]*)|((\\\")*))*)		{puts("lex>>Find string content");puts(yytext);yylval.argp=matchToken(1, Token::arg,Zf,yytext);return arg;}
<ASCIIQUOTATIONSTRING>("\"")				{BEGIN INITIAL;puts("lex>>Find string end");puts(yytext);}



<INITIAL>{FUNCTIONNAME}/("(") 	{
    if (strcmp(yytext, "if") == 0) {
	puts("lex>>Find IF");puts(yytext);
	yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return IF;
    } else if (strcmp(yytext, "elif") == 0||strcmp(yytext, "elseif") == 0) {
	puts("lex>>Find ELIF");puts(yytext);
	yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return ELIF;
    } else if (strcmp(yytext, "else") == 0) {
	puts("lex>>Find ELSE");puts(yytext);
        yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return ELSE;
    } else if (strcmp(yytext, "while") == 0) {
	puts("lex>>Find WHILE");puts(yytext);
        yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return WHILE;
    }else if (strcmp(yytext, "true") == 0) {
	puts("lex>>Find TRUE");puts(yytext);
        yylval.argp = matchToken(1, Token::arg, Sz, "1");
        return arg;
    }else if (strcmp(yytext, "false") == 0) {
	puts("lex>>Find FALSE");puts(yytext);
        yylval.argp = matchToken(1, Token::arg, Sz, "0");
        return arg;
    }else if (strcmp(yytext, "break") == 0) {
	puts("lex>>Find BREAK");puts(yytext);
	yylval.argp=matchToken(1, Token::BREAK, Bs,yytext);
	return BREAK;
    }else if (strcmp(yytext, "continue") == 0) {
	puts("lex>>Find CONTINUE");puts(yytext);
	yylval.argp=matchToken(1, Token::CONTINUE, Bs,yytext);
	return CONTINUE;
    }else if (strcmp(yytext, "abort") == 0) {
	puts("lex>>Find _abort_");puts(yytext);
	yylval.argp=matchToken(1, Token::_abort_, Bs,yytext);
	return _abort_;
    }else{
        puts("lex>>Find function name");puts(yytext);
	yylval.argp=matchToken(1, Token::arg,Zf,yytext);
	return arg;
    }
}
<INITIAL>{ID}/([^"("])	{
    if (strcmp(yytext, "if") == 0) {
	puts("lex>>Find IF");puts(yytext);
	yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return IF;
    } else if (strcmp(yytext, "elif") == 0||strcmp(yytext, "elseif") == 0) {
	puts("lex>>Find ELIF");puts(yytext);
	yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return ELIF;
    } else if (strcmp(yytext, "else") == 0) {
	puts("lex>>Find ELSE");puts(yytext);
        yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return ELSE;
    } else if (strcmp(yytext, "while") == 0) {
	puts("lex>>Find WHILE");puts(yytext);
        yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return WHILE;
    }else if (strcmp(yytext, "true") == 0) {
	puts("lex>>Find TRUE");puts(yytext);
        yylval.argp = matchToken(1, Token::arg, Sz, "1");
        return arg;
    }else if (strcmp(yytext, "false") == 0) {
	puts("lex>>Find FALSE");puts(yytext);
        yylval.argp = matchToken(1, Token::arg, Sz, "0");
        return arg;
    }else if (strcmp(yytext, "break") == 0) {
	puts("lex>>Find BREAK");puts(yytext);
	yylval.argp=matchToken(1, Token::BREAK, Bs,yytext);
	return BREAK;
    }else if (strcmp(yytext, "continue") == 0) {
	puts("lex>>Find CONTINUE");puts(yytext);
	yylval.argp=matchToken(1, Token::CONTINUE, Bs,yytext);
	return CONTINUE;
    }else if (strcmp(yytext, "abort") == 0) {
	puts("lex>>Find _abort_");puts(yytext);
	yylval.argp=matchToken(1, Token::_abort_, Bs,yytext);
	return _abort_;
    }else{
        puts("lex>>Find arg1");puts(yytext);
	yylval.argp=matchToken(1, Token::arg,Bs,yytext);
	return arg;
    }
}
<INITIAL>{UNSIGNEDNUMBER}			{puts("lex>>Find number");puts(yytext);yylval.argp=matchToken(1, Token::arg,Sz,yytext);return arg;}




\"未识别字符\"
<INITIAL>.|\n				{puts("lex>>Unrecognized character");puts(yytext);}
%%

FILE * openfile(){

	printf("in openfile1\n");
	FILE *fp;
	char ch;


	char * buffer = NULL;
	if((buffer = _getcwd( NULL, 0 )) == NULL)perror( "_getcwd error" );
	printf( "%s \n", buffer );


	if ((fp = fopen("C:\\nonSystemFiles\\Flex Windows\\FlexFiles\\COOLANG_COMPILER_AUTO_INTEGRATE\\codeTest1.txt", "r") )== NULL){
		printf("Cannot open file : codeTest1.txt\n");
		return NULL;
	}
	else{
        
		return fp;
	}

}

FILE * precompileFile(){
	printf("in precompileFile\n");
	FILE *fp;
	char ch;
	if ((fp = fopen("C:\\nonSystemFiles\\Flex Windows\\FlexFiles\\COOLANG_COMPILER_AUTO_INTEGRATE\\codeTest1out.txt", "w") )== NULL){
		printf("Cannot open file : codeTest1out.txt\n");
		return NULL;
	}
	else{
		return fp;
	}
}

int pretreate()
{

	//[^ \t\n]+       printf("执行REJECT");REJECT;

	yyin=openfile();
	yyout=stdout;

	if(yyin==NULL||yyout==NULL){ puts("lex>>cant open all File");system("pause");return 1;}
	//yylex();
	//printf("num = %d, id = %d\n", num_num, num_id);
	//因为此法分析为一循环，打印总数字数仅当使用文件操作时可读出
	//fclose(yyin);
	//fclose(yyout);
	//system("pause");
	return 0;
}

int yywrap()//此函数必须由用户提供,或者声明
{
    return 1;//返回1报告文件结尾0标记,为0则继续扫描
}
