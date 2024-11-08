/* A Bison parser, made by GNU Bison 2.7.  */

/* Skeleton implementation for Bison GLR parsers in C
   
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

/* C GLR parser skeleton written by Paul Hilfinger.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "glr.c"

/* Pure parsers.  */
#define YYPURE 0






/* Copy the first part of user declarations.  */
/* Line 207 of glr.c  */
#line 1 "coolang_compiler_auto_integrate.y"

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

	void	setCurrentAssemblyCode();//C++й╣ож


//#include "y.tab.h"
//static int stmtMerge (YYSTYPE x0, YYSTYPE x1);
#if !debug
#define puts(XXX) 
#endif


/* Line 207 of glr.c  */
#line 112 "y.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#include "y.tab.h"

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Default (constant) value used for initialization for null
   right-hand sides.  Unlike the standard yacc.c template, here we set
   the default value of $$ to a zeroed-out value.  Since the default
   value is undefined, this behavior is technically correct.  */
static YYSTYPE yyval_default;

/* Copy the second part of user declarations.  */

/* Line 230 of glr.c  */
#line 141 "y.tab.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#ifndef YYFREE
# define YYFREE free
#endif
#ifndef YYMALLOC
# define YYMALLOC malloc
#endif
#ifndef YYREALLOC
# define YYREALLOC realloc
#endif

#define YYSIZEMAX ((size_t) -1)

#ifdef __cplusplus
   typedef bool yybool;
#else
   typedef unsigned char yybool;
#endif
#define yytrue 1
#define yyfalse 0

#ifndef YYSETJMP
# include <setjmp.h>
# define YYJMP_BUF jmp_buf
# define YYSETJMP(Env) setjmp (Env)
/* Pacify clang.  */
# define YYLONGJMP(Env, Val) (longjmp (Env, Val), YYASSERT (0))
#endif

/*-----------------.
| GCC extensions.  |
`-----------------*/

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

#ifndef YYASSERT
# define YYASSERT(Condition) ((void) ((Condition) || (abort (), 0)))
#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  109
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   7107

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  99
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  250
/* YYNRULES -- Number of states.  */
#define YYNSTATES  501
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule.  */
#define YYMAXRHS 6
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule.  */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   353

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      22,    25,    28,    31,    34,    37,    40,    43,    46,    49,
      53,    57,    61,    65,    69,    74,    78,    82,    86,    90,
      94,    98,   102,   106,   110,   114,   118,   122,   126,   130,
     134,   138,   142,   146,   150,   154,   158,   163,   168,   173,
     175,   177,   179,   182,   186,   188,   190,   193,   196,   198,
     199,   202,   206,   207,   210,   213,   218,   223,   225,   228,
     231,   234,   238,   242,   244,   245,   247,   251,   254,   257,
     260,   263,   266,   269,   272,   275,   278,   282,   287,   293,
     297,   301,   305,   309,   313,   316,   319,   324,   329,   334,
     339,   344,   349,   354,   360,   366,   372,   375,   379,   383,
     388,   394,   400,   405,   410,   415,   419,   423,   427,   431,
     435,   439,   443,   447,   451,   455,   459,   463,   467,   471,
     475,   479,   483,   487,   491,   495,   499,   503,   507,   511,
     515,   519,   524,   529,   534,   536,   539,   542,   545,   547,
     549,   552,   556,   559,   561,   563,   566,   569,   574,   579,
     584,   586,   589,   592,   595,   598,   601,   603,   606,   609,
     612,   615,   618,   621,   624,   627,   630,   633,   636,   639,
     643,   647,   651,   655,   659,   663,   667,   671,   675,   679,
     683,   687,   691,   695,   699,   703,   707,   711,   715,   719,
     723,   727,   731,   735,   739,   743,   747,   752,   757,   759,
     762,   765,   768,   772,   775,   779,   782,   783,   784,   787,
     790,   794,   798,   801,   804,   807,   811,   815,   819,   821,
     823,   826,   830,   834,   838,   842,   844,   846,   849,   855,
     861,   868,   875,   879,   883,   887,   891,   893,   895,   897,
     903
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const short int yyrhs[] =
{
     100,     0,    -1,   101,    -1,   100,   101,    -1,   136,    -1,
     140,    -1,   120,    -1,   117,    -1,   114,    -1,   102,    52,
     102,    -1,    46,   102,    -1,    47,   102,    -1,    55,   102,
      -1,    23,   102,    -1,   102,    56,    -1,    54,   102,    -1,
      53,   102,    -1,   102,    54,    -1,   102,    53,    -1,   102,
      38,   102,    -1,   102,    37,   102,    -1,   102,    39,   102,
      -1,   102,    23,   102,    -1,   102,    97,   114,    -1,   102,
      64,   102,    65,    -1,   102,    47,   102,    -1,   102,    46,
     102,    -1,   102,    51,   102,    -1,   102,    50,   102,    -1,
     102,    49,   102,    -1,   102,    35,   102,    -1,   102,    34,
     102,    -1,   102,    33,   102,    -1,   102,    32,   102,    -1,
     102,    31,   102,    -1,   102,    30,   102,    -1,   102,    36,
     102,    -1,   102,    41,   102,    -1,   102,    40,   102,    -1,
     102,    45,   102,    -1,   102,    44,   102,    -1,   102,    43,
     102,    -1,   102,    42,   102,    -1,   102,    60,   102,    -1,
     102,    29,   102,    -1,    57,   116,    58,    -1,   116,    64,
     112,    65,    -1,   116,    57,   115,    58,    -1,   116,    57,
     116,    58,    -1,   102,    -1,     3,    -1,    64,    -1,   104,
     103,    -1,   105,    26,   103,    -1,     4,    -1,    65,    -1,
     104,   106,    -1,   105,   106,    -1,   102,    -1,    -1,   109,
     108,    -1,   110,    26,   108,    -1,    -1,   109,   111,    -1,
     110,   111,    -1,    17,    57,    17,    58,    -1,    17,    57,
     113,    58,    -1,    19,    -1,    63,   114,    -1,    62,   114,
      -1,    61,   114,    -1,    17,    60,   114,    -1,   113,    60,
     114,    -1,   107,    -1,    -1,   114,    -1,   116,    52,   116,
      -1,    46,   116,    -1,    47,   116,    -1,    55,   116,    -1,
      23,   116,    -1,   116,    56,    -1,    54,   116,    -1,    53,
     116,    -1,   116,    54,    -1,   116,    53,    -1,   116,    97,
     114,    -1,   116,    88,    57,    58,    -1,   116,    87,    57,
     112,    58,    -1,    93,    57,    58,    -1,    92,    57,    58,
      -1,    91,    57,    58,    -1,    90,    57,    58,    -1,    89,
      57,    58,    -1,   116,    85,    -1,   116,    84,    -1,   116,
      82,    57,    58,    -1,   116,    83,    57,    58,    -1,   116,
      81,    57,    58,    -1,    74,    57,   116,    58,    -1,    73,
      57,   116,    58,    -1,    72,    57,   116,    58,    -1,    71,
      57,   116,    58,    -1,   116,    86,    57,   116,    58,    -1,
     116,    80,    57,   116,    58,    -1,   116,    79,    57,   116,
      58,    -1,    76,   123,    -1,   116,    78,   123,    -1,   116,
      77,   123,    -1,   116,    75,    57,    58,    -1,   116,    70,
      57,   116,    58,    -1,   116,    69,    57,   116,    58,    -1,
     116,    68,    57,    58,    -1,   116,    67,    57,    58,    -1,
     116,    66,    57,    58,    -1,   116,    47,   116,    -1,   116,
      46,   116,    -1,   116,    51,   116,    -1,   116,    50,   116,
      -1,   116,    49,   116,    -1,   116,    35,   116,    -1,   116,
      34,   116,    -1,   116,    33,   116,    -1,   116,    32,   116,
      -1,   116,    31,   116,    -1,   116,    30,   116,    -1,   116,
      36,   116,    -1,   116,    41,   116,    -1,   116,    40,   116,
      -1,   116,    45,   116,    -1,   116,    44,   116,    -1,   116,
      43,   116,    -1,   116,    42,   116,    -1,   116,    38,   116,
      -1,   116,    37,   116,    -1,   116,    39,   116,    -1,   116,
      23,   116,    -1,   116,    60,   116,    -1,   116,    26,   116,
      -1,   116,    29,   116,    -1,    57,   116,    58,    -1,   116,
      64,   112,    65,    -1,   116,    57,   115,    58,    -1,   116,
      57,   116,    58,    -1,     5,    -1,   116,     5,    -1,   135,
       5,    -1,   139,     5,    -1,   136,    -1,   140,    -1,    20,
       5,    -1,    20,   116,     5,    -1,    13,     5,    -1,   144,
      -1,   145,    -1,    11,     5,    -1,    12,     5,    -1,    95,
      57,    19,    58,    -1,    94,    57,    19,    58,    -1,    96,
      57,    19,    58,    -1,     3,    -1,   118,   117,    -1,   119,
     117,    -1,   119,   120,    -1,   119,     4,    -1,   118,     4,
      -1,     3,    -1,   121,   116,    -1,   122,     4,    -1,    46,
     116,    -1,    47,   116,    -1,    55,   116,    -1,    39,   116,
      -1,    23,   116,    -1,   116,    56,    -1,    54,   116,    -1,
      53,   116,    -1,   116,    54,    -1,   116,    53,    -1,   116,
      47,   116,    -1,   116,    46,   116,    -1,   116,    51,   116,
      -1,   116,    50,   116,    -1,   116,    49,   116,    -1,   116,
      52,   116,    -1,   116,    35,   116,    -1,   116,    34,   116,
      -1,   116,    33,   116,    -1,   116,    32,   116,    -1,   116,
      31,   116,    -1,   116,    30,   116,    -1,   116,    36,   116,
      -1,   116,    41,   116,    -1,   116,    40,   116,    -1,   116,
      45,   116,    -1,   116,    44,   116,    -1,   116,    43,   116,
      -1,   116,    42,   116,    -1,   116,    38,   116,    -1,   116,
      37,   116,    -1,   116,    39,   116,    -1,   116,    23,   116,
      -1,   116,    60,   116,    -1,   116,    26,   116,    -1,   116,
      29,   116,    -1,    57,   124,    58,    -1,   116,    57,   115,
      58,    -1,   116,    57,   116,    58,    -1,     3,    -1,   125,
     117,    -1,   126,   117,    -1,   125,   124,    -1,   125,   124,
       5,    -1,   126,   124,    -1,   126,   124,     5,    -1,   127,
       4,    -1,    -1,    -1,   129,   124,    -1,   131,   130,    -1,
      14,    60,   132,    -1,    14,    60,   128,    -1,    14,   128,
      -1,    14,   132,    -1,    14,   133,    -1,    14,    60,   133,
      -1,   133,    28,    19,    -1,   134,    26,    19,    -1,   133,
      -1,   134,    -1,   135,   120,    -1,   136,    27,   135,    -1,
      15,    60,    19,    -1,   137,    28,    19,    -1,   138,    26,
      19,    -1,   137,    -1,   138,    -1,   139,   120,    -1,     6,
      57,   116,    58,   132,    -1,     6,    57,   116,    58,   120,
      -1,   141,     9,    57,   116,    58,   132,    -1,   141,     9,
      57,   116,    58,   120,    -1,   141,     7,   132,    -1,   141,
       7,   120,    -1,   142,     7,   132,    -1,   142,     7,   120,
      -1,   141,    -1,   142,    -1,   143,    -1,    10,    57,   116,
      58,   132,    -1,    10,    57,   116,    58,   120,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   137,   137,   138,   142,   143,   144,   145,   149,   150,
     151,   152,   153,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   195,
     197,   198,   200,   201,   203,   204,   206,   207,   211,   213,
     215,   216,   218,   220,   221,   224,   225,   227,   228,   229,
     230,   231,   232,   233,   237,   239,   240,   241,   242,   243,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   314,   315,   316,   317,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     332,   335,   336,   341,   344,   345,   349,   352,   355,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,   400,   403,   406,
     407,   409,   410,   411,   412,   414,   416,   419,   423,   427,
     432,   433,   434,   435,   436,   437,   439,   440,   442,   443,
     445,   446,   449,   451,   452,   456,   457,   459,   461,   462,
     464,   465,   467,   468,   469,   470,   472,   473,   474,   477,
     478
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LBCUR", "RBCUR", "SEMICOLON", "IF",
  "ELSE", "ELSEIF", "ELIF", "WHILE", "BREAK", "CONTINUE", "_abort_",
  "function_declare_attribute", "system_declare_attribute",
  "changeable_attribute", "variable_specific_attribute", "attribute",
  "arg", "_return_", "LOWEST", "LOW", "USER_OP2", "_return_low",
  "_return_high", "COMMA", "TUIDAO", "JICHENG", "SHUCHU", "MIDENG",
  "MODENG", "CHUDENG", "CHENGDENG", "JIANDENG", "JIADENG", "FUZHI", "OR",
  "AND", "USER_OP1", "BUDENG", "DENGYU", "BUXIAOYU", "BUDAYU", "XIAOYU",
  "DAYU", "JIAN", "JIA", "NEG", "MODULO", "CHU", "CHENG", "MI", "SD", "SI",
  "NOT", "USER_OP3", "LSCUR", "RSCUR", "CALL", "DOTDOT", "COMPATIBLE",
  "PENDCHANGEABLE", "CHANGEABLE", "LMCUR", "RMCUR", "BACK", "POPFRONT",
  "POPBACK", "PUSHFRONT", "PUSHBACK", "LOGICJUMP", "SLEEP", "BLOCKEXECUTE",
  "NONBLOCKEXECUTE", "RESET", "EXPR", "FIND_SUBEXPR", "EXIST_SUBEXPR",
  "COUNT", "FIND", "TOSTRG", "TOINT", "TONUM", "TYPENAME", "LENGTH",
  "INSERT", "ERASE", "CLEAR", "MULTISET", "SET", "MULTIMAP", "MAP", "LIST",
  "_LIN", "_FIL", "_LOAD", "DOT", "HIGHEST", "$accept", "code",
  "environment", "strictExpression", "expressionForList", "listBegin",
  "listHalf", "listEnd", "list", "expressionForListAccess",
  "listAccessBegin", "listAccessHalf", "listAccessEnd", "listAccess",
  "variable_composed_attribute", "_arg_", "expression_null", "expression",
  "sentence", "scopeBegin", "scopeHalf", "scope", "expr_scope_begin",
  "expr_scope_half", "expr_scope", "expressionFunctionDeclareRoot",
  "scopeFunctionDeclareBegin", "scopeFunctionDeclareHalf",
  "scopeFunctionDeclareEnd", "scopeFunctionDeclare", "autoScopeBegin",
  "autoScopeEnd", "autoScopeHalf", "autoScope", "functionDeclareBegin",
  "functionDeclareHalf", "functionDeclare", "functionImplement",
  "systemDeclareBegin", "systemDeclareHalf", "systemDeclare",
  "systemImplement", "branchBegin", "branchHalf", "branchEnd", "branch",
  "loop", YY_NULL
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    99,   100,   100,   101,   101,   101,   101,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   103,
     104,   104,   105,   105,   106,   106,   107,   107,   108,   109,
     110,   110,   111,   112,   112,   113,   113,   114,   114,   114,
     114,   114,   114,   114,   115,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   117,   117,   117,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     118,   119,   119,   119,   120,   120,   121,   122,   123,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   124,   124,
     124,   124,   124,   124,   124,   124,   124,   124,   125,   126,
     126,   127,   127,   127,   127,   128,   129,   130,   131,   132,
     133,   133,   133,   133,   133,   133,   134,   134,   135,   135,
     136,   136,   137,   138,   138,   139,   139,   140,   141,   141,
     142,   142,   143,   143,   143,   143,   144,   144,   144,   145,
     145
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     3,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     4,     4,     1,
       1,     1,     2,     3,     1,     1,     2,     2,     1,     0,
       2,     3,     0,     2,     2,     4,     4,     1,     2,     2,
       2,     3,     3,     1,     0,     1,     3,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     3,     4,     5,     3,
       3,     3,     3,     3,     2,     2,     4,     4,     4,     4,
       4,     4,     4,     5,     5,     5,     2,     3,     3,     4,
       5,     5,     4,     4,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     4,     4,     4,     1,     2,     2,     2,     1,     1,
       2,     3,     2,     1,     1,     2,     2,     4,     4,     4,
       1,     2,     2,     2,     2,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     4,     1,     2,
       2,     2,     3,     2,     3,     2,     0,     0,     2,     2,
       3,     3,     2,     2,     2,     3,     3,     3,     1,     1,
       2,     3,     3,     3,     3,     1,     1,     2,     5,     5,
       6,     6,     3,     3,     3,     3,     1,     1,     1,     5,
       5
};

/* YYDPREC[RULE-NUM] -- Dynamic precedence of rule #RULE-NUM (0 if none).  */
static const unsigned char yydprec[] =
{
       0,     0,     1,     2,     5,     5,     3,     2,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     1,     5,     2,     2,     2,     2,
       3,     4,     3,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     3,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     1,     2,     3,     4,     0,     0,     1,     1,     1,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0
};

/* YYMERGER[RULE-NUM] -- Index of merging function for rule #RULE-NUM.  */
static const unsigned char yymerger[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0
};

/* YYDEFACT[S] -- default reduction number in state S.  Performed when
   YYTABLE doesn't specify something else to do.  Zero means the default
   is an error.  */
static const unsigned char yydefact[] =
{
       0,    50,   144,     0,     0,     0,     0,     0,   216,     0,
       0,    67,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     2,     0,
       0,    73,     0,    75,     0,     7,     0,     0,     6,   228,
     229,     0,     4,   235,   236,     0,     5,   246,   247,   248,
     153,   154,     0,     0,   155,   156,   152,   208,   216,     0,
       0,     0,   222,     0,   217,   223,   224,     0,     0,     0,
      50,   150,     0,    80,    77,    78,    83,    82,    79,     0,
      70,    69,    68,     0,     0,     0,     0,   166,     0,     0,
     106,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       3,    54,     0,     0,     0,     0,     0,     0,     0,    55,
      49,    52,    56,     8,     0,     0,    57,     0,   145,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    85,    84,    81,    74,     0,    59,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,    94,     0,     0,     0,     0,   165,
     161,   148,   149,   164,   162,   163,     0,     0,   160,   146,
     230,     0,     0,     0,   147,   237,   216,     0,   216,     0,
       0,   221,   220,   225,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   209,   211,   210,   213,   215,     0,   218,
     219,   232,     0,     0,    71,   151,   140,     0,     0,     0,
       0,   167,   168,    93,    92,    91,    90,    89,     0,     0,
       0,    13,     0,    10,    77,    11,    78,    16,    83,    15,
      82,    12,    79,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,    17,
      14,     0,     0,     0,    74,    59,    53,    72,   136,   138,
     139,   125,   124,   123,   122,   121,   120,   126,   134,   133,
     135,   128,   127,   132,   131,   130,   129,   116,   115,   119,
     118,   117,    76,     0,     0,   137,    62,    62,     0,     0,
       0,     0,     0,     0,     0,   108,   107,     0,     0,     0,
       0,     0,     0,    59,     0,    86,   226,   227,   231,   233,
     234,   243,   242,     0,   245,   244,   216,   216,   173,   172,
     169,   170,   176,   175,   171,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    85,    84,    81,    74,     0,   212,   214,    65,
      66,   102,   101,   100,    99,   158,   157,   159,    45,    22,
      44,    35,    34,    33,    32,    31,    30,    36,    20,    19,
      21,    38,    37,    42,    41,    40,    39,    26,    25,    29,
      28,    27,     9,    43,     0,    23,     0,     0,     0,   142,
     143,    58,    60,    63,     0,    64,   141,   114,   113,   112,
       0,     0,   109,     0,     0,    98,    96,    97,     0,     0,
      87,     0,   239,   238,   250,   249,   205,   201,   203,   204,
     190,   189,   188,   187,   186,   185,   191,   199,   198,   200,
     193,   192,   197,   196,   195,   194,   180,   179,   183,   182,
     181,   184,     0,     0,   202,    24,    47,    48,    46,    61,
     111,   110,   105,   104,   103,    88,   216,   142,   143,   241,
     240
};

