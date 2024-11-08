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

#ifndef COOLANG_VM_INTEGRATE_HPP_
#define COOLANG_VM_INTEGRATE_HPP_

#include "coolang_vm_scan.hpp"
#include <set>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <functional>
#include <random>
#include "json.hpp"
#include "pipeCommunication.hpp"
#include "coolang_utility.hpp"



#define MODE_DEFAULT 0   //不提供cdtdes，直接整合到cdtmain中
#define MODE_INTEGRATE 0   //不提供cdtdes，直接整合到cdtmain中
#define MODE_SEPARATE 1  //提供cdtdes，只对cdtdes匹配，不将cdtdes整合到cdtmain中

using namespace std;
using json = nlohmann::json;
namespace COOLANG {
	class ExpressionHandler {
	public:
		static inline Intg expressionhandler_counter = 0;
		Intg serialNumber = 0;
		Intg type = 0;
		CodeTable expr_cdt;
		Arg expr_arg;
		set<Arg> symbol_set;//used to store new symbols, elements must be symbols.
		bool open_low_bound_exist = false;
		FKA open_low_bound;
		bool open_high_bound_exist = false;
		FKA open_high_bound;
		bool close_low_bound_exist = false;
		FKA close_low_bound;
		bool close_high_bound_exist = false;
		FKA close_high_bound;
		map<Intg, FKA> subtree_searching_progress_recording_map;//serial number, last matched root fka (root to top)

		DataTablePtr ruleFunNameAR = nullptr; //this AR is the rule function name AR of the RULE FUNCTION. This AR will be delete after a rule is applied on the expression. This AR is created to store the value of the frmap in ruleFunBodyAR. Initialized in GroundingState::setMatchState ONLY RULE FUNCTION HAS NAME AR TO AVOID DIRECTLY CREATING TEMPORARY ARGS IN ITS PARENT AR DURING GROUNDING.
		DataTablePtr ruleFunBodyAR = nullptr; //each expressionhandler has its own ruleBodyAR， this AR is the function body AR of the RULE FUNCTION. This AR will be delete after a rule is applied on the expression.
		DataTablePtrW baseAR = nullptr; //The AR where the expr is located (the current AR before entering the rule function body).
		ExpressionHandler();

		ExpressionHandler(const CodeTable& expr_cdt_);

		ExpressionHandler(const OpenFKACDT& expr_fka_cdt_);

		ExpressionHandler(const OpenAddrCDT& expr_addr_cdt_);

		ExpressionHandler(const Arg& arg);

		void setAR(DataTablePtr ruleFunBodyAR, DataTablePtrW baseAR);

		FKA getMinLowBound();

		FKA getMaxHighBound();

		/*
		@param expr: the realexpr, used to provide subexpr to replace args or subtrees in "this" expr
		@param dt: used to provide frmap and values of local data.

		**/
		void flatten(ExpressionHandlerPtrW realexpr, DataTablePtrW dt, map<ArgFormal, ArgReal, ArgComparatorLT> frmap, CodeTableHash* cdth = nullptr, ScopeTableHash* scpth = nullptr);


		void replaceVar(const Arg& argreplace, ExpressionHandlerPtr subexpr);

		/*bool operator==(const ExpressionHandler& exprhandler)const {
			return this->type == exprhandler.type && this->expr_cdt == exprhandler.expr_cdt && this->expr_arg = exprhandler.expr_arg;

		}*/
		/**
		* clone from
		* don't copy serialNumber and subtree_searching_progress_recording_map. clone localAR, doesn't clone baseAR.
		*/
		void deepAssign(ExpressionHandlerPtr expr_);
		Strg toStrg() const;
		//won't output DataTable(AR), searching info
		Strg toBrief() const;


		~ExpressionHandler() {
			this->baseAR == nullptr;
		}
	};









	/**
	 * @param cdtref:函数名作用域中的树,仅用于与未绑定树进行匹配
	 * @param cdtfrom：将整合入cdtmid中的树，当integrate为否时，为函数名作用域内的树；为是时，则为函数体作用域返回的树
	 * @param cdtmid：储存在silo中，从silo中选择满足要求的函数表将其整合入cdtmain中
	 * @param cdtmain：一次代码表，完全整合后成为二次代码表
	 */

	class Matcher;
	class SingleTFIntegrator;
	class MultiTFIntegrator;
	inline CodeTable codeTablenull = CodeTable();







	/*DataPtr copy(DataPtr to, const Data& from) {

		to = DataPtr(new Data());
		*to = from;
		return to;
	}
	DataPtr copy(DataPtr to, const DataPtr const from) {
		to = DataPtr(new Data());
		*to = *from;
		return to;
	}*/



	/**
	 * @name findArgInAR
	 * @param arg
	 * @attention 不查询引用！！！！
	 * @return 查询范围只包括当前AR（包括frmap），考虑formalarg 标志位（formalarg为假直接返回否）
	 */
	bool findArgInAR(const DataTable& ar, const Arg& arg);



	typedef Addr AddrCmp;
	typedef Addr AddrRef;
	typedef ArgReal ArgCmp;
	typedef Arg ArgRef;
	//class MatchStateSilo;
	class MatchState;
	class StateMatrix;
	class GroundingState;
	/**
	 * 此类为状态矩阵
	 * 记录了grounding过程中作用于特定query的function的PCPs
	 * BDDB_o_t is the steps indicating stagnation or advancement
	 */
	class StateMatrix {
	public:
		deque<Strg> rule_domain_add_stack;
		deque<deque<Numb>> tfinfo_add_stack;
		StateMatrix() {
		}
		Intg n = 0; //this is the state of time n

		vector<Strg> knowledge_domain_list;
		vector<Intg> step_list; // 对应的file——class中的函数Grounding走到哪一步了，由相关函数PCP(process control prompt)的最后非0所在位置决定
		vector<Intg> BDDB_o_t_list; //相同阶段逗留步数
		NumVec2 matrix;
		Intg accumulated_steps = 0;

		StateMatrix& operator=(const StateMatrix& sm);

		set<Strg> getknowledgeDomains() const;
		/**
		 If knowledge_domain not found, return -1, else return corresponding step value.
		*/
		Intg getStep(const Strg& knowledge_domain) const;

		/**
		 *
		 * @param rule domain, file_class_name p1 函数所在文件名，p2 函数所在类名, only the most accurate values are recorded.(if a rule is in one class, only the filename& classname combined name will be recorded)
		 * @param tfinfo 即为 与函数绑定的info。注意当前只用info的第一层
		 * bool 1是否成功，int 2 BDDB_o_t的值（相同阶段逗留步数）, Numb 3 rp(reward provided by PCP), Intg 4 添加后，对应知识领域所处的step
		 */
		tuple<bool, Intg, Numb, Intg> add(const pair<Strg, Strg>& file_class_name,
			const TFInfoMat& tfinfo);

		Strg toStrg() const;


	};
	//void to_json(json &j, const Code &code) {
	//}
	//void to_json(json &j, const CodeTable &codeTable) {
	//}
	enum class MatchMode {
		RuleMatchingFormalRealBind = 1 << 0, //compare expression(with real args) and function name(with formal args), feature is allowing args of different Bs to bind.
		SubExprSearchingPlaceholderBind = 1 << 1, //search a subexpression(with/without placeholder) in a expression, in . feature is each node should intersect with corresponding nodes in constraint and data, bs(identifier name) and placeholder.  
		FunctionInvocationFormalRealBind = 1 << 2, //not supported by class Matcher. And no need to achieve it.
	}; //work in Matcher::add(arg1,arg2,arg3,arg4)
	class MatchState {

	public:
		MatchState() {
		}
		MatchMode md;
		DataTablePtrW dt = nullptr;
		CodeTableHash* cdth = nullptr;


		Addr addrcmporg;            //这个ms处理的astcmp最初是从哪里开始
		Addr addrreforg;            //这个ms处理的astref最初是从哪里开始
		CodeTable* cdtcmp = nullptr;
		CodeTable* cdtref = nullptr;
		vector<AddrCmp> addrcmpv;
		vector<Intg> argposcmpv;
		vector<AddrRef> addrrefv;
		vector<Intg> argposrefv;
		set<Arg, ArgComparatorLT> placeholders;


		map<ArgCmp, deque<ArgRef> > cram;
		map<ArgRef, pair<ArgCmp, AddrCmp> > rcam;
		map<AddrCmp, AddrRef> addrm;

