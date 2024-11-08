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
#ifndef COOLANG_VM_SCAN_HPP_
#define COOLANG_VM_SCAN_HPP_

#include "coolang_vm.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <bitset>

 /***************************************
  *                 Scan
  ***************************************/
using namespace std;
namespace COOLANG {
	class ExpressionHandler;
	typedef ExpressionHandler ExprHandler;
	typedef SharedPtr<ExpressionHandler> ExpressionHandlerPtr;
	typedef WeakPtr<ExpressionHandler> ExpressionHandlerPtrW;

	//	class ExprHandlerPool
	//	{
	//	public:
	//		map<ExpressionHandlerPtr, Intg> pointerPool;
	//
	//		/**
	//		 * \brief add a pointer to the pool
	//		 * \param expression_handler
	//		 * @attention: only invokable in ExpressionHandler
	//		 */
	//		void addPointer(ExpressionHandlerPtr expression_handler)
	//		{
	//			if (expression_handler == nullptr)
	//			{
	//				return;
	//			}
	//			if (pointerPool.count(expression_handler) == 0)
	//			{
	//				pointerPool[expression_handler] = 1;
	//			}
	//			else {
	//#if debug && _WIN32
	//				{
	//					static int ignoreCount = 0;
	//					// Assuming info is a std::string, convert to std::wstring
	//					std::string info = "error exit"; // Replace with actual info
	//					info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
	//					std::wstring winfo(info.begin(), info.end());
	//					#if WIN_DEBUG_OUTPUT
//OutputDebugString(winfo.c_str());
//#endif
// cout<<info<<endl;
	//					ignoreCount++;
	//				}
	//#endif
	//				cout << "Already existing ExpressionHandler pointer in addPointer, exit(-1)";
	//				exit(-1);
	//			}
	//			return;
	//		}
	//
	//		/**
	//		 * \brief acquire a pointer from an existing  ExprHandler instance of Arg, and the reference counter of the pointer self-increases.
	//		 * \param expression_handler
	//		 * @attention: only invokable in ExpressionHandler
	//		 */
	//		void acquirePointer(ExpressionHandlerPtr expression_handler)
	//		{
	//			if (expression_handler == nullptr)
	//			{
	//				return;
	//			}
	//			if (pointerPool.count(expression_handler) == 0)
	//			{
	//
	//#if debug && _WIN32
	//				{
	//					static int ignoreCount = 0;
	//					// Assuming info is a std::string, convert to std::wstring
	//					std::string info = "error exit"; // Replace with actual info
	//					info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
	//					std::wstring winfo(info.begin(), info.end());
	//					#if WIN_DEBUG_OUTPUT
//OutputDebugString(winfo.c_str());
//#endif
// cout<<info<<endl;
	//					ignoreCount++;
	//				}
	//#endif
	//				cout << "Invalid ExpressionHandler pointer in acquirePointer, exit(-1)";
	//				exit(-1);
	//			}
	//			else
	//			{
	//				pointerPool[expression_handler] += 1;
	//			}
	//		}
	//
	//		/**
	//		 * \brief release an expression_handler pointer it has obtained, leading the reference counter to self decrease.
	//		 * @attention: only invokable in ExpressionHandler
	//		 */
	//		void releasePointer(ExprHandler* expression_handler)
	//		{
	//			if (expression_handler == nullptr)
	//			{
	//				return;
	//			}
	//			if (pointerPool.count(expression_handler) == 0) {
	//#if debug && _WIN32
	//				{
	//					static int ignoreCount = 0;
	//					// Assuming info is a std::string, convert to std::wstring
	//					std::string info = "error exit"; // Replace with actual info
	//					info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
	//					std::wstring winfo(info.begin(), info.end());
	//					#if WIN_DEBUG_OUTPUT
//OutputDebugString(winfo.c_str());
//#endif
// cout<<info<<endl;
	//					ignoreCount++;
	//				}
	//#endif
	//				cout << "Null ExpressionHandler pointer in releasePointer, exit(-1)";
	//				exit(-1);
	//			}
	//			else {
	//				pointerPool[expression_handler] -= 1;
	//				if (pointerPool[expression_handler] == 0) {
	//					pointerPool.erase(expression_handler);
	//				}
	//
	//			}
	//			return;
	//		}
	//		bool existPointer(ExprHandler* const expression_handler) const {
	//			return pointerPool.count(expression_handler) > 0;
	//
	//		}
	//
	//	};
	//	static ExprHandlerPool exprHandlerPool;



	class ArgComparatorLT {
	private:
		acm mode;
		DataTablePtrW dt;
		CodeTableHash* cdth;
	public:
		ArgComparatorLT(acm mode_ = acm::name | acm::ref, DataTablePtrW dt_ = nullptr, CodeTableHash* cdth_ = nullptr);
		bool operator()(const Arg& lhs, const Arg& rhs) const;
	};
	typedef SharedPtr<Arg> ArgPtr;
	typedef WeakPtr<Arg> ArgPtrW;
	class Arg {
	private:
		bool on_heap = false;
		ArgPtrW thisPtr = nullptr;
	public:

		static inline Intg symbol_count = 0;
		Numb arg_i = 0;
		FKA arg_fka;
		Strg arg_s;
		Intg argFlag = 0;//type
		FKA asc;
		DataTablePtrW ref_ar = nullptr; //this ar is only used for searching reference, not for storing class instance. (class is stored in data)
		bool formalArg = true; //formal Arg is also known to be local arg. which is args directly created in current scope, like function name scope.
		Intg changeable = F_;
		Intg unknown = F_;
		bool compatible = F_;
		bool isSymbol = F_;
		Intg symbolID = 0;
		set<Constraint>  constraints;
		bool isPlaceholder = F_;
		bool isBound = F_;
		//Arg* binding_arg = nullptr; stored in data, expr
		bool isExpression = F_; //todo::这个用处也不大了。这种标识data属性的标志位应当逐步从Arg中去除。现在没时间先留着吧
		//ExprHandler* expression_handler = nullptr;