/* YYPDEFGOTO[NTERM-NUM].  */
static const short int yydefgoto[] =
{
      -1,    37,    38,   120,   121,    39,    40,   122,    41,   432,
     316,   317,   433,   318,    42,    43,   313,   124,    45,    46,
      47,    48,    98,    99,   100,   214,    69,    70,    71,    72,
      73,   220,    74,    75,    49,    50,    51,   181,    53,    54,
      55,   182,    57,    58,    59,    60,    61
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -283
static const short int yypact[] =
{
    1758,  1382,  -283,   -42,   -26,    31,    35,    53,     7,    25,
     -16,  -283,  4264,  4388,  4388,  4388,  4388,  4388,  4388,  4388,
      20,    20,    20,  -283,    18,    39,    54,    69,   106,    73,
      76,    77,    78,    79,    80,    81,    82,  1191,  -283,    15,
       9,  -283,    65,  -283,  3902,  -283,  1476,  1570,  -283,   104,
     114,    52,  1094,   113,   116,    68,  1288,    83,   136,  -283,
    -283,  -283,  4388,  4388,  -283,  -283,  -283,  -283,    19,  1664,
    1664,   142,  -283,  4326,  -283,  -283,  -283,   128,   131,    20,
    -283,  -283,  3971,  5763,  6722,  6722,  6838,  6838,  6944,  4521,
    -283,  -283,  -283,  4388,  4388,  4388,  4388,  -283,  4388,   148,
    -283,    95,    96,    97,    98,    99,   139,   140,   143,  -283,
    -283,  -283,  4450,  4450,  4450,  4450,  4450,  4450,  4388,  -283,
     578,  -283,  -283,  5832,  5901,  4450,  -283,    20,  -283,  4388,
    4388,  4388,  4388,  4388,  4388,  4388,  4388,  4388,  4388,  4388,
    4388,  4388,  4388,  4388,  4388,  4388,  4388,  4388,  4388,  4388,
    4388,  4388,  4388,  4388,  -283,  -283,  -283,  4388,  4388,  -283,
     107,   108,   109,   135,   159,   166,   106,   106,   169,   170,
     173,   174,   177,  -283,  -283,   178,   179,   180,    20,  -283,
    -283,   198,  -283,  -283,  -283,  -283,   219,   244,  -283,  -283,
    -283,   182,   245,   250,  -283,  -283,   190,   213,   268,  4590,
    4659,  -283,  -283,  -283,  4388,  4388,  4388,  4388,  4388,  4388,
    4388,  4326,  4040,  -283,   267,  -283,   271,  -283,  5970,  -283,
    -283,  -283,     8,   220,  -283,  -283,  -283,  4728,  4797,  4866,
    4935,  5763,  -283,  -283,  -283,  -283,  -283,  -283,   224,   225,
     226,   578,  5901,    64,  6764,    64,  6764,   233,  6874,   233,
    6874,    63,  6978,  5004,  4450,  4450,  4450,  4450,  4450,  4450,
    4450,  4450,  4450,  4450,  4450,  4450,  4450,  4450,  4450,  4450,
    4450,  4450,  4450,  4450,  4450,  4450,  4450,  4450,  -283,  -283,
    -283,  4450,  4450,    20,  4388,  -283,  -283,  -283,  6582,  6384,
    6384,  6488,  6488,  6488,  6488,  6488,  6488,  6488,  6582,  6582,
    6582,  6631,  6631,  6680,  6680,  6680,  6680,  6722,  6722,  6802,
    6802,  6802,  6910,   230,  5073,  7010,  4450,   264,   227,   236,
     237,   238,  4388,  4388,   240,  -283,  -283,  4388,  4388,   253,
     262,   266,  4388,  -283,   270,  -283,  -283,  -283,  -283,  -283,
    -283,  -283,  -283,  4388,  -283,  -283,   288,   318,  1832,  6315,
    1901,  1970,  2039,  2108,  2177,  5142,   273,  4388,  4388,  4388,
    4388,  4388,  4388,  4388,  4388,  4388,  4388,  4388,  4388,  4388,
    4388,  4388,  4388,  4388,  4388,  4388,  4388,  4388,  4388,  4388,
    4388,  4388,   495,   706,   803,  4388,  4388,  -283,  -283,  -283,
    -283,  -283,  -283,  -283,  -283,  -283,  -283,  -283,  6039,   263,
    6453,  6556,  6556,  6556,  6556,  6556,  6556,  6556,   263,   263,
     263,  1082,  1082,    -4,    -4,    -4,    -4,    64,    64,   221,
     221,   221,   289,   -44,   168,  -283,   274,  5211,   260,  -283,
    -283,   578,  -283,  -283,  4450,  -283,  -283,  -283,  -283,  -283,
    5280,  5349,  -283,  5418,  5487,  -283,  -283,  -283,  5556,   275,
    -283,  5625,  -283,  -283,  -283,  -283,  -283,  4109,  2246,  2315,
    2384,  2453,  2522,  2591,  2660,  2729,  2798,  2867,  2936,  4178,
    3005,  3074,  3143,  3212,  3281,  3350,  3419,  3488,  3557,  3626,
    3695,  3764,   286,  5694,  3833,  -283,  6108,  6177,  6246,  -283,
    -283,  -283,  -283,  -283,  -283,  -283,   323,   900,   997,  -283,
    -283
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -283,  -283,   292,   -87,   223,  -283,  -283,   306,  -283,   -84,
    -283,  -283,    34,  -274,   276,   -15,  -282,   322,    75,  -283,
    -283,   -47,  -283,  -283,   -72,   -61,  -283,  -283,  -283,   284,
    -283,  -283,  -283,   -67,    -5,  -283,   164,    14,  -283,  -283,
    -283,    17,  -283,  -283,  -283,  -283,  -283
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -208
static const short int yytable[] =
{
     185,   202,   426,    76,   190,    90,    91,    92,   195,   216,
      67,   428,   219,   111,    52,    62,   281,    56,    80,   111,
     282,     8,    67,    80,   123,   241,   243,   245,   247,   249,
     251,    63,    10,     8,    11,   125,    64,    10,   112,    11,
      65,    78,   272,   273,    79,   274,   275,   276,   277,   278,
     279,    52,   280,   283,    56,   188,   281,   189,    66,   449,
     282,   113,   114,   203,   224,    78,   389,    68,   115,   116,
     117,   188,   118,   194,   119,    93,    20,    21,    22,    23,
     119,    20,    21,    22,    23,    77,    24,    25,    26,    27,
     196,    28,   197,   283,   325,   326,    94,   123,   123,   123,
     123,   123,   123,   482,    29,    30,    31,    32,    33,    97,
     123,    95,   287,   274,   275,   276,   277,   278,   279,   280,
     280,   180,   184,   281,   281,   127,    96,   282,   282,   342,
     101,   345,   186,   102,   103,   104,   105,   106,   107,   108,
     187,   192,   193,   198,   213,   215,   217,   221,   222,   341,
     356,   344,   232,   233,   234,   235,   236,   237,   238,   239,
     283,   283,   240,   335,   319,   320,   321,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   416,   417,   418,   419,   420,   421,
     422,   254,   322,   188,   423,   424,     8,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   323,   274,   275,   276,
     277,   278,   279,   324,   280,   191,   327,   328,   281,   431,
     329,   330,   282,   485,   331,   332,   333,   334,   336,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   123,   123,
     123,   123,   123,   337,   339,   283,   123,   123,   425,   340,
     343,   188,   387,   277,   278,   279,   388,   280,   390,   453,
     455,   281,   395,   396,   397,   282,  -208,  -208,   429,   280,
     434,   188,   436,   281,   437,   438,   439,   282,   442,   452,
     454,   123,   265,   266,   267,   268,   269,   270,   271,   272,
     273,   445,   274,   275,   276,   277,   278,   279,   283,   280,
     446,   188,    44,   281,   447,   488,   188,   282,   450,   110,
     283,   456,   486,   495,    82,    83,    84,    85,    86,    87,
      88,    89,   278,   279,   497,   280,   126,   431,   286,   281,
     489,   435,   201,   282,   223,   338,     0,     0,     0,    44,
     283,     0,     0,     0,     0,     0,     0,     0,    44,    44,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   199,   200,   283,     0,     0,     0,
       0,   212,   212,     0,     0,   218,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   227,   228,   229,   230,   123,
     231,     0,     0,     0,     0,     0,     0,     0,     0,   500,
       0,     0,     0,     0,   242,   244,   246,   248,   250,   252,
     253,     0,     0,     0,     0,     0,     0,     0,     0,   499,
       0,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,     0,     0,     0,   314,
     315,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -178,     0,     0,  -178,  -178,
     -85,  -178,  -178,     0,  -178,  -178,  -178,  -178,  -178,  -178,
    -178,     0,  -178,     0,  -178,  -178,     0,     0,   -85,     0,
       0,     0,  -178,  -178,     0,     0,   348,   349,   350,   351,
     352,   353,   354,   355,   -85,     0,     0,     0,     0,     0,
       0,   -85,   -85,     0,     0,     0,     0,     0,   -85,   -85,
    -178,     0,   -85,   -85,     0,     0,  -178,  -178,  -178,   -85,
       0,     0,     0,     0,     0,     0,  -178,  -178,  -178,  -178,
       0,  -178,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -178,  -178,  -178,  -178,  -178,  -178,
    -178,  -178,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   254,     0,     0,     0,     0,   427,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,     0,   274,   275,   276,
     277,   278,   279,     0,   280,     0,     0,     0,   281,     0,
       0,     0,   282,     0,   440,   441,     0,     0,     0,   443,
     444,     0,     0,     0,   448,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   451,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   283,     0,     0,     0,   457,
     458,   459,   460,   461,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   472,   473,   474,   475,   476,   477,
     478,   479,   480,   481,     0,     0,  -177,   483,   484,  -177,
    -177,   -84,  -177,  -177,     0,  -177,  -177,  -177,  -177,  -177,
    -177,  -177,     0,  -177,     0,  -177,  -177,     0,     0,   -84,
       0,     0,     0,  -177,  -177,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   -84,     0,     0,     0,     0,
       0,     0,   -84,   -84,     0,     0,     0,     0,     0,   -84,
     -84,  -177,     0,   -84,   -84,     0,     0,  -177,  -177,  -177,
     -84,     0,     0,     0,     0,     0,     0,  -177,  -177,  -177,
    -177,     0,  -177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -177,  -177,  -177,  -177,  -177,
    -177,  -177,  -177,  -174,     0,     0,  -174,  -174,   -81,  -174,
    -174,     0,  -174,  -174,  -174,  -174,  -174,  -174,  -174,     0,
    -174,     0,  -174,  -174,     0,     0,   -81,     0,     0,     0,
    -174,  -174,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -81,     0,     0,     0,     0,     0,     0,   -81,
     -81,     0,     0,     0,     0,     0,   -81,   -81,  -174,     0,
     -81,   -81,     0,     0,  -174,  -174,  -174,   -81,     0,     0,
       0,     0,     0,     0,  -174,  -174,  -174,  -174,     0,  -174,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -174,  -174,  -174,  -174,  -174,  -174,  -174,  -174,
    -206,     0,     0,  -206,  -206,  -142,  -206,  -206,     0,  -206,
    -206,  -206,  -206,  -206,  -206,  -206,     0,  -206,     0,  -206,
    -206,     0,     0,  -142,     0,     0,     0,  -206,  -206,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -142,
       0,     0,     0,     0,     0,     0,  -142,  -142,     0,     0,
       0,     0,     0,  -142,  -142,  -206,     0,  -142,  -142,     0,
       0,  -206,  -206,  -206,  -142,     0,     0,     0,     0,     0,
       0,  -206,  -206,  -206,  -206,     0,  -206,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -206,
    -206,  -206,  -206,  -206,  -206,  -206,  -206,  -207,     0,     0,
    -207,  -207,  -143,  -207,  -207,     0,  -207,  -207,  -207,  -207,
    -207,  -207,  -207,     0,  -207,     0,  -207,  -207,     0,     0,
    -143,     0,     0,     0,  -207,  -207,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -143,     0,     0,     0,
       0,     0,     0,  -143,  -143,     0,     0,     0,     0,     0,
    -143,  -143,  -207,     0,  -143,  -143,     0,     0,  -207,  -207,
    -207,  -143,     0,     0,     0,     0,     0,     0,  -207,  -207,
    -207,  -207,     0,  -207,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -207,  -207,  -207,  -207,
    -207,  -207,  -207,  -207,    -4,     0,     0,    -4,     0,    -4,
      -4,     0,     0,     0,    -4,    -4,    -4,    -4,    -4,    -4,
       0,    -4,     0,    -4,    -4,     0,     0,    -4,     0,     0,
       0,   191,     0,     0,   268,   269,   270,   271,   272,   273,
       0,   274,   275,   276,   277,   278,   279,     0,   280,     0,
      -4,    -4,   281,     0,     0,     0,   282,    -4,    -4,    -4,
       0,    -4,     0,     0,     0,    -4,    -4,    -4,    -4,     0,
       0,     0,     0,     0,     0,    -4,    -4,    -4,    -4,     0,
      -4,     0,     0,     0,     0,     0,     0,     0,     0,   283,
       0,     0,     0,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,   109,     0,     0,     1,     0,     2,     3,     0,     0,
       0,     4,     5,     6,     7,     8,     9,     0,    10,     0,
      11,    12,     0,     0,    13,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    14,    15,     0,
       0,     0,     0,     0,    16,    17,    18,     0,    19,     0,
       0,     0,    20,    21,    22,    23,     0,     0,     0,     0,
       0,     0,    24,    25,    26,    27,     0,    28,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      29,    30,    31,    32,    33,    34,    35,    36,    -5,     0,
       0,    -5,     0,    -5,    -5,     0,     0,     0,    -5,    -5,
      -5,    -5,    -5,    -5,     0,    -5,     0,    -5,    -5,     0,
       0,    -5,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    -5,    -5,     0,     0,     0,     0,
       0,    -5,    -5,    -5,     0,    -5,     0,     0,     0,    -5,
      -5,    -5,    -5,     0,     0,     0,     0,     0,     0,    -5,
      -5,    -5,    -5,     0,    -5,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    -5,    -5,    -5,
      -5,    -5,    -5,    -5,    -5,   -50,   -50,  -160,  -160,     0,
       0,     0,  -160,  -160,  -160,  -160,  -160,  -160,     0,   -50,
       0,   -50,  -160,     0,     0,   -50,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -50,   -50,
       0,     0,     0,     0,     0,   -50,   -50,   -50,     0,   -50,
       0,     0,     0,   -50,   -50,   -50,   -50,     0,     0,     0,
       0,     0,     0,   -50,   -50,   -50,   -50,     0,   -50,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   -50,   -50,   -50,   -50,   -50,  -160,  -160,  -160,    80,
     179,     2,     3,     0,     0,     0,     4,     5,     6,     7,
       8,     9,     0,    10,     0,    11,    12,     0,     0,    13,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    14,    15,     0,     0,     0,     0,     0,    16,
      17,    18,     0,    19,     0,     0,     0,    20,    21,    22,
      23,     0,     0,     0,     0,     0,     0,    24,    25,    26,
      27,     0,    28,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    29,    30,    31,    32,    33,
      34,    35,    36,     1,   183,     2,     3,     0,     0,     0,
       4,     5,     6,     7,     8,     9,     0,    10,     0,    11,
      12,     0,     0,    13,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    14,    15,     0,     0,
       0,     0,     0,    16,    17,    18,     0,    19,     0,     0,
       0,    20,    21,    22,    23,     0,     0,     0,     0,     0,
       0,    24,    25,    26,    27,     0,    28,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    29,
      30,    31,    32,    33,    34,    35,    36,    80,     0,     2,
       3,     0,     0,     0,     4,     5,     6,     7,     8,     9,
       0,    10,     0,    11,    12,     0,     0,   204,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   205,     0,     0,     0,     0,     0,     0,
     206,   207,     0,     0,     0,     0,     0,   208,   209,   210,
       0,   211,     0,     0,     0,    20,    21,    22,    23,     0,
       0,     0,     0,     0,     0,    24,    25,    26,    27,     0,
      28,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    30,    31,    32,    33,    34,    35,
      36,     1,     0,     2,     3,     0,     0,     0,     4,     5,
       6,     7,     8,     9,     0,    10,     0,    11,    12,     0,
       0,    13,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    16,    17,    18,     0,    19,     0,     0,     0,    20,
      21,    22,    23,     0,     0,     0,     0,     0,     0,    24,
      25,    26,    27,     0,    28,     0,     0,   -80,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    29,    30,    31,
      32,    33,    34,    35,    36,   129,     0,     0,   130,     0,
       0,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
       0,   150,   151,   152,   153,   154,   155,     0,   156,   157,
     -80,     0,   158,     0,     0,     0,   159,     0,   160,   161,
     162,   163,   164,     0,     0,     0,   -77,   165,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,     0,     0,   -77,     0,     0,   -77,     0,   178,
     -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,
     -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,     0,
     150,   151,   152,   153,   154,   155,     0,   156,   157,   -77,
       0,   158,     0,     0,     0,   159,     0,   160,   161,   162,
     163,   164,     0,     0,     0,   -78,   165,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,     0,     0,   -78,     0,     0,   -78,     0,   178,   -78,
     -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,
     -78,   -78,   -78,   -78,   -78,   -78,   -78,   -78,     0,   150,
     151,   152,   153,   154,   155,     0,   156,   157,   -78,     0,
     158,     0,     0,     0,   159,     0,   160,   161,   162,   163,
     164,     0,     0,     0,   -83,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
       0,     0,   -83,     0,     0,   -83,     0,   178,   -83,   -83,
     -83,   -83,   -83,   -83,   -83,   -83,   -83,   -83,   -83,   -83,
     -83,   -83,   -83,   -83,   -83,   -83,   -83,     0,   -83,   -83,
     -83,   -83,  -208,  -208,     0,   156,   157,   -83,     0,   158,
       0,     0,     0,   159,     0,   160,   161,   162,   163,   164,
       0,     0,     0,   -82,   165,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,     0,
       0,   -82,     0,     0,   -82,     0,   178,   -82,   -82,   -82,
     -82,   -82,   -82,   -82,   -82,   -82,   -82,   -82,   -82,   -82,
     -82,   -82,   -82,   -82,   -82,   -82,     0,   -82,   -82,   -82,
     -82,  -208,  -208,     0,   156,   157,   -82,     0,   158,     0,
       0,     0,   159,     0,   160,   161,   162,   163,   164,     0,
       0,     0,   -79,   165,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,     0,     0,
     -79,     0,     0,   -79,     0,   178,   -79,   -79,   -79,   -79,
     -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,   -79,
     -79,   -79,   -79,   -79,   -79,     0,   -79,   -79,   -79,   -79,
     -79,   -79,     0,   156,   157,   -79,     0,   158,     0,     0,
       0,   159,     0,   160,   161,   162,   163,   164,     0,     0,
       0,  -138,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,     0,     0,  -138,
       0,     0,  -138,     0,   178,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,     0,   150,   151,   152,   153,   154,
     155,     0,   156,   157,  -138,     0,   158,     0,     0,     0,
     159,     0,   160,   161,   162,   163,   164,     0,     0,     0,
    -139,   165,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,     0,     0,  -139,     0,
       0,  -139,     0,   178,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,     0,   150,   151,   152,   153,   154,   155,
       0,   156,   157,  -139,     0,   158,     0,     0,     0,   159,
       0,   160,   161,   162,   163,   164,     0,     0,     0,  -125,
     165,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,     0,     0,  -125,     0,     0,
    -125,     0,   178,  -125,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,     0,   150,   151,   152,   153,   154,   155,     0,
     156,   157,  -125,     0,   158,     0,     0,     0,   159,     0,
     160,   161,   162,   163,   164,     0,     0,     0,  -124,   165,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,     0,     0,  -124,     0,     0,  -124,
       0,   178,  -124,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,     0,   150,   151,   152,   153,   154,   155,     0,   156,
     157,  -124,     0,   158,     0,     0,     0,   159,     0,   160,
     161,   162,   163,   164,     0,     0,     0,  -123,   165,     0,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,     0,     0,  -123,     0,     0,  -123,     0,
     178,  -123,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
       0,   150,   151,   152,   153,   154,   155,     0,   156,   157,
    -123,     0,   158,     0,     0,     0,   159,     0,   160,   161,
     162,   163,   164,     0,     0,     0,  -122,   165,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,     0,     0,  -122,     0,     0,  -122,     0,   178,
    -122,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,     0,
     150,   151,   152,   153,   154,   155,     0,   156,   157,  -122,
       0,   158,     0,     0,     0,   159,     0,   160,   161,   162,
     163,   164,     0,     0,     0,  -121,   165,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,     0,     0,  -121,     0,     0,  -121,     0,   178,  -121,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,     0,   150,
     151,   152,   153,   154,   155,     0,   156,   157,  -121,     0,
     158,     0,     0,     0,   159,     0,   160,   161,   162,   163,
     164,     0,     0,     0,  -120,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
       0,     0,  -120,     0,     0,  -120,     0,   178,  -120,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,     0,   150,   151,
     152,   153,   154,   155,     0,   156,   157,  -120,     0,   158,
       0,     0,     0,   159,     0,   160,   161,   162,   163,   164,
       0,     0,     0,  -126,   165,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,     0,
       0,  -126,     0,     0,  -126,     0,   178,  -126,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,     0,   150,   151,   152,
     153,   154,   155,     0,   156,   157,  -126,     0,   158,     0,
       0,     0,   159,     0,   160,   161,   162,   163,   164,     0,
       0,     0,  -134,   165,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,     0,     0,
    -134,     0,     0,  -134,     0,   178,  -134,  -134,  -134,  -134,
    -134,  -134,  -134,  -134,  -134,  -134,   141,   142,   143,   144,
     145,   146,   147,   148,   149,     0,   150,   151,   152,   153,
     154,   155,     0,   156,   157,  -134,     0,   158,     0,     0,
       0,   159,     0,   160,   161,   162,   163,   164,     0,     0,
       0,  -133,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,     0,     0,  -133,
       0,     0,  -133,     0,   178,  -133,  -133,  -133,  -133,  -133,
    -133,  -133,  -133,  -133,  -133,   141,   142,   143,   144,   145,
     146,   147,   148,   149,     0,   150,   151,   152,   153,   154,
     155,     0,   156,   157,  -133,     0,   158,     0,     0,     0,
     159,     0,   160,   161,   162,   163,   164,     0,     0,     0,
    -128,   165,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,     0,     0,  -128,     0,
       0,  -128,     0,   178,  -128,  -128,  -128,  -128,  -128,  -128,
    -128,  -128,  -128,  -128,  -128,  -128,  -128,   144,   145,   146,
     147,   148,   149,     0,   150,   151,   152,   153,   154,   155,
       0,   156,   157,  -128,     0,   158,     0,     0,     0,   159,
       0,   160,   161,   162,   163,   164,     0,     0,     0,  -127,
     165,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,     0,     0,  -127,     0,     0,
    -127,     0,   178,  -127,  -127,  -127,  -127,  -127,  -127,  -127,
    -127,  -127,  -127,  -127,  -127,  -127,   144,   145,   146,   147,
     148,   149,     0,   150,   151,   152,   153,   154,   155,     0,
     156,   157,  -127,     0,   158,     0,     0,     0,   159,     0,
     160,   161,   162,   163,   164,     0,     0,     0,  -132,   165,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,     0,     0,  -132,     0,     0,  -132,
       0,   178,  -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,
    -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,   148,
     149,     0,   150,   151,   152,   153,   154,   155,     0,   156,
     157,  -132,     0,   158,     0,     0,     0,   159,     0,   160,
     161,   162,   163,   164,     0,     0,     0,  -131,   165,     0,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,     0,     0,  -131,     0,     0,  -131,     0,
     178,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,
    -131,  -131,  -131,  -131,  -131,  -131,  -131,  -131,   148,   149,
       0,   150,   151,   152,   153,   154,   155,     0,   156,   157,
    -131,     0,   158,     0,     0,     0,   159,     0,   160,   161,
     162,   163,   164,     0,     0,     0,  -130,   165,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,     0,     0,  -130,     0,     0,  -130,     0,   178,
    -130,  -130,  -130,  -130,  -130,  -130,  -130,  -130,  -130,  -130,
    -130,  -130,  -130,  -130,  -130,  -130,  -130,   148,   149,     0,
     150,   151,   152,   153,   154,   155,     0,   156,   157,  -130,
       0,   158,     0,     0,     0,   159,     0,   160,   161,   162,
     163,   164,     0,     0,     0,  -129,   165,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,     0,     0,  -129,     0,     0,  -129,     0,   178,  -129,
    -129,  -129,  -129,  -129,  -129,  -129,  -129,  -129,  -129,  -129,
    -129,  -129,  -129,  -129,  -129,  -129,   148,   149,     0,   150,
     151,   152,   153,   154,   155,     0,   156,   157,  -129,     0,
     158,     0,     0,     0,   159,     0,   160,   161,   162,   163,
     164,     0,     0,     0,  -116,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
       0,     0,  -116,     0,     0,  -116,     0,   178,  -116,  -116,
    -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,  -116,
    -116,  -116,  -116,  -116,  -116,  -116,  -116,     0,   150,   151,
     152,   153,   154,   155,     0,   156,   157,  -116,     0,   158,
       0,     0,     0,   159,     0,   160,   161,   162,   163,   164,
       0,     0,     0,  -115,   165,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,     0,
       0,  -115,     0,     0,  -115,     0,   178,  -115,  -115,  -115,
    -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,
    -115,  -115,  -115,  -115,  -115,  -115,     0,   150,   151,   152,
     153,   154,   155,     0,   156,   157,  -115,     0,   158,     0,
       0,     0,   159,     0,   160,   161,   162,   163,   164,     0,
       0,     0,  -119,   165,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,     0,     0,
    -119,     0,     0,  -119,     0,   178,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,  -119,
    -119,  -119,  -119,  -119,  -119,     0,  -119,  -119,  -119,   153,
     154,   155,     0,   156,   157,  -119,     0,   158,     0,     0,
       0,   159,     0,   160,   161,   162,   163,   164,     0,     0,
       0,  -118,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,     0,     0,  -118,
       0,     0,  -118,     0,   178,  -118,  -118,  -118,  -118,  -118,
    -118,  -118,  -118,  -118,  -118,  -118,  -118,  -118,  -118,  -118,
    -118,  -118,  -118,  -118,     0,  -118,  -118,  -118,   153,   154,
     155,     0,   156,   157,  -118,     0,   158,     0,     0,     0,
     159,     0,   160,   161,   162,   163,   164,     0,     0,     0,
    -117,   165,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,     0,     0,  -117,     0,
       0,  -117,     0,   178,  -117,  -117,  -117,  -117,  -117,  -117,
    -117,  -117,  -117,  -117,  -117,  -117,  -117,  -117,  -117,  -117,
    -117,  -117,  -117,     0,  -117,  -117,  -117,   153,   154,   155,
       0,   156,   157,  -117,     0,   158,     0,     0,     0,   159,
       0,   160,   161,   162,   163,   164,     0,     0,     0,   -76,
     165,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,     0,     0,   -76,     0,     0,
     -76,     0,   178,   -76,   -76,   -76,   -76,   -76,   -76,   -76,
     -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   -76,
     -76,   -76,     0,   -76,   -76,   -76,   -76,   154,   155,     0,
     156,   157,   -76,     0,   158,     0,     0,     0,   159,     0,
     160,   161,   162,   163,   164,     0,     0,     0,  -137,   165,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,     0,     0,  -137,     0,     0,  -137,
       0,   178,  -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,
    -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,  -137,
    -137,     0,  -137,  -137,  -137,  -137,  -137,  -137,     0,  -137,
    -137,  -137,     0,   158,     0,     0,     0,   159,     0,   160,
     161,   162,   163,   164,     0,     0,     0,   128,   165,     0,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,     0,     0,   129,     0,     0,   130,     0,
     178,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
       0,   150,   151,   152,   153,   154,   155,     0,   156,   157,
       0,     0,   158,     0,     0,     0,   159,     0,   160,   161,
     162,   163,   164,     0,     0,     0,   225,   165,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,     0,     0,   129,     0,     0,   130,     0,   178,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,     0,
     150,   151,   152,   153,   154,   155,     0,   156,   157,     0,
       0,   158,     0,     0,     0,   159,     0,   160,   161,   162,
     163,   164,     0,     0,     0,   128,   165,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,     0,     0,   357,     0,     0,   358,     0,   178,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,     0,   378,
     379,   380,   381,   382,   383,     0,   384,   385,     0,     0,
     386,     0,     0,     0,   159,     0,   160,   161,   162,   163,
     164,     0,     0,     0,  -136,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
       0,     0,  -136,     0,     0,  -136,     0,   178,  -136,  -136,
    -136,  -136,  -136,  -136,  -136,  -136,  -136,  -136,     0,   142,
     143,   144,   145,   146,   147,     0,     0,     0,   150,   151,
     152,   153,     0,     0,     0,   156,     0,  -136,     0,   158,
       0,     0,     0,   159,     0,   160,   161,   162,   163,   164,
       0,     0,     0,  -135,   165,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,     0,
       0,  -135,     0,     0,  -135,     0,   178,  -135,  -135,  -135,
    -135,  -135,  -135,  -135,  -135,  -135,  -135,     0,   142,   143,
     144,   145,   146,   147,     0,     0,     0,   150,   151,   152,
     153,     0,     0,     0,   156,     0,  -135,     0,   158,     0,
       0,     0,   159,     0,   160,   161,   162,   163,   164,     0,
       0,     0,     0,   165,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,    80,     0,    81,
       0,     0,     0,     0,     0,   178,     0,     0,     0,     0,
       0,    10,     0,    11,     0,     0,     0,    13,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      14,    15,     0,     0,     0,     0,     0,    16,    17,    18,
       0,    19,     0,     0,     0,    20,    21,    22,    23,    80,
       0,     0,     0,     0,     0,    24,    25,    26,    27,     0,
      28,     0,     0,    10,     0,    11,     0,     0,     0,   204,
       0,     0,     0,    29,    30,    31,    32,    33,     0,     0,
       0,     0,     0,     0,     0,   205,     0,     0,     0,     0,
       0,     0,   206,   207,     0,     0,     0,     0,     0,   208,
     209,   210,     0,   211,     0,     0,     0,    20,    21,    22,
      23,    80,     0,     0,     0,     0,     0,    24,    25,    26,
      27,     0,    28,     0,     0,    10,     0,    11,     0,     0,
       0,    13,     0,     0,     0,    29,    30,    31,    32,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    14,    15,     0,     0,     0,     0,
       0,    16,    17,    18,     0,    19,     0,     0,     0,    20,
      21,    22,    23,    80,     0,     0,     0,     0,     0,    24,
      25,    26,    27,     0,    28,     0,     0,    10,     0,    11,
       0,     0,     0,   112,     0,     0,     0,    29,    30,    31,
      32,    33,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   113,   114,     0,     0,
       0,     0,     0,   115,   116,   117,     0,   118,     0,     0,
       0,    20,    21,    22,    23,     0,     0,     0,     0,     0,
       0,    24,    25,    26,    27,     0,    28,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    29,
      30,    31,    32,    33,   129,     0,     0,   130,     0,     0,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,     0,
     150,   151,   152,   153,   154,   155,     0,   156,   157,   226,
       0,   158,     0,     0,     0,   159,     0,   160,   161,   162,
     163,   164,     0,     0,     0,     0,   165,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,     0,     0,   129,     0,     0,   130,     0,   178,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,     0,   150,
     151,   152,   153,   154,   155,     0,   156,   157,   346,     0,
     158,     0,     0,     0,   159,     0,   160,   161,   162,   163,
     164,     0,     0,     0,     0,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
       0,     0,   129,     0,     0,   130,     0,   178,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,     0,   150,   151,
     152,   153,   154,   155,     0,   156,   157,   347,     0,   158,
       0,     0,     0,   159,     0,   160,   161,   162,   163,   164,
       0,     0,     0,     0,   165,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,     0,
       0,   129,     0,     0,   130,     0,   178,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,     0,   150,   151,   152,
     153,   154,   155,     0,   156,   157,   391,     0,   158,     0,
       0,     0,   159,     0,   160,   161,   162,   163,   164,     0,
       0,     0,     0,   165,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,     0,     0,
     129,     0,     0,   130,     0,   178,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,     0,   150,   151,   152,   153,
     154,   155,     0,   156,   157,   392,     0,   158,     0,     0,
       0,   159,     0,   160,   161,   162,   163,   164,     0,     0,
       0,     0,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,     0,     0,   129,
       0,     0,   130,     0,   178,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,     0,   150,   151,   152,   153,   154,
     155,     0,   156,   157,   393,     0,   158,     0,     0,     0,
     159,     0,   160,   161,   162,   163,   164,     0,     0,     0,
       0,   165,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,     0,     0,   129,     0,
       0,   130,     0,   178,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,     0,   150,   151,   152,   153,   154,   155,
       0,   156,   157,   394,     0,   158,     0,     0,     0,   159,
       0,   160,   161,   162,   163,   164,     0,     0,     0,     0,
     165,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,     0,     0,   129,     0,     0,
     130,     0,   178,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,     0,   150,   151,   152,   153,   154,   155,     0,
     156,   157,   398,     0,   158,     0,     0,     0,   159,     0,
     160,   161,   162,   163,   164,     0,     0,     0,     0,   165,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,     0,     0,   129,     0,     0,   130,
       0,   178,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,     0,   150,   151,   152,   153,   154,   155,     0,   156,
     157,   430,     0,   158,     0,     0,     0,   159,     0,   160,
     161,   162,   163,   164,     0,     0,     0,     0,   165,     0,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,     0,     0,   357,     0,     0,   358,     0,
     178,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
       0,   378,   379,   380,   381,   382,   383,     0,   384,   385,
     226,     0,   386,     0,     0,     0,   159,     0,   160,   161,
     162,   163,   164,     0,     0,     0,     0,   165,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,     0,     0,   129,     0,     0,   130,     0,   178,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,     0,
     150,   151,   152,   153,   154,   155,     0,   156,   157,   487,
       0,   158,     0,     0,     0,   159,     0,   160,   161,   162,
     163,   164,     0,     0,     0,     0,   165,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,     0,     0,   129,     0,     0,   130,     0,   178,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,     0,   150,
     151,   152,   153,   154,   155,     0,   156,   157,   490,     0,
     158,     0,     0,     0,   159,     0,   160,   161,   162,   163,
     164,     0,     0,     0,     0,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
       0,     0,   129,     0,     0,   130,     0,   178,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,     0,   150,   151,
     152,   153,   154,   155,     0,   156,   157,   491,     0,   158,
       0,     0,     0,   159,     0,   160,   161,   162,   163,   164,
       0,     0,     0,     0,   165,     0,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,     0,     0,
       0,   129,     0,     0,   130,     0,   178,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,     0,   150,   151,   152,
     153,   154,   155,     0,   156,   157,   492,     0,   158,     0,
       0,     0,   159,     0,   160,   161,   162,   163,   164,     0,
       0,     0,     0,   165,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,     0,     0,
     129,     0,     0,   130,     0,   178,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,     0,   150,   151,   152,   153,
     154,   155,     0,   156,   157,   493,     0,   158,     0,     0,
       0,   159,     0,   160,   161,   162,   163,   164,     0,     0,
       0,     0,   165,     0,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,     0,     0,     0,   129,
       0,     0,   130,     0,   178,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,     0,   150,   151,   152,   153,   154,
     155,     0,   156,   157,   494,     0,   158,     0,     0,     0,
     159,     0,   160,   161,   162,   163,   164,     0,     0,     0,
       0,   165,     0,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,     0,     0,     0,   129,     0,
       0,   130,     0,   178,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,     0,   150,   151,   152,   153,   154,   155,
       0,   156,   157,   496,     0,   158,     0,     0,     0,   159,
       0,   160,   161,   162,   163,   164,     0,     0,     0,     0,
     165,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,     0,     0,   129,     0,     0,
     130,     0,   178,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,     0,   150,   151,   152,   153,   154,   155,     0,
     156,   157,   498,     0,   158,     0,     0,     0,   159,     0,
     160,   161,   162,   163,   164,     0,     0,     0,     0,   165,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,     0,     0,   129,     0,     0,   130,
       0,   178,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,     0,   150,   151,   152,   153,   154,   155,     0,   156,
     157,     0,     0,   158,     0,     0,     0,   159,     0,   160,
     161,   162,   163,   164,     0,     0,     0,     0,   165,     0,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,     0,     0,     0,    -8,     0,     0,    -8,     0,
     178,    -8,    -8,    -8,    -8,    -8,    -8,    -8,    -8,    -8,
      -8,    -8,    -8,    -8,    -8,    -8,    -8,    -8,    -8,    -8,
       0,    -8,    -8,    -8,    -8,    -8,    -8,     0,    -8,   -75,
       0,     0,    -8,     0,     0,     0,    -8,     0,   -75,   -75,
     -75,   -75,   -75,     0,     0,     0,     0,   -75,     0,   -75,
     -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,   -75,
     -75,     0,     0,     0,   129,     0,     0,   130,     0,    -8,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,     0,
     150,   151,   152,   153,   154,   155,     0,   156,   284,     0,
       0,   158,     0,     0,     0,   285,     0,   160,   161,   162,
     163,   164,     0,     0,     0,     0,   165,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,     0,     0,   357,     0,     0,   358,     0,   178,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   377,     0,   378,
     379,   380,   381,   382,   383,     0,   384,   385,     0,     0,
     386,     0,     0,     0,   159,     0,   160,   161,   162,   163,
     164,     0,     0,     0,     0,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
       0,     0,   -45,     0,     0,   -45,     0,   178,   -45,   -45,
     -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,
     -45,   -45,   -45,   -45,   -45,   -45,   -45,     0,   -45,   -45,
     -45,   -45,   -45,   -45,     0,   -45,  -140,     0,     0,   -45,
       0,     0,     0,   -45,     0,  -140,  -140,  -140,  -140,  -140,
       0,     0,     0,     0,  -140,     0,  -140,  -140,  -140,  -140,
    -140,  -140,  -140,  -140,  -140,  -140,  -140,  -140,     0,     0,
       0,   -47,     0,     0,   -47,     0,   -45,   -47,   -47,   -47,
     -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,
     -47,   -47,   -47,   -47,   -47,   -47,     0,   -47,   -47,   -47,
     -47,   -47,   -47,     0,   -47,  -142,     0,     0,   -47,     0,
       0,     0,   -47,     0,  -142,  -142,  -142,  -142,  -142,     0,
       0,     0,     0,  -142,     0,  -142,  -142,  -142,  -142,  -142,
    -142,  -142,  -142,  -142,  -142,  -142,  -142,     0,     0,     0,
     -48,     0,     0,   -48,     0,   -47,   -48,   -48,   -48,   -48,
     -48,   -48,   -48,   -48,   -48,   -48,   -48,   -48,   -48,   -48,
     -48,   -48,   -48,   -48,   -48,     0,   -48,   -48,   -48,   -48,
     -48,   -48,     0,   -48,  -143,     0,     0,   -48,     0,     0,
       0,   -48,     0,  -143,  -143,  -143,  -143,  -143,     0,     0,
       0,     0,  -143,     0,  -143,  -143,  -143,  -143,  -143,  -143,
    -143,  -143,  -143,  -143,  -143,  -143,     0,     0,     0,   -46,
       0,     0,   -46,     0,   -48,   -46,   -46,   -46,   -46,   -46,
     -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,
     -46,   -46,   -46,   -46,     0,   -46,   -46,   -46,   -46,   -46,
     -46,     0,   -46,  -141,     0,     0,   -46,     0,     0,     0,
     -46,     0,  -141,  -141,  -141,  -141,  -141,     0,     0,     0,
       0,  -141,     0,  -141,  -141,  -141,  -141,  -141,  -141,  -141,
    -141,  -141,  -141,  -141,  -141,     0,     0,     0,     0,     0,
       0,   130,     0,   -46,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,     0,   142,   143,   144,   145,   146,
     147,   148,   149,     0,   150,   151,   152,   153,   154,   155,
       0,   156,   157,     0,     0,   158,     0,     0,     0,   159,
       0,   160,   161,   162,   163,   164,     0,     0,     0,     0,
     165,     0,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,     0,     0,     0,     0,     0,
       0,     0,   178,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,     0,   150,   151,   152,   153,   154,   155,     0,
     156,   157,     0,     0,   158,     0,     0,     0,   159,     0,
     160,   161,   162,   163,   164,     0,     0,     0,     0,   165,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,     0,     0,     0,     0,     0,     0,
       0,   178,   255,   256,   257,   258,   259,   260,   261,   262,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     273,     0,   274,   275,   276,   277,   278,   279,     0,   280,
       0,     0,     0,   281,     0,     0,     0,   282,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,     0,   150,   151,   152,
     153,   154,   155,     0,   156,   157,     0,     0,   158,     0,
     283,     0,   159,     0,   160,   161,   162,   163,   164,     0,
       0,     0,     0,   165,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,     0,     0,
       0,     0,     0,     0,     0,   178,   256,   257,   258,   259,
     260,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,     0,   274,   275,   276,   277,   278,
     279,     0,   280,     0,     0,     0,   281,     0,     0,     0,
     282,   141,   142,   143,   144,   145,   146,   147,   148,   149,
       0,   150,   151,   152,   153,   154,   155,     0,   156,   157,
       0,     0,   158,     0,     0,     0,   159,     0,   160,   161,
     162,   163,   164,   283,     0,     0,     0,   165,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,     0,   144,   145,   146,   147,   148,   149,   178,
     150,   151,   152,   153,   154,   155,     0,   156,   157,     0,
       0,   158,     0,     0,     0,   159,     0,   160,   161,   162,
     163,   164,     0,     0,     0,     0,   165,     0,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
       0,     0,     0,     0,     0,     0,   148,   149,   178,   150,
     151,   152,   153,   154,   155,     0,   156,   157,     0,     0,
     158,     0,     0,     0,   159,     0,   160,   161,   162,   163,
     164,     0,     0,     0,     0,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
       0,   150,   151,   152,   153,   154,   155,   178,   156,   157,
       0,     0,   158,     0,     0,     0,   159,     0,   160,   161,
     162,   163,   164,     0,     0,     0,     0,   165,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,     0,     0,   150,   151,   152,   153,   154,   155,   178,
     156,   284,     0,     0,   158,     0,     0,     0,   285,     0,
     160,   161,   162,   163,   164,     0,     0,     0,     0,   165,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,   153,   154,   155,     0,   156,   157,
       0,   178,   158,     0,     0,     0,   159,     0,   160,   161,
     162,   163,   164,     0,     0,     0,     0,   165,     0,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,  -208,  -208,     0,   156,   157,     0,     0,   158,   178,
       0,     0,   159,     0,   160,   161,   162,   163,   164,     0,
       0,     0,     0,   165,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,  -208,  -208,     0,
     156,   284,     0,     0,   158,   178,     0,     0,   285,     0,
     160,   161,   162,   163,   164,     0,     0,     0,     0,   165,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   154,   155,     0,   156,   157,     0,     0,
     158,   178,     0,     0,   159,     0,   160,   161,   162,   163,
     164,     0,     0,     0,     0,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
     156,   157,     0,     0,   158,     0,     0,   178,   159,     0,
     160,   161,   162,   163,   164,     0,     0,     0,     0,   165,
       0,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,     0,   156,   284,     0,     0,   158,     0,
       0,   178,   285,     0,   160,   161,   162,   163,   164,     0,
       0,     0,     0,   165,     0,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,     0,     0,     0,
     158,     0,     0,     0,   159,   178,   160,   161,   162,   163,
     164,     0,     0,     0,     0,   165,     0,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,     0,
       0,     0,     0,     0,     0,     0,     0,   178
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const unsigned short int yyconflp[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      51,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   201,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   265,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   267,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   269,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   271,     0,     0,     0,     0,   865,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   343,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   345,     0,     0,     0,     0,     0,
       0,   347,   349,     0,     0,     0,     0,     0,   351,   353,
       0,     0,   355,   357,     0,     0,     0,     0,     0,   359,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   361,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   363,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,   367,   369,     0,     0,     0,     0,     0,   371,
     373,     0,     0,   375,   377,     0,     0,     0,     0,     0,
     379,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   381,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   383,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   385,     0,     0,     0,     0,     0,     0,   387,
     389,     0,     0,     0,     0,     0,   391,   393,     0,     0,
     395,   397,     0,     0,     0,     0,     0,   399,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   867,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   869,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   871,
       0,     0,     0,     0,     0,     0,   873,   875,     0,     0,
       0,     0,     0,   877,   879,     0,     0,   881,   883,     0,
       0,     0,     0,     0,   885,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   887,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     889,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   891,     0,     0,     0,
       0,     0,     0,   893,   895,     0,     0,     0,     0,     0,
     897,   899,     0,     0,   901,   903,     0,     0,     0,     0,
       0,   905,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,    55,     0,    57,
      59,     0,     0,     0,    61,    63,    65,    67,    69,    71,
       0,    73,     0,    75,    77,     0,     0,    79,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      81,    83,     0,     0,     0,     0,     0,    85,    87,    89,
       0,    91,     0,     0,     0,    93,    95,    97,    99,     0,
       0,     0,     0,     0,     0,   101,   103,   105,   107,     0,
     109,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   111,   113,   115,   117,   119,   121,   123,
     125,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   127,     0,
       0,   129,     0,   131,   133,     0,     0,     0,   135,   137,
     139,   141,   143,   145,     0,   147,     0,   149,   151,     0,
       0,   153,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   155,   157,     0,     0,     0,     0,
       0,   159,   161,   163,     0,   165,     0,     0,     0,   167,
     169,   171,   173,     0,     0,     0,     0,     0,     0,   175,
     177,   179,   181,     0,   183,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   185,   187,   189,
     191,   193,   195,   197,   199,     1,     3,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     5,
       0,     7,     0,     0,     0,     9,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    11,    13,
       0,     0,     0,     0,     0,    15,    17,    19,     0,    21,
       0,     0,     0,    23,    25,    27,    29,     0,     0,     0,
       0,     0,     0,    31,    33,    35,    37,     0,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    41,    43,    45,    47,    49,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   273,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     275,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   279,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     281,     0,     0,     0,     0,     0,     0,   283,   285,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   287,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   289,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   291,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   293,
       0,     0,     0,     0,     0,     0,   295,   297,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   299,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   301,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   303,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   305,     0,
       0,     0,     0,     0,     0,   307,   309,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   311,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   313,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   315,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   317,     0,     0,
       0,     0,     0,     0,   319,   321,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   325,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     327,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   329,     0,     0,     0,
       0,     0,     0,   331,   333,     0,     0,     0,     0,     0,
     335,   337,     0,     0,     0,   339,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   469,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   471,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   473,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     475,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   477,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   479,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   481,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   483,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   485,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   487,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   489,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   491,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   493,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   495,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   501,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   503,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   505,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   507,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   509,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   511,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   513,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   515,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   517,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   519,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   521,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   523,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     525,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   527,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   529,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   531,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     541,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   543,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   545,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   547,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   549,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   551,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   553,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   555,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   557,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   559,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   561,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   563,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   565,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   567,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   569,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     571,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   573,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   575,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     577,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   579,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   581,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   583,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   585,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   587,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   589,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   591,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   593,     0,
       0,     0,     0,     0,     0,   595,   597,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   599,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   601,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   603,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   605,     0,     0,
       0,     0,     0,     0,   607,   609,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   611,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   613,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     615,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   617,     0,     0,     0,
       0,     0,     0,   619,   621,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   623,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   625,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   627,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   629,     0,     0,     0,     0,
       0,     0,   631,   633,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   635,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     637,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   639,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   641,     0,     0,     0,     0,     0,
       0,   643,   645,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   647,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   649,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   651,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   653,     0,     0,     0,     0,     0,     0,
     655,   657,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   659,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   661,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   663,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   665,     0,     0,     0,     0,     0,     0,   667,
     669,     0,     0,     0,     0,     0,   671,   673,     0,     0,
     675,   677,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   463,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   465,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   467,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   535,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   537,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   539,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   203,     0,     0,   205,     0,
       0,   207,   209,   211,   213,   215,   217,   219,   221,   223,
     225,   227,   229,   231,   233,   235,   237,   239,   241,   243,
       0,   245,   247,   249,   251,   253,   255,     0,   257,     0,
       0,     0,   259,     0,     0,     0,   261,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   401,     0,     0,   403,     0,     0,   405,   407,
     409,   411,   413,   415,   417,   419,   421,   423,   425,   427,
     429,   431,   433,   435,   437,   439,   441,     0,   443,   445,
     447,   449,   451,   453,     0,   455,     0,     0,     0,   457,
       0,     0,     0,   459,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   679,     0,     0,   681,     0,   461,   683,   685,   687,
     689,   691,   693,   695,   697,   699,   701,   703,   705,   707,
     709,   711,   713,   715,   717,   719,     0,   721,   723,   725,
     727,   729,   731,     0,   733,     0,     0,     0,   735,     0,
       0,     0,   737,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     741,     0,     0,   743,     0,   739,   745,   747,   749,   751,
     753,   755,   757,   759,   761,   763,   765,   767,   769,   771,
     773,   775,   777,   779,   781,     0,   783,   785,   787,   789,
     791,   793,     0,   795,     0,     0,     0,   797,     0,     0,
       0,   799,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   803,
       0,     0,   805,     0,   801,   807,   809,   811,   813,   815,
     817,   819,   821,   823,   825,   827,   829,   831,   833,   835,
     837,   839,   841,   843,     0,   845,   847,   849,   851,   853,
     855,     0,   857,     0,     0,     0,   859,     0,     0,     0,
     861,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   863,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] =
{
       0,   160,     0,   160,     0,   160,     0,   160,     0,   160,
       0,   160,     0,   160,     0,   160,     0,   160,     0,   160,
       0,   160,     0,   160,     0,   160,     0,   160,     0,   160,
       0,   160,     0,   160,     0,   160,     0,   160,     0,   160,
       0,   160,     0,   160,     0,   160,     0,   160,     0,   160,
       0,   216,     0,   148,     0,   148,     0,   148,     0,   148,
       0,   148,     0,   148,     0,   148,     0,   148,     0,   148,
       0,   148,     0,   148,     0,   148,     0,   148,     0,   148,
       0,   148,     0,   148,     0,   148,     0,   148,     0,   148,
       0,   148,     0,   148,     0,   148,     0,   148,     0,   148,
       0,   148,     0,   148,     0,   148,     0,   148,     0,   148,
       0,   148,     0,   148,     0,   148,     0,   148,     0,   148,
       0,   148,     0,   148,     0,   148,     0,   149,     0,   149,
       0,   149,     0,   149,     0,   149,     0,   149,     0,   149,
       0,   149,     0,   149,     0,   149,     0,   149,     0,   149,
       0,   149,     0,   149,     0,   149,     0,   149,     0,   149,
       0,   149,     0,   149,     0,   149,     0,   149,     0,   149,
       0,   149,     0,   149,     0,   149,     0,   149,     0,   149,
       0,   149,     0,   149,     0,   149,     0,   149,     0,   149,
       0,   149,     0,   149,     0,   149,     0,   149,     0,   149,
       0,   216,     0,    75,     0,    75,     0,    75,     0,    75,
       0,    75,     0,    75,     0,    75,     0,    75,     0,    75,
       0,    75,     0,    75,     0,    75,     0,    75,     0,    75,
       0,    75,     0,    75,     0,    75,     0,    75,     0,    75,
       0,    75,     0,    75,     0,    75,     0,    75,     0,    75,
       0,    75,     0,    75,     0,    75,     0,    75,     0,    75,
       0,    75,     0,    75,     0,   216,     0,   216,     0,   216,
       0,   216,     0,   173,     0,   173,     0,   169,     0,   169,
       0,   169,     0,   169,     0,   169,     0,   169,     0,   170,
       0,   170,     0,   170,     0,   170,     0,   170,     0,   170,
       0,   176,     0,   176,     0,   176,     0,   176,     0,   176,
       0,   176,     0,   175,     0,   175,     0,   175,     0,   175,
       0,   175,     0,   175,     0,   171,     0,   171,     0,   171,
       0,   171,     0,   171,     0,   171,     0,   171,     0,   171,
       0,   178,     0,   178,     0,   178,     0,   178,     0,   178,
       0,   178,     0,   178,     0,   178,     0,   178,     0,   178,
       0,   177,     0,   177,     0,   177,     0,   177,     0,   177,
       0,   177,     0,   177,     0,   177,     0,   177,     0,   177,
       0,   174,     0,   174,     0,   174,     0,   174,     0,   174,
       0,   174,     0,   174,     0,   174,     0,   174,     0,   174,
       0,   140,     0,   140,     0,   140,     0,   140,     0,   140,
       0,   140,     0,   140,     0,   140,     0,   140,     0,   140,
       0,   140,     0,   140,     0,   140,     0,   140,     0,   140,
       0,   140,     0,   140,     0,   140,     0,   140,     0,   140,
       0,   140,     0,   140,     0,   140,     0,   140,     0,   140,
       0,   140,     0,   140,     0,   140,     0,   140,     0,   140,
       0,   140,     0,   201,     0,   201,     0,   201,     0,   203,
       0,   203,     0,   203,     0,   204,     0,   204,     0,   204,
       0,   190,     0,   190,     0,   190,     0,   189,     0,   189,
       0,   189,     0,   188,     0,   188,     0,   188,     0,   187,
       0,   187,     0,   187,     0,   186,     0,   186,     0,   186,
       0,   185,     0,   185,     0,   185,     0,   191,     0,   191,
       0,   191,     0,   199,     0,   199,     0,   199,     0,   198,
       0,   198,     0,   198,     0,   200,     0,   200,     0,   200,
       0,   193,     0,   193,     0,   193,     0,   193,     0,   192,
       0,   192,     0,   192,     0,   192,     0,   197,     0,   197,
       0,   197,     0,   197,     0,   196,     0,   196,     0,   196,
       0,   196,     0,   195,     0,   195,     0,   195,     0,   195,
       0,   194,     0,   194,     0,   194,     0,   194,     0,   180,
       0,   180,     0,   180,     0,   180,     0,   180,     0,   180,
       0,   179,     0,   179,     0,   179,     0,   179,     0,   179,
       0,   179,     0,   183,     0,   183,     0,   183,     0,   183,
       0,   183,     0,   183,     0,   182,     0,   182,     0,   182,
       0,   182,     0,   182,     0,   182,     0,   181,     0,   181,
       0,   181,     0,   181,     0,   181,     0,   181,     0,   184,
       0,   184,     0,   184,     0,   184,     0,   184,     0,   184,
       0,   202,     0,   202,     0,   202,     0,   202,     0,   202,
       0,   202,     0,   202,     0,   202,     0,   202,     0,   142,
       0,   142,     0,   142,     0,   142,     0,   142,     0,   142,
       0,   142,     0,   142,     0,   142,     0,   142,     0,   142,
       0,   142,     0,   142,     0,   142,     0,   142,     0,   142,
       0,   142,     0,   142,     0,   142,     0,   142,     0,   142,
       0,   142,     0,   142,     0,   142,     0,   142,     0,   142,
       0,   142,     0,   142,     0,   142,     0,   142,     0,   142,
       0,   143,     0,   143,     0,   143,     0,   143,     0,   143,
       0,   143,     0,   143,     0,   143,     0,   143,     0,   143,
       0,   143,     0,   143,     0,   143,     0,   143,     0,   143,
       0,   143,     0,   143,     0,   143,     0,   143,     0,   143,
       0,   143,     0,   143,     0,   143,     0,   143,     0,   143,
       0,   143,     0,   143,     0,   143,     0,   143,     0,   143,
       0,   143,     0,   141,     0,   141,     0,   141,     0,   141,
       0,   141,     0,   141,     0,   141,     0,   141,     0,   141,
       0,   141,     0,   141,     0,   141,     0,   141,     0,   141,
       0,   141,     0,   141,     0,   141,     0,   141,     0,   141,
       0,   141,     0,   141,     0,   141,     0,   141,     0,   141,
       0,   141,     0,   141,     0,   141,     0,   141,     0,   141,
       0,   141,     0,   141,     0,   216,     0,   206,     0,   206,
       0,   206,     0,   206,     0,   206,     0,   206,     0,   206,
       0,   206,     0,   206,     0,   206,     0,   207,     0,   207,
       0,   207,     0,   207,     0,   207,     0,   207,     0,   207,
       0,   207,     0,   207,     0,   207,     0
};

static const short int yycheck[] =
{
      47,    68,   284,     8,    51,    20,    21,    22,    55,    70,
       3,   285,    73,     4,     0,    57,    60,     0,     3,     4,
      64,    14,     3,     3,    39,   112,   113,   114,   115,   116,
     117,    57,    17,    14,    19,    26,     5,    17,    23,    19,
       5,    57,    46,    47,    60,    49,    50,    51,    52,    53,
      54,    37,    56,    97,    37,     3,    60,     5,     5,   333,
      64,    46,    47,    68,    79,    57,    58,    60,    53,    54,
      55,     3,    57,     5,    65,    57,    61,    62,    63,    64,
      65,    61,    62,    63,    64,    60,    71,    72,    73,    74,
       7,    76,     9,    97,   166,   167,    57,   112,   113,   114,
     115,   116,   117,   385,    89,    90,    91,    92,    93,     3,
     125,    57,   127,    49,    50,    51,    52,    53,    54,    56,
      56,    46,    47,    60,    60,    60,    57,    64,    64,   196,
      57,   198,    28,    57,    57,    57,    57,    57,    57,    57,
      26,    28,    26,     7,    69,    70,     4,    19,    17,   196,
     211,   198,     4,    58,    58,    58,    58,    58,    19,    19,
      97,    97,    19,   178,    57,    57,    57,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,    23,    57,     3,   281,   282,    14,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    57,    49,    50,    51,
      52,    53,    54,    57,    56,    27,    57,    57,    60,   316,
      57,    57,    64,    65,    57,    57,    57,    57,    19,   254,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    19,    19,    97,   281,   282,   283,    19,
      57,     3,     5,    52,    53,    54,     5,    56,    58,   346,
     347,    60,    58,    58,    58,    64,    53,    54,    58,    56,
      26,     3,    65,    60,    58,    58,    58,    64,    58,   346,
     347,   316,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    58,    49,    50,    51,    52,    53,    54,    97,    56,
      58,     3,     0,    60,    58,    65,     3,    64,    58,    37,
      97,    58,    58,    58,    12,    13,    14,    15,    16,    17,
      18,    19,    53,    54,    58,    56,    40,   434,   125,    60,
     434,   317,    68,    64,    78,   191,    -1,    -1,    -1,    37,
      97,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    62,    63,    97,    -1,    -1,    -1,
      -1,    69,    70,    -1,    -1,    73,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    93,    94,    95,    96,   434,
      98,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   496,
      -1,    -1,    -1,    -1,   112,   113,   114,   115,   116,   117,
     118,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   496,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,    -1,    -1,    -1,   157,
     158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     0,    -1,    -1,     3,     4,
       5,     6,     7,    -1,     9,    10,    11,    12,    13,    14,
      15,    -1,    17,    -1,    19,    20,    -1,    -1,    23,    -1,
      -1,    -1,    27,    28,    -1,    -1,   204,   205,   206,   207,
     208,   209,   210,   211,    39,    -1,    -1,    -1,    -1,    -1,
      -1,    46,    47,    -1,    -1,    -1,    -1,    -1,    53,    54,
      55,    -1,    57,    58,    -1,    -1,    61,    62,    63,    64,
      -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,
      -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,    94,
      95,    96,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,   284,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    -1,    -1,    -1,    60,    -1,
      -1,    -1,    64,    -1,   322,   323,    -1,    -1,    -1,   327,
     328,    -1,    -1,    -1,   332,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   343,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,    -1,    -1,     0,   385,   386,     3,
       4,     5,     6,     7,    -1,     9,    10,    11,    12,    13,
      14,    15,    -1,    17,    -1,    19,    20,    -1,    -1,    23,
      -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    47,    -1,    -1,    -1,    -1,    -1,    53,
      54,    55,    -1,    57,    58,    -1,    -1,    61,    62,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    95,    96,     0,    -1,    -1,     3,     4,     5,     6,
       7,    -1,     9,    10,    11,    12,    13,    14,    15,    -1,
      17,    -1,    19,    20,    -1,    -1,    23,    -1,    -1,    -1,
      27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,    -1,
      57,    58,    -1,    -1,    61,    62,    63,    64,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,    76,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    90,    91,    92,    93,    94,    95,    96,
       0,    -1,    -1,     3,     4,     5,     6,     7,    -1,     9,
      10,    11,    12,    13,    14,    15,    -1,    17,    -1,    19,
      20,    -1,    -1,    23,    -1,    -1,    -1,    27,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    39,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    -1,    57,    58,    -1,
      -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    95,    96,     0,    -1,    -1,
       3,     4,     5,     6,     7,    -1,     9,    10,    11,    12,
      13,    14,    15,    -1,    17,    -1,    19,    20,    -1,    -1,
      23,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    -1,    -1,    46,    47,    -1,    -1,    -1,    -1,    -1,
      53,    54,    55,    -1,    57,    58,    -1,    -1,    61,    62,
      63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,
      73,    74,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,
      93,    94,    95,    96,     0,    -1,    -1,     3,    -1,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      -1,    17,    -1,    19,    20,    -1,    -1,    23,    -1,    -1,
      -1,    27,    -1,    -1,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    -1,
      46,    47,    60,    -1,    -1,    -1,    64,    53,    54,    55,
      -1,    57,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    97,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    95,
      96,     0,    -1,    -1,     3,    -1,     5,     6,    -1,    -1,
      -1,    10,    11,    12,    13,    14,    15,    -1,    17,    -1,
      19,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,
      -1,    -1,    -1,    -1,    53,    54,    55,    -1,    57,    -1,
      -1,    -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    71,    72,    73,    74,    -1,    76,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,     0,    -1,
      -1,     3,    -1,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    -1,    17,    -1,    19,    20,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    47,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55,    -1,    57,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    76,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    95,    96,     3,     4,     5,     6,    -1,
      -1,    -1,    10,    11,    12,    13,    14,    15,    -1,    17,
      -1,    19,    20,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,
      -1,    -1,    -1,    -1,    -1,    53,    54,    55,    -1,    57,
      -1,    -1,    -1,    61,    62,    63,    64,    -1,    -1,    -1,
      -1,    -1,    -1,    71,    72,    73,    74,    -1,    76,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    89,    90,    91,    92,    93,    94,    95,    96,     3,
       4,     5,     6,    -1,    -1,    -1,    10,    11,    12,    13,
      14,    15,    -1,    17,    -1,    19,    20,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    47,    -1,    -1,    -1,    -1,    -1,    53,
      54,    55,    -1,    57,    -1,    -1,    -1,    61,    62,    63,
      64,    -1,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    -1,    76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      94,    95,    96,     3,     4,     5,     6,    -1,    -1,    -1,
      10,    11,    12,    13,    14,    15,    -1,    17,    -1,    19,
      20,    -1,    -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    -1,    57,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    94,    95,    96,     3,    -1,     5,
       6,    -1,    -1,    -1,    10,    11,    12,    13,    14,    15,
      -1,    17,    -1,    19,    20,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    47,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,
      -1,    57,    -1,    -1,    -1,    61,    62,    63,    64,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      76,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    94,    95,
      96,     3,    -1,     5,     6,    -1,    -1,    -1,    10,    11,
      12,    13,    14,    15,    -1,    17,    -1,    19,    20,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    47,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55,    -1,    57,    -1,    -1,    -1,    61,
      62,    63,    64,    -1,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    76,    -1,    -1,     5,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    94,    95,    96,    23,    -1,    -1,    26,    -1,
      -1,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      58,    -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,
      68,    69,    70,    -1,    -1,    -1,     5,    75,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    97,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    52,    53,    54,    -1,    56,    57,    58,
      -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,
      69,    70,    -1,    -1,    -1,     5,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    52,    53,    54,    -1,    56,    57,    58,    -1,
      60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,
      70,    -1,    -1,    -1,     5,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    58,    -1,    60,
      -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,    70,
      -1,    -1,    -1,     5,    75,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    -1,
      -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    58,    -1,    60,    -1,
      -1,    -1,    64,    -1,    66,    67,    68,    69,    70,    -1,
      -1,    -1,     5,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    -1,    -1,
      23,    -1,    -1,    26,    -1,    97,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    -1,    60,    -1,    -1,
      -1,    64,    -1,    66,    67,    68,    69,    70,    -1,    -1,
      -1,     5,    75,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    -1,    -1,    23,
      -1,    -1,    26,    -1,    97,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    52,    53,
      54,    -1,    56,    57,    58,    -1,    60,    -1,    -1,    -1,
      64,    -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,
       5,    75,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    -1,    -1,    23,    -1,
      -1,    26,    -1,    97,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    52,    53,    54,
      -1,    56,    57,    58,    -1,    60,    -1,    -1,    -1,    64,
      -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,     5,
      75,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    -1,    -1,    23,    -1,    -1,
      26,    -1,    97,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    58,    -1,    60,    -1,    -1,    -1,    64,    -1,
      66,    67,    68,    69,    70,    -1,    -1,    -1,     5,    75,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    -1,    -1,    23,    -1,    -1,    26,
      -1,    97,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    58,    -1,    60,    -1,    -1,    -1,    64,    -1,    66,
      67,    68,    69,    70,    -1,    -1,    -1,     5,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,
      97,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      58,    -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,
      68,    69,    70,    -1,    -1,    -1,     5,    75,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    97,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    52,    53,    54,    -1,    56,    57,    58,
      -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,
      69,    70,    -1,    -1,    -1,     5,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    52,    53,    54,    -1,    56,    57,    58,    -1,
      60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,
      70,    -1,    -1,    -1,     5,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    58,    -1,    60,
      -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,    70,
      -1,    -1,    -1,     5,    75,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    -1,
      -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    58,    -1,    60,    -1,
      -1,    -1,    64,    -1,    66,    67,    68,    69,    70,    -1,
      -1,    -1,     5,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    -1,    -1,
      23,    -1,    -1,    26,    -1,    97,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    -1,    60,    -1,    -1,
      -1,    64,    -1,    66,    67,    68,    69,    70,    -1,    -1,
      -1,     5,    75,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    -1,    -1,    23,
      -1,    -1,    26,    -1,    97,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    52,    53,
      54,    -1,    56,    57,    58,    -1,    60,    -1,    -1,    -1,
      64,    -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,
       5,    75,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    -1,    -1,    23,    -1,
      -1,    26,    -1,    97,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    52,    53,    54,
      -1,    56,    57,    58,    -1,    60,    -1,    -1,    -1,    64,
      -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,     5,
      75,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    -1,    -1,    23,    -1,    -1,
      26,    -1,    97,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    58,    -1,    60,    -1,    -1,    -1,    64,    -1,
      66,    67,    68,    69,    70,    -1,    -1,    -1,     5,    75,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    -1,    -1,    23,    -1,    -1,    26,
      -1,    97,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    58,    -1,    60,    -1,    -1,    -1,    64,    -1,    66,
      67,    68,    69,    70,    -1,    -1,    -1,     5,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,
      97,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      58,    -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,
      68,    69,    70,    -1,    -1,    -1,     5,    75,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    97,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    52,    53,    54,    -1,    56,    57,    58,
      -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,
      69,    70,    -1,    -1,    -1,     5,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    52,    53,    54,    -1,    56,    57,    58,    -1,
      60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,
      70,    -1,    -1,    -1,     5,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    58,    -1,    60,
      -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,    70,
      -1,    -1,    -1,     5,    75,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    -1,
      -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    58,    -1,    60,    -1,
      -1,    -1,    64,    -1,    66,    67,    68,    69,    70,    -1,
      -1,    -1,     5,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    -1,    -1,
      23,    -1,    -1,    26,    -1,    97,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    -1,    60,    -1,    -1,
      -1,    64,    -1,    66,    67,    68,    69,    70,    -1,    -1,
      -1,     5,    75,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    -1,    -1,    23,
      -1,    -1,    26,    -1,    97,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    52,    53,
      54,    -1,    56,    57,    58,    -1,    60,    -1,    -1,    -1,
      64,    -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,
       5,    75,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    -1,    -1,    23,    -1,
      -1,    26,    -1,    97,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    52,    53,    54,
      -1,    56,    57,    58,    -1,    60,    -1,    -1,    -1,    64,
      -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,     5,
      75,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    -1,    -1,    23,    -1,    -1,
      26,    -1,    97,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    58,    -1,    60,    -1,    -1,    -1,    64,    -1,
      66,    67,    68,    69,    70,    -1,    -1,    -1,     5,    75,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    -1,    -1,    23,    -1,    -1,    26,
      -1,    97,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    58,    -1,    60,    -1,    -1,    -1,    64,    -1,    66,
      67,    68,    69,    70,    -1,    -1,    -1,     5,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,
      97,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,
      68,    69,    70,    -1,    -1,    -1,     5,    75,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    97,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    52,    53,    54,    -1,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,
      69,    70,    -1,    -1,    -1,     5,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    52,    53,    54,    -1,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,
      70,    -1,    -1,    -1,     5,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    -1,    40,
      41,    42,    43,    44,    45,    -1,    -1,    -1,    49,    50,
      51,    52,    -1,    -1,    -1,    56,    -1,    58,    -1,    60,
      -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,    70,
      -1,    -1,    -1,     5,    75,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    -1,
      -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    -1,    40,    41,
      42,    43,    44,    45,    -1,    -1,    -1,    49,    50,    51,
      52,    -1,    -1,    -1,    56,    -1,    58,    -1,    60,    -1,
      -1,    -1,    64,    -1,    66,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,     3,    -1,     5,
      -1,    -1,    -1,    -1,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    17,    -1,    19,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    47,    -1,    -1,    -1,    -1,    -1,    53,    54,    55,
      -1,    57,    -1,    -1,    -1,    61,    62,    63,    64,     3,
      -1,    -1,    -1,    -1,    -1,    71,    72,    73,    74,    -1,
      76,    -1,    -1,    17,    -1,    19,    -1,    -1,    -1,    23,
      -1,    -1,    -1,    89,    90,    91,    92,    93,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    47,    -1,    -1,    -1,    -1,    -1,    53,
      54,    55,    -1,    57,    -1,    -1,    -1,    61,    62,    63,
      64,     3,    -1,    -1,    -1,    -1,    -1,    71,    72,    73,
      74,    -1,    76,    -1,    -1,    17,    -1,    19,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    89,    90,    91,    92,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    46,    47,    -1,    -1,    -1,    -1,
      -1,    53,    54,    55,    -1,    57,    -1,    -1,    -1,    61,
      62,    63,    64,     3,    -1,    -1,    -1,    -1,    -1,    71,
      72,    73,    74,    -1,    76,    -1,    -1,    17,    -1,    19,
      -1,    -1,    -1,    23,    -1,    -1,    -1,    89,    90,    91,
      92,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    -1,    -1,
      -1,    -1,    -1,    53,    54,    55,    -1,    57,    -1,    -1,
      -1,    61,    62,    63,    64,    -1,    -1,    -1,    -1,    -1,
      -1,    71,    72,    73,    74,    -1,    76,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,
      90,    91,    92,    93,    23,    -1,    -1,    26,    -1,    -1,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    52,    53,    54,    -1,    56,    57,    58,
      -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    52,    53,    54,    -1,    56,    57,    58,    -1,
      60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    58,    -1,    60,
      -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    -1,
      -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    58,    -1,    60,    -1,
      -1,    -1,    64,    -1,    66,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    -1,    -1,
      23,    -1,    -1,    26,    -1,    97,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    -1,    60,    -1,    -1,
      -1,    64,    -1,    66,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    -1,    -1,    23,
      -1,    -1,    26,    -1,    97,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    52,    53,
      54,    -1,    56,    57,    58,    -1,    60,    -1,    -1,    -1,
      64,    -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    -1,    -1,    23,    -1,
      -1,    26,    -1,    97,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    52,    53,    54,
      -1,    56,    57,    58,    -1,    60,    -1,    -1,    -1,    64,
      -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    -1,    -1,    23,    -1,    -1,
      26,    -1,    97,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    58,    -1,    60,    -1,    -1,    -1,    64,    -1,
      66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    -1,    -1,    23,    -1,    -1,    26,
      -1,    97,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    58,    -1,    60,    -1,    -1,    -1,    64,    -1,    66,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,
      97,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      58,    -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    97,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    52,    53,    54,    -1,    56,    57,    58,
      -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    52,    53,    54,    -1,    56,    57,    58,    -1,
      60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    58,    -1,    60,
      -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    -1,
      -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    58,    -1,    60,    -1,
      -1,    -1,    64,    -1,    66,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    -1,    -1,
      23,    -1,    -1,    26,    -1,    97,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    58,    -1,    60,    -1,    -1,
      -1,    64,    -1,    66,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    -1,    -1,    23,
      -1,    -1,    26,    -1,    97,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    52,    53,
      54,    -1,    56,    57,    58,    -1,    60,    -1,    -1,    -1,
      64,    -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    -1,    -1,    23,    -1,
      -1,    26,    -1,    97,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    52,    53,    54,
      -1,    56,    57,    58,    -1,    60,    -1,    -1,    -1,    64,
      -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    -1,    -1,    23,    -1,    -1,
      26,    -1,    97,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    58,    -1,    60,    -1,    -1,    -1,    64,    -1,
      66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    -1,    -1,    23,    -1,    -1,    26,
      -1,    97,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      57,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    66,
      67,    68,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,
      97,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    97,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      49,    50,    51,    52,    53,    54,    -1,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    -1,    49,
      50,    51,    52,    53,    54,    -1,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      -1,    -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    -1,    49,    50,
      51,    52,    53,    54,    -1,    56,    57,    -1,    -1,    60,
      -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,    70,
      -1,    -1,    -1,    -1,    75,    -1,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    -1,    -1,
      -1,    23,    -1,    -1,    26,    -1,    97,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    -1,    -1,    60,    -1,
      -1,    -1,    64,    -1,    66,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    -1,    -1,
      23,    -1,    -1,    26,    -1,    97,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    -1,    49,    50,    51,    52,
      53,    54,    -1,    56,    57,    -1,    -1,    60,    -1,    -1,
      -1,    64,    -1,    66,    67,    68,    69,    70,    -1,    -1,
      -1,    -1,    75,    -1,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    -1,    -1,    -1,    23,
      -1,    -1,    26,    -1,    97,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    52,    53,
      54,    -1,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,
      64,    -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,
      -1,    75,    -1,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    26,    -1,    97,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    -1,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    49,    50,    51,    52,    53,    54,
      -1,    56,    57,    -1,    -1,    60,    -1,    -1,    -1,    64,
      -1,    66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,
      75,    -1,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    97,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    49,    50,    51,    52,    53,    54,    -1,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,
      66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    97,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    49,    50,    51,    52,    53,    54,    -1,    56,
      -1,    -1,    -1,    60,    -1,    -1,    -1,    64,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    49,    50,    51,
      52,    53,    54,    -1,    56,    57,    -1,    -1,    60,    -1,
      97,    -1,    64,    -1,    66,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    97,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    -1,    49,    50,    51,    52,    53,
      54,    -1,    56,    -1,    -1,    -1,    60,    -1,    -1,    -1,
      64,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    49,    50,    51,    52,    53,    54,    -1,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,
      68,    69,    70,    97,    -1,    -1,    -1,    75,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    -1,    42,    43,    44,    45,    46,    47,    97,
      49,    50,    51,    52,    53,    54,    -1,    56,    57,    -1,
      -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,
      69,    70,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    97,    49,
      50,    51,    52,    53,    54,    -1,    56,    57,    -1,    -1,
      60,    -1,    -1,    -1,    64,    -1,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      -1,    49,    50,    51,    52,    53,    54,    97,    56,    57,
      -1,    -1,    60,    -1,    -1,    -1,    64,    -1,    66,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    -1,    -1,    49,    50,    51,    52,    53,    54,    97,
      56,    57,    -1,    -1,    60,    -1,    -1,    -1,    64,    -1,
      66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    52,    53,    54,    -1,    56,    57,
      -1,    97,    60,    -1,    -1,    -1,    64,    -1,    66,    67,
      68,    69,    70,    -1,    -1,    -1,    -1,    75,    -1,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    53,    54,    -1,    56,    57,    -1,    -1,    60,    97,
      -1,    -1,    64,    -1,    66,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    53,    54,    -1,
      56,    57,    -1,    -1,    60,    97,    -1,    -1,    64,    -1,
      66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    53,    54,    -1,    56,    57,    -1,    -1,
      60,    97,    -1,    -1,    64,    -1,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      56,    57,    -1,    -1,    60,    -1,    -1,    97,    64,    -1,
      66,    67,    68,    69,    70,    -1,    -1,    -1,    -1,    75,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    -1,    56,    57,    -1,    -1,    60,    -1,
      -1,    97,    64,    -1,    66,    67,    68,    69,    70,    -1,
      -1,    -1,    -1,    75,    -1,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    -1,    -1,    -1,
      60,    -1,    -1,    -1,    64,    97,    66,    67,    68,    69,
      70,    -1,    -1,    -1,    -1,    75,    -1,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    97
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,     5,     6,    10,    11,    12,    13,    14,    15,
      17,    19,    20,    23,    46,    47,    53,    54,    55,    57,
      61,    62,    63,    64,    71,    72,    73,    74,    76,    89,
      90,    91,    92,    93,    94,    95,    96,   100,   101,   104,
     105,   107,   113,   114,   116,   117,   118,   119,   120,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,    57,    57,     5,     5,     5,     3,    60,   125,
     126,   127,   128,   129,   131,   132,   133,    60,    57,    60,
       3,     5,   116,   116,   116,   116,   116,   116,   116,   116,
     114,   114,   114,    57,    57,    57,    57,     3,   121,   122,
     123,    57,    57,    57,    57,    57,    57,    57,    57,     0,
     101,     4,    23,    46,    47,    53,    54,    55,    57,    65,
     102,   103,   106,   114,   116,    26,   106,    60,     5,    23,
      26,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      49,    50,    51,    52,    53,    54,    56,    57,    60,    64,
      66,    67,    68,    69,    70,    75,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    97,     4,
     117,   136,   140,     4,   117,   120,    28,    26,     3,     5,
     120,    27,    28,    26,     5,   120,     7,     9,     7,   116,
     116,   128,   132,   133,    23,    39,    46,    47,    53,    54,
      55,    57,   116,   117,   124,   117,   124,     4,   116,   124,
     130,    19,    17,   113,   114,     5,    58,   116,   116,   116,
     116,   116,     4,    58,    58,    58,    58,    58,    19,    19,
      19,   102,   116,   102,   116,   102,   116,   102,   116,   102,
     116,   102,   116,   116,    23,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    49,    50,    51,    52,    53,    54,
      56,    60,    64,    97,    57,    64,   103,   114,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   115,   116,   116,   109,   110,   112,    57,
      57,    57,    57,    57,    57,   123,   123,    57,    57,    57,
      57,    57,    57,    57,    57,   114,    19,    19,   135,    19,
      19,   120,   132,    57,   120,   132,    58,    58,   116,   116,
     116,   116,   116,   116,   116,   116,   124,    23,    26,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    49,    50,
      51,    52,    53,    54,    56,    57,    60,     5,     5,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   114,   115,   116,   112,    58,
      58,   102,   108,   111,    26,   111,    65,    58,    58,    58,
     116,   116,    58,   116,   116,    58,    58,    58,   116,   112,
      58,   116,   120,   132,   120,   132,    58,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   116,   115,   116,   116,    65,    58,    58,    65,   108,
      58,    58,    58,    58,    58,    58,    58,    58,    58,   120,
     132
};

/* Error token number */
#define YYTERROR 1


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#define YYLEX yylex ()

YYSTYPE yylval;

int yynerrs;
int yychar;

static const int YYEOF = 0;
static const int YYEMPTY = -2;

typedef enum { yyok, yyaccept, yyabort, yyerr } YYRESULTTAG;

#define YYCHK(YYE)                                                           \
   do { YYRESULTTAG yyflag = YYE; if (yyflag != yyok) return yyflag; }       \
   while (YYID (0))

#if YYDEBUG

# ifndef YYFPRINTF
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)          \
do {                                                            \
  if (yydebug)                                                  \
    {                                                           \
      YYFPRINTF (stderr, "%s ", Title);                         \
      yy_symbol_print (stderr, Type, Value);        \
      YYFPRINTF (stderr, "\n");                                 \
    }                                                           \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;

#else /* !YYDEBUG */

# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)

#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYMAXDEPTH * sizeof (GLRStackItem)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

/* Minimum number of free items on the stack allowed after an
   allocation.  This is to allow allocation and initialization
   to be completed by functions that call yyexpandGLRStack before the
   stack is expanded, thus insuring that all necessary pointers get
   properly redirected to new data.  */
#define YYHEADROOM 2

#ifndef YYSTACKEXPANDABLE
# if (! defined __cplusplus \
      || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL))
#  define YYSTACKEXPANDABLE 1
# else
#  define YYSTACKEXPANDABLE 0
# endif
#endif

#if YYSTACKEXPANDABLE
# define YY_RESERVE_GLRSTACK(Yystack)                   \
  do {                                                  \
    if (Yystack->yyspaceLeft < YYHEADROOM)              \
      yyexpandGLRStack (Yystack);                       \
  } while (YYID (0))
#else
# define YY_RESERVE_GLRSTACK(Yystack)                   \
  do {                                                  \
    if (Yystack->yyspaceLeft < YYHEADROOM)              \
      yyMemoryExhausted (Yystack);                      \
  } while (YYID (0))
#endif


#if YYERROR_VERBOSE

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static size_t
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return strlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* !YYERROR_VERBOSE */

/** State numbers, as in LALR(1) machine */
typedef int yyStateNum;

/** Rule numbers, as in LALR(1) machine */
typedef int yyRuleNum;

/** Grammar symbol */
typedef short int yySymbol;

/** Item references, as in LALR(1) machine */
typedef short int yyItemNum;

typedef struct yyGLRState yyGLRState;
typedef struct yyGLRStateSet yyGLRStateSet;
typedef struct yySemanticOption yySemanticOption;
typedef union yyGLRStackItem yyGLRStackItem;
typedef struct yyGLRStack yyGLRStack;

struct yyGLRState {
  /** Type tag: always true.  */
  yybool yyisState;
  /** Type tag for yysemantics.  If true, yysval applies, otherwise
   *  yyfirstVal applies.  */
  yybool yyresolved;
  /** Number of corresponding LALR(1) machine state.  */
  yyStateNum yylrState;
  /** Preceding state in this stack */
  yyGLRState* yypred;
  /** Source position of the first token produced by my symbol */
  size_t yyposn;
  union {
    /** First in a chain of alternative reductions producing the
     *  non-terminal corresponding to this state, threaded through
     *  yynext.  */
    yySemanticOption* yyfirstVal;
    /** Semantic value for this state.  */
    YYSTYPE yysval;
  } yysemantics;
};

struct yyGLRStateSet {
  yyGLRState** yystates;
  /** During nondeterministic operation, yylookaheadNeeds tracks which
   *  stacks have actually needed the current lookahead.  During deterministic
   *  operation, yylookaheadNeeds[0] is not maintained since it would merely
   *  duplicate yychar != YYEMPTY.  */
  yybool* yylookaheadNeeds;
  size_t yysize, yycapacity;
};

struct yySemanticOption {
  /** Type tag: always false.  */
  yybool yyisState;
  /** Rule number for this reduction */
  yyRuleNum yyrule;
  /** The last RHS state in the list of states to be reduced.  */
  yyGLRState* yystate;
  /** The lookahead for this reduction.  */
  int yyrawchar;
  YYSTYPE yyval;
  /** Next sibling in chain of options.  To facilitate merging,
   *  options are chained in decreasing order by address.  */
  yySemanticOption* yynext;
};

/** Type of the items in the GLR stack.  The yyisState field
 *  indicates which item of the union is valid.  */
union yyGLRStackItem {
  yyGLRState yystate;
  yySemanticOption yyoption;
};

struct yyGLRStack {
  int yyerrState;


  YYJMP_BUF yyexception_buffer;
  yyGLRStackItem* yyitems;
  yyGLRStackItem* yynextFree;
  size_t yyspaceLeft;
  yyGLRState* yysplitPoint;
  yyGLRState* yylastDeleted;
  yyGLRStateSet yytops;
};

#if YYSTACKEXPANDABLE
static void yyexpandGLRStack (yyGLRStack* yystackp);
#endif

static void yyFail (yyGLRStack* yystackp, const char* yymsg)
  __attribute__ ((__noreturn__));
static void
yyFail (yyGLRStack* yystackp, const char* yymsg)
{
  if (yymsg != YY_NULL)
    yyerror (yymsg);
  YYLONGJMP (yystackp->yyexception_buffer, 1);
}

static void yyMemoryExhausted (yyGLRStack* yystackp)
  __attribute__ ((__noreturn__));
static void
yyMemoryExhausted (yyGLRStack* yystackp)
{
  YYLONGJMP (yystackp->yyexception_buffer, 2);
}

#if YYDEBUG || YYERROR_VERBOSE
/** A printable representation of TOKEN.  */
static inline const char*
yytokenName (yySymbol yytoken)
{
  if (yytoken == YYEMPTY)
    return "";

  return yytname[yytoken];
}
#endif

/** Fill in YYVSP[YYLOW1 .. YYLOW0-1] from the chain of states starting
 *  at YYVSP[YYLOW0].yystate.yypred.  Leaves YYVSP[YYLOW1].yystate.yypred
 *  containing the pointer to the next state in the chain.  */
static void yyfillin (yyGLRStackItem *, int, int) __attribute__ ((__unused__));
static void
yyfillin (yyGLRStackItem *yyvsp, int yylow0, int yylow1)
{
  int i;
  yyGLRState *s = yyvsp[yylow0].yystate.yypred;
  for (i = yylow0-1; i >= yylow1; i -= 1)
    {
      YYASSERT (s->yyresolved);
      yyvsp[i].yystate.yyresolved = yytrue;
      yyvsp[i].yystate.yysemantics.yysval = s->yysemantics.yysval;
      s = yyvsp[i].yystate.yypred = s->yypred;
    }
}

/* Do nothing if YYNORMAL or if *YYLOW <= YYLOW1.  Otherwise, fill in
 * YYVSP[YYLOW1 .. *YYLOW-1] as in yyfillin and set *YYLOW = YYLOW1.
 * For convenience, always return YYLOW1.  */
static inline int yyfill (yyGLRStackItem *, int *, int, yybool)
     __attribute__ ((__unused__));
static inline int
yyfill (yyGLRStackItem *yyvsp, int *yylow, int yylow1, yybool yynormal)
{
  if (!yynormal && yylow1 < *yylow)
    {
      yyfillin (yyvsp, *yylow, yylow1);
      *yylow = yylow1;
    }
  return yylow1;
}

/** Perform user action for rule number YYN, with RHS length YYRHSLEN,
 *  and top stack item YYVSP.  YYLVALP points to place to put semantic
 *  value ($$), and yylocp points to place for location information
 *  (@$).  Returns yyok for normal return, yyaccept for YYACCEPT,
 *  yyerr for YYERROR, yyabort for YYABORT.  */
/*ARGSUSED*/ static YYRESULTTAG
yyuserAction (yyRuleNum yyn, int yyrhslen, yyGLRStackItem* yyvsp,
              yyGLRStack* yystackp,
              YYSTYPE* yyvalp)
{
  yybool yynormal __attribute__ ((__unused__)) =
    (yystackp->yysplitPoint == YY_NULL);
  int yylow;
# undef yyerrok
# define yyerrok (yystackp->yyerrState = 0)
# undef YYACCEPT
# define YYACCEPT return yyaccept
# undef YYABORT
# define YYABORT return yyabort
# undef YYERROR
# define YYERROR return yyerrok, yyerr
# undef YYRECOVERING
# define YYRECOVERING() (yystackp->yyerrState != 0)
# undef yyclearin
# define yyclearin (yychar = YYEMPTY)
# undef YYFILL
# define YYFILL(N) yyfill (yyvsp, &yylow, N, yynormal)
# undef YYBACKUP
# define YYBACKUP(Token, Value)                                              \
  return yyerror (YY_("syntax error: cannot back up")),     \
         yyerrok, yyerr

  yylow = 1;
  if (yyrhslen == 0)
    *yyvalp = yyval_default;
  else
    *yyvalp = yyvsp[YYFILL (1-yyrhslen)].yystate.yysemantics.yysval;
  switch (yyn)
    {
        case 2:
/* Line 868 of glr.c  */
#line 137 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>code:environment");matchRule(0, Rule::code___environment, NULL,NULL,NULL,NULL);}
    break;

  case 3:
/* Line 868 of glr.c  */
#line 138 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>code:code	environment");matchRule(0, Rule::code___code_environment, NULL,NULL,NULL,NULL);}
    break;

  case 4:
/* Line 868 of glr.c  */
#line 142 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>environment:functionImplement");matchRule(0, Rule::environment___functionImplement, NULL,NULL,NULL,NULL);}
    break;

  case 5:
/* Line 868 of glr.c  */
#line 143 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>environment:systemImplement");matchRule(0, Rule::environment___systemImplement, NULL,NULL,NULL,NULL);}
    break;

  case 6:
/* Line 868 of glr.c  */
#line 144 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>environment:scope");matchRule(0, Rule::environment___scope, NULL,NULL,NULL,NULL);}
    break;

  case 7:
/* Line 868 of glr.c  */
#line 145 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>environment:sentence");matchRule(0, Rule::environment___sentence, NULL,NULL,NULL,NULL);}
    break;

  case 8:
/* Line 868 of glr.c  */
#line 149 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:_arg_");matchRule(0, Rule::expression____arg_,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 9:
/* Line 868 of glr.c  */
#line 150 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	MI	strictExpression");matchRule(1, Rule::expression___expression_MI_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 10:
/* Line 868 of glr.c  */
#line 151 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:JIAN	strictExpression");matchRule(0, Rule::expression___JIAN_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 11:
/* Line 868 of glr.c  */
#line 152 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:JIA	strictExpression");matchRule(0, Rule::expression___JIA_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 12:
/* Line 868 of glr.c  */
#line 153 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:NOT	strictExpression");matchRule(1, Rule::expression___NOT_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 13:
/* Line 868 of glr.c  */
#line 155 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:USER_OP2	strictExpression");matchRule(1, Rule::expression___USER_OP2_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 14:
/* Line 868 of glr.c  */
#line 156 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	USER_OP3");matchRule(1, Rule::expression___expression_USER_OP3,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 15:
/* Line 868 of glr.c  */
#line 157 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:SI	strictExpression");matchRule(1, Rule::expression___SI_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 16:
/* Line 868 of glr.c  */
#line 158 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:SD	strictExpression");matchRule(1, Rule::expression___SD_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 17:
/* Line 868 of glr.c  */
#line 159 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	SI");matchRule(1, Rule::expression___expression_SI,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 18:
/* Line 868 of glr.c  */
#line 160 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	SD");matchRule(1, Rule::expression___expression_SD,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 19:
/* Line 868 of glr.c  */
#line 161 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	AND	strictExpression");matchRule(1, Rule::expression___expression_AND_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 20:
/* Line 868 of glr.c  */
#line 162 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	OR	strictExpression");matchRule(1, Rule::expression___expression_OR_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 21:
/* Line 868 of glr.c  */
#line 163 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	USER_OP1	strictExpression");matchRule(1, Rule::expression___expression_USER_OP1_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 22:
/* Line 868 of glr.c  */
#line 164 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	USER_OP2	strictExpression");matchRule(1, Rule::expression___expression_USER_OP2_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 23:
/* Line 868 of glr.c  */
#line 165 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	DOT	strictExpression");matchRule(1, Rule::expression___expression_DOT_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 24:
/* Line 868 of glr.c  */
#line 166 "coolang_compiler_auto_integrate.y"
    {}
    break;

  case 25:
/* Line 868 of glr.c  */
#line 167 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	JIA	strictExpression");matchRule(1, Rule::expression___expression_JIA_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 26:
/* Line 868 of glr.c  */
#line 168 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	JIAN	strictExpression");matchRule(1, Rule::expression___expression_JIAN_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 27:
/* Line 868 of glr.c  */
#line 169 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	CHENG	strictExpression");matchRule(1, Rule::expression___expression_CHENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 28:
/* Line 868 of glr.c  */
#line 170 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	CHU	strictExpression");matchRule(1, Rule::expression___expression_CHU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 29:
/* Line 868 of glr.c  */
#line 171 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	MODULO	strictExpression");matchRule(1, Rule::expression___expression_MODULO_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 30:
/* Line 868 of glr.c  */
#line 172 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	JIADENG	strictExpression");matchRule(1, Rule::expression___expression_JIADENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 31:
/* Line 868 of glr.c  */
#line 173 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	JIANDENG	strictExpression");matchRule(1, Rule::expression___expression_JIANDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 32:
/* Line 868 of glr.c  */
#line 174 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	CHENGDENG	strictExpression");matchRule(1, Rule::expression___expression_CHENGDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 33:
/* Line 868 of glr.c  */
#line 175 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	CHUDENG	strictExpression");matchRule(1, Rule::expression___expression_CHUDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 34:
/* Line 868 of glr.c  */
#line 176 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	MODENG	strictExpression");matchRule(1, Rule::expression___expression_MODENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 35:
/* Line 868 of glr.c  */
#line 177 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	MIDENG	strictExpression");matchRule(1, Rule::expression___expression_MIDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 36:
/* Line 868 of glr.c  */
#line 178 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	FUZHI	strictExpression");matchRule(1, Rule::expression___expression_FUZHI_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 37:
/* Line 868 of glr.c  */
#line 179 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	DENGYU	strictExpression");matchRule(1, Rule::expression___expression_DENGYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 38:
/* Line 868 of glr.c  */
#line 180 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	BUDENG	strictExpression");matchRule(1, Rule::expression___expression_BUDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 39:
/* Line 868 of glr.c  */
#line 181 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	DAYU	strictExpression");matchRule(1, Rule::expression___expression_DAYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 40:
/* Line 868 of glr.c  */
#line 182 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	XIAOYU	strictExpression");matchRule(1, Rule::expression___expression_XIAOYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 41:
/* Line 868 of glr.c  */
#line 183 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	BUDAYU	strictExpression");matchRule(1, Rule::expression___expression_BUDAYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 42:
/* Line 868 of glr.c  */
#line 184 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	BUXIAOYU	strictExpression");matchRule(1, Rule::expression___expression_BUXIAOYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 43:
/* Line 868 of glr.c  */
#line 185 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	DOTDOT	strictExpression");matchRule(1, Rule::expression___expression_DOTDOT_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 44:
/* Line 868 of glr.c  */
#line 186 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:strictExpression	SHUCHU	strictExpression");matchRule(1, Rule::expression___expression_SHUCHU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 45:
/* Line 868 of glr.c  */
#line 187 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:LSCUR	expression	RSCUR");matchRule(0, Rule::expression___LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 46:
/* Line 868 of glr.c  */
#line 188 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:expression	LMCUR	listAccess	RMCUR");matchRule(1, Rule::expression___expression_LMCUR_listAccess_RMCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 47:
/* Line 868 of glr.c  */
#line 189 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:expression	LSCUR	expression_null	RSCUR");matchRule(1, Rule::expression___expression_LSCUR_expression_null_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 48:
/* Line 868 of glr.c  */
#line 190 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>strictExpression:expression	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 49:
/* Line 868 of glr.c  */
#line 195 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionForList:	strictExpression");matchRule(0, Rule::expressionForList___strictExpression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 50:
/* Line 868 of glr.c  */
#line 197 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>listBegin:	LBCUR");matchRule(1, Rule::listBegin___LBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 51:
/* Line 868 of glr.c  */
#line 198 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>listBegin:	LMCUR");matchRule(1, Rule::listBegin___LBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 52:
/* Line 868 of glr.c  */
#line 200 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>listHalf:listBegin	expressionForList");matchRule(1, Rule::listHalf___listBegin_expressionForList,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 53:
/* Line 868 of glr.c  */
#line 201 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>listHalf:listHalf	COMMA	expressionForList");matchRule(1, Rule::listHalf___listBegin_expressionForList,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 54:
/* Line 868 of glr.c  */
#line 203 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>listEnd:RBCUR");}
    break;

  case 55:
/* Line 868 of glr.c  */
#line 204 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>listEnd:RMCUR");}
    break;

  case 56:
/* Line 868 of glr.c  */
#line 206 "coolang_compiler_auto_integrate.y"
    {puts("list:listBegin	listEnd");matchRule(0, Rule::list___listBegin_listEnd,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 57:
/* Line 868 of glr.c  */
#line 207 "coolang_compiler_auto_integrate.y"
    {puts("list:listHalf	listEnd");matchRule(0, Rule::list___listHalf_listEnd,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 58:
/* Line 868 of glr.c  */
#line 211 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionForList:	strictExpression");matchRule(0, Rule::expressionForList___strictExpression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 59:
/* Line 868 of glr.c  */
#line 213 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>listAccessBegin:NULL");((*yyvalp).argp)=0;matchRule(1, Rule::listAccessBegin___NULL,NULL,NULL,NULL,&((*yyvalp).argp));}
    break;

  case 60:
/* Line 868 of glr.c  */
#line 215 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>listAccessHalf:listAccessBegin	expressionForListAccess");matchRule(1, Rule::listAccessHalf___listAccessBegin_expressionForListAccess,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 61:
/* Line 868 of glr.c  */
#line 216 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>listAccessHalf:listAccessHalf	COMMA	expressionForListAccess");matchRule(1, Rule::listAccessHalf___listAccessBegin_expressionForListAccess,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 62:
/* Line 868 of glr.c  */
#line 218 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>listAccessEnd:NULL");}
    break;

  case 63:
/* Line 868 of glr.c  */
#line 220 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>listAccess:listAccessBegin	listAccessEnd");matchRule(0, Rule::listAccess___listAccessBegin_listAccessEnd,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 64:
/* Line 868 of glr.c  */
#line 221 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>listAccess:listAccessHalf	listAccessEnd");matchRule(0, Rule::listAccess___listAccessHalf_listAccessEnd,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 65:
/* Line 868 of glr.c  */
#line 224 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>variable_composed_attribute:	variable_specific_attribute	LSCUR	variable_specific_attribute	RSCUR");matchRule(1, Rule::variable_specific_attribute___variable_specific_attribute_LSCUR_variable_specific_attribute_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 66:
/* Line 868 of glr.c  */
#line 225 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>variable_composed_attribute:	variable_specific_attribute	LSCUR	variable_composed_attribute	RSCUR");matchRule(1, Rule::variable_specific_attribute___variable_specific_attribute_LSCUR_variable_specific_attribute_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 67:
/* Line 868 of glr.c  */
#line 227 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>_arg_:arg");matchRule(0, Rule::_arg____arg, &(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 68:
/* Line 868 of glr.c  */
#line 228 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>_arg_:CHANGEABLE	arg");matchRule(1, Rule::_arg____CHANGEABLE_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 69:
/* Line 868 of glr.c  */
#line 229 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>_arg_:PENDCHANGEABLE	arg");matchRule(1, Rule::_arg____PENDCHANGEABLE_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 70:
/* Line 868 of glr.c  */
#line 230 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>_arg_:COMPATIBLE	arg");matchRule(1, Rule::_arg____COMPATIBLE_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 71:
/* Line 868 of glr.c  */
#line 231 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>_arg_:variable_specific_attribute	DOTDOT	_arg_");matchRule(1, Rule::_arg____variable_specific_attribute_DOTDOT__arg_, &(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 72:
/* Line 868 of glr.c  */
#line 232 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>_arg_:variable_composed_attribute	DOTDOT	_arg_");matchRule(1, Rule::_arg____variable_specific_attribute_DOTDOT__arg_, &(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 73:
/* Line 868 of glr.c  */
#line 233 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>_arg_:list");matchRule(0, Rule::_arg____list, &(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 74:
/* Line 868 of glr.c  */
#line 237 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression_null:NULL");matchRule(0, Rule::expression_null___NULL, NULL,NULL,NULL,NULL);}
    break;

  case 75:
/* Line 868 of glr.c  */
#line 239 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:_arg_");matchRule(0, Rule::expression____arg_,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 76:
/* Line 868 of glr.c  */
#line 240 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	MI	expression");matchRule(1, Rule::expression___expression_MI_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 77:
/* Line 868 of glr.c  */
#line 241 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:JIAN	expression");matchRule(0, Rule::expression___JIAN_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 78:
/* Line 868 of glr.c  */
#line 242 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:JIA	expression");matchRule(0, Rule::expression___JIA_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 79:
/* Line 868 of glr.c  */
#line 243 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:NOT	expression");matchRule(1, Rule::expression___NOT_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 80:
/* Line 868 of glr.c  */
#line 245 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:USER_OP2	expression");matchRule(1, Rule::expression___USER_OP2_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 81:
/* Line 868 of glr.c  */
#line 246 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	USER_OP3");matchRule(1, Rule::expression___expression_USER_OP3,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 82:
/* Line 868 of glr.c  */
#line 247 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:SI	expression");matchRule(1, Rule::expression___SI_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 83:
/* Line 868 of glr.c  */
#line 248 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:SD	expression");matchRule(1, Rule::expression___SD_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 84:
/* Line 868 of glr.c  */
#line 249 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	SI");matchRule(1, Rule::expression___expression_SI,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 85:
/* Line 868 of glr.c  */
#line 250 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	SD");matchRule(1, Rule::expression___expression_SD,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 86:
/* Line 868 of glr.c  */
#line 251 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	DOT	expression");matchRule(1, Rule::expression___expression_DOT_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 87:
/* Line 868 of glr.c  */
#line 252 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	CLEAR	LSCUR	RSCUR");matchRule(1, Rule::expression___expression_CLEAR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 88:
/* Line 868 of glr.c  */
#line 253 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	ERASE	LSCUR	listAccess	RSCUR");matchRule(1, Rule::expression___expression_ERASE_LSCUR_listAccess_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 89:
/* Line 868 of glr.c  */
#line 254 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:LIST	LSCUR	RSCUR");matchRule(1, Rule::expression___LIST_LSCUR_RSCUR,NULL,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 90:
/* Line 868 of glr.c  */
#line 255 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:MAP	LSCUR	RSCUR");matchRule(1, Rule::expression___MAP_LSCUR_RSCUR,NULL,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 91:
/* Line 868 of glr.c  */
#line 256 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:MULTIMAP	LSCUR	RSCUR");matchRule(1, Rule::expression___MULTIMAP_LSCUR_RSCUR,NULL,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 92:
/* Line 868 of glr.c  */
#line 257 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:SET	LSCUR	RSCUR");matchRule(1, Rule::expression___SET_LSCUR_RSCUR,NULL,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 93:
/* Line 868 of glr.c  */
#line 258 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:MULTISET	LSCUR	RSCUR");matchRule(1, Rule::expression___MULTISET_LSCUR_RSCUR,NULL,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 94:
/* Line 868 of glr.c  */
#line 259 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	LENGTH");matchRule(1, Rule::expression___expression_LENGTH,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 95:
/* Line 868 of glr.c  */
#line 260 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	TYPENAME");matchRule(1, Rule::expression___expression_TYPENAME,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 96:
/* Line 868 of glr.c  */
#line 261 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	TOINT	LSCUT	RSCUR");matchRule(1, Rule::expression___expression_TOINT_LSCUR_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 97:
/* Line 868 of glr.c  */
#line 262 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	TONUM	LSCUT	RSCUR");matchRule(1, Rule::expression___expression_TONUM_LSCUR_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 98:
/* Line 868 of glr.c  */
#line 263 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	TOSTRG	LSCUT	RSCUR");matchRule(1, Rule::expression___expression_TOSTRG_LSCUR_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 99:
/* Line 868 of glr.c  */
#line 264 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:NONBLOCKEXECUTE	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___NONBLOCKEXECUTE_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 100:
/* Line 868 of glr.c  */
#line 265 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:BLOCKEXECUTE	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___BLOCKEXECUTE_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 101:
/* Line 868 of glr.c  */
#line 266 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:SLEEP	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___SLEEP_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 102:
/* Line 868 of glr.c  */
#line 267 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:LOGICJUMP	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___LOGICJUMP_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 103:
/* Line 868 of glr.c  */
#line 268 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	INSERT	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_INSERT_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 104:
/* Line 868 of glr.c  */
#line 269 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	FIND	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_FIND_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 105:
/* Line 868 of glr.c  */
#line 270 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	COUNT	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_COUNT_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 106:
/* Line 868 of glr.c  */
#line 271 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:EXPR	expr_scope");matchRule(0, Rule::expression___EXPR_expr_scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 107:
/* Line 868 of glr.c  */
#line 272 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	EXIST_SUBEXPR	expr_scope");matchRule(1, Rule::expression___expression_EXIST_SUBEXPR_expr_scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 108:
/* Line 868 of glr.c  */
#line 273 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	FIND_SUBEXPR	expr_scope");matchRule(1, Rule::expression___expression_FIND_SUBEXPR_expr_scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 109:
/* Line 868 of glr.c  */
#line 274 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	RESET");matchRule(1, Rule::expression___RESET,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 110:
/* Line 868 of glr.c  */
#line 275 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	PUSHBACK	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_PUSHBACK_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 111:
/* Line 868 of glr.c  */
#line 276 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	PUSHFRONT	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_PUSHFRONT_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 112:
/* Line 868 of glr.c  */
#line 277 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	POPBACK	LSCUT	RSCUR");matchRule(1, Rule::expression___expression_POPBACK_LSCUR_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 113:
/* Line 868 of glr.c  */
#line 278 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	POPFRONT	LSCUT	RSCUR");matchRule(1, Rule::expression___expression_POPFRONT_LSCUR_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 114:
/* Line 868 of glr.c  */
#line 279 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	BACK	LSCUT	RSCUR");matchRule(1, Rule::expression___expression_BACK_LSCUR_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 115:
/* Line 868 of glr.c  */
#line 280 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	JIA	expression");matchRule(1, Rule::expression___expression_JIA_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 116:
/* Line 868 of glr.c  */
#line 281 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	JIAN	expression");matchRule(1, Rule::expression___expression_JIAN_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 117:
/* Line 868 of glr.c  */
#line 282 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	CHENG	expression");matchRule(1, Rule::expression___expression_CHENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 118:
/* Line 868 of glr.c  */
#line 283 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	CHU	expression");matchRule(1, Rule::expression___expression_CHU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 119:
/* Line 868 of glr.c  */
#line 284 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	MODULO	expression");matchRule(1, Rule::expression___expression_MODULO_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 120:
/* Line 868 of glr.c  */
#line 285 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	JIADENG	expression");matchRule(1, Rule::expression___expression_JIADENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 121:
/* Line 868 of glr.c  */
#line 286 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	JIANDENG	expression");matchRule(1, Rule::expression___expression_JIANDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 122:
/* Line 868 of glr.c  */
#line 287 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	CHENGDENG	expression");matchRule(1, Rule::expression___expression_CHENGDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 123:
/* Line 868 of glr.c  */
#line 288 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	CHUDENG	expression");matchRule(1, Rule::expression___expression_CHUDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 124:
/* Line 868 of glr.c  */
#line 289 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	MODENG	expression");matchRule(1, Rule::expression___expression_MODENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 125:
/* Line 868 of glr.c  */
#line 290 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	MIDENG	expression");matchRule(1, Rule::expression___expression_MIDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 126:
/* Line 868 of glr.c  */
#line 291 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	FUZHI	expression");matchRule(1, Rule::expression___expression_FUZHI_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 127:
/* Line 868 of glr.c  */
#line 292 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	DENGYU	expression");matchRule(1, Rule::expression___expression_DENGYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 128:
/* Line 868 of glr.c  */
#line 293 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	BUDENG	expression");matchRule(1, Rule::expression___expression_BUDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 129:
/* Line 868 of glr.c  */
#line 294 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	DAYU	expression");matchRule(1, Rule::expression___expression_DAYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 130:
/* Line 868 of glr.c  */
#line 295 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	XIAOYU	expression");matchRule(1, Rule::expression___expression_XIAOYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 131:
/* Line 868 of glr.c  */
#line 296 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	BUDAYU	expression");matchRule(1, Rule::expression___expression_BUDAYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 132:
/* Line 868 of glr.c  */
#line 297 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	BUXIAOYU	expression");matchRule(1, Rule::expression___expression_BUXIAOYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 133:
/* Line 868 of glr.c  */
#line 298 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	AND	expression");matchRule(1, Rule::expression___expression_AND_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 134:
/* Line 868 of glr.c  */
#line 299 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	OR	expression");matchRule(1, Rule::expression___expression_OR_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 135:
/* Line 868 of glr.c  */
#line 300 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	USER_OP1	expression");matchRule(1, Rule::expression___expression_USER_OP1_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 136:
/* Line 868 of glr.c  */
#line 301 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	USER_OP2	expression");matchRule(1, Rule::expression___expression_USER_OP2_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 137:
/* Line 868 of glr.c  */
#line 302 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	DOTDOT	expression");matchRule(1, Rule::expression___expression_DOTDOT_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 138:
/* Line 868 of glr.c  */
#line 303 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	COMMA	expression");matchRule(1, Rule::expression___expression_COMMA_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 139:
/* Line 868 of glr.c  */
#line 304 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	SHUCHU	expression");matchRule(1, Rule::expression___expression_SHUCHU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 140:
/* Line 868 of glr.c  */
#line 305 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:LSCUR	expression	RSCUR");matchRule(0, Rule::expression___LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 141:
/* Line 868 of glr.c  */
#line 306 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	LMCUR	listAccess	RMCUR");matchRule(1, Rule::expression___expression_LMCUR_listAccess_RMCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 142:
/* Line 868 of glr.c  */
#line 307 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	LSCUR	expression_null	RSCUR");matchRule(1, Rule::expression___expression_LSCUR_expression_null_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 143:
/* Line 868 of glr.c  */
#line 308 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expression:expression	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 144:
/* Line 868 of glr.c  */
#line 314 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:SEMICOLON");matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 145:
/* Line 868 of glr.c  */
#line 315 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:expression	SEMICOLON");matchRule(1, Rule::sentence___expression_SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 146:
/* Line 868 of glr.c  */
#line 316 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:functionDeclare	SEMICOLON");matchRule(1, Rule::sentence___functionDeclare_SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 147:
/* Line 868 of glr.c  */
#line 317 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:systemDeclare	SEMICOLON");matchRule(1, Rule::sentence___systemDeclare_SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 148:
/* Line 868 of glr.c  */
#line 318 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:functionImplement	SEMICOLON");matchRule(1, Rule::sentence___functionImplement,NULL,NULL,NULL,NULL);}
    break;

  case 149:
/* Line 868 of glr.c  */
#line 319 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:systemImplement	SEMICOLON");matchRule(1, Rule::sentence___systemImplement,NULL,NULL,NULL,NULL);}
    break;

  case 150:
/* Line 868 of glr.c  */
#line 320 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:_return_ SEMICOLON");matchRule(1, Rule::sentence____return__SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 151:
/* Line 868 of glr.c  */
#line 321 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:_return_	expression SEMICOLON");matchRule(1, Rule::sentence____return__expression_SEMICOLON,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),NULL,NULL,NULL);}
    break;

  case 152:
/* Line 868 of glr.c  */
#line 322 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:_abort_ SEMICOLON");matchRule(1, Rule::sentence____abort__SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 153:
/* Line 868 of glr.c  */
#line 323 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:branch");matchRule(1, Rule::sentence___branch,NULL,NULL,NULL,NULL);}
    break;

  case 154:
/* Line 868 of glr.c  */
#line 324 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:loop");matchRule(1, Rule::sentence___loop,NULL,NULL,NULL,NULL);}
    break;

  case 155:
/* Line 868 of glr.c  */
#line 325 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:BREAK");matchRule(1, Rule::sentence___BREAK_SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 156:
/* Line 868 of glr.c  */
#line 326 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:CONTINUE");matchRule(1, Rule::sentence___CONTINUE_SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 157:
/* Line 868 of glr.c  */
#line 327 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:_FIL	LSCUR	arg	RSCUR");matchRule(1, Rule::sentence____FIL_LSCUR_arg_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,NULL,NULL);}
    break;

  case 158:
/* Line 868 of glr.c  */
#line 328 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:_LIN	LSCUR	arg	RSCUR");matchRule(1, Rule::sentence____LIN_LSCUR_arg_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,NULL,NULL);}
    break;

  case 159:
/* Line 868 of glr.c  */
#line 329 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>sentence:_LOAD	LSCUR	arg	RSCUR");matchRule(1, Rule::sentence____LOAD_LSCUR_arg_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,NULL,NULL);}
    break;

  case 160:
/* Line 868 of glr.c  */
#line 332 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scopeBegin:LBCUR");matchRule(1, Rule::scopeBegin___LBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 161:
/* Line 868 of glr.c  */
#line 335 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scopeHalf:scopeBegin	sentence");matchRule(0, Rule::scopeHalf___scopeBegin_sentence,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 162:
/* Line 868 of glr.c  */
#line 336 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scopeHalf:scopeHalf	sentence");matchRule(0, Rule::scopeHalf___scopeHalf_sentence,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 163:
/* Line 868 of glr.c  */
#line 341 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scopeHalf:scopeHalf	scope");matchRule(0, Rule::scopeHalf___scopeHalf_scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 164:
/* Line 868 of glr.c  */
#line 344 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scope:scopeHalf	RBCUR");matchRule(1, Rule::scope___scopeHalf_RBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 165:
/* Line 868 of glr.c  */
#line 345 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scope:scopeBegin	RBCUR");matchRule(1, Rule::scope___scopeBegin_RBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 166:
/* Line 868 of glr.c  */
#line 349 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expr_scope_begin:LBCUR");matchRule(1, Rule::expr_scope_begin___LBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 167:
/* Line 868 of glr.c  */
#line 352 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expr_scope_half:expr_scope_begin	expr");matchRule(0, Rule::expr_scope_half___expr_scope_begin_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 168:
/* Line 868 of glr.c  */
#line 355 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scope:expr_scope_half	RBCUR");matchRule(1, Rule::expr_scope___expr_scope_half_RBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 169:
/* Line 868 of glr.c  */
#line 362 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:JIAN	expression");matchRule(0, Rule::expressionFunctionDeclareRoot___JIAN_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 170:
/* Line 868 of glr.c  */
#line 363 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:JIA	expression");matchRule(0, Rule::expressionFunctionDeclareRoot___JIA_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 171:
/* Line 868 of glr.c  */
#line 364 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:NOT	expression");matchRule(1, Rule::expression___NOT_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 172:
/* Line 868 of glr.c  */
#line 365 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:USER_OP1	expression");matchRule(1, Rule::expression___USER_OP1_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 173:
/* Line 868 of glr.c  */
#line 366 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:USER_OP2	expression");matchRule(1, Rule::expression___USER_OP2_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 174:
/* Line 868 of glr.c  */
#line 367 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	USER_OP3");matchRule(1, Rule::expression___expression_USER_OP3,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 175:
/* Line 868 of glr.c  */
#line 368 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:SI	expression");matchRule(1, Rule::expression___SI_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 176:
/* Line 868 of glr.c  */
#line 369 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:SD	expression");matchRule(1, Rule::expression___SD_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 177:
/* Line 868 of glr.c  */
#line 370 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	SI");matchRule(1, Rule::expression___expression_SI,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 178:
/* Line 868 of glr.c  */
#line 371 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	SD");matchRule(1, Rule::expression___expression_SD,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 179:
/* Line 868 of glr.c  */
#line 372 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	JIA	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_JIA_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 180:
/* Line 868 of glr.c  */
#line 373 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	JIAN	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_JIAN_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 181:
/* Line 868 of glr.c  */
#line 374 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	CHENG	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_CHENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 182:
/* Line 868 of glr.c  */
#line 375 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	CHU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_CHU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 183:
/* Line 868 of glr.c  */
#line 376 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	MODULO	expression");matchRule(1, Rule::expression___expression_MODULO_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 184:
/* Line 868 of glr.c  */
#line 377 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	MI	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_MI_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 185:
/* Line 868 of glr.c  */
#line 378 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	JIADENG	expression");matchRule(1, Rule::expression___expression_JIADENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 186:
/* Line 868 of glr.c  */
#line 379 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	JIANDENG	expression");matchRule(1, Rule::expression___expression_JIANDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 187:
/* Line 868 of glr.c  */
#line 380 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	CHENGDENG	expression");matchRule(1, Rule::expression___expression_CHENGDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 188:
/* Line 868 of glr.c  */
#line 381 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	CHUDENG	expression");matchRule(1, Rule::expression___expression_CHUDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 189:
/* Line 868 of glr.c  */
#line 382 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	MODENG	expression");matchRule(1, Rule::expression___expression_MODENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 190:
/* Line 868 of glr.c  */
#line 383 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	MIDENG	expression");matchRule(1, Rule::expression___expression_MIDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 191:
/* Line 868 of glr.c  */
#line 384 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	FUZHI	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_FUZHI_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 192:
/* Line 868 of glr.c  */
#line 385 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	DENGYU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_DENGYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 193:
/* Line 868 of glr.c  */
#line 386 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	BUDENG	expression");matchRule(1, Rule::expression___expression_BUDENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 194:
/* Line 868 of glr.c  */
#line 387 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	DAYU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_DAYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 195:
/* Line 868 of glr.c  */
#line 388 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	XIAOYU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_XIAOYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 196:
/* Line 868 of glr.c  */
#line 389 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	BUDAYU	expression");matchRule(1, Rule::expression___expression_BUDAYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 197:
/* Line 868 of glr.c  */
#line 390 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	BUXIAOYU	expression");matchRule(1, Rule::expression___expression_BUXIAOYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 198:
/* Line 868 of glr.c  */
#line 391 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	AND	expression");matchRule(1, Rule::expression___expression_AND_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 199:
/* Line 868 of glr.c  */
#line 392 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	OR	expression");matchRule(1, Rule::expression___expression_OR_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 200:
/* Line 868 of glr.c  */
#line 393 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	USER_OP1	expression");matchRule(1, Rule::expression___expression_USER_OP1_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 201:
/* Line 868 of glr.c  */
#line 394 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	USER_OP2	expression");matchRule(1, Rule::expression___expression_USER_OP2_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 202:
/* Line 868 of glr.c  */
#line 395 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	DOTDOT	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_DOTDOT_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 203:
/* Line 868 of glr.c  */
#line 396 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	COMMA	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_COMMA_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 204:
/* Line 868 of glr.c  */
#line 397 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	SHUCHU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_SHUCHU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 205:
/* Line 868 of glr.c  */
#line 398 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:LSCUR	expressionFunctionDeclareRoot	RSCUR");matchRule(0, Rule::expressionFunctionDeclareRoot___LSCUR_expressionFunctionDeclareRoot_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 206:
/* Line 868 of glr.c  */
#line 399 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	LSCUR	expression_null	RSCUR");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_LSCUR_expression_null_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 207:
/* Line 868 of glr.c  */
#line 400 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	LSCUR	expression	RSCUR");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 208:
/* Line 868 of glr.c  */
#line 403 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scopeFunctionDeclareBegin:LBCUR");matchRule(1, Rule::scopeFunctionDeclareBegin___LBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 209:
/* Line 868 of glr.c  */
#line 406 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scopeFunctionDeclareHalf:scopeFunctionDeclareBegin	sentence");matchRule(0, Rule::scopeFunctionDeclareHalf___scopeFunctionDeclareBegin_sentence,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 210:
/* Line 868 of glr.c  */
#line 407 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scopeFunctionDeclareHalf:scopeFunctionDeclareHalf	sentence");matchRule(0, Rule::scopeFunctionDeclareHalf___scopeFunctionDeclareHalf_sentence,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 211:
/* Line 868 of glr.c  */
#line 409 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareBegin	expressionFunctionDeclareRoot");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 212:
/* Line 868 of glr.c  */
#line 410 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareBegin	expressionFunctionDeclareRoot	SEMICOLON");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot_SRMICOLON,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 213:
/* Line 868 of glr.c  */
#line 411 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareHalf	expressionFunctionDeclareRoot");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 214:
/* Line 868 of glr.c  */
#line 412 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareHalf	expressionFunctionDeclareRoot	SEMICOLON");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot_SRMICOLON,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 215:
/* Line 868 of glr.c  */
#line 414 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>scopeFunctionDeclare:scopeFunctionDeclareEnd	RBCUR");matchRule(1, Rule::scopeFunctionDeclare___scopeFunctionDeclareEnd_RBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 216:
/* Line 868 of glr.c  */
#line 416 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>autoScopeBegin:NULL");((*yyvalp).argp)=0;matchRule(1, Rule::autoScopeBegin___NULL,NULL,NULL,NULL,&((*yyvalp).argp));}
    break;

  case 217:
/* Line 868 of glr.c  */
#line 419 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>autoScopeEnd:NULL");((*yyvalp).argp)=0;matchRule(0, Rule::autoScopeEnd___NULL,NULL,NULL,NULL,NULL);}
    break;

  case 218:
/* Line 868 of glr.c  */
#line 423 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>autoScopeHalf:autoScopeBegin	expressionFunctionDeclareRoot");matchRule(0, Rule::autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 219:
/* Line 868 of glr.c  */
#line 427 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>autoScope:autoScopeHalf	autoScopeEnd");matchRule(1, Rule::autoScope___autoScopeHalf_autoScopeEnd,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 220:
/* Line 868 of glr.c  */
#line 432 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>functionDeclareBegin:function_declare_attribute	DOTDOT	autoScope");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_autoScope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 221:
/* Line 868 of glr.c  */
#line 433 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>functionDeclareBegin:function_declare_attribute	DOTDOT	scopeFunctionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_scopeFunctionDeclare,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 222:
/* Line 868 of glr.c  */
#line 434 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>functionDeclareBegin:function_declare_attribute	scopeFunctionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_scopeFunctionDeclare,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 223:
/* Line 868 of glr.c  */
#line 435 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>functionDeclareBegin:function_declare_attribute	autoscope");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_autoscope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 224:
/* Line 868 of glr.c  */
#line 436 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>functionDeclareBegin:function_declare_attribute	functionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_functionDeclareBegin,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 225:
/* Line 868 of glr.c  */
#line 437 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>functionDeclareBegin:function_declare_attribute	DOTDOT	functionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_functionDeclareBegin,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 226:
/* Line 868 of glr.c  */
#line 439 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>functionDeclareHalf:functionDeclareBegin	JICHENG	arg");matchRule(1, Rule::functionDeclareHalf___functionDeclareBegin_JICHENG_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 227:
/* Line 868 of glr.c  */
#line 440 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>functionDeclareHalf:functionDeclareHalf	COMMA	arg");matchRule(1, Rule::functionDeclareHalf___functionDeclareBegin_JICHENG_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 228:
/* Line 868 of glr.c  */
#line 442 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>functionDeclare:functionDeclareBegin");matchRule(0, Rule::functionDeclare___functionDeclareBegin,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 229:
/* Line 868 of glr.c  */
#line 443 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>functionDeclare:functionDeclareHalf");matchRule(0, Rule::functionDeclare___functionDeclareHalf,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 230:
/* Line 868 of glr.c  */
#line 445 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>functionImplement:functionDeclare	scope");matchRule(1, Rule::functionImplement___functionDeclare_scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 231:
/* Line 868 of glr.c  */
#line 446 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>functionImplement:functionImplement	TUIDAO	functionDeclare");matchRule(1, Rule::functionImplement___functionImplement_TUIDAO_functionDeclare,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 232:
/* Line 868 of glr.c  */
#line 449 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>systemDeclareBegin:system_declare_attribute	DOTDOT	arg");matchRule(1, Rule::systemDeclareBegin___system_declare_attribute_DOTDOT_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 233:
/* Line 868 of glr.c  */
#line 451 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>systemDeclareHalf:systemDeclareBegin	JICHENG	arg");matchRule(1, Rule::systemDeclareHalf___systemDeclareBegin_JICHENG_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 234:
/* Line 868 of glr.c  */
#line 452 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>systemDeclareHalf:systemDeclareHalf	COMMA	arg");matchRule(1, Rule::systemDeclareHalf___systemDeclareBegin_JICHENG_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 235:
/* Line 868 of glr.c  */
#line 456 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>systemDeclare:systemDeclareBegin");matchRule(0, Rule::systemDeclare___systemDeclareBegin,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 236:
/* Line 868 of glr.c  */
#line 457 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>systemDeclare:systemDeclareHalf");matchRule(0, Rule::systemDeclare___systemDeclareHalf,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 237:
/* Line 868 of glr.c  */
#line 459 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>systemImplement:systemDeclare	scope");matchRule(1, Rule::systemImplement___systemDeclare_scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 238:
/* Line 868 of glr.c  */
#line 461 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>branchBegin:IF	LSCUR	expression	RSCUR	autoScope");matchRule(1, Rule::branchBegin___IF_LSCUR_expression_RSCUR_autoScope,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 239:
/* Line 868 of glr.c  */
#line 462 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>branchBegin:IF	LSCUR	expression	RSCUR	scope");matchRule(1, Rule::branchBegin___IF_LSCUR_expression_RSCUR_scope,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 240:
/* Line 868 of glr.c  */
#line 464 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>branchHalf:branchBegin	ELIF	LSCUR	expression	RSCUR	autoScope");matchRule(1, Rule::branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_autoScope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 241:
/* Line 868 of glr.c  */
#line 465 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>branchHalf:branchBegin	ELIF	LSCUR	expression	RSCUR	scope");matchRule(1, Rule::branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 242:
/* Line 868 of glr.c  */
#line 467 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>branchEnd:branchBegin	ELSE	autoScope");matchRule(1, Rule::branchEnd___branchBegin_ELSE__autoScope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 243:
/* Line 868 of glr.c  */
#line 468 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>branchEnd:branchBegin	ELSE	scope");matchRule(1,  Rule::branchEnd___branchBegin_ELSE__scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 244:
/* Line 868 of glr.c  */
#line 469 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>branchEnd:branchHalf	ELSE	autoScope");matchRule(1, Rule::branchEnd___branchHalf_ELSE__autoScope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 245:
/* Line 868 of glr.c  */
#line 470 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>branchEnd:branchHalf	ELSE	scope");matchRule(1,  Rule::branchEnd___branchHalf_ELSE__scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 246:
/* Line 868 of glr.c  */
#line 472 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>branch:branchBegin");matchRule(0, Rule::branch___branchBegin,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 247:
/* Line 868 of glr.c  */
#line 473 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>branch:branchHalf");matchRule(0,  Rule::branch___branchHalf,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 248:
/* Line 868 of glr.c  */
#line 474 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>branch:branchEnd");matchRule(0, Rule::branch___branchEnd,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 249:
/* Line 868 of glr.c  */
#line 477 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>loop:WHILE	LSCUR	expression	RSCUR	autoScope");matchRule(1, Rule::loop___WHILE_LSCUR_expression_RSCUR_autoScope,&((*yyvalp).argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.argp),NULL);}
    break;

  case 250:
/* Line 868 of glr.c  */
#line 478 "coolang_compiler_auto_integrate.y"
    {puts("yacc>>loop:WHILE	LSCUR	expression	RSCUR	scope");matchRule(1, Rule::loop___WHILE_LSCUR_expression_RSCUR_scope,&((*yyvalp).argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.argp),NULL);}
    break;


/* Line 868 of glr.c  */
#line 4970 "y.tab.c"
      default: break;
    }

  return yyok;
# undef yyerrok
# undef YYABORT
# undef YYACCEPT
# undef YYERROR
# undef YYBACKUP
# undef yyclearin
# undef YYRECOVERING
}


/*ARGSUSED*/ static void
yyuserMerge (int yyn, YYSTYPE* yy0, YYSTYPE* yy1)
{
  YYUSE (yy0);
  YYUSE (yy1);

  switch (yyn)
    {
      
      default: break;
    }
}

                              /* Bison grammar-table manipulation.  */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}

/** Number of symbols composing the right hand side of rule #RULE.  */
static inline int
yyrhsLength (yyRuleNum yyrule)
{
  return yyr2[yyrule];
}

static void
yydestroyGLRState (char const *yymsg, yyGLRState *yys)
{
  if (yys->yyresolved)
    yydestruct (yymsg, yystos[yys->yylrState],
                &yys->yysemantics.yysval);
  else
    {
#if YYDEBUG
      if (yydebug)
        {
          if (yys->yysemantics.yyfirstVal)
            YYFPRINTF (stderr, "%s unresolved ", yymsg);
          else
            YYFPRINTF (stderr, "%s incomplete ", yymsg);
          yy_symbol_print (stderr, yystos[yys->yylrState],
                           YY_NULL);
          YYFPRINTF (stderr, "\n");
        }
#endif

      if (yys->yysemantics.yyfirstVal)
        {
          yySemanticOption *yyoption = yys->yysemantics.yyfirstVal;
          yyGLRState *yyrh;
          int yyn;
          for (yyrh = yyoption->yystate, yyn = yyrhsLength (yyoption->yyrule);
               yyn > 0;
               yyrh = yyrh->yypred, yyn -= 1)
            yydestroyGLRState (yymsg, yyrh);
        }
    }
}

/** Left-hand-side symbol for rule #RULE.  */
static inline yySymbol
yylhsNonterm (yyRuleNum yyrule)
{
  return yyr1[yyrule];
}

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-283)))

/** True iff LR state STATE has only a default reduction (regardless
 *  of token).  */
static inline yybool
yyisDefaultedState (yyStateNum yystate)
{
  return yypact_value_is_default (yypact[yystate]);
}

/** The default reduction for STATE, assuming it has one.  */
static inline yyRuleNum
yydefaultAction (yyStateNum yystate)
{
  return yydefact[yystate];
}

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-208)))

