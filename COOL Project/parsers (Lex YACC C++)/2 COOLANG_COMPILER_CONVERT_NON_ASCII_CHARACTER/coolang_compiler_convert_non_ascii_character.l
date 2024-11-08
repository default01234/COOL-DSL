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


   #include "coolang_compiler_define.h"
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include<cstdio>
#include"coolang_compiler_convert_non_ascii_character.hpp"
   int num_num = 0;
   int num_id = 0;
   int word_count = 0;
#if !debug
#define puts(XXX) 
#endif
	using namespace COOLANG;

	FILE * precompileFp;

%}


%s	COMMENT		
%s	ASCIIS
%s	STRING	
%s	STRING_TRANSM
		

INTEGER			[-+]?[1-9][0-9]*
SYSM			"="|"+"|"-"|"*"|"/"|"~"|"`"|"!"|"@"|"#"|"$"|"%"|"^"|"&"|"("|")"
ARG			[0-9a-zA-Z_.]*
SPACE 			["　"|"	"|" "\n\t]+
STRING			\"(((^\")|(\\\"))*)\"
KEYWORD			("若"|"循环"|"否则"|"否则若"|"设"|"含"|"真"|"假"|"是"|"否")
HANZI			[^\x00-\x7f　]*
ASCII			[\x00-\x21\x23-\x7f]*



		

%%
\"注释\"
<INITIAL>^[{SPACE}\t]*"/*"			{ BEGIN COMMENT; /* 进入多行注释处理状态 */ } 
<COMMENT>[.|{SPACE}]*"*/"[ \t]*\n	{ BEGIN 0;  /*多行注释处理结束*/} 
<INITIAL>^[{SPACE}\t]*"/*".*"*/"[ \t]*\n		{ /* 单行注释 */    }
<INITIAL>^[{SPACE}\t]*"//".*\n			{/*单行注释*/}

\"字符串\"
<INITIAL>[\"]{1}			{BEGIN STRING;puts("发现字符串");puts("lex>>Find \"1 ");puts(yytext);addToken(Token::arg,yytext);}
<STRING>(\\)				{BEGIN STRING_TRANSM;puts("lex>>Find \\ ");puts(yytext);addToken(Token::arg,yytext);}
<STRING_TRANSM>[.]{1}			{BEGIN STRING;puts("转义结束");puts("lex>>Find STRING_TRANSM");puts(yytext);addToken(Token::arg,yytext);}
<STRING>[^\x22\x5c]*				{puts("lex>>Find STRING");puts(yytext);addToken(Token::arg,yytext);}
<STRING>[\"]{1}				{BEGIN INITIAL;/*第二个引号*/puts("字符串结束");puts("lex>>Find \"2 ");puts(yytext);addToken(Token::arg,yytext);}


<INITIAL>{KEYWORD}			{    if (strcmp(yytext, "若") == 0) {
		puts("lex>>Find IF");puts(yytext);
		addToken( Token::IF, "if");
	
	    } else if (strcmp(yytext, "否则若") == 0) {
		puts("lex>>Find ELIF");puts(yytext);
		addToken( Token::ELIF, "elif");
	
	    } else if (strcmp(yytext, "否则") == 0) {
		puts("lex>>Find ELSE");puts(yytext);
		addToken( Token::ELSE, "else");

	    } else if (strcmp(yytext, "循环") == 0) {
		puts("lex>>Find WHILE");puts(yytext);
		addToken( Token::WHILE, "while");
	
	    } else if (strcmp(yytext, "真") == 0) {
		puts("lex>>Find ZHEN");puts(yytext);
		addToken( Token::arg, "1");
	
	    } else if (strcmp(yytext, "假") == 0) {
		puts("lex>>Find JIA cn");puts(yytext);
		addToken( Token::arg, "0");
	
	    } else if (strcmp(yytext, "是") == 0) {
		puts("lex>>Find SHI");puts(yytext);
	        addToken( Token::arg, "1");
	
	    } else if (strcmp(yytext, "否") == 0) {
		puts("lex>>Find FOU");puts(yytext);
	        addToken( Token::arg, "0");
	
	    }else if (strcmp(yytext, "含") == 0) {
		puts("lex>>Find HAN");puts(yytext);
		addToken( Token::arg, "include");
	
	}
    }

<INITIAL>{SPACE}  	{/*什么也不做,滤掉白字符和其它字符*/}
<INITIAL>{HANZI}	{
		puts("lex>>Find HANZI");//printf("\n汉字:%s,长度:%d,编号:%s\n",yytext,yyleng,nonASCIICodeToHex(yytext,yyleng).c_str());
		addToken(Token::changeable_attribute,nonASCIICodeToHex(yytext,yyleng).c_str());
		}
<INITIAL>{ASCII}	{puts("lex>>Find  ASCII");puts(yytext);addToken(Token::arg,yytext);}

%%


FILE * openfile(){

	printf("in openfile\n");
	FILE *fp;
	char ch;


	char * buffer = NULL;
	if((buffer = _getcwd( NULL, 0 )) == NULL)perror( "_getcwd error" );
	printf( "%s \n", buffer );


	if ((fp = fopen("D:\\Flex Windows\\FlexFiles\\COOLANG_COMPILER_CONVERT_NON_ASCII_CHARACTER\\codeTest1.txt", "r") )== NULL){
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
	if ((fp = fopen("D:\\Flex Windows\\FlexFiles\\COOLANG_COMPILER_CONVERT_NON_ASCII_CHARACTER\\codeTest1out.txt", "w") )== NULL){
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
	setOutputCodePath("D:\\Flex Windows\\FlexFiles\\COOLANG_COMPILER_CONVERT_NON_ASCII_CHARACTER\\codeTest1out.txt");
	//yyout=precompileFile();

	if(yyin==NULL||yyout==NULL){ puts("lex>>cant open all File");system("pause");return 1;}
	yylex();
	//printf("num = %d, id = %d\n", num_num, num_id);
	//因为此法分析为一循环，打印总数字数仅当使用文件操作时可读出
	fclose(yyin);
	fclose(yyout);
	//system("pause");
	return 0;
}

int yywrap()//此函数必须由用户提供,或者声明
{
    return 1;//返回1报告文件结尾0标记,为0则继续扫描
}

int main(int argc, char *argv[]){

#if debug
	pretreate();
	puts("\nFlex finish!");
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
