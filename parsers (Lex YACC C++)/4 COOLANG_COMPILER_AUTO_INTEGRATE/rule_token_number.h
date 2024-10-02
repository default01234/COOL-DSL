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
#include"y.tab.h"
#ifndef RULE_TOKEN_NUMBER_H_
#define RULE_TOKEN_NUMBER_H_

#define Token yytokentype
enum Rule {
    code___environment = 1,
    code___code_environment,
    environment_null___NULL,
    environment___functionImplement,
    environment___scope,
    environment___sentence,

	variable_specific_attribute___variable_specific_attribute_LSCUR_variable_specific_attribute_RSCUR,

    _arg____arg,
    _arg____CHANGEABLE_arg,
    _arg____PENDCHANGEABLE_arg,
	_arg____COMPATIBLE_arg,
    _arg____variable_specific_attribute_DOTDOT__arg_,

    expression_null___NULL,
    expression____arg_,
    expression___JIAN_expression,
    expression___JIA_expression,

    expression___NOT_expression,
    expression___USER_OP1_expression,
    expression___USER_OP2_expression,
    expression___expression_USER_OP3,
    expression___SI_expression,
    expression___SD_expression,
    expression___expression_SI,
    expression___expression_SD,
    expression___expression_AND_expression,

    expression___expression_OR_expression,
    expression___expression_USER_OP1_expression,
    expression___expression_USER_OP2_expression,

    expression___LIST_LSCUR_RSCUR,
    expression___MAP_LSCUR_RSCUR,
    expression___MULTIMAP_LSCUR_RSCUR,
    expression___SET_LSCUR_RSCUR,
    expression___MULTISET_LSCUR_RSCUR,

    expression___expression_TOINT_LSCUR_RSCUR,
    expression___expression_TONUM_LSCUR_RSCUR,
    expression___expression_TOSTRG_LSCUR_RSCUR,
    expression___NONBLOCKEXECUTE_LSCUR_expression_RSCUR,
    expression___BLOCKEXECUTE_LSCUR_expression_RSCUR,
    expression___SLEEP_LSCUR_expression_RSCUR,
    expression___LOGICJUMP_LSCUR_expression_RSCUR,


    expression___expression_JIA_expression,
    expression___expression_JIAN_expression,
    expression___expression_CHENG_expression,
    expression___expression_CHU_expression,
    expression___expression_MI_expression,
    expression___expression_MODULO_expression,
	expression___expression_JIADENG_expression,
    expression___expression_JIANDENG_expression,
    expression___expression_CHENGDENG_expression,
    expression___expression_CHUDENG_expression,
    expression___expression_MIDENG_expression,
    expression___expression_MODENG_expression,
    expression___expression_FUZHI_expression,
    expression___expression_DENGYU_expression,
    expression___expression_BUDENG_expression,
    expression___expression_DAYU_expression,
    expression___expression_XIAOYU_expression,
    expression___expression_BUDAYU_expression,
    expression___expression_BUXIAOYU_expression,
    expression___expression_DOTDOT_expression,
    expression___expression_COMMA_expression,
    expression___expression_SHUCHU_expression,
    expression___LSCUR_expression_RSCUR,
    expression___expression_LSCUR_expression_null_RSCUR,
    expression___expression_LSCUR_expression_RSCUR,

    sentence_null___NULL,
    sentence___SEMICOLON,
    sentence___expression_SEMICOLON,
    sentence___functionDeclare_SEMICOLON,
    sentence____return__SEMICOLON,
    sentence____return__expression_SEMICOLON,
	sentence____abort__SEMICOLON,
    sentence___branch,
    sentence___loop,


//    expression____return_,
//    expression____return__expression,

    scopeBegin___LBCUR,
    scopeHalf___scopeBegin_sentence,
    scopeHalf___scopeHalf_sentence,
    scopeHalf___scopeHalf_functionImplement,
    scopeHalf___scopeHalf_functionDeclare,
    scopeHalf___scopeHalf_scope,
    scope___scopeHalf_RBCUR,
    scope___scopeBegin_RBCUR,

	expr_scope_begin___LBCUR,
	expr_scope_half___expr_scope_begin_expression,
	expr_scope___expr_scope_half_RBCUR,

    /*函数声明部分,此部分逐步弃用*/
    expressionFunctionDeclareRoot___JIAN_expression,
    expressionFunctionDeclareRoot___JIA_expression,
    expressionFunctionDeclareRoot___expression_JIA_expression,
    expressionFunctionDeclareRoot___expression_JIAN_expression,
    expressionFunctionDeclareRoot___expression_CHENG_expression,
    expressionFunctionDeclareRoot___expression_CHU_expression,
    expressionFunctionDeclareRoot___expression_MI_expression,
    expressionFunctionDeclareRoot___expression_FUZHI_expression,
    expressionFunctionDeclareRoot___expression_DENGYU_expression,
    expressionFunctionDeclareRoot___expression_DAYU_expression,
    expressionFunctionDeclareRoot___expression_XIAOYU_expression,
	expressionFunctionDeclareRoot___expression_BUDAYU_expression,
    expressionFunctionDeclareRoot___expression_BUXIAOYU_expression,
    expressionFunctionDeclareRoot___expression_DOTDOT_expression,
    expressionFunctionDeclareRoot___expression_COMMA_expression,
    expressionFunctionDeclareRoot___expression_SHUCHU_expression,
    expressionFunctionDeclareRoot___LSCUR_expressionFunctionDeclareRoot_RSCUR,
    expressionFunctionDeclareRoot___expression_LSCUR_expression_null_RSCUR,
    expressionFunctionDeclareRoot___expression_LSCUR_expression_RSCUR,