/** Set *YYACTION to the action to take in YYSTATE on seeing YYTOKEN.
 *  Result R means
 *    R < 0:  Reduce on rule -R.
 *    R = 0:  Error.
 *    R > 0:  Shift to state R.
 *  Set *CONFLICTS to a pointer into yyconfl to 0-terminated list of
 *  conflicting reductions.
 */
static inline void
yygetLRActions (yyStateNum yystate, int yytoken,
                int* yyaction, const short int** yyconflicts)
{
  int yyindex = yypact[yystate] + yytoken;
  if (yypact_value_is_default (yypact[yystate])
      || yyindex < 0 || YYLAST < yyindex || yycheck[yyindex] != yytoken)
    {
      *yyaction = -yydefact[yystate];
      *yyconflicts = yyconfl;
    }
  else if (! yytable_value_is_error (yytable[yyindex]))
    {
      *yyaction = yytable[yyindex];
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
  else
    {
      *yyaction = 0;
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
}

static inline yyStateNum
yyLRgotoState (yyStateNum yystate, yySymbol yylhs)
{
  int yyr;
  yyr = yypgoto[yylhs - YYNTOKENS] + yystate;
  if (0 <= yyr && yyr <= YYLAST && yycheck[yyr] == yystate)
    return yytable[yyr];
  else
    return yydefgoto[yylhs - YYNTOKENS];
}

static inline yybool
yyisShiftAction (int yyaction)
{
  return 0 < yyaction;
}

static inline yybool
yyisErrorAction (int yyaction)
{
  return yyaction == 0;
}

                                /* GLRStates */

/** Return a fresh GLRStackItem.  Callers should call
 * YY_RESERVE_GLRSTACK afterwards to make sure there is sufficient
 * headroom.  */

static inline yyGLRStackItem*
yynewGLRStackItem (yyGLRStack* yystackp, yybool yyisState)
{
  yyGLRStackItem* yynewItem = yystackp->yynextFree;
  yystackp->yyspaceLeft -= 1;
  yystackp->yynextFree += 1;
  yynewItem->yystate.yyisState = yyisState;
  return yynewItem;
}

/** Add a new semantic action that will execute the action for rule
 *  RULENUM on the semantic values in RHS to the list of
 *  alternative actions for STATE.  Assumes that RHS comes from
 *  stack #K of *STACKP. */
static void
yyaddDeferredAction (yyGLRStack* yystackp, size_t yyk, yyGLRState* yystate,
                     yyGLRState* rhs, yyRuleNum yyrule)
{
  yySemanticOption* yynewOption =
    &yynewGLRStackItem (yystackp, yyfalse)->yyoption;
  yynewOption->yystate = rhs;
  yynewOption->yyrule = yyrule;
  if (yystackp->yytops.yylookaheadNeeds[yyk])
    {
      yynewOption->yyrawchar = yychar;
      yynewOption->yyval = yylval;
    }
  else
    yynewOption->yyrawchar = YYEMPTY;
  yynewOption->yynext = yystate->yysemantics.yyfirstVal;
  yystate->yysemantics.yyfirstVal = yynewOption;

  YY_RESERVE_GLRSTACK (yystackp);
}

                                /* GLRStacks */

/** Initialize SET to a singleton set containing an empty stack.  */
static yybool
yyinitStateSet (yyGLRStateSet* yyset)
{
  yyset->yysize = 1;
  yyset->yycapacity = 16;
  yyset->yystates = (yyGLRState**) YYMALLOC (16 * sizeof yyset->yystates[0]);
  if (! yyset->yystates)
    return yyfalse;
  yyset->yystates[0] = YY_NULL;
  yyset->yylookaheadNeeds =
    (yybool*) YYMALLOC (16 * sizeof yyset->yylookaheadNeeds[0]);
  if (! yyset->yylookaheadNeeds)
    {
      YYFREE (yyset->yystates);
      return yyfalse;
    }
  return yytrue;
}

static void yyfreeStateSet (yyGLRStateSet* yyset)
{
  YYFREE (yyset->yystates);
  YYFREE (yyset->yylookaheadNeeds);
}

/** Initialize STACK to a single empty stack, with total maximum
 *  capacity for all stacks of SIZE.  */
static yybool
yyinitGLRStack (yyGLRStack* yystackp, size_t yysize)
{
  yystackp->yyerrState = 0;
  yynerrs = 0;
  yystackp->yyspaceLeft = yysize;
  yystackp->yyitems =
    (yyGLRStackItem*) YYMALLOC (yysize * sizeof yystackp->yynextFree[0]);
  if (!yystackp->yyitems)
    return yyfalse;
  yystackp->yynextFree = yystackp->yyitems;
  yystackp->yysplitPoint = YY_NULL;
  yystackp->yylastDeleted = YY_NULL;
  return yyinitStateSet (&yystackp->yytops);
}


#if YYSTACKEXPANDABLE
# define YYRELOC(YYFROMITEMS,YYTOITEMS,YYX,YYTYPE) \
  &((YYTOITEMS) - ((YYFROMITEMS) - (yyGLRStackItem*) (YYX)))->YYTYPE

/** If STACK is expandable, extend it.  WARNING: Pointers into the
    stack from outside should be considered invalid after this call.
    We always expand when there are 1 or fewer items left AFTER an
    allocation, so that we can avoid having external pointers exist
    across an allocation.  */
static void
yyexpandGLRStack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yynewItems;
  yyGLRStackItem* yyp0, *yyp1;
  size_t yynewSize;
  size_t yyn;
  size_t yysize = yystackp->yynextFree - yystackp->yyitems;
  if (YYMAXDEPTH - YYHEADROOM < yysize)
    yyMemoryExhausted (yystackp);
  yynewSize = 2*yysize;
  if (YYMAXDEPTH < yynewSize)
    yynewSize = YYMAXDEPTH;
  yynewItems = (yyGLRStackItem*) YYMALLOC (yynewSize * sizeof yynewItems[0]);
  if (! yynewItems)
    yyMemoryExhausted (yystackp);
  for (yyp0 = yystackp->yyitems, yyp1 = yynewItems, yyn = yysize;
       0 < yyn;
       yyn -= 1, yyp0 += 1, yyp1 += 1)
    {
      *yyp1 = *yyp0;
      if (*(yybool *) yyp0)
        {
          yyGLRState* yys0 = &yyp0->yystate;
          yyGLRState* yys1 = &yyp1->yystate;
          if (yys0->yypred != YY_NULL)
            yys1->yypred =
              YYRELOC (yyp0, yyp1, yys0->yypred, yystate);
          if (! yys0->yyresolved && yys0->yysemantics.yyfirstVal != YY_NULL)
            yys1->yysemantics.yyfirstVal =
              YYRELOC (yyp0, yyp1, yys0->yysemantics.yyfirstVal, yyoption);
        }
      else
        {
          yySemanticOption* yyv0 = &yyp0->yyoption;
          yySemanticOption* yyv1 = &yyp1->yyoption;
          if (yyv0->yystate != YY_NULL)
            yyv1->yystate = YYRELOC (yyp0, yyp1, yyv0->yystate, yystate);
          if (yyv0->yynext != YY_NULL)
            yyv1->yynext = YYRELOC (yyp0, yyp1, yyv0->yynext, yyoption);
        }
    }
  if (yystackp->yysplitPoint != YY_NULL)
    yystackp->yysplitPoint = YYRELOC (yystackp->yyitems, yynewItems,
                                 yystackp->yysplitPoint, yystate);

  for (yyn = 0; yyn < yystackp->yytops.yysize; yyn += 1)
    if (yystackp->yytops.yystates[yyn] != YY_NULL)
      yystackp->yytops.yystates[yyn] =
        YYRELOC (yystackp->yyitems, yynewItems,
                 yystackp->yytops.yystates[yyn], yystate);
  YYFREE (yystackp->yyitems);
  yystackp->yyitems = yynewItems;
  yystackp->yynextFree = yynewItems + yysize;
  yystackp->yyspaceLeft = yynewSize - yysize;
}
#endif

static void
yyfreeGLRStack (yyGLRStack* yystackp)
{
  YYFREE (yystackp->yyitems);
  yyfreeStateSet (&yystackp->yytops);
}

/** Assuming that S is a GLRState somewhere on STACK, update the
 *  splitpoint of STACK, if needed, so that it is at least as deep as
 *  S.  */
static inline void
yyupdateSplit (yyGLRStack* yystackp, yyGLRState* yys)
{
  if (yystackp->yysplitPoint != YY_NULL && yystackp->yysplitPoint > yys)
    yystackp->yysplitPoint = yys;
}

/** Invalidate stack #K in STACK.  */
static inline void
yymarkStackDeleted (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yytops.yystates[yyk] != YY_NULL)
    yystackp->yylastDeleted = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yystates[yyk] = YY_NULL;
}

