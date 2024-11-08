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
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include<iostream>

	#include "coolang_compiler_auto_integrate.hpp"

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

	int lineNumber = 1;

	void	setCurrentAssemblyCode();//C++实现


//#include "y.tab.h"
//static int stmtMerge (YYSTYPE x0, YYSTYPE x1);
#if !debug
#define puts(XXX) 
#endif

%}
%union{
	
	char	str[2048];
	char *  toString(){return str;}
	int argp;
	int exprp;


}


%token	<argp>	LBCUR	RBCUR	SEMICOLON
%token	<argp>  IF ELSE ELSEIF ELIF WHILE	BREAK	CONTINUE	_abort_	//TRUE	FALSE	true会用1替代,false用0替代,此处不再专门处理
%token	<argp>	function_declare_attribute	system_declare_attribute
%token	<argp>	changeable_attribute
%token	<argp>	variable_specific_attribute
%token	<argp>	attribute
%token	<argp>	arg
%token	<argp>	_return_ 



%type	<argp>	sentence	sentence_null
%type	<argp>	scopeBegin	scopeHalf	scope	expr_scope_begin	expr_scope_half	expr_scope	scopeFunctionDeclareBegin	scopeFunctionDeclareHalf	scopeFunctionDeclareEnd	scopeFunctionDeclare
%type	<argp>	autoScopeBegin	autoScopeEnd	autoScopeHalf	autoScope
%type	<argp>	functionDeclare	functionDeclareBegin	functionDeclareHalf	functionImplement
%type	<argp>	systemDeclare	systemDeclareBegin	systemDeclareHalf	systemImplement
%type	<argp>	list	listBegin	listHalf	listEnd
%type	<argp>	listAccess	listAccessBegin	listAccessHalf	listAccessEnd
%type	<argp>	_arg_
%type	<argp>	variable_composed_attribute
%type	<argp>	expression	expression_null	expressionFunctionDeclareRoot	
%type	<argp>	expressionForList	strictExpression	expressionForListAccess	
%type	<argp>	branch		branchBegin	branchHalf	branchEnd
%type	<argp>	loop
%type	<argp>	environment	environment_null

%right	<argp>	LOWEST
%right	<argp>	LOW
%right	<argp> USER_OP2	//用户定义的右结合运算符，(一元或两元，右侧必须有操作数)
%left	<argp>	_return_low
%left	<argp>	_return_high
%left	<argp>	COMMA
%left	<argp>	TUIDAO
%left	<argp>	JICHENG
%right	<argp>	SHUCHU
%right	<argp>	FUZHI	JIADENG	JIANDENG	CHENGDENG	CHUDENG	MODENG	MIDENG	//+= -= *= /= %= ^=  
%left	<argp>	AND	OR
%left	<argp> USER_OP1	//用户定义的左结合运算符，（一元或两元，右侧必须有操作数）
%left	<argp>	DENGYU	BUDENG 
%left	<argp>	DAYU	XIAOYU	BUDAYU	BUXIAOYU
%left	<argp>	JIA	JIAN	
%nonassoc	<argp>	NEG
%left	<argp>	CHENG	CHU	MODULO
%left	<argp>	MI
%nonassoc	<argp>	SI	SD	//自增自减
%nonassoc	<argp>	NOT
%nonassoc	<argp> USER_OP3	//一元运算符，左侧必须有操作数
%left	<argp>	LSCUR	
%left	<argp>	RSCUR
%left	<argp>	CALL	//用RSCUR的优先级代表CALL的优先级，因为CALL运算符始终无法出现
%right	<argp>	DOTDOT
%right	<argp>	CHANGEABLE	PENDCHANGEABLE	COMPATIBLE
%left	<argp>	LMCUR	
%left	<argp>	RMCUR
%left	<argp>	CLEAR	ERASE	INSERT	LENGTH	TYPENAME	TONUM	TOINT	TOSTRG	FIND	COUNT	EXIST_SUBEXPR	FIND_SUBEXPR	EXPR	RESET	NONBLOCKEXECUTE		BLOCKEXECUTE	SLEEP	LOGICJUMP	PUSHBACK	PUSHFRONT	POPBACK	POPFRONT	BACK
%left	<argp>	LIST	MAP	MULTIMAP	SET	MULTISET
%nonassoc <argp> _LOAD	_FIL	_LIN
%left	<argp>	DOT	
%right	<argp>	HIGHEST	


%debug
%glr-parser


%start	code	
%error-verbose

%% 
code:	
		environment			%dprec 1				{puts("yacc>>code:environment");matchRule(0, Rule::code___environment, NULL,NULL,NULL,NULL);}
	|	code	environment		%dprec 2				{puts("yacc>>code:code	environment");matchRule(0, Rule::code___code_environment, NULL,NULL,NULL,NULL);}
	;
environment_null:									{puts("yacc>>environment_null:NULL");matchRule(0, Rule::environment_null___NULL, NULL,NULL,NULL,NULL);}
	;
environment:	functionImplement		%dprec 5				{puts("yacc>>environment:functionImplement");matchRule(0, Rule::environment___functionImplement, NULL,NULL,NULL,NULL);}
	|	systemImplement			%dprec 5				{puts("yacc>>environment:systemImplement");matchRule(0, Rule::environment___systemImplement, NULL,NULL,NULL,NULL);}
	|	scope				%dprec 3				{puts("yacc>>environment:scope");matchRule(0, Rule::environment___scope, NULL,NULL,NULL,NULL);}
	|	sentence			%dprec 2				{puts("yacc>>environment:sentence");matchRule(0, Rule::environment___sentence, NULL,NULL,NULL,NULL);}
	;