		//memory manage

		void setOnHeap();

		bool onHeap() const;

		inline void setThisPtr(ArgPtrW _this) {
			this->thisPtr = _this;
		}

		ArgPtr getThisPtr() const;
		static void infer_changeable(Arg* arg1_, Arg* arg2_, const Arg& op, Arg* argres_) {
			if (constantChangeableOps.count(op.arg_s) && argres_) {
				argres_->changeable = true;
			}
			else if (constantUnchangeableOps.count(op.arg_s) && argres_) {
				argres_->changeable = false;
			}
			else if (constantAdaptiveChangeableOps.count(op.arg_s) && argres_) {
				argres_->changeable = A_;
			}
			else if (inheritedChangeableOps.count(op.arg_s) && argres_) {
				if (arg1_ && arg1_->changeable == T_ || arg2_ && arg2_->changeable == T_) {
					argres_->changeable = T_;
				}
				else {
					argres_->changeable = max(arg1_ ? arg1_->changeable : 0, arg2_ ? arg2_->changeable : 0);
				}
			}

			if (arg1_ && argres_ && intersect(*arg1_, *argres_, acm::name | acm::asc)) {
				arg1_->changeable = argres_->changeable;

			}
			if (arg2_ && argres_ && intersect(*arg2_, *argres_, acm::name | acm::asc)) {
				arg2_->changeable = argres_->changeable;

			}
			return;
		}

		/// <summary>
		/// extract new implicit constraint from composed constraints and merge it to current constraint set.
		/// </summary>
		/// <param name="dt"></param>
		/// <param name="systh"></param>
		void expandConstraints(DataTablePtrW dt = nullptr, SystemTableHash* systh = nullptr/*for future purpose*/);

		/*	/// <summary>
			/// use the Arg as Key in the dataTable
			/// </summary>
			/// <param name="dt"></param>
			void synchronizeFlagBitFromDataTable(DataTablePtrW dt, acm md = acm::placeholder) {

			}*/

			//compare two constraints, whether they have intersection. md:comparason mode.
		static int intersect(const Arg& argwithconstr1, const Constraint& c1, const Arg& argwithconstr2, const Constraint& c2, acm md, DataTablePtrW dt);
		/// <summary>
		/// compare two args, whether they have an intersection.
		/// </summary>
		/// <param name="arg1_"></param>
		/// <param name="arg2_"></param>
		/// <param name="mode"></param>
		/// <param name="dt"></param>
		/// <param name="cdth">used for EXPRZ_Dz arg comparason for data</param>
		/// <returns></returns>
		static bool intersect(const Arg& arg1_, const Arg& arg2_, acm mode = acm::name | acm::ref, DataTablePtrW dt = nullptr, CodeTableHash* cdth = nullptr);


		/// <summary>
		/// compare two args, whether arg1 <= arg2, in specific aspects.
		/// </summary>
		/// <param name="arg1_"></param>
		/// <param name="arg2_"></param>
		/// <param name="mode"></param>
		/// <param name="dt"></param>
		/// <param name="cdth">used for EXPRZ_Dz arg comparason for data</param>
		/// <returns></returns>
		static bool comparatorLess(const Arg& arg1_, const Arg& arg2_, acm mode = acm::name | acm::ref, DataTablePtrW dt = nullptr, CodeTableHash* cdth = nullptr);

		Arg() {
		}
		const bool operator<(const Arg& arg) const;

		/**
		 * @brief 是否S_Bs、S_Dz、Z_Dz、S_AR、Y_Dz
		 * @return
		 */
		bool isVar() const;

		/**
		 * @brief 是否S_Bs
		 * @return
		 */
		bool isBs() const;

		/**
		 * @brief 是否S_Dz、Z_Dz、Y_Dz
		 * @return
		 */
		bool isDz() const;

		void initializeAsSymbol();


		void resetPlaceholder();
		/**
		 * "=="仅比较内容是否相同，不比较除argFlag外的标志位
		 * @param arg
		 * @return
		 */
		bool operator==(const Arg& arg) const;

		bool operator!=(const Arg& arg) const;


		Arg& operator=(const Arg& arg);

		Arg& assign(const Arg& arg);

		Arg(Intg flag, const FKA& arg_fka);

		Arg(Intg flag, const Numb& arg_i);

		Arg(Intg flag, const Strg& arg_s);

		Arg(Intg flag, DataTablePtr const ar);

		Arg(Intg flag, const FKA& arg_fka, DataTablePtr const ar);

		Arg(Intg flag, const FKA& arg_fka, const Numb& arg_i, const Strg& arg_s);

		Arg(Intg flag, const FKA& arg_fka, const Numb& arg_i, const Strg& arg_s,
			const bool& formalArg);

		Arg(Intg flag, const FKA& arg_fka, const Numb& arg_i, const Strg& arg_s,
			const bool& formalArg, const FKA& asc);

		Arg(Intg flag, const FKA& arg_fka, const Numb& arg_i, const Strg& arg_s,
			const bool& formalArg, const Intg& changeable, const FKA& asc);

		Arg(Intg flag, const FKA& arg_fka, const Numb& arg_i, const Strg& arg_s, Intg symbolID,
			DataTablePtr ref_ar, const bool& formalArg, const Intg& changeable,
			const Intg& unknown, bool compatible, const FKA& asc, const set<Constraint>& constraints);





		Strg toBrief() const;
		Strg toStrg() const;

		ConstraintArg toConstraintArg() const;



	};
	inline Arg argnull = Arg();
	class AssemblyCodeArg {
	public:
		Intg type = 0;
		Intg arg_i = 0;
		Strg arg_s;
		AssemblyCodeArg();


		AssemblyCodeArg(const Intg& type, const Intg& arg_i);

		AssemblyCodeArg(const Intg& type, const Strg& arg_s);

		AssemblyCodeArg(const Intg& type, const Intg& arg_i, const Strg& arg_s);

		Strg toStrg() const;

