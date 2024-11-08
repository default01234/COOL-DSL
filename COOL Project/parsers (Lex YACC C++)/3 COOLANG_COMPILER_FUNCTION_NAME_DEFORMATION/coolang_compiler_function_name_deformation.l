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

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

	#include"coolang_compiler_define.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>


	#include "coolang_compiler_function_name_deformation.hpp"

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
<INITIAL>"\("|"（"				{puts("lex>>Find LSCUR");puts(yytext);addToken(Token::LSCUR,yytext);}
<INITIAL>"\)"|"）"				{puts("lex>>Find RSCUR");puts(yytext);addToken(Token::RSCUR,yytext);}
<INITIAL>"\["|"【"				{puts("lex>>Find LMCUR");puts(yytext);addToken(Token::LMCUR,yytext);}
<INITIAL>"\]"|"】"				{puts("lex>>Find RMCUR");puts(yytext);addToken(Token::RMCUR,yytext);}
<INITIAL>"\{"				{puts("lex>>Find LBCUR");puts(yytext);addToken(Token::LBCUR,yytext);}
<INITIAL>"\}"				{puts("lex>>Find RBCUR");puts(yytext);addToken(Token::RBCUR,yytext);}
<INITIAL>","|"，"				{puts("lex>>Find COMMA");puts(yytext);addToken(Token::COMMA,yytext);}
<INITIAL>";"|"；"				{puts("lex>>Find SEMICOLON");puts(yytext);addToken(Token::SEMICOLON,yytext);}

\"返回\"
<INITIAL>("re:"|"return:")			{puts("lex>>Find _return_");puts(yytext);addToken(Token::_return_,yytext);}
<INITIAL>"abort"				{puts("lex>>Find _abort_");puts(yytext);addToken(Token::_abort_,yytext);}

\"运算符\"
<INITIAL>"#file"/("("|"（")				{BEGIN FILENAME;puts("lex>>Find _FIL BEGIN FILENAME");puts(yytext);addToken(Token::_FIL, yytext);}
<INITIAL>"#line"/("("|"（")				{puts("lex>>Find _LIN");puts(yytext);addToken(Token::_LIN, yytext);}
<INITIAL>"#load"/("("|"（")				{BEGIN FILENAME;puts("lex>>Find _LOAD BEGIN FILENAME");puts(yytext);addToken(Token::_LOAD, yytext);}
<FILENAME>"("|"（"				{puts("lex>>Find LSCUR IN FILENAME");puts(yytext);addToken(Token::LSCUR, yytext);}
<FILENAME>[^\(\)]*			{puts("lex>>Find FILENAME");puts(yytext);addToken(Token::arg,yytext);}
<FILENAME>")"				{BEGIN INITIAL;puts("lex>>END FILENAME, Find RSCUR");puts(yytext);addToken(Token::RSCUR,yytext);}

<INITIAL>"list"/("("|"（")				{puts("lex>>Find LIST");puts(yytext);addToken(Token::LIST, yytext);}
<INITIAL>"map"/("("|"（")				{puts("lex>>Find MAP");puts(yytext);addToken(Token::MAP, yytext);}
<INITIAL>"multimap"/("("|"（")			{puts("lex>>Find MULTIMAP");puts(yytext);addToken(Token::MULTIMAP, yytext);}
<INITIAL>"set"/("("|"（")				{puts("lex>>Find SET");puts(yytext);addToken(Token::SET, yytext);}
<INITIAL>"multiset"/("("|"（")			{puts("lex>>Find MULTISET");puts(yytext);addToken(Token::MULTISET, yytext);}
<INITIAL>"nonBlockExecute"/("("|"（")		{puts("lex>>Find NONBLOCKEXECUTE");puts(yytext);addToken(Token::NONBLOCKEXECUTE, yytext);}
<INITIAL>"blockExecute"/("("|"（")			{puts("lex>>Find BLOCKEXECUTE");puts(yytext);addToken(Token::BLOCKEXECUTE, yytext);}
<INITIAL>"sleep"/("("|"（")				{puts("lex>>Find SLEEP");puts(yytext);addToken(Token::SLEEP, yytext);}
<INITIAL>"logicjump"/("("|"（")				{puts("lex>>Find LOGICJUMP");puts(yytext);addToken(Token::LOGICJUMP, yytext);}
<INITIAL>"expr"/("{")				{puts("lex>>Find EXPR");puts(yytext);addToken(Token::EXPR,yytext);}