/** Undelete the last stack that was marked as deleted.  Can only be
    done once after a deletion, and only when all other stacks have
    been deleted.  */
static void
yyundeleteLastStack (yyGLRStack* yystackp)
{
  if (yystackp->yylastDeleted == YY_NULL || yystackp->yytops.yysize != 0)
    return;
  yystackp->yytops.yystates[0] = yystackp->yylastDeleted;
  yystackp->yytops.yysize = 1;
  YYDPRINTF ((stderr, "Restoring last deleted stack as stack #0.\n"));
  yystackp->yylastDeleted = YY_NULL;
}

static inline void
yyremoveDeletes (yyGLRStack* yystackp)
{
  size_t yyi, yyj;
  yyi = yyj = 0;
  while (yyj < yystackp->yytops.yysize)
    {
      if (yystackp->yytops.yystates[yyi] == YY_NULL)
        {
          if (yyi == yyj)
            {
              YYDPRINTF ((stderr, "Removing dead stacks.\n"));
            }
          yystackp->yytops.yysize -= 1;
        }
      else
        {
          yystackp->yytops.yystates[yyj] = yystackp->yytops.yystates[yyi];
          /* In the current implementation, it's unnecessary to copy
             yystackp->yytops.yylookaheadNeeds[yyi] since, after
             yyremoveDeletes returns, the parser immediately either enters
             deterministic operation or shifts a token.  However, it doesn't
             hurt, and the code might evolve to need it.  */
          yystackp->yytops.yylookaheadNeeds[yyj] =
            yystackp->yytops.yylookaheadNeeds[yyi];
          if (yyj != yyi)
            {
              YYDPRINTF ((stderr, "Rename stack %lu -> %lu.\n",
                          (unsigned long int) yyi, (unsigned long int) yyj));
            }
          yyj += 1;
        }
      yyi += 1;
    }
}