		map<ArgRef, ArgCmp> pcam; // the first element in the pair is placeholderarg in subexpr, the next is the arg corresponding in the parent expr.
		MatchState(CodeTable* cdtcmp, const Addr& addrcmp, CodeTable* cdtref,
			const Addr& addrref, MatchMode md = MatchMode::RuleMatchingFormalRealBind, DataTablePtrW dt = nullptr, CodeTableHash* cdth = nullptr, set<Arg, ArgComparatorLT> placeholders = {});

		MatchState(const MatchState& ms);

		MatchState& operator=(const MatchState& ms);


		bool finished() const;

		void pop(AddrCmp& addrcmp_, Intg& argposcmp, Addr& addrref_,
			Intg& argposref);

		void push(const AddrCmp& addrcmp, const Intg& argposcmp,
			const AddrRef& addrref, const Intg& argposref);

		/**
		 * @brief 比较两个参数是否在事实上等价
		 * @param argcmp
		 * @param argref
		 * @return
		 */
		bool sameOneArg(const ArgCmp& argcmp, const ArgRef& argref) const;

		bool add(const AddrCmp& addrcmp, const AddrRef& addrref);

		/**
		 * @brief 这个函数用递归方式判断以argcmp1（rootaddr1）和argcmp2（rootaddr2）为根节点的两个子树的结构是否相同。
		 * 即子叶结点的（标识符类）参数相同（sameOneArg），子叶结点间的运算符也相同，
		 * 允许非子叶结点的（地址类）参数不同。
		 * @param root1
		 * @param root2
		 * @param rootaddr1
		 * @param rootaddr2
		 * @return
		 */
		bool sameTreeStruct(const ArgCmp& root1, const ArgCmp& root2,
			const AddrCmp& rootaddr1, const AddrCmp& rootaddr2);

		/**
		 * @brief 这个函数用以检查形参的一致性,即在添加argcmp与argref之前确保argref对应的argcmp为空。
		 * 即添加后每个形参(ArgRef)最多只能对应一个实参(ArgCmp)。
		 * 如果一个形参对应多个实参，那么这些多的实参是相同的参数，那么将删除未重复的（未被引用的树）树。
		 * 如果实参不相同则返回假，如果重复树被引用则不删除。
		 * @change 使用rcam
		 * @param
		 * @return
		 */
		bool checkArgRefConsistency(const ArgCmp& argcmp,
			const ArgRef& argref, const Addr& addrcmp, const Addr& addrref);

		/**
		 * @brief 此函数中，引用被转化为所指进行比较，但cram依然使用原引用作为键
		 * @param argcmp
		 * @param argref
		 * @param addrcmp
		 * @param addrref
		 * @return
		 */



		 /// <summary>
		 /// 
		 /// </summary>
		 /// <param name="argcmp"></param>
		 /// <param name="argref"></param>
		 /// <param name="addrcmp"></param>
		 /// <param name="addrref"></param>
		 /// <returns></returns>
		const bool add(const ArgCmp& argcmp, const ArgRef& argref,
			const Addr& addrcmp, const Addr& addrref);

		/**
		 * @name compareSingleCode
		 * @attention 引用（Y）会被转化为所指向的参数
		 * @brief compare CODE in different lines
		 * @param addrcmp
		 * @param cdcmp
		 * @param addrref
		 * @param cdref
		 * @return
		 */
		bool compareSingleCode(const AddrCmp& addrcmp, const Code& cdcmp,
			const AddrRef& addrref, const Code& cdref);

		Intg size() const;


	};

	/**
	 * @class GroundingState 这个类是储存训练数据的核心类！n时刻生成
	 * 这个类用于存储一个约束在grounding过程中的所有的Rule和Fact函数fn根据时间顺序t(0-n)排列的序列、每次被改变的根节点位置pn（数字），
	 * 并会储存变化前的约束对应代码片段cn， 注意，ctn应用fn会变成cn+1。同时，提供前往gsn-1（groundingstaten-1）的地址
	 * @attention 此类中所选择的rule、fact，statemat,以及神经网络预测的概率都是n-1时刻生成或决定的。带点动态规划的感觉，
	 * 先评估再具体执行
	 */
	 /**
	  * @用户输入两个cdt，由此类对两个cdt进行比较。其间使用addr，不使用fka
	  * 这个类是grounding以及函数调用过程的核心类之一，用于将一段代码（约束条件）与函数体（rule的head）进行对比，
	  * 生成形参实参对照表留在此类的实例中，传入后文进行处理。
	  * @funcion :
	  */
	class GroundingState {

	public:
		static inline Intg grounding_state_count = 0;//count how many grounding states has been created.
		static inline Intg getNewID() {
			return grounding_state_count++;
		}

		Intg id = 0;            //不做任何比较，只用于跟踪继承关系
		inline static set<const GroundingState*> garbage;
		typedef FKA TemplateFunctionFKA;

		const GroundingState* lastgs = nullptr;
		Numb policy = 0; //神经网络的期望  n时刻
		StateMatrix statemat;  // n时刻（已经整合tffka的tfinfomat）
		TemplateFunctionFKA tffka; //n时刻确定，n+1时刻应用
		bool rule = false; // tffka is corresponding to a rule function
		Intg bddb_o_t = 0; //此GroundingState被创建时已经在相同阶段停留的步数
	private:
		MatchState matchState; //n时刻确定，n+1时刻应用，储存着match tffka 时对应的信息，就不用再搞了
	public:
		inline const MatchState& getMatchState() const {
			return this->matchState;
		}
		bool exported = false; //此步骤是否已经输出为训练数据了，避免重复输出

		Numb BDDB_value = 0; //状态对应的总回报，总回报大的优先执行
		Intg position = 0; //要改变的节点位置 (root for changed AST)
		Intg succeed = -1; //此constraint是否成功grounding了， -1 invalid, 0 false, 1 true
		bool initialState = false; //是否是未进行ground的ast
		bool finalState = false; //是否是进行ground完毕的ast
		ExpressionHandlerPtr expr; // 约束/query对应的ast


		GroundingState(ExpressionHandlerPtr expr_, bool duplicate, const Numb& policy,
			const Intg& position_, const GroundingState* lastgs_);

		GroundingState(ExpressionHandlerPtr expr_, bool duplicate);
		GroundingState(bool assignNewID);
		GroundingState();


		void checkExprCdt();

		~GroundingState();

		set<Strg> getknowledgeDomains() const;//return{filename与file-classname的集合};

		void setGroundingResult(const Intg& succeed_ = false, const set<CodeTable, function<bool(const CodeTable&, const CodeTable&)>>* successCodeTableSet = nullptr) const;
		void setMatchState(const MatchState& ms_, TemplateFunctionTableHash* tfth, const FKA& tffka_);
		//此处的cdth/systh_用于在导出时查询信息用，不要在内部修改！
			/**
			 * @attention export GroundingState(except final states and initial states)
			 * @param ofs
			 * @param expected_exported_konwledge_domains, 期望导出的knowledgedomain类型（非此类型的gs indomain为false）
			 * @param problem_knowledge_domains, 该problem归属的knowledgedomain
			 * @param duplicated 是否允许重复输出
			 * @param export_succeed_ 是否输出成功的gs
			 * @param export_failed_ 是否输出失败的gs
			 * @param cdth 用来查类型名称
			 * @param systh_ 用来查询类型名称
			 * @return
			 */
		bool bufferTrainingData(const Strg& outputFileName, const set<Strg>& expected_exported_knowledge_domains, const set<Strg>& problem_knowledge_domains, const bool& duplicated,
			const bool& export_succeed_, const bool& export_failed_, const CodeTableHash* cdth,
			const SystemTableHash* systh_) const;

		json toJson(const CodeTableHash* cdth,
			const SystemTableHash* systh_, const bool& indomain) const;

		Strg toStrg() const;
		Strg toStrg(ScopeTableHash* scpth) const;

	};

	class GroundingStateComparatorLT {
	private:
	public:
		GroundingStateComparatorLT() = default;
		bool operator()(const GroundingState& lhs, const GroundingState& rhs) const;
	};

