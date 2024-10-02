/*  COOLang VM implementation
-

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

#ifndef COOLANG_VM_EXECUTE_HPP_
#define COOLANG_VM_EXECUTE_HPP_
#include "coolang_vm_integrate.hpp"
#include <algorithm>
#include <math.h>
 //#include <regex>
#include <thread>
//#include <unistd.h>
using namespace std;
namespace COOLANG {


	/**
	 * @brief 这个类通过比较实际执行的表达式与函数声明表达式的代码段来得到 形参实参对照表frmap
	 * @param cdtreal 实际执行（实参所在）的表达式所在cdt
	 * @param cdtformal 函数声明（形参）所在表达式所在cdt
	 * @param addrreal 实际执行表达式的根节点addr
	 * @param addrformal 函数声明表达式的根节点addr
	 * @attention 注意，cdtformal和cdtreal一旦确定就无法更改！否则会导致地址出错。
	 */
	class [[deprecated("use class Mather")]] FunctionHandleMatcher {
	public:
		CodeTable* cdtreal = nullptr;

		CodeTable* cdtformal = nullptr;

		map<ArgReal, deque<ArgFormal> > am;
		map<ArgFormal, ArgReal> frmap;
		map<AddrReal, AddrFormal> cdm;
		/**
		 * @brief judge whether an argReal and an argFormal is the same one arg.
		 * if two args with same names are at the same one scope, they are the same.
		 * @param argreal
		 * @param argformal
		 * @return
		 */
		bool sameOneArg(const ArgReal& argreal, const ArgFormal& argformal);

		FunctionHandleMatcher(CodeTable* const cdtreal,
			CodeTable* const cdtformal);

		const bool add(const ArgReal& argreal, const ArgFormal& argformal);


		const bool add(const AddrReal& adreal, const AddrFormal& adformal);

		const Addr getSameTrBranchLastArgAddr(CodeTable* codeTable, const Addr& ad,
			const Arg& a);

		const bool reset(CodeTable* cdreal, CodeTable* cdformal);

		const bool match(const AddrReal& adreal, const AddrFormal& adformal);

		map<ArgFormal, ArgReal>& getFRMap();


	};

	Intg CMP(const DataPtr op1_, const DataPtr op2_);

	class Executor {
	private:
		stack<DataTablePtr> arStack; //只能用于存储AR的共享指针，用以避免在子AR中进一步创建AR时父AR丢失的问题。不要在函数中直接修改此变量
		deque<map<FKA,DataTablePtr>>  arDeleteBuffer;//AR will be removed 1 step later.

	public:

		Intg mode = MODE_EXECUTION; //表明处于哪个阶段（GROUNDING or EXECUTION， 默认EXECUTION）
		bool isExecuting = false; //当MODE_GROUNDING时， 对代码段进行ground （正在调用rule（expr）函数） 或者 execute（执行fact函数）
		deque<FKA> fkatrack;
		deque<Strg> ground_exe_track;
		deque<Code> codetrack;
		deque<Strg> artrack;
		Strg currentFIL;
		Intg currentLIN;
		//fstream fsInfo;//for debugging
		//fstream fsOrder; //for debugging
		ostream* fsInfo = &std::cout;
		istream* fsOrder = &std::cin;
		string order; //for debugging
		bool STP = false; //单步
		bool BEG = false; //开始
		bool SPD = false; //暂停
		bool TRM = false; //终止
		map<Strg, set<Intg>> breakPointMap; //记录所有断点

		ScopeTableHash* scpth = nullptr;
		TemplateFunctionTableHash* tfth = nullptr;
		DataTablePtr executorAR = nullptr;
		Intg logicStage = -1; //-1 for invalid. 
		DataTablePtr currentAR = nullptr;
		PipeCommunicationHandler* pipe_writing_handler = nullptr;
		PipeCommunicationHandler* pipe_reading_handler = nullptr;

		/*Intg matchStateSiloMaxCapacity = 30;
		Intg groundingStateSiloMaxCapacity = 300;
		Intg maxSwitchTurn = 30;*/



		/*    //每当currentAR赋值时调用，指向赋值表达式右值
		 //每当deletecurrentAR时，将此指针所指指针置为null
		 DataTable ** currentAR_pp = nullptr;*/
		CodeTableHash* cdth = nullptr;
		//ExpressionHandlerPtr currentExpr = nullptr;
		SystemTableHash* systh = nullptr;

		Setting* setting = nullptr;


		FKI currentCodeFKI;
		FKA currentCodeFKA;
		Code* currentCode = nullptr;
		//ExpressionHandlerPtr currentExpr = nullptr;



		FKI nextCodeFKI; //当FKI与FKA冲突时，一切以FKI为准，寻找FKI对应的FKA
		FKA nextCodeFKA;
		Code* nextCode = nullptr;

		Arg argexe1;
		Arg argexe2;
		Arg operatorexe;
		Arg resultexe;
		FKA getScopeQEFKA(const FKA& codeFKA);
		FKA getScopeQSFKA(const FKA& codeFKA);
		FKA getNextAssociateBraceFKA(const FKA& currentBranchCodeFKA);
		bool getNewCode();
		bool setNextCodeFKI();
		bool ground(); //ground
		bool stepGround(); //ground
		bool execute(); //exe
		bool stepExecute(); //exe
		void setArgScopeFKA(); //ground
		bool executeCurrentCode(); //exe,groundexe
		bool addArgsToCurrentAR_execute();
		bool addArgsToCurrentAR_ground();
		map<ArgFormal, ArgReal> getFRMap();
		Executor(CodeTableHash* cdth,
			TemplateFunctionTableHash* tfth,
			ScopeTableHash* scpth,
			SystemTableHash* systh, Setting* setting);
		Intg update_AR_stack();
		Executor();

		Executor(HashCollection* hashCollection);

		void setPipeCommunicationHandler(PipeCommunicationHandler* pipe_writing_handler = nullptr,
			PipeCommunicationHandler* pipe_reading_handler = nullptr);



		DataTablePtr getParentAR(const FKA& scopeFKA);
		void setMode(Intg mode);
		bool parseOrderStrg(const Strg& orderStrg,
			map<Strg, set<Intg>>& breakPointMap, bool clearMap, bool& STP,
			bool& BEG, bool& SPD, bool& TRM);
		//此函数曾使用regex并引起relocation truncated to fit
		void parseIOFileName(Strg& leftOp, Strg& rightOp, Strg& inpath,
			Strg& outpath);

		//以下函数由executeCurrentCode中的语法糖改编而来

			/**
			 * this lambda used to tell difference between two different pairs.
			 * @return  0 not equal ; 1 equal;
			 * @attention this function will compair key and value.
			 */
		bool op1it_op2it_equal(const pair<const Data, Data>& a,
			const pair<const Data, Data>& b);

		/**
		 * @brief the following functions  implement the basic operation functionalitis.
		 */
		void ADD(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
			Intg& type_res);


		/*
		 * _.cpp
		 *
		 *  Created on: 31-八月-23
		 *      Author: 韩济澎
		 */

		void SUB(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
			Intg& type_res);

		;
		void CHU(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
			Intg& type_res);
		Intg CMP(const DataPtr op1_, const DataPtr op2_);

		void POW(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
			Intg& type_res);

		void ACC(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg pos, Intg& type_a1,
			Intg& type_a2, Intg& type_res);


		void ERS(DataPtr op1_, DataPtr op2_, Intg pos, Intg& type_a1, Intg& type_a2,
			Intg& type_res);

		;
		Intg FND(DataPtr op1_, DataPtr op2_, Intg& type_a1, Intg& type_a2,
			Intg& type_res, DataPtr d2);

		;

		/**
		 * @brief if you pass a list as the param , if the size of the list is two,
		 * the first element of the list will be regarded as the positon or key,
		 *  and the second element will be regarded as the value.this law is only suitable
		 *  for Strg or list/map/multimap, for other, the list whole will be regarded as an element
		 */
		void INS(DataPtr op1_, DataPtr op2_, Intg& type_a1, Intg& type_a2,
			Intg& type_res);
		;

		/**
		 * @brief for map/set/multiset, this function is the same as INSERT
		 */
		void PSB(DataPtr op1_, DataPtr op2_, Intg& type_a1, Intg& type_a2,
			Intg& type_res);

		;
		void PSF(DataPtr op1_, DataPtr op2_, Intg& type_a1, Intg& type_a2,
			Intg& type_res);

		;
		void PPB(DataPtr op1_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
			Intg& type_res);

		;
		void PPF(DataPtr op1_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
			Intg& type_res);

		;
		/**
		 * @brief this function try to return the inference of the back of the last element of the op1.
		 * if can't ,return the duplicate value of the last element.
		 */
		void BCK(DataPtr op1_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
			Intg& type_res);

		;

		void MUL(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
			Intg& type_res);


		Intg FIND_SUBEXPR(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
			Intg& type_res);


		Intg EXIST_SUBEXPR(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
			Intg& type_res);

		void RESET(const Arg& argexe1_, DataPtr op1_);
		;


	public:
		Strg toBrief() const;
		Strg ARtoBrief() const;
		void updateTrack(bool inStepGround);
		void clearTrack();

	};


}
#endif /* COOLANG_VM_EXECUTE_HPP_ */