/** Shift to a new state on stack #K of STACK, corresponding to LR state
 * LRSTATE, at input position POSN, with (resolved) semantic value SVAL.  */
static inline void
yyglrShift (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
            size_t yyposn,
            YYSTYPE* yyvalp)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yytrue;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yysval = *yyvalp;
  yystackp->yytops.yystates[yyk] = yynewState;

  YY_RESERVE_GLRSTACK (yystackp);
}

/** Shift stack #K of YYSTACK, to a new state corresponding to LR
 *  state YYLRSTATE, at input position YYPOSN, with the (unresolved)
 *  semantic value of YYRHS under the action for YYRULE.  */
static inline void
yyglrShiftDefer (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
                 size_t yyposn, yyGLRState* rhs, yyRuleNum yyrule)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yyfalse;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yyfirstVal = YY_NULL;
  yystackp->yytops.yystates[yyk] = yynewState;

  /* Invokes YY_RESERVE_GLRSTACK.  */
  yyaddDeferredAction (yystackp, yyk, yynewState, rhs, yyrule);
}

/** Pop the symbols consumed by reduction #RULE from the top of stack
 *  #K of STACK, and perform the appropriate semantic action on their
 *  semantic values.  Assumes that all ambiguities in semantic values
 *  have been previously resolved.  Set *VALP to the resulting value,
 *  and *LOCP to the computed location (if any).  Return value is as
 *  for userAction.  */