	//Intg GroundingState::grounding_state_count = 0;
	/**
	 * 此类是将约束与核心之一算法所在。其内部储存着针对特定ast的特定一node，所有可能的action。
	 * 树在这里会被征用、冻结与淘汰。
	 */
	template<class deprecated_class>
	class [[deprecated("this class seems to be useless now, and it's not suggested to introduce it again")]] MatchStateSilo {
	public:
		/**
		 * @param matchStateSiloMaxCapacity : max capacity of the tree a root node can match
		 */
		Intg matchStateSiloMaxCapacity = 100;
		multimap<TFInfoMat, MatchState> waitmsm;
		multimap<TFInfoMat, MatchState> resmsm;  //这里我失误了，在应用过程中，这个multimap最多只能有一个元素
		MatchState pop() {
			if (waitmsm.size() <= 0) {

				cout << "MatchStateSilo::pop err::no content, exit(-1)" << endl;

				assert(false); exit(-1);
			}
			MatchState ms(waitmsm.rbegin()->second);
			auto it = waitmsm.end();
			waitmsm.erase(--it);
			return ms;
		}
		void push(const TFInfoMat& organization, const MatchState& ms) {
			while ((Intg)waitmsm.size() > matchStateSiloMaxCapacity) {
				waitmsm.erase(waitmsm.begin());
			}
			if (ms.finished() == true) {
				resmsm.insert(pair<TFInfoMat, MatchState>(organization, ms));
			}
			else if (ms.addrm.size() > 0) {
				waitmsm.insert(pair<TFInfoMat, MatchState>(organization, ms));
			}
			else {

				cout << "MatchStateSilo push err::ms is null" << endl;

				assert(false); exit(-1);
			}
		}

		const Intg size() const {
			return waitmsm.size();
		}
		MatchStateSilo() {
		}
		void setMaxCapacity(Intg matchStateSiloMaxCapacity) {
			this->matchStateSiloMaxCapacity = matchStateSiloMaxCapacity;
		}

	};


	/**
	 * @attention 此类中，比较两个cdt，此步骤不使用fka作为code的索引，但采用fka作为函数检索的索引
	 * matcher操控MatchState类(相当于一个储存待匹配左支右支的stack)匹配两个树
	 * 所以MatchState是matcher的操作单位。
	 *
	 *
	 */
	class Matcher {
	private:
		MatchState ms;
	public:
		//MatchStateSilo mss;
		CodeTable* cdtcmp;
		MatchMode md;
		DataTablePtrW currentAR;



		CodeTable* cdtref = nullptr;
		TemplateFunctionTableHash* tfth = nullptr;
		AddrCmp addrcmp;
		AddrRef addrref;
		CodeTableHash* cdth = nullptr;
		set<Arg, ArgComparatorLT> placeholders = {};

		/// <summary>
		/// input two codetables, and two addrs, compare the trees rooting at the pointed addrs.
		/// 
		/// when in MatchMode::RuleMatchingFormalRealBind, argcmp is in executable code, argcmp is in function name.
		/// when in MatchMode::SubExprSearchingPlaceholderBind, argcmp is in parent expression, arg ref is in subexpr and may include placeholder.
		/// remember, this class is used to match whether an sub expression in cdt cmp has the structure defined in cdt ref.
		/// </summary>
		/// <param name="cdtcmp"></param>
		/// <param name="cdtref"></param>
		/// <param name="tfth"></param>
		/// <param name="addrcmp"></param>
		/// <param name="addrref"></param>
		/// <param name="md"></param>
		/// <param name="dt"></param>
		Matcher(CodeTable* cdtcmp, CodeTable* cdtref,
			TemplateFunctionTableHash* tfth, const AddrCmp& addrcmp,
			const AddrRef& addrref, MatchMode md = MatchMode::RuleMatchingFormalRealBind, DataTablePtrW dt = nullptr, CodeTableHash* cdth = nullptr, set<Arg, ArgComparatorLT> placeholders = {});

		/**
		 * @name compareMultiCode
		 * @brief compare two trees, 但是这个函数更专用，主要用在比较函数头和约束对应代码段。
		 * @param ms
		 * @return
		 */
		const bool compareMultiCode();
		/*
		@reset matchstate using the current args in Matcher
		*/
		void reset();

		const bool match();



		MatchState& getMatchState();

	};
	/**
	 * @name split 返回[min,max)(n==1)或(min,max)(n>1)
	 * @attention 特别的，当max为null时，直接在min之后顺延n个地址
	 * @param min
	 * @param max
	 * @param n
	 * @return
	 */
	vector<Addr> split(const Addr& min, const Addr& max, const Intg n);

	typedef Arg ArgReal;
	typedef Arg ArgForm;
	typedef Addr AddrFormal; //形参
	typedef Addr AddrReal; //实参
	/**
	 * @brief 此类用于将代码段指定位置的一段代码替换为另一段代码。插入的代码会进行参数替换和地址修改。
	 * 是banding与grounding的实际执行者。
	 * @attention 这个类处理两个cdt，一个为待匹配代码所在cdt，另一个为tf所在cdt，
	 *  同时传入在cdt中的addr，故code的检索不采用FKA，
	 *  但要用到scopeTableHash，其检索要用到FKA
	 */
	class SingleTFIntegrator {
	public:
		CodeTable* cdtdes = nullptr;
		CodeTable* cdtfrom = nullptr;
		ScopeTableHash* sth = nullptr;
		Arg attachmentRootNode; //the node where the cdtfrom links to the cdtdes.
		Addr mindes;
		Addr maxdes;
		Addr trfrom;
		FKA scopeFKAdes;

		map<ArgForm, ArgReal, ArgComparatorLT> am_ref_cmp;//variables with same name but different scope should be different here.
		map<AddrCmp, AddrRef> addrm;

		CodeTable cdtmid;
		/**
		 *
		 * @param cdtdes 此代码段的部分片段被替换
		 * @param cdtfrom 次代码段用于替换cdtdes代码片段所在代码段
		 * @param sth
		 * @param mindes 替换部分下限
		 * @param maxdes 替换部分上限 (mindes,maxdes)//mindes、maxdes可以为addrnull
		 * @param trfrom cdtfrom代码段中，用于替换cdtdes代码片段的语法树分支根地址
		 * @param am_cmp_ref 实参（cdtdes中参数）形参（cdtfrom中参数）对照表
		 * @param addrm cdtdes的addrdq中被替换的code的addr与用于替换的代码片段的对照表。
		 如果此表为空， 说明替换的是一个完整的语法树（mindes到maxdes之间的code都会被删除，不会主动将cdtfrom的根节点拼接到cdtdes上）
		 如果此表不为空， 说明替换的是位于地址（mindes到maxdes之间）语法树之间的，由addrm确定的子树，则只删除子树部分，并会将子树根节点对接到parent tree上
		 */
		SingleTFIntegrator(CodeTable* cdtdes, CodeTable* cdtfrom,
			ScopeTableHash* sth, const Addr& mindes, const Addr& maxdes,
			const Addr& trfrom, const map<ArgReal, deque<ArgForm> >& am_cmp_ref,
			const map<AddrReal, AddrFormal>& addrm);








		/**
		 *
		 * @param code 被修改的code
		 * @param scopeFKAdes 被修改code所在scopefka
		 * @param
		 * @return codeFKAdes  被修改code所生成的新的addr
		 */
		Addr replaceLine(CodeTable& cdt, Intg pos, const FKA& scopeFKAdes,
			const Addr& mindes, const Addr& maxdes);

		bool replaceVar();

		bool integrate();


	};

	/**
	 * @brief 此函数用于将语法树中不被访问到的子树删除。
	 * 删除的原则是：如果一行代码的四元式结果参数位在语法树中不被访问到，那么就删除此行代码
	 * @param cdt
	 */
	void deleteInaccessibleSubtree(CodeTable& cdt);

	void unfoldRingStruct(CodeTable& cdt);

	class GroundingStateSilo;
	typedef GroundingStateSilo CodeTableSilo;
	/**
	 * @CodeTabletSilo（GroundingStateSilo）是核心类，
	 * 其内部储存着一次grounding过程中所有的树，并负责，需要修改其接口，不仅提供organization（BDDB_value）
	 * 而且还要提供序列，指引Matcher和MultiTFIntegrator按顺序进行节点优先匹配。
	 *
	 */
	class GroundingStateSilo {

	public:
		Setting* setting = nullptr;
		Intg groundingStateSiloMaxCapacity = 300;
		bool siloEmpty = false;
		ScopeTableHash* scpth = nullptr;