<INITIAL>".clear"/("("|"（")			{puts("lex>>Find CLEAR");puts(yytext);addToken(Token::CLEAR, yytext);}
<INITIAL>".erase"/("("|"（")				{puts("lex>>Find ERASE");puts(yytext);addToken(Token::ERASE, yytext);}
<INITIAL>".insert"/("("|"（")			{puts("lex>>Find INSERT");puts(yytext);addToken(Token::INSERT, yytext);}
<INITIAL>".length"/([^"("^\x00-\x7Fa-zA-Z_0-9]{1})	{puts("lex>>Find LENGTH");puts(yytext);addToken(Token::LENGTH, yytext);}
<INITIAL>".typename"/([^"("^\x00-\x7Fa-zA-Z_0-9]{1})	{puts("lex>>Find TYPENAME");puts(yytext);addToken(Token::TYPENAME,yytext);}
<INITIAL>".toInt"/("("|"（")				{puts("lex>>Find TOINT");puts(yytext);addToken(Token::TOINT,yytext);}
<INITIAL>".toNum"/("("|"（")				{puts("lex>>Find TONUM");puts(yytext);addToken(Token::TONUM,yytext);}
<INITIAL>".toStrg"/("("|"（")			{puts("lex>>Find TOSTRG");puts(yytext);addToken(Token::TOSTRG,yytext);}
<INITIAL>".back"/("("|"（")				{puts("lex>>Find BACK");puts(yytext);addToken(Token::BACK,yytext);}
<INITIAL>".pushBack"/("("|"（")			{puts("lex>>Find PUSHBACK");puts(yytext);addToken(Token::PUSHBACK,yytext);}
<INITIAL>".pushFront"/("("|"（")			{puts("lex>>Find PUSHFRONT");puts(yytext);addToken(Token::PUSHFRONT,yytext);}
<INITIAL>".popBack"/("("|"（")			{puts("lex>>Find POPBACK");puts(yytext);addToken(Token::POPBACK,yytext);}
<INITIAL>".popFront"/("("|"（")			{puts("lex>>Find POPFRONT");puts(yytext);addToken(Token::POPFRONT,yytext);}
<INITIAL>".find"/("("|"（")				{puts("lex>>Find FIND");puts(yytext);addToken(Token::FIND,yytext);}
<INITIAL>".count"/("("|"（")				{puts("lex>>Find COUNT");puts(yytext);addToken(Token::COUNT,yytext);}
<INITIAL>".existsubexpr"/("{")			{puts("lex>>Find EXIST_SUBEXPR");puts(yytext);addToken(Token::EXIST_SUBEXPR,yytext);}
<INITIAL>".findsubexpr"/("{")			{puts("lex>>Find FIND_SUBEXPR");puts(yytext);addToken(Token::FIND_SUBEXPR,yytext);}
<INITIAL>".reset"/("(")			{puts("lex>>Find RESET");puts(yytext);addToken(Token::RESET,yytext);}