		bool operator==(const AssemblyCodeArg& arg) const;

		AssemblyCodeArg& operator=(const AssemblyCodeArg& arg);


		bool operator<(const AssemblyCodeArg& arg) const;

	};

	class AssemblyCode;
	class AssemblyCodeTable;
	class AssemblyCodeTableHash;
	class AssemblyCode {
	public:
		Intg type = COOL_N;

		Intg arg1Flag = 0;

		Intg arg2Flag = 0;

		Intg operatorFlag = 0;

		Intg resultFlag = 0;

		Numb arg1_i = 0;

		Strg arg1_s;

		Numb arg2_i = 0;

		Strg arg2_s;

		Numb operator_i = 0;

		Strg operator_s;

		Numb result_i = 0;

		Strg result_s;

		AssemblyCode(Strg assemblyCode);

		static vector<Strg> splitArgs(Strg str, Strg pattern) {
			vector<Strg> result;
			str += pattern;
			for (int i = 0; i < (int)str.size(); i++) {
				int pos = (int)str.find(pattern, i);
				if (pos != (int)string::npos) {
					result.push_back(str.substr(i, pos - i));
					i = pos + (int)pattern.size() - 1;
				}
			}
			return result;
		}
		AssemblyCode() {
		}
		Strg toStrg() const;

		bool operator==(const AssemblyCode& asc) const;

		AssemblyCode& operator=(const AssemblyCode& asc);

		AssemblyCodeArg operator[](const Intg pos);

		bool includeArg(const AssemblyCodeArg& arg);


	};
	static AssemblyCode ascnull = AssemblyCode();




	class AssemblyCodeTable {
	public:
		Strg fileKey;
		Intg codeForm = 0; //0,1（cos源代码），2（coc），3（cob）//表明代码形式
		bool completed = true; //0无效(不完整)（起占位作用，内部只有加载语句）,1有效（这就是源代码表）
		deque<Strg> dependentCdtFileKey_CodeForm2List;
		deque<Strg> dependentCdtFileKey_CodeForm3List;
		AssemblyCodeTableHash* ascth = nullptr;
		deque<AssemblyCode> acdq;
		deque<Intg> acpdq;
		Strg toStrg() const;
		Intg getLastNot(const vector<Intg>& types);
		AssemblyCodeTable cutTreeBranch(const AssemblyCodeArg& rootarg,
			const Intg& acpUpBound);
		AssemblyCodeTable cutLongestExpression();
		Intg getLastArgAsResult(const AssemblyCodeArg& arg, const Intg& acpUpBound);
		AssemblyCodeTable operator=(const AssemblyCodeTable& asct);


	};
	class AssemblyCodeTableHash {
	public:
		map<Strg, AssemblyCodeTable> file_asct_map;
		map<Addr, Strg> addr_file_map;
		map<Strg, Addr> file_addr_map;
		Addr getLastAsctAddr(const Addr& addr);
		Addr getNextAsctAddr(const Addr& addr);
		Strg getLastAsctFileKey(const Strg& fileKey);
		Strg getNextAsctFileKey(const Strg& fileKey);
		Addr insertAsct(AssemblyCodeTable& asct, const Addr& currentAddr,
			const bool& front);
		Addr appendAsct(AssemblyCodeTable& asct);
		Strg toStrg() const;

		AssemblyCodeTable& operator[](const Intg& pos);
		AssemblyCodeTable& operator[](const Addr& addr);
		AssemblyCodeTable& operator[](const Strg& fileKey);
		Intg size();


	};


	template<typename T_list>
	class ListPrinter {
	public:
		stringstream ss;
		//    ss.precision(PRECISION);
		ListPrinter(const T_list& list) {
			Intg j = list.size();
			for (int i = 0; i < j; i++) {
				ss << list.at(i).toStrg();
			}
		}

	};


	AssemblyCodeTable assemblyCodeTableDeformation(AssemblyCodeTable& asct);



	typedef Addr ScopeAddr;
	typedef FKA ScopeFKA;
	class Scanner;
	/**
	 * @brief 此类进行（解析类表，解析作用域表，解析函数表）
	 */
	class Scanner {
	public:
		////储存所有设置，请从中选取合适设置
		Setting* setting = nullptr;
		////以下两个三维表用于短期存储数据的类型
		map<ScopeFKA, map<Strg, Intg>> scpfka__name_attr_map;
		map<ScopeFKA, map<FKA, Intg>> scpfka__argfka_attr_map;
		////以下map用于存储addr与ar的对应关系。scan过程中每个scope对应一个ar，不自动释放ar，不占ar的引用数
		map<Strg, map<ScopeFKA, DataTablePtr> > file_fka_ar_map;
		///用于储存exprz_dz 所包含的fka。
		set<FKA> exprz_dz_set;
		////以下set用于存储已经整合到codeTableHash中的cdt的fileKey
		set<Strg> fileKeyExistSet;
		////以下set用于存储已经解析过的coc文件对应的fileKey（或已经整合到assemblyCodeTableHash中的cdt的fileKey（stem））
		set<Strg> fileKeyExistSet_asc;

		CodeTable* currentCodeTable = nullptr;
		ScopeTable* currentScopeTable = nullptr;
		SystemTable* currentSystemTable = nullptr;
		TemplateFunctionTable* currentTemplateFunctionTable = nullptr;
		CodeTableHash* codeTableHash = nullptr;
		ScopeTableHash* scopeTableHash = nullptr;
		SystemTableHash* systemTableHash = nullptr;
		TemplateFunctionTableHash* templateFunctionTableHash = nullptr;

		DataTablePtr scannerAR = nullptr;
		DataTablePtr currentAR = nullptr;

		AssemblyCodeTableHash ascth;
		map<Strg, Intg> loadedFiles;   //已经加载的文件，不重复加载，仅限sourceCodeToAssemblyCode内使用
		AssemblyCodeTable* assemblyCodeTable = nullptr;

		Intg currentAssemblyCodePos = -1;
		AssemblyCode currentAssemblyCode;
		AssemblyCode nextAssemblyCode;