		const bool (*onIntegrateFinish)(pair<Numb, GroundingState> p) = nullptr;

		//    Intg currentSiloTurn = 0;
		//    Intg pos = -1;

		//    std::multimap<Numb, GroundingState>::reverse_iterator posrit;
		//    multimap<Numb, GroundingState> silo1;
		//    multimap<Numb, GroundingState> silo2;
		//    multimap<Numb, GroundingState> resultsilo;
		//    multimap<Numb, GroundingState> *currentSilo = &silo1;  //GA without PE
		//    multimap<Numb, GroundingState> *nextSilo = &silo2; //GA without PE
		//    multimap<Numb, GroundingState> *activeSilo = &silo1;  //GA with PE
		//    multimap<Numb, GroundingState> *garbageSilo = &silo2; //GA with PE
		multimap<Numb, const GroundingState*> activeSilo;  //GA with PE
		multimap<Numb, const GroundingState*> garbageSilo; //GA with PE
		multimap<Numb, const GroundingState*> abortedSilo; //GA with PE
		multimap<Numb, const GroundingState*> resultSilo; //GA with PE
		set<GroundingState, GroundingStateComparatorLT> siloAll; // store all the groundingstate instances
		inline static map<Strg, json> trainingDataBuffer; //filename - json to write

	public:
		//    /**
		//     * 交换 currentsilo与nextsilo， 用于BFS的遗传算法,无PE
		//     * @return
		//     */
		//    const bool switchSilo() {
		//        multimap<Numb, GroundingState> *transitSilo = currentSilo;
		//        currentSilo->clear();
		//        currentSilo = nextSilo;
		//        nextSilo = transitSilo;
		//        currentSiloTurn++;
		//        {
		//
		////#if debug
		//
		//            if (debugMode) {
		//                static int ignorecount = 0;
		//                cout << "switchSilo()" << " \n" << "currentSiloTurn:["
		//                        << currentSiloTurn << "\t]\t" << "map size:["
		//                        << currentSilo->size() << "\t]\n" << "\tignorecount:["
		//                        << ignorecount++ << "\t](" << __FILE__<<":"
		//                        << __LINE__ << ":0" << ")\n";
		//
		//                << endl;
		//                for (const auto &p : *currentSilo) {
		//                    cout << json( { "PE", });
		//                    cout << p.second.toStrg() << endl;
		//
		//                }
		//            }
		////#endif
		//        }
		//        return true;
		//    }
		//public:
		//    /**
		//     * 用于无PE AG
		//     *@attention 自动切换silo,如果currentsilo和nextsilo同时为空，则将siloEmpty置为真
		//     * @return pair<组织度，cdt>
		//     */
		//    pair<Numb, GroundingState> getNext() {
		//        pos++;
		//        if (pos == 0 && pos < (Intg) currentSilo->size()) {
		//            posrit = currentSilo->rbegin();
		//            return pair<Numb, GroundingState>(posrit->first, posrit->second);
		//        } else if (pos > 0 && pos < (Intg) currentSilo->size()) {
		//            ++posrit;
		//            return pair<Numb, GroundingState>(posrit->first, posrit->second);
		//        } else {
		//            pos = -1;
		//            switchSilo();
		//            if (currentSilo->size() == 0) {
		//
		//                cout
		//                        << "GroundingStateSilo::getNext() , silo empty, no more candidate for matching, match failed!!! line:["
		//                        << __LINE__ << "]" << endl;
		//
		//                siloEmpty = true;
		//                return pair<Numb, GroundingState>(
		//                        std::numeric_limits<double>::lowest(), GroundingState());
		//            }
		//            return getNext();
		//        }
		//
		//    }
		//    const bool add(const TFInfoMat organization, CodeTable &cdt,
		//            const vector<FKA> &integrateProcess) {
		//        {
		//#if debug
		//            static Intg ignorecount = 0;
		//            cout << "GroundingStateSilo::add in ,ignorecount:[" << ignorecount++
		//                    << "]"
		//                            "\n\t organization:[" << organization << "]"
		//                            "\n\t cdt:[" << cdt.toStrg() << "]" << endl;
		//#endif
		//
		//        }
		//        deleteInaccessibleSubtree(cdt);
		//        unfoldRingStruct(cdt);
		//
		//        Intg count = nextSilo->count(organization);
		//        auto it = nextSilo->find(organization);
		//        for (Intg i = 0; i < count && it != nextSilo->end(); ++i, ++it) {
		//            CodeTable &cdtexist = it->second.codeTable;
		//            if (cdtexist.addrdq.size() == cdt.addrdq.size()) {
		//                Matcher m(const_cast<CodeTable*>(&cdt),
		//                        const_cast<CodeTable*>(&cdtexist), nullptr,
		//                        cdt.addrdq.back(), cdtexist.addrdq.back());
		//                if (m.match() == true) {
		//                    bool same = true;
		//                    for (auto &it : m.mss.resmsm.begin()->second.cram) {
		//                        if (it.second.size() != 1
		//                                || (it.first == it.second[0]) == false) {
		//                            same = false;
		//                            break;
		//                        }
		//
		//                    }
		//                    if (same) {
		//                        {
		//#if debug
		//                            static int ignorecount = 0;
		//                            cout << "add(const int, const CodeTable&)"
		//                                    << "\ttwo trees the same\n"
		//                                    << "tree1(cmp):\n" << cdt.toStrg() << "\n"
		//                                    << "tree2(exist):\n" << cdtexist.toStrg()
		//
		//                                    << "\tignorecount:[" << ignorecount++
		//                                    << "\t](" << __FILE__<<":" << __LINE__
		//                                    << ":0" << ")" << endl;
		//#endif
		//                        }
		//                        return true;
		//                    }
		//
		//                }
		//            }
		//
		//        }
		//
		//        while ((Intg) nextSilo->size() > groundingStateSiloMaxCapacity) {
		//            nextSilo->erase(nextSilo->begin());
		//        }
		//
		//        Intg s = cdt.addrdq.size();
		//        if (s == 0) {
		//
		//            cout << "GroundingStateSilo::add err, cannot add null" << endl;
		//
		//        }
		//        Intg pos = 0;
		//        while (pos < s) {
		//            if (cdt.codedq[pos].assemblyFormula.flagBit.tfFKA == fkanull) {
		//
		//                nextSilo->insert(
		//                        pair<Intg, GroundingState>(organization,
		//                                GroundingState(cdt, integrateProcess)));
		//                while ((Intg) nextSilo->size() > groundingStateSiloMaxCapacity) {
		//                    nextSilo->erase(nextSilo->begin());
		//                }
		//                return true;
		//            }
		//            pos++;
		//        }
		//
		//        {
		//#if debug
		//            static Intg ignorecount = 0;
		//            cout << "GroundingStateSilo::add resultSilo.insert ,ignorecount:["
		//                    << ignorecount++ << "]"
		//                            "\n\t organization:[" << organization << "]"
		//                            "\n\t cdt:[" << cdt.toStrg() << "]" << endl;
		//#endif
		//
		//        }
		//        if (debugMode) {
		//
		//            static Intg ignorecount = 0;
		//            cout << "GroundingStateSilo::add() match success!!! ignorecount:["
		//                    << ignorecount++ << "]"
		//                            "\nkey:\t" << organization << "\nvalue:\n"
		//                    << cdt.toStrg() << endl;
		//
		//        }
		//        resultsilo.insert(
		//                pair<Intg, GroundingState>(organization,
		//                        GroundingState(cdt, integrateProcess)));
		//        if (onIntegrateFinish != nullptr) {
		//            (*onIntegrateFinish)(
		//                    pair<Intg, GroundingState>(organization,
		//                            GroundingState(cdt, integrateProcess)));
		//
		//        }
		//        {
		//#if debug
		//            static Intg ignorecount = 0;
		//            cout << "GroundingStateSilo::add out ,ignorecount:[" << ignorecount++
		//                    << "]"
		//                            "\n\t organization:[" << organization << "]"
		//                            "\n\t cdt:[" << cdt.toStrg() << "]" << endl;
		//#endif
		//
		//        }
		//        return true;
		//    }
			/**
			 *
			 * @return return the BDDB_value n and groundingstate n
			 */
		pair<Numb, const GroundingState*> getNext();