<INITIAL>"~~~"([^~]{1})				{puts("lex>>Find USER_OP3");puts(yytext);addToken(Token::USER_OP3, yytext);}
<INITIAL>"~~"([^~]{1})				{puts("lex>>Find USER_OP2");puts(yytext);addToken(Token::USER_OP2, yytext);}
<INITIAL>"~"([^~]{1})				{puts("lex>>Find USER_OP1");puts(yytext);addToken(Token::USER_OP1, yytext);}
<INITIAL>"&&"					{puts("lex>>Find AND");puts(yytext);addToken(Token::AND, yytext);}
<INITIAL>"&"					{puts("lex>>Find BINARY_AND(USER_OP1)");puts(yytext);addToken(Token::USER_OP1, yytext);}
<INITIAL>"||"					{puts("lex>>Find OR");puts(yytext);addToken(Token::OR, yytext);}
<INITIAL>"|"					{puts("lex>>Find BINARY_OR(USER_OP1)");puts(yytext);addToken(Token::USER_OP1, yytext);}
<INITIAL>"!="|"！="					{puts("lex>>Find BUDENG");puts(yytext);addToken(Token::BUDENG,yytext);}
<INITIAL>"!"|"！"					{puts("lex>>Find NOT");puts(yytext);addToken(Token::NOT, yytext);}
<INITIAL>"+="					{puts("lex>>Find JIADENG");puts(yytext);addToken(Token::JIADENG, yytext);}
<INITIAL>"-="					{puts("lex>>Find JIANDENG");puts(yytext);addToken(Token::JIANDENG, yytext);}
<INITIAL>"*="					{puts("lex>>Find CHENGDENG");puts(yytext);addToken(Token::CHENGDENG, yytext);}
<INITIAL>"/="					{puts("lex>>Find CHUDENG");puts(yytext);addToken(Token::CHUDENG, yytext);}
<INITIAL>"%="					{puts("lex>>Find MODENG");puts(yytext);addToken(Token::MODENG, yytext);}
<INITIAL>"^="					{puts("lex>>Find MIDENG");puts(yytext);addToken(Token::MIDENG, yytext);}
<INITIAL>"++"					{puts("lex>>Find SI");puts(yytext);addToken(Token::SI, yytext);}
<INITIAL>"--"					{puts("lex>>Find SD");puts(yytext);addToken(Token::SD, yytext);}
<INITIAL>"%"					{puts("lex>>Find MODULO");puts(yytext);addToken(Token::MODULO, yytext);}


<INITIAL>"=="					{puts("lex>>Find DENGYU");puts(yytext);addToken(Token::DENGYU,yytext);}
<INITIAL>"=>"					{puts("lex>>Find TUIDAO");puts(yytext);addToken(Token::TUIDAO,yytext);}
<INITIAL>"<<"					{puts("lex>>Find JICHENG");puts(yytext);addToken(Token::JICHENG,yytext);}
<INITIAL>"="					{puts("lex>>Find FUZHI");puts(yytext);addToken(Token::FUZHI,yytext);}
<INITIAL>"-->"					{puts("lex>>Find SHUCHU");puts(yytext);addToken(Token::SHUCHU,yytext);}
<INITIAL>">="					{puts("lex>>Find BUXIAOYU");puts(yytext);addToken(Token::BUXIAOYU,yytext);}
<INITIAL>"<="					{puts("lex>>Find BUDAYU");puts(yytext);addToken(Token::BUDAYU,yytext);}
<INITIAL>">"/([^=>]+)				{puts("lex>>Find DAYU");puts(yytext);addToken(Token::DAYU,yytext);}
<INITIAL>"<"/([^=<]+)				{puts("lex>>Find XIAOYU");puts(yytext);addToken(Token::XIAOYU,yytext);}
<INITIAL>"+"					{puts("lex>>Find JIA");puts(yytext);addToken(Token::JIA,yytext);}
<INITIAL>"-"					{puts("lex>>Find JIAN");puts(yytext);addToken(Token::JIAN,yytext);}
<INITIAL>"*"					{puts("lex>>Find CHENG");puts(yytext);addToken(Token::CHENG,yytext);}
<INITIAL>"/"					{puts("lex>>Find CHU");puts(yytext);addToken(Token::CHU,yytext);}
<INITIAL>"^"					{puts("lex>>Find MI");puts(yytext);addToken(Token::MI,yytext);}
<INITIAL>":"|：					{puts("lex>>Find DOTDOT");puts(yytext);addToken(Token::DOTDOT,yytext);}
<INITIAL>"$"					{puts("lex>>Find CHANGEABLE");puts(yytext);addToken(Token::CHANGEABLE,yytext);}
<INITIAL>"?"					{puts("lex>>Find COMPATIBLE");puts(yytext);addToken(Token::COMPATIBLE,yytext);}
<INITIAL>"#"					{puts("lex>>Find PENDCHANGEABLE");puts(yytext);addToken(Token::PENDCHANGEABLE,yytext);}
<INITIAL>"."					{puts("lex>>Find DOT");puts(yytext);addToken(Token::DOT, yytext);}