		Strg currentFileKey = FILEKEYPRIMARY;

		vector<Addr> scopeStack;
		const Intg pushStack(Intg addr);
		const Intg pushStack(const Addr& addr);

		Code currentCode;
		Scope currentScope;

		//FKA currentScopeFKA = FKA(FILEKEYPRIMARY,Addr()); use currentAR->scopeStructureFKA, or create it one time only for special cases.
		FKA currentCodeFKA;
		FKA lastM;

		//以下代码会在提取信息后停用（Du）
		vector<FKA> pendM;  //该TAC（COOL_M，表达式类型）是否创建了一个待定的变量
		vector<FKA> realM; //该TAC（COOL_M,表达式类型）是否引用了一个已有的实参变量（被out修饰），而不是创建一个当前作用域变量或形参变量 
		vector<FKA> compatibleM; //该TAC （COOL_M）是否令一个变量为compatible
		vector<FKA>	constraintM; // 该TAC （COOL_M） 是否对一个变量施加了一个约束

		bool loadAssemblyCode(const filesystem::path& filePath);
		vector<AssemblyCodeTable> sourceCodeToAssemblyCode(
			const filesystem::path& filePath);
		const bool updateCurrentAssemblyCode();
		const bool setArgScopeFKA();
		const bool addArgsToCurrentAR();
		Addr stackToAddr() const;
		FKA stackToFKA() const;
		//const FKA getQSFKA(const FKA& fka);
		const FKA getQSFKA(Intg addr);            //与currentfilekey组成fka
		const Addr getScopeFKA(const FKA& fka) const;
		const Addr getScopeFKA(Intg addr) const;            //与currentfilekey组成fka
		bool setQuaternionAndFlagBit(const AssemblyCode& asc, AssemblyFormula* asf);
		/**
		 * @brief 查询一个变量的属性
		 * @param varname
		 * @param scopeAddr
		 * @param out 是否从上层作用域开始检索
		 * @return 变量的属性，如果变量不存在，则返回-1；
		 */
		Intg getAttribute(const Strg& varname, const ScopeFKA& scopeFKA, bool out);
		Intg getAttribute(const FKA& arg_fka, const ScopeFKA& scopeFKA, bool out);
		/**
		 * @brief 查询一个变量的信息，其储存在一个arg中
		 * @param varname
		 * @param scopeFKA
		 * @param out 是否从上层作用域开始检索
		 * @return 一个变量，其储存了变量所在scope（asc）、属性等，如果变量不存在，则返回argnull；
		 */
		Arg getArg(const Strg& varname, const ScopeFKA& scopeFKA, bool out);
		Arg getArg(const FKA& arg_fka, const ScopeFKA& scopeFKA, bool out);
		/**
		 * @brief 根据之前的变量重置一个变量的属性，若这个变量不存在且out为假，
		 * 则在scopeFKA中创建一个新的变量
		 * @param varname
		 * @param scopeFKA 变量所在作用域
		 * @param out 是否从上层作用域开始搜索
		 * @return
		 */
		void resetAttribute(const Strg& varname, const ScopeFKA& scopeFKA,
			const Intg& attr, bool out);
		void resetAttribute(const FKA& varFKA, const ScopeFKA& scopeFKA,
			const Intg& attr, bool out);
		/**
		 * @brief 向作用域中添加一个变量属性的索引
		 * @param varname
		 * @param scopeAddr
		 */
		void addAttribute(const Strg& varname, const ScopeFKA& scopeFKA,
			const Intg& attr);
		void addAttribute(const FKA& arg_fka, const ScopeFKA& scopeFKA,
			const Intg& attr);
		const bool scan();
		Scanner();

		Scanner(CodeTableHash* codeTableHash_, ScopeTableHash* scopeTableHash_,
			TemplateFunctionTableHash* templateFunctionTableHash_,
			SystemTableHash* systemTableHash_,
			Setting* setting_);

		Scanner(HashCollection* hashCollection);

		const FKA nextSameScopeTfFKA(const FKA& parentScopeFKA,
			const FKA& codeFKA) const;
		/**
		 * @brief 将code进行解析，更新（reset）参数属性表中的属性，尤其是地址属性
		 * @param code
		 * @brief 用于在扫描过程中存储各个变量对应的属性。
		 * @brief 包括S_Dz、S_AR、、Z_Dz、S_AR、Y_Dz、S_Bs、EXPRZ_Dz等类型
		 * @attention 由于在经过一些操作后变量的类型可能会修改，所以注意及时更新变量的类型
		 */
		void updateArgAttribute(const Code& code);


		~Scanner();
	};
	class DataTable;

	typedef SharedPtr<Data>	DataPtr;


	typedef SharedPtr<map<Data, Data>> DataMapPtr;


	typedef SharedPtr<deque<Data>> DataLstPtr;

	typedef SharedPtr<map<Data, multiset<Data>>> DataMultiMapPtr;

	typedef SharedPtr<set<Data>> DataSetPtr;

	typedef SharedPtr<multiset<Data>> DataMultiSetPtr;


	typedef WeakPtr<Data>	DataPtrW;
	typedef WeakPtr<deque<Data>> DataLstPtrW;
	typedef WeakPtr<map<Data, Data>> DataMapPtrW;
	typedef WeakPtr<map<Data, multiset<Data>>> DataMultiMapPtrW;
	typedef WeakPtr<set<Data>> DataSetPtrW;
	typedef WeakPtr<multiset<Data>> DataMultiSetPtrW;
	class DataTable;
	class Data {
	private:
		DataPtrW thisPtr;
		bool on_heap = false;
	public:
		//dataFlagBit 相当于Arg的flagBit（其实data和Arg差不多）
		Ints dataFlagBit = S_Dz;//0
		//if a Data is a reference var, its function : clean() won't delete the memory inner pointer pointing at.
		bool isReferenceVar = false;