		/**
		 * @brief get BDDB_value n and groundingstate n, by function vector n, codeTable n and grounding state n-1.
		 * then insert the pair<BDDB_value n, grounding state n> into activesilo.
		 * codeTable n is yielded outside this function.
		 * this function is focusing on
		 * @param tffka n
		 * @param filekey_stage set the stage(step) of a certain knowledge domain(bound with filekey) as a certain value
		 * @param organization function vector n
		 * @param position, the position to apply function
		 * @param policy, the position that nn suggests to apply a function to.
		 * @param expr_ n
		 * @param ms n
		 * @param confidenceCoefficient n
		 * @param lastGroundingState n-1 groundingState
		 * @param biasCoefficient: an exceptionally small number, used when two grounding states share a same BDDB_value, the grounding state with bigger biasEnergy(biasCoefficient*position) tackled first.
		 * to be exactly, the grounding state whose position is smaller, or may closer to root in AST, is tackled preferentially.
		 * @return
		 */
		bool add(const pair<Strg, Strg>& file_class_name, const pair<Strg, Intg>& filekey_stage, TemplateFunctionTableHash* tfth, const FKA& tffka,
			const TFInfoMat& organization, const Intg& position,
			const Intg& domain_resolved, const bool& indomain,
			const bool& succeed, const Numb& ac_policies, const Numb& policy, const Numb& ac_advance, const Numb& advance, const Numb& ac_rpsign, const Numb& rpsign, const Numb& ac_rpval, const Numb& rpval, const Numb& ac_rule, const bool& rule, const Numb& ac,
			const MatchState& ms, ExpressionHandlerPtr expr_, bool duplicate_expr_,
			const GroundingState* lastGroundingState, const Numb& biasCoefficient = 1e-10);

		/**
		 *
		 * @param organization
		 * @param gs
		 * @param onIntegrateFinish
		 * @return
		 */
		GroundingStateSilo& reset(ExpressionHandlerPtr expr_, bool duplicate_expr_,
			const bool (*onIntegrateFinish)(GroundingStateSilo* p));

		//    GroundingStateSilo(const pair<Intg, GroundingState> &p,
		//            const bool (*onIntegrateFinish)(pair<Intg, GroundingState> p)) {
		//        currentSilo->insert(p);
		//        this->onIntegrateFinish = onIntegrateFinish;
		//    }
		GroundingStateSilo(Setting* setting, ScopeTableHash* scpth);

		void setMaxCapacity(Intg groundingStateSiloMaxCapacity);
		/// <summary>
		/// get all knowledge domains applied during the grounding process (regardless of the grounding result)
		/// </summary>
		/// <returns></returns>
		set<Strg> getKnowledgeDomains();
		/// <summary>
		/// the outputFileName is not a path. it is a file name like xxxx.json
		/// </summary>
		/// <param name="outputFileName"></param>
		/// <param name="duplicated"></param>
		/// <param name="only_succeed"></param>
		/// <param name="cdth"></param>
		/// <param name="systh_"></param>
		void bufferDataset(const Strg& outputFileName, const set<Strg>& knowledge_domains, const bool& duplicated,
			const bool& export_succeed, const bool& export_failed, const CodeTableHash* cdth,
			const SystemTableHash* systh_);
		static bool flush();
	};
	/**
	 * @this class is used to wrap and unwrap messages from NeuralNetworkWizard
	 */
	class MessageWrapper {
		const CodeTableHash* cdth = nullptr;
		const SystemTableHash* systh = nullptr;
	public:
		MessageWrapper(const CodeTableHash* cdth_, const SystemTableHash* systh_);

		static Strg wrapMessage(const Strg& order_,
			const map<set<Strg>, Strg>& training_data_map);

		Strg wrapMessage(const Strg& order_, const set<Strg>& file_class_name_set,
			const GroundingState& gs);