//strictExpression中不允许以COMMA连接两个表达式，其他与expression完全一致
strictExpression:	_arg_										{puts("yacc>>strictExpression:_arg_");matchRule(0, Rule::expression____arg_,&$1,NULL,NULL,&$$);}
	|	strictExpression	MI	strictExpression		%prec	MI		{puts("yacc>>strictExpression:strictExpression	MI	strictExpression");matchRule(1, Rule::expression___expression_MI_expression,&$1,&$3,&$2,&$$);}
	|	JIAN	strictExpression					%prec	NEG		{puts("yacc>>strictExpression:JIAN	strictExpression");matchRule(0, Rule::expression___JIAN_expression,NULL,&$2,&$1,&$$);}
	|	JIA	strictExpression					%prec	NEG		{puts("yacc>>strictExpression:JIA	strictExpression");matchRule(0, Rule::expression___JIA_expression,NULL,&$2,&$1,&$$);}
	|	NOT	strictExpression					%prec	NOT		{puts("yacc>>strictExpression:NOT	strictExpression");matchRule(1, Rule::expression___NOT_expression,NULL,&$2,&$1,&$$);}
	//|	USER_OP1	strictExpression				%prec	USER_OP1	{puts("yacc>>strictExpression:USER_OP1	strictExpression");matchRule(1, Rule::expression___USER_OP1_expression,NULL,&$2,&$1,&$$);}
	|	USER_OP2	strictExpression				%prec	USER_OP2	{puts("yacc>>strictExpression:USER_OP2	strictExpression");matchRule(1, Rule::expression___USER_OP2_expression,NULL,&$2,&$1,&$$);}
	|	strictExpression	USER_OP3				%prec	USER_OP3	{puts("yacc>>strictExpression:strictExpression	USER_OP3");matchRule(1, Rule::expression___expression_USER_OP3,&$1,NULL,&$2,&$$);}
	|	SI	strictExpression					%prec	SI		{puts("yacc>>strictExpression:SI	strictExpression");matchRule(1, Rule::expression___SI_expression,NULL,&$2,&$1,&$$);}
	|	SD	strictExpression					%prec	SD		{puts("yacc>>strictExpression:SD	strictExpression");matchRule(1, Rule::expression___SD_expression,NULL,&$2,&$1,&$$);}
	|	strictExpression	SI					%prec	SI		{puts("yacc>>strictExpression:strictExpression	SI");matchRule(1, Rule::expression___expression_SI,&$1,NULL,&$2,&$$);}
	|	strictExpression	SD					%prec	SD		{puts("yacc>>strictExpression:strictExpression	SD");matchRule(1, Rule::expression___expression_SD,&$1,NULL,&$2,&$$);}
	|	strictExpression	AND	strictExpression		%prec	AND		{puts("yacc>>strictExpression:strictExpression	AND	strictExpression");matchRule(1, Rule::expression___expression_AND_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	OR	strictExpression		%prec	OR		{puts("yacc>>strictExpression:strictExpression	OR	strictExpression");matchRule(1, Rule::expression___expression_OR_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	USER_OP1	strictExpression	%prec	AND		{puts("yacc>>strictExpression:strictExpression	USER_OP1	strictExpression");matchRule(1, Rule::expression___expression_USER_OP1_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	USER_OP2	strictExpression	%prec	AND		{puts("yacc>>strictExpression:strictExpression	USER_OP2	strictExpression");matchRule(1, Rule::expression___expression_USER_OP2_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	DOT	_arg_				%prec	DOT		{puts("yacc>>strictExpression:strictExpression	DOT	strictExpression");matchRule(1, Rule::expression___expression_DOT_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	LMCUR	strictExpression	RMCUR	%prec	LMCUR		{}
	|	strictExpression	JIA	strictExpression		%prec	JIA		{puts("yacc>>strictExpression:strictExpression	JIA	strictExpression");matchRule(1, Rule::expression___expression_JIA_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	JIAN	strictExpression		%prec	JIAN		{puts("yacc>>strictExpression:strictExpression	JIAN	strictExpression");matchRule(1, Rule::expression___expression_JIAN_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	CHENG	strictExpression		%prec	CHENG		{puts("yacc>>strictExpression:strictExpression	CHENG	strictExpression");matchRule(1, Rule::expression___expression_CHENG_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	CHU	strictExpression		%prec	CHU		{puts("yacc>>strictExpression:strictExpression	CHU	strictExpression");matchRule(1, Rule::expression___expression_CHU_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	MODULO	strictExpression		%prec	MODULO		{puts("yacc>>strictExpression:strictExpression	MODULO	strictExpression");matchRule(1, Rule::expression___expression_MODULO_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	JIADENG	strictExpression		%prec	JIADENG		{puts("yacc>>strictExpression:strictExpression	JIADENG	strictExpression");matchRule(1, Rule::expression___expression_JIADENG_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	JIANDENG	strictExpression	%prec	JIANDENG	{puts("yacc>>strictExpression:strictExpression	JIANDENG	strictExpression");matchRule(1, Rule::expression___expression_JIANDENG_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	CHENGDENG	strictExpression	%prec	CHENGDENG	{puts("yacc>>strictExpression:strictExpression	CHENGDENG	strictExpression");matchRule(1, Rule::expression___expression_CHENGDENG_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	CHUDENG	strictExpression		%prec	CHUDENG		{puts("yacc>>strictExpression:strictExpression	CHUDENG	strictExpression");matchRule(1, Rule::expression___expression_CHUDENG_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	MODENG	strictExpression		%prec	MODENG		{puts("yacc>>strictExpression:strictExpression	MODENG	strictExpression");matchRule(1, Rule::expression___expression_MODENG_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	MIDENG	strictExpression		%prec	MIDENG		{puts("yacc>>strictExpression:strictExpression	MIDENG	strictExpression");matchRule(1, Rule::expression___expression_MIDENG_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	FUZHI	strictExpression		%prec	FUZHI		{puts("yacc>>strictExpression:strictExpression	FUZHI	strictExpression");matchRule(1, Rule::expression___expression_FUZHI_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	DENGYU	strictExpression		%prec	DENGYU		{puts("yacc>>strictExpression:strictExpression	DENGYU	strictExpression");matchRule(1, Rule::expression___expression_DENGYU_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	BUDENG	strictExpression		%prec	BUDENG		{puts("yacc>>strictExpression:strictExpression	BUDENG	strictExpression");matchRule(1, Rule::expression___expression_BUDENG_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	DAYU	strictExpression		%prec	DAYU		{puts("yacc>>strictExpression:strictExpression	DAYU	strictExpression");matchRule(1, Rule::expression___expression_DAYU_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	XIAOYU	strictExpression		%prec	XIAOYU		{puts("yacc>>strictExpression:strictExpression	XIAOYU	strictExpression");matchRule(1, Rule::expression___expression_XIAOYU_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	BUDAYU	strictExpression		%prec	BUDAYU		{puts("yacc>>strictExpression:strictExpression	BUDAYU	strictExpression");matchRule(1, Rule::expression___expression_BUDAYU_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	BUXIAOYU	strictExpression	%prec	BUXIAOYU	{puts("yacc>>strictExpression:strictExpression	BUXIAOYU	strictExpression");matchRule(1, Rule::expression___expression_BUXIAOYU_expression,&$1,&$3,&$2,&$$);}
	|	strictExpression	DOTDOT	strictExpression		%prec	DOTDOT		{puts("yacc>>strictExpression:strictExpression	DOTDOT	strictExpression");matchRule(1, Rule::expression___expression_DOTDOT_expression,&$3,&$1,&$2,&$$);}
	|	strictExpression	SHUCHU	strictExpression		%prec	SHUCHU		{puts("yacc>>strictExpression:strictExpression	SHUCHU	strictExpression");matchRule(1, Rule::expression___expression_SHUCHU_expression,&$1,&$3,&$2,&$$);}
	|	LSCUR	expression	RSCUR								{puts("yacc>>strictExpression:LSCUR	expression	RSCUR");matchRule(0, Rule::expression___LSCUR_expression_RSCUR,&$2,NULL,NULL,&$$);}
	|	expression	LMCUR	listAccess	RMCUR			%prec	LMCUR		{puts("yacc>>strictExpression:expression	LMCUR	listAccess	RMCUR");matchRule(1, Rule::expression___expression_LMCUR_listAccess_RMCUR,&$1,&$3,NULL,&$$);}
	|	expression	LSCUR	expression_null	RSCUR			%prec	LSCUR		{puts("yacc>>strictExpression:expression	LSCUR	expression_null	RSCUR");matchRule(1, Rule::expression___expression_LSCUR_expression_null_RSCUR,&$1,NULL,NULL,&$$);}
	|	expression	LSCUR	expression	RSCUR			%prec	LSCUR		{puts("yacc>>strictExpression:expression	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_LSCUR_expression_RSCUR,&$1,&$3,NULL,&$$);}
	;



expressionForList:	strictExpression			{puts("yacc>>expressionForList:	strictExpression");matchRule(0, Rule::expressionForList___strictExpression,&$1,NULL,NULL,&$$);}
	;
listBegin:	LBCUR						{puts("yacc>>listBegin:	LBCUR");matchRule(1, Rule::listBegin___LBCUR,&$1,NULL,NULL,&$$);}
	|	LMCUR						{puts("yacc>>listBegin:	LMCUR");matchRule(1, Rule::listBegin___LBCUR,&$1,NULL,NULL,&$$);}
	;
listHalf:	listBegin	expressionForList		{puts("yacc>>listHalf:listBegin	expressionForList");matchRule(1, Rule::listHalf___listBegin_expressionForList,&$1,&$2,NULL,&$$);}
	|	listHalf	COMMA	expressionForList	{puts("yacc>>listHalf:listHalf	COMMA	expressionForList");matchRule(1, Rule::listHalf___listBegin_expressionForList,&$1,&$3,NULL,&$$);}
	;
listEnd	:	RBCUR						{puts("yacc>>listEnd:RBCUR");}
	|	RMCUR						{puts("yacc>>listEnd:RMCUR");}
	;
list	:	listBegin	listEnd				{puts("list:listBegin	listEnd");matchRule(0, Rule::list___listBegin_listEnd,&$1,NULL,NULL,&$$);}
	|	listHalf	listEnd				{puts("list:listHalf	listEnd");matchRule(0, Rule::list___listHalf_listEnd,&$1,NULL,NULL,&$$);}	
	;


expressionForListAccess:	strictExpression	{puts("yacc>>expressionForList:	strictExpression");matchRule(0, Rule::expressionForList___strictExpression,&$1,NULL,NULL,&$$);}
	;
listAccessBegin:					{puts("yacc>>listAccessBegin:NULL");$$=0;matchRule(1, Rule::listAccessBegin___NULL,NULL,NULL,NULL,&$$);}
	;
listAccessHalf:		listAccessBegin	expressionForListAccess	{puts("yacc>>listAccessHalf:listAccessBegin	expressionForListAccess");matchRule(1, Rule::listAccessHalf___listAccessBegin_expressionForListAccess,&$1,&$2,NULL,&$$);}
	|	listAccessHalf	COMMA	expressionForListAccess	{puts("yacc>>listAccessHalf:listAccessHalf	COMMA	expressionForListAccess");matchRule(1, Rule::listAccessHalf___listAccessBegin_expressionForListAccess,&$1,&$3,NULL,&$$);}
	;
listAccessEnd:		{puts("yacc>>listAccessEnd:NULL");}
	;
listAccess:	listAccessBegin	listAccessEnd	{puts("yacc>>listAccess:listAccessBegin	listAccessEnd");matchRule(0, Rule::listAccess___listAccessBegin_listAccessEnd,&$1,NULL,NULL,&$$);}
	|	listAccessHalf	listAccessEnd	{puts("yacc>>listAccess:listAccessHalf	listAccessEnd");matchRule(0, Rule::listAccess___listAccessHalf_listAccessEnd,&$1,NULL,NULL,&$$);}
	;

variable_composed_attribute:	variable_specific_attribute	LSCUR	variable_specific_attribute	RSCUR	{puts("yacc>>variable_composed_attribute:	variable_specific_attribute	LSCUR	variable_specific_attribute	RSCUR");matchRule(1, Rule::variable_specific_attribute___variable_specific_attribute_LSCUR_variable_specific_attribute_RSCUR,&$1,&$3,NULL,&$$);}
	|	variable_specific_attribute	LSCUR	variable_composed_attribute	RSCUR	{puts("yacc>>variable_composed_attribute:	variable_specific_attribute	LSCUR	variable_composed_attribute	RSCUR");matchRule(1, Rule::variable_specific_attribute___variable_specific_attribute_LSCUR_variable_specific_attribute_RSCUR,&$1,&$3,NULL,&$$);}
	;
_arg_:		arg									{puts("yacc>>_arg_:arg");matchRule(0, Rule::_arg____arg, &$1,NULL,NULL,&$$);}
	|	CHANGEABLE	_arg_							{puts("yacc>>_arg_:CHANGEABLE	arg");matchRule(1, Rule::_arg____CHANGEABLE_arg,&$2,NULL,&$1,&$$);}	
	|	PENDCHANGEABLE	_arg_							{puts("yacc>>_arg_:PENDCHANGEABLE	arg");matchRule(1, Rule::_arg____PENDCHANGEABLE_arg,&$2,NULL,&$1,&$$);}	
	|	COMPATIBLE	_arg_							{puts("yacc>>_arg_:COMPATIBLE	arg");matchRule(1, Rule::_arg____COMPATIBLE_arg,&$2,NULL,&$1,&$$);}	
	|	variable_specific_attribute	DOTDOT	_arg_				{puts("yacc>>_arg_:variable_specific_attribute	DOTDOT	_arg_");matchRule(1, Rule::_arg____variable_specific_attribute_DOTDOT__arg_, &$3,&$1,&$2,&$$);}
	|	variable_composed_attribute	DOTDOT	_arg_				{puts("yacc>>_arg_:variable_composed_attribute	DOTDOT	_arg_");matchRule(1, Rule::_arg____variable_specific_attribute_DOTDOT__arg_, &$3,&$1,&$2,&$$);}
	|	list									{puts("yacc>>_arg_:list");matchRule(0, Rule::_arg____list, &$1,NULL,NULL,&$$);}
	;


expression_null:									{puts("yacc>>expression_null:NULL");matchRule(0, Rule::expression_null___NULL, NULL,NULL,NULL,NULL);}
	;
expression:	_arg_									{puts("yacc>>expression:_arg_");matchRule(0, Rule::expression____arg_,&$1,NULL,NULL,&$$);}
	|	expression	MI	expression			%prec	MI		{puts("yacc>>expression:expression	MI	expression");matchRule(1, Rule::expression___expression_MI_expression,&$1,&$3,&$2,&$$);}
	|	JIAN	expression					%prec	NEG		{puts("yacc>>expression:JIAN	expression");matchRule(0, Rule::expression___JIAN_expression,NULL,&$2,&$1,&$$);}
	|	JIA	expression					%prec	NEG		{puts("yacc>>expression:JIA	expression");matchRule(0, Rule::expression___JIA_expression,NULL,&$2,&$1,&$$);}
	|	NOT	expression					%prec	NOT		{puts("yacc>>expression:NOT	expression");matchRule(1, Rule::expression___NOT_expression,NULL,&$2,&$1,&$$);}
	//|	USER_OP1	expression				%prec	USER_OP1	{puts("yacc>>expression:USER_OP1	expression");matchRule(1, Rule::expression___USER_OP1_expression,NULL,&$2,&$1,&$$);}
	|	USER_OP2	expression				%prec	USER_OP2	{puts("yacc>>expression:USER_OP2	expression");matchRule(1, Rule::expression___USER_OP2_expression,NULL,&$2,&$1,&$$);}
	|	expression	USER_OP3				%prec	USER_OP3	{puts("yacc>>expression:expression	USER_OP3");matchRule(1, Rule::expression___expression_USER_OP3,&$1,NULL,&$2,&$$);}
	|	SI	expression					%prec	SI		{puts("yacc>>expression:SI	expression");matchRule(1, Rule::expression___SI_expression,NULL,&$2,&$1,&$$);}
	|	SD	expression					%prec	SD		{puts("yacc>>expression:SD	expression");matchRule(1, Rule::expression___SD_expression,NULL,&$2,&$1,&$$);}
	|	expression	SI					%prec	SI		{puts("yacc>>expression:expression	SI");matchRule(1, Rule::expression___expression_SI,&$1,NULL,&$2,&$$);}
	|	expression	SD					%prec	SD		{puts("yacc>>expression:expression	SD");matchRule(1, Rule::expression___expression_SD,&$1,NULL,&$2,&$$);}
	|	expression	DOT	_arg_				%prec	DOT		{puts("yacc>>expression:expression	DOT	expression");matchRule(1, Rule::expression___expression_DOT_expression,&$1,&$3,&$2,&$$);}
	|	expression	CLEAR	LSCUR	RSCUR			%prec	CLEAR		{puts("yacc>>expression:expression	CLEAR	LSCUR	RSCUR");matchRule(1, Rule::expression___expression_CLEAR,&$1,NULL,&$2,&$$);}
	|	expression	ERASE	LSCUR	listAccess	RSCUR	%prec	ERASE		{puts("yacc>>expression:expression	ERASE	LSCUR	listAccess	RSCUR");matchRule(1, Rule::expression___expression_ERASE_LSCUR_listAccess_RSCUR,&$1,&$4,&$2,&$$);}
	|	LIST	LSCUR	RSCUR					%prec	LIST		{puts("yacc>>expression:LIST	LSCUR	RSCUR");matchRule(1, Rule::expression___LIST_LSCUR_RSCUR,NULL,NULL,&$1,&$$);}
	|	MAP	LSCUR	RSCUR					%prec	MAP		{puts("yacc>>expression:MAP	LSCUR	RSCUR");matchRule(1, Rule::expression___MAP_LSCUR_RSCUR,NULL,NULL,&$1,&$$);}
	|	MULTIMAP	LSCUR	RSCUR				%prec	MULTIMAP	{puts("yacc>>expression:MULTIMAP	LSCUR	RSCUR");matchRule(1, Rule::expression___MULTIMAP_LSCUR_RSCUR,NULL,NULL,&$1,&$$);}
	|	SET	LSCUR	RSCUR					%prec	SET		{puts("yacc>>expression:SET	LSCUR	RSCUR");matchRule(1, Rule::expression___SET_LSCUR_RSCUR,NULL,NULL,&$1,&$$);}
	|	MULTISET	LSCUR	RSCUR				%prec	MULTISET	{puts("yacc>>expression:MULTISET	LSCUR	RSCUR");matchRule(1, Rule::expression___MULTISET_LSCUR_RSCUR,NULL,NULL,&$1,&$$);}
	|	expression	LENGTH					%prec	LENGTH		{puts("yacc>>expression:expression	LENGTH");matchRule(1, Rule::expression___expression_LENGTH,&$1,NULL,&$2,&$$);}
	|	expression	TYPENAME				%prec	TYPENAME	{puts("yacc>>expression:expression	TYPENAME");matchRule(1, Rule::expression___expression_TYPENAME,&$1,NULL,&$2,&$$);}
	|	expression	TOINT	LSCUR	RSCUR			%prec	TOINT		{puts("yacc>>expression:expression	TOINT	LSCUT	RSCUR");matchRule(1, Rule::expression___expression_TOINT_LSCUR_RSCUR,&$1,NULL,&$2,&$$);}
	|	expression	TONUM	LSCUR	RSCUR			%prec	TONUM		{puts("yacc>>expression:expression	TONUM	LSCUT	RSCUR");matchRule(1, Rule::expression___expression_TONUM_LSCUR_RSCUR,&$1,NULL,&$2,&$$);}
	|	expression	TOSTRG	LSCUR	RSCUR			%prec	TOSTRG		{puts("yacc>>expression:expression	TOSTRG	LSCUT	RSCUR");matchRule(1, Rule::expression___expression_TOSTRG_LSCUR_RSCUR,&$1,NULL,&$2,&$$);}
	|	NONBLOCKEXECUTE	LSCUR	expression	RSCUR		%prec	NONBLOCKEXECUTE	{puts("yacc>>expression:NONBLOCKEXECUTE	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___NONBLOCKEXECUTE_LSCUR_expression_RSCUR,&$3,NULL,&$1,&$$);}
	|	BLOCKEXECUTE	LSCUR	expression	RSCUR		%prec	BLOCKEXECUTE	{puts("yacc>>expression:BLOCKEXECUTE	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___BLOCKEXECUTE_LSCUR_expression_RSCUR,&$3,NULL,&$1,&$$);}	
	|	SLEEP	LSCUR	expression	RSCUR			%prec	SLEEP		{puts("yacc>>expression:SLEEP	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___SLEEP_LSCUR_expression_RSCUR,&$3,NULL,&$1,&$$);}	
	|	LOGICJUMP	LSCUR	expression	RSCUR		%prec	LOGICJUMP		{puts("yacc>>expression:LOGICJUMP	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___LOGICJUMP_LSCUR_expression_RSCUR,&$3,NULL,&$1,&$$);}	
	|	expression	INSERT	LSCUR	expression	RSCUR	%prec	INSERT		{puts("yacc>>expression:expression	INSERT	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_INSERT_LSCUR_expression_RSCUR,&$1,&$4,&$2,&$$);}
	|	expression	FIND	LSCUR	expression	RSCUR	%prec	FIND		{puts("yacc>>expression:expression	FIND	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_FIND_LSCUR_expression_RSCUR,&$1,&$4,&$2,&$$);}
	|	expression	COUNT	LSCUR	expression	RSCUR	%prec	COUNT		{puts("yacc>>expression:expression	COUNT	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_COUNT_LSCUR_expression_RSCUR,&$1,&$4,&$2,&$$);}
	|	EXPR	expr_scope				%prec	EXPR		{puts("yacc>>expression:EXPR	expr_scope");matchRule(0, Rule::expression___EXPR_expr_scope,&$2,NULL,NULL,&$$);}
	|	expression	EXIST_SUBEXPR	expr_scope		%prec	EXIST_SUBEXPR		{puts("yacc>>expression:expression	EXIST_SUBEXPR	expr_scope");matchRule(1, Rule::expression___expression_EXIST_SUBEXPR_expr_scope,&$1,&$3,&$2,&$$);}
	|	expression	FIND_SUBEXPR	expr_scope		%prec	FIND_SUBEXPR		{puts("yacc>>expression:expression	FIND_SUBEXPR	expr_scope");matchRule(1, Rule::expression___expression_FIND_SUBEXPR_expr_scope,&$1,&$3,&$2,&$$);}
	|	expression	RESET	LSCUR	RSCUR			%prec	FIND_SUBEXPR		{puts("yacc>>expression:expression	RESET");matchRule(1, Rule::expression___RESET,&$1,NULL,&$2,&$$);}
	|	expression	PUSHBACK	LSCUR	expression	RSCUR	%prec	PUSHBACK		{puts("yacc>>expression:expression	PUSHBACK	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_PUSHBACK_LSCUR_expression_RSCUR,&$1,&$4,&$2,&$$);}
	|	expression	PUSHFRONT	LSCUR	expression	RSCUR	%prec	PUSHFRONT		{puts("yacc>>expression:expression	PUSHFRONT	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_PUSHFRONT_LSCUR_expression_RSCUR,&$1,&$4,&$2,&$$);}
	|	expression	POPBACK	LSCUR	RSCUR			%prec	POPBACK		{puts("yacc>>expression:expression	POPBACK	LSCUT	RSCUR");matchRule(1, Rule::expression___expression_POPBACK_LSCUR_RSCUR,&$1,NULL,&$2,&$$);}
	|	expression	POPFRONT	LSCUR	RSCUR		%prec	POPFRONT	{puts("yacc>>expression:expression	POPFRONT	LSCUT	RSCUR");matchRule(1, Rule::expression___expression_POPFRONT_LSCUR_RSCUR,&$1,NULL,&$2,&$$);}
	|	expression	BACK	LSCUR	RSCUR			%prec	BACK		{puts("yacc>>expression:expression	BACK	LSCUT	RSCUR");matchRule(1, Rule::expression___expression_BACK_LSCUR_RSCUR,&$1,NULL,&$2,&$$);}
	|	expression	JIA	expression			%prec	JIA		{puts("yacc>>expression:expression	JIA	expression");matchRule(1, Rule::expression___expression_JIA_expression,&$1,&$3,&$2,&$$);}
	|	expression	JIAN	expression			%prec	JIAN		{puts("yacc>>expression:expression	JIAN	expression");matchRule(1, Rule::expression___expression_JIAN_expression,&$1,&$3,&$2,&$$);}
	|	expression	CHENG	expression			%prec	CHENG		{puts("yacc>>expression:expression	CHENG	expression");matchRule(1, Rule::expression___expression_CHENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	CHU	expression			%prec	CHU		{puts("yacc>>expression:expression	CHU	expression");matchRule(1, Rule::expression___expression_CHU_expression,&$1,&$3,&$2,&$$);}
	|	expression	MODULO	expression			%prec	MODULO		{puts("yacc>>expression:expression	MODULO	expression");matchRule(1, Rule::expression___expression_MODULO_expression,&$1,&$3,&$2,&$$);}
	|	expression	JIADENG	expression			%prec	JIADENG		{puts("yacc>>expression:expression	JIADENG	expression");matchRule(1, Rule::expression___expression_JIADENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	JIANDENG	expression		%prec	JIANDENG	{puts("yacc>>expression:expression	JIANDENG	expression");matchRule(1, Rule::expression___expression_JIANDENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	CHENGDENG	expression		%prec	CHENGDENG	{puts("yacc>>expression:expression	CHENGDENG	expression");matchRule(1, Rule::expression___expression_CHENGDENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	CHUDENG	expression			%prec	CHUDENG		{puts("yacc>>expression:expression	CHUDENG	expression");matchRule(1, Rule::expression___expression_CHUDENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	MODENG	expression			%prec	MODENG		{puts("yacc>>expression:expression	MODENG	expression");matchRule(1, Rule::expression___expression_MODENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	MIDENG	expression			%prec	MIDENG		{puts("yacc>>expression:expression	MIDENG	expression");matchRule(1, Rule::expression___expression_MIDENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	FUZHI	expression			%prec	FUZHI		{puts("yacc>>expression:expression	FUZHI	expression");matchRule(1, Rule::expression___expression_FUZHI_expression,&$1,&$3,&$2,&$$);}
	|	expression	DENGYU	expression			%prec	DENGYU		{puts("yacc>>expression:expression	DENGYU	expression");matchRule(1, Rule::expression___expression_DENGYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	BUDENG	expression			%prec	BUDENG		{puts("yacc>>expression:expression	BUDENG	expression");matchRule(1, Rule::expression___expression_BUDENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	DAYU	expression			%prec	DAYU		{puts("yacc>>expression:expression	DAYU	expression");matchRule(1, Rule::expression___expression_DAYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	XIAOYU	expression			%prec	XIAOYU		{puts("yacc>>expression:expression	XIAOYU	expression");matchRule(1, Rule::expression___expression_XIAOYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	BUDAYU	expression			%prec	BUDAYU		{puts("yacc>>expression:expression	BUDAYU	expression");matchRule(1, Rule::expression___expression_BUDAYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	BUXIAOYU	expression		%prec	BUXIAOYU	{puts("yacc>>expression:expression	BUXIAOYU	expression");matchRule(1, Rule::expression___expression_BUXIAOYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	AND	expression			%prec	AND		{puts("yacc>>expression:expression	AND	expression");matchRule(1, Rule::expression___expression_AND_expression,&$1,&$3,&$2,&$$);}
	|	expression	OR	expression			%prec	OR		{puts("yacc>>expression:expression	OR	expression");matchRule(1, Rule::expression___expression_OR_expression,&$1,&$3,&$2,&$$);}
	|	expression	USER_OP1	expression		%prec	AND		{puts("yacc>>expression:expression	USER_OP1	expression");matchRule(1, Rule::expression___expression_USER_OP1_expression,&$1,&$3,&$2,&$$);}
	|	expression	USER_OP2	expression		%prec	AND		{puts("yacc>>expression:expression	USER_OP2	expression");matchRule(1, Rule::expression___expression_USER_OP2_expression,&$1,&$3,&$2,&$$);}
	|	expression	DOTDOT	expression			%prec	DOTDOT		{puts("yacc>>expression:expression	DOTDOT	expression");matchRule(1, Rule::expression___expression_DOTDOT_expression,&$3,&$1,&$2,&$$);}
	|	expression	COMMA	expression			%prec	COMMA		{puts("yacc>>expression:expression	COMMA	expression");matchRule(1, Rule::expression___expression_COMMA_expression,&$1,&$3,&$2,&$$);}
	|	expression	SHUCHU	expression			%prec	SHUCHU		{puts("yacc>>expression:expression	SHUCHU	expression");matchRule(1, Rule::expression___expression_SHUCHU_expression,&$1,&$3,&$2,&$$);}
	|	LSCUR	expression	RSCUR				%prec	LSCUR		{puts("yacc>>expression:LSCUR	expression	RSCUR");matchRule(0, Rule::expression___LSCUR_expression_RSCUR,&$2,NULL,NULL,&$$);}
	|	expression	LMCUR	listAccess	RMCUR		%prec	LMCUR		{puts("yacc>>expression:expression	LMCUR	listAccess	RMCUR");matchRule(1, Rule::expression___expression_LMCUR_listAccess_RMCUR,&$1,&$3,NULL,&$$);}
	|	expression	LSCUR	expression_null	RSCUR		%prec	LSCUR		{puts("yacc>>expression:expression	LSCUR	expression_null	RSCUR");matchRule(1, Rule::expression___expression_LSCUR_expression_null_RSCUR,&$1,NULL,NULL,&$$);}
	|	expression	LSCUR	expression	RSCUR		%prec	LSCUR		{puts("yacc>>expression:expression	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_LSCUR_expression_RSCUR,&$1,&$3,NULL,&$$);}
	;


sentence_null:										{puts("yacc>>sentence_null:NULL");matchRule(1, Rule::sentence_null___NULL,NULL,NULL,NULL,NULL);}	
	;
sentence:	SEMICOLON					%dprec	1		{puts("yacc>>sentence:SEMICOLON");matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);}
	|	expression	SEMICOLON			%dprec	5		{puts("yacc>>sentence:expression	SEMICOLON");matchRule(1, Rule::sentence___expression_SEMICOLON,NULL,NULL,NULL,NULL);}
	|	functionDeclare	SEMICOLON			%dprec	2		{puts("yacc>>sentence:functionDeclare	SEMICOLON");matchRule(1, Rule::sentence___functionDeclare_SEMICOLON,NULL,NULL,NULL,NULL);}
	|	systemDeclare	SEMICOLON			%dprec	2		{puts("yacc>>sentence:systemDeclare	SEMICOLON");matchRule(1, Rule::sentence___systemDeclare_SEMICOLON,NULL,NULL,NULL,NULL);}
	|	functionImplement				%dprec	2		{puts("yacc>>sentence:functionImplement	SEMICOLON");matchRule(1, Rule::sentence___functionImplement,NULL,NULL,NULL,NULL);}
	|	systemImplement					%dprec	2		{puts("yacc>>sentence:systemImplement	SEMICOLON");matchRule(1, Rule::sentence___systemImplement,NULL,NULL,NULL,NULL);}
	|	_return_	SEMICOLON			%dprec	3		{puts("yacc>>sentence:_return_ SEMICOLON");matchRule(1, Rule::sentence____return__SEMICOLON,NULL,NULL,NULL,NULL);}	
	|	_return_	expression	SEMICOLON	%dprec	4		{puts("yacc>>sentence:_return_	expression SEMICOLON");matchRule(1, Rule::sentence____return__expression_SEMICOLON,&$2,NULL,NULL,NULL);}
	|	_abort_		SEMICOLON			%dprec	3		{puts("yacc>>sentence:_abort_ SEMICOLON");matchRule(1, Rule::sentence____abort__SEMICOLON,NULL,NULL,NULL,NULL);}	
	|	branch									{puts("yacc>>sentence:branch");matchRule(1, Rule::sentence___branch,NULL,NULL,NULL,NULL);}
	|	loop									{puts("yacc>>sentence:loop");matchRule(1, Rule::sentence___loop,NULL,NULL,NULL,NULL);}
	|	BREAK	SEMICOLON							{puts("yacc>>sentence:BREAK");matchRule(1, Rule::sentence___BREAK_SEMICOLON,NULL,NULL,NULL,NULL);}
	|	CONTINUE	SEMICOLON						{puts("yacc>>sentence:CONTINUE");matchRule(1, Rule::sentence___CONTINUE_SEMICOLON,NULL,NULL,NULL,NULL);}
	|	_FIL	LSCUR	arg	RSCUR						{puts("yacc>>sentence:_FIL	LSCUR	arg	RSCUR");matchRule(1, Rule::sentence____FIL_LSCUR_arg_RSCUR,&$3,NULL,NULL,NULL);}
	|	_LIN	LSCUR	arg	RSCUR						{puts("yacc>>sentence:_LIN	LSCUR	arg	RSCUR");matchRule(1, Rule::sentence____LIN_LSCUR_arg_RSCUR,&$3,NULL,NULL,NULL);}
	|	_LOAD	LSCUR	arg	RSCUR						{puts("yacc>>sentence:_LOAD	LSCUR	arg	RSCUR");matchRule(1, Rule::sentence____LOAD_LSCUR_arg_RSCUR,&$3,NULL,NULL,NULL);}
	;

scopeBegin:	LBCUR									{puts("yacc>>scopeBegin:LBCUR");matchRule(1, Rule::scopeBegin___LBCUR,&$1,NULL,NULL,&$$);}
	;

scopeHalf:	scopeBegin	sentence						{puts("yacc>>scopeHalf:scopeBegin	sentence");matchRule(0, Rule::scopeHalf___scopeBegin_sentence,&$1,NULL,NULL,&$$);}
	|	scopeHalf	sentence						{puts("yacc>>scopeHalf:scopeHalf	sentence");matchRule(0, Rule::scopeHalf___scopeHalf_sentence,&$1,NULL,NULL,&$$);}
//	|	scopeHalf	functionImplement	%dprec 5			{puts("yacc>>scopeHalf:scopeHalf	functionImplement");matchRule(0, Rule::scopeHalf___scopeHalf_functionImplement,&$1,NULL,NULL,&$$);}
//	|	scopeHalf	functionDeclare		%dprec 4			{puts("yacc>>scopeHalf:scopeHalf	functionDeclare");matchRule(0, Rule::scopeHalf___scopeHalf_functionDeclare,&$1,NULL,NULL,&$$);}
//	|	scopeHalf	systemImplement		%dprec 5			{puts("yacc>>scopeHalf:scopeHalf	systemImplement");matchRule(0, Rule::scopeHalf___scopeHalf_systemImplement,&$1,NULL,NULL,&$$);}
//	|	scopeHalf	systemDeclare		%dprec 4			{puts("yacc>>scopeHalf:scopeHalf	systemDeclare");matchRule(0, Rule::scopeHalf___scopeHalf_systemDeclare,&$1,NULL,NULL,&$$);}
	|	scopeHalf	scope			%dprec 3			{puts("yacc>>scopeHalf:scopeHalf	scope");matchRule(0, Rule::scopeHalf___scopeHalf_scope,&$1,NULL,NULL,&$$);}
	;

scope:		scopeHalf	RBCUR							{puts("yacc>>scope:scopeHalf	RBCUR");matchRule(1, Rule::scope___scopeHalf_RBCUR,&$1,NULL,NULL,&$$);}
	|	scopeBegin	RBCUR							{puts("yacc>>scope:scopeBegin	RBCUR");matchRule(1, Rule::scope___scopeBegin_RBCUR,&$1,NULL,NULL,&$$);}
	;


expr_scope_begin:	LBCUR									{puts("yacc>>expr_scope_begin:LBCUR");matchRule(1, Rule::expr_scope_begin___LBCUR,&$1,NULL,NULL,&$$);}
	;

expr_scope_half:	expr_scope_begin	expression					{puts("yacc>>expr_scope_half:expr_scope_begin	expr");matchRule(0, Rule::expr_scope_half___expr_scope_begin_expression,&$1,NULL,NULL,&$$);}
	;

expr_scope:		expr_scope_half	RBCUR							{puts("yacc>>scope:expr_scope_half	RBCUR");matchRule(1, Rule::expr_scope___expr_scope_half_RBCUR,&$1,NULL,NULL,&$$);}
	;




expressionFunctionDeclareRoot:
		JIAN	expression				%prec	NEG		{puts("yacc>>expressionFunctionDeclareRoot:JIAN	expression");matchRule(0, Rule::expressionFunctionDeclareRoot___JIAN_expression,NULL,&$2,&$1,&$$);}
	|	JIA	expression				%prec	NEG		{puts("yacc>>expressionFunctionDeclareRoot:JIA	expression");matchRule(0, Rule::expressionFunctionDeclareRoot___JIA_expression,NULL,&$2,&$1,&$$);}
	|	NOT	expression				%prec	NOT		{puts("yacc>>expressionFunctionDeclareRoot:NOT	expression");matchRule(1, Rule::expression___NOT_expression,NULL,&$2,&$1,&$$);}
	|	USER_OP1	expression			%prec	USER_OP1	{puts("yacc>>expressionFunctionDeclareRoot:USER_OP1	expression");matchRule(1, Rule::expression___USER_OP1_expression,NULL,&$2,&$1,&$$);}
	|	USER_OP2	expression			%prec	USER_OP2	{puts("yacc>>expressionFunctionDeclareRoot:USER_OP2	expression");matchRule(1, Rule::expression___USER_OP2_expression,NULL,&$2,&$1,&$$);}
	|	expression	USER_OP3			%prec	USER_OP3	{puts("yacc>>expressionFunctionDeclareRoot:expression	USER_OP3");matchRule(1, Rule::expression___expression_USER_OP3,&$1,NULL,&$2,&$$);}
	|	SI	expression				%prec	SI		{puts("yacc>>expressionFunctionDeclareRoot:SI	expression");matchRule(1, Rule::expression___SI_expression,NULL,&$2,&$1,&$$);}
	|	SD	expression				%prec	SD		{puts("yacc>>expressionFunctionDeclareRoot:SD	expression");matchRule(1, Rule::expression___SD_expression,NULL,&$2,&$1,&$$);}
	|	expression	SI				%prec	SI		{puts("yacc>>expressionFunctionDeclareRoot:expression	SI");matchRule(1, Rule::expression___expression_SI,&$1,NULL,&$2,&$$);}
	|	expression	SD				%prec	SD		{puts("yacc>>expressionFunctionDeclareRoot:expression	SD");matchRule(1, Rule::expression___expression_SD,&$1,NULL,&$2,&$$);}
	|	expression	JIA	expression		%prec	JIA		{puts("yacc>>expressionFunctionDeclareRoot:expression	JIA	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_JIA_expression,&$1,&$3,&$2,&$$);}
	|	expression	JIAN	expression		%prec	JIAN		{puts("yacc>>expressionFunctionDeclareRoot:expression	JIAN	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_JIAN_expression,&$1,&$3,&$2,&$$);}
	|	expression	CHENG	expression		%prec	CHENG		{puts("yacc>>expressionFunctionDeclareRoot:expression	CHENG	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_CHENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	CHU	expression		%prec	CHU		{puts("yacc>>expressionFunctionDeclareRoot:expression	CHU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_CHU_expression,&$1,&$3,&$2,&$$);}
	|	expression	MODULO	expression		%prec	MODULO		{puts("yacc>>expressionFunctionDeclareRoot:expression	MODULO	expression");matchRule(1, Rule::expression___expression_MODULO_expression,&$1,&$3,&$2,&$$);}
	|	expression	MI	expression		%prec	MI		{puts("yacc>>expressionFunctionDeclareRoot:expression	MI	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_MI_expression,&$1,&$3,&$2,&$$);}
	|	expression	JIADENG	expression		%prec	JIADENG		{puts("yacc>>expressionFunctionDeclareRoot:expression	JIADENG	expression");matchRule(1, Rule::expression___expression_JIADENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	JIANDENG	expression	%prec	JIANDENG	{puts("yacc>>expressionFunctionDeclareRoot:expression	JIANDENG	expression");matchRule(1, Rule::expression___expression_JIANDENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	CHENGDENG	expression	%prec	CHENGDENG	{puts("yacc>>expressionFunctionDeclareRoot:expression	CHENGDENG	expression");matchRule(1, Rule::expression___expression_CHENGDENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	CHUDENG	expression		%prec	CHUDENG		{puts("yacc>>expressionFunctionDeclareRoot:expression	CHUDENG	expression");matchRule(1, Rule::expression___expression_CHUDENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	MODENG	expression		%prec	MODENG		{puts("yacc>>expressionFunctionDeclareRoot:expression	MODENG	expression");matchRule(1, Rule::expression___expression_MODENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	MIDENG	expression		%prec	MIDENG		{puts("yacc>>expressionFunctionDeclareRoot:expression	MIDENG	expression");matchRule(1, Rule::expression___expression_MIDENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	FUZHI	expression		%prec	FUZHI		{puts("yacc>>expressionFunctionDeclareRoot:expression	FUZHI	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_FUZHI_expression,&$1,&$3,&$2,&$$);}
	|	expression	DENGYU	expression		%prec	DENGYU		{puts("yacc>>expressionFunctionDeclareRoot:expression	DENGYU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_DENGYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	BUDENG	expression		%prec	BUDENG		{puts("yacc>>expressionFunctionDeclareRoot:expression	BUDENG	expression");matchRule(1, Rule::expression___expression_BUDENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	DAYU	expression		%prec	DAYU		{puts("yacc>>expressionFunctionDeclareRoot:expression	DAYU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_DAYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	XIAOYU	expression		%prec	XIAOYU		{puts("yacc>>expressionFunctionDeclareRoot:expression	XIAOYU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_XIAOYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	BUDAYU	expression		%prec	BUDAYU		{puts("yacc>>expressionFunctionDeclareRoot:expression	BUDAYU	expression");matchRule(1, Rule::expression___expression_BUDAYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	BUXIAOYU	expression	%prec	BUXIAOYU	{puts("yacc>>expressionFunctionDeclareRoot:expression	BUXIAOYU	expression");matchRule(1, Rule::expression___expression_BUXIAOYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	AND	expression		%prec	AND		{puts("yacc>>expressionFunctionDeclareRoot:expression	AND	expression");matchRule(1, Rule::expression___expression_AND_expression,&$1,&$3,&$2,&$$);}
	|	expression	OR	expression		%prec	OR		{puts("yacc>>expressionFunctionDeclareRoot:expression	OR	expression");matchRule(1, Rule::expression___expression_OR_expression,&$1,&$3,&$2,&$$);}
	|	expression	USER_OP1	expression	%prec	LSCUR		{puts("yacc>>expressionFunctionDeclareRoot:expression	USER_OP1	expression");matchRule(1, Rule::expression___expression_USER_OP1_expression,&$1,&$3,&$2,&$$);}
	|	expression	USER_OP2	expression	%prec	LSCUR		{puts("yacc>>expressionFunctionDeclareRoot:expression	USER_OP2	expression");matchRule(1, Rule::expression___expression_USER_OP2_expression,&$1,&$3,&$2,&$$);}
	|	expression	DOTDOT	expression		%prec	DOTDOT		{puts("yacc>>expressionFunctionDeclareRoot:expression	DOTDOT	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_DOTDOT_expression,&$3,&$1,&$2,&$$);}
	|	expression	COMMA	expression		%prec	COMMA		{puts("yacc>>expressionFunctionDeclareRoot:expression	COMMA	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_COMMA_expression,&$1,&$3,&$2,&$$);}
	|	expression	SHUCHU	expression		%prec	SHUCHU		{puts("yacc>>expressionFunctionDeclareRoot:expression	SHUCHU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_SHUCHU_expression,&$1,&$3,&$2,&$$);}
	|	LSCUR	expressionFunctionDeclareRoot	RSCUR				{puts("yacc>>expressionFunctionDeclareRoot:LSCUR	expressionFunctionDeclareRoot	RSCUR");matchRule(0, Rule::expressionFunctionDeclareRoot___LSCUR_expressionFunctionDeclareRoot_RSCUR,&$2,NULL,NULL,&$$);}
	|	expression	LSCUR	expression_null	RSCUR	%prec	CALL		{puts("yacc>>expressionFunctionDeclareRoot:expression	LSCUR	expression_null	RSCUR");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_LSCUR_expression_null_RSCUR,&$1,NULL,NULL,&$$);}
	|	expression	LSCUR	expression	RSCUR	%prec	CALL		{puts("yacc>>expressionFunctionDeclareRoot:expression	LSCUR	expression	RSCUR");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_LSCUR_expression_RSCUR,&$1,&$3,NULL,&$$);}
	;

scopeFunctionDeclareBegin:	LBCUR									{puts("yacc>>scopeFunctionDeclareBegin:LBCUR");matchRule(1, Rule::scopeFunctionDeclareBegin___LBCUR,&$1,NULL,NULL,&$$);}
	;

scopeFunctionDeclareHalf:	scopeFunctionDeclareBegin	sentence				{puts("yacc>>scopeFunctionDeclareHalf:scopeFunctionDeclareBegin	sentence");matchRule(0, Rule::scopeFunctionDeclareHalf___scopeFunctionDeclareBegin_sentence,&$1,NULL,NULL,&$$);}
	|	scopeFunctionDeclareHalf	sentence						{puts("yacc>>scopeFunctionDeclareHalf:scopeFunctionDeclareHalf	sentence");matchRule(0, Rule::scopeFunctionDeclareHalf___scopeFunctionDeclareHalf_sentence,&$1,NULL,NULL,&$$);}
	;
scopeFunctionDeclareEnd:	scopeFunctionDeclareBegin	expressionFunctionDeclareRoot	%dprec	1	{puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareBegin	expressionFunctionDeclareRoot");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot,&$1,NULL,NULL,&$$);}
	|	scopeFunctionDeclareBegin	expressionFunctionDeclareRoot	SEMICOLON	%dprec	2	{puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareBegin	expressionFunctionDeclareRoot	SEMICOLON");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot_SRMICOLON,&$1,NULL,NULL,&$$);}	
	|	scopeFunctionDeclareHalf	expressionFunctionDeclareRoot			%dprec	3	{puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareHalf	expressionFunctionDeclareRoot");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot,&$1,NULL,NULL,&$$);}
	|	scopeFunctionDeclareHalf	expressionFunctionDeclareRoot	SEMICOLON	%dprec	4	{puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareHalf	expressionFunctionDeclareRoot	SEMICOLON");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot_SRMICOLON,&$1,NULL,NULL,&$$);}
	;
scopeFunctionDeclare:		scopeFunctionDeclareEnd	RBCUR							{puts("yacc>>scopeFunctionDeclare:scopeFunctionDeclareEnd	RBCUR");matchRule(1, Rule::scopeFunctionDeclare___scopeFunctionDeclareEnd_RBCUR,&$1,NULL,NULL,&$$);}
	;
autoScopeBegin:										{puts("yacc>>autoScopeBegin:NULL");$$=0;matchRule(1, Rule::autoScopeBegin___NULL,NULL,NULL,NULL,&$$);}
	;

autoScopeEnd:									%dprec 1	{puts("yacc>>autoScopeEnd:NULL");$$=0;matchRule(0, Rule::autoScopeEnd___NULL,NULL,NULL,NULL,NULL);}
//	|	SEMICOLON							%dprec 2	{puts("yacc>>autoScopeEnd:NULL");$$=0;matchRule(0, Rule::autoScopeEnd___NULL,NULL,NULL,NULL,NULL);}
	;

autoScopeHalf:	autoScopeBegin	expressionFunctionDeclareRoot			%dprec 1	{puts("yacc>>autoScopeHalf:autoScopeBegin	expressionFunctionDeclareRoot");matchRule(0, Rule::autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot,&$1,NULL,NULL,&$$);}
//	|	autoScopeBegin	expressionFunctionDeclareRoot	SEMICOLON	%dprec 2	{puts("yacc>>autoScopeHalf:autoScopeBegin	expressionFunctionDeclareRoot	SEMICOLON");matchRule(0, Rule::autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot_SEMICOLON,&$1,NULL,NULL,&$$);}
	;

autoScope:	autoScopeHalf	autoScopeEnd			%dprec 1			{puts("yacc>>autoScope:autoScopeHalf	autoScopeEnd");matchRule(1, Rule::autoScope___autoScopeHalf_autoScopeEnd,&$1,NULL,NULL,&$$);}
//	|	autoScopeHalf	SEMICOLON	autoScopeEnd	%dprec 2			{puts("yacc>>autoScope:autoScopeHalf	autoScopeEnd");matchRule(1, Rule::autoScope___autoScopeHalf_autoScopeEnd,&$1,NULL,NULL,&$$);}
	;

functionDeclareBegin:
		function_declare_attribute	DOTDOT	autoScope			{puts("yacc>>functionDeclareBegin:function_declare_attribute	DOTDOT	autoScope");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_autoScope,&$3,&$1,&$2,&$$);}
	|	function_declare_attribute	DOTDOT	scopeFunctionDeclare		{puts("yacc>>functionDeclareBegin:function_declare_attribute	DOTDOT	scopeFunctionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_scopeFunctionDeclare,&$3,&$1,&$2,&$$);}
	|	function_declare_attribute	scopeFunctionDeclare			{puts("yacc>>functionDeclareBegin:function_declare_attribute	scopeFunctionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_scopeFunctionDeclare,&$2,&$1,NULL,&$$);}
	|	function_declare_attribute	autoScope				{puts("yacc>>functionDeclareBegin:function_declare_attribute	autoscope");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_autoscope,&$2,&$1,NULL,&$$);}
	|	function_declare_attribute	functionDeclareBegin			{puts("yacc>>functionDeclareBegin:function_declare_attribute	functionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_functionDeclareBegin,&$2,&$1,NULL,&$$);}
	|	function_declare_attribute	DOTDOT	functionDeclareBegin		{puts("yacc>>functionDeclareBegin:function_declare_attribute	DOTDOT	functionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_functionDeclareBegin,&$3,&$1,&$2,&$$);}
	;
functionDeclareHalf:	functionDeclareBegin	JICHENG	arg				{puts("yacc>>functionDeclareHalf:functionDeclareBegin	JICHENG	arg");matchRule(1, Rule::functionDeclareHalf___functionDeclareBegin_JICHENG_arg,&$1,&$3,&$2,&$$);}
	|		functionDeclareHalf	COMMA	arg				{puts("yacc>>functionDeclareHalf:functionDeclareHalf	COMMA	arg");matchRule(1, Rule::functionDeclareHalf___functionDeclareBegin_JICHENG_arg,&$1,&$3,&$2,&$$);}
	;
functionDeclare:	functionDeclareBegin						{puts("yacc>>functionDeclare:functionDeclareBegin");matchRule(0, Rule::functionDeclare___functionDeclareBegin,&$1,NULL,NULL,&$$);}
	|		functionDeclareHalf						{puts("yacc>>functionDeclare:functionDeclareHalf");matchRule(0, Rule::functionDeclare___functionDeclareHalf,&$1,NULL,NULL,&$$);}
	;
functionImplement:	functionDeclare	scope						{puts("yacc>>functionImplement:functionDeclare	scope");matchRule(1, Rule::functionImplement___functionDeclare_scope,&$1,&$2,NULL,&$$);}
	|		functionImplement	TUIDAO	functionDeclare			{puts("yacc>>functionImplement:functionImplement	TUIDAO	functionDeclare");matchRule(1, Rule::functionImplement___functionImplement_TUIDAO_functionDeclare,&$3,&$1,&$2,&$$);matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);}
	;

systemDeclareBegin:	system_declare_attribute	DOTDOT	arg			{puts("yacc>>systemDeclareBegin:system_declare_attribute	DOTDOT	arg");matchRule(1, Rule::systemDeclareBegin___system_declare_attribute_DOTDOT_arg,&$3,&$1,&$2,&$$);}
	;
systemDeclareHalf:	systemDeclareBegin	JICHENG	arg				{puts("yacc>>systemDeclareHalf:systemDeclareBegin	JICHENG	arg");matchRule(1, Rule::systemDeclareHalf___systemDeclareBegin_JICHENG_arg,&$1,&$3,&$2,&$$);}
	|		systemDeclareHalf	COMMA	arg				{puts("yacc>>systemDeclareHalf:systemDeclareHalf	COMMA	arg");matchRule(1, Rule::systemDeclareHalf___systemDeclareBegin_JICHENG_arg,&$1,&$3,&$2,&$$);}
	;


systemDeclare:	systemDeclareBegin							{puts("yacc>>systemDeclare:systemDeclareBegin");matchRule(0, Rule::systemDeclare___systemDeclareBegin,&$1,NULL,NULL,&$$);}
	|	systemDeclareHalf							{puts("yacc>>systemDeclare:systemDeclareHalf");matchRule(0, Rule::systemDeclare___systemDeclareHalf,&$1,NULL,NULL,&$$);}
	;
systemImplement:	systemDeclare	scope						{puts("yacc>>systemImplement:systemDeclare	scope");matchRule(1, Rule::systemImplement___systemDeclare_scope,&$1,&$2,NULL,&$$);}
	;
branchBegin:	IF	LSCUR	expression	RSCUR	autoScope			{puts("yacc>>branchBegin:IF	LSCUR	expression	RSCUR	autoScope");matchRule(1, Rule::branchBegin___IF_LSCUR_expression_RSCUR_autoScope,NULL,&$3,&$5,&$$);}
	|	IF	LSCUR	expression	RSCUR	scope				{puts("yacc>>branchBegin:IF	LSCUR	expression	RSCUR	scope");matchRule(1, Rule::branchBegin___IF_LSCUR_expression_RSCUR_scope,NULL,&$3,&$5,&$$);}
	;
branchHalf:	branchBegin	ELIF	LSCUR	expression	RSCUR	autoScope	{puts("yacc>>branchHalf:branchBegin	ELIF	LSCUR	expression	RSCUR	autoScope");matchRule(1, Rule::branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_autoScope,&$1,&$4,&$6,&$$);}
	|	branchBegin	ELIF	LSCUR	expression	RSCUR	scope		{puts("yacc>>branchHalf:branchBegin	ELIF	LSCUR	expression	RSCUR	scope");matchRule(1, Rule::branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_scope,&$1,&$4,&$6,&$$);}
	;
branchEnd:	branchBegin	ELSE	autoScope					{puts("yacc>>branchEnd:branchBegin	ELSE	autoScope");matchRule(1, Rule::branchEnd___branchBegin_ELSE__autoScope,&$1,NULL,&$3,&$$);}
	|	branchBegin	ELSE	scope						{puts("yacc>>branchEnd:branchBegin	ELSE	scope");matchRule(1,  Rule::branchEnd___branchBegin_ELSE__scope,&$1,NULL,&$3,&$$);}
	|	branchHalf	ELSE	autoScope					{puts("yacc>>branchEnd:branchHalf	ELSE	autoScope");matchRule(1, Rule::branchEnd___branchHalf_ELSE__autoScope,&$1,NULL,&$3,&$$);}
	|	branchHalf	ELSE	scope						{puts("yacc>>branchEnd:branchHalf	ELSE	scope");matchRule(1,  Rule::branchEnd___branchHalf_ELSE__scope,&$1,NULL,&$3,&$$);}
	;
branch:		branchBegin								{puts("yacc>>branch:branchBegin");matchRule(0, Rule::branch___branchBegin,&$1,NULL,NULL,&$$);}
	|	branchHalf								{puts("yacc>>branch:branchHalf");matchRule(0,  Rule::branch___branchHalf,&$1,NULL,NULL,&$$);}
	|	branchEnd								{puts("yacc>>branch:branchEnd");matchRule(0, Rule::branch___branchEnd,&$1,NULL,NULL,&$$);}
	;

loop:		WHILE	LSCUR	expression	RSCUR	autoScope			{puts("yacc>>loop:WHILE	LSCUR	expression	RSCUR	autoScope");matchRule(1, Rule::loop___WHILE_LSCUR_expression_RSCUR_autoScope,&$$,&$3,&$5,NULL);}
	|	WHILE	LSCUR	expression	RSCUR	scope				{puts("yacc>>loop:WHILE	LSCUR	expression	RSCUR	scope");matchRule(1, Rule::loop___WHILE_LSCUR_expression_RSCUR_scope,&$$,&$3,&$5,NULL);}
	;
%%

int main(int argc, char *argv[]){

#if debug
	extern FILE * yyin;
	extern FILE * yyout ;
	setOutputCodePath("C:\\nonSystemFiles\\Flex Windows\\FlexFiles\\COOLANG_COMPILER_AUTO_INTEGRATE\\codeTest1out.txt");
	pretreate();
#else
	extern FILE * yyin;
	yyin = fopen(argv[1],"r");
	extern FILE * yyout ;
	 yyout = stdout;
	setOutputCodePath(argv[2]);
	if(yyin==NULL||yyout==NULL){ puts("cant open all File");system("pause");return 1;}
#endif
      
	//pretreate();
	//yylex();
	yyparse();

	puts("yacc>>yacc finish!");
	matchFinish();
#if debug
	system("pause");
#endif
	return 0;
}
void yyerror(const char *s){
	printf("出错啦\n");
	printf("%s\n", s);

}

//static int stmtMerge(YYSTYPE x0, YYSTYPE x1){ 
//puts("<OR> ");
// return 0;
//}