    scopeFunctionDeclareBegin___LBCUR,
    scopeFunctionDeclareHalf___scopeFunctionDeclareBegin_sentence,
    scopeFunctionDeclareHalf___scopeFunctionDeclareHalf_sentence,
    scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot,
    scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot_SRMICOLON,
    scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot,
    scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot_SRMICOLON,
    scopeFunctionDeclare___scopeFunctionDeclareEnd_RBCUR,

    autoScopeBegin___NULL,
    autoScopeEnd___NULL,
    autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot,
    autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot_SEMICOLON, //todo: 需要修改，有一定冲突
    autoScope___autoScopeHalf_autoScopeEnd,

    functionDeclareBegin___function_declare_attribute_DOTDOT_autoScope,
    functionDeclareBegin___function_declare_attribute_DOTDOT_scopeFunctionDeclare,
    functionDeclareBegin___function_declare_attribute_scopeFunctionDeclare,
    functionDeclareBegin___function_declare_attribute_autoscope,
    functionDeclareBegin___function_declare_attribute_functionDeclareBegin,
    functionDeclareBegin___function_declare_attribute_DOTDOT_functionDeclareBegin,

    functionDeclareHalf___functionDeclareBegin_JICHENG_arg,

    functionDeclare___functionDeclareBegin,
    functionDeclare___functionDeclareHalf,

    functionImplement___functionDeclare_scope,
    functionImplement___functionImplement_TUIDAO_functionDeclare,
//
//    sentenceFunctionDeclare_null___NULL,
//    sentenceFunctionDeclare___expressionFunctionDeclareRoot_SEMICOLON,
//    sentenceFunctionDeclare___expressionFunctionDeclareRoot,
//
//    scopeFunctionDeclareBegin___NULL,
//    scopeFunctionDeclareBegin___LBCUR,
//
//    scopeFunctionDeclareEnd___NULL,
//    scopeFunctionDeclareEnd___RBCUR,
//
//    scopeFunctionDeclareHalf___scopeFunctionDeclareBegin_sentenceFunctionDeclare,
//
//    scopeFunctionDeclare___scopeFunctionDeclareHalf_scopeFunctionDeclareEnd,
//
//    functionDeclare___function_declare_attribute_DOTDOT_scopeFunctionDeclare,
//    functionDeclare___function_declare_attribute_scopeFunctionDeclare,
//    functionDeclare___function_declare_attribute_functionDeclare,
//    functionDeclare___function_declare_attribute_DOTDOT_functionDeclare,
//    functionImplement___functionDeclare_scope,
    branchBegin___IF_LSCUR_expression_RSCUR_autoScope,
    branchBegin___IF_LSCUR_expression_RSCUR_scope,
    branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_autoScope,
    branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_scope,
    branchEnd___branchBegin_ELSE__autoScope,
    branchEnd___branchBegin_ELSE__scope,
    branchEnd___branchHalf_ELSE__autoScope,
    branchEnd___branchHalf_ELSE__scope,
    branch___branchHalf,
    branch___branchBegin,
    branch___branchEnd,
    loop___WHILE_LSCUR_expression_RSCUR_autoScope,
    loop___WHILE_LSCUR_expression_RSCUR_scope,

    systemDeclareBegin___system_declare_attribute_DOTDOT_arg,
    systemDeclareHalf___systemDeclareBegin_JICHENG_arg,
    systemDeclare___systemDeclareBegin,
    systemDeclare___systemDeclareHalf,
    systemImplement___systemDeclare_scope,
    environment___systemImplement,
    sentence___systemDeclare_SEMICOLON,
    scopeHalf___scopeHalf_systemImplement,
    scopeHalf___scopeHalf_systemDeclare,
    sentence___functionImplement,
    sentence___systemImplement,

    expression___expression_DOT_expression,

    expressionForList___strictExpression,
    listBegin___LBCUR,
    listHalf___listBegin_expressionForList,
    list___listBegin_listEnd,
    list___listHalf_listEnd,
    listAccessBegin___NULL,
    listAccessHalf___listAccessBegin_expressionForListAccess,
    listAccess___listAccessBegin_listAccessEnd,
    listAccess___listAccessHalf_listAccessEnd,
    _arg____list,
    expression___expression_LMCUR_listAccess_RMCUR,

    expression___expression_CLEAR,
    expression___expression_ERASE_LSCUR_listAccess_RSCUR,
    expression___expression_INSERT_LSCUR_expression_RSCUR,
    expression___expression_LENGTH,
    expression___expression_TYPENAME,

    expression___expression_FIND_LSCUR_expression_RSCUR,
    expression___expression_COUNT_LSCUR_expression_RSCUR,

	expression___EXPR_expr_scope,
	expression___expression_EXIST_SUBEXPR_expr_scope,
	expression___expression_FIND_SUBEXPR_expr_scope,
	expression___RESET,
	
    expression___expression_PUSHBACK_LSCUR_expression_RSCUR,
    expression___expression_PUSHFRONT_LSCUR_expression_RSCUR,
    expression___expression_POPBACK_LSCUR_RSCUR,
    expression___expression_POPFRONT_LSCUR_RSCUR,
    expression___expression_BACK_LSCUR_RSCUR,

	

    sentence____FIL_LSCUR_arg_RSCUR,
    sentence____LIN_LSCUR_arg_RSCUR,
    sentence____LOAD_LSCUR_arg_RSCUR,
    sentence___BREAK_SEMICOLON,
    sentence___CONTINUE_SEMICOLON,
};

#endif /* RULE_TOKEN_NUMBER_H_ */
