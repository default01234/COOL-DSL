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

    _arg____arg,
    _arg____CHANGEABLE_arg,
    _arg____PENDCHANGEABLE_arg,
    _arg____variable_specific_attribute_DOTDOT__arg_,

    expression_null___NULL,
    expression____arg_,
    expression___JIAN_expression,
    expression___JIA_expression,
    expression___expression_JIA_expression,
    expression___expression_JIAN_expression,
    expression___expression_CHENG_expression,
    expression___expression_CHU_expression,
    expression___expression_MI_expression,
    expression___expression_FUZHI_expression,
    expression___expression_DENGYU_expression,
    expression___expression_DAYU_expression,
    expression___expression_XIAOYU_expression,
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

    /*函数声明部分*/
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

    functionDeclare___function_declare_attribute_DOTDOT_autoScope,
    functionDeclare___function_declare_attribute_DOTDOT_scopeFunctionDeclare,
    functionDeclare___function_declare_attribute_scopeFunctionDeclare,
    functionDeclare___function_declare_attribute_autoscope,
    functionDeclare___function_declare_attribute_functionDeclare,
    functionDeclare___function_declare_attribute_DOTDOT_functionDeclare,
    functionImplement___functionDeclare_scope,
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

};

#endif /* RULE_TOKEN_NUMBER_H_ */