\"函数声明属性\"
<INITIAL>("e"|"exp"|"expr"|"expression")/(":"|"\xFF\x1A")	{puts("lex>>Find function_declare_attribute e");puts(yytext);addToken(Token::function_declare_attribute,yytext);}
<INITIAL>"@"/(":"|：)			{puts("lex>>Find function_declare_attribute @:");puts(yytext);addToken(Token::function_declare_attribute,yytext);}
<INITIAL>"@"/("{")			{puts("lex>>Find function_declare_attribute @{");puts(yytext);char attr[10]= "@(0)";addToken(Token::function_declare_attribute,attr);}
<INITIAL>"@"/(((([^\x00-\x7F"　""	"" "]+)|([a-zA-Z_]+))[a-zA-Z_0-9]*)+)		{puts("lex>>Find function_declare_attribute @{FUNCTIONNAME}");puts(yytext);addToken(Token::function_declare_attribute,yytext);}
<INITIAL>"@("|"@（"				{BEGIN FUNCTION_ATTRIBUTE;puts("lex>>Find function_declare_attribute @(");puts(yytext);addToken(Token::function_declare_attribute,yytext);}
<FUNCTION_ATTRIBUTE>{SIGNEDNUMBER}(((,|，)|(;|；)){SIGNEDNUMBER})*((,)|(;))?			{puts("lex>>Find function_declare_attribute @(infovector)");puts(yytext);addToken(Token::function_declare_attribute,yytext);}
<FUNCTION_ATTRIBUTE>")"|"）"				{BEGIN INITIAL;puts("lex>>End function_declare_attribute @(number)");puts(yytext);addToken(Token::function_declare_attribute,yytext);}
<FUNCTION_ATTRIBUTE>.|\n				{puts("lex>>Unrecognized character in FUNCTION_ATTRIBUTE");puts(yytext);}

\"可变属性\"
<INITIAL>"$"/(":"|：)			{puts("lex>>Find changeable_attribute $");puts(yytext);addToken(Token::changeable_attribute,yytext);}

\"变量特有属性\"
<INITIAL>"out"/(":"|：)			{puts("lex>>Find variable_specific_attribute out");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}
<INITIAL>"new"/(":"|：)			{puts("lex>>Find variable_specific_attribute new");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}
<INITIAL>("immediate"|"imm")/(":"|：)	{puts("lex>>Find variable_specific_attribute immediate");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}
<INITIAL>("placeholder")/(":"|：)	{puts("lex>>Find variable_specific_attribute placeholder");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}

\"变量特有复合属性\"
<INITIAL>("eq"|"ne"|"gte"|"gt"|"lte"|"lt"|"not")/"("	{BEGIN VARIABLE_ATTRIBUTE;puts("lex>>Find variable_specific_attribute attr(*) attr: ");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}
<VARIABLE_ATTRIBUTE>"("					{puts("lex>>Find variable_specific_attribute begin, LSCUR: ");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}

<VARIABLE_ATTRIBUTE>(({SIGNEDNUMBER})|({UNSIGNEDNUMBER}))	{puts("lex>>Find variable_specific_attribute attr(numb) numb: ");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}
<VARIABLE_ATTRIBUTE>{ID}				{puts("lex>>Find variable_specific_attribute attr(ID) ID:");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}