static inline YYRESULTTAG
yydoAction (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
            YYSTYPE* yyvalp)
{
  int yynrhs = yyrhsLength (yyrule);

  if (yystackp->yysplitPoint == YY_NULL)
    {
      /* Standard special case: single stack.  */
      yyGLRStackItem* rhs = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
      YYASSERT (yyk == 0);
      yystackp->yynextFree -= yynrhs;
      yystackp->yyspaceLeft += yynrhs;
      yystackp->yytops.yystates[0] = & yystackp->yynextFree[-1].yystate;
      return yyuserAction (yyrule, yynrhs, rhs, yystackp,
                           yyvalp);
    }
  else
    {
      /* At present, doAction is never called in nondeterministic
       * mode, so this branch is never taken.  It is here in
       * anticipation of a future feature that will allow immediate
       * evaluation of selected actions in nondeterministic mode.  */
      int yyi;
      yyGLRState* yys;
      yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
      yys = yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred
        = yystackp->yytops.yystates[yyk];
      for (yyi = 0; yyi < yynrhs; yyi += 1)
        {
          yys = yys->yypred;
          YYASSERT (yys);
        }
      yyupdateSplit (yystackp, yys);
      yystackp->yytops.yystates[yyk] = yys;
      return yyuserAction (yyrule, yynrhs, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
                           yystackp, yyvalp);
    }
}

