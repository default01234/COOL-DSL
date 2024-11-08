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
     LSCUR = 258,
     RSCUR = 259,
     LMCUR = 260,
     RMCUR = 261,
     LBCUR = 262,
     RBCUR = 263,
     COMMA = 264,
     SEMICOLON = 265,
     IF = 266,
     ELSE = 267,
     ELSEIF = 268,
     ELIF = 269,
     WHILE = 270,
     function_declare_attribute = 271,
     changeable_attribute = 272,
     variable_specific_attribute = 273,
     attribute = 274,
     arg = 275,
     _return_ = 276,
     LOWEST = 277,
     _return_low = 278,
     _return_high = 279,
     SHUCHU = 280,
     FUZHI = 281,
     XIAOYU = 282,
     DAYU = 283,
     DENGYU = 284,
     JIAN = 285,
     JIA = 286,
     CHU = 287,
     CHENG = 288,
     MI = 289,
     NEG = 290,
     CALL = 291,
     DOTDOT = 292,
     PENDCHANGEABLE = 293,
     CHANGEABLE = 294,
     HIGHEST = 295
   };
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2579 of glr.c  */
#line 34 "han_compiler.y"


    char    str[2048];
    char *  toString(){return str;}
    int argp;
    int exprp;




/* Line 2579 of glr.c  */
#line 107 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