		json unwrapMessage(const Strg& reply_);


	};
	class Executor;
	//execute code snippet with this class
	//	class GroundingExecutor :Executor {
	//		CodeTableHash* cdth = nullptr;
	//		TemplateFunctionTableHash* tfth = nullptr;
	//		ScopeTableHash* scpth = nullptr;
	//		SystemTableHash* systh = nullptr;
	//		DataTablePtrW currentAR = nullptr;
	//
	//	public:
	//
	//		GroundingExecutor(CodeTableHash* cdth,
	//			TemplateFunctionTableHash* tfth,
	//			ScopeTableHash* scpth,
	//			SystemTableHash* systh) {
	//			this->cdth = cdth;
	//			this->tfth = tfth;
	//			this->scpth = scpth;
	//			this->systh = systh;
	//		}
	//
	//
	//		/*
	//		The returned expressionhandler should not be directly operated on. You should copy useful elements in it.
	//		*/
	//		ExpressionHandlerPtr invokeExprFunction(const map<Arg, Arg>& frmap, FKA function, ExpressionHandlerPtr expr) {
	//
	//
	//		}
	//		//
	//		void executeLatestGroundedTree(DataTable* currentAR, const FKA& currentFKA) {
	//
	//		}
	//
	//		void FlattenExpression(Arg arg) {
	//
	//
	//
	//		}
	//
	//		void execute(ExpressionHandlerPtr expr) {
	//			if (currentAR->)
	//				if ((*tfth)[matchedTFFKA].integrate == true) {
	//
	//					CodeTable& cdtmid = (*cdtdes);
	//
	//					//以下代码将操作匹配到的 函数的返回的表达式储存到cdtfrom中
	//					CodeTable cdtFunReturn;
	//					//MultiTFIntegrator会认为以下几种类型的代码不属于语法树。其不会被用来替换原代码段
	//					vector<Intg> exclude_types{ COOL_QS, COOL_QE, COOL_C, COOL_ME, COOL_FUN, COOL_R, COOL_FIL, COOL_LIN, EXPRQE, EXPRQS };
	//
	//					const FKA& returnHandle =
	//						(*scpth)[(*tfth)[matchedTFFKA].bodyScopeFKA].returnHandle[0];
	//
	//					const FKA&& aboveCodeFKA = cdth->getAboveCodeFKA(returnHandle);
	//
	//					bool&& notexclude = (find(exclude_types.begin(),
	//						exclude_types.end(),
	//						(*cdth)[aboveCodeFKA].assemblyFormula.flagBit.type)
	//						== exclude_types.end());
	//
	//					if (notexclude) {
	//
	//						GroundingExecutor ge(this->cdth, this->);
	//
	//
	//						//cdtFunReturn =
	//						//	cdth->copyTree(
	//						//		cdth->getLast(COOL_M,
	//						//			(*scpth)[(*tfth)[matchedTFFKA].bodyScopeFKA].returnHandle[0]));
	//
	//
	//
	//					}
	//					else {
	//						{
	//#if debug
	//							static int ignorecount = 0;
	//							cout << "GenerateCdtByGroundingState()" << "no tree found"
	//								<< "\tignorecount:[" << ignorecount++ << "\t]("
	//								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
	//								<< endl;
	//#endif
	//						}
	//					}
	//					for (Code& cd : cdtFunReturn.codedq) {
	//						cd.assemblyFormula.flagBit.tfFKA = fkanull;
	//						cd.assemblyFormula.flagBit.functionHandleFlag = false;
	//
	//					}
	//
	//					Addr maxdes;
	//					if (pos < (Intg)(cdtmid.addrdq.size() - 1)) {
	//
	//						maxdes = cdtmid.addrdq[pos + 1];
	//					}
	//					else {
	//						if (maxdeslimit.fileKey == cdtmid.fileKey) {
	//
	//							maxdes = maxdeslimit.addr;
	//						}
	//						else {
	//							maxdes = addrnull;
	//						}
	//					}
	//
	//					//
	//
	//					MatchState& ms = gs->matchState;
	//					CodeTable cdtmid_clone(cdtmid);
	//					Intg posmindes = pos;
	//					Addr mindes = cdtmid_clone.addrdq[posmindes];
	//
	//					while (ms.addrm.find(mindes) != ms.addrm.end()) {
	//						if (posmindes >= 0) {
	//							mindes = cdtmid_clone.addrdq[posmindes--];
	//						}
	//						else {
	//							if (mindeslimit.fileKey == cdtmid_clone.fileKey) {
	//
	//								mindes = mindeslimit.addr;
	//							}
	//							else {
	//								mindes = addrnull;
	//							}
	//							break;
	//						}
	//
	//					}
	//
	//					if (cdtFunReturn.addrdq.size() > 0) {
	//						SingleTFIntegrator stfi(&cdtmid_clone, &cdtFunReturn, scpth,
	//							mindes, maxdes, cdtFunReturn.addrdq[0], ms.cram,
	//							ms.addrm);
	//
	//						stfi.integrate();
	//
	//					}
	//					else {
	//						//todo:当cdtFunReturn内储存代码长度为零时，说明返回表达式是一个参数。
	//						//在cdtmid中寻找与所匹配函数名称（cdtref）的最后一行的结果相同的参数，
	//						//将其替换为返回的参数。
	//						Addr& addrcmp = addrnode;
	//						map<ArgForm, ArgReal> frmap;
	//						map<ArgReal, deque<ArgRef> >& rfmap = ms.cram;
	//						for (auto iter = rfmap.begin(); iter != rfmap.end(); iter++) {
	//							for (const ArgForm& aform : iter->second) {
	//								frmap.insert(
	//									pair<ArgForm, ArgReal>(aform, iter->first));
	//							}
	//
	//						}
	//						Arg& argBeReplaced = frmap[Arg(Bs, "ans")];
	//						FKA& returnHandleFKA =
	//							(*scpth)[(*tfth)[matchedTFFKA].bodyScopeFKA].returnHandle[0];
	//
	//						Arg& argBeUsedToReplace =
	//							frmap[(*cdtref)[returnHandleFKA.addr][0]];
	//
	//						const Addr& nextArgAddr =
	//							cdtmid_clone.getSameTrBranchNextArgAddr(addrcmp,
	//								argBeReplaced);
	//						if (nextArgAddr == addrnull) {
	//
	//							{
	//								static int ignorecount = 0;
	//								std::cerr
	//									<< "generateCdtByGroundingState(GroundingState*)"
	//									<< " nextArgAddr == addrnull, throw(-1) "
	//									<< "\tignorecount:[" << ignorecount++ << "\t]("
	//									<< __FILE__ << ":" << __LINE__ << ":0" << ")"
	//									<< std::endl;
	//							}
	//
	//							throw(-1);
	//						}
	//						for (auto& it : ms.addrm) {
	//
	//							cdtmid_clone.eraseCode(it.first, 0);
	//						}
	//						Code& codeReplace = cdtmid_clone[nextArgAddr];
	//						if (cdtmid_clone[nextArgAddr][0] == argBeReplaced) {
	//
	//							/*codeReplace.assemblyFormula.flagBit.arg1_flag =
	//								argBeUsedToReplace.argFlag;
	//							codeReplace.assemblyFormula.quaternion.arg1_fka =
	//								argBeUsedToReplace.arg_fka;
	//							codeReplace.assemblyFormula.quaternion.arg1_i =
	//								argBeUsedToReplace.arg_i;
	//							codeReplace.assemblyFormula.quaternion.arg1_s =
	//								argBeUsedToReplace.arg_s;
	//							codeReplace.assemblyFormula.flagBit.formalArgFlag[0] =
	//								argBeUsedToReplace.formalArg;
	//							codeReplace.assemblyFormula.flagBit.changeable[0] =
	//								argBeUsedToReplace.changeable;*/
	//							codeReplace.setArg(0, argBeReplaced, true);
	//
	//
	//						}
	//						else if (cdtmid_clone[nextArgAddr][1] == argBeReplaced) {
	//							codeReplace.setArg(1, argBeReplaced, true);
	//						}
	//						else if (cdtmid_clone[nextArgAddr][2] == argBeReplaced) {
	//							codeReplace.setArg(2, argBeReplaced, true);
	//						}
	//						else if (cdtmid_clone[nextArgAddr][3] == argBeReplaced) {
	//							codeReplace.setArg(3, argBeReplaced, true);
	//						}
	//
	//					}
	//#if debug
	//					{
	//						static int ignorecount = 0;
	//						std::cout << "(" << __FILE__ << ":" << __LINE__ << ":0"
	//							<< ")\tgenerateCdtByGroundingState(GroundingState*)"
	//							<< " return1 cdt:\n" << cdtmid_clone.toStrg()
	//							<< "\tignorecount:[" << ignorecount++ << "\t]"
	//							<< std::endl;
	//					}
	//#endif
	//					return cdtmid_clone;
	//
	//				}
	//				else if (matchedTFFKA.addr > addrnull
	//					&& matchedTFFKA.fileKey != FILEKEYBUILTIN) {
	//					//tf是返回运算值的函数
	//
	//					CodeTable& cdtmid = (*cdtdes);
	//
	//					CodeTable cdtFunAnnounce = get<0>(cdth->copyTree(
	//						(*tfth)[matchedTFFKA].templateHandleFKA));
	//
	//					Addr maxdes;
	//
	//					if (pos < (Intg)(cdtmid.addrdq.size() - 1)) {
	//						maxdes = cdtmid.addrdq[pos + 1];
	//					}
	//					else {
	//						if (maxdeslimit.fileKey == cdtmid.fileKey) {
	//							maxdes = maxdeslimit.addr;
	//						}
	//						else {
	//							maxdes = addrnull;
	//						}
	//					}
	//
	//					CodeTable cdtmid_clone(cdtmid);
	//					MatchState& ms = gs->matchState;
	//
	//					Intg posmindes = pos;
	//					Addr mindes = cdtmid_clone.addrdq[posmindes];
	//					do {
	//						--posmindes;
	//						if (posmindes < 0) {
	//
	//							if (mindeslimit.fileKey == cdtmid.fileKey) {
	//								mindes = mindeslimit.addr;
	//							}
	//							else {
	//								mindes = addrnull;
	//							}
	//							break;
	//						}
	//						mindes = cdtmid_clone.addrdq[posmindes];
	//
	//					} while (ms.addrm.find(mindes) != ms.addrm.end()
	//						|| (*cdtdes)[mindes].assemblyFormula.flagBit.execute == Du);
	//					SingleTFIntegrator stfi(&cdtmid_clone, &cdtFunAnnounce, scpth,
	//						mindes, maxdes, cdtFunAnnounce.addrdq[0], ms.cram,
	//						ms.addrm);
	//
	//					stfi.integrate();
	//
	//#if debug
	//					{
	//						static int ignorecount = 0;
	//						std::cout << "(" << __FILE__ << ":" << __LINE__ << ":0"
	//							<< ")\tgenerateCdtByGroundingState(GroundingState*)"
	//							<< " return2 cdt:\n" << cdtmid_clone.toStrg()
	//							<< "\tignorecount:[" << ignorecount++ << "\t]"
	//							<< std::endl;
	//					}
	//#endif
	//					return cdtmid_clone;
	//
	//				}
	//				else if ((matchedTFFKA.addr < addrnull
	//					|| matchedTFFKA.fileKey == FILEKEYBUILTIN)
	//					&& (*tfth)[matchedTFFKA].integrate == false) {
	//					//匹配到了内置函数的情况
	//
	//					CodeTable& cdtmid = (*cdtdes);
	//					CodeTable cdtmid_clone(cdtmid);
	//
	//					Intg s = 1;
	//
	//					for (Intg offset = 0; offset < s; offset++) {
	//						cdtmid_clone[pos - offset].assemblyFormula.flagBit.tfFKA =
	//							matchedTFFKA;
	//						cdtmid_clone[pos - offset].assemblyFormula.flagBit.functionHandleFlag =
	//							T_;
	//					}
	//#if debug
	//					{
	//						static int ignorecount = 0;
	//						std::cout << "(" << __FILE__ << ":" << __LINE__ << ":0"
	//							<< ")generateCdtByGroundingState(GroundingState*)"
	//							<< " return3 cdt:\n" << cdtmid_clone.toStrg()
	//							<< "\tignorecount:[" << ignorecount++ << "\t]"
	//							<< std::endl;
	//					}
	//#endif
	//					return cdtmid_clone;
	//
	//				}
	//
	//			{
	//				static int ignorecount = 0;
	//				std::cerr << "generateCdtByGroundingState(GroundingState*)"
	//					<< " Cannot take action, throw(-1) " << "\tignorecount:["
	//					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
	//					<< ":0" << ")" << std::endl;
	//			}
	//
	//			throw(-1);
	//
	//		}
	//
	//
	//
	//	};

