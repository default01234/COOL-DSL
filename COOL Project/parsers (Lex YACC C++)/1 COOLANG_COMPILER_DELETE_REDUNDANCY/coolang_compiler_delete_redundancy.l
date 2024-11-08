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
	#include"coolang_define.hpp"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#ifndef fileno
	#define fileno(__F) ((__F)->_file)
	#endif


	#include "y.tab.h" 

	void yyerror(const char*);
	int num_num = 0;
	int num_id = 0;
	int word_count = 0;
//	extern char*	yylval;头文件里面有
	void st();
#if !debug
#define puts(XXX) 
#endif
%}


%s	SINGLE_COMMENT
%s	MULTI_COMMENT
%s	STRING	
%s	STRING_TRANSM
		

ID	(([^ \x00-\x7F]+)|([a-zA-Z_][a-zA-Z_0-9]*))
SPACE	("　"|"	"|" "|([ \n\t\r]))





%%




<INITIAL>"//"[^\n]*			        {/*去除注释*/puts("LEX>>Find single comment");puts(yytext);}
<INITIAL>"/*"([^\*]|(\*)*[^\*/])*(\*)*"*/"  {puts("LEX>>Find multi comment");puts(yytext);}
<INITIAL>"/\*"				{BEGIN MULTI_COMMENT;puts("LEX>>Find multi comment");puts(yytext);}
<MULTI_COMMENT>[^\*]*			{;puts(yytext);}
<MULTI_COMMENT>"*"+[^\*\/]*		{;puts(yytext);}
<MULTI_COMMENT>"\*/"			{BEGIN INITIAL;puts("LEX>>End multi comment");puts(yytext);}

<INITIAL>"\""(([^\"\n]*)|("\\\"")*)*"\""		{/*字符串*/puts("LEX>>Find string");puts(yytext);ECHO;}


<INITIAL>"#file"("("|"（").*(")"|"）")			{puts("LEX>>Find #file(.*)");puts(yytext);ECHO;}
<INITIAL>"#load"("("|"（").*(")"|"）")			{puts("LEX>>Find #load(.*)");puts(yytext);ECHO;}





<INITIAL>{SPACE}			{/*去除非字符串空格*/;puts(yytext);}

<INITIAL>[^ "\/\*\r\n\t]+		{puts("LEX>>Find useful characters");puts(yytext);ECHO;}
<INITIAL>.				{puts("LEX>>Other character");puts(yytext);ECHO;}
%%


FILE * openfile(){

	printf("in openfile\n");
	FILE *fp;
	char ch;
	
	char * buffer = NULL;
	if((buffer = _getcwd( NULL, 0 )) == NULL)perror( "_getcwd error" );
	printf( "%s \n", buffer );


	if ((fp = fopen("D:\\Flex Windows\\FlexFiles\\COOLANG_COMPILER_DELETE_REDUNDANCY\\codeTest1.txt", "r") )== NULL){
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
	if ((fp = fopen("D:\\Flex Windows\\FlexFiles\\COOLANG_COMPILER_DELETE_REDUNDANCY\\codeTest1out.txt", "w") )== NULL){
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
	yyout=precompileFile();

	if(yyin==NULL||yyout==NULL){ puts("cant open all files");system("pause");return 1;}
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
void st(){

	//strcpy(char *dest,char *src);
	strcpy(yylval.toString(),yytext);
}