		Numb content_i = 0;
		Strg content_s;
		FKA content_fka;
		DataTablePtr content_ar = nullptr;
		DataPtr content_ptr = nullptr;
		DataLstPtr content_lst = nullptr;
		DataMapPtr content_map = nullptr;
		DataMultiMapPtr content_multimap = nullptr;
		DataSetPtr content_set = nullptr;
		DataMultiSetPtr content_multiset = nullptr;
		bool isExpression = false;
		ExpressionHandlerPtr expr;




		////memory management
		//void* operator new(size_t size) {
		//	void* ptr = ::operator new(size);

		//	return ptr;
		//}

		/*
			Do not call manually， only called by make_shared_ptr
		*/
		void setOnHeap();

		bool onHeap() const;

		inline void setThisPtr(DataPtrW _this) {
			this->thisPtr = _this;
		}

		DataPtr getThisPtr() const;

		//弃用
		//    Addr scopeStructureAddr;
		//    Addr linkAddr;
		Data() {}



		Data& shallowAssign(const Data& data_);
		Data& operator=(const Data& data_);
		Data& operator=(Data&& data_);
		Data& operator=(const Arg& arg);
		Data(const Data& data_);
		/**
		 * @brief duplicate this Data and the memory pointed by inner pointers.
		 * @param data_
		 * @return
		 */
		Data deepAssign() const;
		void deepAssignTo(DataPtr dt) const;
		void deepAssignFrom(DataPtr dt);

		//    DataPtr deepAssign(const Arg &arg);
		bool operator==(const Data& data_) const;
		explicit Data(const Arg& argName, bool assign = true, DataTablePtrW  dt = nullptr, CodeTableHash* cdth = nullptr);

		explicit Data(const Numb& num);
		explicit Data(const Intg& type, void* par_or_plst);

		explicit Data(const Strg& strg);
		void addExpression(ExpressionHandlerPtr eh, bool duplicate);


		Strg toStrg() const;
		Strg valueToStrg() const;
		Numb valueToNumb() const;
		bool valueToBool() const;
		Intg containerSize() const;
		void clear();
		bool operator<(const Data& dt) const;
		template<typename TypeList>
		static TypeList deepAssignLST(const TypeList& lst) {
			TypeList tl;
			for (const auto& it : lst) {
				tl.push_back(it.deepAssign());
			}
			return tl;

		}
		template<typename TypeSet>
		static TypeSet deepAssignSET(const TypeSet& set_) {
			TypeSet ts;
			for (const auto& it : set_) {
				ts.insert(it.deepAssign());
			}
			return ts;
		}

		//void removeExpression() {
		//	if (this->isExpression) {
		//		exprHandlerPool.releasePointer(this->expr);
		//	}
		//	this->isExpression = false;
		//}

		~Data();


	};
	static Data datanull = Data();
	typedef Arg ArgFormal;
	typedef Arg ArgReal;
	typedef Arg ArgKey; //only allowed key arg type is S_Dz，S_Bs, EXPRZ_Dz, S_AR
	typedef Arg SymKey; //must be a symbol(is symbol is true)

	/// <summary>
	/// for element argdmap, 
	/// </summary>
	class DataTable {
	private:
		DataTable(const DataTable&);
		bool locked = false;
		bool rollback_on_heap = false;
		bool on_heap = false;
		DataTablePtrW thisPtr = nullptr;
	public:

		bool backwardExecute = F_;
		//此deque用于指定反向函数运行结束后应当跳转到的meetaddr。
		deque<FKA> bfMeetFKAdq;

		Intg type = 0;
		/*map<FKA, Data> argdmap;
		map<Strg, Data> sdmap;*/
		map<ArgKey, Data> argdmap; //argdmap 中不区分不同asc的参数，即arg.asc对此位无效。对应arg的data请储存于各自scope的ar中
		inline static map<SymKey, Data> symdmap;
		map<ArgFormal, ArgReal, ArgComparatorLT> frmap;//argdmap 中区分不同asc的参数，即arg.asc对此位有效。
		FKA scopeStructureFKA;
		DataTablePtrW parentAR = nullptr;
		DataTablePtrW returnAR = nullptr;
		FKA meetFKA;
		deque<DataTablePtrW> queryARList;
		deque<DataTablePtr> queryARList_local; // such as the inherited class of this AR(class). They are owned by this ar. all the elements in this list should be referenced in queryARList again.
		//内存处理部分
		//bool onHeap = false;
		//Intg referenceCount = 0;//only influence heap instance
		//inline static std::set<DataTablePtr> heapObjSet;
		//void* operator new(std::size_t size) {
		//	DataTablePtr dt = (DataTablePtr)malloc(size);
		//	heapObjSet.insert(dt);
		//	return dt;
		//}
		//please use DELETE_SYSB or DELETE_AR instead of this delete
		//void operator delete(void* ptr_) {
		//	return free(ptr_);
		//}

		//DataTable() {
		//	if (heapObjSet.count(this)) {
		//		this->onHeap = true;
		//	}
		//	else {
		//		this->onHeap = false;
		//	}
		//}

		/*
			Do not call manually， only called by make_shared_ptr
		*/
		void setOnHeap();

		bool onHeap() const;

		inline void setThisPtr(DataTablePtrW _this) {
			this->thisPtr = _this;
		}

		DataTablePtr getThisPtr() const;

		/*
		Considering this datatable as the AR of a function call, return the reference of the last function root AR (upper ar for this)
		*/
		map<ArgFormal, ArgReal, ArgComparatorLT>& getFunctionCallFRMap(TemplateFunctionTableHash* tfth, ScopeTableHash* scpth);

		//use the two functions to ban/allow smartptr managing the memory:
		bool lock();
		bool unlock();

		DataTable() :frmap(ArgComparatorLT(acm::name | acm::asc)) {}
		/**
		 *
		 * @param parentAR
		 * @param queryAR
		 * @param scopeStructureAddr
		 */
		DataTable(DataTablePtrW parentAR, DataTablePtrW queryAR,
			const FKA& scopeStructureFKA);
		DataTable(DataTablePtrW parentAR, DataTablePtrW queryAR, DataTablePtrW returnAR,
			const FKA& scopeStructureFKA, const Intg& type);