	class Executor;
	struct StrgSetComparator {
		bool operator()(const std::set<std::string>& lhs, const std::set<std::string>& rhs) const {
			return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}
	};
	/**
	 * @本类操控Matcher和SingleTFIntegrater进行表达式层面上的grounding与banding，
	 * 利用一个循环驱动Matcher、GroundingStateSilo、SingleIntegrator实现表达式层面的grounding与banding
	 * 并将形成完整的代码表silo。详见Silo，详见此类中的integrate()
	 * @class 这个类提供了将一段包含一个或多个函数调用的表达式与代码进行匹配的方法,
	 * @attention 在2022年3月17日21:34:16之后的版本中，此类将提供指定匹配模式的接口
	 * @attention 此类跨cdt，使用fka检索code，同时使用fka检索scope、templateFunction、system
	 *
	 */
	class MultiTFIntegrator {
	public:
		Intg mode = MODE_DEFAULT;
		CodeTable* seperateCdtDes = nullptr;
		Intg matchStateSiloMaxCapacity = 300;

		Intg groundingStateSiloMaxCapacity = 300;
		bool root = false;
		Intg maxSwitchTurn = 200;

		bool train = true; //是否导出数据
		bool neuralNetworkEnabled = true;  //是否需要神经网络
		static inline map<set<Strg>, Strg> knowledge_domains__dataset_filename__map;

		set<Strg> DSP_domains;
		PipeCommunicationHandler* pipe_writing_handler = nullptr;
		PipeCommunicationHandler* pipe_reading_handler = nullptr;

		Setting* setting = nullptr;

		multimap<Intg, CodeTable> icmap;
		MultiTFIntegrator* parent = nullptr;
		GroundingStateSilo groundingStateSilo;
		DataTablePtr currentAR = nullptr;

		CodeTableHash* cdth = nullptr;
		FKA fkades;
		TemplateFunctionTableHash* tfth = nullptr;
		ScopeTableHash* scpth = nullptr;
		SystemTableHash* systh = nullptr;

		MessageWrapper mw;

		Executor* executor = nullptr;

		static inline Intg AST_transformation_count = 0;
		static inline Intg explored_space_count = 0;
		static inline Intg eliminated_constraints_count = 0;
		static inline Intg punished_constraints_count = 0;
		static inline Intg accumulated_ac = 0;
		static inline Intg accumulated_ci = 0;
		static inline Intg neural_network_assistance_count = 0;
		static inline Intg reasoning_failure_times = 0;

		/***
		 *
		 * @param cdth
		 * @param fkades 需要进行整合匹配的目标位置fka
		 * @param tfth
		 * @param scpth
		 */
		MultiTFIntegrator(CodeTableHash* cdth, const FKA& fkades,
			TemplateFunctionTableHash* tfth, ScopeTableHash* scpth,
			SystemTableHash* systh, DataTablePtr currentAR, Setting* setting,
			PipeCommunicationHandler* pipe_writing_handler_,
			PipeCommunicationHandler* pipe_reading_handler_);

		MultiTFIntegrator(Executor* executor_);
		/**
		 * 将工作模式设置为MODE_SEPARATE，其效果是匹配时不再将结果直接整合到cdtmain中，
		 * 但需提供整合代码段目标以及来源
		 * @arg
		 */
		void setModeSeparate(CodeTable& seperateCdtDes);

		void setMaxCapacity(Intg matchStateSiloMaxCapacity,
			Intg groundingStateSiloMaxCapacity, Intg maxSwitchTurn);

		/**
		 * @brief 获得指定表达式匹配函数时从何处开始寻找可访问函数
		 * @brief
		 * @param expFKA表达式中的任意一个codeFKA地址
		 * @attention 任何时候表达式都不能跨cdt
		 * @return 通常而言，输入的参数expfka就满足要求，
		 * 但如果其所在表达式中存在成员引用，例如a.b()，
		 * 则输入的参数expfka就不满足要求，将选取类内部上一行代码的fka。
		 */
		FKA getFunIndexFKA(const FKA& expFKA);


		/**
		 * @brief utilize groundingState n to generate ExpressionHandler
		 * for groundingState n+1 under the specified environment
		 *
		 * @param gs
		 * @return codetabel n+1
		 * @throw -1 when can't generate code based on gs n.
		 */
		ExpressionHandlerPtr generateExpressionHandlerByGroundingState(const GroundingState* gs);
		/**
		 * this function is the api to interact with the python program.
		 * @param gs
		 * @return
		 */
		json getNeuralNetworkAgentPrediction(const GroundingState* gs);

		/**
		 * @brief 获取理论上的置信度（从模型文件中读取）
		 * @return
		 */
		 //[[deprecated]] Numb getTheoreticConfidenceEfficient() {
		 //	//get from reply
		 //	return 1;
		 //}
		 /**
		  * @brief 返回表达式(由this->fkades标识)可以访问到的所有的类名和文件名组成的集合，并根据集合内容生成一个hash code
		  * 可以从getLastAccessibleTFFKA那里获得
		  * @return
		  */
		set<Strg> getAllAccessiableClassAndFile();


		/**
		 * @brief control complete grounding procedure:
		 * loop( groundingstate in groundingstatesilo){
		 *     get policy
		 *     control matching
		 *     caculate the confidence coefficient
		 *
		 *     utilizing the above three to generate new groundingstates in groundStatesilo;
		 *
		 * }
		 * @return
		 */
		bool integrate();

	}
	;
	//class Integrator;
	class CodeBlock {
	public:
		Intg type = COOL_M;                                    //可以为COOL_M、COOL_QS、COOL_QE、COOL_C、COOL_R
		bool dependent = false; //如果一个codeBlock在创建CodeTable时没有涉及未知值，那么它就是独立的（false），有未知值时，是依赖的（true）
		set<Arg> argUnknown; // 这个codeBlock所要求的未知值
		set<Arg> argTemporary; //储存所有在本block中   产生   临时变量（引用的临时变量不算）
		CodeTable codeTable; //codeTable
		Intg QSid = 0;
		Intg QEid = 0;
		Intg id = 0; //唯一id
		CodeBlock(Intg type, bool dependent, Intg id, Intg QSid,
			const Strg& cdtFileKey);

		CodeBlock();

		void addUnknownArg(const Arg& arg);

		Strg toStrg();


	};
	class CodeBlockTable {
	public:

		deque<Intg> iddq;
		deque<Intg> biddq; //逆向函数执行时的顺序backwardIDdeque

		map<Intg, CodeBlock> icbmap;
		void addBlock(Intg id, const CodeBlock& cb);

		/**
		 * @attention 这个函数可以向CodeBlockTable中添加codeBlock。
		 * 当id在icbmap中不存在时，会在返回引用的同时向iddq中pushback id。
		 * @param id
		 * @return
		 */
		CodeBlock& operator[](Intg id);

		void erase(Intg id);

		Strg toStrg();


	};
	/**
	 * @brief 此类进行逆推。
	 * @param 此类有两个主要参数：codeTableHash：逆向函数声明所在codeTableHash，采用fka检索
	 * @param bfbScopeCdt：逆向函数函数体的cdt，最初为ffbscope的克隆，
	 * 经处理后变成bfbscope并插入codetablehash，采用addr检索
	 * 此类要同时使用fka和addr检索。
	 */
	class BackStepper {
	public:
		Executor* executor = nullptr;
		CodeTableHash* codeTableHash = nullptr;
		ScopeTableHash* scopeTableHash = nullptr;
		DataTablePtr currentAR = nullptr;
		TemplateFunctionTableHash* templateFunctionTableHash = nullptr;
		SystemTableHash* systemTableHash = nullptr;
		Setting* setting = nullptr;
		Intg numID = 1;

		Intg currentScopeID = 0;
		vector<Intg> scopeIDStack;

		CodeBlockTable codeBlockTable;

