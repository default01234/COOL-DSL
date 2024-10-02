/* A Bison parser, made by GNU Bison 2.7.  */

/* Skeleton interface for Bison GLR parsers in C
   
      Copyright (C) 2002-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LBCUR = 258,
     RBCUR = 259,
     SEMICOLON = 260,
     IF = 261,
     ELSE = 262,
     ELSEIF = 263,
     ELIF = 264,
     WHILE = 265,
     BREAK = 266,
     CONTINUE = 267,
     _abort_ = 268,
     function_declare_attribute = 269,
     system_declare_attribute = 270,
     changeable_attribute = 271,
     variable_specific_attribute = 272,
     attribute = 273,
     arg = 274,
     _return_ = 275,
     LOWEST = 276,
     LOW = 277,
     USER_OP2 = 278,
     _return_low = 279,
     _return_high = 280,
     COMMA = 281,
     TUIDAO = 282,
     JICHENG = 283,
     SHUCHU = 284,
     MIDENG = 285,
     MODENG = 286,
     CHUDENG = 287,
     CHENGDENG = 288,
     JIANDENG = 289,
     JIADENG = 290,
     FUZHI = 291,
     OR = 292,
     AND = 293,
     USER_OP1 = 294,
     BUDENG = 295,
     DENGYU = 296,
     BUXIAOYU = 297,
     BUDAYU = 298,
     XIAOYU = 299,
     DAYU = 300,
     JIAN = 301,
     JIA = 302,
     NEG = 303,
     MODULO = 304,
     CHU = 305,
     CHENG = 306,
     MI = 307,
     SD = 308,
     SI = 309,
     NOT = 310,
     USER_OP3 = 311,
     LSCUR = 312,
     RSCUR = 313,
     CALL = 314,
     DOTDOT = 315,
     COMPATIBLE = 316,
     PENDCHANGEABLE = 317,
     CHANGEABLE = 318,
     LMCUR = 319,
     RMCUR = 320,
     BACK = 321,
     POPFRONT = 322,
     POPBACK = 323,
     PUSHFRONT = 324,
     PUSHBACK = 325,
     LOGICJUMP = 326,
     SLEEP = 327,
     BLOCKEXECUTE = 328,
     NONBLOCKEXECUTE = 329,
     RESET = 330,
     EXPR = 331,
     FIND_SUBEXPR = 332,
     EXIST_SUBEXPR = 333,
     COUNT = 334,
     FIND = 335,
     TOSTRG = 336,
     TOINT = 337,
     TONUM = 338,
     TYPENAME = 339,
     LENGTH = 340,
     INSERT = 341,
     ERASE = 342,
     CLEAR = 343,
     MULTISET = 344,
     SET = 345,
     MULTIMAP = 346,
     MAP = 347,
     LIST = 348,
     _LIN = 349,
     _FIL = 350,
     _LOAD = 351,
     DOT = 352,
     HIGHEST = 353
   };
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2579 of glr.c  */
#line 56 "coolang_compiler_auto_integrate.y"

	
	char	str[2048];
	char *  toString(){return str;}
	int argp;
	int exprp;




/* Line 2579 of glr.c  */
#line 165 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
