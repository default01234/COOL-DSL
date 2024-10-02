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
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include<iostream>
	#include"coolang_define.hpp"
	int yylex(void);

	void yyerror(const char *);
	int pretreate();
	using namespace std;
	extern const char* yytext;
	//#define YYSTYPE char[100]
	#define YYERROR_VERBOSE
	#define YYDEBUG 1
	char* stradd(char* dest,const char* src1,const char* src2){
		if(dest!=src1){
			return strcat(dest,src2);
		}
		if(dest!=src2){
			return strcat(dest,src1);
		}
		return strcat(strcat(dest,src1),src2);
	}
#ifndef debug
#define debug true
#endif
#if !debug
#define puts(XXX)
#define printf(XXX1,...)
#endif

%}
%union{
	
	char	str[2048];
	char *  toString(){return str;}


}

%token	<str>	NAME	DOTDOT	LSCUR	RSCUR	LMCUR	RMCUR	LBCUR	RBCUR	COMMA

%type	<str>	normalFunName	arg	argList	argCurves	argAttr	

%debug
%glr-parser

%start	pureFun	


%% 
pureFun	:	LSCUR	normalFunName	RSCUR				{printf("[0.1]%s\n",$2);}
	|	LSCUR	normalFunName	COMMA				{printf("[0.2]%s\n",$2);}
	|	COMMA	normalFunName	COMMA				{printf("[0.3]%s\n",$2);}
	|	normalFunName		LBCUR				{printf("[0.4]%s\n",$1);}
	|	normalFunName						{printf("[0.5]%s\n",$1);}
	;
normalFunName	:	NAME	argList					{stradd($$,$1,$2);printf("[1]%s\n",$$);}						
	|	argList	NAME						{stradd($$,$1,$2);printf("[2]%s\n",$$);}					
	|	normalFunName	argList					{stradd($$,$1,$2);printf("[3]%s\n",$$);}					
	|	normalFunName	NAME					{stradd($$,$1,$2);printf("[4]%s\n",$$);}
	;

arg	:		NAME						{strcat($$,$1);printf("[5]%s\n",$$);}
	|	argAttr	NAME						{stradd($$,$1,$2);printf("[6]%s\n",$$);}	
	|	argAttr	normalFunName					{stradd($$,$1,$2);printf("[7]%s\n",$$);}
	|	normalFunName
	;
argList	:	argCurves	arg	RSCUR				{strcat(stradd($$,$1,$2),$3);printf("[8]%s|%s|%s|%s\n",$$,$1,$2,$3);}
	|	LSCUR	RSCUR						{stradd($$,$1,$2);printf("[9]%s\n",$$);}
	;
argCurves	:	argCurves	arg	COMMA			{strcat(stradd($$,$1,$2),$3);printf("[10]%s\n",$$);}
	|	LSCUR							{strcpy($$,$1);printf("[11]%s\n",$$);}
	;
argAttr	:	NAME	DOTDOT						{stradd($$,$1,$2);printf("[12]%s\n",$$);}
	|	normalFunName	DOTDOT					{stradd($$,$1,$2);printf("[13]%s\n",$$);}
	;


%%

int main(int argc, char *argv[]){



#if debug
	extern FILE * yyin;
	extern FILE * yyout ;
	pretreate();
#else
	pathStr argv1path;
	pathStr argv2path;
	assign(argv1path,argv[1]);
	assign(argv2path,argv[2]);
	auto && argv1str = WStringToString(argv1path);
	auto && argv2str = WStringToString(argv2path);
	extern FILE * yyin;
	yyin = fopen(argv1str.c_str(),"r");
	extern FILE * yyout ;
	yyout = fopen(argv2str.c_str(),"w");
	if(yyin==NULL||yyout==NULL){ puts("cant open all Files");system("pause");return 1;}
#endif
	yylex();
	//yyparse();

	puts("yacc finish!");
	fclose(yyin);
	fclose(yyout);
#if debug
	system("pause");
#endif
	return 0;
}
void yyerror(const char *s){
	printf("³ö´íÀ²\n");
	printf("%s\n", s);

}