		map<Arg, Arg> old_new_map; //map<oldArg,newArg>
		map<Arg, set<Intg>> unknown_id_map; //储存所有的未知变量以及依赖此arg的codeBlockID
		set<Arg> newarg; //储存所有引入的newarg，初始化时要加上
		//储存所有临时变量，并记录其出现次数。临时变量即不能被用户访问的变量，属性S地址，
		//一个临时变量只能出现最多两次，首次出现必须作为四元式的结果，第二次出现不能作为四元式的结果。
		map<Arg, Intg> temporaryArg_occ_map;
		//储存所有临时变量，以及引用这些临时变量值的block的集合（set长度最大为2）
		map<Arg, set<Intg>> temporaryArg_id_map;
		//  map<Arg, set<Intg>> unknown_id_map; //arg_dependentBlockID_map
		set<Intg> return_id;
		deque<Arg> argAnnounce;
		Intg maxBackStep = 10;  //逆推时最大允许代入的步数，应当放到设置里面
		typedef multimap<Intg, set<Intg>> T_num_idset_map;
		deque<T_num_idset_map> step_map_dq; //每个map都是未知量个数、代入顺序的map
		set<FKA> scopeFKASet;

		Addr currentFBAd; //current function body addr
		//current function body code
		Code* currentFBCd = nullptr; //current function body code

		Addr nextFBAd;
		FKA currentScopeFKA;
		Intg currentFBCdPos = 0;
		Intg nextFBCdPos = 0;
		vector<FKA> scopeStack;

		set<Arg> argKnown;
		set<Arg> argUnknown;
		set<Arg> mustKnown;

		//以下两者的fileKey必须相同
		FKA bfnScopeFKA;
		CodeTable* bfbScopeCdt = nullptr;
		Intg argID = 0;
		~BackStepper();


		/**
		 * 删除指定id的codeBlock，同时清理new_id_map
		 * @param id
		 */
		void eraseBlock(Intg id);

		/**
		 * 当idset中的所有式子互相代入后，还有多少个未知变量（估计值，未计算）
		 * @param idset
		 * @return
		 */
		Intg getUnknownArgNum(set<Intg> idset);

		/**
		 * 当idset中的所有式子互相代入后，还有多少个未知变量（估计值，未计算）
		 * @param idset
		 * @return
		 */
		set<Arg> getUnknownArgSet(set<Intg> idset);

		/**
		 * @brief 将id1代表的codeBlock整合到id2代表的block中。所述操作的block均在cbt中。
		 * @param id1
		 * @param id2
		 * @param cbt
		 * @return 代入成功/失败
		 */
		bool integrateTwoBlock(Intg id1, Intg id2, CodeBlockTable* cbt);

		/**
		 * @brief 将blockIDset中的所有代码段进行整合，其基本思路是，从前向后依次合并：
		 * 找到最小id对应的block，先将其与和其有公共未知变量的block合并，
		 * 然后逐步推进到根节点
		 * @param integrateIDblock
		 * @return 整合完成后的节点[root]
		 */
		CodeBlock integrateBlock(set<Intg> blockIDset);

		void setBackwardFunctionInfo(FKA& bfnScopeFKA, CodeTable* bfbScopeCdt);

		/**
		 * 以arg为模板返回一个新的arg，这个arg的标识符除argFlag外继承原arg
		 * @param arg
		 * @return
		 */
		Arg getNewArg(const Arg& arg);

		/**
		 * 根据当前argKnown、argUnknown设置cdt代码的可变属性，
		 * 在此过程中会进一步修改argKnown，argUnknown、temporaryArg_occ_map、temporaryArg_id_map的值
		 * @brief 判断方法：
		 * 1 当此函数的所有参数中至少包含一个未知参数时
		 * 2 该未知参数到根节点路径上的参数
		 * 3 该表达式上所有待定（被修改）的参数到最终根节点的参数，换句话说，这一步要将所有的可变变量变成待定变量
		 *@attention 注意，这一步需要对cdt进行参数值替换，并返回CodeBlock
		 *@attention 注意，如果cdt中存在赋值“=”语句，那么一个值在被赋值后的后续语句中只能被引用，不能再被主动修改。
		 * @param cdt
		 * @return cb
		 */
		CodeBlock setUnknownAttributeAndBlock(CodeTable& cdt);

		/**
		 * 分析函数名称表达式，获取已知未知参数,初始化old_new_map
		 */
		void analysisBFN();
		/**
		 * 分析函数名称表达式，获取已知未知参数,初始化old_new_map
		 */

		BackStepper(Executor* executor);
		//2 进行第一次正向执行，修改Z_Dz地址
		void backwardFunctionFirstForwardExecution();

		//3 第二次正向执行
		void backwardFunctionSecondForwardExecution();
		//3 第3次正向执行
		void backwardFunctionThirdForwardExecution();
		//3 第一次反向执行
		void backwardFunctionFirstBackwardExecution();
		//4 将代码整合到程序中
			/**
			 * 代码段实际地址取决于 split函数，这一步会对原scopeAddr进行优化
			 * @param appendFKA 修改的代码段将插入appendFKA之后
			 * @param addrmin 修改后的代码段的起始地址
			 * @param addrmax 修改后代码段终止地址
			 */
		void backwardFunctionIntegrate(const FKA& appendFKA, const Addr& addrmin,
			const Addr& addrmax);
	};
	template<class class_deprecated = nullptr_t>
	class [[deprecated("Use Executor in Grounding Mode instead")]] Integrator {
	public:
		Intg matchStateSiloMaxCapacity = 30;
		Intg groundingStateSiloMaxCapacity = 300;
		Intg maxSwitchTurn = 30;
		PipeCommunicationHandler* pipe_writing_handler = nullptr;
		PipeCommunicationHandler* pipe_reading_handler = nullptr;

		Scanner* scanner = nullptr;

		CodeTableHash* codeTableHash = nullptr;
		ScopeTableHash* scopeTableHash = nullptr;
		TemplateFunctionTableHash* templateFunctionTableHash = nullptr;
		DataTablePtr currentAR = nullptr;
		SystemTableHash* systemTableHash = nullptr;
		Setting* setting = nullptr;

		FKI currentCodeFKI;
		FKA currentCodeFKA;

		Code* currentCode = nullptr;

		FKI nextCodeFKI;
		FKA nextCodeFKA;

		Code* nextCode = nullptr;
		const bool getNewCode();
		const bool setNextCodeFKI(FKI& fki);
		const bool integrate();

		const bool setArgScopeFKA();
		const bool addArgsToCurrentAR();
		Integrator() {

		}
		Integrator(CodeTableHash* codeTableHash,
			TemplateFunctionTableHash* templateFunctionTableHash,
			ScopeTableHash* scopeTableHash, DataTablePtr dataTable,
			SystemTableHash* systemTableHash, Setting* setting) {
			this->codeTableHash = codeTableHash;
			this->templateFunctionTableHash = templateFunctionTableHash;
			this->scopeTableHash = scopeTableHash;
			this->currentAR = dataTable;
			this->systemTableHash = systemTableHash;
			this->setting = setting;
			this->matchStateSiloMaxCapacity = setting->matchStateSiloMaxCapacity;
			this->groundingStateSiloMaxCapacity =
				setting->groundingStateSiloMaxCapacity;
			this->maxSwitchTurn = setting->maxSwitchTurn;
		}
		Integrator(HashCollection* hashCollection, DataTablePtr dataTable) {
			this->codeTableHash = &hashCollection->cdth;
			this->templateFunctionTableHash = &hashCollection->tfth;
			this->scopeTableHash = &hashCollection->scpth;
			this->currentAR = dataTable;
			this->systemTableHash = &hashCollection->systh;
			this->setting = &hashCollection->setting;
			this->matchStateSiloMaxCapacity =
				hashCollection->setting.matchStateSiloMaxCapacity;
			this->groundingStateSiloMaxCapacity =
				hashCollection->setting.groundingStateSiloMaxCapacity;
			this->maxSwitchTurn = hashCollection->setting.maxSwitchTurn;

		}

		void setMaxCapacity(Intg matchStateSiloMaxCapacity,
			Intg groundingStateSiloMaxCapacity, Intg maxSwitchTurn) {
			this->matchStateSiloMaxCapacity = matchStateSiloMaxCapacity;
			this->groundingStateSiloMaxCapacity = groundingStateSiloMaxCapacity;
			this->maxSwitchTurn = maxSwitchTurn;
		}
		void setPipeCommunicationHandler(
			PipeCommunicationHandler* pipe_writing_handler_ = nullptr,
			PipeCommunicationHandler* pipe_reading_handler_ = nullptr) {
			this->pipe_writing_handler = pipe_writing_handler_;
			this->pipe_reading_handler = pipe_reading_handler_;
		}
	};


}  // namespace VM

#endif /* COOLANG_VM_INTEGRATE_HPP_ */