<VARIABLE_ATTRIBUTE>"\"\""					{puts("lex>>Find variable_specific_attribute attr(string) string content: null ");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}
<VARIABLE_ATTRIBUTE>("\"")/([^\"])				{BEGIN VARIABLE_ATTRIBUTE_ASCIIQUOTATIONSTRING;puts("lex>>Find variable_specific_attribute attr(string) string start ");puts("[");puts(yytext);puts("]");addToken(Token::variable_specific_attribute,yytext);}
<VARIABLE_ATTRIBUTE_ASCIIQUOTATIONSTRING>((([^\"\n]*)|((\\\")*))*)		{puts("lex>>Find variable_specific_attribute attr(string) content: ");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}
<VARIABLE_ATTRIBUTE_ASCIIQUOTATIONSTRING>("\"")				{BEGIN VARIABLE_ATTRIBUTE;puts("lex>>Find variable_specific_attribute string end");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}

<VARIABLE_ATTRIBUTE>")"|"）"				{BEGIN INITIAL;puts("lex>>End variable_specific_attribute attr(*), RSCUR: ");puts(yytext); addToken(Token::variable_specific_attribute,yytext);}
<VARIABLE_ATTRIBUTE>.|\n				{puts("lex>>Unrecognized character in VARIABLE_ATTRIBUTE");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}


\"变量\"

<INITIAL>("\"")(([^\"\n]*)|("\\\"")*)*("\"")		{puts("lex>>Find string");puts(yytext);addToken(Token::arg,yytext);}
<INITIAL>{FUNCTIONNAME}/("("|（)   	{
    if (strcmp(yytext, "if") == 0) {
	puts("lex>>Find IF");puts(yytext);
	addToken( Token::IF, yytext);

    } else if (strcmp(yytext, "elif") == 0||strcmp(yytext, "elseif") == 0) {
	puts("lex>>Find ELIF");puts(yytext);
	addToken( Token::ELIF, yytext);

    } else if (strcmp(yytext, "else") == 0) {
	puts("lex>>Find ELSE");puts(yytext);
        addToken( Token::ELSE, yytext);

    } else if (strcmp(yytext, "while") == 0) {
	puts("lex>>Find WHILE");puts(yytext);
        addToken( Token::WHILE, yytext);

    }else{
        puts("lex>>Find function name");puts(yytext);
	addToken(Token::arg,yytext);

    }
}
<INITIAL>{ID}/([^"("（])	{
    if (strcmp(yytext, "if") == 0) {
	puts("lex>>Find IF");puts(yytext);
	addToken( Token::IF,  yytext);

    } else if (strcmp(yytext, "elif") == 0||strcmp(yytext, "elseif") == 0) {
	puts("lex>>Find ELIF");puts(yytext);
	addToken( Token::ELIF, yytext);

    } else if (strcmp(yytext, "else") == 0) {
	puts("lex>>Find ELSE");puts(yytext);
        addToken( Token::ELSE,  yytext);

    } else if (strcmp(yytext, "while") == 0) {
	puts("lex>>Find WHILE");puts(yytext);
        addToken( Token::WHILE, yytext);

    }else{
        puts("lex>>Find arg");puts(yytext);
	addToken(Token::arg,yytext);

    }
}
<INITIAL>{UNSIGNEDNUMBER}			{puts("lex>>Find number");puts(yytext);addToken(Token::arg,yytext);}




\"未识别字符\"
<INITIAL>.|\n				{puts("lex>>Unrecognized character");puts(yytext);}
%%

FILE * openfile(){

	printf("in openfile\n");
	FILE *fp;
	char ch;


	char * buffer = NULL;
	if((buffer = _getcwd( NULL, 0 )) == NULL)perror( "_getcwd error" );
	printf( "%s \n", buffer );


	if ((fp = fopen("C:\\nonSystemFiles\\Flex Windows\\FlexFiles\\COOLANG_COMPILER_FUNCTION_NAME_DEFORMATION\\codeTest1.txt", "r") )== NULL){
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
	if ((fp = fopen("C:\\nonSystemFiles\\Flex Windows\\FlexFiles\\COOLANG_COMPILER_FUNCTION_NAME_DEFORMATION\\codeTest1out.txt", "w") )== NULL){
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
	setOutputCodePath("C:\\nonSystemFiles\\Flex Windows\\FlexFiles\\COOLANG_COMPILER_FUNCTION_NAME_DEFORMATION\\codeTest1out.txt");
	//yyout=precompileFile();

	if(yyin==NULL||yyout==NULL){ puts("lex>>cant open all File");system("pause");return 1;}
	yylex();
	//printf("num = %d, id = %d\n", num_num, num_id);
	//因为此法分析为一循环，打印总数字数仅当使用文件操作时可读出
	flush();
	fclose(yyin);
	fclose(yyout);
	system("pause");
	return 0;
}

int yywrap()//此函数必须由用户提供,或者声明
{
    return 1;//返回1报告文件结尾0标记,为0则继续扫描
}

int main(int argc, char *argv[]){


#if debug
	pretreate();
	system("pause");
	
#else
	yyin = fopen(argv[1],"r");
	 yyout = stdout;
	setOutputCodePath(argv[2]);
	if(yyin==NULL||yyout==NULL){ puts("cant open all File");system("pause");return 1;}
	yylex();
	fclose(yyin);
	fclose(yyout);
#endif
	return 0;
}