#if !YYDEBUG
# define YY_REDUCE_PRINT(Args)
#else
# define YY_REDUCE_PRINT(Args)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print Args;               \
} while (YYID (0))

/*----------------------------------------------------------.
| Report that the RULE is going to be reduced on stack #K.  |
`----------------------------------------------------------*/

/*ARGSUSED*/ static inline void
yy_reduce_print (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
                 YYSTYPE* yyvalp)
{
  int yynrhs = yyrhsLength (yyrule);
  yybool yynormal __attribute__ ((__unused__)) =
    (yystackp->yysplitPoint == YY_NULL);
  yyGLRStackItem* yyvsp = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
  int yylow = 1;
  int yyi;
  YYUSE (yyvalp);
  YYFPRINTF (stderr, "Reducing stack %lu by rule %d (line %lu):\n",
             (unsigned long int) yyk, yyrule - 1,
             (unsigned long int) yyrline[yyrule]);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
                       &(((yyGLRStackItem const *)yyvsp)[YYFILL ((yyi + 1) - (yynrhs))].yystate.yysemantics.yysval)
                                              );
      YYFPRINTF (stderr, "\n");
    }
}
#endif

/** Pop items off stack #K of STACK according to grammar rule RULE,
 *  and push back on the resulting nonterminal symbol.  Perform the
 *  semantic action associated with RULE and store its value with the
 *  newly pushed state, if FORCEEVAL or if STACK is currently
 *  unambiguous.  Otherwise, store the deferred semantic action with
 *  the new state.  If the new state would have an identical input
 *  position, LR state, and predecessor to an existing state on the stack,
 *  it is identified with that existing state, eliminating stack #K from
 *  the STACK.  In this case, the (necessarily deferred) semantic value is
 *  added to the options for the existing state's semantic value.
 */
static inline YYRESULTTAG
yyglrReduce (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
             yybool yyforceEval)
{
  size_t yyposn = yystackp->yytops.yystates[yyk]->yyposn;

  if (yyforceEval || yystackp->yysplitPoint == YY_NULL)
    {
      YYSTYPE yysval;

      YY_REDUCE_PRINT ((yystackp, yyk, yyrule, &yysval));
      YYCHK (yydoAction (yystackp, yyk, yyrule, &yysval));
      YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyrule], &yysval, &yyloc);
      yyglrShift (yystackp, yyk,
                  yyLRgotoState (yystackp->yytops.yystates[yyk]->yylrState,
                                 yylhsNonterm (yyrule)),
                  yyposn, &yysval);
    }
  else
    {
      size_t yyi;
      int yyn;
      yyGLRState* yys, *yys0 = yystackp->yytops.yystates[yyk];
      yyStateNum yynewLRState;

      for (yys = yystackp->yytops.yystates[yyk], yyn = yyrhsLength (yyrule);
           0 < yyn; yyn -= 1)
        {
          yys = yys->yypred;
          YYASSERT (yys);
        }
      yyupdateSplit (yystackp, yys);
      yynewLRState = yyLRgotoState (yys->yylrState, yylhsNonterm (yyrule));
      YYDPRINTF ((stderr,
                  "Reduced stack %lu by rule #%d; action deferred.  Now in state %d.\n",
                  (unsigned long int) yyk, yyrule - 1, yynewLRState));
      for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
        if (yyi != yyk && yystackp->yytops.yystates[yyi] != YY_NULL)
          {
            yyGLRState *yysplit = yystackp->yysplitPoint;
            yyGLRState *yyp = yystackp->yytops.yystates[yyi];
            while (yyp != yys && yyp != yysplit && yyp->yyposn >= yyposn)
              {
                if (yyp->yylrState == yynewLRState && yyp->yypred == yys)
                  {
                    yyaddDeferredAction (yystackp, yyk, yyp, yys0, yyrule);
                    yymarkStackDeleted (yystackp, yyk);
                    YYDPRINTF ((stderr, "Merging stack %lu into stack %lu.\n",
                                (unsigned long int) yyk,
                                (unsigned long int) yyi));
                    return yyok;
                  }
                yyp = yyp->yypred;
              }
          }
      yystackp->yytops.yystates[yyk] = yys;
      yyglrShiftDefer (yystackp, yyk, yynewLRState, yyposn, yys0, yyrule);
    }
  return yyok;
}

static size_t
yysplitStack (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yysplitPoint == YY_NULL)
    {
      YYASSERT (yyk == 0);
      yystackp->yysplitPoint = yystackp->yytops.yystates[yyk];
    }
  if (yystackp->yytops.yysize >= yystackp->yytops.yycapacity)
    {
      yyGLRState** yynewStates;
      yybool* yynewLookaheadNeeds;

      yynewStates = YY_NULL;

      if (yystackp->yytops.yycapacity
          > (YYSIZEMAX / (2 * sizeof yynewStates[0])))
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yycapacity *= 2;

      yynewStates =
        (yyGLRState**) YYREALLOC (yystackp->yytops.yystates,
                                  (yystackp->yytops.yycapacity
                                   * sizeof yynewStates[0]));
      if (yynewStates == YY_NULL)
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yystates = yynewStates;

      yynewLookaheadNeeds =
        (yybool*) YYREALLOC (yystackp->yytops.yylookaheadNeeds,
                             (yystackp->yytops.yycapacity
                              * sizeof yynewLookaheadNeeds[0]));
      if (yynewLookaheadNeeds == YY_NULL)
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yylookaheadNeeds = yynewLookaheadNeeds;
    }
  yystackp->yytops.yystates[yystackp->yytops.yysize]
    = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yylookaheadNeeds[yystackp->yytops.yysize]
    = yystackp->yytops.yylookaheadNeeds[yyk];
  yystackp->yytops.yysize += 1;
  return yystackp->yytops.yysize-1;
}

/** True iff Y0 and Y1 represent identical options at the top level.
 *  That is, they represent the same rule applied to RHS symbols
 *  that produce the same terminal symbols.  */
static yybool
yyidenticalOptions (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  if (yyy0->yyrule == yyy1->yyrule)
    {
      yyGLRState *yys0, *yys1;
      int yyn;
      for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
           yyn = yyrhsLength (yyy0->yyrule);
           yyn > 0;
           yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
        if (yys0->yyposn != yys1->yyposn)
          return yyfalse;
      return yytrue;
    }
  else
    return yyfalse;
}

/** Assuming identicalOptions (Y0,Y1), destructively merge the
 *  alternative semantic values for the RHS-symbols of Y1 and Y0.  */
static void
yymergeOptionSets (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  yyGLRState *yys0, *yys1;
  int yyn;
  for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
       yyn = yyrhsLength (yyy0->yyrule);
       yyn > 0;
       yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
    {
      if (yys0 == yys1)
        break;
      else if (yys0->yyresolved)
        {
          yys1->yyresolved = yytrue;
          yys1->yysemantics.yysval = yys0->yysemantics.yysval;
        }
      else if (yys1->yyresolved)
        {
          yys0->yyresolved = yytrue;
          yys0->yysemantics.yysval = yys1->yysemantics.yysval;
        }
      else
        {
          yySemanticOption** yyz0p = &yys0->yysemantics.yyfirstVal;
          yySemanticOption* yyz1 = yys1->yysemantics.yyfirstVal;
          while (YYID (yytrue))
            {
              if (yyz1 == *yyz0p || yyz1 == YY_NULL)
                break;
              else if (*yyz0p == YY_NULL)
                {
                  *yyz0p = yyz1;
                  break;
                }
              else if (*yyz0p < yyz1)
                {
                  yySemanticOption* yyz = *yyz0p;
                  *yyz0p = yyz1;
                  yyz1 = yyz1->yynext;
                  (*yyz0p)->yynext = yyz;
                }
              yyz0p = &(*yyz0p)->yynext;
            }
          yys1->yysemantics.yyfirstVal = yys0->yysemantics.yyfirstVal;
        }
    }
}

/** Y0 and Y1 represent two possible actions to take in a given
 *  parsing state; return 0 if no combination is possible,
 *  1 if user-mergeable, 2 if Y0 is preferred, 3 if Y1 is preferred.  */
static int
yypreference (yySemanticOption* y0, yySemanticOption* y1)
{
  yyRuleNum r0 = y0->yyrule, r1 = y1->yyrule;
  int p0 = yydprec[r0], p1 = yydprec[r1];

  if (p0 == p1)
    {
      if (yymerger[r0] == 0 || yymerger[r0] != yymerger[r1])
        return 0;
      else
        return 1;
    }
  if (p0 == 0 || p1 == 0)
    return 0;
  if (p0 < p1)
    return 3;
  if (p1 < p0)
    return 2;
  return 0;
}

static YYRESULTTAG yyresolveValue (yyGLRState* yys,
                                   yyGLRStack* yystackp);


/** Resolve the previous N states starting at and including state S.  If result
 *  != yyok, some states may have been left unresolved possibly with empty
 *  semantic option chains.  Regardless of whether result = yyok, each state
 *  has been left with consistent data so that yydestroyGLRState can be invoked
 *  if necessary.  */
static YYRESULTTAG
yyresolveStates (yyGLRState* yys, int yyn,
                 yyGLRStack* yystackp)
{
  if (0 < yyn)
    {
      YYASSERT (yys->yypred);
      YYCHK (yyresolveStates (yys->yypred, yyn-1, yystackp));
      if (! yys->yyresolved)
        YYCHK (yyresolveValue (yys, yystackp));
    }
  return yyok;
}

/** Resolve the states for the RHS of OPT, perform its user action, and return
 *  the semantic value and location.  Regardless of whether result = yyok, all
 *  RHS states have been destroyed (assuming the user action destroys all RHS
 *  semantic values if invoked).  */
static YYRESULTTAG
yyresolveAction (yySemanticOption* yyopt, yyGLRStack* yystackp,
                 YYSTYPE* yyvalp)
{
  yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
  int yynrhs = yyrhsLength (yyopt->yyrule);
  YYRESULTTAG yyflag =
    yyresolveStates (yyopt->yystate, yynrhs, yystackp);
  if (yyflag != yyok)
    {
      yyGLRState *yys;
      for (yys = yyopt->yystate; yynrhs > 0; yys = yys->yypred, yynrhs -= 1)
        yydestroyGLRState ("Cleanup: popping", yys);
      return yyflag;
    }

  yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred = yyopt->yystate;
  {
    int yychar_current = yychar;
    YYSTYPE yylval_current = yylval;
    yychar = yyopt->yyrawchar;
    yylval = yyopt->yyval;
    yyflag = yyuserAction (yyopt->yyrule, yynrhs,
                           yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
                           yystackp, yyvalp);
    yychar = yychar_current;
    yylval = yylval_current;
  }
  return yyflag;
}