		void setFRMap(const map<ArgFormal, ArgReal>& frmap);

		const Intg addData(const FKA& fka, const Data& data);
		//Data& operator[](const FKA& fka);
		Data& operator[](const Strg& name);
		Data& operator[](const Arg& arg);
		const Arg getKeyArg(const Arg& argindex);
		void setKeyArg(const Arg& newkey);
		set<Arg, ArgComparatorLT> getPlaceholders(const CodeTable& cdt);
		bool operator==(const DataTable& dt) const;
		bool operator<(const DataTable& dt) const;
		DataTable& operator=(const DataTable& dt);
		bool add(const Arg& arg);
		bool findcurrent(const Arg& arg) const;

		/**
		 * @ 返回arg所在scope的fka，当前ar及其子ar
		 * @param arg
		 * @return
		 */
		FKA getArgScopeFKA(const Arg& arg) const;
		bool findall(const Arg& arg) const;
		DataTablePtrW getArgAR(const Arg& arg) const;
		FKA setArgScopeFKA(Arg& arg);
		static DataTablePtrW getAccessibleAR(DataTablePtrW searchingAR, const FKA& arScopeFKA) {

			//    set<DataTablePtr> checkedAR; //
			//    set<DataTablePtr> checkingAR;
			//    set<DataTablePtr> candidateAR;
			map<DataTablePtrW, Intg> checkedAR;
			map<DataTablePtrW, Intg> checkingAR;
			map<DataTablePtrW, Intg> candidateAR;
			checkingAR.insert(make_pair(searchingAR, 0));

			while (true) {
				for (auto ar_it : checkingAR) {
					auto ar = ar_it.first;
					if (checkedAR.count(ar)) {
						continue;

					}
					else {
						if (ar != nullptr && ar->scopeStructureFKA == arScopeFKA) {
							return ar;
						}
						else if (ar != nullptr) {
							candidateAR.insert(make_pair(ar->parentAR, 0));
							candidateAR.insert(make_pair(ar->returnAR, 0));
							for (auto queryAR : ar->queryARList) {
								candidateAR.insert(make_pair(queryAR, 0));
							}
							checkedAR.insert(make_pair(ar, 0));
						}
					}

				}
				for (auto checkedAR_ : checkedAR) {

					candidateAR.erase(checkedAR_.first);
				}
				if (candidateAR.size() > 0) {
					checkingAR.clear();
					candidateAR.swap(checkingAR);
					checkingAR.erase(nullptr);
				}
				else {
					return nullptr;
				}

			}
		}

		void deepAssign(DataTablePtr& d1, DataTablePtrW d2, bool copyClassInstances = true);
		Strg _toStrg(bool withheader = false, set<const DataTable*>* handled = nullptr) const;
		Strg toStrg() const;///only work as a sealed version for Strg toStrg(bool withheader = false, set<const DataTable*>* handled = nullptr) const;
		//DataTable& clone(DataTablePtr ar);
		~DataTable() {}
	};


