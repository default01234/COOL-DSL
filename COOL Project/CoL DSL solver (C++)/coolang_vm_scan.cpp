#include"coolang_vm_execute.hpp"
using namespace std;
using namespace filesystem;
namespace COOLANG {

	/// <summary>
	/// Merge a list of Addrs to an Addr
	/// </summary>
	/// <param name="scopeStack"></param>
	/// <returns></returns>
	Addr stackToAddr(const vector<Addr>& scopeStack) {
		Addr a;
		for (int i = 0; i < (int)scopeStack.size(); i++) {
			a = link(a, scopeStack.at(i));

		}
		return a;
	}
	vector<path> getFilePath(const path& path_, const string& fileKey,
		const bool& withSuffix) {
#if debug
			{
				static int ignorecount = 0;
				std::cout << "getFilePath(const path&, const string&, const bool&)"
					<< " in path:[" << pathStrToUTF8(path_) << "] " << "fileKey:["
					<< fileKey << "]" << "\tignorecount:[" << ignorecount++
					<< "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")"
					<< std::endl;
			}
#endif
			if (exists(path_) == false) {

				{
					static int ignorecount = 0;
					cout << "getFilePath(path&, string&, const bool &) path:["
						<< pathStrToUTF8(path_) << "] " << "fileKey:[" << fileKey
						<< "]" << " path not exists ,exit(-1)" << "\tignorecount:["
						<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
						<< ":0" << ")" << endl;
				}

#if debug && _WIN32
				{
					static int ignoreCount = 0;
					// Assuming info is a std::string, convert to std::wstring
					std::string info = "error exit"; // Replace with actual info
					info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
					std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
					OutputDebugString(winfo.c_str());
#endif
					cout << info << endl;
					ignoreCount++;
				}
#endif
				exit(-1);

			}
			vector<path> pathvct;
			directory_entry dir_entry(path_);
			if (dir_entry.is_directory()) {
				directory_iterator dir_iter(path_);
				for (auto& it : dir_iter) {
					auto&& pathvct__ = getFilePath(it.path(), fileKey, withSuffix);
					pathvct.insert(pathvct.end(), pathvct__.begin(), pathvct__.end());
				}

			}
			else {
				if (withSuffix == false) {
					string&& stem_ = pathToUTF8(path_.stem());
					if (stem_ == fileKey) {
						pathvct.push_back(path_);
					}
				}
				else {
					if (pathStrToUTF8(path_.filename()) == fileKey) {
						pathvct.push_back(path_);
					}
				}

			}
			return pathvct;

	}


	/*************************************

	AssemblyCodeArg


	*********************************/


	AssemblyCodeArg::AssemblyCodeArg() {
	}

	AssemblyCodeArg::AssemblyCodeArg(const Intg& type, const Intg& arg_i) {
		this->type = type;
		this->arg_i = arg_i;
	}
	AssemblyCodeArg::AssemblyCodeArg(const Intg& type, const Strg& arg_s) {
		this->type = type;
		this->arg_s = arg_s;
	}
	AssemblyCodeArg::AssemblyCodeArg(const Intg& type, const Intg& arg_i, const Strg& arg_s) {
		this->type = type;
		this->arg_i = arg_i;
		this->arg_s = arg_s;
	}
	Strg AssemblyCodeArg::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << "type:[" << type << "\t] arg_i:[" << arg_i << "\t] arg_s:["
			<< arg_s << "\t]" << endl;
		return ss.str();
	}
	bool AssemblyCodeArg::operator==(const AssemblyCodeArg& arg) const {
		return this->type == arg.type && this->arg_i == arg.arg_i
			&& this->arg_s == arg.arg_s;
	}

	AssemblyCodeArg& AssemblyCodeArg::operator=(const AssemblyCodeArg& arg) {
		this->type = arg.type;
		this->arg_i = arg.arg_i;
		this->arg_s = arg.arg_s;
		return *this;
	}
	bool AssemblyCodeArg::operator<(const AssemblyCodeArg& arg) const {
		if (this->type < arg.type) {
			return true;
		}
		else if (this->type > arg.type) {
			return false;
		}

		if (this->arg_i < arg.arg_i) {
			return true;
		}
		else if (this->arg_i > arg.arg_i) {
			return false;
		}

		if (this->arg_s < arg.arg_s) {
			return true;
		}
		else {
			return false;
		}

	}



	/***********************************

	AssemblyCode

	************************************/
	bool AssemblyCode::operator==(const AssemblyCode& asc) const {
		return type == asc.type && arg1Flag == asc.arg1Flag &&

			arg2Flag == asc.arg2Flag &&

			operatorFlag == asc.operatorFlag &&

			resultFlag == asc.resultFlag &&

			arg1_i == asc.arg1_i && arg1_s == asc.arg1_s &&

			arg2_i == asc.arg2_i &&

			arg2_s == asc.arg2_s &&

			operator_i == asc.operator_i &&

			operator_s == asc.operator_s &&

			result_i == asc.result_i &&

			result_s == asc.result_s;

	}

	AssemblyCode::AssemblyCode(Strg assemblyCode) {
		vector<Strg> args = splitArgs(assemblyCode, COC_DELIMITER);
		if (args.size() != 9) {
			*this = AssemblyCode();
			return;
		}
		type = toIntg(args.at(0));
		arg1Flag = toIntg(args.at(5));
		arg2Flag = toIntg(args.at(6));
		operatorFlag = toIntg(args.at(7));
		resultFlag = toIntg(args.at(8));
		if (arg1Flag == Dz || arg1Flag == Sz) {
			try
			{
				arg1_i = toNumb(args.at(1));
			}
			catch (const CalculationException&)
			{
				arg1_i = 0;
			}
		}
		else if (arg1Flag == Bs || arg1Flag == Zf) {
			arg1_s = args.at(1);
		}
		else {
#if debug
			cerr << "AssemblyCode arg1Flag err" << endl;
#endif
		}

		if (arg2Flag == Dz || arg2Flag == Sz) {
			try
			{
				arg2_i = toNumb(args.at(2));
			}
			catch (const CalculationException&)
			{
				arg2_i = 0;
			}
		}
		else if (arg2Flag == Bs || arg2Flag == Zf) {
			arg2_s = args.at(2);
		}
		else {
#if debug
			cerr << "AssemblyCode arg2Flag err" << endl;
#endif
		}

		if (operatorFlag == Dz || operatorFlag == Sz) {
			try
			{
				operator_i = toNumb(args.at(3));
			}
			catch (const CalculationException&)
			{
				operator_i = 0;
			}
		}
		else if (operatorFlag == Bs || operatorFlag == Zf) {
			operator_s = args.at(3);
		}
		else {
#if debug
			cerr << "AssemblyCode operatorFlag err" << endl;
#endif
		}

		if (resultFlag == Dz || resultFlag == Sz) {
			try
			{
				result_i = toNumb(args.at(4));
			}
			catch (const CalculationException&)
			{
				result_i = 0;
			}
		}
		else if (resultFlag == Bs || resultFlag == Zf) {
			result_s = args.at(4);
		}
		else {
#if debug
			cerr << "AssemblyCode resultFlag err" << endl;
#endif
		}
		{
#if debug
			static int ignorecount = 0;
			cout
				<< "AssemblyCode(Strg) enclosing_method(enclosing_method_arguments)"
				<< "\nas:[" << this->toStrg() << "\t]" << "\nignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__ << ":0"
				<< ")" << endl;
#endif
		}

	}

	AssemblyCode& AssemblyCode::operator=(const AssemblyCode& asc) {
		type = asc.type;
		arg1Flag = asc.arg1Flag;

		arg2Flag = asc.arg2Flag;

		operatorFlag = asc.operatorFlag;

		resultFlag = asc.resultFlag;

		arg1_i = asc.arg1_i;
		arg1_s = asc.arg1_s;

		arg2_i = asc.arg2_i;

		arg2_s = asc.arg2_s;

		operator_i = asc.operator_i;

		operator_s = asc.operator_s;

		result_i = asc.result_i;

		result_s = asc.result_s;
		return *this;

	}

	AssemblyCodeArg AssemblyCode::operator[](const Intg pos) {
		switch (pos) {
		case 0:
			return AssemblyCodeArg(arg1Flag, arg1_i, arg1_s);
			break;
		case 1:
			return AssemblyCodeArg(arg2Flag, arg2_i, arg2_s);
			break;
		case 2:
			return AssemblyCodeArg(operatorFlag, operator_i, operator_s);
			break;
		case 3:
			return AssemblyCodeArg(resultFlag, result_i, result_s);
			break;
		default:
			return AssemblyCodeArg();
			break;
		}

	}

	bool AssemblyCode::includeArg(const AssemblyCodeArg& arg) {
		AssemblyCodeArg&& arg1 = (*this)[0];
		AssemblyCodeArg&& arg2 = (*this)[1];
		AssemblyCodeArg&& argop = (*this)[2];
		AssemblyCodeArg&& argres = (*this)[3];
		return arg1 == arg || arg2 == arg || argop == arg || argres == arg;

	}

	Strg AssemblyCode::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << type << "\\.";
		if ((arg1Flag == Dz && arg1_i != 0) || arg1Flag == Sz) {
			ss << arg1_i << "\\.";
		}
		else {
			ss << arg1_s << "\\.";
		}
		if ((arg2Flag == Dz && arg2_i != 0) || arg2Flag == Sz) {
			ss << arg2_i << "\\.";
		}
		else {
			ss << arg2_s << "\\.";
		}
		if ((operatorFlag == Dz && operator_i != 0) || operatorFlag == Sz) {
			ss << operator_i << "\\.";
		}
		else {
			ss << operator_s << "\\.";
		}
		if ((resultFlag == Dz && result_i != 0) || resultFlag == Sz) {
			ss << result_i << "\\.";
		}
		else {
			ss << result_s << "\\.";
		}

		ss << arg1Flag << "\\." << arg2Flag << "\\." << operatorFlag << "\\." << resultFlag;


		return ss.str();
	}


	/************************************

	AssemblyCodeTable

	***********************************/
	AssemblyCodeTable AssemblyCodeTable::operator=(const AssemblyCodeTable& asct) {
		this->acdq = asct.acdq;
		this->acpdq = asct.acpdq;
		this->fileKey = asct.fileKey;
		return *this;
	}
	/**
	 * @brief 从最末端（包括）开始检索不属于type类型的代码的地址（acp）
	 * @param types
	 * @return 找不到返回-1
	 */
	Intg AssemblyCodeTable::getLastNot(const vector<Intg>& types) {
		Intg pos = acpdq.size() - 1;
		if (pos < 0) {
#if debug
			cerr
				<< "AssemblyCodeTable::getLastNot debug::get Array end,return addrnull"
				<< endl;
#endif
			return -1;
		}
		Intg a = -1;

		while (pos >= 0) {
			if (std::find(types.begin(), types.end(), acdq[pos].type)
				!= types.end()) {
				pos--;
			}
			else {
				a = acpdq[pos];
				return a;
			}

		}
		return a;
	}
	/**
	 * @brief 剪切末端最长表达式中的以rootarg为根节点的树枝
	 * @param rootarg
	 * @return
	 */
	AssemblyCodeTable AssemblyCodeTable::cutTreeBranch(
		const AssemblyCodeArg& rootarg, const Intg& acpUpBound) {
		Intg acpEnd = this->getLastNot(vector<Intg> { COOL_Y, COOL_M, COOL_N, COOL_LST });
		Intg posEnd = getAddrPosition(acpdq, acpEnd);
		Intg acp = this->getLastArgAsResult(rootarg, acpUpBound);
		Intg pos = getAddrPosition(this->acpdq, acp);
		AssemblyCodeTable asct;
		map<AssemblyCodeArg, Intg> childArgs;
		childArgs.insert(make_pair(rootarg, 0));
		while (pos > posEnd) {
			AssemblyCode& asc = acdq[pos];
			if (childArgs.count(asc[3]) > 0) {
				asct.acpdq.push_front(this->acpdq[pos]);
				asct.acdq.push_front(asc);
				//判断arg1与arg2是否构成树的非叶子结点
				//（这里认为只有临时变量（S_Dz）才能成为非叶子结点）
				if (asc.arg1Flag == Dz) {
					childArgs[asc[0]] = 0;
				}
				if (asc.arg2Flag == Dz) {
					childArgs[asc[1]] = 0;
				}
				this->acdq.erase(acdq.begin() + pos);
				this->acpdq.erase(acpdq.begin() + pos);

			}
			--pos;

		}
		return asct;

	}
	/**
	 * @brief 从末端开始，截取最长的表达式，范围仅限当前asct
	 * @return 截取掉的部分
	 */
	AssemblyCodeTable AssemblyCodeTable::cutLongestExpression() {
		Intg posEnd = 0;
		Intg ascpos = getLastNot(vector<Intg> { COOL_Y, COOL_N, COOL_M, COOL_LST });
		posEnd = getAddrPosition(acpdq, ascpos);
		Intg pos = acpdq.size() - 1;
		AssemblyCodeTable asct;
		while (pos > posEnd) {
			asct.acdq.push_front(this->acdq[pos]);
			asct.acpdq.push_front(this->acpdq[pos]);
			this->acdq.erase(acdq.begin() + pos);
			this->acpdq.erase(acpdq.begin() + pos);
			--pos;

		}
		return asct;
	}
	/**
	 * @brief 从末端开始检索，获得arg上次以“运算结果”出现的代码的地址（acp），只检索末端最长表达式
	 * @param arg,acpUpBound
	 * @return acp,找不到返回-1；acp≤acpUpBound
	 */
	Intg AssemblyCodeTable::getLastArgAsResult(const AssemblyCodeArg& arg,
		const Intg& acpUpBound) {
		Intg pos = this->acpdq.size() - 1;
		if (acpUpBound < this->acpdq.back()) {
			pos = getAddrPosition(this->acpdq, acpUpBound);
			if (pos < 0) {
				//没找到acpUpBound对应具体位置
				pos = getLastAddrPosition(this->acpdq, acpUpBound);
			}
		}
		Intg acpend = this->getLastNot(vector<Intg> { COOL_Y, COOL_M, COOL_N, COOL_LST });
		Intg posEnd;
		if (acpend < 0) {

			posEnd = -1;
		}
		else {
			posEnd = getAddrPosition(acpdq, acpend);
		}
		while (pos > posEnd) {
			if (acdq[pos][3] == arg) {
				return acpdq[pos];
			}
			--pos;
		}
		return -1;
	}
	Strg AssemblyCodeTable::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << this->fileKey << "\n";
		Intg len = this->acdq.size();
		for (Intg i = 0; i < len; ++i) {
			ss << i << "\t" << "pos[" << acpdq[i] << "]\t" << acdq[i].toStrg()
				<< "\n";

		}
		ss << endl;

		return ss.str();

	}

	AssemblyCodeTable assemblyCodeTableDeformation(AssemblyCodeTable& asct) {
#if debug
		{
			static int ignorecount = 0;
			cout << "assemblyCodeTableDeformation(AssemblyCodeTable&)" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif
		deque<AssemblyCodeTable> asctdq;
		Intg pos = asct.acpdq.size() - 1;
		Intg acpBegin = asct.acpdq[pos];
		Intg acp;
		while (pos > -1) {
			acp = asct.acpdq[pos];
			if (pos > -1 && asct.acdq[pos].type == COOL_Y && acp != acpBegin) {
				AssemblyCode& asc_ = asct.acdq[pos + 1];
				if ((asc_.type == COOL_Y && asc_[0] == asct.acdq[pos][3])
					|| (asc_.type == COOL_M && asc_[0] == asct.acdq[pos][3]
						&& asc_[2].arg_s == "call")) {
					//当前asc是上一个asc的父系或上一个call语句的左支
					--pos;
					continue;

				}
				else {
					//当前asc是一棵新的Y树的根节点
					AssemblyCodeTable&& asct_ = asct.cutTreeBranch(
						asct.acdq[pos][3], asct.acpdq[pos]);
					asctdq.push_front(assemblyCodeTableDeformation(asct_));
					pos = getLastAddrPosition(asct.acpdq, acp);

				}

			}
			if (pos > -1 && asct.acdq[pos].type == COOL_LST && acp != acpBegin) {
				AssemblyCode& asc_ = asct.acdq[pos + 1];
				if (asc_.type == COOL_LST && asc_[3] == asct.acdq[pos][3]) {
					//当前asc是上一个asc的父系
					--pos;
					continue;

				}
				else {
					//当前asc是一棵新的Y树的根节点
					AssemblyCodeTable&& asct_ = asct.cutTreeBranch(
						asct.acdq[pos][3], asct.acpdq[pos]);
					asctdq.push_front(assemblyCodeTableDeformation(asct_));
					pos = getLastAddrPosition(asct.acpdq, acp);

				}

			}
			if (pos > -1 && asct.acdq[pos].type == COOL_M && acp != acpBegin
				&& asct.acdq[pos].arg1Flag == Dz
				&& asct.acdq[pos][2].arg_s == "call") {
				Intg argacp = asct.getLastArgAsResult(asct.acdq[pos][0],
					asct.acpdq[pos]);
				if (argacp < 0) {
					--pos;
					continue;
				}
				Intg argpos = getAddrPosition(asct.acpdq, argacp);
				AssemblyCode& asc_ = asct.acdq[argpos];
				if (asc_.type == COOL_Y && asc_[3] == asct.acdq[pos][0]) {

					//当前asc是一棵新的Y树的根节点的call调用
					AssemblyCodeTable&& asct_ = asct.cutTreeBranch(
						asct.acdq[pos][3], asct.acpdq[pos]);
					asctdq.push_front(assemblyCodeTableDeformation(asct_));
					pos = getLastAddrPosition(asct.acpdq, acp);

				}
				else {
					--pos;
					continue;
				}

			}
			--pos;

		}
		for (auto& asctit : asctdq) {
			//        asct.acdq.insert(asct.acdq.begin(), AssemblyCode());
			asct.acdq.insert(asct.acdq.begin(), asctit.acdq.begin(),
				asctit.acdq.end());

		}
		return asct;

	}

	/**
	 * @brief 获得hash中位于指定位置的asct的引用,禁止用此函数创建新的键值对
	 * @param pos （0,1,2,3）
	 * @return
	 */
	AssemblyCodeTable& AssemblyCodeTableHash::operator[](const Intg& pos) {
		if (pos >= size()) {

			{
				static int ignorecount = 0;
				cout << "AssemblyCodeTableHash::operator [](const int&)"
					<< " out of bound, exit(-1) " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}
		auto&& it = this->addr_file_map.begin();
		Intg pos_ = pos;
		while (pos_-- > 0) {
			++it;
		}
		Strg& fileKey = it->second;
		return file_asct_map[fileKey];

	}
	/**
	 * @brief 获得hash中位于指定addr的asct的引用,禁止用此函数创建新的键值对
	 * @param pos （0,1,2,3）
	 * @return
	 */
	AssemblyCodeTable& AssemblyCodeTableHash::operator[](const Addr& addr) {
		if (addr_file_map.count(addr) <= 0) {

			{
				static int ignorecount = 0;
				cout << "AssemblyCodeTableHash::operator [](const Addr &addr)"
					<< " addr not found, exit(-1) " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}
		Strg& fileKey = addr_file_map[addr];
		return file_asct_map[fileKey];

	}
	/**
	 * @brief 获得hash中位于指定Strg#x的asct的引用,禁止用此函数创建新的键值对
	 * @param pos （0,1,2,3）
	 * @return
	 */
	AssemblyCodeTable& AssemblyCodeTableHash::operator[](const Strg& fileKey) {
		if (file_asct_map.count(fileKey) <= 0) {

			{
				static int ignorecount = 0;
				cout << "AssemblyCodeTableHash::operator [](const Strg &fileKey)"
					<< " fileKey not found, exit(-1) " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}
		return file_asct_map[fileKey];

	}
	/**
	 * @brief 返回Asct对应addr在ascth中的上一个addr，如果不存在则返回addrnull
	 * @param addr
	 * @return
	 */
	Addr AssemblyCodeTableHash::getLastAsctAddr(const Addr& addr) {
		auto&& it = addr_file_map.find(addr);
		if (it == addr_file_map.begin()) {
			return addrnull;
		}
		else {
			--it;
			return it->first;
		}
	}
	Addr AssemblyCodeTableHash::getNextAsctAddr(const Addr& addr) {
		auto&& it = addr_file_map.find(addr);
		if (it == addr_file_map.end()) {
			return addrnull;
		}
		else {
			++it;
			return it->first;
		}
	}
	/**
	 * @brief 返回Asct对应Strg在ascth中的下一个filekey，如果不存在则返回“”
	 * @param addr
	 * @return
	 */
	Strg AssemblyCodeTableHash::getLastAsctFileKey(const Strg& fileKey) {
		auto&& it = file_addr_map.find(fileKey);
		if (it == file_addr_map.end()) {
			throw exception("Invalid fileKey");
		}
		auto fileAddr = it->second;
		auto it2 = addr_file_map.find(fileAddr);
		if (it2 == addr_file_map.begin()) {
			return "";
		}
		else {
			--it2;
			return it2->second;
		}
	}
	Strg AssemblyCodeTableHash::getNextAsctFileKey(const Strg& fileKey) {
		auto&& it = file_addr_map.find(fileKey);
		if (it == file_addr_map.end()) {
			throw exception("Invalid fileKey");
		}
		auto fileAddr = it->second;
		auto it2 = addr_file_map.find(fileAddr);
		++it2;
		if (it2 == addr_file_map.end()) {
			return "";
		}
		else {
			return it2->second;
		}
	}
	/**
	 * @brief 将asct插入到当前addr之前或之后
	 * @param asct
	 * @param currentAddr
	 * @param front 为真插入前，为假插入后面
	 * @return asct所插入后对应的地址。
	 */
	Addr AssemblyCodeTableHash::insertAsct(AssemblyCodeTable& asct,
		const Addr& currentAddr, const bool& front) {
		asct.ascth = this;
		if (currentAddr == addrnull) {
			return appendAsct(asct);
		}
		if (front == true) {

			Addr&& lastAddr = getLastAsctAddr(currentAddr);
			auto&& addrv = split(lastAddr, currentAddr, 2);
			Addr& addrInsert = addrv[1];
			this->file_addr_map[asct.fileKey] = addrInsert;
			this->addr_file_map[addrInsert] = asct.fileKey;
			this->file_asct_map[asct.fileKey] = asct;
			return addrInsert;

		}
		else {
			Addr&& nextAddr = getNextAsctAddr(currentAddr);
			if (nextAddr != addrnull) {
				auto&& addrv = split(nextAddr, currentAddr, 2);
				Addr& addrInsert = addrv[1];
				this->file_addr_map[asct.fileKey] = addrInsert;
				this->addr_file_map[addrInsert] = asct.fileKey;
				this->file_asct_map[asct.fileKey] = asct;
				return addrInsert;

			}
			else {
				return appendAsct(asct);
			}

		}
	}
	/**
	 * @brief 在当前hash末尾添加一个asct
	 * @param asct
	 * @return
	 */
	Addr AssemblyCodeTableHash::appendAsct(AssemblyCodeTable& asct) {
		asct.ascth = this;
		if (file_addr_map.size() <= 0) {
			Addr addrInsert(1000);
			this->addr_file_map[addrInsert] = asct.fileKey;
			this->file_asct_map[asct.fileKey] = asct;
			this->file_addr_map[asct.fileKey] = addrInsert;
			return addrInsert;

		}
		else {
			Addr addrInsert = addr_file_map.rbegin()->first.addrv[0] + 3;
			this->addr_file_map[addrInsert] = asct.fileKey;
			this->file_asct_map[asct.fileKey] = asct;
			this->file_addr_map[asct.fileKey] = addrInsert;
			return addrInsert;

		}

	}

	Strg AssemblyCodeTableHash::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		for (auto& pr : file_asct_map) {
			ss << pr.second.toStrg();
		}

		return ss.str();
	}

	Intg AssemblyCodeTableHash::size() {
		return file_asct_map.size();
	}

	/***************************************

	Arg

	*********************************/

	ArgComparatorLT::ArgComparatorLT(acm mode_ /*= acm::name | acm::ref*/, DataTablePtrW dt_ /*= nullptr*/, CodeTableHash* cdth_ /*= nullptr*/) :mode(mode_), dt(dt_), cdth(cdth_) {
	}
	bool ArgComparatorLT::operator()(const Arg& lhs, const Arg& rhs) const {

		return Arg::comparatorLess(lhs, rhs, mode);



	}

	void Arg::setOnHeap() {
		this->on_heap = true;
	}
	bool Arg::onHeap() const {
		return this->on_heap;
	}
	ArgPtr Arg::getThisPtr() const {
		if (on_heap == true) {
			return thisPtr;
		}
		else {
			return ArgPtr(this);
		}
	}
	bool  Arg::isVar() const {
		return (!(arg_i == 0 && arg_fka == fkanull && arg_s == ""
			&& ref_ar == nullptr))
			&& (argFlag == S_Bs || argFlag == S_Dz || argFlag == Z_Dz || argFlag == EXPRZ_Dz
				|| argFlag == S_AR || argFlag == Y_Dz);
	}

	bool  Arg::isBs() const {
		return (!(arg_i == 0 && arg_fka == fkanull && arg_s == ""
			&& ref_ar == nullptr)) && (argFlag == S_Bs || argFlag == X_Bs || argFlag == M_Bs);
	}

	bool  Arg::isDz() const {
		return (!(arg_i == 0 && arg_fka == fkanull && arg_s == ""
			&& ref_ar == nullptr))
			&& (argFlag == S_Dz || argFlag == Z_Dz || argFlag == EXPRZ_Dz || argFlag == Y_Dz);

	}
	void  Arg::initializeAsSymbol()
	{
		this->argFlag = S_Bs;
		this->isSymbol = true;
		this->symbolID = ++symbol_count;
	}


	ConstraintArg Arg::toConstraintArg() const {
		ConstraintArg ca;
		ca.argFlag = this->argFlag;
		ca.arg_i = this->arg_i;
		ca.arg_s = this->arg_s;
		ca.arg_fka = this->arg_fka;
		return ca;
	}
	/// <summary>
		/// extract new implicit constraint from composed constraints and merge it to current constraint set.
		/// </summary>
		/// <param name="dt"></param>
		/// <param name="systh"></param>
	void Arg::expandConstraints(DataTablePtrW dt, SystemTableHash* systh /*= nullptr*//*for future purpose*/) {

		bool addstring = false;//add constraint "string"
		bool addnumber = false;//add constraint "number"
		set<Strg> classnames;//add classnames;
		for (auto& c : this->constraints) {
			if (valueComparators.count(c.instruction)) {

				auto& constrarg = c.args[0];
				{
					if (constrarg.isImmediate() == false && dt != nullptr) {

						Arg arg;
						if (constrarg.isBs()) {
							arg.argFlag = constrarg.argFlag;

						}
						else if (constrarg.isDz()) {
							arg.argFlag = constrarg.argFlag;
						}

						Data& data = dt->operator[](arg);
						if (data.dataFlagBit == Sz) {
							addnumber = true;
						}
						else if (data.dataFlagBit == Zf) {
							addstring = true;
						}
						else if (data.dataFlagBit == S_AR && systh != nullptr) {
							FKA& sysfka = data.content_ar->scopeStructureFKA;
							classnames.insert(systh->operator[](sysfka).systemName);

						}
						else {

#if debug && _WIN32

							{
								static int ignoreCount = 0;
								// Assuming info is a std::string, convert to std::wstring
								std::string info = "unsupported data value for expandConstraint, exit(-1)."; // Replace with actual info
								info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
								std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
								OutputDebugString(winfo.c_str());
#endif
								cout << info << endl;
								ignoreCount++;
							}
#endif
							cerr << "unsupported data value for expandConstraint, exit(-1)." << endl;
							exit(-1);

						}

					}
					else {
						if (constrarg.argFlag == Sz) {
							addnumber = true;
						}
						else if (constrarg.argFlag == Zf) {
							addstring = true;
						}


					}
				}

			}
		}
		if (addnumber || this->argFlag == Sz) {
			Constraint c1("number");

			this->constraints.insert(c1);
			if (this->argFlag == Sz) {

				Constraint c2("eq", { this->toConstraintArg() });
				Constraint c3("immediate");
				this->constraints.insert(c2);
				this->constraints.insert(c3);

			}

		}
		if (addstring || this->argFlag == Zf) {
			Constraint c1("string");
			this->constraints.insert(c1);
			if (this->argFlag == Zf) {

				Constraint c2("eq", { this->toConstraintArg() });
				Constraint c3("immediate");
				this->constraints.insert(c2);
				this->constraints.insert(c3);

			}

		}
		for (auto& classname : classnames) {
			Constraint c_(classname);
			this->constraints.insert(c_);

		}

	}


	//return 0 not intersect, 1 intersect, 2 incomparable
	int Arg::intersect(const Arg& argwithconstr1, const Constraint& constr1, const Arg& argwithconstr2, const Constraint& constr2, acm md, DataTablePtrW dt) {

		// Helper function to convert data to arg if needed
		auto getDataConstraintArg = [&dt](const ConstraintArg& constrarg) -> ConstraintArg {
			if (constrarg.isImmediate() == false) {
				if (dt == nullptr) {

#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = "datatable cannot be null! exit(-1)"; // Replace with actual info
						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
						std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
						OutputDebugString(winfo.c_str());
#endif
						cout << info << endl;
						ignoreCount++;
					}
#endif
					cerr << "datatable cannot be null! exit(-1)" << endl;
					exit(-1);

				}

				Arg arg;
				if (constrarg.isBs()) {
					arg.argFlag = constrarg.argFlag;

				}
				else if (constrarg.isDz()) {
					arg.argFlag = constrarg.argFlag;
				}

				Data& data = (*dt)[arg];
				ConstraintArg constrargret;
				if (data.dataFlagBit == Sz) {
					constrargret.argFlag = Sz;
					constrargret.arg_i = data.content_i;
				}
				else if (data.dataFlagBit == Zf) {
					constrargret.argFlag = Zf;
					constrargret.arg_s = data.content_s;
				}
				else {

#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = "invalid data value for intersecting, exit(-1)."; // Replace with actual info
						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
						std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
						OutputDebugString(winfo.c_str());
#endif
						cout << info << endl;
						ignoreCount++;
					}
#endif
					cerr << "invalid data value for intersecting, exit(-1)." << endl;
					exit(-1);
				}


				return constrargret; // Assuming ConstraintArg can convert data to Arg
			}
			else {


				return constrarg;
			}
			};

		// Helper function to check arg types
		auto areConstraintArgsComparable = [](const ConstraintArg& ca1, const ConstraintArg& ca2) -> bool {
			return ca1.argFlag == ca2.argFlag;
			};

		/// 2.1 Numerical Constraints
		/// c1 must belong to {@COOLANG::valueComparators}, c2 uncertain.
		auto numericalIntersect = [&dt, getDataConstraintArg, areConstraintArgsComparable](const Constraint& c1, const Constraint& c2)->int {


			if (valueComparators.count(c2.instruction) == 0) return 2; // Incomparable if different instructions

			ConstraintArg&& c1Arg = getDataConstraintArg(c1.args.front());
			ConstraintArg&& c2Arg = getDataConstraintArg(c2.args.front());

			if (!areConstraintArgsComparable(c1Arg, c2Arg)) {
				return 2;
			} // Incomparable if different constrarg types

			// Add detailed comparison logic for each pair of constraints
			 // eq and eq
			if (c1.instruction == "eq" && c2.instruction == "eq") {
				return (c1Arg == c2Arg) ? 1 : 0;
			}

			// eq and ne
			else if (c1.instruction == "eq" && c2.instruction == "ne") {
				return (c1Arg != c2Arg) ? 1 : 0;
			}

			// eq and gte
			else if (c1.instruction == "eq" && c2.instruction == "gte") {
				return (c1Arg >= c2Arg) ? 1 : 0;
			}

			// eq and gt
			else if (c1.instruction == "eq" && c2.instruction == "gt") {
				return (c1Arg > c2Arg) ? 1 : 0;
			}

			// eq and lte
			else if (c1.instruction == "eq" && c2.instruction == "lte") {
				return (c1Arg <= c2Arg) ? 1 : 0;
			}

			// eq and lt
			else if (c1.instruction == "eq" && c2.instruction == "lt") {
				return (c1Arg < c2Arg) ? 1 : 0;
			}

			// ne and eq
			else if (c1.instruction == "ne" && c2.instruction == "eq") {
				return (c1Arg != c2Arg) ? 1 : 0;
			}

			// ne and ne
			else if (c1.instruction == "ne" && c2.instruction == "ne") {
				return 1; // Not equal constraints always intersect
			}

			// ne and gte
			else if (c1.instruction == "ne" && c2.instruction == "gte") {
				// Special case: if c1Arg is exactly equal to c2Arg, no intersection
				return (c1Arg == c2Arg) ? 0 : 1;
			}

			// ne and gt
			else if (c1.instruction == "ne" && c2.instruction == "gt") {
				// Special case: if c1Arg is exactly equal to c2Arg, no intersection
				return (c1Arg == c2Arg) ? 0 : 1;
			}

			// ne and lte
			else if (c1.instruction == "ne" && c2.instruction == "lte") {
				// Special case: if c1Arg is exactly equal to c2Arg, no intersection
				return (c1Arg == c2Arg) ? 0 : 1;
			}

			// ne and lt
			else if (c1.instruction == "ne" && c2.instruction == "lt") {
				// Special case: if c1Arg is exactly equal to c2Arg, no intersection
				return (c1Arg == c2Arg) ? 0 : 1;
			}

			// gte and eq
			else if (c1.instruction == "gte" && c2.instruction == "eq") {
				return (c1Arg <= c2Arg) ? 1 : 0;
			}

			// gte and ne
			else if (c1.instruction == "gte" && c2.instruction == "ne") {
				// Special case: if c1Arg is exactly equal to c2Arg, no intersection
				return (c1Arg == c2Arg) ? 0 : 1;
			}

			// gte and gte
			else if (c1.instruction == "gte" && c2.instruction == "gte") {
				return (c1Arg == c2Arg || c1Arg <= c2Arg) ? 1 : 0;
			}

			// gte and gt
			else if (c1.instruction == "gte" && c2.instruction == "gt") {
				return (c1Arg > c2Arg) ? 1 : 0;
			}

			// gte and lte
			else if (c1.instruction == "gte" && c2.instruction == "lte") {
				return (c1Arg <= c2Arg) ? 1 : 0;
			}

			// gte and lt
			else if (c1.instruction == "gte" && c2.instruction == "lt") {
				return (c1Arg < c2Arg) ? 1 : 0;
			}

			// gt and eq
			else if (c1.instruction == "gt" && c2.instruction == "eq") {
				return (c1Arg < c2Arg) ? 1 : 0;
			}

			// gt and ne
			else if (c1.instruction == "gt" && c2.instruction == "ne") {
				// Special case: if c1Arg is exactly equal to c2Arg, no intersection
				return (c1Arg == c2Arg) ? 0 : 1;
			}

			// gt and gte
			else if (c1.instruction == "gt" && c2.instruction == "gte") {
				return (c1Arg >= c2Arg) ? 1 : 0;
			}

			// gt and gt
			else if (c1.instruction == "gt" && c2.instruction == "gt") {
				return (c1Arg == c2Arg || c1Arg > c2Arg) ? 1 : 0;
			}

			// gt and lte
			else if (c1.instruction == "gt" && c2.instruction == "lte") {
				return (c1Arg <= c2Arg) ? 1 : 0;
			}

			// gt and lt
			else if (c1.instruction == "gt" && c2.instruction == "lt") {
				return (c1Arg < c2Arg) ? 1 : 0;
			}

			// lte and eq
			else if (c1.instruction == "lte" && c2.instruction == "eq") {
				return (c1Arg >= c2Arg) ? 1 : 0;
			}

			// lte and ne
			else if (c1.instruction == "lte" && c2.instruction == "ne") {
				// Special case: if c1Arg is exactly equal to c2Arg, no intersection
				return (c1Arg == c2Arg) ? 0 : 1;
			}

			// lte and gte
			else if (c1.instruction == "lte" && c2.instruction == "gte") {
				return (c1Arg >= c2Arg) ? 1 : 0;
			}

			// lte and gt
			else if (c1.instruction == "lte" && c2.instruction == "gt") {
				return (c1Arg > c2Arg) ? 1 : 0;
			}

			// lte and lte
			else if (c1.instruction == "lte" && c2.instruction == "lte") {
				return (c1Arg == c2Arg || c1Arg >= c2Arg) ? 1 : 0;
			}

			// lte and lt
			else if (c1.instruction == "lte" && c2.instruction == "lt") {
				return (c1Arg > c2Arg) ? 1 : 0;
			}

			// lt and eq
			else if (c1.instruction == "lt" && c2.instruction == "eq") {
				return (c1Arg > c2Arg) ? 1 : 0;
			}

			// lt and ne
			else if (c1.instruction == "lt" && c2.instruction == "ne") {
				// Special case: if c1Arg is exactly equal to c2Arg, no intersection
				return (c1Arg == c2Arg) ? 0 : 1;
			}

			// lt and gte
			else if (c1.instruction == "lt" && c2.instruction == "gte") {
				return (c1Arg >= c2Arg) ? 1 : 0;
			}

			// lt and gt
			else if (c1.instruction == "lt" && c2.instruction == "gt") {
				return (c1Arg > c2Arg) ? 1 : 0;
			}

			// lt and lte
			else if (c1.instruction == "lt" && c2.instruction == "lte") {
				return (c1Arg >= c2Arg) ? 1 : 0;
			}

			// lt and lt
			else if (c1.instruction == "lt" && c2.instruction == "lt") {
				return (c1Arg == c2Arg || c1Arg > c2Arg) ? 1 : 0;
			}

			return 0; // No intersection for unhandled cases




			};
		if (constr1.instruction == "eq" || constr1.instruction == "ne" || constr1.instruction == "gte" || constr1.instruction == "gt" || constr1.instruction == "lte" || constr1.instruction == "lt") {
			return numericalIntersect(constr1, constr2);
		}
		if (constr2.instruction == "eq" || constr2.instruction == "ne" || constr2.instruction == "gte" || constr2.instruction == "gt" || constr2.instruction == "lte" || constr2.instruction == "lt") {
			return numericalIntersect(constr2, constr1);
		}

		// 2.2 Logic Constraint


		auto logicIntersect = [&dt, getDataConstraintArg, areConstraintArgsComparable](const Constraint& c1, const Constraint& c2)->int {
			if (c2.instruction == "not") {
				return 1; // Intersection if different instructions
			}

			if (c2.instruction != c1.instruction) {
				auto c1Arg = getDataConstraintArg(c1.args.front());
				auto c2Arg = getDataConstraintArg(c2.args.front());

				if (!areConstraintArgsComparable(c1Arg, c2Arg) || c2.args.front() != c2.args.front()) {
					return 1; // Intersection if different args or types

				}
				if (c1.args.front().argFlag == X_Bs && c1.args.front().arg_s == c2.instruction) {
					return 0;
				}
			}
			return 2;
			};
		if (constr1.instruction == "not") {

			return logicIntersect(constr1, constr2);
		}
		if (constr1.instruction == "not") {

			return logicIntersect(constr2, constr1);
		}

		// 2.3 Attribute Constraint

		//whether c2 is child constraint of c1.
		auto attrIntersect = [&dt, getDataConstraintArg, areConstraintArgsComparable](const Constraint& c1, const Constraint& c2, const Arg& argwithconstr2_)->int {
			if (c2.instruction == "immediate" && c2.instruction == "immediate") {
				return 1; // Intersection if both are immediate
			}
			else if (c1.instruction == "immediate" && c2.instruction != "immediate") {
				return (!argwithconstr2_.isVar()) ? 1 : 0; // Intersection only if argwithconstr2 is immediate
			}
			return 2;
			};
		if ((static_cast<int>(md) & static_cast<int>(acm::immediate)) > 0) {
			if (constr1.instruction == "immediate") {
				return attrIntersect(constr1, constr2, argwithconstr2);
			}
			if (constr2.instruction == "immediate") {
				return attrIntersect(constr2, constr1, argwithconstr1);
			}

		}





		// 2.4 General Constraints 
		if (constr1.instruction != constr2.instruction) {
			return 2; // Incomparable if different instructions
		}
		// Check if args are the same for same instructions
		//  args are stored in a deque<ConstraintArg>
		if (constr1.args.size() != constr2.args.size()) {
			return 0; // Incomparable if different number of args
		}

		for (size_t i = 0; i < constr1.args.size(); ++i) {
			ConstraintArg c1Arg = getDataConstraintArg(constr1.args[i]);
			ConstraintArg c2Arg = getDataConstraintArg(constr2.args[i]);
			if (!areConstraintArgsComparable(c1Arg, c2Arg) || c1Arg != c2Arg) return 0; // No intersection if different args
		}

		return 1; // Intersection if all checks pass
	}
	/// <summary>
		/// compare two args, whether they have an intersection.
		/// </summary>
		/// <param name="arg1_"></param>
		/// <param name="arg2_"></param>
		/// <param name="mode"></param>
		/// <param name="dt"></param>
		/// <param name="cdth">used for EXPRZ_Dz arg comparason for data</param>
		/// <returns></returns>
	bool Arg::intersect(const Arg& arg1_, const Arg& arg2_, acm mode /*= acm::name | acm::ref*/, DataTablePtrW dt/* = nullptr*/, CodeTableHash* cdth/* = nullptr*/) {
		//std::bitset<static_cast<int>(ArgCmpMode_COUNT)> modes(static_cast<int>(mode));
		ArgPtr arg1(&arg1_);
		ArgPtr arg2(&arg2_);


		if (static_cast<int>(mode & acm::ref) != 0) {

			while (arg1->argFlag == Y_Dz) {
				*arg1 = globalRefTable[*arg1];
			}
			while (arg2->argFlag == Y_Dz) {
				*arg2 = globalRefTable[*arg2];
			}
			//				if (arg1->argFlag == Y_Dz && arg2->argFlag == Y_Dz) {
			//#if debug
			//					{
			//						static int ignorecount = 0;
			//						cout << "Arg::operator ==(const Arg&)" << " " << "\tignorecount:["
			//							<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
			//							<< ":0" << ")" << endl;
			//			}
			//#endif
			//					//在预执行过程中，multimap相当于普通的map
			//					return globalRefTable.argfka_ref_map.find(arg1->arg_fka)->second
			//						== globalRefTable.argfka_ref_map.find(arg2->arg_fka)->second;
			//				}
			//				else {
			//					if (arg1->argFlag == Y_Dz) {
			//#if debug
			//						{
			//							static int ignorecount = 0;
			//							cout << "Arg::operator ==(const Arg&)" << " "
			//								<< "\tignorecount:[" << ignorecount++ << "\t]("
			//								<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			//		}
			//#endif
			//						return intersect(globalRefTable[*arg1], *arg2, mode, dt);
			//	}
			//					if (arg2->argFlag == Y_Dz) {
			//
			//						return *this == globalRefTable[arg];
			//					}
			//}

		}
		if (static_cast<int>(mode & acm::placeholder) != 0) {

			if (arg1->isPlaceholder && arg1->isBound)
			{
				if (dt == nullptr) {

#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = "datatable cannot be nullptr when searching the arg bound with placeholder, exit(-1)"; // Replace with actual info
						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
						std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
						OutputDebugString(winfo.c_str());
#endif
						cout << info << endl;
						ignoreCount++;
					}
#endif

				}

				if ((*dt)[*arg1].expr->type == EXPRESSIONHANDLER_ARG) {
					arg1 = ArgPtr(&((*dt)[*arg1].expr->expr_arg));
				}
				else {
					arg1 = ArgPtr::make();
					*arg1 = (*dt)[*arg1].expr->expr_cdt.codedq.back()[3];
				}
			}
			if (arg2->isPlaceholder && arg2->isBound)
			{
				if (dt == nullptr) {

#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = "datatable cannot be nullptr when searching the arg bound with placeholder, exit(-1)"; // Replace with actual info
						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
						std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
						OutputDebugString(winfo.c_str());
#endif
						cout << info << endl;
						ignoreCount++;
					}
#endif

				}

				if ((*dt)[*arg2].expr->type == EXPRESSIONHANDLER_ARG) {
					arg2 = ArgPtr(&((*dt)[*arg2].expr->expr_arg));
				}
				else {
					arg2 = ArgPtr::make();
					*arg2 = (*dt)[*arg2].expr->expr_cdt.codedq.back()[3];
				}
			}






		}





		// Constraint check
		if (static_cast<int>(mode & acm::constraint) != 0) {
			arg1->expandConstraints();
			arg2->expandConstraints();


			for (const Constraint& c1 : arg1->constraints) {
				for (const Constraint& c2 : arg2->constraints) {
					int result = intersect(*arg1, c1, *arg2, c2, mode, dt);
					if (result == 0) {
						return false;
					}
					// If uncomparable, continue checking
				}
			}

		}

		// Check other modes
		if (static_cast<int>(mode & acm::changeable) != 0 && (arg1->changeable != arg2->changeable && arg1->changeable != A_ && arg2->changeable != A_)) {
			return false;
		}
		if (static_cast<int>(mode & acm::unknown) != 0 && arg1->unknown != arg2->unknown)
		{
			return false;
		}
		if (static_cast<int>(mode & acm::compatible) != 0 && arg1->compatible != arg2->compatible)
		{
			return false;
		}
		if (static_cast<int>(mode & acm::asc) != 0)
		{


			if (arg1->argFlag == S_Bs && nonAscRelatedArgNames.count(arg1->arg_s) == 0 && arg2->argFlag == S_Bs && nonAscRelatedArgNames.count(arg1->arg_s) == 0) {

				if (arg1->asc != arg2->asc) {
					return false;
				};

			}
		}


		auto dataintersect = [&dt, &cdth](ArgPtr arg1, ArgPtr arg2)->bool {
			if (dt == nullptr && (arg1->isVar() || arg2->isVar())) {
				return false;

			}
			if (!(dt->findall(*arg1) && dt->findall(*arg2))) {
				return false;
			}
			DataPtr data1 = DataPtr::make(*arg1, true, dt, cdth);
			DataPtr data2 = DataPtr::make(*arg2, true, dt, cdth);

			return *data1 == *data2 && (*data1) != datanull;

			};

		//if no vacant placeholder available, check the name (identifier, addr)
		if (
			(static_cast<int>(mode & acm::placeholder) != 0 && ((arg1->isPlaceholder == true && arg1->isBound == false) || (arg2->isPlaceholder == true && arg2->isBound == false))) == false

			) {



			// Name check (assuming this compares type and value(include identifier and fka))

			if (static_cast<int>(mode & acm::name) != 0)
			{

				if ((arg1->argFlag == arg2->argFlag && arg1->arg_i == arg2->arg_i && arg1->arg_s == arg2->arg_s && arg1->arg_fka == arg2->arg_fka && arg1->symbolID == arg2->symbolID) == false) {
					if (static_cast<int>(mode & acm::or_data) != 0) {
						if (!dataintersect(arg1, arg2)) {
							return false;
						}
					}
					else
					{
						return false;
					}

				}
			}

			//symbolID check
			if (static_cast<int>(mode & acm::symbol) != 0) {
				if (arg1->isSymbol || arg2->isSymbol)
				{
					if (arg1->isSymbol != arg2->isSymbol || arg1->symbolID != arg2->symbolID)
					{
						if (static_cast<int>(mode & acm::or_data) != 0) {
							if (!dataintersect(arg1, arg2)) {
								return false;
							}
						}
						else
						{
							return false;
						}
					}
				}
			}
		}
		else {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "Find vacant placeholder, jump over name, identifier, fka comparason"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			return true;
		}

		// identifier check
		if (static_cast<int>(mode & acm::identifier) != 0)
		{
			if ((arg1->argFlag == arg2->argFlag && arg1->arg_s == arg2->arg_s) == false) {
				if (static_cast<int>(mode & acm::or_data) != 0) {
					if (!dataintersect(arg1, arg2)) {
						return false;
					}
				}
				else
				{
					return false;
				}

			}
		}
		// addr check 
		if (static_cast<int>(mode & acm::fka) != 0)
		{
			if ((arg1->argFlag == arg2->argFlag && arg1->arg_fka == arg2->arg_fka) == false) {
				if (static_cast<int>(mode & acm::or_data) != 0) {
					if (!dataintersect(arg1, arg2)) {
						return false;
					}
				}
				else
				{
					return false;
				}

			}
		}

		// Data check
		if (static_cast<int>(mode & acm::data) != 0) {

			return dataintersect(arg1, arg2);

		}
		return true;
	}
	/// <summary>
		/// compare two args, whether arg1 < arg2. this function is just used to distinguish two args at specific aspects.
		/// </summary>
		/// <param name="arg1_"></param>
		/// <param name="arg2_"></param>
		/// <param name="mode"></param>
		/// <param name="dt"></param>
		/// <param name="cdth">used for EXPRZ_Dz arg comparason for data</param>
		/// <returns></returns>
	bool Arg::comparatorLess(const Arg& arg1_, const Arg& arg2_, acm mode /*= acm::name | acm::ref*/, DataTablePtrW dt/* = nullptr*/, CodeTableHash* cdth/* = nullptr*/) {
		//std::bitset<static_cast<int>(ArgCmpMode_COUNT)> modes(static_cast<int>(mode));
		ArgPtr arg1(&arg1_);
		ArgPtr arg2(&arg2_);


		if (static_cast<int>(mode & acm::ref) != 0) {

			while (arg1->argFlag == Y_Dz) {
				*arg1 = globalRefTable[*arg1];
			}
			while (arg2->argFlag == Y_Dz) {
				*arg2 = globalRefTable[*arg2];
			}
			//				if (arg1->argFlag == Y_Dz && arg2->argFlag == Y_Dz) {
			//#if debug
			//					{
			//						static int ignorecount = 0;
			//						cout << "Arg::operator ==(const Arg&)" << " " << "\tignorecount:["
			//							<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
			//							<< ":0" << ")" << endl;
			//			}
			//#endif
			//					//在预执行过程中，multimap相当于普通的map
			//					return globalRefTable.argfka_ref_map.find(arg1->arg_fka)->second
			//						== globalRefTable.argfka_ref_map.find(arg2->arg_fka)->second;
			//				}
			//				else {
			//					if (arg1->argFlag == Y_Dz) {
			//#if debug
			//						{
			//							static int ignorecount = 0;
			//							cout << "Arg::operator ==(const Arg&)" << " "
			//								<< "\tignorecount:[" << ignorecount++ << "\t]("
			//								<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			//		}
			//#endif
			//						return intersect(globalRefTable[*arg1], *arg2, mode, dt);
			//	}
			//					if (arg2->argFlag == Y_Dz) {
			//
			//						return *this == globalRefTable[arg];
			//					}
			//}

		}
		if (static_cast<int>(mode & acm::placeholder) != 0) {

			if (arg1->isPlaceholder && arg1->isBound)
			{
				if (dt == nullptr) {

#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = "datatable cannot be nullptr when searching the arg bound with placeholder, exit(-1)"; // Replace with actual info
						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
						std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
						OutputDebugString(winfo.c_str());
#endif
						cout << info << endl;
						ignoreCount++;
					}
#endif

				}

				if ((*dt)[*arg1].expr->type == EXPRESSIONHANDLER_ARG) {
					arg1 = ArgPtr(&((*dt)[*arg1].expr->expr_arg));
				}
				else {
					arg1 = ArgPtr::make();
					*arg1 = (*dt)[*arg1].expr->expr_cdt.codedq.back()[3];
				}
			}
			if (arg2->isPlaceholder && arg2->isBound)
			{
				if (dt == nullptr) {

#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = "datatable cannot be nullptr when searching the arg bound with placeholder, exit(-1)"; // Replace with actual info
						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
						std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
						OutputDebugString(winfo.c_str());
#endif
						cout << info << endl;
						ignoreCount++;
					}
#endif

				}

				if ((*dt)[*arg2].expr->type == EXPRESSIONHANDLER_ARG) {
					arg2 = ArgPtr(&((*dt)[*arg2].expr->expr_arg));
				}
				else {
					arg2 = ArgPtr::make();
					*arg2 = (*dt)[*arg2].expr->expr_cdt.codedq.back()[3];
				}
			}






		}





		// Constraint check
		if (static_cast<int>(mode & acm::constraint) != 0) {

			if (arg1->constraints > arg2->constraints) {
				return false;

			}
			else if (arg1->constraints < arg2->constraints) {
				return true;
			}

		}

		// Check other modes
		if (static_cast<int>(mode & acm::changeable) != 0) {
			if (arg1->changeable > arg2->changeable) {
				return false;
			}
			else if (arg1->changeable < arg2->changeable) {
				return true;
			}
		}
		if (static_cast<int>(mode & acm::unknown) != 0)
		{
			if (arg1->unknown > arg2->unknown)
			{
				return false;
			}
			else if (arg1->unknown < arg2->unknown) {
				return true;
			}
		}
		if (static_cast<int>(mode & acm::compatible) != 0)
		{
			if (arg1->compatible > arg2->compatible)
			{
				return false;
			}
			else if (arg1->compatible < arg2->compatible) {
				return true;
			}
		}
		if (static_cast<int>(mode & acm::asc) != 0)
		{


			FKA fka1 = arg1->asc;
			FKA fka2 = arg2->asc;
			if (arg1->argFlag != S_Bs || nonAscRelatedArgNames.count(arg1->arg_s) != 0) {
				fka1 = fkanull;
			}

			if (arg2->argFlag != S_Bs || nonAscRelatedArgNames.count(arg2->arg_s) != 0) {
				fka2 = fkanull;
			}

			if (fka1 > fka2) {
				return false;
			}
			else if (fka1 < fka2) {
				return true;
			}
		}


		auto dataLess = [&dt, &cdth](ArgPtr arg1, ArgPtr arg2)->bool {
			if (dt == nullptr && (arg1->isVar() || arg2->isVar())) {
				return false;

			}
			if (!(dt->findall(*arg1) && dt->findall(*arg2))) {
				return false;
			}
			DataPtr data1 = DataPtr::make(*arg1, true, dt, cdth);
			DataPtr data2 = DataPtr::make(*arg2, true, dt, cdth);

			return *data1 < *data2 && (*data1) != datanull;

			};
		if (static_cast<int>(mode & acm::or_data) != 0) {

			throw exception("acm::or_data is not allowed in comparator less for inverse comparason inconsistency.");

		}
		//if no vacant placeholder available, check the name (identifier, addr)
		if (
			(static_cast<int>(mode & acm::placeholder) != 0 && ((arg1->isPlaceholder == true && arg1->isBound == false) || (arg2->isPlaceholder == true && arg2->isBound == false))) == false

			) {

			// Name check (assuming this compares type and value(include identifier and fka))

			if (static_cast<int>(mode & acm::name) != 0)
			{

				if (arg1->isSymbol < arg2->isSymbol) {
					return true;
				}
				else if (arg1->isSymbol > arg2->isSymbol) {
					return false;

				}

				else if (arg1->isSymbol == arg2->isSymbol && arg2->isSymbol == 1) {

					if (arg1->symbolID < arg2->symbolID) {
						return true;
					}
					else if (arg1->symbolID > arg2->symbolID) {
						return false;
					}


				}


				else if (arg1->isSymbol == arg2->isSymbol && arg2->isSymbol == 0) {


					if (arg1->argFlag < arg2->argFlag) {
						return true;

					}
					else if (arg1->argFlag == arg2->argFlag) {

						if (arg1->argFlag == Sz) {
							if (arg1->arg_i < arg2->arg_i) {
								return true;
							}
							else if (arg1->arg_i > arg2->arg_i) {
								return false;
							}
						}
						if (arg1->isDz()) {
							if (arg1->arg_fka > arg2->arg_fka) {
								return false;
							}
							else if (arg1->arg_fka < arg2->arg_fka) {
								return true;
							}

						}
						if (arg1->isBs() || arg1->argFlag == Zf) {
							if (arg1->arg_s < arg2->arg_s) {
								return true;
							}
							else if (arg1->arg_s > arg2->arg_s) {
								return false;
							}

						}





					}



				}


			}


			//symbolID check
			if (static_cast<int>(mode & acm::symbol) != 0)
			{

				if (arg1->isSymbol < arg2->isSymbol) {
					return true;
				}
				else if (arg1->isSymbol > arg2->isSymbol) {
					return false;

				}

				else if (arg1->isSymbol == arg2->isSymbol && arg2->isSymbol == 1) {

					if (arg1->symbolID < arg2->symbolID) {
						return true;
					}
					else if (arg1->symbolID > arg2->symbolID) {
						return false;
					}


				}
			}





			// identifier check
			if (static_cast<int>(mode & acm::identifier) != 0)
			{
				if (arg1->argFlag == arg2->argFlag && (arg1->isBs() || arg1->argFlag == Zf)) {
					if (arg1->arg_s < arg2->arg_s) {
						return true;
					}
					else if (arg1->arg_s > arg2->arg_s) {
						return false;
					}

				}
				else {
					if (arg1->argFlag > arg2->argFlag) {
						return false;
					}
					else if (arg1->argFlag < arg2->argFlag) {
						return true;
					}

				}
			}
			// addr check 
			if (static_cast<int>(mode & acm::fka) != 0)
			{
				if (arg1->argFlag == arg2->argFlag && arg1->isDz()) {
					if (arg1->arg_fka < arg2->arg_fka) {
						return true;
					}
					else if (arg2->arg_fka < arg1->arg_fka) {
						return false;
					}

				}
				else {
					if (arg1->argFlag > arg2->argFlag) {
						return false;
					}
					else if (arg1->argFlag < arg2->argFlag) {
						return true;
					}

				}
			}

		}
		else {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "Find vacant placeholder, jump over name, identifier, fka comparason"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif

		}
		// Data check
		if (static_cast<int>(mode & acm::data) != 0) {

			if (dataLess(arg2, arg1)) {
				return false;
			}
			else if (dataLess(arg1, arg2)) {
				return true;

			}

		}

		return false; // arg1 == arg2
	}

	/// <summary>
	/// this is only used for sorting. Only sorted by its name(type and value),symbolID is reguarded as name.
	///  Don't add flagbit(compatible, unknown, changeable, isSymbol, isExpression, ) comparason in it
	/// </summary>
	/// <param name="arg"></param>
	/// <returns></returns>
	const bool Arg::operator<(const Arg& arg) const {

		if (argFlag < arg.argFlag) {
			return true;
		}
		else if (argFlag > arg.argFlag) {
			return false;
		}
		if (arg_i < arg.arg_i) {
			return true;
		}
		else if (arg_i > arg.arg_i) {
			return false;
		}
		if (arg_fka < arg.arg_fka) {
			return true;
		}
		else if (arg_fka > arg.arg_fka) {
			return false;
		}
		if (symbolID < arg.symbolID) {
			return true;
		}
		else if (symbolID > arg.symbolID) {
			return false;
		}
		if (arg_s < arg.arg_s) {
			return true;
		}
		else {
			return false;
		}

	}
	/**
	 * @attention 对于非引用，不比较asc等标志位，仅仅比较内容是否相同。不比较（GroundingProcess部分，如expr，symbol，constraint；placeholder只对其绑定内容进行比较。）
	 * 相当于 找到引用、placeholder所对应的arg后，进行acm::name 的intersection
	 * 对于引用与非引用的比较，则比较引用所指变量的asc与非引用变量的asc是否相同。
	 * 引用间的比较，仅比较引用路径是否相同。
	 * @attention 注意！！由于引用与非引用之间进行比较时比较asc位，此时应该选取asc位有效的变量进行比较
	 * @param arg
	 * @return
	 */
	bool Arg::operator==(const Arg& arg) const {

		return Arg::intersect(*this, arg);
		/*return argFlag == arg.argFlag && arg_i == arg.arg_i
			&& arg_fka == arg.arg_fka && arg_s == arg.arg_s;*/

	}
	bool Arg::operator!=(const Arg& arg) const {
		return !((*this) == arg);
	}
	void Arg::resetPlaceholder()
	{
		if (this->isPlaceholder == false)
		{
			return;
		}
		else
		{
			this->isBound = false;
			//this->binding_arg = nullptr;

		}

	}
	/**
	 * @attention same to assign
	 * @param arg
	 * @return
	 */
	Arg& Arg::operator=(const Arg& arg) {
		return assign(arg);
	}
	//Arg doesn't store any data, so its deepAssign and shallow assign is the same,just use assign is enough
	Arg& Arg::assign(const Arg& arg) {

		arg_i = arg.arg_i;
		arg_fka = arg.arg_fka;
		arg_s = arg.arg_s;
		this->ref_ar = arg.ref_ar;
		argFlag = arg.argFlag;
		formalArg = arg.formalArg;
		asc = arg.asc;
		this->changeable = arg.changeable;
		this->unknown = arg.unknown;
		this->compatible = arg.compatible;
		this->isPlaceholder = arg.isPlaceholder;
		this->isBound = arg.isBound;
		this->isExpression = arg.isExpression;
		this->isSymbol = arg.isSymbol;
		this->symbolID = arg.symbolID;
		this->ref_ar = arg.ref_ar;
		this->constraints = arg.constraints;
		return (*this);
	}
	Arg::Arg(Intg flag, const FKA& arg_fka) {
		argFlag = flag;
		this->arg_fka = arg_fka;
	}
	Arg::Arg(Intg flag, const Numb& arg_i) {
		argFlag = flag;
		this->arg_i = arg_i;
	}
	Arg::Arg(Intg flag, const Strg& arg_s) {
		argFlag = flag;
		this->arg_s = arg_s;
	}
	Arg::Arg(Intg flag, DataTablePtr const ar) {
		argFlag = flag;
		this->ref_ar = ref_ar;
	}
	Arg::Arg(Intg flag, const FKA& arg_fka, DataTablePtr const ar) {
		argFlag = flag;
		this->arg_fka = arg_fka;
		this->ref_ar = ref_ar;
	}
	Arg::Arg(Intg flag, const FKA& arg_fka, const Numb& arg_i, const Strg& arg_s) {
		argFlag = flag;
		this->arg_fka = arg_fka;
		this->arg_i = arg_i;
		this->arg_s = arg_s;
	}
	Arg::Arg(Intg flag, const FKA& arg_fka, const Numb& arg_i, const Strg& arg_s,
		const bool& formalArg) {
		argFlag = flag;
		this->arg_fka = arg_fka;
		this->arg_i = arg_i;
		this->arg_s = arg_s;
		this->formalArg = formalArg;
	}
	Arg::Arg(Intg flag, const FKA& arg_fka, const Numb& arg_i, const Strg& arg_s,
		const bool& formalArg, const FKA& asc) {
		argFlag = flag;
		this->arg_fka = arg_fka;
		this->arg_i = arg_i;
		this->arg_s = arg_s;
		this->formalArg = formalArg;
		this->asc = asc;
	}
	Arg::Arg(Intg flag, const FKA& arg_fka, const Numb& arg_i, const Strg& arg_s,
		const bool& formalArg, const Intg& changeable, const FKA& asc) {
		argFlag = flag;
		this->arg_fka = arg_fka;
		this->arg_i = arg_i;
		this->arg_s = arg_s;
		this->formalArg = formalArg;
		this->changeable = changeable;
		this->asc = asc;
	}

	Arg::Arg(Intg flag, const FKA& arg_fka, const Numb& arg_i, const Strg& arg_s, Intg symbolID,
		DataTablePtr ref_ar, const bool& formalArg, const Intg& changeable,
		const Intg& unknown, bool compatible, const FKA& asc, const set<Constraint>& constraints) {
		argFlag = flag;
		this->arg_fka = arg_fka;
		this->arg_i = arg_i;
		this->arg_s = arg_s;
		this->symbolID = symbolID;
		this->ref_ar = ref_ar;
		this->formalArg = formalArg;
		this->changeable = changeable;
		this->unknown = unknown;
		this->compatible = compatible;
		this->asc = asc;
		this->constraints = constraints;
	}
	Strg Arg::toBrief() const {
		/* if (this->argFlag == Y_Dz) {
		 cout << "Y_Dz\t" << "path:["
		 << globalRefTable.argfka_ref_map.find(arg_fka)->second.toStrg();
		 return globalRefTable[*this].toStrg();
		 }*/
		stringstream ss;
		ss.precision(PRECISION);
		//attribute
		if (formalArg == false) {
			ss << "out";
		}
		if (changeable == T_) {
			ss << "$";
		}
		else if (changeable == A_) {
			ss << "#";
		}
		if (compatible) {
			ss << "?";
		}
		if (isPlaceholder) {
			ss << "placeholder:";
		}
		if (isBound) {
			ss << "bound:";
		}
		if (unknown) {
			ss << "unknown:";
		}
		//constraint
		int i = 0;
		for (auto& c : constraints) {
			ss << c.instruction;
			if (c.args.size() > 0) {
				ss << "(";
			}
			for (auto& a : c.args) {
				if (i++ != 0) {
					ss << ",";
				}
				if (a.argFlag == Y_Dz) {
					ss << "Y_Dz(" << (a.arg_fka.toStrg()) << ")";
				}
				else if (a.argFlag == Z_Dz) {
					ss << "Z_Dz(" << (a.arg_fka.toStrg()) << ")";
				}
				else if (a.argFlag == EXPRZ_Dz) {
					ss << "EXPRZ_Dz(" << (a.arg_fka.toStrg()) << ")";
				}
				else if (a.argFlag == S_Dz) {
					ss << "S_Dz(" << (a.arg_fka.toStrg()) << ")";
				}
				else if (a.argFlag == S_Bs || a.argFlag == M_Bs || a.argFlag == X_Bs) {
					ss << a.arg_s;
				}
				else if (a.argFlag == S_LST) {
					ss << "LST{" << a.arg_s << a.arg_fka.toStrg() << "}";
				}
				else if (a.argFlag == S_MAP) {
					ss << "MAP{" << a.arg_s << a.arg_fka.toStrg() << "}";
				}
				else if (a.argFlag == MULTIMAP) {
					ss << "MULTIMAP{" << a.arg_s << a.arg_fka.toStrg() << "}";
				}
				else if (a.argFlag == S_SET) {
					ss << "SET{" << a.arg_s << a.arg_fka.toStrg() << "}";
				}
				else if (a.argFlag == S_MULTISET) {
					ss << "MULTISET{" << a.arg_s << a.arg_fka.toStrg() << "}";
				}
				else if (a.argFlag == Sz) {
					ss << a.arg_i;
				}
				else if (a.argFlag == Zf) {
					ss << "\"" << a.arg_s << "\"";
				}
			}
			if (c.args.size() > 0) {
				ss << ")";
			}
			ss << ":";
		}

		//argscope

		if (this->argFlag == S_Bs && nonAscRelatedArgNames.count(this->arg_s) == 0 && (asc.fileKey != "" || asc.addr.size() > 0)) {
			ss << "asc(" << asc.toStrg() << "):";
		}
		//name
		if (argFlag == Y_Dz) {
			ss << "Y_Dz(" << (this->arg_fka.toStrg()) << ")";
		}
		else if (argFlag == Z_Dz) {
			ss << "Z_Dz(" << (this->arg_fka.toStrg()) << ")";
		}
		else if (argFlag == EXPRZ_Dz) {
			ss << "EXPRZ_Dz(" << (this->arg_fka.toStrg()) << ")";
		}
		else if (argFlag == S_Dz && this->arg_fka.addr != addrnull) {
			ss << "S_Dz(" << this->arg_fka.toStrg() << ")";
		}
		else if (argFlag == S_Bs || argFlag == M_Bs || argFlag == X_Bs) {
			ss << arg_s;
		}
		else if (argFlag == S_LST) {
			ss << "LST{" << arg_s << arg_fka.toStrg() << "}";
		}
		else if (argFlag == S_MAP) {
			ss << "MAP{" << arg_s << arg_fka.toStrg() << "}";
		}
		else if (argFlag == MULTIMAP) {
			ss << "MULTIMAP{" << arg_s << arg_fka.toStrg() << "}";
		}
		else if (argFlag == S_SET) {
			ss << "SET{" << arg_s << arg_fka.toStrg() << "}";
		}
		else if (argFlag == S_MULTISET) {
			ss << "MULTISET{" << arg_s << arg_fka.toStrg() << "}";
		}
		else if (argFlag == Sz) {
			ss << arg_i;
		}
		else if (argFlag == Zf) {
			ss << "\"" << arg_s << "\"";
		}

		if (isSymbol) {
			ss << "sym(" << symbolID << ")";
		}

		return ss.str();
	}

	Strg Arg::toStrg() const {
		/* if (this->argFlag == Y_Dz) {
		 cout << "Y_Dz\t" << "path:["
		 << globalRefTable.argfka_ref_map.find(arg_fka)->second.toStrg();
		 return globalRefTable[*this].toStrg();
		 }*/
		 /*stringstream ss;
		 ss.precision(PRECISION);
		 ss << "argFlag:[" << argFlag << "]\t" << "arg_i:[" << arg_i << "]\t"
			 << "arg_fka:[" << arg_fka.toStrg() << "]\t" << "arg_s:[" << arg_s
			 << "]\t" << "formalArg:["
			 << formalArg << "]\t" << "changeable:[" << changeable << "]\t"
			 << "asc:[" << asc.toStrg() << "]\t" << endl;

		 return ss.str();*/
		return toBrief();
	}


	/*************************************


	Data


	*****************************************/

	/*
			Do not call manually， only called by make_shared_ptr
		*/
	void Data::setOnHeap() {
		this->on_heap = true;
		return;
	}
	bool Data::onHeap() const {
		return this->on_heap;
	}
	DataPtr Data::getThisPtr() const {
		if (on_heap == true) {
			return thisPtr;
		}
		else {
			return DataPtr(this);
		}
	}
	void Data::deepAssignFrom(DataPtr dt) {
		dt->deepAssignTo(DataPtr(this));
	}

	Data::~Data() {
		//this->clear();
		//removeExpression();
	}

	void Data::addExpression(ExpressionHandlerPtr eh, bool duplicate) {

		if (duplicate == false) {
			this->expr = eh;
		}
		else {
			this->expr = ExpressionHandlerPtr::make();
			this->expr->deepAssign(eh);
		}

	}
	/**
	 * @brief 根据Arg的类型创建一个同类型的data，此arg有一个根据constraint确定的默认的值
	 * todo: currently, we just create default data for numbers, default data for string is not supported now.
	 * @param arg
	 * @param assign Adopt the value of arg to initial the value of Data obj
	 * @param dt & cdth Used for looking up.
	 */
	Data::Data(const Arg& arg, bool assign /*= true*/, DataTablePtrW dt/* = nullptr*/, CodeTableHash* cdth/* = nullptr*/) {
		/*if (arg.isDz()) {
			dataFlagBit = arg.argFlag;
			if (assign) {
				this->content_fka = arg.arg_fka;
			}

		}
		else if (arg.isBs()) {
			dataFlagBit = arg.argFlag;
			if (assign) {
				this->content_s = arg.arg_s;
			}
		}*/

		this->dataFlagBit = S_Dz; //default to be zero, means nothing, no value.

		if (arg.constraints.count(Constraint("string")) > 0 || arg.argFlag == Zf) {
			if (assign) {
				this->dataFlagBit = Zf;
				content_s = arg.arg_s; //maybe ""

			}
		}
		else if (arg.constraints.count(Constraint("number")) > 0 || arg.argFlag == Sz) {
			if (arg.argFlag == Sz && assign) {
				dataFlagBit = Sz;
				content_i = arg.arg_i;
			}
			else if (arg.constraints.count(Constraint("number")) && assign) {
				dataFlagBit = Sz;
				CombinedInterval<Numb> resi;
				resi.addRelationalInterval(OpType::lte, std::numeric_limits<Numb>::infinity());
				bool includeInterval = false;
				CombinedInterval<Numb> cmbi;
				for (auto& c : arg.constraints) {
					if (c.instruction == "lt") {
						cmbi.addRelationalInterval(OpType::lt, c.args.front().arg_i);

					}
					else if (c.instruction == "lte") {
						cmbi.addRelationalInterval(OpType::lte, c.args.front().arg_i);

					}
					else if (c.instruction == "gt") {
						cmbi.addRelationalInterval(OpType::gt, c.args.front().arg_i);

					}
					else if (c.instruction == "gte") {
						cmbi.addRelationalInterval(OpType::gte, c.args.front().arg_i);

					}
					else if (c.instruction == "eq") {
						cmbi.addRelationalInterval(OpType::eq, c.args.front().arg_i);

					}
					else if (c.instruction == "ne") {
						cmbi.addRelationalInterval(OpType::ne, c.args.front().arg_i);

					}
					if (cmbi.size() > 0) {
						includeInterval = true;
						resi = resi ^ cmbi;
						cmbi = CombinedInterval<Numb>();
					}

				}

				if (resi.size() > 0) {
					content_i = resi.getDefaultValue();
				}
				else if (includeInterval == false) {
					content_i = 0;
				}
				else {

#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = "conflict constraints, exit(-1)"; // Replace with actual info
						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
						std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
						OutputDebugString(winfo.c_str());
#endif
						cout << info << endl;
						ignoreCount++;
					}
#endif
					cerr << "conflict constraints, exit(-1)" << endl;
					exit(-1);
				}

			}

		}
		else if (arg.argFlag == EXPRZ_Dz) {
			if (assign) {
				dataFlagBit = EXPRZ_Dz;
				this->isExpression = true;
				if (cdth != nullptr) {
					FKA exprfka = cdth->getNext(COOL_M, arg.arg_fka);
					this->expr = ExpressionHandlerPtr::make(cdth->copyTree(exprfka));
				}
				else {

#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = "error exit"; // Replace with actual info
						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
						std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
						OutputDebugString(winfo.c_str());
#endif
						cout << info << endl;
						ignoreCount++;
					}
#endif

					throw arg;
				}
			}
		}
		if (arg.argFlag == S_AR) {
			dataFlagBit = S_AR;
			/*safe_delete(content_ar);*/
			if (assign) {
				//don't assigned here. assigned by executing class code.

			}

		}


	}
	/**
	 * @brief 将data的value转化为string，仅可以在执行过程中调用.
	 * @return
	 */
	Strg Data::valueToStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		if (this->dataFlagBit == Sz) {
			ss << this->content_i;
		}
		else if (this->dataFlagBit == Zf) {
			ss << this->content_s;
		}
		else if (this->dataFlagBit == COOL_LST) {
			ss << "{";
			Intg len = this->content_lst->size();
			auto it = this->content_lst->begin();
			for (Intg i = 0; i < len; ++i, ++it) {
				ss << it->valueToStrg();
				if (i < len - 1) {
					ss << ",";
				}
			}
			ss << "}";
		}
		else if (this->dataFlagBit == SET) {
			ss << "{";
			Intg len = this->content_set->size();
			auto it = this->content_set->begin();
			for (Intg i = 0; i < len; ++i, ++it) {
				ss << it->valueToStrg();
				if (i < len - 1) {
					ss << ",";
				}
			}
			ss << "}";
		}
		else if (this->dataFlagBit == MULTISET) {
			ss << "{";
			Intg len = this->content_multiset->size();
			auto it = this->content_multiset->begin();
			for (Intg i = 0; i < len; ++i, ++it) {
				ss << it->valueToStrg();
				if (i < len - 1) {
					ss << ",";
				}
			}
			ss << "}";
		}
		else if (this->dataFlagBit == MAP) {
			ss << "{";
			Intg len = this->content_map->size();
			auto it = this->content_map->begin();
			for (Intg i = 0; i < len; ++i, ++it) {
				ss << it->first.valueToStrg();
				ss << ":";

				ss << it->second.valueToStrg();
				if (i < len - 1) {
					ss << ",";
				}
			}
			ss << "}";
		}
		else if (this->dataFlagBit == MULTIMAP) {
			ss << "{";
			Intg len = this->content_multimap->size();
			auto it = this->content_multimap->begin();
			for (Intg i = 0; i < len; ++i, ++it) {
				ss << it->first.valueToStrg();
				ss << ":";
				ss << "{";
				Intg lenmultiset = it->second.size();
				auto itmultiset = it->second.begin();
				for (Intg imultiset = 0; imultiset < lenmultiset;
					++imultiset, ++itmultiset) {
					ss << itmultiset->valueToStrg();
					if (i < len - 1) {
						ss << ",";
					}
				}
				ss << "}";
				if (i < len - 1) {
					ss << ",";
				}
			}
			ss << "}";
		}
		else if (this->dataFlagBit == S_PTR) {
			return this->content_ptr->valueToStrg();
		}
		return ss.str();
	}
	/**
 * @brief simply assign the @arg data_ to this without copying memory inner pointers pointing at.
 * @param data_
 * @attention this function is unsafe and won't duplicate the memory the pointer pointing at,
 * and shouldn't be exclusively used in vm_execute process.(you can use with deepAssign to copy memory)
 * @return
 */

	Data& Data::shallowAssign(const Data& data_) {

		if (this != &data_) {
			this->clear();
		}
		else {
			return (*this);
		}
		const Data* dtp = &data_;
		//    while (data_.dataFlagBit == S_PTR) {
		//        dtp = data_.content_ptr;
		//    }

		dataFlagBit = (*dtp).dataFlagBit;
		isReferenceVar = (*dtp).isReferenceVar;
		content_i = (*dtp).content_i;
		content_s = (*dtp).content_s;
		content_lst = (*dtp).content_lst;
		content_set = (*dtp).content_set;
		content_multiset = (*dtp).content_multiset;
		content_map = (*dtp).content_map;
		content_multimap = (*dtp).content_multimap;
		content_fka = (*dtp).content_fka;
		content_ptr = (*dtp).content_ptr;
		content_ar = (*dtp).content_ar;
		//    if ((*dtp).dataFlagBit == S_AR) {
		//        if ((*dtp).content_ar != nullptr) {
		//            safe_delete(content_ar);
		//
		//            content_ar = (*dtp).content_ar;
		//        }
		//    }
		//    if ((*dtp).dataFlagBit == COOL_LST) {
		//        if ((*dtp).content_lst != nullptr) {
		//            safe_delete(content_lst);
		//            content_lst = (*dtp).content_lst;
		//        }
		//    }
		//    if ((*dtp).dataFlagBit == S_MAP) {
		//        if ((*dtp).content_map != nullptr) {
		//            safe_delete(content_map);
		//            content_map = (*dtp).content_map;
		//        }
		//    }
		//    if ((*dtp).dataFlagBit == S_MULTIMAP) {
		//        if ((*dtp).content_multimap != nullptr) {
		//            safe_delete(content_multimap);
		//            content_multimap = (*dtp).content_multimap;
		//        }
		//    }
		//    if ((*dtp).dataFlagBit == S_SET) {
		//        if ((*dtp).content_set != nullptr) {
		//            safe_delete(content_set);
		//            content_set = (*dtp).content_set;
		//        }
		//    }
		//    if ((*dtp).dataFlagBit == S_MULTISET) {
		//        if ((*dtp).content_multiset != nullptr) {
		//            safe_delete(content_multiset);
		//            content_multiset = (*dtp).content_multiset;
		//        }
		//    }

		//    scopeStructureAddr = data_.scopeStructureAddr;
		//    linkAddr = data_.linkAddr;
		return (*this);
	}

	//default to be deepAssign
	Data& Data::operator=(const Data& data_) {
		this->deepAssignFrom(DataPtr(&data_));
		return *this;
	}
	Data::Data(const Data& data_) {
		this->deepAssignFrom(DataPtr(&data_));
	}
	//default to be deepAssign
	Data& Data::operator=(Data&& data_) {
		this->shallowAssign(data_);
		return *this;
	}
	[[deprecated("This function is banned for not suitable for current ")]]
	Data& Data::operator=(const Arg& arg) {
		exception e("using banned function Data::operator=(const Arg& arg), this function will cause ambiguous");
		throw e;


		if (arg.argFlag == S_Dz) {
			dataFlagBit = S_Dz;
			content_fka = arg.arg_fka;

		}
		else if (arg.argFlag == S_Bs) {
			dataFlagBit = S_Bs;
			content_s = arg.arg_s;
		}
		else if (arg.argFlag == Zf) {
			dataFlagBit = Zf;
			content_s = arg.arg_s;
		}
		else if (arg.argFlag == Sz) {
			dataFlagBit = Sz;
			content_i = arg.arg_i;
		}
		if (arg.argFlag == S_AR) {
			dataFlagBit = S_AR;
			/*safe_delete(content_ar);*/
			//if (arg.ref_ar != nullptr) {
			//	content_ar = DataTablePtr::make();
			//	DataTable::deepAssign(content_ar, arg.ref_ar, true);
			//	/*(*content_ar) = *(arg.ref_ar);*/
			//}

		}

		return (*this);
	}
	/**
	 * @brief judge whether two data are the same, and will compare pointers member ..
	 * @param data_
	 * @return
	 */
	bool Data::operator==(const Data& data_) const {
		if (this == &data_) {
			return true;
		}
		if (data_.dataFlagBit == S_PTR) {
			return (*this) == (*data_.content_ptr);
		}
		if (this->dataFlagBit == S_PTR) {
			return (*this->content_ptr) == data_;
		}
		if (dataFlagBit == data_.dataFlagBit) {
			if (dataFlagBit == Sz) {
				return this->content_i == data_.content_i;
			}
			else if (dataFlagBit == Zf) {
				return this->content_s == data_.content_s;
			}
			else if (dataFlagBit == COOL_LST) {
				return *this->content_lst == *data_.content_lst;
			}
			else if (dataFlagBit == SET) {
				return *this->content_set == *data_.content_set;
			}
			else if (dataFlagBit == MULTISET) {
				return *this->content_multiset == *data_.content_multiset;
			}
			else if (dataFlagBit == MAP) {
				return *this->content_map == *data_.content_map;
			}
			else if (dataFlagBit == MULTIMAP) {
				return *this->content_multimap == *data_.content_multimap;
			}
			else if (dataFlagBit == S_AR) {

				return this->content_ar->operator ==(*data_.content_ar);

			}
			return true;

		}
		else {
			return false;
		}

	}
	/**
	 * @brief this function provides a way to order a list of Data, it doesn't have pratical usage meaning .
	 * @attention this function will compare the memory the inner pointer pointing at.
	 * @param dt
	 * @return
	 */
	bool Data::operator<(const Data& dt) const {
		if (this->dataFlagBit == dt.dataFlagBit) {
			if (this->dataFlagBit == Sz) {
				return this->content_i < dt.content_i;
			}
			else if (this->dataFlagBit == Zf) {
				return this->content_s.compare(dt.content_s);
			}
			else if (this->dataFlagBit == S_AR) {
				return this->content_ar->operator <(*dt.content_ar);
			}
			else if (this->dataFlagBit == S_MAP) {
				Intg lenmap = min(this->content_map->size(),
					dt.content_map->size());
				auto itcontent_map1 = this->content_map->begin();
				auto itcontent_map2 = dt.content_map->begin();
				while (--lenmap >= 0) {
					if (itcontent_map1->first < itcontent_map2->first) {
						return true;

					}
					else if (itcontent_map1->first == itcontent_map2->first) {
						if (itcontent_map1->second < itcontent_map2->second) {
							return true;
						}
						else if (itcontent_map1->second
							== itcontent_map2->second) {
							++itcontent_map1;
							++itcontent_map2;
						}
						else {
							return false;
						}

					}
					else {
						return false;
					}

				}
				if (content_map->size() < dt.content_map->size()) {
					return true;
				}
				else if (content_map->size() > dt.content_map->size()) {
					return false;
				}

			}
			else if (this->dataFlagBit == S_MULTIMAP) {
				Intg lenmultimap = min(this->content_multimap->size(),
					dt.content_multimap->size());
				auto itcontent_multimap1 = this->content_multimap->begin();
				auto itcontent_multimap2 = dt.content_multimap->begin();
				while (--lenmultimap >= 0) {
					if (itcontent_multimap1->first < itcontent_multimap2->first) {
						return true;

					}
					else if (itcontent_multimap1->first
						== itcontent_multimap2->first) {

						auto& content_multiset_1 = itcontent_multimap1->second;
						auto& content_multiset_2 = itcontent_multimap2->second;
						Intg lenmultiset = min(content_multiset_1.size(),
							content_multiset_2.size());
						auto itcontent_multiset_1 = content_multiset_1.begin();
						auto itcontent_multiset_2 = content_multiset_2.begin();
						while (--lenmultiset >= 0) {
							if ((*itcontent_multiset_1) < (*itcontent_multiset_2)) {
								return true;

							}
							else if ((*itcontent_multiset_1)
								== (*itcontent_multiset_1)) {

								++itcontent_multiset_1;
								++itcontent_multiset_2;

							}
							else {
								return false;
							}

						}
						if (content_multiset_1.size() < content_multiset_2.size()) {
							return true;
						}
						else if (content_multiset_1.size()
						> content_multiset_2.size()) {
							return false;
						}
						else {
							++itcontent_multimap1;
							++itcontent_multimap2;
						}

					}
					else {
						return false;
					}

				}
				if (content_multimap->size() < dt.content_multimap->size()) {
					return true;
				}
				else if (content_multimap->size()
					>= dt.content_multimap->size()) {
					return false;
				}

			}
			else if (this->dataFlagBit == S_SET) {
				Intg lenset = min(this->content_set->size(),
					dt.content_set->size());
				auto itcontent_set1 = this->content_set->begin();
				auto itcontent_set2 = dt.content_set->begin();
				while (--lenset >= 0) {
					if ((*itcontent_set1) < (*itcontent_set2)) {
						return true;

					}
					else if ((*itcontent_set1) == (*itcontent_set1)) {

						++itcontent_set1;
						++itcontent_set2;

					}
					else {
						return false;
					}

				}
				if (content_set->size() < dt.content_set->size()) {
					return true;
				}
				else if (content_set->size() >= dt.content_set->size()) {
					return false;
				}

			}
			else if (this->dataFlagBit == S_MULTISET) {
				Intg lenmultiset = min(this->content_multiset->size(),
					dt.content_multiset->size());
				auto itcontent_multiset1 = this->content_multiset->begin();
				auto itcontent_multiset2 = dt.content_multiset->begin();
				while (--lenmultiset >= 0) {
					if ((*itcontent_multiset1) < (*itcontent_multiset2)) {
						return true;

					}
					else if ((*itcontent_multiset1) == (*itcontent_multiset1)) {

						++itcontent_multiset1;
						++itcontent_multiset2;

					}
					else {
						return false;
					}

				}
				if (content_multiset->size() < dt.content_multiset->size()) {
					return true;
				}
				else if (content_multiset->size()
					>= dt.content_multiset->size()) {
					return false;
				}

			}
			else if (this->dataFlagBit == COOL_LST) {
				Intg lenmultiset = min(this->content_lst->size(),
					dt.content_lst->size());
				auto itcontent_lst1 = this->content_lst->begin();
				auto itcontent_lst2 = dt.content_lst->begin();
				while (--lenmultiset >= 0) {
					if ((*itcontent_lst1) < (*itcontent_lst2)) {
						return true;

					}
					else if ((*itcontent_lst1) == (*itcontent_lst1)) {

						++itcontent_lst1;
						++itcontent_lst2;

					}
					else {
						return false;
					}

				}
				if (content_lst->size() < dt.content_lst->size()) {
					return true;
				}
				else if (content_lst->size() >= dt.content_lst->size()) {
					return false;
				}

			}
			return false;
		}
		else {
			return this->dataFlagBit < dt.dataFlagBit;
		}
	}
	/**
	 * @brief transform the data value to a number.
	 * @attention only active when dataFlagType is Sz/Zf or a lst/set/multiset who have
	 * single element. if a lst/set/multiset has more than one elements, transform the first
	 * element data value to numb. if a lst/set/multiset has no element, error.
	 *
	 * @return
	 */
	Numb Data::valueToNumb() const {
		if (this->dataFlagBit == Zf) {
			return toNumb(this->content_s);
		}
		else if (this->dataFlagBit == Sz) {
			return this->content_i;
		}
		else if (this->dataFlagBit == S_LST && this->content_lst->size() > 0) {
			return this->content_lst->begin().operator *().valueToNumb();
		}
		else if (this->dataFlagBit == S_SET && this->content_set->size() > 0) {
			return this->content_set->begin().operator *().valueToNumb();
		}
		else if (this->dataFlagBit == S_MULTISET
			&& this->content_multiset->size() > 0) {
			return this->content_multiset->begin().operator *().valueToNumb();
		}
		else if (this->dataFlagBit == S_PTR) {
			return this->content_ptr->valueToNumb();
		}
		else {

			THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid conversion from Data(" + this->toStrg() + ") to Numb.");


		}
	}
	/**
	 * @brief transform the data value to a bool.
	 * @return not 0,"",{}
	 */
	bool Data::valueToBool() const {
		if (this->dataFlagBit == Zf) {
			return this->content_s != "";
		}
		else if (this->dataFlagBit == Sz) {
			return this->content_i != 0;
		}
		else if (this->dataFlagBit == S_LST || this->dataFlagBit == S_SET
			|| this->dataFlagBit == S_MULTISET || this->dataFlagBit == S_MAP
			|| this->dataFlagBit == S_MULTIMAP) {
			return this->containerSize() > 0;
		}
		else if (this->dataFlagBit == S_PTR) {
			return this->content_ptr->valueToNumb();
		}
		else {


			THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid conversion from Data(" + this->toStrg() + ") to Bool.");
		}
	}
	/**
	 * @brief if a Data object's type is a container, return the size of the container.
	 * else if type is strg and equal "", return 0, else return 1.
	 * if the type is ptr, return the size of the object the pointer pointing at
	 * @attention
	 * @return
	 */
	Intg Data::containerSize() const {
		if (this->dataFlagBit == S_PTR) {
			return this->content_ptr->containerSize();

		}
		else if (this->dataFlagBit == Sz) {
			return 1;

		}
		else if (this->dataFlagBit == Zf) {
			return this->content_s != "";

		}
		else if (this->dataFlagBit == S_AR) {
			return 1;

		}
		else if (this->dataFlagBit == S_LST) {
			return this->content_lst->size();

		}
		else if (this->dataFlagBit == S_SET) {
			return this->content_set->size();

		}
		else if (this->dataFlagBit == S_MULTISET) {
			return this->content_multiset->size();

		}
		else if (this->dataFlagBit == S_MAP) {
			return this->content_map->size();

		}
		else if (this->dataFlagBit == S_MULTIMAP) {
			return this->content_multimap->size();

		}
		else {
			return 0;
		}

	}


	Data::Data(const Numb& num) {
		dataFlagBit = Sz;
		content_i = num;
	}
	Data::Data(const Intg& type, void* par_or_plst) {
		this->dataFlagBit = type;
		if (par_or_plst) {
			if (type == S_AR) {
				content_ar = *(DataTablePtr*)(par_or_plst);
			}
			else if (type == COOL_LST) {
				content_lst = *(DataLstPtr*)par_or_plst;
			}
			else if (type == S_PTR) {
				content_ptr = *(DataPtr*)(par_or_plst);

			}
		}
		else {
			if (type == S_AR) {
				content_ar = DataTablePtr::make();
			}
			else if (type == COOL_LST) {
				content_lst = DataLstPtr::make();
			}
			else if (type == S_PTR) {
				content_ptr = nullptr;

			}
		}

	}
	Data::Data(const Strg& strg) {
		dataFlagBit = Zf;
		content_s = strg;
	}
	/**
	 * @brief duplicate the data object and the memory the inner pointor pointing at.
	 * @return
	 */
	Data Data::deepAssign() const {

		Data* this_ = const_cast<Data*>(this);
		Data dtdup;
		DataPtr dt = this_->getThisPtr();
		dtdup.dataFlagBit = this->dataFlagBit;

		while (dt->dataFlagBit == S_PTR) {
			dt = dt->content_ptr;
		}
		if (dt->isExpression) {
			dtdup.isExpression = true;
			dtdup.addExpression(dt->expr, true);

		}
		if (dt->dataFlagBit == Sz || dt->dataFlagBit == Zf) {
			dtdup.dataFlagBit = dt->dataFlagBit;
			dtdup.content_i = dt->content_i;
			dtdup.content_s = dt->content_s;
			return dtdup;
		}
		else if (dt->dataFlagBit == S_LST) {

			dtdup.content_lst = DataLstPtr::make();
			for (const auto& it : *dt->content_lst) {
				dtdup.content_lst->push_back(it.deepAssign());
			}
			return dtdup;

		}
		else if (dt->dataFlagBit == S_SET) {

			dtdup.content_set = DataSetPtr::make();
			for (const auto& it : *dt->content_set) {
				dtdup.content_set->insert(it.deepAssign());
			}
			return dtdup;

		}
		else if (dt->dataFlagBit == S_MULTISET) {

			dtdup.content_multiset = DataMultiSetPtr::make();
			for (const auto& it : *dt->content_multiset) {
				dtdup.content_multiset->insert(it.deepAssign());
			}
			return dtdup;

		}
		else if (dt->dataFlagBit == S_MAP) {

			dtdup.content_map = DataMapPtr::make();
			for (const auto& it : *dt->content_map) {
				dtdup.content_map->insert(
					make_pair(it.first.deepAssign(), it.second.deepAssign()));
			}
			return dtdup;

		}
		else if (dt->dataFlagBit == S_MULTIMAP) {

			dtdup.content_multimap = DataMultiMapPtr::make();
			for (const auto& it : *dt->content_multimap) {
				auto mtset = multiset<Data>();
				for (const auto& it2 : it.second) {
					mtset.insert(it2.deepAssign());
				}
				dtdup.content_multimap->insert(make_pair(it.first.deepAssign(), mtset));
			}
			return dtdup;

		}


		return datanull;

	}
	Strg Data::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		if (this->dataFlagBit == S_Dz) {
			ss << "null";
		}
		else if (this->dataFlagBit == Sz) {
			ss << this->content_i;
		}
		else if (this->dataFlagBit == Zf) {
			ss << "\"" << this->content_s << "\"";
		}
		else if (this->dataFlagBit == S_AR) {
			if (this->content_ar != nullptr) {
				ss << "AR(" << this->content_ar->scopeStructureFKA.toStrg() << ")";
			}
			else {
				ss << "AR(null)";
			}
		}
		else if (this->dataFlagBit == S_PTR) {
			ss << "PTR(" << this->content_ptr->toStrg() << ")";
		}
		else if (this->dataFlagBit == S_MAP) {
			ss << this->content_i;
		}
		else if (this->dataFlagBit == Sz) {
			ss << this->content_i;
		}
		else if (this->dataFlagBit == S_LST) {
			ss << "LST" << COOLANG::toStrg<deque<Data> >(*content_lst);//already with { xxx }
		}
		else if (this->dataFlagBit == S_SET) {
			ss << "SET" << COOLANG::toStrg<set<Data> >(*content_set);//already with { xxx }
		}
		else if (this->dataFlagBit == S_MULTISET) {
			ss << "MULTISET" << COOLANG::toStrg<multiset<Data> >(*content_multiset);//already with { xxx }
		}
		else if (this->dataFlagBit == S_MAP) {
			ss << "MAP" << COOLANG::toStrg<Data, Data >(*content_map);//already with { xxx }
		}
		else if (this->dataFlagBit == S_MULTIMAP) {
			int i = 0;
			ss << "MULTIMAP";
			ss << "{";
			for (auto& p : *content_multimap) {
				if (i++ != 0) {
					ss << ",";
				}
				ss << p.first.toStrg() << ":"
					<< COOLANG::toStrg<multiset<Data>>(p.second);
			}
			ss << "}";//already with { xxx }
		}

		if (this->isExpression) {
			if (this->expr == nullptr) {
				ss << "-expr(null)";
			}
			else {
				ss << "-expr(" << this->expr->serialNumber << ")";
				ss << hline;
				if (this->expr->type == EXPRESSIONHANDLER_ARG) {
					ss << calign("expr_arg(" + this->expr->expr_arg.toStrg() + ")", 96);
				}
				else {
					ss << this->expr->expr_cdt.toStrg();
				}
				ss << hline;
			}

		}





		return ss.str();
	}

	/**
	 * @brief duplicate the data object and the memory the inner pointor pointing at to a pointed memory.
	 * @return
	 */
	void Data::deepAssignTo(DataPtr dtdup) const {
		if (dtdup == nullptr) {
			dtdup = DataPtr::make();
		}
		else {
			dtdup->clear();
		}
		dtdup->dataFlagBit = this->dataFlagBit;
		Data* dt = const_cast<Data*>(this);
		while (dt->dataFlagBit == S_PTR) {
			dt = dt->content_ptr.get();
		}
		if (dt->isExpression) {
			dtdup->isExpression = true;
			dtdup->addExpression(dt->expr, true);

		}
		if (dt->dataFlagBit == Sz || dt->dataFlagBit == Zf) {
			dtdup->dataFlagBit = dt->dataFlagBit;
			dtdup->content_i = dt->content_i;
			dtdup->content_s = dt->content_s;
			//*dtdup = *this;
		}
		else if (dt->dataFlagBit == S_LST) {

			dtdup->content_lst = DataLstPtr::make();
			for (const auto& it : *dt->content_lst) {
				dtdup->content_lst->push_back(it.deepAssign());
			}

		}
		else if (dt->dataFlagBit == S_SET) {

			dtdup->content_set = DataSetPtr::make();
			for (const auto& it : *dt->content_set) {
				dtdup->content_set->insert(it.deepAssign());
			}

		}
		else if (dt->dataFlagBit == S_MULTISET) {

			dtdup->content_multiset = DataMultiSetPtr::make();
			for (const auto& it : *dt->content_multiset) {
				dtdup->content_multiset->insert(it.deepAssign());
			}

		}
		else if (dt->dataFlagBit == S_MAP) {

			dtdup->content_map = DataMapPtr::make();
			for (const auto& it : *dt->content_map) {
				dtdup->content_map->insert(
					make_pair(it.first.deepAssign(), it.second.deepAssign()));
			}

		}
		else if (dt->dataFlagBit == S_MULTIMAP) {

			dtdup->content_multimap = DataMultiMapPtr::make();
			for (const auto& it : *dt->content_multimap) {
				auto mlset = multiset<Data>();
				for (const auto& it2 : it.second) {
					mlset.insert(it2.deepAssign());
				}
				dtdup->content_multimap->insert(make_pair(it.first.deepAssign(), mlset));
			}

		}

	}


	/**
	 * @brief this function will delete the memory inner pointer pointing at
	 * @attention
	 * if its type is S_PTR or is a reference var, it won't delete the memory the inner pointer pointing at on its own
	 * using pointer reference counter
	 *
	 */

	void Data::clear() {
		if (isReferenceVar || this->dataFlagBit == Sz || this->dataFlagBit == Zf
			|| this->dataFlagBit == S_PTR) {
			this->content_i = 0;
			this->content_s = "";
			this->content_ar = nullptr;
			this->content_lst = nullptr;
			this->content_set = nullptr;
			this->content_multiset = nullptr;
			this->content_map = nullptr;
			this->content_multimap = nullptr;
			return;
		}
		if (dataFlagBit == S_AR && content_ar != nullptr && content_ar->type == SYSB) {
			content_ar == nullptr;
			//			try {
			//
			//				DELETE_SYSB(content_ar);
			//			}
			//			catch (exception& e) {
			//#if debug
			//				{
			//					static int ignorecount = 0;
			//					std::cout << "Data::~Data()" << e.what() << "\tignorecount:["
			//						<< ignorecount++ << "\t](" << __FILE__ << ":"
			//						<< __LINE__ << ":0" << ")" << std::endl;
			//				}
			//#endif
			//			}
		}
		else if (dataFlagBit == S_LST && content_lst != nullptr) {
			for (auto& elem : *content_lst) {
				elem.clear();
			}
			content_lst->clear();
			//safe_delete(content_lst);
			content_lst = nullptr;
		}
		else if (dataFlagBit == S_MAP && content_map != nullptr) {
			for (auto& itpair : *content_map) {
				auto key = itpair.first;
				auto value = itpair.second;
				key.clear();
				value.clear();
			}
			content_map->clear();
			content_map = nullptr;
			//safe_delete(content_map);
		}
		else if (dataFlagBit == S_MULTIMAP && content_multimap != nullptr) {
			for (auto& itpair : *content_multimap) {
				auto key = itpair.first;
				key.clear();
				for (auto& itvalue : itpair.second) {
					auto itvalue_ = itvalue;
					itvalue_.clear();
				}
			}
			content_multimap->clear();
			content_multimap = nullptr;
			//safe_delete(content_multimap);
		}
		else if (dataFlagBit == S_SET && content_set != nullptr) {
			for (auto& elem : *content_set) {
				auto elem_ = elem;
				elem_.clear();
			}
			content_set->clear();
			content_set = nullptr;
			//safe_delete(content_set);
		}
		else if (dataFlagBit == S_MULTISET && content_multiset != nullptr) {

			for (auto& elem : *content_multiset) {
				auto elem_ = elem;
#if debug
				{
					static int ignorecount = 0;
					std::cout << "Data::clear() S_MULTISET" << " elem:["
						<< elem.toStrg() << "]" << "\tignorecount:["
						<< ignorecount++ << "\t](" << __FILE__ << ":"
						<< __LINE__ << ":0" << ")" << std::endl;
				}
#endif
				elem_.clear();
			}

			content_multiset->clear();
			content_multimap = nullptr;

			/*		try {
						safe_delete(content_multiset);
					}
					catch (exception& e) {
		#if debug
						{
							static int ignorecount = 0;
							std::cout << "Data::clear() S_MULTISET" << " delete error:"
								<< e.what() << "\tignorecount:[" << ignorecount++
								<< "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")"
								<< std::endl;
						}
		#endif
					}*/

		}
		this->dataFlagBit = S_Dz;


	}



	/***********************************


	DataTable


	***********************************/
	/*
			Do not call manually， only called by make_shared_ptr
		*/
	void DataTable::setOnHeap() {
		this->on_heap = true;
		return;
	}
	bool DataTable::onHeap() const {
		return this->on_heap;
	}

	DataTablePtr DataTable::getThisPtr() const {
		if (on_heap == true) {
			return thisPtr;
		}
		else {
			return DataTablePtr(this);
		}
	}
	map<ArgFormal, ArgReal, ArgComparatorLT>& DataTable::getFunctionCallFRMap(TemplateFunctionTableHash* tfth, ScopeTableHash* scpth) {
		auto tmpar = thisPtr;
		auto& ssf = tmpar->scopeStructureFKA;
		auto fname = tfth->getTFFKA(ssf, scpth);
		if (fname == fkanull) {
			throw exception("AR not in function body");
		}
		auto fbody = tfth->operator[](fname).bodyScopeFKA;
		while (tmpar != nullptr) {

			if (tmpar->scopeStructureFKA == fbody) {
				return tmpar->frmap;
			}
			else {
				tmpar = tmpar->parentAR;
			}
		}
		throw exception("AR not in function body");
	}
	//use the two functions to ban/allow smartptr managing the memory:
	bool DataTable::lock() {
		if (this->on_heap == false) {
			cerr << "only the SmartPtr owned onheap instance can be locked! exit(-1)" << endl;

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "only SmartPtr owned onheap instance can be locked! exit(-1)"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);

		}
		if (locked == true) {
			cerr << "The memory of this object cannot be overlocked, exit(-1)" << endl;

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "The memory of this object cannot be overlocked, exit(-1)"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}
		else {
			this->locked = true;
			this->rollback_on_heap = this->on_heap;
			this->on_heap = false;
		}
		return this->on_heap;
	};
	bool DataTable::unlock() {
		if (locked == false) {
			cerr << "The memory of this object cannot be overunlocked, exit(-1)" << endl;

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "The memory of this object cannot be overunlocked, exit(-1)"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}
		else {
			this->locked = false;
			this->on_heap = this->rollback_on_heap;
			this->rollback_on_heap = false;
		}
		return this->on_heap;
	};
	DataTable::DataTable(DataTablePtrW parentAR, DataTablePtrW queryAR,
		const FKA& scopeStructureFKA) :frmap(ArgComparatorLT(acm::name | acm::asc)) {
		/*if (heapObjSet.count(this)) {
			this->onHeap = true;
		}
		else {
			this->onHeap = false;
		}*/
		this->parentAR = parentAR;
		if (queryAR != nullptr) {
			if (queryAR->type == SYSB) {
				this->queryARList_local.push_back(queryAR);
			}
			this->queryARList.push_back(queryAR);
		}
		this->scopeStructureFKA = scopeStructureFKA;
	}
	DataTable::DataTable(DataTablePtrW parentAR, DataTablePtrW queryAR,
		DataTablePtrW returnAR, const FKA& scopeStructureFKA, const Intg& type) :frmap(ArgComparatorLT(acm::name | acm::asc)) {

		this->parentAR = parentAR;
		if (queryAR != nullptr) {
			this->queryARList.push_back(queryAR);
			if (queryAR->type == SYSB) {
				this->queryARList_local.push_back(queryAR);
			}

		}
		this->returnAR = returnAR;
		this->scopeStructureFKA = scopeStructureFKA;
		this->type = type;
	}
	void DataTable::setFRMap(const map<ArgFormal, ArgReal>& frmap) {
		this->frmap.insert(frmap.begin(), frmap.end());
		return;
	}
	/**
	 * @name findcurrent
	 * @param arg
	 * @return whether arg exit in THIS(current) ar,include frmap ,not include parentAR and queryAR
	 */
	bool DataTable::findcurrent(const Arg& arg) const {
		return findArgInAR(*this, arg);
	}
	/**
	 * @name findall
	 * @param arg
	 * @return whether arg exit in THIS(current)/parentAR/queryAR ar(recursively), include frmap
	 */
	bool DataTable::findall(const Arg& arg) const {
#if debug
		{
			static int ignorecount = 0;
			cout << "DataTable::findall(const Arg&)" << " arg:" << "["
				<< arg.toStrg() << "]" << "\tignorecount:[" << ignorecount++
				<< "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif
		if (findcurrent(arg) == true) {
			return true;
		}
		//    if (arg.formalArg == false || arg.asc != this->scopeStructureAddr) {
#if debug
		{
			static int ignorecount = 0;
			cout << "DataTable::findall(const Arg&)" << " " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif
		Arg arg2 = arg;
		arg2.formalArg = true;
		deque<const deque<DataTablePtrW>*> queryARList_deque;
		queryARList_deque.push_back(&queryARList);
		set<DataTablePtr> checkedAR; //不重复检索AR
#if debug
		{
			static int ignorecount = 0;
			cout << "DataTable::findall(const Arg&)" << " " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif
		while (queryARList_deque.size() > 0) {
#if debug
			{
				static int ignorecount = 0;
				cout << "DataTable::findall(const Arg&)" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			const deque<DataTablePtrW>* queryARList_ = queryARList_deque.front();
			queryARList_deque.pop_front();
			auto it = queryARList_->begin();
			for (; it != queryARList_->end(); ++it) {
				auto ar = *it;
#if debug
				{
					static int ignorecount = 0;
					cout << "DataTable::findall(const Arg&)" << " "
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
				}
#endif
				if (checkedAR.count(ar)) {
					continue;
				}
#if debug
				{
					static int ignorecount = 0;
					cout << "DataTable::findall(const Arg&)" << " "
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
				}
#endif
				checkedAR.insert(ar);
				if (ar != nullptr) {
					if (ar->findcurrent(arg2) == true) {
#if debug
						{
							static int ignorecount = 0;
							cout << "DataTable::findall(const Arg&)" << " "
								<< "\tignorecount:[" << ignorecount++ << "\t]("
								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
								<< endl;
						}
#endif
						return true;
					}
					queryARList_deque.push_back(&(ar->queryARList));
				}
#if debug
				{
					static int ignorecount = 0;
					cout << "DataTable::findall(const Arg&)" << " "
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
				}
#endif
			}
#if debug
			{
				static int ignorecount = 0;
				cout << "DataTable::findall(const Arg&)" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
		}
#if debug
		{
			static int ignorecount = 0;
			cout << "DataTable::findall(const Arg&)" << " " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif

		return false;

		//    }
		//    return false;

	}
	/**
	 * @brief findall的改进版，获得arg所在scope的Addr，如果无法检索到变量，则抛出false异常。
	 * @param arg
	 * @return
	 * @throw 立即数将throw
	 */
	FKA DataTable::getArgScopeFKA(const Arg& arg) const {

		const auto& arg_ar = this->getArgAR(arg);
		if (arg_ar != nullptr) {
			return arg_ar->scopeStructureFKA;
		}
		else {
#if debug
			{
				static int ignorecount = 0;
				cout << "DataTable::getArgScopeFKA(const Arg&)"
					<< " can't find arg,throw arg" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			throw arg;
		}


		//Arg arg2 = arg;
		//arg2.formalArg = true;
		//deque<const deque<DataTablePtrW>*> queryARList_deque;
		//queryARList_deque.push_back(&queryARList);
		//set<DataTablePtr> checkedAR; //不重复检索AR
		//while (queryARList_deque.size() > 0) {

		//	const deque<DataTablePtrW>* queryARList_ = queryARList_deque.front();
		//	queryARList_deque.pop_front();
		//	auto it = queryARList_->begin();
		//	for (; it != queryARList_->end(); ++it) {
		//		auto ar = *it;
		//		if (checkedAR.count(ar)) {
		//			continue;
		//		}
		//		checkedAR.insert(ar);
		//		if (ar != nullptr) {
		//			if (ar->findcurrent(arg2) == true) {
		//				return ar->scopeStructureFKA;
		//			}
		//			queryARList_deque.push_back(&(ar->queryARList));
		//		}
		//	}
		//}


	}
	/**
	 * @attention 引用不考虑引用目标
	 * @param arg
	 * @return
	 */
	DataTablePtrW DataTable::getArgAR(const Arg& arg) const {

		if (findcurrent(arg) == true) {
			return DataTablePtr(this);
		}
		//    if (arg.formalArg == false || arg.asc != this->scopeStructureAddr) {
		Arg arg2 = arg;
		arg2.formalArg = true;
		deque<const deque<DataTablePtrW>*> queryARList_deque;
		queryARList_deque.push_back(&queryARList);
		set<DataTablePtr> checkedAR; //不重复检索AR
		while (queryARList_deque.size() > 0) {

			const deque<DataTablePtrW>* queryARList_ = queryARList_deque.front();
			queryARList_deque.pop_front();
			auto it = queryARList_->begin();
			for (auto ar = it; it != queryARList_->end(); ++it) {
				if (checkedAR.count(*ar)) {
					continue;
				}
				checkedAR.insert(*ar);
				if (*ar != nullptr) {
					if ((*ar)->findcurrent(arg2) == true) {
						return *ar;
					}
					queryARList_deque.push_back(&((*ar)->queryARList));
				}
			}
		}
#if debug
		{
			static int ignorecount = 0;
			cout << "DataTable::getArgAR(const Arg&)" << " can't find AR"
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif
		return nullptr;

		//    }
		//    return false;

	}
	FKA DataTable::setArgScopeFKA(Arg& arg) {
		if (arg.argFlag == M_Bs) {
			return fkanull;
		}
		if (arg.argFlag == X_Bs) {
			return fkanull;
		}
		if (arg.argFlag == Z_Dz) {
			arg.argFlag = S_AR;
		}
		DataTablePtr dt = getArgAR(arg);
		if (dt != nullptr) {
			arg.asc = dt->scopeStructureFKA;
			return arg.asc;
		}
		else {
#if debug
			cerr << "DataTable::setArgScope err , can't find arg!" << endl;
#endif
			return fkanull;
		}
	}
	/**
	 * @brief 获得当前ar可以访问到的ar，可以是自身、父ar、queryar或returnar
	 * @param parentScopeFKA
	 * @return
	 */
	 //DataTablePtrW DataTable::getAccessibleAR(const FKA& arScopeFKA) const {

	 //	//    set<DataTablePtr> checkedAR; //
	 //	//    set<DataTablePtr> checkingAR;
	 //	//    set<DataTablePtr> candidateAR;
	 //	map<DataTablePtrW, Intg> checkedAR;
	 //	map<DataTablePtrW, Intg> checkingAR;
	 //	map<DataTablePtrW, Intg> candidateAR;
	 //	checkingAR.insert(make_pair(this, 0));

	 //	while (true) {
	 //		for (auto ar_it : checkingAR) {
	 //			auto ar = ar_it.first;
	 //			if (checkedAR.count(ar)) {
	 //				continue;

	 //			}
	 //			else {
	 //				if (ar != nullptr && ar->scopeStructureFKA == arScopeFKA) {
	 //					return ar;
	 //				}
	 //				else if (ar != nullptr) {
	 //					candidateAR.insert(make_pair(ar->parentAR, 0));
	 //					candidateAR.insert(make_pair(ar->returnAR, 0));
	 //					for (auto queryAR : ar->queryARList) {
	 //						candidateAR.insert(make_pair(queryAR, 0));
	 //					}
	 //					checkedAR.insert(make_pair(ar, 0));
	 //				}
	 //			}

	 //		}
	 //		for (auto checkedAR_ : checkedAR) {

	 //			candidateAR.erase(checkedAR_.first);
	 //		}
	 //		if (candidateAR.size() > 0) {
	 //			checkingAR.clear();
	 //			candidateAR.swap(checkingAR);
	 //			checkingAR.erase(nullptr);
	 //		}
	 //		else {
	 //			return nullptr;
	 //		}

	 //	}
	 //}
	 //Addr DataTable::getArgScopeAddr(const Arg &arg) {
	 //#if debug
	 //    cout << "DataTable::getArgScopeAddr arg:[" << arg.toStrg() << "]" << endl;
	 //#endif
	 //    if (arg.argFlag == M_Bs || arg.argFlag == Sz || arg.argFlag == X_Bs) {
	 //        return Addr();
	 //
	 //    } else if (arg.argFlag == Z_Dz) {
	 //        Arg arg_(arg);
	 //        arg_.argFlag = S_AR;
	 //        return getArgScopeAddr(arg_);
	 //    }
	 //    DataTable *dt = getArgAR(arg);
	 //    if (dt) {
	 //        return dt->scopeStructureAddr;
	 //
	 //    } else {
	 //#if debug
	 //        cout << "DataTable::getArgScope err , can't find arg!\tArg:["
	 //                << arg.toStrg() << "]\t" << endl;
	 //#endif
	 //        return Addr();
	 //    }
	 //}
	 /**
	  * @brief 将一个以arg为key的元组添加到ar中，元组的值为默认值
	  * @param arg
	  * @attention for the args of M_BS , SZ, Zf , or formalArg==false/(non-local arg) this will simply return true
	  * if the arg is argnull, it is unable to become the key of a pair, and simply return true.
	  * @return if
	  */
	bool DataTable::add(const Arg& arg) {
		if (arg.operator ==(argnull)) {
			return false;
		}
		//need_modify
		if (arg.argFlag == X_Bs) {
			return true;
		}

		if (arg.argFlag == M_Bs || arg.argFlag == Sz || arg.argFlag == Zf) {
			return true;
		}
		if (arg.formalArg == false) {
			return true;

		}
		if (arg.argFlag == Z_Dz) {

			Arg arg_;
			arg_ = arg;
			arg_.argFlag = S_AR;
#if debug
			{
				static int ignorecount = 0;
				std::cout << "DataTable::add(const Arg&)"
					<< " arg.argFlag == Z_Dz arg:" << arg.toStrg() << ""
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << std::endl;
			}
#endif
			return this->add(arg_);
		}
		if (arg.argFlag == EXPRZ_Dz) {

			Arg arg_;
			arg_ = arg;
			arg_.argFlag = S_Dz;
			return this->add(arg_);
		}

#if debug
		cout << "DataTable::add ,Arg:[" << arg.toStrg() << "]\t" << endl;
#endif
		//@attention 函数声明中的引用默认不是形式参数
		if (arg.formalArg == true && arg.argFlag != Y_Dz
			&& findcurrent(arg) == true) {
			return true;
		}
		else if (arg.formalArg == true && findcurrent(arg) == false) {

			if (arg.isSymbol) {
				symdmap[arg] = Data(arg);

			}
			else {
				argdmap[arg] = Data(arg);
			}
			//if (arg.argFlag == S_Dz) {
			//	argdmap[arg] = Data(arg);
			//	//argdmap[arg].dataFlagBit = S_Dz;
			//}
			//else if (arg.argFlag == Y_Dz) {
			//	argdmap[arg] = Data(arg);
			//	/*argdmap[arg].dataFlagBit = Y_Dz;
			//	argdmap[arg].content_ar = arg.ref_ar;*/
			//}
			//else if (arg.argFlag == S_Bs) {
			//	argdmap[arg] = Data(arg);
			//	//sdmap[arg.arg_s].dataFlagBit = S_Bs;
			//}
			//else if (arg.argFlag == S_AR) {
			//	argdmap[arg] = Data(arg);
			//	//argdmap[arg.arg_fka].dataFlagBit = S_AR;
			//} if (arg.argFlag == EXPRZ_Dz) {
			//	argdmap[arg] = Data(arg);
			//	//argdmap[arg.arg_fka].isExpression = true;
			//}
			return true;
		}
#if debug
		cerr << "DataTable::add err,Arg:[" << arg.toStrg() << "]\t" << endl;
#endif
		return false;

	}
	/*Data& DataTable::operator[](const Addr& addr) {

	 if (addr == addrnull) {
	 #if debug
	 cerr << "DataTable::operator[] err: addrnull" << endl;
	 #endif
	 }
	 DataTablePtr ar = this;
	 Arg arg(S_Dz, addr);
	 if (findall(arg) == false) {
	 arg.argFlag = S_AR;
	 }
	 if (findall(arg) == false) {
	 return Data().DataNull();
	 #if debug
	 cerr << "DataTable::operator[Addr] err: can't find Data. Addr:["
	 << addr.toStrg() << "]" << endl;
	 #endif
	 }
	 while (ar != nullptr) {
	 if ((ar->findcurrent(arg)) != true) {
	 ar = ar->queryAR;
	 } else {
	 return ar->argdmap[addr];
	 }

	 }

	 #if debug
	 cerr << "DataTable::operator[Addr] err: can't find Data. Addr:["
	 << addr.toStrg() << "]" << endl;
	 #endif
	 return datanull;

	 }*/
	[[deprecated("Use operator[](const Arg&)")]]
	//Data& DataTable::operator[]( const FKA& fka) {

	//	Data& data = (*this)[Arg(S_AR, fka)];
	//	/*        if (data.dataFlagBit == Y_Dz) {
	//	 return globalRefTable.getDataRef(Arg(Y_Dz, data.content_fka, this));

	//	 } else
	//	 return data;*/
	//	return data;
	//}
	/*Data& DataTable::operator[](const Strg& name) {
	 DataTablePtr ar = this;
	 Arg arg(S_Bs, name);
	 if (findall(arg) == false) {
	 return Data().DataNull();
	 #if debug
	 cerr << "DataTable::operator[Strg:name] err: can't find Data. name:["
	 << name << "]" << endl;
	 #endif
	 }
	 while (ar != nullptr) {
	 if ((ar->findcurrent(arg)) != true) {
	 ar = ar->queryAR;
	 } else {
	 return ar->sdmap[name];
	 }

	 }

	 #if debug
	 cerr << "DataTable::operator[Strg:name] err: can't find Data. name:["
	 << name << "]" << endl;
	 #endif
	 return datanull;
	 }*/
	Data& DataTable::operator[](const Strg& name) {
		return (*this)[Arg(S_Bs, name)];
	}
	/**
	 * @attention 虽然返回的也是引用，但是和通常[key]运算符的差别是：
	 * 如果key不存在，会报错，因此在使用key修改值时需要先使用add 添加元组；
	 * 此外，当所查询到的Data的类型是S_PTR时，不会返回PTR所指的Data，
	 * 因为这个data有可能为空，需要用户进一步判断
	 * @param arg
	 * @return
	 */
	Data& DataTable::operator[](const Arg& arg) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "DataTable::operator [](const Arg&)" << " arg:["
				<< arg.toStrg() << "]" << "\tignorecount:[" << ignorecount++
				<< "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")"
				<< std::endl;
		}
#endif
		if (arg.argFlag == Y_Dz) {
			return globalRefTable.getDataRef(arg);
		}
		if (arg.argFlag == Z_Dz) {
			Arg arg_ = arg;
			arg_.argFlag = S_AR;
			return (*this)[arg_];
		}
		if (arg.argFlag == EXPRZ_Dz) {
			Arg arg_ = arg;
			arg_.argFlag = S_Dz;
			return (*this)[arg_];
		}
		if (arg.argFlag == Sz || arg.argFlag == Zf) {
#if debug
			cout << "DataTable::operator[] warm 0 , arg is Sz||Zf ,arg:["
				<< arg.toStrg() << "]" << endl;
#endif
			throw arg;
		}

		if (findall(arg) == false) {

#if debug && _WIN32
			{
				stringstream ss;
				ss << bbhline;
				ss << "DataTable(" << this->scopeStructureFKA.toStrg() << ")::operator[](" << arg.toStrg() << "), error exit:" << endl;
				ss << "DataTable::this:" << "\n";
				ss << this->toStrg();
				ss << bbhline;
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = ss.str(); // Replace with actual info
				info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
				std::cout << info << std::endl;
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif

				ignoreCount++;
			}
#endif
			cout << "DataTable::operator[] err, exit(-1), arg:[" << arg.toStrg()
				<< "]" << endl;

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			assert(false);
			exit(-1);

		}
#if debug
		{
			static int ignorecount = 0;
			cout << "DataTable::operator [](const Arg&)" << " " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif

		//1 search in currentAR(this)// argdmap / frmap->query ar(function invocation)
		if (findcurrent(arg) == true) {

			if (argdmap.find(arg) != argdmap.end()) {
				return argdmap[arg];
			}
			else if (symdmap.find(arg) != symdmap.end()) {
				return symdmap[arg];
			}
#if debug
			{
				static int ignorecount = 0;
				cout << "DataTable::operator [](const Arg&)" << " "
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
			if (frmap.find(arg) != frmap.end()) {

				Arg& argr = frmap[arg];
				if (argr.argFlag == Sz || argr.argFlag == Zf) {

#if debug
					cout << "DataTable::operator[] warm 1 , arg is Sz||Zf ,arg:["
						<< arg.toStrg() << "]" << endl;
#endif
#if debug
					{
						static int ignorecount = 0;
						cout << "\tignorecount:[" << ignorecount++ << "\t]("
							<< __FILE__ << ":" << __LINE__ << ":0" << ")"
							<< endl;
					}
#endif
					throw argr;
				}
				else {
					for (auto& query_ar : this->queryARList) {
						if (query_ar->findall(argr)) {
							return query_ar->operator[](argr);
						}

					}
					throw exception("Cannot found corresponding actual/real arg in frmap.");

					//#if debug
					//					{
					//						static int ignorecount = 0;
					//						cout << "DataTable::operator [](const Arg&)" << " "
					//							<< "\tignorecount:[" << ignorecount++ << "\t]("
					//							<< __FILE__ << ":" << __LINE__ << ":0" << ")"
					//							<< endl;
					//					}
					//#endif
					//
					//					DataTablePtrW argr_locate_ar = DataTable::getAccessibleAR(this->getThisPtr(), argr.asc);
					//
					//					if (argr_locate_ar != nullptr) {
					//						//查询继承的类的成员、上层作用域的成员，
					//						//对于这类成员，变量的asc就是它所在ar的asc
					//						return (*argr_locate_ar)[argr];
					//					}
					//					else if (this->returnAR != nullptr) {
					//						//如果一个变量存在、可访问、有asc，但无法检索到其asc对应的ar，
					//						//那么这个变量可能是函数调用中的形参，其所在作用域为函数声明作用域，故无ar
					//						//对于这类变量，进入returnAR查询
					//						if (this->returnAR->findall(argr)) {
					//							return this->returnAR->operator [](argr);
					//						}
					//					}
					//					else {
					//						throw argr;
					//					}

				}

			}

		}

		//2 search in queryAR (not function invocation)
		Arg arg2 = arg;
		if (arg2.argFlag == Z_Dz) {
			arg2.argFlag = S_AR;
		}
		arg2.formalArg = true;
		deque<const deque<DataTablePtrW>*> queryARList_deque;
		queryARList_deque.push_back(&queryARList);
		//这里应该用set<DataTablePtr>的，但是使用set后无法进入此函数进行调试，所以用map先代替了
		std::map<DataTablePtr, Intg> checkedAR; //不重复检索AR
		while (queryARList_deque.size() > 0) {

			const deque<DataTablePtrW>* queryARList_ = queryARList_deque.front();
			queryARList_deque.pop_front();
			for (auto ar : *queryARList_) {
				if (checkedAR.count(ar)) {
					continue;
				}
				checkedAR.insert(make_pair(ar, 0));
				if (ar != nullptr) {
					if (ar->findall(arg2) == true) {

						try {
							return ar->operator [](arg2);
						}
						catch (Arg& argr) {
							throw argr;
						}

					}
					else {
						queryARList_deque.push_back(&(ar->queryARList));
					}

				}
			}
		}

		//3 search in parentAR
		if (this->parentAR != nullptr && this->parentAR->findall(arg)) {
			try {
				return this->parentAR->operator [](arg);
			}
			catch (Arg& argr) {
				throw argr;
			}
		}

		std::cerr << "DataTable::operator[] cannot find data by index arg, exit(-1)"
			<< endl;

#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = "error exit"; // Replace with actual info
			info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif
			cout << info << endl;
			ignoreCount++;
		}
#endif
		assert(false);
		exit(-1);
	}




	/// <summary>
	/// return the arg as key in the dataTable. for reference and formal params, it will return the actual args directly pair with the data.
	/// </summary>
	/// <param name="argindex"></param>
	/// <returns></returns>
	const Arg DataTable::getKeyArg(const Arg& argindex) {

#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = ""; // Replace with actual info
			info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif
			cout << info << endl;
			ignoreCount++;
		}
#endif


		Arg arg = argindex;

		if (argindex.argFlag == Y_Dz) {
			arg = globalRefTable.operator[](argindex);
		}
		if (arg.argFlag == Z_Dz) {
			Arg arg_ = arg;
			arg_.argFlag = S_Dz;
			return getKeyArg(arg_);
		}
		if (arg.argFlag == Sz || arg.argFlag == Zf) {
			return arg;
		}

		if (findall(arg) == false) {

			return arg;

		}
#if debug
		{
			static int ignorecount = 0;
			cout << "DataTable::operator [](const Arg&)" << " " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif

		//1 search in currentAR(this)
		if (findcurrent(arg) == true) {

			if (argdmap.find(arg) != argdmap.end()) {
				auto it = argdmap.find(arg);
				return it->first;
			}
			else if (symdmap.find(arg) != symdmap.end()) {
				auto it = symdmap.find(arg);
				return it->first;
			}

#if debug
			{
				static int ignorecount = 0;
				cout << "DataTable::operator [](const Arg&)" << " "
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
			if (frmap.find(arg) != frmap.end()) {

				Arg& argr = frmap[arg];
				if (argr.argFlag == Sz || argr.argFlag == Zf) {

#if debug
					cout << "DataTable::operator[] warm 1 , arg is Sz||Zf ,arg:["
						<< arg.toStrg() << "]" << endl;
#endif
#if debug
					{
						static int ignorecount = 0;
						cout << "\tignorecount:[" << ignorecount++ << "\t]("
							<< __FILE__ << ":" << __LINE__ << ":0" << ")"
							<< endl;
					}
#endif
					return argr;
				}
				else {

					for (auto& query_ar : this->queryARList) {
						if (query_ar->findall(argr)) {
							return query_ar->getKeyArg(argr);
						}

					}
					throw exception("Cannot found corresponding actual/real arg in frmap.");

					//#if debug
					//					{
					//						static int ignorecount = 0;
					//						cout << "DataTable::operator [](const Arg&)" << " "
					//							<< "\tignorecount:[" << ignorecount++ << "\t]("
					//							<< __FILE__ << ":" << __LINE__ << ":0" << ")"
					//							<< endl;
					//					}
					//#endif
					//					DataTablePtrW argr_locate_ar = DataTable::getAccessibleAR(this->getThisPtr(), argr.asc);
					//					if (argr_locate_ar != nullptr) {
					//						//查询继承的类的成员、上层作用域的成员，
					//						//对于这类成员，变量的asc就是它所在ar的asc
					//						return (*argr_locate_ar).getKeyArg(argr);
					//					}
					//					else if (this->returnAR != nullptr) {
					//						//如果一个变量存在、可访问、有asc，但无法检索到其asc对应的ar，
					//						//那么这个变量可能是函数调用中的形参，其所在作用域为函数声明作用域，故无ar
					//						//对于这类变量，进入returnAR查询
					//						if (this->returnAR->findall(argr)) {
					//							return this->returnAR->getKeyArg(argr);
					//						}
					//					}
					//					else {
					//						return argr;
					//					}

				}

			}

		}

		//2 search in queryAR
		Arg arg2 = arg;
		if (arg2.argFlag == Z_Dz) {
			arg2.argFlag = S_AR;
		}
		arg2.formalArg = true;
		deque<const deque<DataTablePtrW>*> queryARList_deque;
		queryARList_deque.push_back(&queryARList);
		//这里应该用set<DataTablePtr>的，但是使用set后无法进入此函数进行调试，所以用map先代替了
		std::map<DataTablePtr, Intg> checkedAR; //不重复检索AR
		while (queryARList_deque.size() > 0) {

			const deque<DataTablePtrW>* queryARList_ = queryARList_deque.front();
			queryARList_deque.pop_front();
			auto it = queryARList_->begin();
			if (it == queryARList_->end()) {
				continue;
			}
			for (auto ar = *it; it != queryARList_->end(); ++it) {
				if (checkedAR.count(ar)) {
					continue;
				}
				checkedAR.insert(make_pair(ar, 0));
				if (ar != nullptr) {
					if (ar->findall(arg2) == true) {



						return ar->getKeyArg(arg2);


					}
					else {
						queryARList_deque.push_back(&(ar->queryARList));
					}

				}
			}
		}

		//3 search in parentAR
		if (this->parentAR != nullptr && this->parentAR->findall(arg)) {

			return this->parentAR->getKeyArg(arg);

		}


#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = "DataTable::getKeyArg cannot find arg by index arg"; // Replace with actual info
			info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif
			cout << info << endl;
			ignoreCount++;
		}
#endif

		cout << "DataTable::getKeyArg cannot find arg by index arg"
			<< endl;

		return arg;
	}
	/// <summary>
	/// set the newkey (arg) to replace the original key in the dataTable. for reference and formal params, it will replace the actual key directly pair with the data. it worth mentioning that, all the flagbits except the name(type and value) and asc(flag indicating which scope the arg is in) will be replaced! a bound placeholder is an expression, not an pointer or reference. therefore won't be pass.
	/// </summary>
	/// <param name="newkey"></param>
	/// <returns></returns>
	void DataTable::setKeyArg(const Arg& newkey) {

#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = "error exit"; // Replace with actual info
			info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif
			cout << info << endl;
			ignoreCount++;
		}
#endif
		///copy the flagbits of the newkey to the arg.
		auto setFlagBits = [](Arg& arg, const Arg& newkey)->void {
			Arg  argtmp = arg;
			arg = newkey;
			arg.argFlag = argtmp.argFlag;
			arg.arg_i = argtmp.arg_i;
			arg.arg_s = argtmp.arg_s;
			arg.arg_fka = argtmp.arg_fka;
			arg.asc = argtmp.asc;
			};

		Arg arg = newkey;

		if (newkey.argFlag == Y_Dz) {
			arg = globalRefTable.operator[](newkey);
		}
		if (arg.argFlag == Z_Dz) {
			Arg arg_ = arg;
			setFlagBits(arg_, newkey);
			arg_.argFlag = S_AR;
			return setKeyArg(arg_);
		}
		if (arg.argFlag == Sz || arg.argFlag == Zf) {
			return;
		}

		if (findall(arg) == false) {

			return;

		}
#if debug
		{
			static int ignorecount = 0;
			cout << "DataTable::operator [](const Arg&)" << " " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif

		//1 search in currentAR(this)
		if (findcurrent(arg) == true) {

			if (argdmap.find(arg) != argdmap.end()) {
				auto nodeHandler = argdmap.extract(arg);
				auto& key = nodeHandler.key();
				setFlagBits(key, newkey);
				argdmap.insert(move(nodeHandler));
				return;
			}
			if (symdmap.find(arg) != symdmap.end()) {
				auto nodeHandler = symdmap.extract(arg);
				auto& key = nodeHandler.key();
				setFlagBits(key, newkey);
				symdmap.insert(move(nodeHandler));
				return;
			}



			if (frmap.find(arg) != frmap.end()) {

				Arg& argr = frmap[arg];
				setFlagBits(argr, newkey);
				if (argr.argFlag == Sz || argr.argFlag == Zf) {


					return;
				}
				else {


					for (auto& query_ar : this->queryARList) {
						if (query_ar->findall(argr)) {
							return query_ar->setKeyArg(argr);
						}

					}
					throw exception("Cannot found corresponding actual/real arg in frmap.");
					//DataTablePtrW argr_locate_ar = DataTable::getAccessibleAR(this->getThisPtr(), argr.asc);
					//if (argr_locate_ar != nullptr) {
					//	//查询继承的类的成员、上层作用域的成员，
					//	//对于这类成员，变量的asc就是它所在ar的asc
					//	return (*argr_locate_ar).setKeyArg(argr);
					//}
					//else if (this->returnAR != nullptr) {
					//	//如果一个变量存在、可访问、有asc，但无法检索到其asc对应的ar，
					//	//那么这个变量可能是函数调用中的形参，其所在作用域为函数声明作用域，故无ar
					//	//对于这类变量，进入returnAR查询
					//	if (this->returnAR->findall(argr)) {
					//		return this->returnAR->setKeyArg(argr);
					//	}
					//}
					//else {
					//	return;
					//}

				}

			}

		}

		//2 search in queryAR
		Arg arg2 = arg;
		if (arg2.argFlag == Z_Dz) {
			arg2.argFlag = S_AR;
		}
		arg2.formalArg = true;
		deque<const deque<DataTablePtrW>*> queryARList_deque;
		queryARList_deque.push_back(&queryARList);
		//这里应该用set<DataTablePtr>的，但是使用set后无法进入此函数进行调试，所以用map先代替了
		std::map<DataTablePtr, Intg> checkedAR; //不重复检索AR
		while (queryARList_deque.size() > 0) {

			const deque<DataTablePtrW>* queryARList_ = queryARList_deque.front();
			queryARList_deque.pop_front();
			auto it = queryARList_->begin();
			if (it == queryARList_->end()) {
				continue;
			}
			for (auto ar = *it; it != queryARList_->end(); ++it) {
				if (checkedAR.count(ar)) {
					continue;
				}
				checkedAR.insert(make_pair(ar, 0));
				if (ar != nullptr) {
					if (ar->findall(arg2) == true) {



						return ar->setKeyArg(arg2);


					}
					else {
						queryARList_deque.push_back(&(ar->queryARList));
					}

				}
			}
		}

		//3 search in parentAR
		if (this->parentAR != nullptr && this->parentAR->findall(arg)) {

			return this->parentAR->setKeyArg(arg);

		}


#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = "DataTable::setKeyArg cannot find arg by index arg"; // Replace with actual info
			info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif
			cout << info << endl;
			ignoreCount++;
		}
#endif

		cout << "DataTable::getKeyArg cannot find arg by index arg"
			<< endl;

		return;
	}

	set<Arg, ArgComparatorLT> DataTable::getPlaceholders(const CodeTable& cdt) {
		set<Arg, ArgComparatorLT> placeholders;
		for (const auto& cd : cdt.codedq) {
			for (int i : {0, 1, 3}) {
				Arg arg = this->getKeyArg(cd[i]);
				if (arg.isPlaceholder) {
					placeholders.insert(arg);
				}
			}
		}
		return placeholders;
	}
	//default to be deepAssign
	DataTable& DataTable::operator=(const DataTable& dt) {
		DataTablePtr d1(this);
		DataTablePtr d2(&dt);
		this->deepAssign(d1, d2, true);
		/*argdmap = dt.argdmap;
		scopeStructureFKA = dt.scopeStructureFKA;
		meetFKA = dt.meetFKA;
		parentAR = dt.parentAR;
		returnAR = dt.returnAR;
		queryARList = dt.queryARList;*/

		return (*this);
	}
	/**
	 * @brief compare two dataTable, and will compare the memory inner pointer pointing at.
	 * @attention if two dataTable 's type is system, compare their inherit systems
	 * @param dt
	 * @return
	 */
	bool DataTable::operator==(const DataTable& dt) const {
		if (this->type != dt.type) {
			return false;
		}
		if (this->type == SYSB) {
			Intg len = this->queryARList.size();
			if (len != (int)dt.queryARList.size()) {
				return false;
			}
			while (--len >= 0) {
				if (this->queryARList[len]->type == SYSB) {

					if (!(this->queryARList[len]->operator ==(*dt.queryARList[len]))) {
						return false;
					}
				}

			}
		}

		const map<ArgKey, Data>& dtargdmap = dt.argdmap;
		const map<ArgFormal, ArgReal, ArgComparatorLT>& dtfrmap = dt.frmap;
		const FKA& dtscopeStructureFKA = dt.scopeStructureFKA;
		const FKA& dtmeetFKA = dt.meetFKA;
		return argdmap ==

			dtargdmap && scopeStructureFKA ==

			dtscopeStructureFKA && meetFKA ==

			dtmeetFKA && frmap == dtfrmap;
	}
	///**
	// * @brief clone data from one ar to this datatable.
	// * @attention Only clone the memory of current ar, not other ars that refered in the ar will only copy their pointers.
	// * @param dt
	// * @return
	// */
	//DataTable& DataTable::clone(DataTablePtr ar) {

	//	this->backwardExecute = ar->backwardExecute;
	//	//此deque用于指定反向函数运行结束后应当跳转到的meetaddr。
	//	this->bfMeetFKAdq = ar->bfMeetFKAdq;

	//	this->type = ar->type;
	//	this->argdmap = ar->argdmap;
	//	this->sdmap = ar->sdmap;
	//	this->frmap = ar->frmap;

	//	this->scopeStructureFKA = ar->scopeStructureFKA;
	//	this->parentAR = ar->parentAR;
	//	this->returnAR = ar->returnAR;
	//	this->meetFKA = ar->meetFKA;
	//	this->queryARList = ar->queryARList;
	//	if (queryAR) {
	//		this->queryARList.push_back(queryAR);
	//	}

	//}
	/**
	 * @brief compare two dataTable, and will compare the memory inner pointer pointing tp.
	 * @attention if two dataTable 's type is system(class,type == SYSB), compare their inherit systems.
	 * this function only provide a way to order two different DataTable objects and doesn't
	 * have any other practical use.
	 * @brief compare order:
	 *  type
	 *  scopeStructFKA
	 *  argdmap
	 *  sdmap
	 *  frmap
	 *  queryARList（SYSB）、meetFKA（NON SYSB）
	 *
	 * @param dt
	 * @return
	 */
	bool DataTable::operator<(const DataTable& dt) const {
		if (this->type < dt.type) {
			return true;
		}

		if (this->scopeStructureFKA < dt.scopeStructureFKA) {
			return true;
		}
		else if (this->scopeStructureFKA == dt.scopeStructureFKA) {
			//compare argdmap
			Intg lenfkadmap = min(this->argdmap.size(), dt.argdmap.size());
			auto itfkadmap1 = this->argdmap.begin();
			auto itfkadmap2 = dt.argdmap.begin();
			while (--lenfkadmap >= 0) {
				if (itfkadmap1->first < itfkadmap2->first) {
					return true;

				}
				else if (itfkadmap1->first == itfkadmap2->first) {
					if (itfkadmap1->second < itfkadmap2->second) {
						return true;
					}
					else if (itfkadmap1->second == itfkadmap2->second) {
						++itfkadmap1;
						++itfkadmap2;
					}
					else {
						return false;
					}

				}
				else {
					return false;
				}

			}
			if (argdmap.size() < dt.argdmap.size()) {
				return true;
			}
			else if (argdmap.size() > dt.argdmap.size()) {
				return false;
			}

			//compare frmap
			Intg lenfrmap = min(this->frmap.size(), dt.frmap.size());
			auto itfrmap1 = this->frmap.begin();
			auto itfrmap2 = dt.frmap.begin();
			while (--lenfrmap >= 0) {
				if (itfrmap1->first < itfrmap2->first) {
					return true;

				}
				else if (itfrmap1->first == itfrmap2->first) {
					if (itfrmap1->second < itfrmap2->second) {
						return true;
					}
					else if (itfrmap1->second == itfrmap2->second) {
						++itfrmap1;
						++itfrmap2;
					}
					else {
						return false;
					}

				}
				else {
					return false;
				}

			}
			if (frmap.size() < dt.frmap.size()) {
				return true;
			}
			else if (frmap.size() > dt.frmap.size()) {
				return false;
			}

			//SYSB compare queryAR
			if (this->type == SYSB) {

				Intg lenqueryAr = min(this->queryARList.size(),
					dt.queryARList.size());
				auto itqueryAr1 = this->queryARList.begin();
				auto itqueryAr2 = dt.queryARList.begin();
				while (--lenqueryAr >= 0) {
					if (*(*itqueryAr1) < *(*itqueryAr2)) {
						return true;

					}
					else if (*(*itqueryAr1) == *(*itqueryAr2)) {
						++itqueryAr1;
						++itqueryAr2;

					}
					else {
						return false;
					}

				}
				if (queryARList.size() < dt.queryARList.size()) {
					return true;
				}
				else if (queryARList.size() >= dt.queryARList.size()) {
					return false;
				}
			}
			else {
				//NON-SYSB compare meetFKA
				if (this->meetFKA < dt.meetFKA) {
					return true;
				}
				else {
					return false;
				}

			}
			return false;
		}
		else {
			return false;
		}

	}
	/** \brief duplicate from d2 to d1
	 * @attention duplicate the memory d1 points at，d1's reference num set to 1
	 * d2's reference num doesn's change, this function be called recursively
	 * @param d1
	 * @param d2
	 * @return
	 */
	void DataTable::deepAssign(DataTablePtr& d1, DataTablePtrW d2, bool copyClassInstances) {

		//if (d1 && d1->onHeap) {
		//	if (d1->type == SYSB) {
		//		DELETE_SYSB(d1);

		//	}
		//	else {

		//		DELETE_AR(d1);
		//	}
		//}
		d1 = DataTablePtr::make();
		if (d2 != nullptr) {


			d1->scopeStructureFKA = d2->scopeStructureFKA;
			d1->meetFKA = d2->meetFKA;
			d1->parentAR = d2->parentAR;

			d1->returnAR = d2->returnAR;
			d1->frmap = d2->frmap;
			d1->backwardExecute = d2->backwardExecute;
			d1->bfMeetFKAdq = d2->bfMeetFKAdq;


			for (auto& fkad : d2->argdmap) {
				if (fkad.second.dataFlagBit == S_AR && fkad.second.content_ar != nullptr) {
					DataTablePtr dt = nullptr;
					fkad.second.content_ar->deepAssign(dt, fkad.second.content_ar, copyClassInstances);
					d1->argdmap.insert(make_pair(fkad.first, Data(S_AR, &dt)));

				}
				else {
					d1->argdmap.insert(fkad);

				}

			}
			for (auto& qry : d2->queryARList) {
				DataTablePtrW p;


				if (qry != nullptr) {
					if (qry->type == SYSB && qry->type == SYSB && copyClassInstances) {
						DataTablePtr dt = DataTablePtr::make();
						qry->deepAssign(dt, qry, copyClassInstances);
						d1->queryARList_local.push_back(dt);
						d1->queryARList.push_back(dt);

					}
					else {

						d1->queryARList.push_back(qry);


					}
				}
			}
		}
		else {
			d1 = nullptr;
		}


	}
	Strg DataTable::toStrg() const {
		return this->_toStrg(true, nullptr);
	}
	Strg DataTable::_toStrg(bool withheader /*= true*/, set<const DataTable*>* handled/* = nullptr*/) const {
		bool deleteset = false;
		if (handled == nullptr) {
			handled = new set<const DataTable*>();
			handled->insert(this);
			deleteset = true;
		}
		else {
			if (handled->count(this) > 0) {
				return "\n" + calign("\DataTable ScopeStructureFKA(" + this->scopeStructureFKA.toStrg() + ") has been outputted", 96) + "\n" + hline;

			}
			else {
				handled->insert(this);
			}
		}
		stringstream ss;
		if (withheader) {
			ss << hline;
			ss << calign("DataTable", 96);
		}
		ss << hline;
		ss << "ScopeStructureFKA(" + this->scopeStructureFKA.toStrg() + ")\t" + (this->backwardExecute ? "BackwardExecute\t" : "") + (this->bfMeetFKAdq.size() > 0 ? ("backwardFunctionMeetFKA:[" + COOLANG::toStrg<deque<FKA> >(this->bfMeetFKAdq) + "]\t") : "") + (this->meetFKA != fkanull ? ("meetFKA(" + this->meetFKA.toStrg() + ")") : "") << endl;
		ss << "queryScopeFKA:\t";
		std::ranges::for_each(this->queryARList, [&](const DataTablePtrW& elem) {ss << (elem != nullptr ? elem->scopeStructureFKA.toStrg() + "\t" : ""); });
		ss << endl;
		ss << "parentScopeFKA:\t" << (this->parentAR != nullptr ? this->parentAR->scopeStructureFKA.toStrg() : "") << "\n";
		ss << "returnScopeFKA:\t" << (this->returnAR != nullptr ? this->returnAR->scopeStructureFKA.toStrg() : "") << "\n";
		if (argdmap.size() > 0) {
			ss << hline;
			ss << calign("DataTable(" + this->scopeStructureFKA.toStrg() + ")::argdmap", 96) << "\n";
			ss << calign("arg", 48) << calign("data", 48) << "\n";
			for (auto& p1 : argdmap) {
				ss << calign(p1.first.toStrg(), 48) << calign(p1.second.toStrg(), 48) << "\n";
				if (p1.second.dataFlagBit == S_AR && p1.second.content_ar != nullptr) {
					ss << p1.second.content_ar->_toStrg(false, handled);
				}
			}

		}
		if (deleteset && this->symdmap.size() > 0) {
			ss << hline;
			ss << calign("DataTablePtr(" + this->scopeStructureFKA.toStrg() + ")::symdmap(symbol --- actual/real)", 96) << "\n";
			ss << calign("symbol arg", 48) << calign("data", 48);
			for (auto& p2 : this->symdmap) {
				ss << calign(p2.first.toStrg(), 48) << calign(p2.second.toStrg(), 48) << "\n";
				if (p2.second.dataFlagBit == S_AR && p2.second.content_ar != nullptr) {
					ss << p2.second.content_ar->_toStrg(false, handled);
				}

			}
		}
		if (this->frmap.size() > 0) {
			ss << hline;
			ss << calign("DataTablePtr(" + this->scopeStructureFKA.toStrg() + ")::frmap(formal/local --- actual/real)", 96) << "\n";
			ss << calign("formal/local arg", 48) << calign("actual/real arg", 48) << "\n";
			for (auto& p3 : this->frmap) {
				ss << calign(p3.first.toStrg(), 48) << calign(p3.second.toStrg(), 48) << "\n";

			}
		}




		if (this->queryARList_local.size() > 0) {
			ss << hline;

			Strg queryARFKAStr;
			int i = 0;
			for (auto& ar_ : queryARList_local) {
				if (ar_ == nullptr) {
					continue;
				}
				if (i++ != 0) {
					queryARFKAStr += ",";
				}
				queryARFKAStr += ar_->scopeStructureFKA.toStrg();
			}
			ss << calign("DatTable(" + this->scopeStructureFKA.toStrg() + ")::queryARList_local/inheritedClassInstances{" + queryARFKAStr + "}", 96);

			for (auto& e : this->queryARList_local) {
				if (e == nullptr) {
					continue;
				}
				ss << hline;
				ss << calign("DatTable(" + this->scopeStructureFKA.toStrg() + ")::queryAR/inheritedClassInstance(" + e->scopeStructureFKA.toStrg() + ")", 96);
				ss << e->_toStrg(false, handled);
			}

		}

		if (this->queryARList.size() > 0) {
			ss << hline;
			Strg queryARFKAStr;
			int i = 0;
			for (auto& ar_ : queryARList_local) {
				if (ar_ == nullptr) {
					continue;
				}
				if (i++ != 0) {
					queryARFKAStr += ",";
				}
				queryARFKAStr += ar_->scopeStructureFKA.toStrg();
			}
			ss << calign("DatTable(" + this->scopeStructureFKA.toStrg() + ")::queryARList{" + queryARFKAStr + "}", 96);

			for (auto& e : this->queryARList) {
				if (e == nullptr) {
					continue;
				}
				ss << hline;
				ss << calign("DatTable(" + this->scopeStructureFKA.toStrg() + ")::queryAR(" + e->scopeStructureFKA.toStrg() + ")", 96);
				try {
					ss << e->_toStrg(false, handled);
				}
				catch (exception& e) {
					ss << e.what();
				}
			}

		}

		if (this->returnAR != nullptr) {
			ss << hline;
			ss << calign("DatTable(" + this->scopeStructureFKA.toStrg() + ")::returnAR(" + this->returnAR->scopeStructureFKA.toStrg() + ")", 96);
			ss << this->returnAR->_toStrg(false, handled);
		}
		if (this->parentAR != nullptr) {
			ss << hline;
			ss << calign("DatTable(" + this->scopeStructureFKA.toStrg() + ")::parentAR(" + this->parentAR->scopeStructureFKA.toStrg() + ")", 96);
			ss << this->parentAR->_toStrg(false, handled);
		}

		ss << hline;

		if (deleteset) {
			safe_delete(handled);
		}
		return ss.str();
	}

	/**
	 * @name findArgInAR
	 * @param arg
	 * @attention 不查询引用！！！！
	 * @return 查询范围只包括当前AR（包括frmap），考虑formalarg 标志位（formalarg为假直接返回否）
	 */
	bool findArgInAR(const DataTable& ar, const Arg& arg) {
#if debug
		static Intg ignorecount = 0;
		cout << "findArgInAR arg1:[" << arg.toStrg() << "]\targ:[" << arg.toStrg()
			<< "]\tignorecount:[" << ignorecount++ << "]" << endl;
#endif

		if (arg.isVar() == false) {

			return true;
		}
		else if (arg.argFlag == Z_Dz) {
			Arg arg_;
			arg_ = arg;
			arg_.argFlag = S_AR;
			return findArgInAR(ar, arg_);
		}
		if (arg.formalArg == F_) {
			return false;
		}
		if (arg.isSymbol) {
			auto iter = ar.symdmap.find(arg);
			if (iter != ar.symdmap.end()) {
				return true;
			}
		}
		if (arg.argFlag == S_Dz || arg.argFlag == EXPRZ_Dz || arg.argFlag == S_Bs || arg.argFlag == S_AR) {
#if debug
			cout << "findArgInAR arg2:[" << arg.toStrg() << "]" << endl;
#endif
			auto iter = ar.argdmap.find(arg);
			if (iter != ar.argdmap.end()) {
				return true;
			}

		}

		else {


#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "invalid arg type, exit(-1). arg: " + arg.toStrg(); // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			std::cerr << "invalid arg type, exit(-1). arg: " + arg.toStrg() << endl;

		}

#if debug
		{
			static int ignorecount = 0;
			cout << "findArgInAR(const DataTablePtr, const Arg&)" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif
		auto iter1 = ar.frmap.find(arg);
		if (iter1 != ar.frmap.end()) {
			for (auto query_ar : ar.queryARList) {
				if (query_ar->findall(iter1->second)) {
					return true;
				}

			}
			return false;
		}
		return false;

	}



	/**********************************

	ReferenceData

	************************************/

	/**
	 * @brief 返回reference指向的实例Data的引用，例如a.b.c = 10，则返回10对应data
	 * @return
	 */
	Data& ReferenceData::getData() {
		if (refData != nullptr) {
			return *refData;

		}
		else if (referencePath.size() > 0 && rootAR != nullptr) {
			Data* data = nullptr;
			DataTablePtrW rootAR = this->rootAR;
			for (auto& arg : referencePath) {
				data = &rootAR->operator [](arg);
				if (data->dataFlagBit == S_AR) {
					rootAR = data->content_ar;
				}
			}
			return *data;
		}

#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = " invalid reference index, exit(-1)"; // Replace with actual info
			info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif
			cout << info << endl;
			ignoreCount++;
		}
#endif

		cerr << " invalid reference index, exit(-1)" << endl;
		exit(-1);
	}
	/**
	 *
	 * @return 返回所指Arg所被创建的AR
	 */
	DataTablePtrW const ReferenceData::getDestAR() {

		if (referencePath.size() == 0) {
			return rootAR;

		}
		else if (referencePath.size() > 0 && rootAR != nullptr) {
			Data* data = nullptr;
			DataTablePtrW rootAR = this->rootAR;
			Intg pos = 0;
			Intg len = referencePath.size();

			while (pos < len - 1) {
				Arg& arg = referencePath[pos];
				try {
					data = &rootAR->operator [](arg);
					if (data->dataFlagBit == S_AR) {
						rootAR = data->content_ar;
					}
					else {
						break;
					}

				}
				catch (Arg& immed_arg) {

#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = " err ,exit(-1) "; // Replace with actual info
						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
						std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
						OutputDebugString(winfo.c_str());
#endif
						cout << info << endl;
						ignoreCount++;
					}
#endif
					cerr << " err ,exit(-1) " << endl;
					exit(-1);
				}
				++pos;
			}
			return rootAR;
		}

#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = " can't find ref, exit(-1)"; // Replace with actual info
			info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif
			cout << info << endl;
			ignoreCount++;
		}
#endif
		cerr << " can't find ref, exit(-1) " << endl;
		exit(-1);

	}
	/**
	 * @brief 获得reference的末端Arg的复制，例如a.b.c = 10，则返回c，通过c的asc来确定(此步骤重置c的asc),
	 * 当为MODE_SCAN时,需要提供AttributeTable*
	 * @return
	 */
	Arg ReferenceData::getArg() const {
#if debug
		{
			static int ignorecount = 0;
			cout << "getArg()" << " " << "\tignorecount:[" << ignorecount++
				<< "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif
		Arg refArg;

		Data* data = nullptr;
		DataTablePtrW rootAR = this->rootAR;
		Intg pathLen = referencePath.size();
		Intg pos = 0;
		while (pos < pathLen - 1) {
			try {
				data = &rootAR->operator [](referencePath[pos]);
				if (data->dataFlagBit == S_AR) {
					rootAR = data->content_ar;
				}
			}
			catch (Arg& is_immed_arg) {
				break;

			}
			pos++;
		}
		refArg = referencePath.back();
		refArg.asc = rootAR->getArgScopeFKA(referencePath.back());
		return refArg;


#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = " can't find arg, exit(-1)"; // Replace with actual info
			info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif
			cout << info << endl;
			ignoreCount++;
		}
#endif
		cerr << " can't find arg, exit(-1)" << endl;
		exit(-1);
	}
	void ReferenceData::clear() {
		refData = nullptr;

		referencePath.clear();
	}
	/**
	 * @attention 当两者之一不存在实例时（refData == nullptr &&rootAR == nullptr），只比较两个引用的指向arg（基本不会出现）
	 * @attention 当两者均存在实例时refData != nullptr ||rootAR != nullptr，只有两个引用的指向Arg和指向arg所在AR
	 * @attention 不要比较两个引用所指的data，因为预执行时其所指的data可能为空
	 * @param refdata
	 * @return
	 */
	bool ReferenceData::operator==(const ReferenceData& refdata) const {
		if ((refData == nullptr && rootAR == nullptr)
			|| (refdata.refData == nullptr && refdata.rootAR == nullptr)) {
			return getArg() == const_cast<ReferenceData*>(&refdata)->getArg();
		}
		else {
			return getArg() == const_cast<ReferenceData*>(&refdata)->getArg()
				&& const_cast<ReferenceData*>(this)->getDestAR()
				== const_cast<ReferenceData*>(&refdata)->getDestAR();
		}
	}
	Strg ReferenceData::toStrg() {
		stringstream ss;
		ss.precision(PRECISION);
		for (auto& arg : referencePath) {
			ss << arg.toStrg() << "\t";
		}
		return ss.str();
	}

	Scanner::Scanner() :scannerAR(DataTablePtr::make(nullptr, nullptr, FKA(FILEKEYPRIMARY, Addr()))) {
		currentAR = this->scannerAR;
	}
	/**********************************

	ReferenceTable

	************************************/
	Scanner::Scanner(CodeTableHash* codeTableHash_, ScopeTableHash* scopeTableHash_,
		TemplateFunctionTableHash* templateFunctionTableHash_,
		SystemTableHash* systemTableHash_,
		Setting* setting_) :scannerAR(DataTablePtr::make(nullptr, nullptr, FKA(FILEKEYPRIMARY, Addr()))) {
		codeTableHash = codeTableHash_;
		scopeTableHash = scopeTableHash_;
		templateFunctionTableHash = templateFunctionTableHash_;
		systemTableHash = systemTableHash_;
		currentAR = this->scannerAR;
		this->setting = setting_;

	}

	Scanner::Scanner(HashCollection* hashCollection) :scannerAR(DataTablePtr::make(nullptr, nullptr, FKA(FILEKEYPRIMARY, Addr()))) {
		codeTableHash = &hashCollection->cdth;
		scopeTableHash = &hashCollection->scpth;
		templateFunctionTableHash = &hashCollection->tfth;
		systemTableHash = &hashCollection->systh;
		setting = &hashCollection->setting;
		currentAR = this->scannerAR;

	}



	/**
	 * @brief 定位一个引用被使用的地址，然后分析代码并初始化一个referenceData
	 * @param rootAR 引用所在的ar
	 * @param codeTable
	 * @param currentCodeFKA【引用被定义的地址，此地址（类型为Y）】
	 * @param pos 【0,1,2,3】
	 */

	void ReferenceTable::addRefData(DataTablePtr rootAR,
		CodeTableHash* codeTableHash_, const FKA& currentCodeFKA_,
		const Intg& pos) {
#if debug
			{
				static int ignorecount = 0;
				cout
					<< "ReferenceTable::addRefData(DataTablePtr, CodeTableHash*, const FKA&, const int&)"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif

			FKA fkaref = currentCodeFKA_;
			Code* code = &(*codeTableHash_)[fkaref];
			Arg&& argref = (*code)[pos];
			argref.ref_ar = rootAR;
#if debug
			{
				static int ignorecount = 0;
				cout
					<< "ReferenceTable::addRefData(DataTablePtr, CodeTableHash*, const Addr&, const int&)"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
			auto&& it = argfka_ref_map.find(argref.arg_fka);
#if debug
			{
				static int ignorecount = 0;
				cout
					<< "ReferenceTable::addRefData(DataTablePtr, CodeTableHash*, const Addr&, const int&)"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
			if (it != argfka_ref_map.end()) {
				Intg n = argfka_ref_map.count(argref.arg_fka);
				while (--n >= 0) {
					if (it->second.rootAR == rootAR) {
#if debug
						{
							static int ignorecount = 0;
							cout
								<< "addRefData(DataTablePtr, CodeTable*, const Addr&, const int&)"
								<< " referenceData exist,return"
								<< "\tignorecount:[" << ignorecount++ << "\t]("
								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
								<< endl;
						}
#endif
						return;
					}
					++it;
				}
				pair<FKA, ReferenceData> p(*it);
				p.second.rootAR = rootAR;
				argfka_ref_map.insert(p);
				return;
			}
			pair<FKA, ReferenceData> p;
			while (true) {
				if (code->includeArg(argref, acm::name | acm::asc)) {
					Arg&& arg1 = (*code)[0];
					Arg&& arg2 = (*code)[1];
					Arg&& argop = (*code)[2];
					p.second.referencePath.push_front(arg2);
					if (arg1.argFlag == Y_Dz) {
						argref = arg1;
						fkaref = codeTableHash_->getLast(COOL_Y, fkaref);
						code = &(*codeTableHash_)[fkaref];
					}
					else {
						p.second.referencePath.push_front(arg1);
						break;
					}

				}
			}
			p.first = argref.arg_fka;
			p.second.rootAR = rootAR;
			p.second.scopeFKA = (*codeTableHash_)[currentCodeFKA_].scopeFKA;
			argfka_ref_map.insert(p);
			return;

	}
	/**
	 * @brief 定位一个引用被使用的地址，以及引用变量，然后分析代码并初始化一个referenceData
	 * @param rootAR 非scan模式专用
	 * @param currentCodeFKA【引用被定义的地址，此地址（类型为Y）】
	 * @param argref 【类型Y_Dz】
	 */
	void ReferenceTable::addRefData(DataTablePtr rootAR,
		CodeTableHash* codeTableHash_, const FKA& currentCodeFKA_, Arg argref) {
#if debug
			{
				static int ignorecount = 0;
				cout << "addRefData(DataTablePtr, CodeTable*, const Addr&, Arg)" << " "
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif

			FKA adref = currentCodeFKA_;
			Code* code = &(*codeTableHash_)[adref];

			auto&& it = argfka_ref_map.find(argref.arg_fka);
			if (it != argfka_ref_map.end()) {
				Intg n = argfka_ref_map.count(argref.arg_fka);
				while (--n >= 0) {
					if (it->second.rootAR == rootAR) {
#if debug
						{
							static int ignorecount = 0;
							cout
								<< "addRefData(DataTablePtr, CodeTable*, const Addr&, const int&)"
								<< " referenceData exist,return"
								<< "\tignorecount:[" << ignorecount++ << "\t]("
								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
								<< endl;
						}
#endif
						return;
					}
					++it;
				}
				pair<FKA, ReferenceData> p(*it);
				p.second.rootAR = rootAR;
				argfka_ref_map.insert(p);
				return;
			}
			pair<FKA, ReferenceData> p;
			while (true) {
				if (code->includeArg(argref, acm::name | acm::asc)) {
					Arg&& arg1 = (*code)[0];
					Arg&& arg2 = (*code)[1];
					Arg&& argop = (*code)[2];
					p.second.referencePath.push_front(arg2);
					if (arg1.argFlag == Y_Dz) {
						argref = arg1;
						adref = codeTableHash_->getLast(COOL_Y, adref);
						code = &(*codeTableHash_)[adref];
					}
					else {
						p.second.referencePath.push_front(arg1);
						break;
					}

				}
			}
			p.first = adref;
			p.second.rootAR = rootAR;
			p.second.scopeFKA = (*codeTableHash_)[currentCodeFKA_].scopeFKA;
			argfka_ref_map.insert(p);
			return;

	}
	void ReferenceTable::clear() {
#if debug
		{
			static int ignorecount = 0;
			cout << "ReferenceTable::clear()" << " " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif
		argfka_ref_map.clear();
	}
	void ReferenceTable::erase(const Arg& arg) {
		if (arg.argFlag != Y_Dz) {

			{
				static int ignorecount = 0;
				cout << "void erase(const Arg &arg)" << " arg isn't ref,exit(-1)"
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << endl;
			}

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}
		argfka_ref_map.erase(arg.arg_fka);
	}
	/**
	 * @brief 根据arg获得其对应的referenceData（注意得到的不是引用），参数arg的asc以表示其所引用的参数所在scope
	 * @param arg必须为引用Y_Dz！
	 * @return
	 */

	ReferenceData ReferenceTable::getReferenceData(const Arg& y_dz_arg) {
#if debug
		{
			static int ignorecount = 0;
			cout << "ReferenceTable::getReferenceData(const Arg&)" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif

		const DataTablePtr rootAR = y_dz_arg.ref_ar;
		const Arg& arg = y_dz_arg;
		if (arg.argFlag != Y_Dz) {

			{
				static int ignorecount = 0;
				cout << "operator [](const Arg&)" << " arg isn't ref,exit(-1)"

					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__ << ":"
					<< __LINE__ << ":0" << ")" << endl;
			}

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}

		if (rootAR == nullptr) {
			return argfka_ref_map.find(arg.arg_fka)->second;
		}
		else {
			Intg n = argfka_ref_map.count(arg.arg_fka);
			auto it = argfka_ref_map.find(arg.arg_fka);
			while (--n >= 0) {
				if ((*it).second.rootAR == rootAR) {

					return it->second;
				}
				++it;
			}

			{
				static int ignorecount = 0;
				cerr << "getReferenceData(const Arg&)"
					<< " can't find matched AR,exit(-1)" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}

	}
	/**
	 * @brief 根据arg获得其对应的参数（非Data），参数arg的asc以表示其所引用的参数所在scope
	 * @param arg必须为引用Y_Dz！
	 * @return the real Arg pointed by y_dz_arg
	 */
	Arg ReferenceTable::operator[](const Arg& y_dz_arg) const {

#if debug
		{
			static int ignorecount = 0;
			cout << "ReferenceTable::operator [](const Arg&)" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif

		const DataTablePtr rootAR = y_dz_arg.ref_ar;
		const Arg& arg = y_dz_arg;
		if (arg.argFlag != Y_Dz) {

			{
				static int ignorecount = 0;
				cout << "ReferenceTable::operator [](const Arg&) arg:["
					<< arg.toStrg() << "]" << " arg isn't ref, exit(-1)"

					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << endl;
			}

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}

		if (rootAR == nullptr) {
			return argfka_ref_map.find(arg.arg_fka)->second.getArg();
		}
		else {
			Intg n = argfka_ref_map.count(arg.arg_fka);
			auto it = argfka_ref_map.find(arg.arg_fka);
			while (--n >= 0) {
				if ((*it).second.rootAR == rootAR) {

					return it->second.getArg();
				}
				++it;
			}

			{
				static int ignorecount = 0;
				cout << "ReferenceTable::operator [](const Arg&)"
					<< " can't find matched AR, exit(-1)" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}

	}
	/**
	 * @brief 根据arg获得其对应的参数（非Data），参数arg的asc以表示其所引用的参数所在scope
	 * @param arg必须为引用Y_Dz！
	 * @return
	 */
	Data& ReferenceTable::getDataRef(const Arg& y_dz_arg) {
		const Arg& arg = y_dz_arg;
		const DataTablePtr rootAR = y_dz_arg.ref_ar;
		if (arg.argFlag != Y_Dz) {

			{
				static int ignorecount = 0;
				cout << "getDataRef(const Arg &arg)" << " arg isn't ref,exit(-1)"

					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__ << ":"
					<< __LINE__ << ":0" << ")" << endl;
			}

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}
		if (rootAR == nullptr) {

			{
				static int ignorecount = 0;
				cout << "getDataRef(const pair<DataTablePtr,Arg>)"
					<< " rootAR == nullptr,can't get examples" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}
		else {
			Intg n = argfka_ref_map.count(arg.arg_fka);
			auto&& it = argfka_ref_map.find(arg.arg_fka);
			while (--n >= 0) {
				if ((*it).second.rootAR == rootAR) {
					return it->second.getData();
				}
				++it;
			}

			{
				static int ignorecount = 0;
				cerr << "getDataRef(const Arg &arg)"
					<< " can't find matched AR,exit(-1)" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}

	}

	/*********************************************

	Scanner

	*****************************************/


	/**
		 * @brief 将code进行解析，更新（reset）参数属性表中的属性，尤其是地址属性
		 * @param code
		 * @brief 用于在扫描过程中存储各个变量对应的属性。
		 * @brief 包括S_Dz、S_AR、、Z_Dz、S_AR、Y_Dz、S_Bs、EXPRZ_Dz等类型
		 * @attention 由于在经过一些操作后变量的类型可能会修改，所以注意及时更新变量的类型
		 */
	void Scanner::updateArgAttribute(const Code& code) {
#if debug
		{
			static int ignorecount = 0;
			cout << "updateArgAttribute(const Code&)" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif
		// setArgScopeFKA();
		addArgsToCurrentAR();

		Arg&& arg1 = code[0];
		Arg&& arg2 = code[1];
		Arg&& argop = code[2];
		Arg&& argres = code[3];
		if (arg1.isBs()) {
			resetAttribute(arg1.arg_s, currentCode.scopeFKA, arg1.argFlag,
				false);

		}
		else if (arg1.isDz()) {

			resetAttribute(arg1.arg_fka, currentCode.scopeFKA, arg1.argFlag,
				false);
		};
		if (arg2.isBs()) {
			resetAttribute(arg2.arg_s, currentCode.scopeFKA, arg2.argFlag,
				false);

		}
		else if (arg2.isDz()) {
			resetAttribute(arg2.arg_fka, currentCode.scopeFKA, arg2.argFlag,
				false);
		};
		if (argop.isBs()) {
			resetAttribute(argop.arg_s, currentCode.scopeFKA, argop.argFlag,
				false);

		}
		else if (argop.isDz()) {
			resetAttribute(argop.arg_fka, currentCode.scopeFKA, argop.argFlag,
				false);
		};
		if (argres.isBs()) {
			resetAttribute(argres.arg_s, currentCode.scopeFKA, argres.argFlag,
				false);

		}
		else if (argres.isDz()) {
			resetAttribute(argres.arg_fka, currentCode.scopeFKA, argres.argFlag,
				false);
		};
	}
	Scanner::~Scanner() {
		//for (auto& ar : DataTable::heapObjSet) {
		//	delete ar;
		//}
		//DataTable::heapObjSet.clear();
	}

	/**
	 * @brief 将filePath指向的源文件翻译成多个AssemblyCodeTable，储存在数组中。
	 * @attention 每当切换一次文件，就再次生成一个assemblyCodeTable填在数组后部
	 * @param filePath（文件的绝对路径）
	 * @return
	 */
	vector<AssemblyCodeTable> Scanner::sourceCodeToAssemblyCode(
		const filesystem::path& filePath) {
		using namespace filesystem;
		{
#if debug
			static int ignorecount = 0;
			cout << "Scanner::loadAssemblyCode(const Strg&)" << " filePath:["
				<< filePath << "]" << "\tignorecount:[" << ignorecount++
				<< "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
#endif
		}
		path filePath_(filePath);
		Strg fileStem = pathToUTF8(filePath_.stem()); //当前assemblyCodeTable所处的file的fileKey（name）
		if (fileKeyExistSet_asc.count(fileStem) > 0) {
			return vector<AssemblyCodeTable>();

		}
		else {
			fileKeyExistSet_asc.insert(fileStem);
		}
		fstream f(filePath.c_str(), fstream::in);

		string line;
		AssemblyCode asc;
		AssemblyCode lastAsc;
		AssemblyCodeTable assemblyCodeTable;
		assemblyCodeTable.completed = true;
		assemblyCodeTable.codeForm = 2;
		vector<AssemblyCodeTable> asctv;
		bool adjust_code_order_at_end_of_expression = false;
		Intg position = 0;
		Intg fileSubSerialNumber = 0;                //用于标识当文件中有一个或多个load时被分割的情况
		vector<Strg> fkLOAD2;  //记录当前文件依赖多少coc文件
		vector<Strg> fkLOAD3;  //记录当前文件依赖多少cob文件
		while (getline(f, line)) {
			[[maybe_unused]] Intg pos__1 = f.tellg();
			[[maybe_unused]] Intg pos__2 = f.tellg();
			if (line == "") {
				continue;
			}
			++position;
#if debugasctv
			{
				static int ignorecount = 0;
				std::cout << "Scanner::sourceCodeToAssemblyCode(const Strg&)"
					<< " line:[" << line << "]" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << std::endl;
			}
#endif
			lastAsc = asc;
			asc = AssemblyCode(line);
			if (asc == ascnull) {
				assemblyCodeTable.acdq.push_back(asc);
				assemblyCodeTable.acpdq.push_back(position);
				continue;
			}

			//根据情况导入依赖文件
			if (asc.type == COOL_LOAD) {

				//如果#load所依赖的文件已经导入过此文件，则不再重新导入文件
				if (loadedFiles.count(asc.arg1_s) > 0) {
					//                ++position;
					assemblyCodeTable.acdq.push_back(ascnull); //以空指令替换掉load指令
					assemblyCodeTable.acpdq.push_back(position);
					continue;
				}
				{
					//由于禁用了多文件，所以文件一定已经被导入，直接跳过。启用多文件时请禁用作用域
					//                ++position;

					assemblyCodeTable.acdq.push_back(ascnull); //以空指令替换掉load指令
					assemblyCodeTable.acpdq.push_back(position);
					assemblyCodeTable.completed = true;
					asctv.push_back(assemblyCodeTable);

					fileStem = asc.arg1_s;
					assemblyCodeTable.fileKey = fileStem + "#"
						+ toStrg(fileSubSerialNumber);
					++fileSubSerialNumber;
					assemblyCodeTable.acdq.clear();
					assemblyCodeTable.acpdq.clear();
					assemblyCodeTable.completed = true;
					adjust_code_order_at_end_of_expression = false;
					continue;
				}
				loadedFiles.insert(make_pair(asc.arg1_s, 0));

				//1 读取下一非空行判断是否已经将待加载文件融合入filepath指向的coc文件中
				Intg filePointerPos = f.tellg();
				string line_;
				getline(f, line_);

				AssemblyCode nextAssemblyCode_(line_);
				while (nextAssemblyCode_.type == COOL_ME || nextAssemblyCode_.type == COOL_N) {
					getline(f, line_);
					nextAssemblyCode_ = AssemblyCode(line_);

				}

				Intg curfpos = filePointerPos;
				f.seekg(curfpos);
				getline(f, line_);

				while (line_ != line) {

					--curfpos;
					f.seekg(curfpos);
					getline(f, line_);

				}

				std::streampos pos__3 = f.tellg();

				//            getline(f, line_);
				bool fileMerged = nextAssemblyCode_.type == COOL_FIL
					&& nextAssemblyCode_.arg1_s == asc.arg1_s;
				//如果#load所依赖的文件已经融入此文件，则不再重新导入文件
				if (fileMerged == true) {
					//                ++position;
					assemblyCodeTable.acdq.push_back(ascnull); //以空指令替换掉load指令
					assemblyCodeTable.acpdq.push_back(position);
					assemblyCodeTable.completed = true;
					++fileSubSerialNumber;
					asctv.push_back(assemblyCodeTable);
					assemblyCodeTable.fileKey = asc.arg1_s + "#"
						+ toStrg(fileSubSerialNumber);
					assemblyCodeTable.acdq.clear();
					assemblyCodeTable.acpdq.clear();
					assemblyCodeTable.completed = true;
					adjust_code_order_at_end_of_expression = false;

					continue;
				}
				//2导入依赖文件
				//2.1（优先加载编译好的cob文件）
				auto&& path_cob_v1 = getFilePath(coolang_project_path.c_str(),
					asc.arg1_s + COOLANG_BINARY_CODE_SUFFIX, true);
				if (path_cob_v1.size() <= 0) {
					auto&& path_cob_v2 = getFilePath(coolang_exec_path.c_str(),
						asc.arg1_s + COOLANG_BINARY_CODE_SUFFIX, true);
					path_cob_v1.insert(path_cob_v1.end(), path_cob_v2.begin(),
						path_cob_v2.end());
					if (path_cob_v1.size() <= 0) {
						auto&& path_cob_v3 = getFilePath(coolang_home_path.c_str(),
							asc.arg1_s + COOLANG_BINARY_CODE_SUFFIX, true);
						path_cob_v1.insert(path_cob_v1.end(), path_cob_v3.begin(),
							path_cob_v3.end());
					}
				}
				if (path_cob_v1.size() > 0) {
					fkLOAD3.push_back(asc.arg1_s);
					if (assemblyCodeTable.acdq.size() > 0) {
						assemblyCodeTable.fileKey = fileStem + "#"
							+ toStrg(fileSubSerialNumber);
						++fileSubSerialNumber;
						asctv.push_back(assemblyCodeTable);
						assemblyCodeTable.acdq.clear();
						assemblyCodeTable.acpdq.clear();
						//                    position = 0;
						adjust_code_order_at_end_of_expression = false;
					}
					assemblyCodeTable.fileKey = asc.arg1_s;
					assemblyCodeTable.completed = false;
					assemblyCodeTable.codeForm = 3;
					assemblyCodeTable.acdq.push_back(asc);
					assemblyCodeTable.acpdq.push_back(position);
					++fileSubSerialNumber;
					asctv.push_back(assemblyCodeTable);
					assemblyCodeTable.acdq.clear();
					assemblyCodeTable.acpdq.clear();
					assemblyCodeTable.fileKey = fileStem;
					assemblyCodeTable.completed = true;
					assemblyCodeTable.codeForm = 2;
					//                position = 0;
					adjust_code_order_at_end_of_expression = false;
				}
				else {
					//没有找到编译好的cob文件，则加载coc文件
					auto&& path_coc_v1 = getFilePath(coolang_project_path.c_str(),
						asc.arg1_s + COOLANG_CHARACTER_CODE_SUFFIX, true);
					if (path_coc_v1.size() <= 0) {
						auto&& path_coc_v2 = getFilePath(coolang_exec_path.c_str(),
							asc.arg1_s + COOLANG_CHARACTER_CODE_SUFFIX, true);
						path_coc_v1.insert(path_coc_v1.end(), path_coc_v2.begin(),
							path_coc_v2.end());
						if (path_coc_v1.size() <= 0) {
							auto&& path_coc_v3 = getFilePath(
								coolang_home_path.c_str(),
								asc.arg1_s + COOLANG_CHARACTER_CODE_SUFFIX,
								true);
							path_coc_v1.insert(path_coc_v1.end(),
								path_coc_v3.begin(), path_coc_v3.end());

						}

					}
					fkLOAD2.push_back(asc.arg1_s);
					if (assemblyCodeTable.acdq.size() > 0) {
						assemblyCodeTable.fileKey = fileStem + "#"
							+ toStrg(fileSubSerialNumber);
						++fileSubSerialNumber;
						assemblyCodeTable.completed = true;
						asctv.push_back(assemblyCodeTable);
						assemblyCodeTable.acdq.clear();
						assemblyCodeTable.acpdq.clear();
						adjust_code_order_at_end_of_expression = false;
						//                    position = 0;

					}
					if (path_coc_v1.size() <= 0) {

						{
							static int ignorecount = 0;
							std::cout
								<< "Scanner::sourceCodeToAssemblyCode(const Strg&) fileName:["
								<< asc.arg1_s << "]"
								<< " can't found #load files, exit(-1) "
								<< "\tignorecount:[" << ignorecount++ << "\t]("
								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
								<< std::endl;
						}

#if debug && _WIN32
						{
							static int ignoreCount = 0;
							// Assuming info is a std::string, convert to std::wstring
							std::string info = "error exit"; // Replace with actual info
							info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
							std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
							OutputDebugString(winfo.c_str());
#endif
							cout << info << endl;
							ignoreCount++;
						}
#endif
						exit(-1);
					}

					vector<AssemblyCodeTable>&& asctv_ = sourceCodeToAssemblyCode(
						path_coc_v1.front().string());

					set<Strg> asctfkvexist;
					for (auto& asctexist : asctv) {
						asctfkvexist.insert(asctexist.fileKey);
					}

					for (Intg i = 0, len = asctv_.size(); i < len; ++i) {
						if (asctfkvexist.count(asctv_[i].fileKey) > 0) {
							continue;
						}
						asctv.push_back(asctv_[i]);

					}
					assemblyCodeTable.fileKey = fileStem + "#"
						+ toStrg(fileSubSerialNumber);
					++fileSubSerialNumber;
					asctv.push_back(assemblyCodeTable);
					assemblyCodeTable.acdq.clear();
					assemblyCodeTable.acpdq.clear();
					assemblyCodeTable.completed = true;
					adjust_code_order_at_end_of_expression = false;
					//                position = 0;

				}
			}
			//3将文件依赖关系写入assemblyCodeTable
			if (fkLOAD2.size() > 0) {

				assemblyCodeTable.dependentCdtFileKey_CodeForm2List.insert(
					assemblyCodeTable.dependentCdtFileKey_CodeForm2List.begin(),
					fkLOAD2.begin(), fkLOAD2.end());

			}
			if (fkLOAD3.size() > 0) {

				assemblyCodeTable.dependentCdtFileKey_CodeForm3List.insert(
					assemblyCodeTable.dependentCdtFileKey_CodeForm3List.begin(),
					fkLOAD3.begin(), fkLOAD3.end());

			}
			//todo:: 多文件被禁用，此处为模拟的多文件,后续开发请注意
			if (asc.type == COOL_FIL && asc.arg1_s != fileStem) {
				if (fileStem == "") {
					fileStem = asc.arg1_s;
					this->assemblyCodeTable->fileKey = fileStem;
					assemblyCodeTable.acdq.push_back(asc);
					assemblyCodeTable.acpdq.push_back(position);
					continue;
				}
				else {
					assemblyCodeTable.fileKey = fileStem + "#" + COOLANG::toStrg(max(0, fileSubSerialNumber - 1));
					assemblyCodeTable.acdq.push_back(ascnull); //以空指令替换掉load指令
					assemblyCodeTable.acpdq.push_back(position);
					assemblyCodeTable.completed = true;
					asctv.push_back(assemblyCodeTable);

					fileStem = asc.arg1_s;
					assemblyCodeTable.fileKey = fileStem + "#"
						+ toStrg(fileSubSerialNumber);
					++fileSubSerialNumber;
					assemblyCodeTable.acdq.clear();
					assemblyCodeTable.acpdq.clear();
					assemblyCodeTable.completed = true;
					adjust_code_order_at_end_of_expression = false;
					continue;
				}

			}
			else if (asc.type == COOL_Y || asc.type == COOL_LST) {
				adjust_code_order_at_end_of_expression = true;
				assemblyCodeTable.acdq.push_back(asc);
				assemblyCodeTable.acpdq.push_back(position);
			}
			else if (adjust_code_order_at_end_of_expression
				== true && asc.type != COOL_Y && asc.type != COOL_LST && asc.type != COOL_M
				&& asc.type != COOL_N && asc.type != COOL_LIN && asc.type != COOL_FIL
				&& asc.type != COOL_LOAD) {
				adjust_code_order_at_end_of_expression = false;
				AssemblyCodeTable&& asct = assemblyCodeTable.cutLongestExpression();
				Intg position_old = position;
				position = assemblyCodeTable.acpdq.back();

				AssemblyCodeTable&& asct_deformed = assemblyCodeTableDeformation(
					asct);
				assemblyCodeTable.acdq.insert(assemblyCodeTable.acdq.end(),
					asct_deformed.acdq.begin(), asct_deformed.acdq.end());
				for (int i = 0; i < (Intg)asct_deformed.acdq.size(); i++) {
					++position;
					assemblyCodeTable.acpdq.push_back(position);
				}
				if (position < position_old) {
					position = position_old;
				}
				else {

					{
						static int ignorecount = 0;
						std::cerr
							<< "Scanner::sourceCodeToAssemblyCode(const Strg&)"
							<< " asct deformation misposition, exit(-1) "
							<< "\tignorecount:[" << ignorecount++ << "\t]("
							<< __FILE__ << ":" << __LINE__ << ":0" << ")"
							<< std::endl;
					}

#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = "error exit"; // Replace with actual info
						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
						std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
						OutputDebugString(winfo.c_str());
#endif
						cout << info << endl;
						ignoreCount++;
					}
#endif
					exit(-1);

				}
				//            ++position;
				assemblyCodeTable.acdq.push_back(asc);
				assemblyCodeTable.acpdq.push_back(position);
			}
			else {
				assemblyCodeTable.acdq.push_back(asc);
				assemblyCodeTable.acpdq.push_back(position);
			}
		}
		f.close();
		assemblyCodeTable.fileKey = fileStem + "#" + toStrg(fileSubSerialNumber);
		asctv.push_back(assemblyCodeTable);
		return asctv;

	}
	bool Scanner::loadAssemblyCode(const filesystem::path& filePath) {
		{
#if debug
			static int ignorecount = 0;
			cout << "Scanner::loadAssemblyCode(const Strg&)" << " filePath:["
				<< filePath << "]" << "\tignorecount:[" << ignorecount++
				<< "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
#endif
		}
		vector<AssemblyCodeTable>&& asctv = sourceCodeToAssemblyCode(filePath);
		for (auto& asct : asctv) {

			ascth.appendAsct(asct);

		}

#if debug
		{
			static int ignorecount = 0;
			cout << "Scanner::loadAssemblyCode(const Strg&)" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << endl;
			cout << "||||||||||||||||||||||||||||||||||||||" << endl;
			cout << ascth.toStrg() << endl;
		}
#endif
		return true;
	}
	/**
	 * @attention 此步骤仅仅更新assemblyCodeTable、currentAssemblyCode、和currentFileKey、nextAssemblyCode、lastM，
	 * 对于currentCodeFKA等需要根据具体代码生成的地址不在此处更新
	 * @return
	 */
	const bool Scanner::updateCurrentAssemblyCode() {
		++currentAssemblyCodePos;

		//选取asc
		if (assemblyCodeTable == nullptr && ascth.size() > 0) {
			currentFileKey = ascth.addr_file_map.begin()->second;
			assemblyCodeTable = &ascth[currentFileKey];
			fileKeyExistSet.insert(currentFileKey);

		}
		//载入load3updateCurrentAssemblyCode
		if (assemblyCodeTable->completed == false
			&& assemblyCodeTable->codeForm == 3) {
			auto pathv1 = getFilePath(coolang_exec_path.c_str(),
				assemblyCodeTable->fileKey + COOLANG_BINARY_CODE_SUFFIX, true);
			if (pathv1.size() <= 0) {
				pathv1 = getFilePath(coolang_project_path,
					assemblyCodeTable->fileKey + COOLANG_BINARY_CODE_SUFFIX,
					true);
				if (pathv1.size() <= 0) {
					pathv1 = getFilePath(coolang_home_path,
						assemblyCodeTable->fileKey + COOLANG_BINARY_CODE_SUFFIX,
						true);
					if (pathv1.size() <= 0) {

						{
							static int ignorecount = 0;
							cerr << "Scanner::updateCurrentAssemblyCode()"
								<< " FileNotFound, exit(-1) "
								<< "\tignorecount:[" << ignorecount++ << "\t]("
								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
								<< endl;
						}

#if debug && _WIN32
						{
							static int ignoreCount = 0;
							// Assuming info is a std::string, convert to std::wstring
							std::string info = "error exit"; // Replace with actual info
							info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
							std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
							OutputDebugString(winfo.c_str());
#endif
							cout << info << endl;
							ignoreCount++;
						}
#endif
						exit(-1);
					}
				}
			}
			HashCollection hc;
			hc.loadFromBinaryFile(pathToPathStr(pathv1.front()));
			hc.completeAll();
			fileKeyExistSet.erase(currentFileKey);
			for (auto& afp : hc.cdth.addr_file_map) {
				if (fileKeyExistSet.count(afp.second) > 0) {
					continue;
				}
				else {

					fileKeyExistSet.insert(afp.second);
					CodeTable& cdt_ = hc.cdth[afp.second];
					this->codeTableHash->appendCdt(cdt_);
					if (hc.scpth.file_scpt_map.count(afp.second) > 0) {
						ScopeTable& scp_ = hc.scpth[afp.second];
						this->scopeTableHash->add(scp_);
					}
					if (hc.tfth.file_tft_map.count(afp.second) > 0) {
						TemplateFunctionTable& tft_ = hc.tfth[afp.second];
						this->templateFunctionTableHash->add(tft_);
					}
					if (hc.systh.file_syst_map.count(afp.second) > 0) {
						SystemTable& syst_ = hc.systh[afp.second];
						this->systemTableHash->add(syst_);
					}

				}

			}

			while (true) {

				currentCodeFKA = codeTableHash->getNextCodeFKA(currentCodeFKA);
				if (currentCodeFKA == fkanull) {
					break;
				}
				currentCode = codeTableHash->operator [](currentCodeFKA);
				updateArgAttribute(currentCode);
			}
			//从nextfileKey的asct开始
			Strg currentFileKey = ascth.getNextAsctFileKey(currentFileKey);
			if (currentFileKey == "") {
				return false;
			}
			else {
				fileKeyExistSet.insert(currentFileKey);
				assemblyCodeTable = &ascth[currentFileKey];
				currentAssemblyCodePos = -1;
				return updateCurrentAssemblyCode();
			}

		}

		if (currentCode.assemblyFormula.flagBit.type == COOL_M
			|| currentCode.assemblyFormula.flagBit.type == COOL_R) {
			lastM = currentCodeFKA;
		}
		//由于此函数在每个scan周期内最先执行，因此currentAssemblyCodePos直接自增。
		//但考虑到其初始值为0，自增为1后会跳过第一行代码，因此后面应用时再减一
		//    currentAssemblyCodePos++;
		currentCode = codenull;
		Intg size_ = assemblyCodeTable->acdq.size();
		if (currentAssemblyCodePos + 1 < size_) {
			currentAssemblyCode = assemblyCodeTable->acdq.at(
				currentAssemblyCodePos);
			nextAssemblyCode = assemblyCodeTable->acdq.at(
				currentAssemblyCodePos + 1);

		}
		else if (currentAssemblyCodePos + 1 == size_) {

			currentAssemblyCode = assemblyCodeTable->acdq.at(
				currentAssemblyCodePos);
			nextAssemblyCode = AssemblyCode();

		}
		else {
			currentFileKey = ascth.getNextAsctFileKey(currentFileKey);
			if (currentFileKey == "") {
				return false;
			}
			else {
				fileKeyExistSet.insert(currentFileKey);
				assemblyCodeTable = &ascth[currentFileKey];
				currentAssemblyCodePos = -1;
				return updateCurrentAssemblyCode();
			}
			return false;
		}

		return true;
	}
	/**
	 * @brief 设置变量对应的scopeFKA，只有当currentCode中的formalArg标志位正确设置后此才有用。
	 * @return
	 */
	const bool [[deprecated("use Executor::setArgScopeFKA/FKA")]] Scanner::setArgScopeFKA() {
#if debug
		{
			static int ignorecount = 0;
			cout << "Scanner::setArgScopeFKA()" << " in " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif
		if (currentCode.assemblyFormula.flagBit.type == COOL_C
			&& currentCode[0].argFlag == S_Bs && currentCode[1].argFlag == X_Bs) {
			currentCode.assemblyFormula.flagBit.a1s = currentCode.scopeFKA;
			currentCode.assemblyFormula.flagBit.res = currentCode.scopeFKA;
			(*currentAR).add(currentCode[0]);
		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_M
			&& currentCode.assemblyFormula.flagBit.execute == F_) {


#if debug
			cout << "Scanner::setArgScopeFKA a1s" << endl;
#endif
			if (currentCode[0].isVar()) {

				try {
					currentCode.assemblyFormula.flagBit.a1s =
						currentAR->getArgScopeFKA(currentCode[0]);
				}
				catch (Arg& var_not_exist) {
					currentCode.assemblyFormula.flagBit.a1s = currentCode.scopeFKA;
				}
			}

#if debug
			cout << "Scanner::setArgScopeFKA a2s" << endl;
#endif
			if (currentCode[1].isVar()) {
				try {
					currentCode.assemblyFormula.flagBit.a2s =
						currentAR->getArgScopeFKA(currentCode[1]);
				}
				catch (Arg& var_not_exist) {
					currentCode.assemblyFormula.flagBit.a2s = currentCode.scopeFKA;
				}
			}

#if debug
			cout << "Scanner::setArgScopeFKA ops" << endl;
#endif
			if (currentCode[2].isVar()) {
				try {
					currentCode.assemblyFormula.flagBit.ops =
						currentAR->getArgScopeFKA(currentCode[2]);
				}
				catch (Arg& var_not_exist) {
					currentCode.assemblyFormula.flagBit.ops = currentCode.scopeFKA;
				}
			}
#if debug
			cout << "Scanner::setArgScopeFKA res" << endl;
#endif
			if (currentCode[3].isVar()) {
				try {
					currentCode.assemblyFormula.flagBit.res =
						currentAR->getArgScopeFKA(currentCode[3]);
				}
				catch (Arg& var_not_exist) {
					currentCode.assemblyFormula.flagBit.res = currentCode.scopeFKA;
				}
			}
#if debug
			cout << "Scanner::setArgScopeFKA end" << endl;
#endif

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_R
			&& currentCode.assemblyFormula.flagBit.execute == T_) {
			currentCode.assemblyFormula.flagBit.a1s = currentAR->getArgScopeFKA(
				currentCode[0]);

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_ABT
			&& currentCode.assemblyFormula.flagBit.execute == T_) {
			//nothing 
		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_M
			&& currentCode.assemblyFormula.flagBit.execute == T_) {
			if ((currentCode.assemblyFormula.flagBit.result_flag == S_Dz)
				&& currentCode.assemblyFormula.flagBit.formalArgFlag[3]
				== true) {
				(*currentAR).add(currentCode[3]);
			}
#if debug
			cout << "Scanner::setArgScopeFKA a1s2" << endl;
#endif
			if (currentCode[0].isVar()) {
				try {
					currentCode.assemblyFormula.flagBit.a1s =
						currentAR->getArgScopeFKA(currentCode[0]);
				}
				catch (Arg& var_not_exist) {
					currentCode.assemblyFormula.flagBit.a1s = currentCode.scopeFKA;
				}
			}

#if debug
			cout << "Scanner::setArgScopeFKA a2s2" << endl;
#endif
			if (currentCode[1].isVar()) {
				try {
					currentCode.assemblyFormula.flagBit.a2s =
						currentAR->getArgScopeFKA(currentCode[1]);
				}
				catch (Arg& var_not_exist) {
					currentCode.assemblyFormula.flagBit.a2s = currentCode.scopeFKA;
				}
			}

#if debug
			cout << "Scanner::setArgScopeFKA ops2" << endl;
#endif
			if (currentCode[2].isVar()) {
				try {
					currentCode.assemblyFormula.flagBit.ops =
						currentAR->getArgScopeFKA(currentCode[2]);
				}
				catch (Arg& var_not_exist) {
					currentCode.assemblyFormula.flagBit.ops = currentCode.scopeFKA;
				}
			}
#if debug
			cout << "Scanner::setArgScopeFKA res2" << endl;
#endif
			if (currentCode[3].isVar()) {
				try {
					currentCode.assemblyFormula.flagBit.res =
						currentAR->getArgScopeFKA(currentCode[3]);
				}
				catch (Arg& var_not_exist) {
					currentCode.assemblyFormula.flagBit.res = currentCode.scopeFKA;
				}
			}
#if debug
			cout << "Scanner::setArgScopeFKA end2" << endl;
#endif

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_QS
			&& currentCode.assemblyFormula.flagBit.execute == F_) {
			currentCode.assemblyFormula.flagBit.res =
				currentCode.assemblyFormula.quaternion.result_fka;

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_QE
			&& currentCode.assemblyFormula.flagBit.execute == F_) {
			currentCode.assemblyFormula.flagBit.res =
				currentCode.assemblyFormula.quaternion.result_fka;

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_QS
			&& currentCode.assemblyFormula.flagBit.execute == COOL_R) {
			currentCode.assemblyFormula.flagBit.res =
				currentCode.assemblyFormula.quaternion.result_fka;

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_QE
			&& currentCode.assemblyFormula.flagBit.execute == COOL_R) {
			currentCode.assemblyFormula.flagBit.res =
				currentCode.assemblyFormula.quaternion.result_fka;

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_FUN) {
			//        //这是原来的
			//        currentCode.assemblyFormula.flagBit.a1s =
			//                currentCode.assemblyFormula.quaternion.arg1_fka;
			//        currentCode.assemblyFormula.flagBit.a2s =
			//                currentCode.assemblyFormula.quaternion.arg2_fka;
			//        currentCode.assemblyFormula.flagBit.res =
			//                currentCode.assemblyFormula.quaternion.result_fka;
					//修改时间2022年7月24日21:56:55
			currentCode.assemblyFormula.flagBit.a1s = currentCode.scopeFKA;
			currentCode.assemblyFormula.flagBit.a2s = currentCode.scopeFKA;
			currentCode.assemblyFormula.flagBit.res = currentCode.scopeFKA;

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_TD) {
			//        //这是原来的
			//修改时间2022年7月24日21:56:55
			currentCode.assemblyFormula.flagBit.a1s = currentCode.scopeFKA;
			currentCode.assemblyFormula.flagBit.a2s = currentCode.scopeFKA;
			currentCode.assemblyFormula.flagBit.res = currentCode.scopeFKA;
		}
#if debug
		{
			static int ignorecount = 0;
			cout << "Scanner::setArgScopeFKA()" << " out " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif
		return true;
	}
	const bool Scanner::addArgsToCurrentAR() {
#if debug
		{
			static int ignorecount = 0;
			cout << "Scanner::addArgsToCurrentAR()" << " in " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif
		if (currentCode.assemblyFormula.flagBit.type == COOL_C
			&& currentCode[0].argFlag == S_Bs && currentCode[1].argFlag == X_Bs) {
			if (currentCode[1].arg_s == "new" || currentCode[1].arg_s == "out" || currentCode[1].arg_s == "placeholder") {
				(*currentAR).add(currentCode[0]);
			}
			else {
				//创建类的实例，直接使用类的AR
				(*currentAR).add(currentCode[0]);
				System& sys = (*systemTableHash)[pair<Strg, FKA>(
					currentCode.assemblyFormula.quaternion.arg2_s,
					currentCode.scopeFKA)];
				(*currentAR)[currentCode[0]] =
					(*currentAR)[Arg(S_Dz, sys.systemFKA)];
			}

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_M
			&& currentCode.assemblyFormula.flagBit.execute != Du) {
			/**
			 * 不用管引用，引用已经被置于COOL_M之前，已经被处理过了
			 */

			Arg&& arg_1 = currentCode[0];
			Arg&& arg_2 = currentCode[1];
			Arg&& arg_op = currentCode[2];
			Arg&& arg_res = currentCode[3];
			if (arg_1.isVar() && arg_1.argFlag != Y_Dz) {
				(*currentAR).add(currentCode[0]);
			}
			if (arg_2.isVar() && arg_2.argFlag != Y_Dz) {
				(*currentAR).add(currentCode[1]);
			}
			if (arg_op.isVar() && arg_op.argFlag != Y_Dz) {
				(*currentAR).add(currentCode[2]);
			}
			if (arg_res.isVar() && arg_res.argFlag != Y_Dz) {
				(*currentAR).add(currentCode[3]);
			}
		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_Y
			&& currentCode.assemblyFormula.flagBit.execute != Du) {
			Arg&& arg_1 = currentCode[0];
			Arg&& arg_2 = currentCode[1];
			Arg&& arg_op = currentCode[2];
			Arg&& arg_res = currentCode[3];
			//引用的arg1、arg2无意义，故忽略其添加
			/*if (arg_1.isVar() && arg_1.argFlag != Y_Dz) {
			 (*currentAR).add(currentCode[0]);
			 } else if (currentCode.assemblyFormula.flagBit.arg1_flag == Y_Dz) {

			 arg_1.ref_ar = currentAR;
			 (*currentAR).add(arg_1);
			 (*currentAR)[arg_1].content_ar = currentAR;
			 }
			 if (arg_2.isVar() && arg_2.argFlag != Y_Dz) {
			 (*currentAR).add(currentCode[1]);
			 } else if (currentCode.assemblyFormula.flagBit.arg2_flag == Y_Dz) {

			 arg_2.ref_ar = currentAR;
			 (*currentAR).add(arg_2);
			 (*currentAR)[arg_2].content_ar = currentAR;
			 }
			 if (currentCode.assemblyFormula.flagBit.operator_flag == S_Dz) {
			 (*currentAR).add(currentCode[2]);
			 } else if (currentCode.assemblyFormula.flagBit.operator_flag == Y_Dz) {
			 globalRefTable.addRefData(this->currentAR, codeTable,
			 currentCodeFKA, 2);

			 arg_op.ref_ar = currentAR;
			 (*currentAR).add(arg_op);
			 (*currentAR)[arg_op].content_ar = currentAR;
			 }*/
			if (arg_res.isVar() && arg_res.argFlag != Y_Dz) {
				(*currentAR).add(currentCode[3]);
			}
			else if (currentCode.assemblyFormula.flagBit.result_flag == Y_Dz) {
				globalRefTable.addRefData(this->currentAR, codeTableHash,
					currentCodeFKA, 3);

				arg_res.ref_ar = currentAR;
				(*currentAR).add(arg_res);
				try {
					Data& resData = (*currentAR)[arg_res];
					resData.content_ar = currentAR;
				}
				catch (Arg& referencePath_include_immed) {
#if debug
					{
						static int ignorecount = 0;
						cout << "Scanner::addArgsToCurrentAR()"
							<< " referencePath_include_immed arg:["
							<< referencePath_include_immed.toStrg() << "]\t"
							<< "\tignorecount:[" << ignorecount++ << "\t]("
							<< __FILE__ << ":" << __LINE__ << ":0" << ")"
							<< endl;
					}
#endif
				}
			}

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_QS) {
			auto&& arg_scop = currentCode[3];
			(*currentAR).add(arg_scop);
			(*currentAR)[arg_scop].dataFlagBit = S_AR;
			DataTablePtr ar = nullptr;
			ar = DataTablePtr::make(currentAR, currentAR, currentCode.scopeFKA);

			(*currentAR)[Arg(S_AR, currentCode.scopeFKA)].content_ar = ar;
			file_fka_ar_map[currentFileKey].insert(
				make_pair(currentCode.scopeFKA, ar));
			currentAR = (*currentAR)[Arg(S_AR, currentCode.scopeFKA)].content_ar;
			/*ar=nullptr;*/

		}
		else if (currentCode.assemblyFormula.flagBit.type == EXPRQS) {

			//create ar for inner variables to reasoning their types, won't change argFlag(EXPRZ_DZ) in codeTable
			auto&& arg_scop = currentCode[3];
			arg_scop.argFlag = Z_Dz;
			(*currentAR).add(arg_scop);
			(*currentAR)[Arg(S_AR, currentCode.scopeFKA)].dataFlagBit = S_AR;
			/*safe_delete((*currentAR)[currentCode.scopeFKA].content_ar);*/
			DataTablePtr ar = nullptr;
			ar = DataTablePtr::make(currentAR, currentAR, currentCode.scopeFKA);
			(*currentAR)[Arg(S_AR, currentCode.scopeFKA)].content_ar = ar;
			file_fka_ar_map[currentFileKey].insert(
				make_pair(currentCode.scopeFKA, ar));
			currentAR = (*currentAR)[Arg(S_AR, currentCode.scopeFKA)].content_ar;
			/*DELETE_AR(ar);*/

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_QE) {




			currentAR = currentAR->parentAR;
		}
		else if (currentCode.assemblyFormula.flagBit.type == EXPRQE) {
			//the same as COOL_QE
			currentAR = currentAR->parentAR;

			//(*currentAR)[currentCode[3]].dataFlagBit = S_AR;



		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_R
			&& currentCode.assemblyFormula.flagBit.execute == COOL_R) {

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_R
			&& currentCode.assemblyFormula.flagBit.execute == COOL_R) {

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_QS
			&& currentCode.assemblyFormula.flagBit.execute == COOL_R) {
			//scan步骤无法进入此步,预执行步骤可进入此步
			if ((*scopeTableHash)[FKA(currentFileKey, stackToAddr())].functionFlag
				== SYSB) {
				//预执行过程中的继承处理
				(*currentAR).add(currentCode[3]);
				(*currentAR)[Arg(S_AR, currentCode.scopeFKA)].dataFlagBit = S_AR;
				(*currentAR)[Arg(S_AR, currentCode.scopeFKA)].content_ar->scopeStructureFKA =
					currentCode.scopeFKA;
				/*safe_delete((*currentAR)[currentCode.scopeFKA].content_ar);*/

				DataTablePtr ar = nullptr;
				/*AS(ar,
					new DataTable(currentAR,
						(*currentAR)[(*scopeTableHash)[currentCode.scopeFKA].parentScopeFKA].content_ar,
						currentCode.scopeFKA));
				AS((*currentAR)[currentCode.scopeFKA].content_ar, ar);*/
				ar =
					DataTablePtr::make(currentAR,
						(*currentAR)[Arg(S_AR, (*scopeTableHash)[currentCode.scopeFKA].parentScopeFKA)].content_ar,
						currentCode.scopeFKA);
				(*currentAR)[Arg(S_AR, currentCode.scopeFKA)].content_ar = ar;
				file_fka_ar_map[currentFileKey].insert(
					make_pair(currentCode.scopeFKA, ar));
				currentAR = (*currentAR)[Arg(S_AR, currentCode.scopeFKA)].content_ar;

				System& currentSystem = (*systemTableHash)[currentCode.scopeFKA];
				//将继承类的预执行实例直接添加到querylist中完成继承,for class inherit, the child should keep a sharedptr for the parent in local, to avoid the release of parent.
				for (auto& fki : currentSystem.decedentSystemFKIList) {
					currentAR->queryARList.push_front(
						(*currentAR->parentAR)[Arg(S_AR, (*systemTableHash)[fki].systemFKA)].content_ar);
					currentAR->queryARList_local.push_front(
						(*currentAR->parentAR)[Arg(S_AR, (*systemTableHash)[fki].systemFKA)].content_ar);
				}
				//DELETE_AR(ar);

			}
			else {
				//预执行过程中的函数调用等处理
				(*currentAR).add(currentCode[3]);
				(*currentAR)[Arg(S_AR, currentCode.scopeFKA)].dataFlagBit = S_AR;
				(*currentAR)[Arg(S_AR, currentCode.scopeFKA)].content_ar->scopeStructureFKA =
					currentCode.scopeFKA;
				/*safe_delete((*currentAR)[currentCode.scopeFKA].content_ar);*/
				DataTablePtr queryAR_ = nullptr;
				if ((*scopeTableHash)[currentCode.scopeFKA].requireScopeFKA
					!= fkanull) {
					queryAR_ =
						(*currentAR)[Arg(S_AR, (*scopeTableHash)[currentCode.scopeFKA].requireScopeFKA)].content_ar;
				}
				else {
					queryAR_ = currentAR;
				}

				DataTablePtr ar = nullptr;
				ar = DataTablePtr::make(currentAR, queryAR_, currentCode.scopeFKA);
				(*currentAR)[Arg(S_AR, currentCode.scopeFKA)].content_ar = ar;
				file_fka_ar_map[currentFileKey].insert(
					make_pair(currentCode.scopeFKA, ar));
				currentAR = (*currentAR)[Arg(S_AR, currentCode.scopeFKA)].content_ar;
				/*DELETE_AR(ar);*/
			}

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_QE
			&& currentCode.assemblyFormula.flagBit.execute == COOL_R) {
			//scan步骤无法进入此步

			(*currentAR)[currentCode[3]].dataFlagBit = S_AR;

			currentAR = currentAR->parentAR;

		}
		else if (currentCode.assemblyFormula.flagBit.type == COOL_TD
			&& currentCode.assemblyFormula.flagBit.execute == T_) {
			//scan过程中不处理COOL_TD
		}
#if debug
		{
			static int ignorecount = 0;
			cout << "Scanner::addArgsToCurrentAR()" << " out " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif
		return true;
	}

	//const AssemblyCode Scanner::getNewAssemblyCode();
	Addr Scanner::stackToAddr() const {
		Addr a;
		for (int i = 0; i < (int)scopeStack.size(); i++) {
			a = link(a, scopeStack.at(i));

		}
		return a;
	}

	/*const FKA Scanner::getQSFKA(const FKA& fka) {
		return FKA(fka.fileKey, link(stackToAddr(), fka.addr));
	}*/


	/// <summary>
	/// used to get the FKA when a new Scope is about to create, that is, find code of type QS/EXPRQS.
	/// </summary>
	/// <param name="addr">int, the addr of currently scanning asc</param>
	/// <returns></returns>
	const FKA Scanner::getQSFKA(Intg addr) {
		return FKA(currentFileKey, link(stackToAddr(), addr));
	}

	const FKA Scanner::nextSameScopeTfFKA(const FKA& parentScopeFKA,
		const FKA& codeFKA) const {
		FKA a = templateFunctionTableHash->nextTfFKA(codeFKA);
		if (a != fkanull) {
			FKA b = (*scopeTableHash)[a].parentScopeFKA;
			if (b == parentScopeFKA) {
				return a;
			}
			else if ((*scopeTableHash)[a].scopeCoor2
		> (*scopeTableHash)[parentScopeFKA].scopeCoor2
				&& (*scopeTableHash)[parentScopeFKA].scopeCoor2 != addrnull) {
				return fkanull;

			}
			else {
				while ((*scopeTableHash)[b].parentScopeFKA != parentScopeFKA) {
					b = (*scopeTableHash)[b].parentScopeFKA;
				}
				return this->nextSameScopeTfFKA(parentScopeFKA,
					FKA((*scopeTableHash)[b].fileKey,
						(*scopeTableHash)[b].scopeCoor2));
			}

		}
		return a;
	}
	/**
	 * @brief 查询一个变量的属性
	 * @param varname
	 * @param scopeAddr
	 * @param out 是否从上层作用域开始检索
	 * @return 变量的属性，如果变量不存在，则返回-1；
	 */
	Intg Scanner::getAttribute(const Strg& varname, const ScopeFKA& scopeFKA,
		bool out) {
		FKA indexScopeFKA = scopeFKA;
		if (out == true) {
			if (scpfka__name_attr_map.find(indexScopeFKA)
				!= scpfka__name_attr_map.end()) {
				indexScopeFKA = (*scopeTableHash)[indexScopeFKA].parentScopeFKA;
			}
			else {
				return -1;
			}
		}
		do {
			auto&& it_name_attr_map = scpfka__name_attr_map.find(indexScopeFKA);
			if (it_name_attr_map == scpfka__name_attr_map.end()) {
				return -1;
			}
			else {
				auto&& it_attr = it_name_attr_map->second.find(varname);
				if (it_attr == it_name_attr_map->second.end()) {
					if (indexScopeFKA == fkanull) {
						return -1;
					}
					else if (scpfka__name_attr_map.find(indexScopeFKA)
						!= scpfka__name_attr_map.end()) {
						indexScopeFKA =
							(*scopeTableHash)[indexScopeFKA].parentScopeFKA;
					}
					else {
						return -1;
					}
				}
				else {
					return it_attr->second;
				}

			}

		} while (true);

	}

	Intg Scanner::getAttribute(const FKA& arg_fka, const ScopeFKA& scopeFKA,
		bool out) {

		FKA indexScopeFKA = scopeFKA;
		if (out == true) {
			if (scpfka__argfka_attr_map.find(indexScopeFKA)
				!= scpfka__argfka_attr_map.end()) {
				indexScopeFKA = (*scopeTableHash)[indexScopeFKA].parentScopeFKA;
			}
			else {
				return -1;
			}
		}
		do {
			auto&& it_fka_attr_map = scpfka__argfka_attr_map.find(indexScopeFKA);
			if (it_fka_attr_map == scpfka__argfka_attr_map.end()) {
				return -1;
			}
			else {
				auto&& it_attr = it_fka_attr_map->second.find(arg_fka);
				if (it_attr == it_fka_attr_map->second.end()) {
					if (indexScopeFKA == fkanull) {
						return -1;
					}
					else if (scpfka__argfka_attr_map.find(indexScopeFKA)
						!= scpfka__argfka_attr_map.end()) {
						indexScopeFKA =
							(*scopeTableHash)[indexScopeFKA].parentScopeFKA;
					}
					else {
						return -1;
					}
				}
				else {
					return it_attr->second;
				}

			}

		} while (true);

	}
	/**
	 * @brief 查询一个变量的信息，其储存在一个arg中
	 * @param varname
	 * @param scopeAddr
	 * @param out 是否从上层作用域开始检索
	 * @return 一个变量，其储存了变量所在scope（asc）、属性等，如果变量不存在，则返回argnull；
	 */
	Arg Scanner::getArg(const Strg& varname, const ScopeFKA& scopeFKA, bool out) {
		FKA indexScopeFKA = scopeFKA;
		if (out == true) {
			if (scpfka__name_attr_map.find(indexScopeFKA)
				!= scpfka__name_attr_map.end()) {
				indexScopeFKA = (*scopeTableHash)[indexScopeFKA].parentScopeFKA;
			}
			else {
				return argnull;
			}
		}
		do {
			auto&& it_name_attr_map = scpfka__name_attr_map.find(indexScopeFKA);
			if (it_name_attr_map == scpfka__name_attr_map.end()) {
				return argnull;
			}
			else {
				auto&& it_attr = it_name_attr_map->second.find(varname);
				if (it_attr == it_name_attr_map->second.end()) {
					if (indexScopeFKA == fkanull) {
						return argnull;
					}
					else if (scpfka__name_attr_map.find(indexScopeFKA)
						!= scpfka__name_attr_map.end()) {
						indexScopeFKA =
							(*scopeTableHash)[indexScopeFKA].parentScopeFKA;
					}
					else {
						return argnull;
					}
				}
				else {
					Arg argInfo;
					argInfo.argFlag = it_attr->second;
					argInfo.arg_s = it_attr->first;
					argInfo.asc = indexScopeFKA;
					return argInfo;
				}

			}

		} while (true);

	}
	Arg Scanner::getArg(const FKA& arg_fka, const ScopeFKA& scopeFKA, bool out) {

		FKA indexScopeFKA = scopeFKA;
		if (out == true) {
			if (scpfka__argfka_attr_map.find(indexScopeFKA)
				!= scpfka__argfka_attr_map.end()) {
				indexScopeFKA = (*scopeTableHash)[indexScopeFKA].parentScopeFKA;
			}
			else {
				return argnull;
			}
		}
		do {
			auto&& it_addr_attr_map = scpfka__argfka_attr_map.find(indexScopeFKA);
			if (it_addr_attr_map == scpfka__argfka_attr_map.end()) {
				return argnull;
			}
			else {
				auto&& it_attr = it_addr_attr_map->second.find(arg_fka);
				if (it_attr == it_addr_attr_map->second.end()) {
					if (indexScopeFKA == fkanull) {
						return argnull;
					}
					else if (scpfka__argfka_attr_map.find(indexScopeFKA)
						!= scpfka__argfka_attr_map.end()) {
						indexScopeFKA =
							(*scopeTableHash)[indexScopeFKA].parentScopeFKA;
					}
					else {
						return argnull;
					}
				}
				else {
					Arg argInfo;
					argInfo.argFlag = it_attr->second;
					argInfo.arg_fka = it_attr->first;
					argInfo.asc = indexScopeFKA;
					return argInfo;
				}

			}

		} while (true);

	}
	/**
	 * @brief 根据之前的变量重置一个变量的属性，若这个变量不存在且out为假，
	 * 则在scopeFKA中创建一个新的变量
	 * @param varname
	 * @param scopeAddr 变量所在作用域
	 * @param out 是否从上层作用域开始搜索
	 * @return
	 */
	void Scanner::resetAttribute(const Strg& varname, const ScopeFKA& scopeFKA,
		const Intg& attr, bool out) {
#if debug
			{
				static int ignorecount = 0;
				cout
					<< "resetAttribute(const Strg &varname, const ScopeFKA &scopeFKA,const Intg &attr, bool out)"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
			bool varNotExist = false;
			FKA indexScopeFKA = scopeFKA;
			if (out == true) {
				if (scpfka__name_attr_map.find(indexScopeFKA)
					!= scpfka__name_attr_map.end()) {
					indexScopeFKA = (*scopeTableHash)[indexScopeFKA].parentScopeFKA;
				}
				else {
					varNotExist = true;
				}
			}
			do {
				auto&& it_name_attr_map = scpfka__name_attr_map.find(indexScopeFKA);
				if (it_name_attr_map == scpfka__name_attr_map.end()) {
					varNotExist = true;
					break;
				}
				else {
					auto&& it_attr = it_name_attr_map->second.find(varname);
					if (it_attr == it_name_attr_map->second.end()) {
						if (indexScopeFKA == fkanull) {
							varNotExist = true;
							break;
						}
						else if (scpfka__name_attr_map.find(indexScopeFKA)
							!= scpfka__name_attr_map.end()) {
							indexScopeFKA =
								(*scopeTableHash)[indexScopeFKA].parentScopeFKA;
						}
						else {
							varNotExist = true;
							break;
						}
					}
					else {
						scpfka__name_attr_map[indexScopeFKA][varname] = attr;
						return;
					}

				}

			} while (true);
			if (out == false && varNotExist == true) {
				scpfka__name_attr_map[scopeFKA][varname] = attr;
			}

	}
	void Scanner::resetAttribute(const FKA& varFKA, const ScopeFKA& scopeFKA,
		const Intg& attr, bool out) {
#if debug
			{
				static int ignorecount = 0;
				cout
					<< "resetAttribute(const Addr &varFKA, const ScopeFKA &scopeFKA, const Intg &attr, bool out)"
					<< " in " << "varFKA:[" << varFKA.toStrg() << "]\tscopeAddr:["
					<< scopeFKA.toStrg() << "]\tattr:[" << attr << "]\tout:[" << out
					<< "]" << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif

			bool varNotExist = false;
			FKA indexScopeFKA = scopeFKA;
			if (out == true) {
				if (scpfka__argfka_attr_map.find(indexScopeFKA)
					!= scpfka__argfka_attr_map.end()) {
					indexScopeFKA = (*scopeTableHash)[indexScopeFKA].parentScopeFKA;
				}
				else {
					varNotExist = true;
				}
			}
			do {
				auto&& it_addr_attr_map = scpfka__argfka_attr_map.find(indexScopeFKA);
				if (it_addr_attr_map == scpfka__argfka_attr_map.end()) {
					varNotExist = true;
					break;
				}
				else {
					auto&& it_attr = it_addr_attr_map->second.find(varFKA);
					if (it_attr == it_addr_attr_map->second.end()) {
						if (indexScopeFKA == fkanull) {
							varNotExist = true;
							break;
						}
						else if (indexScopeFKA != fkanull
							&& scpfka__argfka_attr_map.find(indexScopeFKA)
							!= scpfka__argfka_attr_map.end()) {
							indexScopeFKA =
								(*scopeTableHash)[indexScopeFKA].parentScopeFKA;
						}
						else {
							varNotExist = true;
							break;
						}
					}
					else {
						scpfka__argfka_attr_map[indexScopeFKA][varFKA] = attr;
						return;
					}

				}

			} while (true);
			if (out == false && varNotExist == true) {
				scpfka__argfka_attr_map[scopeFKA][varFKA] = attr;
			}

	}
	/**
	 * @brief 向作用域中添加一个变量属性的索引
	 * @param varname
	 * @param scopeAddr
	 */
	void Scanner::addAttribute(const Strg& varname, const ScopeFKA& scopeFKA,
		const Intg& attr) {
		scpfka__name_attr_map[scopeFKA][varname] = attr;
	}
	void Scanner::addAttribute(const FKA& arg_fka, const ScopeFKA& scopeFKA,
		const Intg& attr) {
		scpfka__argfka_attr_map[scopeFKA][arg_fka] = attr;
	}
	//const Addr Scanner::getScopeAddr(const Addr& addr) const;
	//const Addr Scanner::getScopeAddr(Intg addr) const;
	/**
	 * @attention 在这个函数中，所有域地址（Z_Dz）、函数地址都会被转化为绝对地址，add constraints, changeable, compatible args to related map in Scanner.
	 * 所有变量地址（S_Dz）都仍为相对地址
	 * @param asc
	 * @param asf
	 * @return
	 */
	bool Scanner::setQuaternionAndFlagBit(const AssemblyCode& asc,
		AssemblyFormula* asf) {
		asf->flagBit.type = asc.type;

		///handle qs
		if (asc.type == COOL_QS) {
			asf->flagBit.result_flag = Z_Dz;
			asf->quaternion.result_fka = FKA(currentFileKey, stackToAddr());
		}
		else if (asc.type == EXPRQS) {
			asf->flagBit.result_flag = EXPRZ_Dz;
			asf->quaternion.result_fka = FKA(currentFileKey, stackToAddr());
		}

		//// handle attributes
		else if (asc.type == COOL_C && asc.arg2Flag == Bs && asc.arg2_s == "new") {
			switch (asc.arg1Flag) {
			case Dz:
				asf->flagBit.arg1_flag = S_Dz;
				asf->quaternion.arg1_fka = FKA(currentFileKey, asc.arg1_i == 0 ? addrnull : Addr(asc.arg1_i));
				break;
			case Sz:
				asf->flagBit.arg1_flag = Sz;
				asf->quaternion.arg1_i = asc.arg1_i;
				break;
			case Bs:
				asf->flagBit.arg1_flag = S_Bs;
				asf->quaternion.arg1_s = asc.arg1_s;
				break;
			case Zf:
				asf->flagBit.arg1_flag = Zf;
				asf->quaternion.arg1_s = asc.arg1_s;
				break;

			default:
				break;
			}
			switch (asc.arg2Flag) {
			case Dz:
				asf->flagBit.arg2_flag = S_Dz;
				asf->quaternion.arg2_fka = FKA(currentFileKey, asc.arg2_i == 0 ? addrnull : Addr(asc.arg2_i));
				break;
			case Sz:
				asf->flagBit.arg2_flag = Sz;
				asf->quaternion.arg2_i = asc.arg2_i;
				break;
			case Bs:
				asf->flagBit.arg2_flag = X_Bs;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;
			case Zf:
				asf->flagBit.arg2_flag = Zf;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;

			default:
				break;
			}
			asf->quaternion.operator_s = asc.operator_s;
			asf->flagBit.operator_flag = M_Bs;
			switch (asc.resultFlag) {
			case Dz:
				asf->flagBit.result_flag = S_Dz;
				asf->quaternion.result_fka = FKA(currentFileKey,
					asc.result_i == 0 ? addrnull : Addr(asc.result_i));
				break;
			case Sz:
				asf->flagBit.result_flag = Sz;
				asf->quaternion.result_i = asc.result_i;
				break;
			case Bs:
				asf->flagBit.result_flag = S_Bs;
				asf->quaternion.result_s = asc.result_s;
				break;
			case Zf:
				asf->flagBit.result_flag = Zf;
				asf->quaternion.result_s = asc.result_s;
				break;

			default:
				break;
			}
			if (asc.arg1_s != asc.result_s) {
#if debug
				cerr << "COOL_C (a1,x,x,result) err, should be a1==result" << endl;
#endif
			}
		}
		else if ((asc.type == COOL_C || asc.type == COOL_M) && asc.operatorFlag == Bs && asc.operator_s == ":"
			&& (asc.arg2Flag == Bs || asc.arg2Flag == Dz) && asc.arg2_s != "new") {



			switch (asc.arg1Flag) {
			case Dz:
				asf->flagBit.arg1_flag = S_Dz;
				asf->quaternion.arg1_fka = FKA(currentFileKey, asc.arg1_i == 0 ? addrnull : Addr(asc.arg1_i));
				break;
			case Sz:
				asf->flagBit.arg1_flag = Sz;
				asf->quaternion.arg1_i = asc.arg1_i;
				break;
			case Bs:
				asf->flagBit.arg1_flag = S_Bs;
				asf->quaternion.arg1_s = asc.arg1_s;
				break;
			case Zf:

				asf->flagBit.arg1_flag = Zf;
				asf->quaternion.arg1_s = asc.arg1_s;
				break;

			default:
				break;
			}
			switch (asc.arg2Flag) {
			case Dz:
				asf->flagBit.arg2_flag = X_Dz;
				asf->quaternion.arg2_fka = FKA(currentFileKey, asc.arg2_i == 0 ? addrnull : Addr(asc.arg2_i));
				break;
			case Sz:
				asf->flagBit.arg2_flag = Sz;
				asf->quaternion.arg2_i = asc.arg2_i;
				break;
			case Bs:
				asf->flagBit.arg2_flag = X_Bs;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;
			case Zf:
				asf->flagBit.arg2_flag = Zf;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;

			default:
				break;
			}
			asf->quaternion.operator_s = asc.operator_s;
			asf->flagBit.operator_flag = M_Bs;
			switch (asc.resultFlag) {
			case Dz:
				asf->flagBit.result_flag = S_Dz;
				asf->quaternion.result_fka = FKA(currentFileKey,
					asc.result_i == 0 ? addrnull : Addr(asc.result_i));
				break;
			case Sz:
				asf->flagBit.result_flag = Sz;
				asf->quaternion.result_i = asc.result_i;
				break;
			case Bs:
				asf->flagBit.result_flag = S_Bs;
				asf->quaternion.result_s = asc.result_s;
				break;
			case Zf:
				asf->flagBit.result_flag = Zf;
				asf->quaternion.result_s = asc.result_s;
				break;

			default:
				break;
			}
			/*Intg attr = -1;
			if (asc.arg1Flag == Dz
				&& (attr = getAttribute(asf->quaternion.arg1_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg1_flag = attr;

			}
			else if (asc.arg1Flag == Bs
				&& (attr = getAttribute(asf->quaternion.arg1_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg1_flag = attr;

			}
			if (asc.arg2Flag == Dz
				&& (attr = getAttribute(asf->quaternion.arg2_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg2_flag = attr;

			}
			else if (asc.arg2Flag == Bs
				&& (attr = getAttribute(asf->quaternion.arg2_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg2_flag = attr;

			}
			if (asc.resultFlag == Dz
				&& (attr = getAttribute(asf->quaternion.result_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.result_flag = attr;

			}
			else if (asc.resultFlag == Bs
				&& (attr = getAttribute(asf->quaternion.result_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.result_flag = attr;

			}*/
			if (!(asc.arg1Flag == asc.resultFlag && asc.arg1_i == asc.result_i
				&& asc.arg1_s != asc.result_s)) {
#if debug
				cerr << "COOL_M (arg1,out,:,result) err,should be arg1==result" << endl;
#endif
			}
			if (asc.arg2_s == "out") {
				asf->flagBit.execute = Du;
				realM.push_back(currentCodeFKA);
			}
			else {
				constraintM.push_back(currentCodeFKA);
			}


		}
		//else if (

		//	((asc.type == COOL_C || asc.type == COOL_M) && (asc.operatorFlag == Bs && asc.operator_s == ":"))
		//	&&
		//	(
		//		!(asc.arg2Flag == Bs && asc.arg2_s == "new")
		//		)
		//	) {
		//	asf->flagBit.arg1_flag = S_Bs;
		//	asf->quaternion.arg1_s = asc.arg1_s;
		//	if (asc.arg2Flag == Bs
		//		) {
		//		asf->flagBit.arg2_flag = X_Bs;
		//		asf->quaternion.arg2_s = asc.arg2_s;
		//	}
		//	else if (asc.arg2Flag == Dz) {
		//		asf->flagBit.arg2_flag = X_Dz;
		//		asf->quaternion.arg2_fka = FKA(currentFileKey, asc.arg2_i==0?addrnull:Addr(asc.arg2_i));;
		//	}
		//	asf->flagBit.operator_flag = M_Bs;
		//	asf->quaternion.operator_s = asc.operator_s;
		//	asf->flagBit.result_flag = S_Bs;
		//	asf->quaternion.result_s = asc.result_s;
		//	if (asc.operator_s == ":" && (asc.arg2_s != "symbol" && asc.arg2_s != "placeholder")) {
		//		asf->flagBit.compatible[0] = T_;
		//	}

		//	constraintM.push_back(currentCodeFKA);

		//}
		else if (asc.type == COOL_M && asc.operatorFlag == Bs
			&& ((asc.operator_s == "$"
				|| (asc.arg2Flag == Bs && asc.operator_s == ":"
					&& asc.arg2_s == "$"))
				|| (asc.operator_s == "#"
					|| (asc.arg2Flag == Bs && asc.operator_s == ":"
						&& asc.arg2_s == "#"))
				|| asc.operator_s == "?")) {

			switch (asc.arg1Flag) {
			case Dz:
				asf->flagBit.arg1_flag = S_Dz;
				asf->quaternion.arg1_fka = FKA(currentFileKey, asc.arg1_i == 0 ? addrnull : Addr(asc.arg1_i));
				break;
			case Sz:
#if debug
				cerr << "COOL_M (Sz,x,$#,x) err,should be Dz" << endl;
#endif
				asf->flagBit.arg1_flag = Sz;
				asf->quaternion.arg1_i = asc.arg1_i;
				break;
			case Bs:
				asf->flagBit.arg1_flag = S_Bs;
				asf->quaternion.arg1_s = asc.arg1_s;
				break;
			case Zf:
#if debug
				cerr << "COOL_M (Zf,x,$#,x) err,should be Dz/Bs" << endl;
#endif
				asf->flagBit.arg1_flag = Zf;
				asf->quaternion.arg1_s = asc.arg1_s;
				break;

			default:
				break;
			}
			switch (asc.arg2Flag) {
			case Dz:
				asf->flagBit.arg2_flag = X_Dz;
				asf->quaternion.arg2_fka = FKA(currentFileKey, asc.arg2_i == 0 ? addrnull : Addr(asc.arg2_i));
				break;
			case Sz:
				asf->flagBit.arg2_flag = Sz;
				asf->quaternion.arg2_i = asc.arg2_i;
				break;
			case Bs:
				asf->flagBit.arg2_flag = X_Bs;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;
			case Zf:
				asf->flagBit.arg2_flag = Zf;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;

			default:
				break;
			}
			asf->quaternion.operator_s = asc.operator_s;
			asf->flagBit.operator_flag = M_Bs;
			switch (asc.resultFlag) {
			case Dz:
				asf->flagBit.result_flag = S_Dz;
				asf->quaternion.result_fka = FKA(currentFileKey,
					asc.result_i == 0 ? addrnull : Addr(asc.result_i));
				break;
			case Sz:
#if debug
				cerr << "COOL_M (x,x,$#,Sz) err,should be Dz" << endl;
#endif
				asf->flagBit.result_flag = Sz;
				asf->quaternion.result_i = asc.result_i;
				break;
			case Bs:
				asf->flagBit.result_flag = S_Bs;
				asf->quaternion.result_s = asc.result_s;
				break;
			case Zf:
#if debug
				cerr << "COOL_M (x,x,$#,Zf) err,should be Dz/Bs" << endl;
#endif
				asf->flagBit.result_flag = Zf;
				asf->quaternion.result_s = asc.result_s;
				break;

			default:
				break;
			}
			/*Intg attr = -1;
			if (asc.arg1Flag == Dz
				&& (attr = getAttribute(asf->quaternion.arg1_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg1_flag = attr;

			}
			else if (asc.arg1Flag == Bs
				&& (attr = getAttribute(asf->quaternion.arg1_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg1_flag = attr;

			}
			if (asc.arg2Flag == Dz
				&& (attr = getAttribute(asf->quaternion.arg2_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg2_flag = attr;

			}
			else if (asc.arg2Flag == Bs
				&& (attr = getAttribute(asf->quaternion.arg2_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg2_flag = attr;

			}
			if (asc.resultFlag == Dz
				&& (attr = getAttribute(asf->quaternion.result_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.result_flag = attr;

			}
			else if (asc.resultFlag == Bs
				&& (attr = getAttribute(asf->quaternion.result_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.result_flag = attr;

			}*/
			if (!(asc.arg1Flag == asc.resultFlag && asc.arg1_i == asc.result_i
				&& asc.arg1_s == asc.result_s)) {
#if debug
				cerr << "COOL_M (arg1,x,$#,result) err,should be arg1==result" << endl;
#endif
			}
			if (asc.operator_s == "$"
				|| (asc.arg2Flag == Bs && asc.operator_s == ":"
					&& asc.arg2_s == "$")) {
				asf->flagBit.changeable[0] = T_;
				/*            asf->flagBit.changeable[3] = T_;
				 * 虽然此步可以确定achangeable3的可变性，但由于pendpainter会跳过可变性完全确定的代码，因此不能在此处直接给changeable3赋值
				 *
				 * */
				pendM.push_back(currentCodeFKA);
			}
			if (asc.operator_s == "#"
				|| (asc.arg2Flag == Bs && asc.operator_s == ":"
					&& asc.arg2_s == "#")) {
				asf->flagBit.changeable[0] = A_;
				/*            asf->flagBit.changeable[3] = A_;
				 * 虽然此步可以确定achangeable3的可变性，但由于pendpainter会跳过可变性完全确定的代码，因此不能在此处直接给changeable3赋值
				 * */
				pendM.push_back(currentCodeFKA);
			}


			if (asc.operator_s == "?") {
				asf->flagBit.compatible[0] = T_;
				compatibleM.push_back(currentCodeFKA);
			}



		}

		/////handle basic built-in operators.
		else if (asc.type == COOL_R
			|| (asc.type == COOL_M && asc.operatorFlag == Bs
				&& (attributeRelatedOps.count(asc.operator_s) == 0  /*attribution operations are not alowed here
				 || asc.operator_s == "-"
				 || asc.operator_s == "*" || asc.operator_s == "/"
				 || asc.operator_s == "^" || asc.operator_s == "=="
				 || asc.operator_s == "=" || asc.operator_s == "-->"
				 || asc.operator_s == ">" || asc.operator_s == "<"
				 || asc.operator_s == ">=" || asc.operator_s == "<="
				 || asc.operator_s == "+="||asc.operator_s == "-="
				 || asc.operator_s == "*="||asc.operator_s == "/="
				 || asc.operator_s == "^="||asc.operator_s == "||"
				 || asc.operator_s == "!"||asc.operator_s == "&&"
				 || asc.operator_s == "COMMA"*/)
				)) {
			switch (asc.arg1Flag) {
			case Dz:
				asf->flagBit.arg1_flag = S_Dz;
				asf->quaternion.arg1_fka = FKA(currentFileKey, asc.arg1_i == 0 ? addrnull : Addr(asc.arg1_i));
				break;
			case Sz:
				asf->flagBit.arg1_flag = Sz;
				asf->quaternion.arg1_i = asc.arg1_i;
				break;
			case Bs:
				asf->flagBit.arg1_flag = S_Bs;
				asf->quaternion.arg1_s = asc.arg1_s;
				break;
			case Zf:
				asf->flagBit.arg1_flag = Zf;
				asf->quaternion.arg1_s = asc.arg1_s;
				break;

			default:
				break;
			}
			switch (asc.arg2Flag) {
			case Dz:
				asf->flagBit.arg2_flag = S_Dz;
				asf->quaternion.arg2_fka = FKA(currentFileKey, asc.arg2_i == 0 ? addrnull : Addr(asc.arg2_i));
				break;
			case Sz:
				asf->flagBit.arg2_flag = Sz;
				asf->quaternion.arg2_i = asc.arg2_i;
				break;
			case Bs:
				asf->flagBit.arg2_flag = S_Bs;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;
			case Zf:
				asf->flagBit.arg2_flag = Zf;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;

			default:
				break;
			}
			asf->quaternion.operator_s = asc.operator_s;
			asf->flagBit.operator_flag = M_Bs;
			switch (asc.resultFlag) {
			case Dz:
				asf->flagBit.result_flag = S_Dz;
				asf->quaternion.result_fka = FKA(currentFileKey,
					asc.result_i == 0 ? addrnull : Addr(asc.result_i));
				break;
			case Sz:
				asf->flagBit.result_flag = Sz;
				asf->quaternion.result_i = asc.result_i;
				break;
			case Bs:
				asf->flagBit.result_flag = S_Bs;
				asf->quaternion.result_s = asc.result_s;
				break;
			case Zf:
				asf->flagBit.result_flag = Zf;
				asf->quaternion.result_s = asc.result_s;
				break;

			default:
				break;
			}

			/*Intg attr = -1;
			if (asc.arg1Flag == Dz
				&& (attr = getAttribute(asf->quaternion.arg1_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg1_flag = attr;

			}
			else if (asc.arg1Flag == Bs
				&& (attr = getAttribute(asf->quaternion.arg1_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg1_flag = attr;

			}
			if (asc.arg2Flag == Dz
				&& (attr = getAttribute(asf->quaternion.arg2_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg2_flag = attr;

			}
			else if (asc.arg2Flag == Bs
				&& (attr = getAttribute(asf->quaternion.arg2_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg2_flag = attr;

			}
			if (asc.resultFlag == Dz
				&& (attr = getAttribute(asf->quaternion.result_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.result_flag = attr;

			}
			else if (asc.resultFlag == Bs
				&& (attr = getAttribute(asf->quaternion.result_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.result_flag = attr;

			}*/

		}




		else if (asc.type == COOL_Y && asc.operatorFlag == Bs
			&& asc.operator_s == ".") {
#if debug
				{
					static int ignorecount = 0;
					cout
						<< "Scanner::setQuaternionAndFlagBit(const AssemblyCode&, AssemblyFormula*)"
						<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
				}
#endif
				asf->quaternion.result_fka = FKA(currentFileKey, asc.result_i == 0 ? addrnull : Addr(asc.result_i));
				asf->flagBit.result_flag = Y_Dz;
				asf->quaternion.operator_s = asc.operator_s;
				asf->flagBit.operator_flag = M_Bs;
				//Intg attr = -1;
				//FKA arg2scopeFKA;
				//if (asc.arg1Flag == Dz
				//	&& (attr = getAttribute(asf->quaternion.arg1_fka,
				//		FKA(currentFileKey, stackToAddr()), false)) != -1) {
				//	asf->flagBit.arg1_flag = attr;
				//	asf->quaternion.arg1_fka = FKA(currentFileKey, asc.arg1_i == 0 ? addrnull : Addr(asc.arg1_i));
				//	if (asf->flagBit.arg1_flag == Y_Dz) {
				//		arg2scopeFKA = globalRefTable[Arg(Y_Dz,
				//			asf->quaternion.arg1_fka, currentAR)].asc;

				//	}
				//	else {
				//		arg2scopeFKA = (*currentAR).getArgScopeFKA(
				//			Arg(S_Dz, asf->quaternion.arg1_fka));
				//	}

				//}
				//else if (asc.arg1Flag == Bs) {
				//	asf->flagBit.arg1_flag = S_Bs;
				//	asf->quaternion.arg1_s = asc.arg1_s;
				//	/*
				//	 * 以下代码无效，尝试通过函数名字检索函数是徒劳的，因为函数以S_AR形式储存在
				//	 * AR中，意味着AR中是没有函数名字的键的
				//	 DataTable *arg1AR = (*currentAR)[asf->quaternion.arg1_s].content_ar;
				//	 arg2scopeAddr = arg1AR->scopeStructureAddr;
				//	 */
				//}

				//if (asc.arg2Flag == Dz) {
				//	asf->flagBit.arg2_flag = S_Dz;
				//	asf->quaternion.arg2_fka = FKA(currentFileKey, asc.arg2_i == 0 ? addrnull : Addr(asc.arg2_i));

				//}
				//else if (asc.arg2Flag == Bs) {
				//	asf->flagBit.arg2_flag = S_Bs;
				//	asf->quaternion.arg2_s = asc.arg2_s;
				//}
				//else if (asc.arg2Flag == Zf) {
				//	asf->flagBit.arg2_flag = Zf;
				//	asf->quaternion.arg2_s = asc.arg2_s;
				//}

		}
		else if (asc.type == COOL_LST && asc.operatorFlag == Bs) {
#if debug
			{
				static int ignorecount = 0;
				cout
					<< "Scanner::setQuaternionAndFlagBit(const AssemblyCode&, AssemblyFormula*)"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
			if (asc.arg1Flag == Dz) {
				asf->flagBit.arg1_flag = S_Dz;
				asf->quaternion.arg1_fka = FKA(currentFileKey, asc.arg1_i == 0 ? addrnull : Addr(asc.arg1_i));

			}
			else if (asc.arg1Flag == Bs) {
				asf->flagBit.arg1_flag = S_Bs;
				asf->quaternion.arg1_s = asc.arg1_s;
			}
			else if (asc.arg1Flag == Zf) {
				asf->flagBit.arg1_flag = Zf;
				asf->quaternion.arg1_s = asc.arg1_s;
			}
			else if (asc.arg1Flag == Sz) {
				asf->flagBit.arg1_flag = Sz;
				asf->quaternion.arg1_i = asc.arg1_i;
			}

			if (asc.arg2Flag == Dz) {
				asf->flagBit.arg2_flag = S_Dz;
				asf->quaternion.arg2_fka = FKA(currentFileKey, asc.arg2_i == 0 ? addrnull : Addr(asc.arg2_i));
			}
			else if (asc.arg2Flag == Bs) {
				asf->flagBit.arg2_flag = S_Bs;
				asf->quaternion.arg2_s = asc.arg2_s;
			}
			else if (asc.arg2Flag == Zf) {
				asf->flagBit.arg2_flag = Zf;
				asf->quaternion.arg2_s = asc.arg2_s;
			}
			else if (asc.arg2Flag == Sz) {
				asf->flagBit.arg2_flag = Sz;
				asf->quaternion.arg2_i = asc.arg2_i;
			}
			asf->quaternion.operator_s = asc.operator_s;
			asf->flagBit.operator_flag = M_Bs;
			if (asc.resultFlag == Dz) {
				asf->flagBit.result_flag = S_Dz;
				asf->quaternion.result_fka = FKA(currentFileKey,
					asc.result_i == 0 ? addrnull : Addr(asc.result_i));
			}
			else if (asc.resultFlag == Bs) {
				asf->flagBit.result_flag = S_Bs;
				asf->quaternion.result_s = asc.result_s;
			}
			else if (asc.resultFlag == Zf) {
				asf->flagBit.result_flag = Zf;
				asf->quaternion.result_s = asc.result_s;
			}
			else if (asc.resultFlag == Sz) {
				asf->flagBit.result_flag = Sz;
				asf->quaternion.result_i = asc.result_i;
			}

		}



		else if (asc.type == COOL_M && asc.operatorFlag == Bs
			&& (asc.operator_s == "[]" || asc.operator_s == "ca"
				|| asc.operator_s == "call" || asc.operator_s == "attr_call")) {
			switch (asc.arg1Flag) {
			case Dz:
				asf->flagBit.arg1_flag = S_Dz;
				asf->quaternion.arg1_fka.addr = asc.arg1_i;
				asf->quaternion.arg1_fka.fileKey = currentFileKey;
				break;
			case Sz:
				asf->flagBit.arg1_flag = Sz;
				asf->quaternion.arg1_i = asc.arg1_i;
				break;
			case Bs:
				asf->flagBit.arg1_flag = S_Bs;
				asf->quaternion.arg1_s = asc.arg1_s;
				break;
			case Zf:
				asf->flagBit.arg1_flag = Zf;
				asf->quaternion.arg1_s = asc.arg1_s;
				break;

			default:
				break;
			}
			switch (asc.arg2Flag) {
			case Dz:
				asf->flagBit.arg2_flag = S_Dz;
				asf->quaternion.arg2_fka.addr = asc.arg2_i;
				asf->quaternion.arg2_fka.fileKey = currentFileKey;
				break;
			case Sz:
				asf->flagBit.arg2_flag = Sz;
				asf->quaternion.arg2_i = asc.arg2_i;
				break;
			case Bs:
				asf->flagBit.arg2_flag = S_Bs;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;
			case Zf:
				asf->flagBit.arg2_flag = Zf;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;

			default:
				break;
			}
			asf->quaternion.operator_s = asc.operator_s;
			asf->flagBit.operator_flag = M_Bs;
			switch (asc.resultFlag) {
			case Dz:
				asf->flagBit.result_flag = S_Dz;
				asf->quaternion.result_fka.addr = asc.result_i;
				asf->quaternion.result_fka.fileKey = currentFileKey;
				break;
			case Sz:
				asf->flagBit.result_flag = Sz;
				asf->quaternion.result_i = asc.result_i;
				break;
			case Bs:
				asf->flagBit.result_flag = S_Bs;
				asf->quaternion.result_s = asc.result_s;
				break;
			case Zf:
				asf->flagBit.result_flag = Zf;
				asf->quaternion.result_s = asc.result_s;
				break;

			default:
				break;
			}
			/*Intg attr = -1;
			if (asc.arg1Flag == Dz
				&& (attr = getAttribute(asf->quaternion.arg1_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg1_flag = attr;

			}
			else if (asc.arg1Flag == Bs
				&& (attr = getAttribute(asf->quaternion.arg1_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg1_flag = attr;

			}
			if (asc.arg2Flag == Dz
				&& (attr = getAttribute(asf->quaternion.arg2_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg2_flag = attr;

			}
			else if (asc.arg2Flag == Bs
				&& (attr = getAttribute(asf->quaternion.arg2_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg2_flag = attr;

			}
			if (asc.resultFlag == Dz
				&& (attr = getAttribute(asf->quaternion.result_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.result_flag = attr;

			}
			else if (asc.resultFlag == Bs
				&& (attr = getAttribute(asf->quaternion.result_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.result_flag = attr;

			}*/
		}
		else if (asc.type == COOL_M && asc.operatorFlag == Bs
			/*&& (asc.operator_s == "LENGTH" || asc.operator_s == "CLEAR"
			 || asc.operator_s == "ERASE" || asc.operator_s == "TYPENAME"
			 || asc.operator_s == "INSERT" || asc.operator_s == "COUNT"
			 || asc.operator_s == "FIND"

			 || asc.operator_s == "TONUM" || asc.operator_s == "TOSTRG"
			 || asc.operator_s == "TOINT"

			 || asc.operator_s == "NONBLOCKEXECUTE"
			 || asc.operator_s == "BLOCKEXECUTE"
			 || asc.operator_s == "SLEEP"

			 || asc.operator_s == "LIST" || asc.operator_s == "SET"
			 || asc.operator_s == "MULTISET" || asc.operator_s == "MAP"
			 || asc.operator_s == "MULTIMAP"

			 || asc.operator_s == "PUSHBACK"
			 || asc.operator_s == "PUSHFRONT"
			 || asc.operator_s == "POPBACK"
			 || asc.operator_s == "POPFRONT" || asc.operator_s == "BACK")*/
			) {
			switch (asc.arg1Flag) {
			case Dz:
				asf->flagBit.arg1_flag = S_Dz;
				if (asc.arg1_i != 0) {
					asf->quaternion.arg1_fka.addr = asc.arg1_i;
					asf->quaternion.arg1_fka.fileKey = currentFileKey;
				}
				break;
			case Sz:
				asf->flagBit.arg1_flag = Sz;
				asf->quaternion.arg1_i = asc.arg1_i;
				break;
			case Bs:
				asf->flagBit.arg1_flag = S_Bs;
				asf->quaternion.arg1_s = asc.arg1_s;
				break;
			case Zf:
				asf->flagBit.arg1_flag = Zf;
				asf->quaternion.arg1_s = asc.arg1_s;
				break;

			default:
				break;
			}
			switch (asc.arg2Flag) {
			case Dz:
				asf->flagBit.arg2_flag = S_Dz;
				if (asc.arg2_i != 0) {
					asf->quaternion.arg2_fka.addr = asc.arg2_i;
					asf->quaternion.arg2_fka.fileKey = currentFileKey;
				}
				break;
			case Sz:
				asf->flagBit.arg2_flag = Sz;
				asf->quaternion.arg2_i = asc.arg2_i;
				break;
			case Bs:
				asf->flagBit.arg2_flag = S_Bs;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;
			case Zf:
				asf->flagBit.arg2_flag = Zf;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;

			default:
				break;
			}
			asf->quaternion.operator_s = asc.operator_s;
			asf->flagBit.operator_flag = M_Bs;
			switch (asc.resultFlag) {
			case Dz:
				asf->flagBit.result_flag = S_Dz;
				if (asc.result_i != 0) {
					asf->quaternion.result_fka.addr = asc.result_i;
					asf->quaternion.result_fka.fileKey = currentFileKey;
				}
				break;
			case Sz:
				asf->flagBit.result_flag = Sz;
				asf->quaternion.result_i = asc.result_i;
				break;
			case Bs:
				asf->flagBit.result_flag = S_Bs;
				asf->quaternion.result_s = asc.result_s;
				break;
			case Zf:
				asf->flagBit.result_flag = Zf;
				asf->quaternion.result_s = asc.result_s;
				break;

			default:
				break;
			}
			/*Intg attr = -1;
			if (asc.arg1Flag == Dz
				&& (attr = getAttribute(asf->quaternion.arg1_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg1_flag = attr;

			}
			else if (asc.arg1Flag == Bs
				&& (attr = getAttribute(asf->quaternion.arg1_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg1_flag = attr;

			}
			if (asc.arg2Flag == Dz
				&& (attr = getAttribute(asf->quaternion.arg2_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg2_flag = attr;

			}
			else if (asc.arg2Flag == Bs
				&& (attr = getAttribute(asf->quaternion.arg2_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.arg2_flag = attr;

			}
			if (asc.resultFlag == Dz
				&& (attr = getAttribute(asf->quaternion.result_fka,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.result_flag = attr;

			}
			else if (asc.resultFlag == Bs
				&& (attr = getAttribute(asf->quaternion.result_s,
					FKA(currentFileKey, stackToAddr()), false)) != -1) {
				asf->flagBit.result_flag = attr;

			}*/
		}
		else if (asc.type == COOL_ME) {

			asf->flagBit.type = COOL_ME;

		}
		else if (asc.type == COOL_BRK) {

			asf->flagBit.type = COOL_BRK;

		}
		else if (asc.type == COOL_CTN) {

			asf->flagBit.type = COOL_CTN;

		}
		else if (asc.type == COOL_FIL) {

			asf->flagBit.type = COOL_FIL;
			asf->flagBit.arg1_flag = Zf;
			asf->quaternion.arg1_s = asc.arg1_s;

		}
		else if (asc.type == COOL_LIN) {

			asf->flagBit.type = COOL_LIN;
			asf->flagBit.arg1_flag = Sz;
			asf->quaternion.arg1_i = asc.arg1_i;
		}
		else if (asc.type == COOL_QE) {
			asf->flagBit.type = COOL_QE;
			asf->flagBit.result_flag = Z_Dz;

			asf->quaternion.result_fka.addr = stackToAddr();
			asf->quaternion.result_fka.fileKey = currentFileKey;
		}
		else if (asc.type == EXPRQE) {
			asf->flagBit.type = EXPRQE;
			asf->flagBit.result_flag = EXPRZ_Dz;

			asf->quaternion.result_fka.addr = stackToAddr();
			asf->quaternion.result_fka.fileKey = currentFileKey;
		}
		else if (((asc.type == COOL_FUN && asc.operator_s == "@")
			|| (asc.type == COOL_TD && asc.operator_s == "=>"))
			&& asc.operatorFlag == Bs) {
			switch (asc.arg1Flag) {
			case Dz:
				asf->flagBit.arg1_flag = Z_Dz;
				asf->quaternion.arg1_fka.addr = link(currentAR->scopeStructureFKA.addr,
					asc.arg1_i);
				asf->quaternion.arg1_fka.fileKey = currentFileKey;
				break;
			case Sz:
#if debug
				cerr << "COOL_FUN COOL_TD (Sz,x,x,x) err(should be Dz)" << endl;
#endif
				break;
			case Bs:
#if debug
				cerr << "COOL_FUN COOL_TD(Bs,x,x,x) err(should be Dz)" << endl;
#endif
				break;
			case Zf:
#if debug
				cerr << "COOL_FUN COOL_TD(Zf,x,x,x) err(should be Dz)" << endl;
#endif
				break;

			default:
#if debug
				cerr << "COOL_FUN COOL_TD(arg1Flag,x,x,x) err(should be Dz)" << endl;
#endif
				break;
			}
			switch (asc.arg2Flag) {
			case Dz:
				asf->flagBit.arg2_flag = Z_Dz;
				asf->quaternion.arg2_fka.addr = link(currentAR->scopeStructureFKA.addr,
					asc.arg2_i);
				asf->quaternion.arg2_fka.fileKey = currentFileKey;
				break;
			case Sz:
				asf->flagBit.arg2_flag = Sz;
				asf->quaternion.arg2_i = asc.arg2_i;
				break;
			case Bs:
				asf->flagBit.arg2_flag = S_Bs;
				asf->quaternion.arg2_s = asc.arg2_s;
#if debug
				clog << "COOL_FUN COOL_TD(x,Bs,x,x) warn(should be Sz)" << endl;
#endif
				break;
			case Zf:
				asf->flagBit.arg2_flag = Zf;
				asf->quaternion.arg2_s = asc.arg2_s;
#if debug
				cerr << "COOL_FUN COOL_TD(x,Zf,x,x) err(should be Sz)" << endl;
#endif
				break;

			default:
				break;
			}
			asf->quaternion.operator_s = asc.operator_s;
			asf->flagBit.operator_flag = M_Bs;
			switch (asc.resultFlag) {
			case Dz:
				asf->flagBit.result_flag = Z_Dz;
				asf->quaternion.result_fka.addr = link(currentAR->scopeStructureFKA.addr,
					asc.result_i);
				asf->quaternion.result_fka.fileKey = currentFileKey;
				break;
			case Sz:
				asf->flagBit.result_flag = Sz;
				asf->quaternion.result_i = asc.result_i;
				break;
			case Bs:
				asf->flagBit.result_flag = S_Bs;
				asf->quaternion.result_s = asc.result_s;
				break;
			case Zf:
				asf->flagBit.result_flag = Zf;
				asf->quaternion.result_s = asc.result_s;
				break;

			default:
				break;
			}
			if (asc.resultFlag != asc.arg1Flag || asc.result_i != asc.arg1_i) {
#if debug
				cerr << "COOL_FUN COOL_TD(arg1,x,x,result) err(arg1 not equal result)" << endl;
#endif
			}
			asf->quaternion.arg1_fka.addr = link(currentAR->scopeStructureFKA.addr, asc.arg1_i);
			asf->quaternion.arg1_fka.fileKey = currentFileKey;
			asf->quaternion.result_fka = asf->quaternion.arg1_fka;

		}
		else if (asc.type == COOL_FUN && asc.operator_s == ":" && asc.operatorFlag == Bs
			&& asc.arg2Flag == Bs
			&& (asc.arg2_s == "e" || asc.arg2_s == "exp"
				|| asc.arg2_s == "expression")) {
			if (asc.resultFlag != asc.arg1Flag || asc.result_i != asc.arg1_i) {
#if debug
				cerr << "COOL_FUN (arg1,x,x,result) err(arg1 not equal result)" << endl;
#endif
			}
			asf->flagBit.type = COOL_FUN;
			asf->flagBit.arg1_flag = Z_Dz;
			asf->quaternion.arg1_fka.addr = link(currentAR->scopeStructureFKA.addr, asc.arg1_i);
			asf->quaternion.arg1_fka.fileKey = currentFileKey;
			asf->flagBit.arg2_flag = X_Bs;
			asf->quaternion.arg2_s = "e";
			asf->flagBit.operator_flag = M_Bs;
			asf->quaternion.operator_s = ":";
			asf->flagBit.result_flag = Z_Dz;
			asf->quaternion.result_fka = asf->quaternion.arg1_fka;

		}
		//		else if (asc.type == COOL_FUN && asc.operator_s == ":" && asc.operatorFlag == Bs
		//			&& asc.arg2Flag == Bs && asc.arg2_s == "!") {
		//			if (asc.resultFlag != asc.arg1Flag || asc.result_i != asc.arg1_i) {
		//#if debug
		//				cerr << "COOL_FUN (arg1,x,x,result) err(arg1 not equal result)" << endl;
		//#endif
		//			}
		//			asf->flagBit.type = COOL_FUN;
		//			asf->flagBit.arg1_flag = Z_Dz;
		//			asf->quaternion.arg1_fka.addr = link(currentScopeFKA.addr, asc.arg1_i);
		//			asf->quaternion.arg1_fka.addr = currentFileKey;
		//			asf->flagBit.arg2_flag = X_Bs;
		//			asf->quaternion.arg2_s = "!";
		//			asf->flagBit.operator_flag = M_Bs;
		//			asf->quaternion.operator_s = ":";
		//			asf->flagBit.result_flag = Z_Dz;
		//			asf->quaternion.result_fka = asf->quaternion.arg1_fka;
		//			//backward function name
		//#if debug
		//			{
		//				static int ignorecount = 0;
		//				cout
		//					<< "Scanner::setQuaternionAndFlagBit(const AssemblyCode&, AssemblyFormula*)"
		//					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
		//					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
		//			}
		//#endif
		//			//backward function name scope fileKeyAddr
		//			FKA bfnScopeFKA = FKA(currentFileKey,
		//				link(stackToAddr(), currentAssemblyCode.arg1_i));
		//
		//			(*templateFunctionTableHash)[bfnScopeFKA].isBackwardFunction = true;
		//
		//			FKA& bfnHandleFKA =
		//				(*templateFunctionTableHash)[bfnScopeFKA].templateHandleFKA;
		//			Code& bfnHandle = (*codeTableHash)[bfnHandleFKA];
		//
		//		}
		else if (asc.type == COOL_FUN && asc.operator_s == "" && asc.operator_i == 0
			&& asc.arg1Flag == Dz && asc.arg2Flag == Dz && asc.resultFlag == Dz
			&& asc.arg1_i == asc.result_i && asc.arg1_i != asc.arg2_i) {
			asf->flagBit.type = COOL_FUN;
			asf->flagBit.arg1_flag = Z_Dz;
			FKA a = FKA(currentFileKey, link(currentAR->scopeStructureFKA.addr, asc.arg1_i));
			while ((*scopeTableHash)[a].validTemplateFunctionNameFKA != fkanull) {
				a = (*scopeTableHash)[a].validTemplateFunctionNameFKA;
			}
			asf->quaternion.arg1_fka = a;
			asf->flagBit.arg2_flag = Z_Dz;
			asf->quaternion.arg2_fka.addr = link(currentAR->scopeStructureFKA.addr, asc.arg2_i);
			asf->quaternion.arg2_fka.fileKey = currentFileKey;
			asf->flagBit.operator_flag = 0;
			asf->flagBit.result_flag = Z_Dz;
			asf->quaternion.result_fka = a;
		}
		else if (asc.type == COOL_B) {
			asf->flagBit.type = COOL_B;
			switch (asc.arg1Flag) {
			case Dz:
				asf->flagBit.arg1_flag = S_Dz;
				asf->quaternion.arg1_fka.addr = asc.arg1_i == 0 ? addrnull : Addr(asc.arg1_i);
				asf->quaternion.arg1_fka.fileKey = currentFileKey;
				break;
			case Sz:
				asf->flagBit.arg1_flag = Sz;
				asf->quaternion.arg1_i = asc.arg1_i;
				break;
			default:
				break;
			}
			switch (asc.arg2Flag) {
			case Dz:
				asf->flagBit.arg2_flag = S_Dz;
				asf->quaternion.arg2_fka.addr = asc.arg2_i == 0 ? addrnull : Addr(asc.arg2_i);
				asf->quaternion.arg2_fka.fileKey = currentFileKey;
				break;
			case Sz:
				asf->flagBit.arg2_flag = Sz;
				asf->quaternion.arg2_i = asc.arg2_i;
				break;
			case Bs:
				asf->flagBit.arg2_flag = S_Bs;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;
			case Zf:
				asf->flagBit.arg2_flag = Zf;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;

			default:
				break;
			}
			asf->flagBit.operator_flag = Z_Dz;
			asf->quaternion.operator_fka.addr = link(currentAR->scopeStructureFKA.addr,
				asc.operator_i);
			asf->quaternion.operator_fka.fileKey = currentFileKey;

			asf->flagBit.result_flag = S_Dz;
			asf->quaternion.result_fka.addr = asc.result_i == 0 ? addrnull : Addr(asc.result_i);
			asf->quaternion.result_fka.fileKey = currentFileKey;
		}
		else if (asc.type == COOL_L) {
			asf->flagBit.type = COOL_L;

			asf->flagBit.arg1_flag = S_Dz;
			asf->quaternion.arg1_fka.addr = asc.arg1_i == 0 ? addrnull : Addr(asc.arg1_i);
			asf->quaternion.arg1_fka.fileKey = currentFileKey;

			switch (asc.arg2Flag) {
			case Dz:
				asf->flagBit.arg2_flag = S_Dz;
				asf->quaternion.arg2_fka.addr = asc.arg2_i == 0 ? addrnull : Addr(asc.arg2_i);
				asf->quaternion.arg2_fka.fileKey = currentFileKey;
				break;
			case Sz:
				asf->flagBit.arg2_flag = Sz;
				asf->quaternion.arg2_i = asc.arg2_i;
				break;
			case Bs:
				asf->flagBit.arg2_flag = S_Bs;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;
			case Zf:
				asf->flagBit.arg2_flag = Zf;
				asf->quaternion.arg2_s = asc.arg2_s;
				break;

			default:
				break;
			}
			asf->flagBit.operator_flag = Z_Dz;
			asf->quaternion.operator_fka.addr = link(currentAR->scopeStructureFKA.addr,
				asc.operator_i);
			asf->quaternion.operator_fka.fileKey = currentFileKey;

		}
		else if (asc.type == COOL_S && asc.operatorFlag == Bs
			&& asc.operator_s == ":") {
			//(systemName,system,:,systemName)

			asf->flagBit.arg1_flag = X_Bs;
			asf->quaternion.arg1_s = asc.arg1_s;

			asf->flagBit.arg2_flag = X_Bs;
			asf->quaternion.arg2_s = asc.arg2_s;

			asf->quaternion.operator_s = asc.operator_s;
			asf->flagBit.operator_flag = M_Bs;

			asf->flagBit.result_flag = X_Bs;
			asf->quaternion.result_s = asc.result_s;

		}
		else if (asc.type == COOL_S && asc.arg1Flag == Bs && asc.arg2Flag == Dz
			&& asc.operator_s == "") {
			//(systemName,systemScopeAddr,,systemName)

			asf->flagBit.arg1_flag = X_Bs;
			asf->quaternion.arg1_s = asc.arg1_s;

			asf->flagBit.arg2_flag = Z_Dz;
			asf->quaternion.arg2_fka.addr = link(stackToAddr(), asc.arg2_i);
			asf->quaternion.arg2_fka.fileKey = currentFileKey;

			asf->flagBit.result_flag = X_Bs;
			asf->quaternion.result_s = asc.result_s;

		}
		else if (asc.type == COOL_S && asc.arg1Flag == Bs && asc.arg2Flag == Bs
			&& asc.operator_s == "<<") {
			//(systemName,systemName,<<,systemName)

			asf->flagBit.arg1_flag = X_Bs;
			asf->quaternion.arg1_s = asc.arg1_s;

			asf->flagBit.arg2_flag = X_Bs;
			asf->quaternion.arg2_s = asc.arg2_s;

			asf->quaternion.operator_s = asc.operator_s;
			asf->flagBit.operator_flag = M_Bs;

			asf->flagBit.result_flag = X_Bs;
			asf->quaternion.result_s = asc.result_s;

		}

		//如果变量属性在argAttributeTable中可查，则以argAttributeTable中内容为准
		//将所有S_Dz前都link scope addr， 以使所有S_Dz都不超过当前code addr。
		//!!!!仅限S_Dz，其他类型地址请在特殊处理!!!

		if (asf->flagBit.arg1_flag == S_Dz && asf->quaternion.arg1_fka.addr != addrnull) {
			asf->quaternion.arg1_fka.addr = link(this->stackToAddr(), asf->quaternion.arg1_fka.addr);

		}
		if (asf->flagBit.arg2_flag == S_Dz && asf->quaternion.arg2_fka.addr != addrnull) {
			asf->quaternion.arg2_fka.addr = link(this->stackToAddr(), asf->quaternion.arg2_fka.addr);

		}
		if (asf->flagBit.result_flag == S_Dz && asf->quaternion.result_fka.addr != addrnull) {
			asf->quaternion.result_fka.addr = link(this->stackToAddr(), asf->quaternion.result_fka.addr);

		}


		//set constraints in the flagbits
		/*Intg attr = -1;
		if (asc.arg1Flag == Dz
			&& (attr = getAttribute(asf->quaternion.arg1_fka,
				FKA(currentFileKey, stackToAddr()), false)) != -1) {
			asf->flagBit.arg1_flag = attr;

		}*/
		if (asf->flagBit.arg1_flag == S_Bs
			&& (this->currentAR->findcurrent(Arg(S_Bs, asf->quaternion.arg1_s)))
			) {
			asf->flagBit.arg1_constraints = this->currentAR->getKeyArg(Arg(S_Bs, asf->quaternion.arg1_s)).constraints;

		}
		/*if (asc.arg2Flag == Dz
			&& (attr = getAttribute(asf->quaternion.arg2_fka,
				FKA(currentFileKey, stackToAddr()), false)) != -1) {
			asf->flagBit.arg2_flag = attr;

		}*/
		if (asf->flagBit.arg2_flag == S_Bs
			&& (this->currentAR->findcurrent(Arg(S_Bs, asf->quaternion.arg2_s)))
			) {
			asf->flagBit.arg2_constraints = this->currentAR->getKeyArg(Arg(S_Bs, asf->quaternion.arg2_s)).constraints;

		}
		/*if (asc.resultFlag == Dz
			&& (attr = getAttribute(asf->quaternion.result_fka,
				FKA(currentFileKey, stackToAddr()), false)) != -1) {
			asf->flagBit.result_flag = attr;

		}*/
		if (asf->flagBit.result_flag == S_Bs
			&& (this->currentAR->findcurrent(Arg(S_Bs, asf->quaternion.result_s)))
			) {
			asf->flagBit.result_constraints = this->currentAR->getKeyArg(Arg(S_Bs, asf->quaternion.result_s)).constraints;

		}


		return true;
	}
	/// <summary>
	/// this function is just a framework, and detailed work is done in:
	/// *
	/// *create AR and add args: addArgsToCurrentAR 
	/// </summary>
	/// <returns></returns>
	const bool Scanner::scan() {
		if (assemblyCodeTable == nullptr && ascth.size() > 0) {
			currentFileKey = ascth.addr_file_map.begin()->second;
		}
		else {
			currentFileKey = currentAR->scopeStructureFKA.fileKey;
		}
		currentScope.fileKey = currentAR->scopeStructureFKA.fileKey;
		scopeTableHash->newScopeStart(currentAR->scopeStructureFKA, currentScope);


		while (updateCurrentAssemblyCode() == true) {
			{
#if debug
				static int ignorecount = 0;
				cout << "Scanner::scan() currentAssemblyCode:["
					<< currentAssemblyCode.toStrg() << "\t]"
					<< "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__
					<< ":" << __LINE__ << ":0" << ")" << endl;
#endif
			}

#if debug && _WIN32
			{
				stringstream ss;
				ss << "currentAssemblyCode: ["
					<< currentAssemblyCode.toStrg() << "\t]" << endl;
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = ss.str(); // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif

			if (currentAssemblyCode.type == COOL_QS) {
				currentScope.parentScopeFKA = currentAR->scopeStructureFKA;
				scopeStack.push_back(currentAssemblyCode.result_i);
				FKA currentScopeFKA = FKA(currentFileKey, stackToAddr());
				currentCodeFKA = getQSFKA(
					assemblyCodeTable->acpdq[currentAssemblyCodePos]);

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentScopeFKA, a);

				currentCode.assemblyFormula.flagBit.execute = T_;

				codeTableHash->addCode(currentCodeFKA, currentCode);
#if debug
				cout << "p 2" << endl;
#endif
				currentScope.scopeCoor1 = currentCodeFKA.addr;

				scopeTableHash->newScopeStart(currentScopeFKA, currentScope);
			}
			else if (currentAssemblyCode.type == EXPRQS) {

#if debug && _WIN32
				{
					static int ignoreCount = 0;
					// Assuming info is a std::string, convert to std::wstring
					std::string info = "EXPRQS found"; // Replace with actual info
					info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
					std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
					OutputDebugString(winfo.c_str());
#endif
					cout << info << endl;
					ignoreCount++;
				}
#endif

				currentScope.parentScopeFKA = currentAR->scopeStructureFKA;
				scopeStack.push_back(currentAssemblyCode.result_i);
				FKA currentScopeFKA = FKA(currentFileKey, stackToAddr());
				exprz_dz_set.insert(currentScopeFKA);
				currentCodeFKA = getQSFKA(
					assemblyCodeTable->acpdq[currentAssemblyCodePos]);
				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentScopeFKA, a);

				currentCode.assemblyFormula.flagBit.execute = T_;

				codeTableHash->addCode(currentCodeFKA, currentCode);

				currentScope.scopeCoor1 = currentCodeFKA.addr;

				scopeTableHash->newScopeStart(currentScopeFKA, currentScope);
				(*scopeTableHash)[currentScopeFKA].functionFlag =
					EXPRB;
			}
			else if (currentAssemblyCode.type == COOL_Y) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));
				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				currentCode.assemblyFormula.flagBit.execute = T_;
				codeTableHash->addCode(currentCodeFKA, currentCode);
				globalRefTable.addRefData(currentAR, codeTableHash, currentCodeFKA,
					3);
			}
			else if (currentAssemblyCode.type == COOL_LST) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));
				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				currentCode.assemblyFormula.flagBit.execute = T_;
				codeTableHash->addCode(currentCodeFKA, currentCode);
			}
			else if (currentAssemblyCode.type == COOL_M) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);

				codeTableHash->addCode(currentCodeFKA, currentCode);
				if (nextAssemblyCode.type != COOL_M && nextAssemblyCode.type != COOL_R) {
#if debug
					{
						static int ignorecount = 0;
						std::cout << "Scanner::scan()"
							<< " handle pendArg and formArg, currentCodeFKA:["
							<< currentCodeFKA.toStrg() << "]"
							<< "\tignorecount:[" << ignorecount++ << "\t]("
							<< __FILE__ << ":" << __LINE__ << ":0" << ")"
							<< std::endl;
					}
#endif

					if (pendM.size() != 0) {
						PendPainter pp(this);

						pp.paintAll();
						pendM.clear();
					}

					if (realM.size() != 0) {
						RealPainter rp(this);
						rp.paintAll();
						realM.clear();
					}

					if (compatibleM.size() != 0) {
						CompatiblePainter cp(this);
						cp.paintAll();
						compatibleM.clear();
					}
					//if (constraintM.size() != 0) {
					ConstraintPainter cp(this);
					cp.paintAll();
					constraintM.clear();

					//}


#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						stringstream ss;
						ss << bbhline;
						ss << __FUNCTION__ << " After painting changeable/real/compatible/constraint:\n";
						ss << get<0>(this->codeTableHash->copyTree(currentCodeFKA)).toStrg();
						ss << bbhline;
						std::string info = ss.str(); // Replace with actual info
						info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
						std::cout << info << std::endl;
						std::wstring winfo(info.begin(), info.end());
						OutputDebugString(winfo.c_str());
						ignoreCount++;
					}
#endif

				}

			}
			else if (currentAssemblyCode.type == COOL_ME) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);

			}
			else if (currentAssemblyCode.type == COOL_BRK) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);

			}
			else if (currentAssemblyCode.type == COOL_CTN) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);

			}
			else if (currentAssemblyCode.type == COOL_FIL) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);

			}
			else if (currentAssemblyCode.type == COOL_LIN) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);

			}
			else if (currentAssemblyCode.type == COOL_N) {
				//空代码仅起分割作用，用于引导匹配。无需对其做任何处理。
				//与COOL_ME不同的是，N不能截断最长表达式.这也意味着被N分割的两段COOL_M类型代码其间可能有共用的临时变量

				currentCodeFKA =
					FKA(currentFileKey,
						link(stackToAddr(),
							assemblyCodeTable->acpdq[currentAssemblyCodePos
							]));
				currentCodeFKA = FKA(currentFileKey, link(currentCodeFKA.addr, 1));
				currentCodeFKA = FKA(currentFileKey, link(currentCodeFKA.addr, 3));
				assemblyCodeTable->acdq.erase(
					assemblyCodeTable->acdq.begin() + currentAssemblyCodePos
				);
				assemblyCodeTable->acpdq.erase(
					assemblyCodeTable->acpdq.begin() + currentAssemblyCodePos
				);
				//currentAssemblyCodePos -= 1;

				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA,
					AssemblyFormula());
				codeTableHash->addCode(currentCodeFKA, currentCode);

			}
			else if (currentAssemblyCode.type == COOL_C) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);

			}
			else if (currentAssemblyCode.type == COOL_QE) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);

				currentScope = (*scopeTableHash)[currentAR->scopeStructureFKA];
				currentScope.scopeCoor2 = currentCodeFKA.addr;
				if (currentScope.parentScopeFKA
					!= fkanull && currentScope.functionFlag == RFB
					&& (*scopeTableHash)[currentScope.parentScopeFKA].functionFlag
					!= FB) {
					(*scopeTableHash)[currentScope.parentScopeFKA].functionFlag =
						RFB;
					push_back(
						(*scopeTableHash)[currentScope.parentScopeFKA].returnHandle,
						currentScope.returnHandle);
				}
				(*scopeTableHash)[currentAR->scopeStructureFKA] = currentScope;

				//返回上一级Scope
				if (this->scopeStack.size() > 0) {
					this->scopeStack.pop_back();
				}
				FKA currentScopeFKA = FKA(currentFileKey, stackToAddr());
				currentScope = (*scopeTableHash)[currentScopeFKA];

			}
			else if (currentAssemblyCode.type == EXPRQE) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);

				currentScope = (*scopeTableHash)[currentAR->scopeStructureFKA];
				currentScope.scopeCoor2 = currentCodeFKA.addr;

				(*scopeTableHash)[currentAR->scopeStructureFKA].functionFlag =
					EXPRB;

				(*scopeTableHash)[currentAR->scopeStructureFKA] = currentScope;

				//返回上一级Scope
				if (this->scopeStack.size() > 0) {
					this->scopeStack.pop_back();
				}
				FKA currentScopeFKA = FKA(currentFileKey, stackToAddr());
				currentScope = (*scopeTableHash)[currentScopeFKA];

			}
			else if (currentAssemblyCode.type == COOL_FUN
				&& currentAssemblyCode.operatorFlag == Bs
				&& currentAssemblyCode.operator_s == "@") {

				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);
				FKA scopeFKA = FKA(currentFileKey,
					link(currentAR->scopeStructureFKA.addr, currentAssemblyCode.arg1_i));
				(*scopeTableHash)[scopeFKA].functionFlag = FN;

				for (FKA fka = FKA((*scopeTableHash)[scopeFKA].fileKey,
					(*scopeTableHash)[scopeFKA].scopeCoor1);

					fka != fkanull
					&& fka.addr
					<= (*scopeTableHash)[scopeFKA].scopeCoor2;

					fka = codeTableHash->getNextCodeFKA(fka)) {
#if debug
						{
							static int ignorecount = 0;
							std::cout << "Scanner::scan()" << " (COOL_FUN,Bs,@)"
								<< "\tignorecount:[" << ignorecount++ << "\t]("
								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
								<< std::endl;
						}
#endif
						if ((*codeTableHash)[fka].assemblyFormula.flagBit.execute != Du) {
							(*codeTableHash)[fka].assemblyFormula.flagBit.execute =
								F_;
						}

				}

				TemplateFunction tf((*scopeTableHash)[scopeFKA]);
				FKA tfFKA = scopeFKA;
				tf.templateHandleFKA = lastM;
				//            tf.organization = currentAssemblyCode.arg2_i;
				tf.organization = TFInfoMat(currentAssemblyCode.arg2_s);

				if ((*codeTableHash)[lastM].assemblyFormula.flagBit.changeable[3]
					== T_) {
					tf.isBackwardFunction = T_;
				}

				//以下为比较之前是否有相同声明
				DecComparator dc(this);
				FKA tfFKA1 = nextSameScopeTfFKA(
					(*scopeTableHash)[tfFKA].parentScopeFKA, tfFKA);
				while (tfFKA1 != fkanull) {
					if (((*templateFunctionTableHash)[tfFKA1].organization
						!= tf.organization)
						|| ((*templateFunctionTableHash)[tfFKA1].integrate
							!= tf.integrate)) {
						tfFKA1 = nextSameScopeTfFKA(
							(*scopeTableHash)[tfFKA1].parentScopeFKA, tfFKA1);
						continue;
					}
					if (dc.cmpDecCodeTree(
						(*codeTableHash).getLast(COOL_M,
							FKA((*scopeTableHash)[tfFKA].fileKey,
								(*scopeTableHash)[tfFKA].scopeCoor2)),
						(*codeTableHash).getLast(COOL_M,
							FKA((*scopeTableHash)[tfFKA1].fileKey,
								(*scopeTableHash)[tfFKA1].scopeCoor2)))
						== true) {
						Intg pos1 =
							getAddrPosition(
								codeTableHash->operator [](
									tf.nameStartFKA.fileKey).addrdq,
								tf.nameStartFKA.addr);
						Intg pos2 =
							getAddrPosition(
								codeTableHash->operator[](
									tf.nameEndFKA.fileKey).addrdq,
								tf.nameEndFKA.addr);
						while (pos1 <= pos2) {
							codeTableHash->operator[](tf.nameStartFKA.fileKey).codedq[pos1].assemblyFormula.flagBit.execute =
								Du;
							pos1++;
						}
						if ((*scopeTableHash)[tfFKA1].validTemplateFunctionNameFKA
							!= fkanull) {
							(*scopeTableHash)[tfFKA].validTemplateFunctionNameFKA =
								(*scopeTableHash)[tfFKA1].validTemplateFunctionNameFKA;
						}
						else {
							(*scopeTableHash)[tfFKA].validTemplateFunctionNameFKA =
								tfFKA1;
						}
						break;

					}
					else {
						tfFKA1 = nextSameScopeTfFKA(
							(*scopeTableHash)[tfFKA].parentScopeFKA, tfFKA1);
					}
					dc.clear();

				}
				//将函数声明添加到templateFunctionTable中，并根据后续属性设置进行完善
				TemplateFunction* tfptr = nullptr;
				if (tfFKA1 == fkanull) {
					tf.nameStartFKA = FKA(tfFKA.fileKey,
						scopeTableHash->operator [](tfFKA).scopeCoor1);
					(*templateFunctionTableHash).add(tfFKA, tf, scopeTableHash);
					tfptr = &(*templateFunctionTableHash)[tfFKA];

				}
				else {
					tfptr = &(*templateFunctionTableHash)[tfFKA1];
				}
				while (nextAssemblyCode.type == COOL_FUN
					&& FKA(currentFileKey,
						link(currentAR->scopeStructureFKA.addr, nextAssemblyCode.arg1_i))
					== tfFKA
					//                    && nextAssemblyCode.operatorFlag == Bs
					//                    && nextAssemblyCode.operator_s == ":"
					) {
					updateCurrentAssemblyCode();
					currentCodeFKA =
						FKA(currentFileKey,
							link(stackToAddr(),
								assemblyCodeTable->acpdq[currentAssemblyCodePos]));
					AssemblyFormula a;
					setQuaternionAndFlagBit(currentAssemblyCode, &a);
					currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
					codeTableHash->addCode(currentCodeFKA, currentCode);
					updateArgAttribute(currentCode);
					if (currentCode.assemblyFormula.quaternion.arg2_s == "e"
						|| currentCode.assemblyFormula.quaternion.arg2_s == "exp"
						|| currentCode.assemblyFormula.quaternion.arg2_s
						== "expr"
						|| currentCode.assemblyFormula.quaternion.arg2_s
						== "expression") {
						tfptr->integrate = T_;
					}

				}

			}
			else if (currentAssemblyCode.type == COOL_S
				&& currentAssemblyCode.operatorFlag == Bs
				&& currentAssemblyCode.operator_s == ":") {
#if debug
					{
						static int ignorecount = 0;
						cout << "Scanner::scan()" << " " << "\tignorecount:["
							<< ignorecount++ << "\t](" << __FILE__ << ":"
							<< __LINE__ << ":0" << ")" << endl;
					}
#endif

					currentCodeFKA = FKA(currentFileKey,
						link(stackToAddr(),
							assemblyCodeTable->acpdq[currentAssemblyCodePos]));

					AssemblyFormula a;
					setQuaternionAndFlagBit(currentAssemblyCode, &a);
					currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
					codeTableHash->addCode(currentCodeFKA, currentCode);
					System sys(currentAssemblyCode.arg1_s,
						FKA(currentFileKey, stackToAddr())); //System::System(const Strg &name, const FKA &parentScopeFKA)
					systemTableHash->add(sys);

			}
			else if (currentAssemblyCode.type == COOL_S
				&& currentAssemblyCode.operatorFlag == Bs
				&& currentAssemblyCode.operator_s == "<<") {

				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);
				systemTableHash->inherit(currentAssemblyCode.arg1_s,
					FKA(currentFileKey, stackToAddr()),
					currentAssemblyCode.arg2_s);

			}
			else if (currentAssemblyCode.type == COOL_S
				&& currentAssemblyCode.arg2Flag == Dz
				&& currentAssemblyCode.operator_s == "") {

				//此步骤进行scope与sys进行绑定。由于scope位于sys声明之后，
				//因此需要对已有的sys进行补充，
				//同时，重新添加sys对应的ar并补充其继承的类对应的ar
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);
				FKA&& bodyScopeFKA = FKA(currentFileKey,
					link(stackToAddr(), currentAssemblyCode.arg2_i));
				Scope& bodyScope = (*scopeTableHash)[bodyScopeFKA];
				bodyScope.functionFlag = SYSB;
				FKA&& scopeCoor1fka = FKA(bodyScope.fileKey, bodyScope.scopeCoor1);
				FKA&& scopeCoor2fka = FKA(bodyScope.fileKey, bodyScope.scopeCoor2);
				(*codeTableHash)[scopeCoor1fka].assemblyFormula.flagBit.execute =
					COOL_R;
				(*codeTableHash)[scopeCoor2fka].assemblyFormula.flagBit.execute =
					COOL_R;

				System sys(currentAssemblyCode.arg1_s, bodyScopeFKA,
					FKA(currentFileKey, stackToAddr()));
				systemTableHash->add(sys);
				currentAR->add(Arg(S_Dz, bodyScopeFKA));
				(*currentAR)[Arg(S_AR, bodyScopeFKA)].dataFlagBit = S_AR;
				DataTablePtr sysAR = (*currentAR)[Arg(S_AR, bodyScopeFKA)].content_ar =
					file_fka_ar_map[currentFileKey].find(bodyScopeFKA)->second;
				sysAR->type = SYSB;
				System& sysWhole = (*systemTableHash)[bodyScopeFKA];

				for (auto fki : sysWhole.decedentSystemFKIList) {
					FKA& decedentSysFKA = (*systemTableHash)[fki].systemFKA;
					sysAR->queryARList.push_front(
						file_fka_ar_map[currentFileKey][decedentSysFKA]);
				}

			}
			else if (currentAssemblyCode.type == COOL_TD
				&& currentAssemblyCode.operatorFlag == Bs
				&& currentAssemblyCode.operator_s == "=>") {

				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);

				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);
				//backward funciton name
				FKA bfnScopeFKA = currentCode.assemblyFormula.quaternion.arg1_fka;
				//forward function name
				FKA ffnScopeFKA = currentCode.assemblyFormula.quaternion.arg2_fka;
				(*templateFunctionTableHash)[bfnScopeFKA].isBackwardFunction = true;
				(*templateFunctionTableHash)[bfnScopeFKA].forwardFunctionFKA =
					ffnScopeFKA;

				FKA& bfnHandleFKA =
					(*templateFunctionTableHash)[bfnScopeFKA].templateHandleFKA;
				Code& bfnHandle = (*codeTableHash)[bfnHandleFKA];

			}
			else if (currentAssemblyCode.type == COOL_FUN
				&& currentAssemblyCode.arg1Flag == Dz
				&& currentAssemblyCode.arg2Flag == Dz
				&& currentAssemblyCode.resultFlag == Dz
				&& currentAssemblyCode.arg1_i == currentAssemblyCode.result_i
				&& currentAssemblyCode.arg1_i != currentAssemblyCode.arg2_i
				&& currentAssemblyCode.operator_i == 0
				&& currentAssemblyCode.operator_s == "") {

				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));
				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);
				(*templateFunctionTableHash)[a.quaternion.arg1_fka].bodyScopeFKA =
					a.quaternion.arg2_fka;
				(*templateFunctionTableHash)[a.quaternion.arg1_fka].bodyStartFKA =
					FKA(a.quaternion.arg2_fka.fileKey,
						(*scopeTableHash)[a.quaternion.arg2_fka].scopeCoor1);

				(*templateFunctionTableHash)[a.quaternion.arg1_fka].bodyEndFKA =
					FKA(a.quaternion.arg2_fka.fileKey,
						(*scopeTableHash)[a.quaternion.arg2_fka].scopeCoor2);
				(*templateFunctionTableHash)[a.quaternion.arg1_fka].returnHandle =
					(*scopeTableHash)[a.quaternion.arg2_fka].returnHandle;
				(*scopeTableHash)[a.quaternion.arg2_fka].requireScopeFKA =
					a.quaternion.arg1_fka;
#if debug
				cout << "Scanner::scan COOL_FUN Dz Dz null Dz" << endl;
#endif

				if ((*scopeTableHash)[a.quaternion.arg2_fka].functionFlag == RFB
					&& (*scopeTableHash)[a.quaternion.arg2_fka].parentScopeFKA
					!= fkanull) {
#if debug
					cout << "Scanner::scan COOL_FUN Dz Dz null Dz 3" << endl;
#endif
					FKA pa = (*scopeTableHash)[a.quaternion.arg2_fka].parentScopeFKA;
#if debug
					cout << "Scanner::scan COOL_FUN Dz Dz null Dz 4" << endl;
#endif

					for (auto& fkait : (*scopeTableHash)[a.quaternion.arg2_fka].returnHandle) {
						erase((*scopeTableHash)[pa].returnHandle, fkait);
					}
				}

				(*scopeTableHash)[a.quaternion.arg2_fka].functionFlag =
					FB;

				(*codeTableHash)[FKA(a.quaternion.arg2_fka.fileKey,
					(*scopeTableHash)[a.quaternion.arg2_fka].scopeCoor1)].assemblyFormula.flagBit.execute =
					COOL_R;
				(*codeTableHash)[FKA(a.quaternion.arg2_fka.fileKey,
					(*scopeTableHash)[a.quaternion.arg2_fka].scopeCoor2)].assemblyFormula.flagBit.execute =
					COOL_R;

				FKA scopeFKA = FKA(currentFileKey,
					link(currentAR->scopeStructureFKA.addr, currentAssemblyCode.arg2_i));
				FKA cdfka = FKA((*scopeTableHash)[scopeFKA].fileKey,
					(*scopeTableHash)[scopeFKA].scopeCoor1);
				for (cdfka = codeTableHash->getNextCodeFKA(cdfka);

					cdfka.fileKey == (*scopeTableHash)[scopeFKA].fileKey
					&& cdfka.addr
					<= (*scopeTableHash)[scopeFKA].scopeCoor2
					&& cdfka != fkanull;

					cdfka = codeTableHash->getNextCodeFKA(cdfka)) {
					if ((*codeTableHash)[cdfka].assemblyFormula.flagBit.type == COOL_QS
						&& ((*codeTableHash)[cdfka].assemblyFormula.flagBit.execute
							== COOL_R
							|| (*codeTableHash)[cdfka].assemblyFormula.flagBit.execute
							== F_)) {
						FKA jumpscopeFKA = (*codeTableHash)[cdfka].scopeFKA;
						cdfka = FKA((*scopeTableHash)[jumpscopeFKA].fileKey,
							(*scopeTableHash)[jumpscopeFKA].scopeCoor2);
					}
					if ((*codeTableHash)[cdfka].assemblyFormula.flagBit.type == COOL_M) {
						Intg* changeable_ =
							(*codeTableHash)[cdfka].assemblyFormula.flagBit.changeable;

						if (changeable_[0] == A_) {
							changeable_[0] = F_;
						}
						if (changeable_[1] == A_) {
							changeable_[1] = F_;
						}
						if (changeable_[2] == A_) {
							changeable_[2] = F_;
						}
						if (changeable_[3] == A_) {
							changeable_[3] = F_;
						}

					}

				}
			}
			else if (currentAssemblyCode.type == COOL_R) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);

				if (pendM.size() != 0) {
					PendPainter pp(this);
					pp.paintAll();
					pendM.clear();
				}
				if (realM.size() != 0) {
					RealPainter rp(this);
					rp.paintAll();
					realM.clear();
				}
				if (compatibleM.size() != 0) {
					CompatiblePainter cp(this);
					cp.paintAll();
					compatibleM.clear();
				}
				if (constraintM.size() != 0) {
					ConstraintPainter cp(this);
					cp.paintAll();
					constraintM.clear();
				}
				if (currentScope.functionFlag != FB) {
					currentScope.functionFlag = RFB;
					(*scopeTableHash)[currentAR->scopeStructureFKA].returnHandle.push_back(
						currentCodeFKA);
				}

				while (nextAssemblyCode.type != COOL_QE) {
					updateCurrentAssemblyCode();
				}

			}
			else if (currentAssemblyCode.type == COOL_ABT) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				codeTableHash->addCode(currentCodeFKA, currentCode);

			}
			else if (currentAssemblyCode.type == COOL_B) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				if (currentAssemblyCode.arg2Flag == S_Dz) {

					Code& conditionRoot =
						(*codeTableHash)[currentCode.assemblyFormula.quaternion.arg2_fka];
					currentCode.assemblyFormula.flagBit.changeable[1] =
						conditionRoot.assemblyFormula.flagBit.changeable[3];
					currentCode.assemblyFormula.flagBit.formalArgFlag[1] =
						conditionRoot.assemblyFormula.flagBit.formalArgFlag[3];

				}
				codeTableHash->addCode(currentCodeFKA, currentCode);

				Scope& conditionalBodyScope =
					(*scopeTableHash)[a.quaternion.operator_fka];
				conditionalBodyScope.conditionFlag = BRB;
				conditionalBodyScope.conditionCodeFKA = currentCodeFKA;

				(*codeTableHash)[FKA(conditionalBodyScope.fileKey,
					conditionalBodyScope.scopeCoor1)].assemblyFormula.flagBit.execute =
					COOL_R;
				(*codeTableHash)[FKA(conditionalBodyScope.fileKey,
					conditionalBodyScope.scopeCoor2)].assemblyFormula.flagBit.execute =
					COOL_R;

			}
			else if (currentAssemblyCode.type == COOL_L) {
				currentCodeFKA = FKA(currentFileKey,
					link(stackToAddr(),
						assemblyCodeTable->acpdq[currentAssemblyCodePos]));

				AssemblyFormula a;
				setQuaternionAndFlagBit(currentAssemblyCode, &a);
				currentCode = Code(currentFileKey, currentAR->scopeStructureFKA, a);
				if (currentAssemblyCode.arg2Flag == S_Dz) {

					Code& conditionRoot =
						(*codeTableHash)[currentCode.assemblyFormula.quaternion.arg2_fka];
					currentCode.assemblyFormula.flagBit.changeable[1] =
						conditionRoot.assemblyFormula.flagBit.changeable[3];
					currentCode.assemblyFormula.flagBit.formalArgFlag[1] =
						conditionRoot.assemblyFormula.flagBit.formalArgFlag[3];

				}
				codeTableHash->addCode(currentCodeFKA, currentCode);

				Scope& conditionalBodyScope =
					(*scopeTableHash)[a.quaternion.operator_fka];
				conditionalBodyScope.conditionFlag = LOB;
				conditionalBodyScope.conditionCodeFKA = currentCodeFKA;

				(*codeTableHash)[FKA(conditionalBodyScope.fileKey,
					conditionalBodyScope.scopeCoor1)].assemblyFormula.flagBit.execute =
					COOL_R;
				(*codeTableHash)[FKA(conditionalBodyScope.fileKey,
					conditionalBodyScope.scopeCoor2)].assemblyFormula.flagBit.execute =
					COOL_R;

			}

			auto& cd = (*codeTableHash)[currentCodeFKA];
			if (cd[0].isDz() && exprz_dz_set.count(cd[0].arg_fka) > 0) {
				cd.assemblyFormula.flagBit.arg1_flag = EXPRZ_Dz;
			}
			if (cd[1].isDz() && exprz_dz_set.count(cd[1].arg_fka) > 0) {
				cd.assemblyFormula.flagBit.arg2_flag = EXPRZ_Dz;
			}
			updateArgAttribute(currentCode);

#if debug && _WIN32
			{
				stringstream ss;
				ss << "currentCode: ["
					<< currentCode.toStrg() << "\t]" << endl;
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = ss.str(); // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif

		}

		//{
		//以下部分用于将codeTable中的changeable标志位为A_转换成F_
#if debug
		cout << "Scanner::scan updateCurrentAssemblyCode == false\t" << endl;
#endif

		CodeTable* cdtp = nullptr;
		Strg fileKeyIt = codeTableHash->file_cdt_map.begin()->first;
		while (fileKeyIt != "") {
			cdtp = &codeTableHash->operator [](fileKeyIt);
			fileKeyIt = codeTableHash->getNextCdtFileKey(fileKeyIt);

			for (Addr ad = cdtp->addrdq[0];
				ad != addrnull && ad <= cdtp->addrdq.back();
				ad = cdtp->getNextCodeAddr(ad)) {
#if debug
					{
						static int ignorecount = 0;
						cout << "Scanner::scan updateCurrentAssemblyCode == false\t"
							<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
							<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
					}
#endif
					Code& cd = (*cdtp)[ad];
					if (cd.assemblyFormula.flagBit.type == COOL_QS
						&& (cd.assemblyFormula.flagBit.execute == COOL_R
							|| cd.assemblyFormula.flagBit.execute == F_)) {
						FKA jumpscopeFKA = cd.scopeFKA;
						ad = (*scopeTableHash)[jumpscopeFKA].scopeCoor2;
					}
					if (cd.assemblyFormula.flagBit.type == COOL_M) {
						Intg* changeable_ = cd.assemblyFormula.flagBit.changeable;
						/**
						 * 这一段意味着在可执行的语句中不允许pendchangeable的存在，如果存在则会被替换为unchangeable
						 */

						if (changeable_[0] == A_) {
							changeable_[0] = F_;
						}
						if (changeable_[1] == A_) {
							changeable_[1] = F_;
						}
						if (changeable_[2] == A_) {
							changeable_[2] = F_;
						}
						if (changeable_[3] == A_) {
							changeable_[3] = F_;
						}

					}

			}
		}
		//}

		return true;
	}

	/*************************************

	DecComparator

	********************************/

	DecComparator::DecComparator(Scanner* scanner_) {
		scanner = scanner_;
		codeTableHash = scanner_->codeTableHash;
	}

	DecComparator& DecComparator::clear() {
		am.clear();
		cdm.clear();
		return (*this);
	}

	bool DecComparator::add(const ArgDec& arg, const ArgDec& argtf) {
		if (am[arg] == argtf) {
			return true;
		}
		else if (am[arg] == argDecnull) {
			am[arg] = argtf;
			return true;
		}
		else {
			return false;
		}
	}

	bool DecComparator::add(const FKA& fka1, const FKA& fka2) {
		if (cdm[fka1] == fka2) {
			return true;
		}
		else if (cdm[fka1] == fkanull) {
			cdm[fka1] = fka2;
			return true;
		}
		else {
			return false;
		}
	}

	/**
		 * @attention 不比较非类型标志位
		 * @param cdFKA
		 * @param arg
		 * @return
		 */
	bool DecComparator::includeArg(const FKA& cdFKA, const ArgDec& arg) const {
		return ((*codeTableHash)[cdFKA].assemblyFormula.flagBit.arg1_flag
			== arg.argFlag
			&& (*codeTableHash)[cdFKA].assemblyFormula.quaternion.arg1_fka
			== arg.arg_fka
			&& (*codeTableHash)[cdFKA].assemblyFormula.quaternion.arg1_i
			== arg.arg_i
			&& (*codeTableHash)[cdFKA].assemblyFormula.quaternion.arg1_s
			== arg.arg_s)
			|| ((*codeTableHash)[cdFKA].assemblyFormula.flagBit.arg2_flag
				== arg.argFlag
				&& (*codeTableHash)[cdFKA].assemblyFormula.quaternion.arg2_fka
				== arg.arg_fka
				&& (*codeTableHash)[cdFKA].assemblyFormula.quaternion.arg2_i
				== arg.arg_i
				&& (*codeTableHash)[cdFKA].assemblyFormula.quaternion.arg2_s
				== arg.arg_s)

			|| ((*codeTableHash)[cdFKA].assemblyFormula.flagBit.operator_flag
				== arg.argFlag
				&& (*codeTableHash)[cdFKA].assemblyFormula.quaternion.operator_fka
				== arg.arg_fka
				&& (*codeTableHash)[cdFKA].assemblyFormula.quaternion.operator_i
				== arg.arg_i
				&& (*codeTableHash)[cdFKA].assemblyFormula.quaternion.operator_s
				== arg.arg_s)
			|| ((*codeTableHash)[cdFKA].assemblyFormula.flagBit.result_flag
				== arg.argFlag
				&& (*codeTableHash)[cdFKA].assemblyFormula.quaternion.result_fka
				== arg.arg_fka
				&& (*codeTableHash)[cdFKA].assemblyFormula.quaternion.result_i
				== arg.arg_i
				&& (*codeTableHash)[cdFKA].assemblyFormula.quaternion.result_s
				== arg.arg_s);

	}

	FKA DecComparator::getSameTrBranchLastArgAddr(const FKA& fka, const ArgDec& a) {
		FKA fkaabove = (*codeTableHash).getAboveCodeFKA(fka);
		while ((*codeTableHash)[fkaabove].assemblyFormula.flagBit.type == COOL_M
			|| (*codeTableHash)[fkaabove].assemblyFormula.flagBit.type == COOL_Y
			|| (*codeTableHash)[fkaabove].assemblyFormula.flagBit.execute
			== Du) {
			if ((*codeTableHash)[fkaabove].assemblyFormula.flagBit.type != COOL_Y
				&& includeArg(fkaabove,
					a) && (*codeTableHash)[fkaabove].assemblyFormula.flagBit.execute != Du) {
				return fkaabove;
			}
			fkaabove = (*codeTableHash).getAboveCodeFKA(fkaabove);
		}
		return fkanull;

	}

	bool DecComparator::cmpDecCodeTree(const FKA& tr, const FKA& trTf) {
		if (cdm[tr] == trTf) {
			return true;
		}
		else if (add(tr, trTf) == false) {
			return false;
		}
		ArgDec arg1_1((*codeTableHash)[tr].assemblyFormula.flagBit.arg1_flag,

			(*codeTableHash)[tr].assemblyFormula.quaternion.arg1_fka,
			(*codeTableHash)[tr].assemblyFormula.quaternion.arg1_i,
			(*codeTableHash)[tr].assemblyFormula.quaternion.arg1_s);
		ArgDec arg2_1((*codeTableHash)[trTf].assemblyFormula.flagBit.arg1_flag,

			(*codeTableHash)[trTf].assemblyFormula.quaternion.arg1_fka,
			(*codeTableHash)[trTf].assemblyFormula.quaternion.arg1_i,
			(*codeTableHash)[trTf].assemblyFormula.quaternion.arg1_s);
		ArgDec arg1_2((*codeTableHash)[tr].assemblyFormula.flagBit.arg2_flag,

			(*codeTableHash)[tr].assemblyFormula.quaternion.arg2_fka,
			(*codeTableHash)[tr].assemblyFormula.quaternion.arg2_i,
			(*codeTableHash)[tr].assemblyFormula.quaternion.arg2_s);
		ArgDec arg2_2((*codeTableHash)[trTf].assemblyFormula.flagBit.arg2_flag,

			(*codeTableHash)[trTf].assemblyFormula.quaternion.arg2_fka,
			(*codeTableHash)[trTf].assemblyFormula.quaternion.arg2_i,
			(*codeTableHash)[trTf].assemblyFormula.quaternion.arg2_s);
		ArgDec operator1_1(
			(*codeTableHash)[tr].assemblyFormula.flagBit.operator_flag,

			(*codeTableHash)[tr].assemblyFormula.quaternion.operator_fka,
			(*codeTableHash)[tr].assemblyFormula.quaternion.operator_i,
			(*codeTableHash)[tr].assemblyFormula.quaternion.operator_s);
		ArgDec operator2_1(
			(*codeTableHash)[trTf].assemblyFormula.flagBit.arg1_flag,

			(*codeTableHash)[trTf].assemblyFormula.quaternion.operator_fka,
			(*codeTableHash)[trTf].assemblyFormula.quaternion.operator_i,
			(*codeTableHash)[trTf].assemblyFormula.quaternion.operator_s);
		ArgDec result1_1(
			(*codeTableHash)[tr].assemblyFormula.flagBit.result_flag,

			(*codeTableHash)[tr].assemblyFormula.quaternion.result_fka,
			(*codeTableHash)[tr].assemblyFormula.quaternion.result_i,
			(*codeTableHash)[tr].assemblyFormula.quaternion.result_s);
		ArgDec result2_1(
			(*codeTableHash)[trTf].assemblyFormula.flagBit.result_flag,

			(*codeTableHash)[trTf].assemblyFormula.quaternion.result_fka,
			(*codeTableHash)[trTf].assemblyFormula.quaternion.result_i,
			(*codeTableHash)[trTf].assemblyFormula.quaternion.result_s);
		if (tr != fkanull && trTf != fkanull) {
			if (add(arg1_1, arg2_1) && add(arg1_2, arg2_2)
				&& add(operator1_1, operator2_1)
				&& add(result1_1, result2_1)) {
				return cmpDecCodeTree(getSameTrBranchLastArgAddr(tr, arg1_1),
					getSameTrBranchLastArgAddr(trTf, arg2_1))
					&& cmpDecCodeTree(
						getSameTrBranchLastArgAddr(tr, arg1_2),
						getSameTrBranchLastArgAddr(trTf, arg2_2))
					&& cmpDecCodeTree(
						getSameTrBranchLastArgAddr(tr, operator1_1),
						getSameTrBranchLastArgAddr(trTf, operator2_1))
					&& cmpDecCodeTree(
						getSameTrBranchLastArgAddr(tr, result1_1),
						getSameTrBranchLastArgAddr(trTf, result2_1));
			}
			return false;
		}
		else if (tr == fkanull && trTf == fkanull) {
			return true;
		}
		else {
			return false;
		}

	}

	/************************************

	PendPainter

	**********************************/

	PendPainter::PendPainter(Scanner* s_) {
		scanner = s_;
		codeTableHash = scanner->codeTableHash;
	}
	bool [[deprecated]] PendPainter::painted(const Arg& arg, const FKA& pendFKA) {
		Intg flagBitChangeable = arg.changeable;
		if (flagBitChangeable == T_) {
			return ((*codeTableHash)[pendFKA][0] != arg
				|| ((*codeTableHash)[pendFKA][0] == arg
					&& (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[0]
					== T_))
				&& ((*codeTableHash)[pendFKA][1] != arg
					|| ((*codeTableHash)[pendFKA][1] == arg
						&& (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[1]
						== T_))
				&& ((*codeTableHash)[pendFKA][2] != arg
					|| ((*codeTableHash)[pendFKA][2] == arg
						&& (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[2]
						== T_))
				&& ((*codeTableHash)[pendFKA][3] != arg
					|| ((*codeTableHash)[pendFKA][3] == arg
						&& (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3]
						== T_));
		}
		else if (flagBitChangeable == A_) {
			return ((*codeTableHash)[pendFKA][0] != arg
				|| ((*codeTableHash)[pendFKA][0] == arg
					&& ((*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[0]
						== A_
						|| (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[0]
						== T_)))
				&& ((*codeTableHash)[pendFKA][1] != arg
					|| ((*codeTableHash)[pendFKA][1] == arg
						&& ((*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[1]
							== A_
							|| (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[1]
							== T_)))
				&& ((*codeTableHash)[pendFKA][2] != arg
					|| ((*codeTableHash)[pendFKA][2] == arg
						&& ((*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[2]
							== A_
							|| (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[2]
							== T_)))
				&& ((*codeTableHash)[pendFKA][3] != arg
					|| ((*codeTableHash)[pendFKA][3] == arg
						&& ((*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3]
							== A_
							|| (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3]
							== T_)));
		}
		else if (flagBitChangeable == F_) {
			return ((*codeTableHash)[pendFKA][0] != arg
				|| ((*codeTableHash)[pendFKA][0] == arg
					&& (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[0]
					== F_))
				&& ((*codeTableHash)[pendFKA][1] != arg
					|| ((*codeTableHash)[pendFKA][1] == arg
						&& (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[1]
						== F_))
				&& ((*codeTableHash)[pendFKA][2] != arg
					|| ((*codeTableHash)[pendFKA][2] == arg
						&& (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[2]
						== F_))
				&& ((*codeTableHash)[pendFKA][3] != arg
					|| ((*codeTableHash)[pendFKA][3] == arg
						&& (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3]
						== F_));
		}
		{

			static int ignorecount = 0;
			cerr << "painted(const Arg&, const FKA&)" << " exit(-1)"
				<< "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			exit(-1);
		}

	}
	bool [[deprecated]] PendPainter::paint(const Arg& arg, const FKA& pendFKA) {
		Intg flagBitChangeable = arg.changeable;
		if (pendFKA == fkanull || arg == argnull) {
#if debug
			cout << "PendPainter::paint 1" << endl;
#endif
			return false;
		}
		//deprecated
		//		if (painted(arg, pendFKA) == true) {
		//#if debug
		//			cout << "PendPainter::paint 2 painted==true" << endl;
		//#endif
		//			return true;
		//		}
		else {
			Intg paintArgResultFlag = false;
			if (flagBitChangeable == T_) {
				if ((*codeTableHash)[pendFKA][0] == arg) {
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[0] =
						T_;
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3] =
						T_;
					paintArgResultFlag = true;

				}
				if ((*codeTableHash)[pendFKA][1] == arg) {
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[1] =
						T_;
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3] =
						T_;
					paintArgResultFlag = true;
				}
				if ((*codeTableHash)[pendFKA][2] == arg) {
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[2] =
						T_;
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3] =
						T_;
				}
				if ((*codeTableHash)[pendFKA][3] == arg) {
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3] =
						T_;
					paintArgResultFlag = true;
				}

			}
			else if (flagBitChangeable == A_) {

				if ((*codeTableHash)[pendFKA][0] == arg) {
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[0] =
						A_;
					if ((*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3]
						!=
						T_) {
						(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3] =
							A_;
					}
					paintArgResultFlag = true;
				}
				if ((*codeTableHash)[pendFKA][1] == arg) {
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[1] =
						A_;
					if ((*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3]
						!=
						T_) {
						(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3] =
							A_;
					}
					paintArgResultFlag = true;
				}
				if ((*codeTableHash)[pendFKA][2] == arg) {
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[2] =
						A_;
					if ((*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3]
						!=
						T_) {
						(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3] =
							A_;
					}
				}
				if ((*codeTableHash)[pendFKA][3] == arg) {
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3] =
						A_;
					paintArgResultFlag = true;
				}
				if ((*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[0]
					== T_
					|| (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[1]
					== T_
					|| (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[2]
					== T_
					|| (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3]
					== T_) {
					paintArgResultFlag = false;
				}
			}
			else if (flagBitChangeable == F_) {
				if ((*codeTableHash)[pendFKA][0] == arg) {
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[0] =
						F_;

				}
				if ((*codeTableHash)[pendFKA][1] == arg) {
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[1] =
						F_;
				}
				if ((*codeTableHash)[pendFKA][2] == arg) {
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[2] =
						F_;
				}
				if ((*codeTableHash)[pendFKA][3] == arg) {
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3] =
						F_;
					paintArgResultFlag = true;
				}
				if ((*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[0]
					== F_
					&& (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[1]
					== F_
					&& (*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[2]
					== F_) {
					(*codeTableHash)[pendFKA].assemblyFormula.flagBit.changeable[3] =
						F_;
					paintArgResultFlag = true;
				}
				else {
					paintArgResultFlag = false;
				}
			}
			if (paintArgResultFlag == true) {
				Arg argResult = (*codeTableHash)[pendFKA][3];
				FKA nextfka = codeTableHash->getSameTrBranchNextArgFKA(pendFKA,
					argResult, acm::name);
				paint(argResult, nextfka);
			}

			paint(arg, codeTableHash->getSameTrBranchNextArgFKA(pendFKA, arg, acm::name));

			return true;
		}

	}
	bool PendPainter::paintAll() {
		CodeTable cdt;
		if (scanner->pendM.size() != 0) {
			cdt = get<0>(codeTableHash->copyTree(scanner->pendM.back()));
		}
		cdt.checkChangeable();

#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			stringstream ss;
			ss << bbhline;
			ss << __FUNCTION__ << "\n";
			ss << "checked snippet:" << "\n";
			ss << cdt.toStrg();
			ss << bbhline;
			std::string info = ss.str(); // Replace with actual info
			info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
			std::cout << info << std::endl;
			std::wstring winfo(info.begin(), info.end());
			OutputDebugString(winfo.c_str());
			ignoreCount++;
		}
#endif

		for (auto& ad : cdt.addrdq) {
			auto& cdref = cdt[ad];
			auto& cdreal = (*codeTableHash)[FKA(cdt.fileKey, ad)];
			for (Intg i = 0; i < 4; i++)
			{
				cdreal.assemblyFormula.flagBit.changeable[i] =
					cdref.assemblyFormula.flagBit.changeable[i];
			}
		}
		while (scanner->pendM.size() != 0) {

			FKA fka = scanner->pendM.back();
			scanner->pendM.pop_back();
			changeableArgs.insert((*codeTableHash)[fka][0]);

			(*codeTableHash)[fka].assemblyFormula.flagBit.execute = Du;
		}

		scanner->pendM.clear();
		return true;

	}

	/******************************************

	RealPainter

	********************************/

	RealPainter::RealPainter(Scanner* s_) {
		scanner = s_;
		codeTableHash = scanner->codeTableHash;
	}
	bool RealPainter::painted(const Arg& arg, const FKA& realFKA) {
		return ((*codeTableHash)[realFKA][0] != arg
			|| ((*codeTableHash)[realFKA][0] == arg
				&& (*codeTableHash)[realFKA].assemblyFormula.flagBit.formalArgFlag[0]
				== F_))
			&& ((*codeTableHash)[realFKA][1] != arg
				|| ((*codeTableHash)[realFKA][1] == arg
					&& (*codeTableHash)[realFKA].assemblyFormula.flagBit.formalArgFlag[1]
					== F_))
			&& ((*codeTableHash)[realFKA][2] != arg
				|| ((*codeTableHash)[realFKA][2] == arg
					&& (*codeTableHash)[realFKA].assemblyFormula.flagBit.formalArgFlag[2]
					== F_))
			&& ((*codeTableHash)[realFKA][3] != arg
				|| ((*codeTableHash)[realFKA][3] == arg
					&& (*codeTableHash)[realFKA].assemblyFormula.flagBit.formalArgFlag[3]
					== F_));
	}

	bool RealPainter::paint(const Arg& arg, const FKA& realFKA) {
		if (realFKA == fkanull) {
			return false;
		}

		//		if (painted(arg, realFKA) == true || realFKA == fkanull) {
		//#if debug
		//			cout << 11.5 << endl;
		//#endif
		//			return true;
		//		}
		else {
			if ((*codeTableHash)[realFKA][0] == arg) {
				(*codeTableHash)[realFKA].assemblyFormula.flagBit.formalArgFlag[0] =
					F_;
			}
			if ((*codeTableHash)[realFKA][1] == arg) {
				(*codeTableHash)[realFKA].assemblyFormula.flagBit.formalArgFlag[1] =
					F_;
			}
			if ((*codeTableHash)[realFKA][2] == arg) {
				(*codeTableHash)[realFKA].assemblyFormula.flagBit.formalArgFlag[2] =
					F_;
			}
			if ((*codeTableHash)[realFKA][3] == arg) {
				(*codeTableHash)[realFKA].assemblyFormula.flagBit.formalArgFlag[3] =
					F_;
			}
			FKA nextfka = codeTableHash->getSameTrBranchNextArgFKA(realFKA,
				arg, acm::name);
			if (nextfka != fkanull) {

				paint(arg, nextfka);
			}

			return true;
		}

	}
	bool RealPainter::paintAll() {
		{
			CodeTable cdt;
			if (scanner->realM.size() != 0) {
				cdt = get<0>(codeTableHash->copyTree(scanner->realM.back()));
			}
			cdt.checkReal();

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				stringstream ss;
				ss << bbhline;
				ss << __FUNCTION__ << "\n";
				ss << "checked snippet:" << "\n";
				ss << cdt.toStrg();
				ss << bbhline;
				std::string info = ss.str(); // Replace with actual info
				info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
				std::cout << info << std::endl;
				std::wstring winfo(info.begin(), info.end());
				OutputDebugString(winfo.c_str());
				ignoreCount++;
			}
#endif

			for (auto& ad : cdt.addrdq) {
				auto& cdref = cdt[ad];
				auto& cdthis = (*codeTableHash)[FKA(cdt.fileKey, ad)];
				for (Intg i = 0; i < 4; i++)
				{
					cdthis.assemblyFormula.flagBit.formalArgFlag[i] =
						cdref.assemblyFormula.flagBit.formalArgFlag[i];
				}
			}
			while (scanner->realM.size() != 0) {

				FKA fka = scanner->realM.back();
				scanner->realM.pop_back();
				realArgs.insert((*codeTableHash)[fka][0]);

				(*codeTableHash)[fka].assemblyFormula.flagBit.execute = Du;
			}

			scanner->realM.clear();
			return true;

		}

	}


	/*****************************************

	CompatiblePainter

	***********************************/

	CompatiblePainter::CompatiblePainter(Scanner* s_) {
		scanner = s_;
		codeTableHash = scanner->codeTableHash;
	}

	bool CompatiblePainter::painted(const Arg& arg, const FKA& compatibleFKA) {
		return ((*codeTableHash)[compatibleFKA][0] != arg
			|| ((*codeTableHash)[compatibleFKA][0] == arg
				&& (*codeTableHash)[compatibleFKA].assemblyFormula.flagBit.compatible[0]
				== F_))
			&& ((*codeTableHash)[compatibleFKA][1] != arg
				|| ((*codeTableHash)[compatibleFKA][1] == arg
					&& (*codeTableHash)[compatibleFKA].assemblyFormula.flagBit.compatible[1]
					== F_))
			&& ((*codeTableHash)[compatibleFKA][2] != arg
				|| ((*codeTableHash)[compatibleFKA][2] == arg
					&& (*codeTableHash)[compatibleFKA].assemblyFormula.flagBit.compatible[2]
					== F_))
			&& ((*codeTableHash)[compatibleFKA][3] != arg
				|| ((*codeTableHash)[compatibleFKA][3] == arg
					&& (*codeTableHash)[compatibleFKA].assemblyFormula.flagBit.compatible[3]
					== F_));
	}

	bool CompatiblePainter::paint(const Arg& arg, const FKA& compatibleFKA) {
		if (compatibleFKA == fkanull) {
			return false;
		}

		//		if (painted(arg, compatibleFKA) == true || compatibleFKA == fkanull) {
		//#if debug
		//			cout << 11.5 << endl;
		//#endif
		//			return true;
		//		}
		else {
			if ((*codeTableHash)[compatibleFKA][0] == arg) {
				(*codeTableHash)[compatibleFKA].assemblyFormula.flagBit.compatible[0] =
					F_;
			}
			if ((*codeTableHash)[compatibleFKA][1] == arg) {
				(*codeTableHash)[compatibleFKA].assemblyFormula.flagBit.compatible[1] =
					F_;
			}
			if ((*codeTableHash)[compatibleFKA][2] == arg) {
				(*codeTableHash)[compatibleFKA].assemblyFormula.flagBit.compatible[2] =
					F_;
			}
			if ((*codeTableHash)[compatibleFKA][3] == arg) {
				(*codeTableHash)[compatibleFKA].assemblyFormula.flagBit.compatible[3] =
					F_;
			}
			FKA nextfka = codeTableHash->getSameTrBranchNextArgFKA(compatibleFKA,
				arg, acm::name);
			if (nextfka != fkanull) {

				paint(arg, nextfka);
			}

			return true;
		}

	}

	bool CompatiblePainter::paintAll() {
		CodeTable cdt;
		if (scanner->compatibleM.size() != 0) {
			cdt = get<0>(codeTableHash->copyTree(scanner->compatibleM.back()));
		}
		cdt.checkCompatible();

#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			stringstream ss;
			ss << bbhline;
			ss << __FUNCTION__ << "\n";
			ss << "checked snippet:" << "\n";
			ss << cdt.toStrg();
			ss << bbhline;
			std::string info = ss.str(); // Replace with actual info
			info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
			std::cout << info << std::endl;
			std::wstring winfo(info.begin(), info.end());
			OutputDebugString(winfo.c_str());
			ignoreCount++;
		}
#endif

		for (auto& ad : cdt.addrdq) {
			auto& cdref = cdt[ad];
			auto& cdthis = (*codeTableHash)[FKA(cdt.fileKey, ad)];
			for (Intg i = 0; i < 4; i++)
			{
				cdthis.assemblyFormula.flagBit.compatible[i] =
					cdref.assemblyFormula.flagBit.compatible[i];
			}
		}
		while (scanner->compatibleM.size() != 0) {

			FKA fka = scanner->compatibleM.back();
			scanner->compatibleM.pop_back();
			compatibleArgs.insert((*codeTableHash)[fka][0]);

			(*codeTableHash)[fka].assemblyFormula.flagBit.execute = Du;
		}

		scanner->compatibleM.clear();
		return true;
	}

	/*****************************************************

	ConstraintPainter

	**********************************************/
	ConstraintPainter::ConstraintPainter(Scanner* s_) {
		scanner = s_;
		codeTableHash = scanner->codeTableHash;
	}


	void ConstraintPainter::extractConstraint(const FKA& constraintFKA) {

		Constraint c;
		Code& base = (*codeTableHash)[constraintFKA];
		if (base.assemblyFormula.flagBit.type == COOL_M) {
			base.assemblyFormula.flagBit.execute = Du;
		}
		Arg&& arg = base[0];
		Arg&& attr = base[1];
		if (attr.argFlag == S_Bs) {
			c.instruction = attr.arg_s;
			if (arg_constr_map.count(arg) > 0) {
				arg_constr_map[arg].insert(c);
			}
			else {
				arg_constr_map[arg] = set<Constraint>();
				arg_constr_map[arg].insert(c);
			}
		}
		else if (attr.argFlag == S_Dz || attr.argFlag == X_Dz) {
			FKA&& constrfka = (*codeTableHash).getSameTrBranchLastArgFKA(constraintFKA, attr, acm::name);

			Code& cd = (*codeTableHash)[constrfka];
			if (cd.assemblyFormula.flagBit.type == COOL_M) {
				cd.assemblyFormula.flagBit.execute = Du;
			}
			Arg&& arg1 = cd[0];
			Arg&& arg2 = cd[1];
			Arg&& argop = cd[3];
			Arg&& argres = cd[4];
			if (argop.argFlag == M_Bs && argop.arg_s == "attr_call") {
				if (arg1.argFlag == M_Bs || arg1.argFlag == X_Bs) {
					c.instruction = arg1.arg_s;
					if (arg2.isDz() == false) {

						c.args.push_front(arg2.toConstraintArg());

					}
					else {
						FKA argfka = (*codeTableHash).getSameTrBranchLastArgFKA(constraintFKA, attr, acm::name);
						Code* cd_ = &(*codeTableHash)[argfka];
						if (cd_->assemblyFormula.flagBit.type == COOL_M) {
							cd_->assemblyFormula.flagBit.execute = Du;
						}
						arg1 = (*cd_)[0];
						arg2 = (*cd_)[1];
						argop = (*cd_)[3];
						argres = (*cd_)[4];
						while (arg1.isDz() == true) {

							if (cd_->assemblyFormula.flagBit.type == COOL_M) {
								cd_->assemblyFormula.flagBit.execute = Du;
							}
							arg1 = (*cd_)[0];
							arg2 = (*cd_)[1];
							argop = (*cd_)[3];
							argres = (*cd_)[4];

							if ((argop.argFlag == M_Bs || argop.argFlag == X_Bs) && argop.arg_s == "COMMA") {
								if (arg2.isDz() == false) {

									c.args.push_front(arg2.toConstraintArg());

								}
								else {

#if debug && _WIN32
									{
										static int ignoreCount = 0;
										// Assuming info is a std::string, convert to std::wstring
										std::string info = "Unrecognized Constraint, exit(-1)"; // Replace with actual info
										info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
										std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
										OutputDebugString(winfo.c_str());
#endif
										cout << info << endl;
										ignoreCount++;
									}
#endif
									cout << "Unrecognized Constraint, exit(-1)" << endl;
									exit(-1);

								}

								if (arg1.isDz() == false) {

									c.args.push_front(arg1.toConstraintArg());

								}
								else {

#if debug && _WIN32
									{
										static int ignoreCount = 0;
										// Assuming info is a std::string, convert to std::wstring
										std::string info = "Unrecognized Constraint, exit(-1)"; // Replace with actual info
										info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
										std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
										OutputDebugString(winfo.c_str());
#endif
										cout << info << endl;
										ignoreCount++;
									}
#endif
									cout << "Unrecognized Constraint, exit(-1)" << endl;
									exit(-1);

								}

							}
							argfka = (*codeTableHash).getSameTrBranchLastArgFKA(constraintFKA, attr, acm::name);
							if (argfka == fkanull) {
								break;
							}
							cd_ = &(*codeTableHash)[argfka];

						}

					}
				}
			}


		}


	}
	bool ConstraintPainter::painted(const Arg& arg, const FKA& constraintFKA) {
		return ((*codeTableHash)[constraintFKA][0] != arg
			|| ((*codeTableHash)[constraintFKA][0] == arg
				&& (*codeTableHash)[constraintFKA].assemblyFormula.flagBit.arg1_constraints.size() != 0))
			&& ((*codeTableHash)[constraintFKA][1] != arg
				|| ((*codeTableHash)[constraintFKA][1] == arg
					&& (*codeTableHash)[constraintFKA].assemblyFormula.flagBit.arg2_constraints.size() != 0))
			&& ((*codeTableHash)[constraintFKA][2] != arg
				|| ((*codeTableHash)[constraintFKA][2] == arg
					&& (*codeTableHash)[constraintFKA].assemblyFormula.flagBit.operator_constraints.size() != 0))
			&& ((*codeTableHash)[constraintFKA][3] != arg
				|| ((*codeTableHash)[constraintFKA][3] == arg
					&& (*codeTableHash)[constraintFKA].assemblyFormula.flagBit.result_constraints.size() != 0));
	}

	bool ConstraintPainter::paint(const Arg& arg, const FKA& constraintFKA) {
		if (constraintFKA == fkanull) {
			return false;
		}

		/*if (painted(arg, constraintFKA) == true) {

			return true;
		}*/
		else {
			Arg arg_ = arg;
			arg_.expandConstraints(this->scanner->currentAR, this->scanner->systemTableHash);
			if ((*codeTableHash)[constraintFKA][0] == arg) {
				(*codeTableHash)[constraintFKA].assemblyFormula.flagBit.arg1_constraints =
					arg_.constraints;
			}
			if ((*codeTableHash)[constraintFKA][1] == arg) {
				(*codeTableHash)[constraintFKA].assemblyFormula.flagBit.arg2_constraints =
					arg_.constraints;
			}
			if ((*codeTableHash)[constraintFKA][2] == arg) {
				(*codeTableHash)[constraintFKA].assemblyFormula.flagBit.operator_constraints =
					arg_.constraints;
			}
			if ((*codeTableHash)[constraintFKA][3] == arg) {
				(*codeTableHash)[constraintFKA].assemblyFormula.flagBit.result_constraints =
					arg_.constraints;
			}
			FKA nextfka = codeTableHash->getSameTrBranchNextArgFKA(constraintFKA,
				arg, acm::name);
			if (nextfka != fkanull) {

				paint(arg, nextfka);
			}

			return true;
		}

	}

	bool ConstraintPainter::paintAll() {
		CodeTable cdt;
		if (scanner->constraintM.size() != 0) {
			cdt = get<0>(codeTableHash->copyTree(scanner->constraintM.back()));
		}
		else {
			cdt = get<0>(codeTableHash->copyTree(scanner->currentCodeFKA));

		}
		auto&& addrs_Du = cdt.checkConstraints(true);

#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			stringstream ss;
			ss << bbhline;
			ss << __FUNCTION__ << "\n";
			ss << "checked snippet:" << "\n";
			ss << cdt.toStrg();
			ss << bbhline;
			std::string info = ss.str(); // Replace with actual info
			info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
			std::cout << info << std::endl;
			std::wstring winfo(info.begin(), info.end());
			OutputDebugString(winfo.c_str());
			ignoreCount++;
		}
#endif

		for (auto& ad : cdt.addrdq) {
			auto& cdref = cdt[ad];
			auto& cdthis = (*codeTableHash)[FKA(cdt.fileKey, ad)];

			cdthis.assemblyFormula.flagBit.arg1_constraints =
				cdref.assemblyFormula.flagBit.arg1_constraints;
			cdthis.assemblyFormula.flagBit.arg2_constraints =
				cdref.assemblyFormula.flagBit.arg2_constraints;
			cdthis.assemblyFormula.flagBit.operator_constraints =
				cdref.assemblyFormula.flagBit.operator_constraints;
			cdthis.assemblyFormula.flagBit.result_constraints =
				cdref.assemblyFormula.flagBit.result_constraints;
		}



		for (auto addrdu : addrs_Du) {
			auto& cdthis = (*codeTableHash)[FKA(cdt.fileKey, addrdu)];

			if (cdthis.assemblyFormula.flagBit.type == COOL_M) {
				cdthis.assemblyFormula.flagBit.execute = Du;
			}

		}
		scanner->constraintM.clear();

		return true;
	}

}