#if YYDEBUG
static void
yyreportTree (yySemanticOption* yyx, int yyindent)
{
  int yynrhs = yyrhsLength (yyx->yyrule);
  int yyi;
  yyGLRState* yys;
  yyGLRState* yystates[1 + YYMAXRHS];
  yyGLRState yyleftmost_state;

  for (yyi = yynrhs, yys = yyx->yystate; 0 < yyi; yyi -= 1, yys = yys->yypred)
    yystates[yyi] = yys;
  if (yys == YY_NULL)
    {
      yyleftmost_state.yyposn = 0;
      yystates[0] = &yyleftmost_state;
    }
  else
    yystates[0] = yys;

  if (yyx->yystate->yyposn < yys->yyposn + 1)
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, empty>\n",
               yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
               yyx->yyrule - 1);
  else
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, tokens %lu .. %lu>\n",
               yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
               yyx->yyrule - 1, (unsigned long int) (yys->yyposn + 1),
               (unsigned long int) yyx->yystate->yyposn);
  for (yyi = 1; yyi <= yynrhs; yyi += 1)
    {
      if (yystates[yyi]->yyresolved)
        {
          if (yystates[yyi-1]->yyposn+1 > yystates[yyi]->yyposn)
            YYFPRINTF (stderr, "%*s%s <empty>\n", yyindent+2, "",
                       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]));
          else
            YYFPRINTF (stderr, "%*s%s <tokens %lu .. %lu>\n", yyindent+2, "",
                       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]),
                       (unsigned long int) (yystates[yyi - 1]->yyposn + 1),
                       (unsigned long int) yystates[yyi]->yyposn);
        }
      else
        yyreportTree (yystates[yyi]->yysemantics.yyfirstVal, yyindent+2);
    }
}
#endif

/*ARGSUSED*/ static YYRESULTTAG
yyreportAmbiguity (yySemanticOption* yyx0,
                   yySemanticOption* yyx1)
{
  YYUSE (yyx0);
  YYUSE (yyx1);

#if YYDEBUG
  YYFPRINTF (stderr, "Ambiguity detected.\n");
  YYFPRINTF (stderr, "Option 1,\n");
  yyreportTree (yyx0, 2);
  YYFPRINTF (stderr, "\nOption 2,\n");
  yyreportTree (yyx1, 2);
  YYFPRINTF (stderr, "\n");
#endif

  yyerror (YY_("syntax is ambiguous"));
  return yyabort;
}

/** Resolve the ambiguity represented in state S, perform the indicated
 *  actions, and set the semantic value of S.  If result != yyok, the chain of
 *  semantic options in S has been cleared instead or it has been left
 *  unmodified except that redundant options may have been removed.  Regardless
 *  of whether result = yyok, S has been left with consistent data so that
 *  yydestroyGLRState can be invoked if necessary.  */
static YYRESULTTAG
yyresolveValue (yyGLRState* yys, yyGLRStack* yystackp)
{
  yySemanticOption* yyoptionList = yys->yysemantics.yyfirstVal;
  yySemanticOption* yybest = yyoptionList;
  yySemanticOption** yypp;
  yybool yymerge = yyfalse;
  YYSTYPE yysval;
  YYRESULTTAG yyflag;

  for (yypp = &yyoptionList->yynext; *yypp != YY_NULL; )
    {
      yySemanticOption* yyp = *yypp;

      if (yyidenticalOptions (yybest, yyp))
        {
          yymergeOptionSets (yybest, yyp);
          *yypp = yyp->yynext;
        }
      else
        {
          switch (yypreference (yybest, yyp))
            {
            case 0:
              return yyreportAmbiguity (yybest, yyp);
              break;
            case 1:
              yymerge = yytrue;
              break;
            case 2:
              break;
            case 3:
              yybest = yyp;
              yymerge = yyfalse;
              break;
            default:
              /* This cannot happen so it is not worth a YYASSERT (yyfalse),
                 but some compilers complain if the default case is
                 omitted.  */
              break;
            }
          yypp = &yyp->yynext;
        }
    }

  if (yymerge)
    {
      yySemanticOption* yyp;
      int yyprec = yydprec[yybest->yyrule];
      yyflag = yyresolveAction (yybest, yystackp, &yysval);
      if (yyflag == yyok)
        for (yyp = yybest->yynext; yyp != YY_NULL; yyp = yyp->yynext)
          {
            if (yyprec == yydprec[yyp->yyrule])
              {
                YYSTYPE yysval_other;
                yyflag = yyresolveAction (yyp, yystackp, &yysval_other);
                if (yyflag != yyok)
                  {
                    yydestruct ("Cleanup: discarding incompletely merged value for",
                                yystos[yys->yylrState],
                                &yysval);
                    break;
                  }
                yyuserMerge (yymerger[yyp->yyrule], &yysval, &yysval_other);
              }
          }
    }
  else
    yyflag = yyresolveAction (yybest, yystackp, &yysval);

  if (yyflag == yyok)
    {
      yys->yyresolved = yytrue;
      yys->yysemantics.yysval = yysval;
    }
  else
    yys->yysemantics.yyfirstVal = YY_NULL;
  return yyflag;
}

static YYRESULTTAG
yyresolveStack (yyGLRStack* yystackp)
{
  if (yystackp->yysplitPoint != YY_NULL)
    {
      yyGLRState* yys;
      int yyn;

      for (yyn = 0, yys = yystackp->yytops.yystates[0];
           yys != yystackp->yysplitPoint;
           yys = yys->yypred, yyn += 1)
        continue;
      YYCHK (yyresolveStates (yystackp->yytops.yystates[0], yyn, yystackp
                             ));
    }
  return yyok;
}

static void
yycompressStack (yyGLRStack* yystackp)
{
  yyGLRState* yyp, *yyq, *yyr;

  if (yystackp->yytops.yysize != 1 || yystackp->yysplitPoint == YY_NULL)
    return;

  for (yyp = yystackp->yytops.yystates[0], yyq = yyp->yypred, yyr = YY_NULL;
       yyp != yystackp->yysplitPoint;
       yyr = yyp, yyp = yyq, yyq = yyp->yypred)
    yyp->yypred = yyr;

  yystackp->yyspaceLeft += yystackp->yynextFree - yystackp->yyitems;
  yystackp->yynextFree = ((yyGLRStackItem*) yystackp->yysplitPoint) + 1;
  yystackp->yyspaceLeft -= yystackp->yynextFree - yystackp->yyitems;
  yystackp->yysplitPoint = YY_NULL;
  yystackp->yylastDeleted = YY_NULL;

  while (yyr != YY_NULL)
    {
      yystackp->yynextFree->yystate = *yyr;
      yyr = yyr->yypred;
      yystackp->yynextFree->yystate.yypred = &yystackp->yynextFree[-1].yystate;
      yystackp->yytops.yystates[0] = &yystackp->yynextFree->yystate;
      yystackp->yynextFree += 1;
      yystackp->yyspaceLeft -= 1;
    }
}

static YYRESULTTAG
yyprocessOneStack (yyGLRStack* yystackp, size_t yyk,
                   size_t yyposn)
{
  int yyaction;
  const short int* yyconflicts;
  yyRuleNum yyrule;

  while (yystackp->yytops.yystates[yyk] != YY_NULL)
    {
      yyStateNum yystate = yystackp->yytops.yystates[yyk]->yylrState;
      YYDPRINTF ((stderr, "Stack %lu Entering state %d\n",
                  (unsigned long int) yyk, yystate));

      YYASSERT (yystate != YYFINAL);

      if (yyisDefaultedState (yystate))
        {
          yyrule = yydefaultAction (yystate);
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, "Stack %lu dies.\n",
                          (unsigned long int) yyk));
              yymarkStackDeleted (yystackp, yyk);
              return yyok;
            }
          YYCHK (yyglrReduce (yystackp, yyk, yyrule, yyfalse));
        }
      else
        {
          yySymbol yytoken;
          yystackp->yytops.yylookaheadNeeds[yyk] = yytrue;
          if (yychar == YYEMPTY)
            {
              YYDPRINTF ((stderr, "Reading a token: "));
              yychar = YYLEX;
            }

          if (yychar <= YYEOF)
            {
              yychar = yytoken = YYEOF;
              YYDPRINTF ((stderr, "Now at end of input.\n"));
            }
          else
            {
              yytoken = YYTRANSLATE (yychar);
              YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
            }

          yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);

          while (*yyconflicts != 0)
            {
              size_t yynewStack = yysplitStack (yystackp, yyk);
              YYDPRINTF ((stderr, "Splitting off stack %lu from %lu.\n",
                          (unsigned long int) yynewStack,
                          (unsigned long int) yyk));
              YYCHK (yyglrReduce (yystackp, yynewStack,
                                  *yyconflicts, yyfalse));
              YYCHK (yyprocessOneStack (yystackp, yynewStack,
                                        yyposn));
              yyconflicts += 1;
            }

          if (yyisShiftAction (yyaction))
            break;
          else if (yyisErrorAction (yyaction))
            {
              YYDPRINTF ((stderr, "Stack %lu dies.\n",
                          (unsigned long int) yyk));
              yymarkStackDeleted (yystackp, yyk);
              break;
            }
          else
            YYCHK (yyglrReduce (yystackp, yyk, -yyaction,
                                yyfalse));
        }
    }
  return yyok;
}

/*ARGSUSED*/ static void
yyreportSyntaxError (yyGLRStack* yystackp)
{
  if (yystackp->yyerrState != 0)
    return;
#if ! YYERROR_VERBOSE
  yyerror (YY_("syntax error"));
#else
  {
  yySymbol yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);
  size_t yysize0 = yytnamerr (YY_NULL, yytokenName (yytoken));
  size_t yysize = yysize0;
  yybool yysize_overflow = yyfalse;
  char* yymsg = YY_NULL;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected").  */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[yystackp->yytops.yystates[0]->yylrState];
      yyarg[yycount++] = yytokenName (yytoken);
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for this
             state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;
          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytokenName (yyx);
                {
                  size_t yysz = yysize + yytnamerr (YY_NULL, yytokenName (yyx));
                  yysize_overflow |= yysz < yysize;
                  yysize = yysz;
                }
              }
        }
    }

  switch (yycount)
    {
#define YYCASE_(N, S)                   \
      case N:                           \
        yyformat = S;                   \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  {
    size_t yysz = yysize + strlen (yyformat);
    yysize_overflow |= yysz < yysize;
    yysize = yysz;
  }

  if (!yysize_overflow)
    yymsg = (char *) YYMALLOC (yysize);

  if (yymsg)
    {
      char *yyp = yymsg;
      int yyi = 0;
      while ((*yyp = *yyformat))
        {
          if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
            {
              yyp += yytnamerr (yyp, yyarg[yyi++]);
              yyformat += 2;
            }
          else
            {
              yyp++;
              yyformat++;
            }
        }
      yyerror (yymsg);
      YYFREE (yymsg);
    }
  else
    {
      yyerror (YY_("syntax error"));
      yyMemoryExhausted (yystackp);
    }
  }
#endif /* YYERROR_VERBOSE */
  yynerrs += 1;
}

/* Recover from a syntax error on *YYSTACKP, assuming that *YYSTACKP->YYTOKENP,
   yylval, and yylloc are the syntactic category, semantic value, and location
   of the lookahead.  */
/*ARGSUSED*/ static void
yyrecoverSyntaxError (yyGLRStack* yystackp)
{
  size_t yyk;
  int yyj;

  if (yystackp->yyerrState == 3)
    /* We just shifted the error token and (perhaps) took some
       reductions.  Skip tokens until we can proceed.  */
    while (YYID (yytrue))
      {
        yySymbol yytoken;
        if (yychar == YYEOF)
          yyFail (yystackp, YY_NULL);
        if (yychar != YYEMPTY)
          {
            yytoken = YYTRANSLATE (yychar);
            yydestruct ("Error: discarding",
                        yytoken, &yylval);
          }
        YYDPRINTF ((stderr, "Reading a token: "));
        yychar = YYLEX;
        if (yychar <= YYEOF)
          {
            yychar = yytoken = YYEOF;
            YYDPRINTF ((stderr, "Now at end of input.\n"));
          }
        else
          {
            yytoken = YYTRANSLATE (yychar);
            YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
          }
        yyj = yypact[yystackp->yytops.yystates[0]->yylrState];
        if (yypact_value_is_default (yyj))
          return;
        yyj += yytoken;
        if (yyj < 0 || YYLAST < yyj || yycheck[yyj] != yytoken)
          {
            if (yydefact[yystackp->yytops.yystates[0]->yylrState] != 0)
              return;
          }
        else if (! yytable_value_is_error (yytable[yyj]))
          return;
      }

  /* Reduce to one stack.  */
  for (yyk = 0; yyk < yystackp->yytops.yysize; yyk += 1)
    if (yystackp->yytops.yystates[yyk] != YY_NULL)
      break;
  if (yyk >= yystackp->yytops.yysize)
    yyFail (yystackp, YY_NULL);
  for (yyk += 1; yyk < yystackp->yytops.yysize; yyk += 1)
    yymarkStackDeleted (yystackp, yyk);
  yyremoveDeletes (yystackp);
  yycompressStack (yystackp);

  /* Now pop stack until we find a state that shifts the error token.  */
  yystackp->yyerrState = 3;
  while (yystackp->yytops.yystates[0] != YY_NULL)
    {
      yyGLRState *yys = yystackp->yytops.yystates[0];
      yyj = yypact[yys->yylrState];
      if (! yypact_value_is_default (yyj))
        {
          yyj += YYTERROR;
          if (0 <= yyj && yyj <= YYLAST && yycheck[yyj] == YYTERROR
              && yyisShiftAction (yytable[yyj]))
            {
              /* Shift the error token.  */
              YY_SYMBOL_PRINT ("Shifting", yystos[yytable[yyj]],
                               &yylval, &yyerrloc);
              yyglrShift (yystackp, 0, yytable[yyj],
                          yys->yyposn, &yylval);
              yys = yystackp->yytops.yystates[0];
              break;
            }
        }
      if (yys->yypred != YY_NULL)
        yydestroyGLRState ("Error: popping", yys);
      yystackp->yytops.yystates[0] = yys->yypred;
      yystackp->yynextFree -= 1;
      yystackp->yyspaceLeft += 1;
    }
  if (yystackp->yytops.yystates[0] == YY_NULL)
    yyFail (yystackp, YY_NULL);
}

#define YYCHK1(YYE)                                                          \
  do {                                                                       \
    switch (YYE) {                                                           \
    case yyok:                                                               \
      break;                                                                 \
    case yyabort:                                                            \
      goto yyabortlab;                                                       \
    case yyaccept:                                                           \
      goto yyacceptlab;                                                      \
    case yyerr:                                                              \
      goto yyuser_error;                                                     \
    default:                                                                 \
      goto yybuglab;                                                         \
    }                                                                        \
  } while (YYID (0))


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
  int yyresult;
  yyGLRStack yystack;
  yyGLRStack* const yystackp = &yystack;
  size_t yyposn;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY;
  yylval = yyval_default;

  if (! yyinitGLRStack (yystackp, YYINITDEPTH))
    goto yyexhaustedlab;
  switch (YYSETJMP (yystack.yyexception_buffer))
    {
    case 0: break;
    case 1: goto yyabortlab;
    case 2: goto yyexhaustedlab;
    default: goto yybuglab;
    }
  yyglrShift (&yystack, 0, 0, 0, &yylval);
  yyposn = 0;

  while (YYID (yytrue))
    {
      /* For efficiency, we have two loops, the first of which is
         specialized to deterministic operation (single stack, no
         potential ambiguity).  */
      /* Standard mode */
      while (YYID (yytrue))
        {
          yyRuleNum yyrule;
          int yyaction;
          const short int* yyconflicts;

          yyStateNum yystate = yystack.yytops.yystates[0]->yylrState;
          YYDPRINTF ((stderr, "Entering state %d\n", yystate));
          if (yystate == YYFINAL)
            goto yyacceptlab;
          if (yyisDefaultedState (yystate))
            {
              yyrule = yydefaultAction (yystate);
              if (yyrule == 0)
                {

                  yyreportSyntaxError (&yystack);
                  goto yyuser_error;
                }
              YYCHK1 (yyglrReduce (&yystack, 0, yyrule, yytrue));
            }
          else
            {
              yySymbol yytoken;
              if (yychar == YYEMPTY)
                {
                  YYDPRINTF ((stderr, "Reading a token: "));
                  yychar = YYLEX;
                }

              if (yychar <= YYEOF)
                {
                  yychar = yytoken = YYEOF;
                  YYDPRINTF ((stderr, "Now at end of input.\n"));
                }
              else
                {
                  yytoken = YYTRANSLATE (yychar);
                  YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
                }

              yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);
              if (*yyconflicts != 0)
                break;
              if (yyisShiftAction (yyaction))
                {
                  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
                  yychar = YYEMPTY;
                  yyposn += 1;
                  yyglrShift (&yystack, 0, yyaction, yyposn, &yylval);
                  if (0 < yystack.yyerrState)
                    yystack.yyerrState -= 1;
                }
              else if (yyisErrorAction (yyaction))
                {

                  yyreportSyntaxError (&yystack);
                  goto yyuser_error;
                }
              else
                YYCHK1 (yyglrReduce (&yystack, 0, -yyaction, yytrue));
            }
        }

      while (YYID (yytrue))
        {
          yySymbol yytoken_to_shift;
          size_t yys;

          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            yystackp->yytops.yylookaheadNeeds[yys] = yychar != YYEMPTY;

          /* yyprocessOneStack returns one of three things:

              - An error flag.  If the caller is yyprocessOneStack, it
                immediately returns as well.  When the caller is finally
                yyparse, it jumps to an error label via YYCHK1.

              - yyok, but yyprocessOneStack has invoked yymarkStackDeleted
                (&yystack, yys), which sets the top state of yys to NULL.  Thus,
                yyparse's following invocation of yyremoveDeletes will remove
                the stack.

              - yyok, when ready to shift a token.

             Except in the first case, yyparse will invoke yyremoveDeletes and
             then shift the next token onto all remaining stacks.  This
             synchronization of the shift (that is, after all preceding
             reductions on all stacks) helps prevent double destructor calls
             on yylval in the event of memory exhaustion.  */

          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            YYCHK1 (yyprocessOneStack (&yystack, yys, yyposn));
          yyremoveDeletes (&yystack);
          if (yystack.yytops.yysize == 0)
            {
              yyundeleteLastStack (&yystack);
              if (yystack.yytops.yysize == 0)
                yyFail (&yystack, YY_("syntax error"));
              YYCHK1 (yyresolveStack (&yystack));
              YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));

              yyreportSyntaxError (&yystack);
              goto yyuser_error;
            }

          /* If any yyglrShift call fails, it will fail after shifting.  Thus,
             a copy of yylval will already be on stack 0 in the event of a
             failure in the following loop.  Thus, yychar is set to YYEMPTY
             before the loop to make sure the user destructor for yylval isn't
             called twice.  */
          yytoken_to_shift = YYTRANSLATE (yychar);
          yychar = YYEMPTY;
          yyposn += 1;
          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            {
              int yyaction;
              const short int* yyconflicts;
              yyStateNum yystate = yystack.yytops.yystates[yys]->yylrState;
              yygetLRActions (yystate, yytoken_to_shift, &yyaction,
                              &yyconflicts);
              /* Note that yyconflicts were handled by yyprocessOneStack.  */
              YYDPRINTF ((stderr, "On stack %lu, ", (unsigned long int) yys));
              YY_SYMBOL_PRINT ("shifting", yytoken_to_shift, &yylval, &yylloc);
              yyglrShift (&yystack, yys, yyaction, yyposn,
                          &yylval);
              YYDPRINTF ((stderr, "Stack %lu now in state #%d\n",
                          (unsigned long int) yys,
                          yystack.yytops.yystates[yys]->yylrState));
            }

          if (yystack.yytops.yysize == 1)
            {
              YYCHK1 (yyresolveStack (&yystack));
              YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
              yycompressStack (&yystack);
              break;
            }
        }
      continue;
    yyuser_error:
      yyrecoverSyntaxError (&yystack);
      yyposn = yystack.yytops.yystates[0]->yyposn;
    }

 yyacceptlab:
  yyresult = 0;
  goto yyreturn;

 yybuglab:
  YYASSERT (yyfalse);
  goto yyabortlab;

 yyabortlab:
  yyresult = 1;
  goto yyreturn;

 yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;

 yyreturn:
  if (yychar != YYEMPTY)
    yydestruct ("Cleanup: discarding lookahead",
                YYTRANSLATE (yychar), &yylval);

  /* If the stack is well-formed, pop the stack until it is empty,
     destroying its entries as we go.  But free the stack regardless
     of whether it is well-formed.  */
  if (yystack.yyitems)
    {
      yyGLRState** yystates = yystack.yytops.yystates;
      if (yystates)
        {
          size_t yysize = yystack.yytops.yysize;
          size_t yyk;
          for (yyk = 0; yyk < yysize; yyk += 1)
            if (yystates[yyk])
              {
                while (yystates[yyk])
                  {
                    yyGLRState *yys = yystates[yyk];
                  if (yys->yypred != YY_NULL)
                      yydestroyGLRState ("Cleanup: popping", yys);
                    yystates[yyk] = yys->yypred;
                    yystack.yynextFree -= 1;
                    yystack.yyspaceLeft += 1;
                  }
                break;
              }
        }
      yyfreeGLRStack (&yystack);
    }

  /* Make sure YYID is used.  */
  return YYID (yyresult);
}

/* DEBUGGING ONLY */
#if YYDEBUG
static void yypstack (yyGLRStack* yystackp, size_t yyk)
  __attribute__ ((__unused__));
static void yypdumpstack (yyGLRStack* yystackp) __attribute__ ((__unused__));

static void
yy_yypstack (yyGLRState* yys)
{
  if (yys->yypred)
    {
      yy_yypstack (yys->yypred);
      YYFPRINTF (stderr, " -> ");
    }
  YYFPRINTF (stderr, "%d@%lu", yys->yylrState,
             (unsigned long int) yys->yyposn);
}

static void
yypstates (yyGLRState* yyst)
{
  if (yyst == YY_NULL)
    YYFPRINTF (stderr, "<null>");
  else
    yy_yypstack (yyst);
  YYFPRINTF (stderr, "\n");
}

static void
yypstack (yyGLRStack* yystackp, size_t yyk)
{
  yypstates (yystackp->yytops.yystates[yyk]);
}

#define YYINDEX(YYX)                                                         \
    ((YYX) == YY_NULL ? -1 : (yyGLRStackItem*) (YYX) - yystackp->yyitems)


static void
yypdumpstack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yyp;
  size_t yyi;
  for (yyp = yystackp->yyitems; yyp < yystackp->yynextFree; yyp += 1)
    {
      YYFPRINTF (stderr, "%3lu. ",
                 (unsigned long int) (yyp - yystackp->yyitems));
      if (*(yybool *) yyp)
        {
          YYFPRINTF (stderr, "Res: %d, LR State: %d, posn: %lu, pred: %ld",
                     yyp->yystate.yyresolved, yyp->yystate.yylrState,
                     (unsigned long int) yyp->yystate.yyposn,
                     (long int) YYINDEX (yyp->yystate.yypred));
          if (! yyp->yystate.yyresolved)
            YYFPRINTF (stderr, ", firstVal: %ld",
                       (long int) YYINDEX (yyp->yystate
                                             .yysemantics.yyfirstVal));
        }
      else
        {
          YYFPRINTF (stderr, "Option. rule: %d, state: %ld, next: %ld",
                     yyp->yyoption.yyrule - 1,
                     (long int) YYINDEX (yyp->yyoption.yystate),
                     (long int) YYINDEX (yyp->yyoption.yynext));
        }
      YYFPRINTF (stderr, "\n");
    }
  YYFPRINTF (stderr, "Tops:");
  for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
    YYFPRINTF (stderr, "%lu: %ld; ", (unsigned long int) yyi,
               (long int) YYINDEX (yystackp->yytops.yystates[yyi]));
  YYFPRINTF (stderr, "\n");
}
#endif
/* Line 2575 of glr.c  */
#line 480 "coolang_compiler_auto_integrate.y"


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
	printf("ЁЖ╢Мю╡\n");
	printf("%s\n", s);

}

//static int stmtMerge(YYSTYPE x0, YYSTYPE x1){ 
//puts("<OR> ");
// return 0;
//}