	/**
	 *
	 */
	 //	void DELETE_AR(DataTablePtr ptr) {
	 //		if (!ptr) {
	 //			return;
	 //		}
	 //		//ptr->onHeap = false;
	 //#if debug
	 //		{
	 //			static int ignorecount = 0;
	 //			std::cout << "DELETE_AR(DataTablePtr)" << " ARStructFKA:["
	 //				<< ptr->scopeStructureFKA.toStrg() << "]" << "\tignorecount:["
	 //				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
	 //				<< ":0" << ")" << std::endl;
	 //		}
	 //#endif
	 //		--(ptr->referenceCount);
	 //		if (ptr->referenceCount <= 0) {
	 //			ptr->heapObjSet.erase(ptr);
	 //			for (auto& fkad : ptr->argdmap) {
	 //				if (fkad.second.dataFlagBit == S_AR) {
	 //					if (fkad.second.content_ar
	 //						&& fkad.second.content_ar->onHeap == true) {
	 //#if debug
	 //							{
	 //								static int ignorecount = 0;
	 //								std::cout << "DELETE_AR(DataTablePtr)" << " ARStructFKA:["
	 //									<< ptr->scopeStructureFKA.toStrg() << "]"
	 //									<< "\tignorecount:[" << ignorecount++ << "\t]("
	 //									<< __FILE__ << ":" << __LINE__ << ":0" << ")"
	 //									<< std::endl;
	 //							}
	 //#endif
	 //							DELETE_AR(fkad.second.content_ar);
	 //#if debug
	 //							{
	 //								static int ignorecount = 0;
	 //								std::cout << "DELETE_AR(DataTablePtr)" << " ARStructFKA:["
	 //									<< ptr->scopeStructureFKA.toStrg() << "]"
	 //									<< "\tignorecount:[" << ignorecount++ << "\t]("
	 //									<< __FILE__ << ":" << __LINE__ << ":0" << ")"
	 //									<< std::endl;
	 //							}
	 //#endif
	 //					}
	 //				}
	 //
	 //			}
	 //			for (auto& sd : ptr->sdmap) {
	 //				if (sd.second.dataFlagBit == S_AR) {
	 //					if (sd.second.content_ar
	 //						&& sd.second.content_ar->onHeap == true) {
	 //#if debug
	 //							{
	 //								static int ignorecount = 0;
	 //								std::cout << "DELETE_AR(DataTablePtr)" << " ARStructFKA:["
	 //									<< ptr->scopeStructureFKA.toStrg() << "]"
	 //									<< "\tignorecount:[" << ignorecount++ << "\t]("
	 //									<< __FILE__ << ":" << __LINE__ << ":0" << ")"
	 //									<< std::endl;
	 //							}
	 //#endif
	 //							DELETE_AR(sd.second.content_ar);
	 //#if debug
	 //							{
	 //								static int ignorecount = 0;
	 //								std::cout << "DELETE_AR(DataTablePtr)" << " ARStructFKA:["
	 //									<< ptr->scopeStructureFKA.toStrg() << "]"
	 //									<< "\tignorecount:[" << ignorecount++ << "\t]("
	 //									<< __FILE__ << ":" << __LINE__ << ":0" << ")"
	 //									<< std::endl;
	 //							}
	 //#endif
	 //
	 //					}
	 //				}
	 //			}
	 //			for (auto& fd : ptr->frmap) {
	 //				if (fd.second.argFlag == S_AR) {
	 //					if (fd.first.ref_ar && fd.first.ref_ar->onHeap == true) {
	 //#if debug
	 //						{
	 //							static int ignorecount = 0;
	 //							std::cout << "DELETE_AR(DataTablePtr)" << " ARStructFKA:["
	 //								<< ptr->scopeStructureFKA.toStrg() << "]"
	 //								<< "\tignorecount:[" << ignorecount++ << "\t]("
	 //								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
	 //								<< std::endl;
	 //						}
	 //#endif
	 //						DELETE_AR(fd.first.ref_ar);
	 //#if debug
	 //						{
	 //							static int ignorecount = 0;
	 //							std::cout << "DELETE_AR(DataTablePtr)" << " ARStructFKA:["
	 //								<< ptr->scopeStructureFKA.toStrg() << "]"
	 //								<< "\tignorecount:[" << ignorecount++ << "\t]("
	 //								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
	 //								<< std::endl;
	 //						}
	 //#endif
	 //
	 //					}
	 //				}
	 //			}
	 //			if (ptr->type == SYSB) {
	 //				for (auto& qry : ptr->queryARList) {
	 //					if (qry && qry->type == SYSB) {
	 //#if debug
	 //						{
	 //							static int ignorecount = 0;
	 //							std::cout << "DELETE_AR(DataTablePtr)" << " ARStructFKA:["
	 //								<< ptr->scopeStructureFKA.toStrg() << "]"
	 //								<< "\tignorecount:[" << ignorecount++ << "\t]("
	 //								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
	 //								<< std::endl;
	 //						}
	 //#endif
	 //						DELETE_AR(qry);
	 //#if debug
	 //						{
	 //							static int ignorecount = 0;
	 //							std::cout << "DELETE_AR(DataTablePtr)" << " ARStructFKA:["
	 //								<< ptr->scopeStructureFKA.toStrg() << "]"
	 //								<< "\tignorecount:[" << ignorecount++ << "\t]("
	 //								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
	 //								<< std::endl;
	 //						}
	 //#endif
	 //					}
	 //				}
	 //			}
	 //			DELETE_AR(ptr->parentAR);
	 //			DELETE_AR(ptr->returnAR);
	 //#if debug
	 //			{
	 //				static int ignorecount = 0;
	 //				std::cout << "DELETE_AR(DataTablePtr)" << " ARStructFKA:["
	 //					<< ptr->scopeStructureFKA.toStrg() << "]"
	 //					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
	 //					<< ":" << __LINE__ << ":0" << ")" << std::endl;
	 //			}
	 //#endif
	 //			if (ptr->onHeap) {
	 //			
	 //				delete ptr;
	 //			}
	 //#if debug
	 //			{
	 //				static int ignorecount = 0;
	 //				std::cout << "DELETE_AR(DataTablePtr)" << " ARStructFKA:["
	 //					<< ptr->scopeStructureFKA.toStrg() << "]"
	 //					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
	 //					<< ":" << __LINE__ << ":0" << ")" << std::endl;
	 //			}
	 //#endif
	 //			ptr = nullptr;
	 //			return;
	 //		}
	 //	}
	 //	//reduce the datatable reference of a class instance count by 1, if to zero, delete it if it is on heap, else waiting for its destructor working. 
	 //	void DELETE_SYSB(DataTablePtr ptr) {
	 //		if (!ptr) {
	 //			return;
	 //		}
	 //		if (ptr->type != SYSB) {
	 //			DELETE_AR(ptr);
	 //		}
	 //		ptr->heapObjSet.erase(ptr);
	 //		--(ptr->referenceCount);
	 //		if (ptr->referenceCount <= 0) {
	 //			for (auto& fkad : ptr->argdmap) {
	 //				if (fkad.second.dataFlagBit == S_AR) {
	 //					if (fkad.second.content_ar
	 //						&& fkad.second.content_ar->onHeap == true) {
	 //						DELETE_AR(fkad.second.content_ar);
	 //
	 //					}
	 //				}
	 //
	 //			}
	 //			for (auto& sd : ptr->sdmap) {
	 //				if (sd.second.dataFlagBit == S_AR) {
	 //					if (sd.second.content_ar
	 //						&& sd.second.content_ar->onHeap == true) {
	 //						DELETE_AR(sd.second.content_ar);
	 //
	 //					}
	 //				}
	 //			}
	 //			for (auto& qry : ptr->queryARList) {
	 //				if (qry && qry->type == SYSB) {
	 //					DELETE_SYSB(qry);
	 //				}
	 //			}
	 //
	 //			if (ptr->onHeap) {
	 //				delete ptr;
	 //			}
	 //			ptr = nullptr;
	 //			return;
	 //		}
	 //	}
		 /**
		  * @attention 赋值，引用数+1，左操作数必须不为空,
		  * the original memory object of p1's reference count is reduced by 1
		  * @param d1
		  * @param d2
		  * @return
		  */
		  //DataTablePtr AS(DataTablePtr& d1, DataTablePtr d2) {
		  //	if (d1 && d1->onHeap) {
		  //		if (d1->type == SYSB) {
		  //			DELETE_SYSB(d1);

		  //		}
		  //		else {

		  //			DELETE_AR(d1);
		  //		}
		  //	}
		  //	if (d2 && d2->onHeap) {
		  //		++(d2->referenceCount);
		  //	}
		  //	d1 = d2;
		  //	return d1;

		  //}


