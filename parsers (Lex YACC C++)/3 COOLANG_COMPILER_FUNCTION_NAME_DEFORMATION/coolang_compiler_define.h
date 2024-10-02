/*
 * coolang_define.h
 *
 *  Created on: 2022年8月16日
 *      Author: dell
 */

#ifndef COOLANG_DEFINE_H_
#define COOLANG_DEFINE_H_

#define debug false
#define Intg int
#define Intl int64_t
#define Numb double

#define safe_delete(p)  { if(p) { delete (p);     (p)=NULL; } }
#define FILEKEYEND "_end_"
#define FILEKEYBUILTIN "built-in"
#define COOLANG_PROJECT_PATH "COOLANG_PROJECT_PATH"
#define COOLANG_HOME "COOLANG_HOME"
#define COOLANG_SOURCE_CODE_SUFFIX ".cos"
#define COOLANG_CHARACTER_CODE_SUFFIX ".coc"
#define COOLANG_BINARY_CODE_SUFFIX ".cob"

#define T_  true
#define F_ false
#define A_ 3

#define N 0 //什么也不做
#define QS 1
#define QE 2
#define C 3
#define M 4
#define ME 5
#define F 6
#define R 7  //返回
#define B 8  //分支
#define L 9  //循环
#define TD 10 //推导
#define S 11 //系统
#define Y 12 //引用
#define LST 13 //列表
#define FIL 14 //文件 原则上，FIL、LIN不能打断表达式，但大多数情况下遇到嵌入表达式的FIL、LIN会被直接忽略
#define LIN 15 //行数
#define LOAD 16 

#define Dz 0
#define Sz 1
#define Bs 2
#define Zf 3

#define S_Dz 0  //用于arg、data均可，表明其内容为数据区的地址，可用于<dataTable>键、值
#define Sz 1    //用于arg，或data，可用于<dataTable>值
#define S_Bs 2  //用于arg、data均可，表明其内容为数据区的标识，可用于<dataTable>键、值
#define Zf 3    //用于arg、data均可，表明其内容为字符，可用于<dataTable>值
#define Z_Dz 4  //用于arg，表明其内容为作用域表的地址，在<dataTable>中被替换为S_AR
#define M_Bs 5  //用于arg，表明这是一个模板用到的符号+-*/等
#define M_Dz 6  //用于arg，表明这是一个模板用到的符号在符号表中的地址
#define X_Bs 7  //用于arg，表明这是一个作用于变量的属性，亦或是用户定义的在系统表中的类
#define X_Dz 8  //用于arg，表明这是一个作用于变量的属性，亦或是用户定义的在系统表中的类
#define S_AR 9  //通常用于data，表明这是一个类的实例，可用于<dataTable>的值
#define Y_Dz 10 //用于arg，表明这是一个引用，储存在引用表，可以作为Code中四元式标志位，可用于<dataTable>键，注意，类型Y_Dz的arg其rootAR不能为空
#define S_LST LST//#define LST 13 //用于arg，用于Data，表明此Data中储存着一个列表
#define S_PTR 14//仅用于Data，用于说明对此data进行赋值、访问(查询、比大小等)操作请使用ptr所指data，但删除此data时不删除ptr所指data；

#define FN 1   //函数名称作用域
#define FB 2   //普通函数体
#define RFB 3  //函数体，包含return
#define BRB 4  //分支的体
#define LOB 5  //循环的体
#define SYSB 6 //系统的体

#define Du 2
#define R_ R //一定条件下执行





#endif /* COOLANG_DEFINE_H_ */