	class ReferenceData {
	public:
		DataPtr refData = nullptr;
		deque<Arg> referencePath;
		DataTablePtrW rootAR = nullptr;
		FKA scopeFKA;

		Data& getData();

		DataTablePtrW const getDestAR();

		Arg getArg() const;

		void clear();

		bool operator==(const ReferenceData& refdata) const;

		ReferenceData& operator=(const ReferenceData& referenceData) {
			this->refData = referenceData.refData;
			this->referencePath = referenceData.referencePath;
			this->rootAR = referenceData.rootAR;
			this->scopeFKA = referenceData.scopeFKA;
			return *this;
		}

		Strg toStrg();
		~ReferenceData() {
#if debug
			{
				cout << "~ReferenceData()" << " " << "\tignorecount:[" << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
		}

	};
	class ReferenceTable {
	public:
		Intg mode = MODE_SCANNING;

		//Addr 为Y_Dz对应的FKA，亦即变量名
		multimap<FKA, ReferenceData> argfka_ref_map;
		//    //rootAR用于检索、删除、修改引用的实例。不同rootAR中的引用不一定相同
		//    DataTable *rootAR = nullptr;
		//    void setRootAR(const DataTable *rootAR) {
		//        this->rootAR = rootAR;
		//    }

		void addRefData(DataTablePtr rootAR, CodeTableHash* codeTableHash_,
			const FKA& currentCodeFKA_, const Intg& pos);

		void addRefData(DataTablePtr rootAR, CodeTableHash* codeTableHash_,
			const FKA& currentCodeFKA_, Arg argref);

		void clear();

		void erase(const Arg& arg);

		Arg operator[](const Arg& y_dz_arg) const;

		Data& getDataRef(const Arg& y_dz_arg);

		ReferenceData getReferenceData(const Arg& y_dz_arg);

	};
	/**
	 * @var globalRefTable 全局均仅使用此一个引用表，引用只在执行和预执行时被确定，因此此表也在这两个阶段被使用
	 * @attention 引用将被转换为所指向的参数进行比较，
	 * 但此函数本着通用函数不处理复杂逻辑的准则，不会自动转换引用
	 */
	static ReferenceTable globalRefTable;







	typedef Arg ArgDec;
	inline ArgDec argDecnull = ArgDec();
	class DecComparator {
	public:
		Scanner* scanner;
		CodeTableHash* codeTableHash;
		DecComparator(Scanner* scanner_);

		map<ArgDec, ArgDec> am;                //argmap
		//const bool add(const ArgDec& arg, const ArgDec &argtf);
		map<FKA, FKA> cdm;                //codemap
		//const Addr getSameTrBranchLastArgAddr(const Addr& ad, const ArgDec& a);
		//const bool cmpDecCodeTree(const Addr & tr, const Addr & trTf);

		DecComparator& clear();

		bool add(const ArgDec& arg, const ArgDec& argtf);

		bool add(const FKA& fka1, const FKA& fka2);

		/**
		 * @attention 不比较非类型标志位
		 * @param cdFKA
		 * @param arg
		 * @return
		 */
		bool includeArg(const FKA& cdFKA, const ArgDec& arg) const;

		FKA getSameTrBranchLastArgAddr(const FKA& fka, const ArgDec& a);

		bool cmpDecCodeTree(const FKA& tr, const FKA& trTf);

	};
	/**
	 * 用于确定参数的可变性
	 */
	typedef Arg ArgPend;
	class PendPainter {
		set<ArgPend> changeableArgs;
		set<Arg> unchangeableArgs;
	public:
		Scanner* scanner = nullptr;
		CodeTableHash* codeTableHash = nullptr;
		PendPainter(Scanner* s_);

		[[deprecated("isn't needed")]]
		bool painted(const Arg& arg, const FKA& pendFKA);
		[[deprecated("isn't needed")]]
		bool paint(const Arg& arg, const FKA& pendFKA);
		bool paintAll();

	};

	typedef Arg ArgReal;
	class RealPainter {
		set<ArgReal> realArgs;
	public:
		Scanner* scanner = nullptr;
		CodeTableHash* codeTableHash = nullptr;
		RealPainter(Scanner* s_);
		[[deprecated("isn't needed")]]
		bool painted(const Arg& arg, const FKA& realFKA);
		[[deprecated("isn't needed")]]
		bool paint(const Arg& arg, const FKA& realFKA);
		bool paintAll();

	}
	;


	class CompatiblePainter {
		set<Arg> compatibleArgs;
	public:
		Scanner* scanner = nullptr;
		CodeTableHash* codeTableHash = nullptr;
		CompatiblePainter(Scanner* s_);
		[[deprecated("isn't needed")]]
		bool painted(const Arg& arg, const FKA& compatibleFKA);
		[[deprecated("isn't needed")]]
		bool paint(const Arg& arg, const FKA& compatibleFKA);

		bool paintAll();


	};
	class ConstraintPainter {
		map<Arg, set<Constraint>> arg_constr_map;
	public:
		Scanner* scanner = nullptr;
		CodeTableHash* codeTableHash = nullptr;
		ConstraintPainter(Scanner* s_);

		/**
		* can only extract: attr, attr(var),attr(var1,var2,...).no operator is allowed in var.
		*
		*/
		[[deprecated("Moved to CodeTable")]]
		void extractConstraint(const FKA& constraintFKA);
		[[deprecated("isn't needed")]]
		bool painted(const Arg& arg, const FKA& constraintFKA);
		[[deprecated("isn't needed")]]
		bool paint(const Arg& arg, const FKA& constraintFKA);

		bool paintAll();


	};
	/**
	 * @brief 输出所有目录文件夹下所有名字为fileName的路径
	 * @param path_
	 * @param fileKey   withSuffix//是否有后缀
	 * @return
	 */
	using namespace filesystem;
	vector<path> getFilePath(const path& path_, const string& fileKey,
		const bool& withSuffix);


	Addr stackToAddr(const vector<Addr>& scopeStack);


}

#endif /* COOLANG_VM_SCAN_HPP_ */