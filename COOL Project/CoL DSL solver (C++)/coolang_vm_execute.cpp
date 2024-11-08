#include"coolang_vm_execute.hpp"
using namespace std;
namespace COOLANG {
	/**
		 * @brief judge whether an argReal and an argFormal is the same one arg.
		 * if two args with same names are at the same one scope, they are the same.
		 * @param argreal
		 * @param argformal
		 * @return
		 */
	bool FunctionHandleMatcher::sameOneArg(const ArgReal& argreal, const ArgFormal& argformal) {
#if debug
		cout << "FunctionHandleMatcher::sameOneArg argreal:["
			<< argreal.toStrg() << "]\targformal:[" << argformal.toStrg()
			<< "]\t" << endl;
#endif
		if (/*argformal.formalArg == argreal.formalArg
		 &&*/argreal.asc == argformal.asc && argreal == argformal) {
			return true;
		}
		return false;
	}

	FunctionHandleMatcher::FunctionHandleMatcher(CodeTable* const cdtreal,
		CodeTable* const cdtformal) {
		this->cdtreal = cdtreal;

		this->cdtformal = cdtformal;

	}
	const bool FunctionHandleMatcher::add(const ArgReal& argreal, const ArgFormal& argformal) {
#if debug
		cout << "FunctionHandleMatcher::add \n\targreal:[" << argreal.toStrg()
			<< "]\n\targformal:[" << argformal.toStrg() << "]" << endl;
#endif
		if (argformal.changeable != A_
			&& argreal.changeable != argformal.changeable) {
#if debug
			cout << "FunctionHandleMatcher::add false1\n\targreal:["
				<< argreal.toStrg() << "]\n\targformal:["
				<< argformal.toStrg() << "]" << endl;
#endif
			return false;
		}
		if (argreal == argnull && argformal == argnull) {
			return true;
		}

		if (am[argreal].size() == 0) {
			am[argreal].push_back(Arg());
		}

		if (argreal.argFlag == Sz) {
			if (argreal.changeable == true) {
#if debug
				cerr
					<< "FunctionHandleMatcher::add err, Sz & changeable==true ,exit -1"
					<< endl;
#endif
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
				assert(false); exit(-1);
			}
			if (argformal.changeable == true) {
#if debug
				cout << "FunctionHandleMatcher::add false2\n\targreal:["
					<< argreal.toStrg() << "]\n\targformal:["
					<< argformal.toStrg() << "]" << endl;
#endif
				return false;
			}
			if (argformal.argFlag == Sz) {
				return argreal == argformal;
			}
			else if (argformal.argFlag == S_Dz || argformal.argFlag == S_Bs) {
				for (const ArgFormal& a : am[argreal]) {
					if (a == argformal) {
						return true;
					}
				}
				if (am[argreal][0] == argnull) {
					am[argreal][0] = argformal;
				}
				else {
					am[argreal].push_back(argformal);
				}
				return true;
			}

		}

		if (argreal.argFlag == X_Bs) {
#if debug
			cout
				<< "FunctionHandleMatcher::add compare SHUXING,jump and return true. argreal:["
				<< argreal.toStrg() << "]\targformal:[" << argreal.toStrg()
				<< "]" << endl;
#endif
			return true;
		}

		if (argreal.argFlag == M_Bs) {
			return argreal == argformal;
		}
		if (am[argreal][0] == argformal) {
			return true;
		}
		else if (argformal.formalArg == true && am[argreal][0] == argnull) {
			am[argreal][0] = argformal;
			return true;
		}
		else if (argformal.formalArg == false
			&& sameOneArg(argreal, argformal) == true) {
			am[argreal][0] = argformal;
			return true;
		}
		else {
#if debug
			cout << "FunctionHandleMatcher::add false3\n\targreal:["
				<< argreal.toStrg() << "]\n\targformal:["
				<< argformal.toStrg() << "]" << endl;
#endif
			return false;
		}
	}



	const bool FunctionHandleMatcher::add(const AddrReal& adreal, const AddrFormal& adformal) {
#if debug
		cout << "FunctionHandleMatcher::add(addr,addr) adreal:["
			<< adreal.toStrg() << "]\tadformal:[" << adformal.toStrg()
			<< "]\t" << endl;
#endif
		if (cdm[adreal] == adformal) {
			return true;
		}
		else if (cdm[adreal] == addrnull) {
			cdm[adreal] = adformal;
			return true;
		}
		else {
			return false;
		}
	}
	/// <summary>
	/// this function takes asc into consideration. 
	/// </summary>
	/// <param name="codeTable"></param>
	/// <param name="ad"></param>
	/// <param name="a"></param>
	/// <returns></returns>
	const Addr FunctionHandleMatcher::getSameTrBranchLastArgAddr(CodeTable* codeTable, const Addr& ad,
		const Arg& a) {
#if debug
		cout << "FunctionHandleMatcher::getSameTrBranchLastArgAddr ad:["
			<< ad.toStrg() << "]\ta:[" << a.toStrg() << "]\t" << endl;
#endif

		acm mode = acm::asc | acm::name;
		if (a.argFlag == M_Bs) {
			return addrnull;
		}
		Addr addr = (*codeTable).getAboveCodeAddr(ad);
		while ((*codeTable)[addr].assemblyFormula.flagBit.type == COOL_M
			|| (*codeTable)[addr].assemblyFormula.flagBit.execute == Du) {
			if ((*codeTable)[addr].includeArg(a, mode)
				&& (*codeTable)[addr].assemblyFormula.flagBit.execute
				!= Du) {
				return addr;
			}
			addr = (*codeTable).getAboveCodeAddr(addr);
		}
		return Addr();

	}
	const bool FunctionHandleMatcher::reset(CodeTable* cdreal, CodeTable* cdformal) {
		this->cdtreal = cdtreal;
		this->cdtformal = cdtformal;
		am.clear();
		cdm.clear();
		return true;
	}
	//	const bool FunctionHandleMatcher::match(const AddrReal& adreal, const AddrFormal& adformal) {
	//#if debug
	//		cout << "FunctionHandleMatcher::match adreal:[" << adreal.toStrg()
	//			<< "]\tadformal:[" << adformal.toStrg() << "]\t" << endl;
	//#endif
	//		if (adformal == addrnull) {
	//			getFRMap();
	//			return true;
	//		}
	//		if (cdm.find(adreal) != cdm.end()) {
	//			if (cdm[adreal] == adformal) {
	//				getFRMap();
	//				return true;
	//			}
	//			else {
	//#if debug
	//				cerr << "FunctionHandleMatcher::match err,adreal:["
	//					<< adreal.toStrg() << "]\tadformal:["
	//					<< adformal.toStrg() << "]" << endl;
	//#endif
	//			}
	//
	//		}
	//		else if (add(adreal, adformal) == false) {
	//#if debug
	//			cout << "FunctionHandleMatcher::match err,adreal:["
	//				<< adreal.toStrg() << "]\tadformal:[" << adformal.toStrg()
	//				<< "]" << endl;
	//#endif
	//			return false;
	//		}
	//		ArgReal argreal1 = (*cdtreal)[adreal][0];
	//		ArgReal argreal2 = (*cdtreal)[adreal][1];
	//		ArgReal operatorreal = (*cdtreal)[adreal][2];
	//		ArgReal resultreal = (*cdtreal)[adreal][3];
	//
	//		if (adformal < addrnull) {
	//#if debug
	//			cout << "FunctionHandleMatcher::match adformal>addrnull,adreal:["
	//				<< adreal.toStrg() << "]\tadformal:[" << adformal.toStrg()
	//				<< "]" << endl;
	//#endif
	//			switch (adformal[0]) {
	//			case S_S_JIA:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "+");
	//				break;
	//			case S_S_JIAN:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "-");
	//				break;
	//			case S_S_CHENG:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "*");
	//				break;
	//			case S_S_CHU:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "/");
	//				break;
	//			case S_S_FUZHI:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "=");
	//				break;
	//			case S_S_DENGYU:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "==");
	//				break;
	//			case S_S_BUDENG:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "!=");
	//				break;
	//			case S_S_DAYU:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == ">");
	//				break;
	//			case S_S_XIAOYU:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "<");
	//				break;
	//			case S_S_BUDAYU:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "<=");
	//				break;
	//			case S_S_BUXIAOYU:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == ">=");
	//				break;
	//			case S_S_MI:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "^");
	//				break;
	//			case CALL:
	//				return ((argreal1.argFlag == S_Bs || argreal1.argFlag == Zf)
	//					&& (argreal2.argFlag == S_Dz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == Sz
	//						|| argreal2.argFlag == Zf)
	//					&& (operatorreal.arg_s == "call"
	//						|| operatorreal.arg_s == "ca"));
	//				break;
	//			case SHUCHU:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "-->");
	//				break;
	//			case LST_ACCESS:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "[]");
	//				break;
	//			case LENGTH:
	//				return (argreal1.argFlag == S_Bs || argreal1.argFlag == S_Dz)
	//
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "LENGTH");
	//				break;
	//			case CLEAR:
	//				return (argreal1.argFlag == S_Bs || argreal1.argFlag == S_Dz)
	//
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "CLEAR");
	//				break;
	//			case ERASE:
	//				return (argreal1.argFlag == S_Bs || argreal1.argFlag == S_Dz)
	//					&& (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
	//						|| argreal2.argFlag == S_Dz)
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "ERASE");
	//				break;
	//			case TYPENAME:
	//				return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
	//					|| argreal1.argFlag == S_Dz)
	//
	//					&& (operatorreal.argFlag == M_Bs
	//						&& operatorreal.arg_s == "TYPENAME");
	//				break;
	//			case S_S_AND:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "&&")
	//					&& (argreal1.changeable != T_
	//						&& argreal2.changeable != T_);
	//				break;
	//			case S_S_OR:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "||")
	//					&& (argreal1.changeable != T_
	//						&& argreal2.changeable != T_);
	//				break;
	//			case S_S_NOT:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "!"
	//					&& argreal2.changeable != T_);
	//				break;
	//			case S_S_MODULO:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "%")
	//					&& (argreal1.changeable != T_
	//						&& argreal2.changeable != T_);
	//				break;
	//			case S_S_JIADENG:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "+=")
	//					&& (argreal1.changeable != T_
	//						&& argreal2.changeable != T_);
	//				break;
	//			case S_S_JIANDENG:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "-=")
	//					&& (argreal1.changeable != T_
	//						&& argreal2.changeable != T_);
	//				break;
	//			case S_S_CHENGDENG:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "*=")
	//					&& (argreal1.changeable != T_
	//						&& argreal2.changeable != T_);
	//				break;
	//			case S_S_CHUDENG:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "/=")
	//					&& (argreal1.changeable != T_
	//						&& argreal2.changeable != T_);
	//				break;
	//			case S_S_MODENG:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "%=")
	//					&& (argreal1.changeable != T_
	//						&& argreal2.changeable != T_);
	//				break;
	//			case S_S_MIDENG:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "^=")
	//					&& (argreal1.changeable != T_
	//						&& argreal2.changeable != T_);
	//				break;
	//			case S_SI:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "++"
	//					&& argreal1.changeable != T_);
	//				break;
	//			case S_SD:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "--"
	//					&& argreal1.changeable != T_);
	//				break;
	//			case S_MAP:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "MAP");
	//				break;
	//			case S_MULTIMAP:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "MULTIMAP");
	//				break;
	//			case S_SET:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "SET");
	//				break;
	//			case S_MULTISET:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "MULTISET");
	//				break;
	//			case S_TONUM:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "TONUM"
	//					&& argreal1.changeable != T_);
	//				break;
	//			case S_TOSTRG:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "TOSTRG"
	//					&& argreal1.changeable != T_);
	//				break;
	//			case S_TOINT:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "TOINT"
	//					&& argreal1.changeable != T_);
	//				break;
	//			case S_NONBLOCKEXECUTE:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "NONBLOCKEXECUTE"
	//					&& argreal1.changeable != T_);
	//				break;
	//			case S_BLOCKEXECUTE:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "BLOCKEXECUTE"
	//					&& argreal1.changeable != T_);
	//				break;
	//			case S_SLEEP:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "SLEEP"
	//					&& argreal1.changeable != T_);
	//				break;
	//			case S_FIND:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "FIND"
	//					&& argreal1.changeable != T_);
	//				break;
	//			case S_COUNT:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "COUNT"
	//					&& argreal1.changeable != T_);
	//				break;
	//			case S_INSERT:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "INSERT"
	//					&& (argreal1.changeable != T_
	//						&& argreal2.changeable != T_));
	//				break;
	//			case S_PUSHBACK:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "PUSHBACK"
	//					&& (argreal1.changeable != T_
	//						&& argreal2.changeable != T_));
	//				break;
	//			case S_PUSHFRONT:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "PUSHFRONT"
	//					&& (argreal1.changeable != T_
	//						&& argreal2.changeable != T_));
	//				break;
	//			case S_POPBACK:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "POPBACK"
	//					&& argreal1.changeable != T_);
	//				break;
	//			case S_POPFRONT:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "POPFRONT"
	//					&& argreal1.changeable != T_);
	//				break;
	//			case S_BACK:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "BACK"
	//					&& argreal1.changeable != T_);
	//				break;
	//			case SI_S:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "++"
	//					&& argreal2.changeable != T_);
	//				break;
	//			case SD_S:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "--"
	//					&& argreal2.changeable != T_);
	//				break;
	//			case S_S_FIND_SUBEXPR:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "FIND_SUBEXPR");
	//				break;
	//			case S_S_EXIST_SUBEXPR:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "EXIST_SUBEXPR");
	//				break;
	//			case S_RESET:
	//				return (operatorreal.argFlag == M_Bs
	//					&& operatorreal.arg_s == "RESET");
	//				break;
	//
	//				//case COMMA:
	//				//	/**todo::如果要支持多态，请在预编译时对函数名称进行变形
	//				//	 * 将逗号表达式纳入匹配项会导致原来对多态的自动支持不可用
	//				//	 */
	//				//	return (operatorreal.argFlag == M_Bs
	//				//		&& operatorreal.arg_s == "COMMA")
	//				//		/*&& (argreal1.changeable != T_
	//				//		 && argreal2.changeable != T_)*/
	//				//		;
	//				//	break;
	//			default:
	//				break;
	//			}
	//
	//		}
	//		ArgReal argformal1 = (*cdtformal)[adformal][0];
	//		ArgReal argformal2 = (*cdtformal)[adformal][1];
	//		ArgReal operatorformal = (*cdtformal)[adformal][2];
	//		ArgReal resultformal = (*cdtformal)[adformal][3];
	//
	//		if (adreal != addrnull && adformal != addrnull) {
	//			bool add1 = add(argreal1, argformal1);
	//			if (add1 == false) {
	//#if debug
	//				cout << "add1false:adreal:[" << adreal.toStrg()
	//					<< "]\tadformal:[" << adformal.toStrg() << "]"
	//					<< "\n\targreal1:[" << argreal1.toStrg() << "]"
	//					<< "\n\targformal1:[" << argformal1.toStrg() << "]"
	//
	//					<< endl;
	//#endif
	//			}
	//
	//			bool add2 = add(argreal2, argformal2);
	//			if (add2 == false) {
	//#if debug
	//				cout << "add2false:adreal:[" << adreal.toStrg()
	//					<< "]\tadformal:[" << adformal.toStrg() << "]"
	//					<< "\n\targreal2:[" << argreal2.toStrg() << "]"
	//					<< "\n\targformal2:[" << argformal2.toStrg() << "]"
	//					<< endl;
	//#endif
	//			}
	//
	//			bool addop = add(operatorreal, operatorformal);
	//
	//			if (addop == false) {
	//#if debug
	//				cout << "addopfalse:adreal:[" << adreal.toStrg()
	//					<< "]\tadformal:[" << adformal.toStrg() << "]"
	//					<< "\n\toperatorreal:[" << operatorreal.toStrg() << "]"
	//					<< "\n\toperatorformal:[" << operatorformal.toStrg()
	//					<< "]" << endl;
	//#endif
	//			}
	//			bool addre = add(resultreal, resultformal);
	//			if (addre == false) {
	//#if debug
	//				cout << "addformalalse:adreal:[" << adreal.toStrg()
	//					<< "]\tadformal:[" << adformal.toStrg() << "]"
	//
	//					<< "\n\tresultreal:[" << resultreal.toStrg() << "]"
	//					<< "\n\tresultformal:[" << resultformal.toStrg() << "]"
	//					<< endl;
	//#endif
	//			}
	//			if (add1 && add2 && addop && addre) {
	//				Addr adreal1;
	//				Addr adformal1;
	//				adreal1 = cdtreal->getAboveCodeAddr(adreal);
	//				adformal1 = cdtformal->getAboveCodeAddr(adformal);
	//				if ((*cdtreal)[adreal1].assemblyFormula.flagBit.type != COOL_M
	//					|| (*cdtreal)[adreal1].assemblyFormula.flagBit.functionHandleFlag
	//					== T_) {
	//					getFRMap();
	//					return true;
	//				}
	//				else if ((*cdtreal)[adreal1].assemblyFormula.flagBit.type == COOL_M
	//					&& (*cdtreal)[adreal1].assemblyFormula.flagBit.functionHandleFlag
	//					== F_) {
	//
	//					return match(adreal1, adformal1);
	//				}
	//				else {
	//#if debug
	//					cerr << "FunctionHandleMatcher::match err,exit(-1)" << endl;
	//#endif
	//#if debug && _WIN32
	//					{
	//						static int ignoreCount = 0;
	//						// Assuming info is a std::string, convert to std::wstring
	//						std::string info = "error exit"; // Replace with actual info
	//						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
	//						std::wstring winfo(info.begin(), info.end());
	//#if WIN_DEBUG_OUTPUT
	//						OutputDebugString(winfo.c_str());
	//#endif
	//						cout << info << endl;
	//						ignoreCount++;
	//					}
	//#endif
	//					assert(false); exit(-1);
	//				}
	//
	//			}
	//			return false;
	//		}
	//		else if (adformal == addrnull) {
	//			getFRMap();
	//			return true;
	//		}
	//		else {
	//			return false;
	//		}
	//	}

		/*map<ArgFormal, ArgReal>& FunctionHandleMatcher::getFRMap() {
	#if debug
			cout << "FunctionHandleMatcher::getFRMap in" << endl;
	#endif

			for (auto iter = am.begin(); iter != am.end(); iter++) {
				for (ArgFormal& af : iter->second) {
					if (iter->first.formalArg == false) {
						Arg argreal(iter->first);
						argreal.ref_ar = iter->first.ref_ar;
						argreal.formalArg = true;
						this->frmap.insert(pair<ArgFormal, ArgReal>(af, argreal));
					}
					else {
						this->frmap.insert(
							pair<ArgFormal, ArgReal>(af, iter->first));
					}
				}
			}
	#if debug
			cout << "FunctionHandleMatcher::getFRMap out" << endl;
	#endif

			return frmap;
		}*/

	void Executor::setPipeCommunicationHandler(PipeCommunicationHandler* pipe_writing_handler /*= nullptr*/,
		PipeCommunicationHandler* pipe_reading_handler /*= nullptr*/) {
		this->pipe_reading_handler = pipe_reading_handler;
		this->pipe_writing_handler = pipe_writing_handler;
	}
	void Executor::parseIOFileName(Strg& leftOp, Strg& rightOp, Strg& inpath,
		Strg& outpath) {
		function<void(const Strg& str, bool&, Strg&)> extractFileNameFromStr =
			[](const Strg& str, bool& succeed, Strg& name) {
			const Strg& prefix = "#FILE(";
			const Strg& suffix = ")";

			if (str.size() <= prefix.size() + suffix.size()) {
				succeed = false;
				//                name = "";
				return;// The string is too short to match the pattern.
			}

			// Check if the beginning of str matches the prefix
			if (str.substr(0, prefix.size()) != prefix) {
				succeed = false;
				//                name = "";
				return;
			}

			// Check if the end of str matches the suffix
			if (str.substr(str.size() - suffix.size()) != suffix) {
				succeed = false;
				//                name = "";
				return;
			}

			// Extract the inner string
			succeed = true;
			name = str.substr(prefix.size(),
				str.size() - prefix.size() - suffix.size());
			return;
			};
		bool lresult = false;
		bool rresult = false;
		extractFileNameFromStr(leftOp, lresult, inpath);
		extractFileNameFromStr(rightOp, rresult, outpath);
		if (!lresult) {

			strgReplace(leftOp, "\\n", "\n");
			strgReplace(leftOp, "\\t", "\t");
			strgReplace(leftOp, "\\r", "\r");
			strgReplace(leftOp, "\\\"", "\"");
			strgReplace(leftOp, "\\'", "\'");
			strgReplace(leftOp, "\\a", "\a");
			strgReplace(leftOp, "\\v", "\v");
			//                    strgReplace(leftOp, "\\u", "\u");
			//                    strgReplace(leftOp, "\\x", "\x");
			//不满足格式就以字符串方式处理
			//                    std::regex newline_pattern("(\\n)");
			//                    std::regex tab_pattern("(\\t)");
			//                    std::regex enter_pattern("(\\r)");
			//                    leftOp = regex_replace(leftOp, newline_pattern, "\n");
			//                    leftOp = regex_replace(leftOp, tab_pattern, "\t");
			//                    leftOp = regex_replace(leftOp, enter_pattern, "\r");
		}
		//        smatch rresults;
		if (!rresult) {

			{
				static int ignorecount = 0;
				std::cout << "Executor::executeCurrentCode()"
					<< " can't analysis output filepath, exit(-1) str:["
					<< rightOp << "]" << "\tignorecount:[" << ignorecount++
					<< "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")"
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
			assert(false); exit(-1);
		}
		//        std::regex path_pattern("#FILE\\((.*)\\)"); //此句曾使用regex并引起relocation truncated to fit
		//        smatch lresults;
		//        if (regex_match(leftOp, lresults, path_pattern)) {
		//
		//            //如果满足格式就从文件读取
		//
		//            inpath = lresults[1];
		//
		//        } else {
		//
		//            strgReplace(leftOp, "\\n", "\n");
		//            strgReplace(leftOp, "\\t", "\t");
		//            strgReplace(leftOp, "\\r", "\r");
		//            strgReplace(leftOp, "\\\"", "\"");
		//            strgReplace(leftOp, "\\'", "\'");
		//            strgReplace(leftOp, "\\a", "\a");
		//            strgReplace(leftOp, "\\v", "\v");
		//            //                    strgReplace(leftOp, "\\u", "\u");
		//            //                    strgReplace(leftOp, "\\x", "\x");
		//            //不满足格式就以字符串方式处理
		//            //                    std::regex newline_pattern("(\\n)");
		//            //                    std::regex tab_pattern("(\\t)");
		//            //                    std::regex enter_pattern("(\\r)");
		//            //                    leftOp = regex_replace(leftOp, newline_pattern, "\n");
		//            //                    leftOp = regex_replace(leftOp, tab_pattern, "\t");
		//            //                    leftOp = regex_replace(leftOp, enter_pattern, "\r");
		//        }
		//        smatch rresults;
		//        if (regex_match(rightOp, rresults, path_pattern)) {
		//
		//            outpath = rresults[1];
		//
		//        } else {
		//
		//            {
		//                static int ignorecount = 0;
		//                std::cout << "Executor::executeCurrentCode()"
		//                        << " can't analysis output filepath, exit(-1) str:["
		//                        << rightOp << "]" << "\tignorecount:[" << ignorecount++
		//                        << "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")"
		//                        << std::endl;
		//            }
		//
		//            assert(false); exit(-1);
		//        }
	}

	//以下函数由executeCurrentCode中的语法糖改编而来

		/**
		 * this lambda used to tell difference between two different pairs.
		 * @return  0 not equal ; 1 equal;
		 * @attention this function will compair key and value.
		 */
	bool Executor::op1it_op2it_equal(const pair<const Data, Data>& a,
		const pair<const Data, Data>& b) {
		if (a.first == b.first) {

			if (a.second == b.second) {
				return true;
			}

		}
		return false;
	}
	/**
		 * @brief the following functions  implement the basic operation functionalitis.
		 */
	void Executor::ADD(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {
		DataPtr op1 = op1_;
		DataPtr op2 = op2_;
		DataPtr opres = opres_;
		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}
		while (opres->dataFlagBit == S_PTR) {
			opres = opres->content_ptr;
		}
		if (opres != op1) {
			opres->clear();
		}

		if (type_a1 == Sz) {
			if (opres != op1) {
				opres->clear();

				(*opres) = op1->deepAssign();
			}
			if (type_a2 == Sz) {
				(opres->content_i) = (op1->content_i) + (op2->content_i);
			}
			else if (type_a2 == Zf) {
				Numb op2val = 0;
				try {
					op2val = toNumb(op2->content_s);
				}
				catch (CalculationException& e) {
					if (this->mode == MODE_GROUNDING) {
						cout << "Ignore calculation errors in reasoning: " << e.what() << " use 1 as default." << endl;
						op2val = 0;
					}
					else {
						throw;
					}
				}
				(opres->content_i) = (op1->content_i) + op2val;
			}
			else {
				Numb op2val = 0;
				try
				{
					op2val = op2->valueToNumb();
				}
				catch (const CalculationException& e)
				{
					if (this->mode == MODE_GROUNDING) {
						cout << "Ignore calculation errors in reasoning: " << e.what() << " use 0 as default." << endl;
						op2val = 0;
					}
					else {
						throw;
					}
				}
				(opres->content_i) = (op1->content_i) + op2val;
			}

		}
		else if (type_a1 == Zf) {
			if (opres != op1) {
				opres->clear();

				(*opres) = op1->deepAssign();
			}
			if (type_a2 == Zf) {

				(opres->content_s) = (op1->content_s) + (op2->content_s);
			}
			else if (type_a2 == Sz) {
				(opres->content_s) = (op1->content_s) + toStrg(op2->content_i);
			}
			else {
				(opres->content_s) = (op1->content_s) + op2->valueToStrg();
			}

		}
		else if (type_a1 == COOL_LST) {

			if (opres != op1) {

				(*opres) = op1->deepAssign();
			}
			if (type_a2 == Zf || type_a2 == Sz) {
				for (auto& elem : *opres->content_lst) {
					ADD(&elem, op2, &elem, type_a1, type_a2, type_res);
				}
			}
			else if (type_a2 == COOL_LST) {
				Intg len = min(opres->content_lst->size(),
					op2->content_lst->size());
				auto itopres = opres->content_lst->begin();
				auto itop2 = op2->content_lst->begin();
				while (len-- > 0) {
					ADD(&*itopres, &*itop2, &*itopres, type_a1, type_a2,
						type_res);
				}

			}
			else if (type_a2 == SET) {
				Intg len = min(opres->content_lst->size(),
					op2->content_set->size());
				auto itopres = opres->content_lst->begin();
				auto itop2 = op2->content_set->begin();
				while (len-- > 0) {
					ADD(DataPtr(&*itopres),
						DataPtr(&*itop2),
						DataPtr(&*itopres), type_a1, type_a2,
						type_res);
				}
			}
			else if (type_a2 == MULTISET) {
				Intg len = min(opres->content_lst->size(),
					op2->content_multiset->size());
				auto itopres = opres->content_lst->begin();
				auto itop2 = op2->content_multiset->begin();
				while (len-- > 0) {
					ADD(DataPtr(&*itopres),
						DataPtr(&*itop2),
						DataPtr(&*itopres), type_a1, type_a2,
						type_res);
				}
			}

		}
		else if (type_a1 == SET) {
			if (opres != op1) {

				*opres = op1->deepAssign();
			}
			if (type_a2 == Zf) {
				(*opres->content_set).insert(Data(op2->content_s));
			}
			else if (type_a2 == Sz) {
				(*opres->content_set).insert(Data(op2->content_i));
			}
			else if (type_a2 == COOL_LST) {
				auto&& d2dup = op2->deepAssign();
				(*opres->content_set).insert(d2dup.content_lst->begin(),
					d2dup.content_lst->end());
			}
			else if (type_a2 == SET) {
				auto&& d2dup = op2->deepAssign();
				(*opres->content_set).insert(d2dup.content_set->begin(),
					d2dup.content_set->end());
			}
			else if (type_a2 == MULTISET) {
				auto&& d2dup = op2->deepAssign();
				(*opres->content_set).insert(d2dup.content_multiset->begin(),
					d2dup.content_multiset->end());
			}
			else if (type_a2 == MAP) {
				for (auto& it : *op2->content_map)
					(*opres->content_set).insert(it.first.deepAssign());
			}
			else if (type_a2 == MULTIMAP) {
				for (auto& it : *op2->content_multimap)
					(*opres->content_set).insert(it.first.deepAssign());
			}
		}
		else if (type_a1 == MULTISET) {
			if (opres != op1) {

				*opres = op1->deepAssign();
			}
			if (type_a2 == Zf) {
				(*opres->content_multiset).insert(Data(op2->content_s));
			}
			else if (type_a2 == Sz) {
				(*opres->content_multiset).insert(Data(op2->content_i));
			}
			else if (type_a2 == COOL_LST) {
				auto&& d2dup = op2->deepAssign();
				(*opres->content_multiset).insert(d2dup.content_lst->begin(),
					d2dup.content_lst->end());
			}
			else if (type_a2 == SET) {
				auto&& d2dup = op2->deepAssign();
				(*opres->content_multiset).insert(d2dup.content_set->begin(),
					d2dup.content_set->end());
			}
			else if (type_a2 == MULTISET) {
				auto&& d2dup = op2->deepAssign();
				(*opres->content_multiset).insert(
					d2dup.content_multiset->begin(),
					d2dup.content_multiset->end());
			}
			else if (type_a2 == MAP) {
				for (auto& it : *op2->content_map)
					(*opres->content_multiset).insert(it.first.deepAssign());
			}
			else if (type_a2 == MULTIMAP) {
				for (auto& it : *op2->content_multimap)
					(*opres->content_multiset).insert(it.first.deepAssign());
			}
		}
		else if (type_a1 == MAP) {
			if (op1 != opres) {
				*opres = op1->deepAssign();
			}
			if (type_a2 == Zf) {
				(*opres->content_map).insert(
					make_pair(Data(op2->content_s), Data()));
			}
			else if (type_a2 == Sz) {
				(*opres->content_map).insert(
					make_pair(Data(op2->content_i), Data()));
			}
			else if (type_a2 == COOL_LST) {
				for (auto& it : *op2->content_lst)
					(*opres->content_map).insert(make_pair(it.deepAssign(), Data()));
			}
			else if (type_a2 == SET) {
				for (auto& it : *op2->content_set)
					(*opres->content_map).insert(make_pair(it.deepAssign(), Data()));
			}
			else if (type_a2 == MULTISET) {
				for (auto& it : *op2->content_set)
					(*opres->content_map).insert(make_pair(it.deepAssign(), Data()));
			}
			else if (type_a2 == MAP) {
				auto&& d2dup = op2->deepAssign();
				(*opres->content_map).insert(d2dup.content_map->begin(),
					d2dup.content_map->end());
			}
			else if (type_a2 == MULTIMAP) {
#if debug
				{
					static int ignorecount = 0;
					std::cerr << "Executor::executeCurrentCode()"
						<< " can't add map with multimap, exit(-1) "
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")"
						<< std::endl;
				}
#endif
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
				assert(false); exit(-1);
			}
		}
		else if (type_a1 == MULTIMAP) {
			if (op1 != opres) {

				*opres = op1->deepAssign();
			}
			if (type_a2 == Zf) {
				(*opres->content_multimap).insert(
					make_pair(Data(op2->content_s), multiset<Data>()));
			}
			else if (type_a2 == Sz) {
				(*opres->content_multimap).insert(
					make_pair(Data(op2->content_i), multiset<Data>()));
			}
			else if (type_a2 == COOL_LST) {
				for (auto& it : *op2->content_lst)
					(*opres->content_multimap).insert(
						make_pair(it.deepAssign(), multiset<Data>()));
			}
			else if (type_a2 == SET) {
				for (auto& it : *op2->content_set)
					(*opres->content_multimap).insert(
						make_pair(it.deepAssign(), multiset<Data>()));
			}
			else if (type_a2 == MULTISET) {
				for (auto& it : *op2->content_set)
					(*opres->content_multimap).insert(
						make_pair(it.deepAssign(), multiset<Data>()));
			}
			else if (type_a2 == MAP) {
				for (auto& it : *op2->content_map)
					(*opres->content_multimap)[it.first.deepAssign()].insert(
						it.second.deepAssign());
			}
			else if (type_a2 == MULTISET) {
				auto&& op2dup = op2->deepAssign();
				for (auto& it : *op2dup.content_multimap) {

					opres->content_multimap->operator [](it.first).insert(
						it.second.begin(), it.second.end());
				}

			}
		}
		else {

			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

			}
		}

	}
	/*
		 * _.cpp
		 *
		 *  Created on: 31-八月-23
		 *      Author: 韩济澎
		 */

	void Executor::SUB(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {
		DataPtr op1 = op1_;
		DataPtr op2 = op2_;
		DataPtr opres = opres_;
		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}
		while (opres->dataFlagBit == S_PTR) {
			opres = opres->content_ptr;
		}
		if (opres != op1) {
			opres->clear();

		}

		if (type_a1 == Sz) {
			if (opres != op1) {
				opres->clear();
				opres->dataFlagBit = Sz;

			}
			if (type_a2 == Sz) {
				(opres->content_i) = (op1->content_i) - (op2->content_i);
			}
			else if (type_a2 == Zf) {

				Numb op2val = 0;
				try {
					op2val = toNumb(op2->content_s);
				}
				catch (CalculationException& e) {
					if (this->mode == MODE_GROUNDING) {
						cout << "Ignore calculation errors in reasoning: " << e.what() << " use 0 as default." << endl;
						op2val = 0;
					}
					else {
						throw;
					}
				}


				(opres->content_i) = (op1->content_i) - op2val;
			}
			else {
				Numb op2val = 0;
				try
				{
					op2val = op2->valueToNumb();
				}
				catch (const CalculationException& e)
				{
					if (this->mode == MODE_GROUNDING) {
						cout << "Ignore calculation errors in reasoning: " << e.what() << " use 0 as default." << endl;
						op2val = 0;
					}
					else {
						throw;
					}
				}
				(opres->content_i) = (op1->content_i) - op2val /*op2->valueToNumb()*/;
				//(opres->content_i) = (op1->content_i) - op2->valueToNumb();
			}

		}
		else if (type_a1 == Zf) {
			if (opres != op1) {
				opres->clear();
				opres->dataFlagBit = Zf;

			}
			if (type_a2 == Zf) {
				auto strgv = split(op1->content_s, op2->content_s);
				opres->clear();
				opres->dataFlagBit = Zf;
				for (auto& it : strgv) {
					opres->content_s.operator +=(it);
				}

			}
			else if (type_a2 == Sz) {
				auto&& strgv = split(op1->content_s, toStrg(op2->content_i));
				opres->clear();
				opres->dataFlagBit = Zf;
				for (auto& it : strgv) {
					opres->content_s.operator +=(it);
				}
			}
			else if (type_a2 == COOL_LST) {
				vector<Strg> vd1;
				vector<Strg> vd2;
				vd2.push_back(op1->content_s);
				for (const auto& datait : *op2->content_lst) {
					Strg seg;
					if (datait.dataFlagBit == Sz) {
						seg = toStrg(datait.content_i);
					}
					else if (datait.dataFlagBit == Zf) {
						seg = datait.content_s;
					}
					if (seg == "") {
						continue;
					}
					vd1.swap(vd2);
					if (vd1.size() <= 0) {
						break;
					}
					for (auto& strgit : vd1) {
						if (strgit.size() <= 0) {
							continue;
						}
						else {
							auto&& splitv = split(strgit, seg);
							vd2.insert(vd2.end(), splitv.begin(), splitv.end());
						}

					}

					vd1.clear();

				}
				opres->clear();
				opres->dataFlagBit = Zf;
				for (auto& strgfrag : vd2) {
					opres->content_s.operator +=(strgfrag);
				}

			}
			else if (type_a2 == SET) {
				vector<Strg> vd1;
				vector<Strg> vd2;
				vd2.push_back(op1->content_s);
				for (const auto& datait : *op2->content_set) {
					Strg seg;
					if (datait.dataFlagBit == Sz) {
						seg = toStrg(datait.content_i);
					}
					else if (datait.dataFlagBit == Zf) {
						seg = datait.content_s;
					}
					if (seg == "") {
						continue;
					}
					vd1.swap(vd2);
					if (vd1.size() <= 0) {
						break;
					}
					for (auto& strgit : vd1) {
						if (strgit.size() <= 0) {
							continue;
						}
						else {
							auto&& splitv = split(strgit, seg);
							vd2.insert(vd2.end(), splitv.begin(), splitv.end());
						}

					}

					vd1.clear();

				}
				opres->clear();
				opres->dataFlagBit = Zf;
				for (auto& strgfrag : vd2) {
					opres->content_s.operator +=(strgfrag);
				}

			}
			else if (type_a2 == MULTISET) {
				vector<Strg> vd1;
				vector<Strg> vd2;
				vd2.push_back(op1->content_s);
				for (const auto& datait : *op2->content_multiset) {
					Strg seg;
					if (datait.dataFlagBit == Sz) {
						seg = toStrg(datait.content_i);
					}
					else if (datait.dataFlagBit == Zf) {
						seg = datait.content_s;
					}
					if (seg == "") {
						continue;
					}
					vd1.swap(vd2);
					if (vd1.size() <= 0) {
						break;
					}
					for (auto& strgit : vd1) {
						if (strgit.size() <= 0) {
							continue;
						}
						else {
							auto&& splitv = split(strgit, seg);
							vd2.insert(vd2.end(), splitv.begin(), splitv.end());
						}

					}

					vd1.clear();

				}
				opres->clear();
				opres->dataFlagBit = Zf;
				for (auto& strgfrag : vd2) {
					opres->content_s.operator +=(strgfrag);
				}

			}
		}
		else if (type_a1 == COOL_LST) {

			if (opres != op1) {

				(*opres) = op1->deepAssign();
			}
			if (type_a2 == Zf || type_a2 == Sz) {
				for (auto& elem : *opres->content_lst) {
					SUB(&elem, op2, &elem, type_a1, type_a2, type_res);
				}
			}
			else if (type_a2 == COOL_LST) {
				Intg len = min(opres->content_lst->size(),
					op2->content_lst->size());
				auto itopres = opres->content_lst->begin();
				auto itop2 = op2->content_lst->begin();
				while (len-- > 0) {
					SUB(DataPtr(&*itopres),
						DataPtr(&*itop2),
						DataPtr(&*itopres), type_a1, type_a2,
						type_res);
				}

			}
			else if (type_a2 == SET) {
				Intg len = min(opres->content_lst->size(),
					op2->content_set->size());
				auto itopres = opres->content_lst->begin();
				auto itop2 = op2->content_set->begin();
				while (len-- > 0) {
					SUB(DataPtr(&*itopres),
						DataPtr(&*itop2),
						DataPtr(&*itopres), type_a1, type_a2,
						type_res);
				}
			}
			else if (type_a2 == MULTISET) {
				Intg len = min(opres->content_lst->size(),
					op2->content_multiset->size());
				auto itopres = opres->content_lst->begin();
				auto itop2 = op2->content_multiset->begin();
				while (len-- > 0) {
					SUB(DataPtr(&*itopres),
						DataPtr(&*itop2),
						DataPtr(&*itopres), type_a1, type_a2,
						type_res);
				}
			}

		}
		else if (type_a1 == SET) {
			if (op1 == opres) {
				*opres = op1->deepAssign();
			}
			if (type_a2 == Zf) {
				(*opres->content_set).erase(Data(op2->content_s));
			}
			else if (type_a2 == Sz) {
				(*opres->content_set).erase(Data(op2->content_i));
			}
			else if (type_a2 == COOL_LST) {
				for (auto& elem : *op2->content_lst) {
					auto it = opres->content_set->find(elem);
					Data erasetemp = it.operator *();
					(*opres->content_set).erase(it);
					erasetemp.clear();
				}
			}
			else if (type_a2 == SET) {
				for (auto& elem : *op2->content_set) {
					auto it = opres->content_set->find(elem);
					Data erasetemp = it.operator *();
					(*opres->content_set).erase(it);
					erasetemp.clear();
				}
			}
			else if (type_a2 == MULTISET) {
				for (auto& elem : *op2->content_multiset) {
					auto it = opres->content_set->find(elem);
					Data erasetemp = it.operator *();
					(*opres->content_set).erase(it);
					erasetemp.clear();
				}
			}
		}
		else if (type_a1 == MULTISET) {
			if (opres != op1) {

				*opres = op1->deepAssign();
			}
			if (type_a2 == Zf) {
				(*opres->content_multiset).erase(Data(op2->content_s));
			}
			else if (type_a2 == Sz) {
				(*opres->content_multiset).erase(Data(op2->content_i));
			}
			else if (type_a2 == COOL_LST) {
				for (auto& elem : *op2->content_lst) {
					auto it = opres->content_multiset->find(elem);
					Data erasetemp = it.operator *();
					(*opres->content_multiset).erase(it);
					erasetemp.clear();
				}
			}
			else if (type_a2 == SET) {
				for (auto& elem : *op2->content_set) {
					auto it = opres->content_multiset->find(elem);
					Data erasetemp = it.operator *();
					(*opres->content_multiset).erase(it);
					erasetemp.clear();
				}
			}
			else if (type_a2 == MULTISET) {
				for (auto& elem : *op2->content_multiset) {
					auto it = opres->content_multiset->find(elem);
					Data erasetemp = it.operator *();
					(*opres->content_multiset).erase(it);
					erasetemp.clear();
				}
			}
		}
		else if (type_a1 == MAP) {
			if (opres != op1) {

				*opres = op1->deepAssign();
			}
			if (type_a2 == Zf || type_a2 == Sz) {
				auto pairit = opres->content_map->find(*op2);
				Data keydelete = pairit->first;
				Data valuedelete = pairit->second;
				(*opres->content_map).erase(pairit);
				keydelete.clear();
				valuedelete.clear();
			}
			else if (type_a2 == COOL_LST) {
				for (auto& elem : *op2->content_lst) {
					auto pairit = opres->content_map->find(elem);
					Data keydelete = pairit->first;
					Data valuedelete = pairit->second;
					(*opres->content_map).erase(pairit);
					keydelete.clear();
					valuedelete.clear();
				}
			}
			else if (type_a2 == SET) {
				for (auto& elem : *op2->content_set) {
					auto pairit = opres->content_map->find(elem);
					Data keydelete = pairit->first;
					Data valuedelete = pairit->second;
					(*opres->content_map).erase(pairit);
					keydelete.clear();
					valuedelete.clear();
				}
			}
			else if (type_a2 == MULTISET) {
				for (auto& elem : *op2->content_multiset) {
					auto pairit = opres->content_map->find(elem);
					Data keydelete = pairit->first;
					Data valuedelete = pairit->second;
					(*opres->content_map).erase(pairit);
					keydelete.clear();
					valuedelete.clear();
				}
			}
			else if (type_a2 == MAP) {
				for (auto& pair_ : *op2->content_map) {
					auto pairit = opres->content_map->find(pair_.first);
					Data keydelete = pairit->first;
					Data valuedelete = pairit->second;
					(*opres->content_map).erase(pairit);
					keydelete.clear();
					valuedelete.clear();
				}
			}
			else if (type_a2 == MULTIMAP) {
				for (auto& pair_ : *op2->content_multimap) {
					auto pairit = opres->content_map->find(pair_.first);
					Data keydelete = pairit->first;
					Data valuedelete = pairit->second;
					(*opres->content_map).erase(pairit);
					keydelete.clear();
					valuedelete.clear();
				}
			}
		}
		else if (type_a1 == MULTIMAP) {
			if (opres != op1) {

				*opres = op1->deepAssign();
			}
			if (type_a2 == Zf || type_a2 == Sz) {
				auto pairit = opres->content_multimap->find(*op2);
				Data keydelete = pairit->first;
				Data valuedelete;
				valuedelete.dataFlagBit = MULTISET;
				valuedelete.content_multiset = &pairit->second;
				(*opres->content_multimap).erase(pairit);
				keydelete.clear();
				valuedelete.clear();
			}
			else if (type_a2 == COOL_LST) {
				for (auto& elem : *op2->content_lst) {
					auto pairit = opres->content_multimap->find(elem);
					Data keydelete = pairit->first;
					Data valuedelete;
					valuedelete.dataFlagBit = MULTISET;
					valuedelete.content_multiset = &pairit->second;
					(*opres->content_multimap).erase(pairit);
					keydelete.clear();
					valuedelete.clear();
				}
			}
			else if (type_a2 == SET) {
				for (auto& elem : *op2->content_set) {
					auto pairit = opres->content_multimap->find(elem);
					Data keydelete = pairit->first;
					Data valuedelete;
					valuedelete.dataFlagBit = MULTISET;
					valuedelete.content_multiset = &pairit->second;
					(*opres->content_multimap).erase(pairit);
					keydelete.clear();
					valuedelete.clear();
				}
			}
			else if (type_a2 == MULTISET) {
				for (auto& elem : *op2->content_multiset) {
					auto pairit = opres->content_multimap->find(elem);
					Data keydelete = pairit->first;
					Data valuedelete;
					valuedelete.dataFlagBit = MULTISET;
					valuedelete.content_multiset = &pairit->second;
					(*opres->content_multimap).erase(pairit);
					keydelete.clear();
					valuedelete.clear();
				}
			}
			else if (type_a2 == MAP) {
				for (auto& pair_ : *op2->content_map) {
					auto pairit = opres->content_multimap->find(pair_.first);
					Data keydelete = pairit->first;
					Data valuedelete;
					valuedelete.dataFlagBit = MULTISET;
					valuedelete.content_multiset = &pairit->second;
					(*opres->content_multimap).erase(pairit);
					keydelete.clear();
					valuedelete.clear();
				}
			}
			else if (type_a2 == MULTIMAP) {
				for (auto& pair_ : *op2->content_multimap) {
					auto pairit = opres->content_multimap->find(pair_.first);
					Data keydelete = pairit->first;
					Data valuedelete;
					valuedelete.dataFlagBit = MULTISET;
					valuedelete.content_multiset = &pairit->second;
					(*opres->content_multimap).erase(pairit);
					keydelete.clear();
					valuedelete.clear();
				}
			}
		}
		else {

			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

			}


		}

	}
	void Executor::CHU(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {
		DataPtr op1 = op1_;
		DataPtr op2 = op2_;
		DataPtr opres = opres_;
		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}
		while (opres->dataFlagBit == S_PTR) {
			opres = opres->content_ptr;
		}
		if (opres != op1) {
			opres->clear();
		}

		if (type_a1 == Sz) {
			if (opres != op1) {
				(*opres) = op1->deepAssign();
			}
			if (type_a2 == Sz) {
				Numb op2val = op2->content_i;
				if (op2->content_i == 0)
				{
					if (this->mode == MODE_GROUNDING) {
						cout << "Ignore calculation errors in reasoning: " << "divided by zero," << " use 1 as default." << endl;
						op2val = 1;
					}
					else {
						THROW_CALCULATION_EXCEPTION(CalculationErrorType::DivideByZero, "Divided by zero");
					}
				}
				(opres->content_i) = (op1->content_i) / op2val;
			}
			else if (type_a2 == Zf) {
				Numb op2val = 1;
				try {
					op2val = toNumb(op2->content_s);
				}
				catch (CalculationException& e) {
					if (this->mode == MODE_GROUNDING) {
						cout << "Ignore calculation errors in reasoning: " << e.what() << " use 1 as default." << endl;
						op2val = 1;
					}
					else {
						throw;
					}
				}
				if (op2val == 0)
				{
					if (this->mode == MODE_GROUNDING) {
						cout << "Ignore calculation errors in reasoning: " << "divided by zero," << " use 1 as default." << endl;
						op2val = 1;
					}
					else {
						THROW_CALCULATION_EXCEPTION(CalculationErrorType::DivideByZero, "Divided by zero");
					}
				}
				(opres->content_i) = (op1->content_i) / op2val;
			}
			else {
				Numb op2val = 1;
				try
				{
					op2val = op2->valueToNumb();
				}
				catch (const CalculationException& e)
				{
					if (this->mode == MODE_GROUNDING) {
						cout << "Ignore calculation errors in reasoning: " << e.what() << " use 1 as default." << endl;
						op2val = 1;
					}
					else {
						throw;
					}
				}
				(opres->content_i) = (op1->content_i) / op2val /*op2->valueToNumb()*/;
			}

		}
		else if (type_a1 == Zf) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

			}


		}
		else if (type_a1 == COOL_LST) {

			if (opres != op1) {

				(*opres) = op1->deepAssign();
			}
			if (type_a2 == Zf || type_a2 == Sz) {
				for (auto& elem : *opres->content_lst) {
					CHU(&elem, op2, &elem, type_a1, type_a2, type_res);
				}
			}
			else if (type_a2 == COOL_LST) {
				Intg len = min(opres->content_lst->size(),
					op2->content_lst->size());
				auto itopres = opres->content_lst->begin();
				auto itop2 = op2->content_lst->begin();
				while (len-- > 0) {
					CHU(DataPtr(&*itopres),
						DataPtr(&*itop2),
						DataPtr(&*itopres), type_a1, type_a2,
						type_res);
				}

			}
			else if (type_a2 == SET) {
				Intg len = min(opres->content_lst->size(),
					op2->content_set->size());
				auto itopres = opres->content_lst->begin();
				auto itop2 = op2->content_set->begin();
				while (len-- > 0) {
					CHU(DataPtr(&*itopres),
						DataPtr(&*itop2),
						DataPtr(&*itopres), type_a1, type_a2,
						type_res);
				}
			}
			else if (type_a2 == MULTISET) {
				Intg len = min(opres->content_lst->size(),
					op2->content_multiset->size());
				auto itopres = opres->content_lst->begin();
				auto itop2 = op2->content_multiset->begin();
				while (len-- > 0) {
					CHU(DataPtr(&*itopres),
						DataPtr(&*itop2),
						DataPtr(&*itopres), type_a1, type_a2,
						type_res);
				}
			}

		}
		else if (type_a1 == SET) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

			}


		}
		else if (type_a1 == MULTISET) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

			}


		}
		else if (type_a1 == MAP) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

			}


		}
		else if (type_a1 == MULTIMAP) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

			}


		}
		else {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

			}

		}

	}

	/**
	return:
	-1 if op1<op2,
	0 if op1 == op2 ,
	1 if op1 > op2,
	expr > non-expr
	big serial number expr> small serial number expr
	*/
	Intg Executor::CMP(const DataPtr op1_, const DataPtr op2_) {
		DataPtr d1 = op1_;
		DataPtr d2 = op2_;


		if (d1->isExpression || d2->isExpression) {
			ExpressionHandlerPtr e1 = nullptr;
			ExpressionHandlerPtr e2 = nullptr;

			if (d1->isExpression == false) {
				e1 = ExpressionHandlerPtr::make(argexe1);
				e1->flatten((*currentAR)[Arg(S_Bs, "thisexpr")].expr, currentAR, currentAR->getFunctionCallFRMap(tfth, scpth), this->cdth, this->scpth);

			}
			else {
				e1 = d1->expr;
			}

			if (d2->isExpression == false) {
				e2 = ExpressionHandlerPtr::make(argexe2);
				e2->flatten((*currentAR)[Arg(S_Bs, "thisexpr")].expr, currentAR, currentAR->getFunctionCallFRMap(tfth, scpth), this->cdth, this->scpth);

			}
			else {
				e2 = d2->expr;
			}
			if (e1->type == e2->type && e1->expr_arg == e2->expr_arg &&
				e1->expr_cdt == e2->expr_cdt) {
				return 0;
			}
			else {
				if (d1->isExpression == true && d2->isExpression == false) {
					return 1;
				}
				else if (d1->isExpression == false && d2->isExpression == true) {
					return -1;
				}
				else {
					return e1->serialNumber > e2->serialNumber ? 1 : -1;
				}

			}


		}
		try {
			return COOLANG::CMP(op1_, op2_);
		}
		catch (CalculationException& e) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return 0;
			}
			else {
				throw;

			}
		}
	}
	;
	void Executor::POW(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {

		DataPtr op1 = op1_;
		DataPtr op2 = op2_;
		DataPtr opres = opres_;
		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}
		while (opres->dataFlagBit == S_PTR) {
			opres = opres->content_ptr;
		}
		if (op1 == op2 && op2 == opres) {
			return;
		}
		if (op1->dataFlagBit == S_AR || op2->dataFlagBit == S_AR) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

			}

		}
		if (op1->dataFlagBit == Sz) {
			if (opres != op1) {
				opres->clear();
				opres->dataFlagBit = Sz;

			}
			if (op2->dataFlagBit == Sz) {
				opres->content_i = pow(op1->content_i, op2->content_i);

			}
			else if (op2->dataFlagBit == Zf) {
				Numb op2val = 1;
				try {
					op2val = toNumb(op2->content_s);
				}
				catch (CalculationException& e) {
					if (this->mode == MODE_GROUNDING) {
						cout << "Ignore calculation errors in reasoning: " << e.what() << " use 1 as default." << endl;
						op2val = 1;
					}
					else {
						throw;
					}
				}
				opres->content_i = pow(op1->content_i, op2val);
			}
			else if (op2->dataFlagBit == COOL_LST) {
				if (op2->content_lst->size() == 1) {
					if (op2->content_lst->operator [](0).dataFlagBit == Sz) {
						opres->content_i = pow(op1->content_i,
							op2->content_lst->operator [](0).content_i);
					}
					else if (op2->content_lst->operator [](0).dataFlagBit
						== Zf) {
						opres->content_i =
							pow(op1->content_i,
								toNumb(
									op2->content_lst->operator [](0).content_s));

					}

				}
			}
			else if (op2->dataFlagBit == SET) {
				if (op2->content_set->size() == 1) {
					if (op2->content_set->begin().operator *().dataFlagBit
						== Sz) {
						opres->content_i =
							pow(op1->content_i,
								op2->content_set->begin().operator *().content_i);
					}
					else if (op2->content_set->begin().operator *().dataFlagBit
						== Zf) {
						opres->content_i =
							pow(op1->content_i,
								toNumb(
									op2->content_set->begin().operator *().content_s));
					}
				}
			}
			else if (op2->dataFlagBit == MULTISET) {
				if (op2->content_multiset->size() == 1) {
					if (op2->content_multiset->begin().operator *().dataFlagBit
						== Sz) {
						opres->content_i =
							pow(op1->content_i,
								op2->content_multiset->begin().operator *().content_i);
					}
					else if (op2->content_multiset->begin().operator *().dataFlagBit
						== Zf) {
						opres->content_i =
							pow(op1->content_i,
								toNumb(
									op2->content_multiset->begin().operator *().content_s));
					}
				}
			}
			else if (op2->dataFlagBit == MAP) {
				if (this->mode == MODE_GROUNDING) {
					cout << "Ignore calculation errors in reasoning." << endl;
					return;
				}
				else {
					THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

				}

			}
			else if (op2->dataFlagBit == MULTIMAP) {
				if (this->mode == MODE_GROUNDING) {
					cout << "Ignore calculation errors in reasoning." << endl;
					return;
				}
				else {
					THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

				}

			}

		}
		else if (op1->dataFlagBit == Zf) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

			}

		}
		else if (op1->dataFlagBit == COOL_LST) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

			}

		}
		else if (op1->dataFlagBit == SET) {
			if (opres != op1) {
				opres->clear();
				opres->dataFlagBit = SET;
				opres->content_set = DataSetPtr(new set<Data>());
			}

			if (op2->dataFlagBit == Sz) {

				if (op1->content_set->count(*op2) > 0) {
					opres->clear();
					opres->dataFlagBit = SET;
					opres->content_set = DataSetPtr(new set<Data>());
					opres->content_set->insert(*op2);

				}

			}
			else if (op2->dataFlagBit == Zf) {

				if (op1->content_set->count(*op2) > 0) {
					opres->clear();
					opres->dataFlagBit = SET;
					opres->content_set = DataSetPtr(new set<Data>());
					opres->content_set->insert(*op2);
				}

			}
			else if (op2->dataFlagBit == COOL_LST) {

				multiset<Data> op2__;
				op2__.insert(op2->content_lst->begin(),
					op2->content_lst->end());
				set<Data> sd1;
				set_intersection(op1->content_set->begin(),
					op1->content_set->end(), op2__.begin(), op2__.end(),
					inserter(sd1, sd1.begin()));
				if (opres == op1) {
					deque<Data> eras;
					set_difference(opres->content_set->begin(),
						opres->content_set->end(), sd1.begin(), sd1.end(),
						inserter(eras, eras.begin()));
					for (auto& elem : eras) {
						opres->content_set->erase(elem);
						elem.clear();
					}
				}

				for (const auto& it : sd1) {

					if (opres->content_set->count(it) <= 0) {
						opres->content_set->insert(it.deepAssign());
					}
				}
			}
			else if (op2->dataFlagBit == SET) {

				set<Data> sd1;
				set_intersection(op1->content_set->begin(),
					op1->content_set->end(), op2->content_set->begin(),
					op2->content_set->end(), inserter(sd1, sd1.begin()));
				if (opres == op1) {
					deque<Data> eras;
					set_difference(opres->content_set->begin(),
						opres->content_set->end(), sd1.begin(), sd1.end(),
						inserter(eras, eras.begin()));
					for (auto& elem : eras) {
						opres->content_set->erase(elem);
						elem.clear();
					}
				}

				for (const auto& it : sd1) {

					if (opres->content_set->count(it) <= 0) {
						opres->content_set->insert(it.deepAssign());
					}
				}
			}
			else if (op2->dataFlagBit == MULTISET) {

				set<Data> sd1;
				set_intersection(op1->content_set->begin(),
					op1->content_set->end(), op2->content_multiset->begin(),
					op2->content_multiset->end(),
					inserter(sd1, sd1.begin()));
				if (opres == op1) {
					deque<Data> eras;
					set_difference(opres->content_set->begin(),
						opres->content_set->end(), sd1.begin(), sd1.end(),
						inserter(eras, eras.begin()));
					for (auto& elem : eras) {
						opres->content_set->erase(elem);
						elem.clear();
					}
				}

				for (const auto& it : sd1) {

					if (opres->content_set->count(it) <= 0) {
						opres->content_set->insert(it.deepAssign());
					}
				}
			}
			else if (op2->dataFlagBit == MAP
				|| op2->dataFlagBit == MULTIMAP) {
				if (this->mode == MODE_GROUNDING) {
					cout << "Ignore calculation errors in reasoning." << endl;
					return;
				}
				else {
					THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

				}

			}
			else if (op2->dataFlagBit == MULTIMAP) {
				if (this->mode == MODE_GROUNDING) {
					cout << "Ignore calculation errors in reasoning." << endl;
					return;
				}
				else {
					THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

				}

			}

		}
		else if (op1->dataFlagBit == MULTISET) {

			if (opres != op1) {
				opres->clear();
				opres->dataFlagBit = MULTISET;
				opres->content_multiset = DataMultiSetPtr(new multiset<Data>());
			}

			if (op2->dataFlagBit == Sz) {

				if (op1->content_multiset->count(*op2) > 0) {
					opres->clear();
					opres->dataFlagBit = MULTISET;
					opres->content_multiset = DataMultiSetPtr(new multiset<Data>());
					opres->content_multiset->insert(*op2);

				}

			}
			else if (op2->dataFlagBit == Zf) {
				if (op1->content_multiset->count(*op2) > 0) {
					opres->clear();
					opres->dataFlagBit = MULTISET;
					opres->content_multiset = DataMultiSetPtr(new multiset<Data>());
					opres->content_multiset->insert(*op2);

				}
			}
			else if (op2->dataFlagBit == COOL_LST) {

				multiset<Data> op2__;
				op2__.insert(op2->content_lst->begin(),
					op2->content_lst->end());
				multiset<Data> sd1;
				set_intersection(op1->content_set->begin(),
					op1->content_set->end(), op2__.begin(), op2__.end(),
					inserter(sd1, sd1.begin()));
				if (opres == op1) {
					deque<Data> eras;
					set_difference(opres->content_set->begin(),
						opres->content_set->end(), sd1.begin(), sd1.end(),
						inserter(eras, eras.begin()));
					for (auto& elem : eras) {
						opres->content_multiset->erase(elem);
						elem.clear();
					}
				}

				for (const auto& it : sd1) {

					if (opres->content_multiset->count(it) <= 0) {
						opres->content_multiset->insert(it.deepAssign());
					}
				}
			}
			else if (op2->dataFlagBit == SET) {

				multiset<Data> sd1;
				set_intersection(op1->content_multiset->begin(),
					op1->content_multiset->end(), op2->content_set->begin(),
					op2->content_set->end(), inserter(sd1, sd1.begin()));
				if (opres == op1) {
					deque<Data> eras;
					set_difference(opres->content_multiset->begin(),
						opres->content_multiset->end(), sd1.begin(),
						sd1.end(), inserter(eras, eras.begin()));
					for (auto& elem : eras) {
						opres->content_multiset->erase(elem);
						elem.clear();
					}
				}

				for (const auto& it : sd1) {

					if (opres->content_multiset->count(it) <= 0) {
						opres->content_multiset->insert(it.deepAssign());
					}
				}
			}
			else if (op2->dataFlagBit == MULTISET) {

				multiset<Data> sd1;
				set_intersection(op1->content_multiset->begin(),
					op1->content_multiset->end(),
					op2->content_multiset->begin(),
					op2->content_multiset->end(),
					inserter(sd1, sd1.begin()));
				if (opres == op1) {
					deque<Data> eras;
					set_difference(opres->content_multiset->begin(),
						opres->content_multiset->end(), sd1.begin(),
						sd1.end(), inserter(eras, eras.begin()));
					for (auto& elem : eras) {
						opres->content_multiset->erase(elem);
						elem.clear();
					}
				}

				for (const auto& it : sd1) {

					if (opres->content_multiset->count(it) <= 0) {
						opres->content_multiset->insert(it.deepAssign());
					}
				}
			}
			else if (op2->dataFlagBit == MAP) {
				if (this->mode == MODE_GROUNDING) {
					cout << "Ignore calculation errors in reasoning." << endl;
					return;
				}
				else {
					THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

				}

			}
			else if (op2->dataFlagBit == MULTIMAP) {
				if (this->mode == MODE_GROUNDING) {
					cout << "Ignore calculation errors in reasoning." << endl;
					return;
				}
				else {
					THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

				}

			}

		}
		else if (op1->dataFlagBit == MAP) {
			if (opres != op1) {
				opres->clear();
				opres->dataFlagBit = MAP;
				opres->content_map = DataMapPtr(new map<Data, Data>());
			}
			Data oprestmp;
			oprestmp.dataFlagBit = MAP;
			oprestmp.content_map = DataMapPtr(new map<Data, Data>());
			if (op2->dataFlagBit == Sz) {

				if (op1->content_map->count(*op2) > 0) {

					oprestmp.content_map->insert(
						make_pair(*op2,
							op1->content_map->operator [](*op2)));

				}

			}
			else if (op2->dataFlagBit == Zf) {
				if (op1->content_map->count(*op2) > 0) {
					oprestmp.content_map->insert(
						make_pair(op2->deepAssign(),
							op1->content_map->operator [](*op2).deepAssign()));
				}

			}
			else if (op2->dataFlagBit == COOL_LST) {

				multiset<Data> op1_;
				multiset<Data> op2_;
				for (auto itmap : *op1->content_map) {
					op1_.insert(itmap.first);

				}

				op2_.insert(op2->content_lst->begin(), op2->content_lst->end());
				set<Data> sd1;

				set_intersection(op1_.begin(), op1_.end(), op2_.begin(),
					op2_.end(), inserter(sd1, sd1.begin()));

				for (const auto& it : sd1) {
					oprestmp.content_map->insert(
						make_pair(it.deepAssign(),
							op1->content_map->operator [](it).deepAssign()));

				}
			}
			else if (op2->dataFlagBit == SET) {

				multiset<Data> op1__;
				set<Data>& op2__ = *op2->content_set;
				for (auto itmap : *op1->content_map) {
					op1__.insert(itmap.first);

				}
				set<Data> sd1;

				set_intersection(op1__.begin(), op1__.end(), op2__.begin(),
					op2__.end(), inserter(sd1, sd1.begin()));

				for (const auto& it : sd1) {
					oprestmp.content_map->insert(
						make_pair(it.deepAssign(),
							op1->content_map->operator [](it).deepAssign()));

				}
			}
			else if (op2->dataFlagBit == MULTISET) {

				set<Data> op1__;
				multiset<Data>& op2__ = *op2->content_multiset;
				for (auto itmap : *op1->content_map) {
					op1__.insert(itmap.first);

				}
				set<Data> sd1;

				set_intersection(op1__.begin(), op1__.end(), op2__.begin(),
					op2__.end(), inserter(sd1, sd1.begin()));

				for (const auto& it : sd1) {
					oprestmp.content_map->insert(
						make_pair(it.deepAssign(),
							op1->content_map->operator [](it).deepAssign()));

				}
			}
			else if (op2->dataFlagBit == MAP) {

				set<Data> op1__;
				set<Data> op2__;
				for (auto itmap : *op1->content_map) {
					op1__.insert(itmap.first);

				}
				for (auto itmap : *op2->content_map) {
					op2__.insert(itmap.first);

				}
				set<Data> sd1;

				set_intersection(op1__.begin(), op1__.end(), op2__.begin(),
					op2__.end(), inserter(sd1, sd1.begin()));

				for (const auto& it : sd1) {
					oprestmp.content_map->insert(
						make_pair(it.deepAssign(),
							op1->content_map->operator [](it).deepAssign()));

				}
			}
			else if (op2->dataFlagBit == MULTIMAP) {

				set<Data> op1__;
				set<Data> op2__;
				for (auto itmap : *op1->content_map) {
					op1__.insert(itmap.first);

				}
				for (auto itmap : *op2->content_map) {
					op2__.insert(itmap.first);

				}
				set<Data> sd1;

				set_intersection(op1__.begin(), op1__.end(), op2__.begin(),
					op2__.end(), inserter(sd1, sd1.begin()));

				for (const auto& it : sd1) {
					oprestmp.content_map->insert(
						make_pair(it.deepAssign(),
							op1->content_map->operator [](it).deepAssign()));

				}
			}
			opres->operator =(oprestmp);

		}
		else if (op1->dataFlagBit == MULTIMAP) {

			Data oprestmp;

			oprestmp.dataFlagBit = MULTIMAP;
			oprestmp.content_multimap = DataMultiMapPtr(new map<Data, multiset<Data>>());

			if (op2->dataFlagBit == Sz) {
				if (op1->content_multimap->count(*op2) > 0) {
					oprestmp.content_multimap->insert(
						make_pair(op2->deepAssign(),
							Data::deepAssignSET(
								op1->content_multimap->operator [](
									*op2))));
				}

			}
			else if (op2->dataFlagBit == Zf) {
				if (op1->content_multimap->count(*op2) > 0) {
					oprestmp.content_multimap->insert(
						make_pair(op2->deepAssign(),
							Data::deepAssignSET(
								op1->content_multimap->operator [](
									*op2))));
				}

			}
			else if (op2->dataFlagBit == COOL_LST) {

				multiset<Data> op1__;
				multiset<Data> op2__;
				for (auto itmap : *op1->content_multimap) {
					op1__.insert(itmap.first);

				}

				op2__.insert(op2->content_lst->begin(),
					op2->content_lst->end());
				set<Data> sd1;

				set_intersection(op1__.begin(), op1__.end(), op2__.begin(),
					op2__.end(), inserter(sd1, sd1.begin()));

				for (const auto& it : sd1) {
					oprestmp.content_multimap->insert(
						make_pair(it.deepAssign(),
							Data::deepAssignSET(
								op1->content_multimap->operator [](
									*op1))));

				}
			}
			else if (op2->dataFlagBit == SET) {

				multiset<Data> op1__;
				set<Data>& op2__ = *op2->content_set;
				for (auto itmap : *op1->content_multimap) {
					op1__.insert(itmap.first);

				}
				set<Data> sd1;

				set_intersection(op1__.begin(), op1__.end(), op2__.begin(),
					op2__.end(), inserter(sd1, sd1.begin()));

				for (const auto& it : sd1) {
					oprestmp.content_multimap->insert(
						make_pair(it.deepAssign(),
							Data::deepAssignSET(
								op1->content_multimap->operator [](
									it))));

				}
			}
			else if (op2->dataFlagBit == MULTISET) {

				multiset<Data> op1__;
				multiset<Data>& op2__ = *op2->content_multiset;
				for (auto itmap : *op1->content_multimap) {
					op1__.insert(itmap.first);

				}
				set<Data> sd1;

				set_intersection(op1__.begin(), op1__.end(), op2__.begin(),
					op2__.end(), inserter(sd1, sd1.begin()));

				for (const auto& it : sd1) {
					oprestmp.content_multimap->insert(
						make_pair(it.deepAssign(),
							Data::deepAssignSET(
								op1->content_multimap->operator [](
									it))));

				}
			}
			else if (op2->dataFlagBit == MAP) {

				set<Data> op1__;
				set<Data> op2__;
				for (auto itmap : *op1->content_multimap) {
					op1__.insert(itmap.first);

				}
				for (auto itmap : *op2->content_map) {
					op2__.insert(itmap.first);

				}
				set<Data> sd1;

				set_intersection(op1__.begin(), op1__.end(), op2__.begin(),
					op2__.end(), inserter(sd1, sd1.begin()));

				for (const auto& it : sd1) {
					oprestmp.content_multimap->insert(
						make_pair(it.deepAssign(),
							Data::deepAssignSET(
								op1->content_multimap->operator [](
									it))));

				}
			}
			else if (op2->dataFlagBit == MULTIMAP) {

				set<Data> op1__;
				set<Data> op2__;
				for (auto itmap : *op1->content_multimap) {
					op1__.insert(itmap.first);

				}
				for (auto itmap : *op2->content_map) {
					op2__.insert(itmap.first);

				}
				set<Data> sd1;

				set_intersection(op1__.begin(), op1__.end(), op2__.begin(),
					op2__.end(), inserter(sd1, sd1.begin()));

				for (const auto& it : sd1) {
					oprestmp.content_multimap->insert(
						make_pair(it.deepAssign(),
							Data::deepAssignSET(
								op1->content_multimap->operator [](
									it))));

				}
			}
			opres->clear();
			opres->operator =(oprestmp);

		}
		else {

			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

			}



		}

	}

	void Executor::ACC(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg pos, Intg& type_a1,
		Intg& type_a2, Intg& type_res) {
		DataPtr op1 = op1_;
		DataPtr op2 = op2_;
		DataPtr opres = opres_;

		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}
		while (opres->dataFlagBit == S_PTR) {
			opres = opres->content_ptr;
		}
		DataPtr indexdata = nullptr;
		if (op2->dataFlagBit == S_LST) {
			indexdata = &op2->content_lst->operator [](pos);
			while (indexdata->dataFlagBit == S_PTR) {
				indexdata = indexdata->content_ptr;
			}
		}
		else {
			indexdata = op2;
		}

		if (op1->dataFlagBit == Zf) {
			opres->clear();
			opres->dataFlagBit = Zf;
			opres->content_s = op1->content_s.operator [](
				(Intg)indexdata->valueToNumb());
		}
		else if (op1->dataFlagBit == COOL_LST) {
			opres->clear();
			auto it = op1->content_lst->begin();
			Intg index = (Intg)indexdata->valueToNumb();
			while (index-- > 0) {
				++it;
			}
			DataPtr oprestemp = &it.operator *();
			while (oprestemp->dataFlagBit == S_PTR) {
				oprestemp = oprestemp->content_ptr;
			}
			if ((oprestemp->dataFlagBit == COOL_LST || oprestemp->dataFlagBit == SET
				|| oprestemp->dataFlagBit == MULTISET
				|| oprestemp->dataFlagBit == MAP
				|| oprestemp->dataFlagBit == MULTIMAP)
				&& op2->dataFlagBit == COOL_LST
				&& (pos < op2->content_lst->size() - 1)) {
				ACC(oprestemp, op2, opres, ++pos, type_a1, type_a2, type_res);

			}
			else {
				opres->dataFlagBit = S_PTR;
				opres->content_ptr = oprestemp;

			}

		}
		else if (op1->dataFlagBit == SET) {
			opres->clear();
			auto it = op1->content_set->begin();
			Intg index = (Intg)indexdata->valueToNumb();
			while (index-- > 0) {
				++it;
			}
			DataPtr oprestemp = DataPtr(&it.operator *());
			while (oprestemp->dataFlagBit == S_PTR) {
				oprestemp = oprestemp->content_ptr;
			}
			if ((oprestemp->dataFlagBit == COOL_LST || oprestemp->dataFlagBit == SET
				|| oprestemp->dataFlagBit == MULTISET
				|| oprestemp->dataFlagBit == MAP
				|| oprestemp->dataFlagBit == MULTIMAP)
				&& op2->dataFlagBit == COOL_LST
				&& (pos < (Intg)(op2->content_lst->size()) - 1)) {
				ACC(oprestemp, op2, opres, ++pos, type_a1, type_a2, type_res);

			}
			else {
				opres->operator =(oprestemp->deepAssign());

			}

		}
		else if (op1->dataFlagBit == MULTISET) {
			opres->clear();
			auto it = op1->content_multiset->begin();
			Intg index = (Intg)indexdata->valueToNumb();
			while (index-- > 0) {
				++it;
			}
			DataPtr oprestemp = DataPtr(&it.operator *());
			while (oprestemp->dataFlagBit == S_PTR) {
				oprestemp = oprestemp->content_ptr;
			}
			if ((oprestemp->dataFlagBit == COOL_LST || oprestemp->dataFlagBit == SET
				|| oprestemp->dataFlagBit == MULTISET
				|| oprestemp->dataFlagBit == MAP
				|| oprestemp->dataFlagBit == MULTIMAP)
				&& op2->dataFlagBit == COOL_LST
				&& (pos < (Intg)(op2->content_lst->size()) - 1)) {
				ACC(oprestemp, op2, opres, ++pos, type_a1, type_a2, type_res);

			}
			else {
				opres->operator =(oprestemp->deepAssign());

			}

		}
		else if (op1->dataFlagBit == MAP) {
			opres->clear();

			DataPtr oprestemp = &op1->content_map->operator [](*indexdata);
			while (oprestemp->dataFlagBit == S_PTR) {
				oprestemp = oprestemp->content_ptr;
			}
			if ((oprestemp->dataFlagBit == COOL_LST || oprestemp->dataFlagBit == SET
				|| oprestemp->dataFlagBit == MULTISET
				|| oprestemp->dataFlagBit == MAP
				|| oprestemp->dataFlagBit == MULTIMAP)
				&& op2->dataFlagBit == COOL_LST
				&& (pos < (Intg)(op2->content_lst->size()) - 1)) {
				ACC(oprestemp, op2, opres, ++pos, type_a1, type_a2, type_res);

			}
			else {
				opres->dataFlagBit = S_PTR;
				opres->content_ptr = oprestemp;
			}

		}
		else if (op1->dataFlagBit == MULTIMAP) {
			opres->clear();

			auto* multisettemp = &op1->content_multimap->operator [](
				*indexdata);
			Data oprestemp;
			oprestemp.dataFlagBit = MULTISET;
			oprestemp.content_multiset = multisettemp;

			if (op2->dataFlagBit == COOL_LST
				&& pos < (Intg)(op2->content_lst->size()) - 1) {
				ACC(&oprestemp, op2, opres, ++pos, type_a1, type_a2, type_res);

			}
			else {
				opres->isReferenceVar = true;
				opres->dataFlagBit = MULTISET;
				opres->content_multiset = multisettemp;
			}

		}

	}
	void Executor::ERS(DataPtr op1_, DataPtr op2_, Intg pos, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {

		DataPtr op1 = op1_;
		DataPtr op2 = op2_;

		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}

		DataPtr indexdata = nullptr;
		if (op2->dataFlagBit == S_LST) {
			indexdata = &op2->content_lst->operator [](pos);
			while (indexdata->dataFlagBit == S_PTR) {
				indexdata = indexdata->content_ptr;
			}
		}
		else {
			indexdata = op2;
		}

		if (op1->dataFlagBit == Zf) {
			op1->content_s.erase((Intg)indexdata->valueToNumb(), 1);
		}
		else if (op1->dataFlagBit == COOL_LST) {

			auto it = op1->content_lst->begin();
			Intg index = (Intg)indexdata->valueToNumb();
			while (index-- > 0) {
				++it;
			}
			DataPtr op1temp = &it.operator *();
			while (op1temp->dataFlagBit == S_PTR) {
				op1temp = op1temp->content_ptr;
			}
			if ((op1temp->dataFlagBit == COOL_LST || op1temp->dataFlagBit == SET
				|| op1temp->dataFlagBit == MULTISET
				|| op1temp->dataFlagBit == MAP
				|| op1temp->dataFlagBit == MULTIMAP)
				&& op2->dataFlagBit == COOL_LST
				&& pos < (Intg)op2->content_lst->size() - 1) {
				ERS(op1temp, op2, ++pos, type_a1, type_a2, type_res);

			}
			else {
				op1temp->clear();
				op1->content_lst->erase(it);

			}

		}
		else if (op1->dataFlagBit == SET) {

			auto it = op1->content_set->find(*indexdata);
			if (it == op1->content_set->end()) {
				return;
			}

			DataPtr op1temp = DataPtr(&it.operator *());
			while (op1temp->dataFlagBit == S_PTR) {
				op1temp = op1temp->content_ptr;
			}
			if ((op1temp->dataFlagBit == COOL_LST || op1temp->dataFlagBit == SET
				|| op1temp->dataFlagBit == MULTISET
				|| op1temp->dataFlagBit == MAP
				|| op1temp->dataFlagBit == MULTIMAP)
				&& op2->dataFlagBit == COOL_LST
				&& pos < (Intg)(op2->content_lst->size()) - 1) {
				ERS(op1temp, op2, 1 + pos, type_a1, type_a2, type_res);

			}
			else {
				Data eraseTmp = *op1temp;
				op1->content_set->erase(it);
				eraseTmp.clear();
			}

		}
		else if (op1->dataFlagBit == MULTISET) {

			auto it = op1->content_multiset->find(*indexdata);
			auto itstore = it;
			++itstore;
			if (it == op1->content_multiset->end()) {
				return;
			}
			Intg num = op1->content_multiset->count(*indexdata);
			while (num-- > 0) {

				DataPtr op1temp = DataPtr(&it.operator *());
				while (op1temp->dataFlagBit == S_PTR) {
					op1temp = op1temp->content_ptr;
				}
				if ((op1temp->dataFlagBit == COOL_LST || op1temp->dataFlagBit == SET
					|| op1temp->dataFlagBit == MULTISET
					|| op1temp->dataFlagBit == MAP
					|| op1temp->dataFlagBit == MULTIMAP)
					&& op2->dataFlagBit == COOL_LST
					&& pos < (Intg)(op2->content_lst->size()) - 1) {
					ERS(op1temp, op2, 1 + pos, type_a1, type_a2, type_res);

				}
				else {
					Data eraseTmp = *op1temp;
					op1->content_multiset->erase(it);
					eraseTmp.clear();
				}
				it = itstore;
				++itstore;
			}

		}
		else if (op1->dataFlagBit == MAP) {

			auto it = op1->content_map->find(*indexdata);
			DataPtr op1temp = &op1->content_map->operator [](*indexdata);
			while (op1temp->dataFlagBit == S_PTR) {
				op1temp = op1temp->content_ptr;
			}
			if ((op1temp->dataFlagBit == COOL_LST || op1temp->dataFlagBit == SET
				|| op1temp->dataFlagBit == MULTISET
				|| op1temp->dataFlagBit == MAP
				|| op1temp->dataFlagBit == MULTIMAP)
				&& op2->dataFlagBit == COOL_LST
				&& pos < (Intg)(op2->content_lst->size()) - 1) {
				ERS(op1temp, op2, 1 + pos, type_a1, type_a2, type_res);

			}
			else {
				op1temp->clear();
				Data keytemp = it->first;
				op1->content_map->erase(it);
				keytemp.clear();
			}

		}
		else if (op1->dataFlagBit == MULTIMAP) {

			auto it = op1->content_multimap->find(*indexdata);
			auto* multisettemp = &op1->content_multimap->operator [](
				*indexdata);
			Data valuedata;
			valuedata.dataFlagBit = MULTISET;
			valuedata.content_multiset = multisettemp;

			DataPtr op1temp = &valuedata;
			while (op1temp->dataFlagBit == S_PTR) {
				op1temp = op1temp->content_ptr;
			}
			if ((op1temp->dataFlagBit == COOL_LST || op1temp->dataFlagBit == SET
				|| op1temp->dataFlagBit == MULTISET
				|| op1temp->dataFlagBit == MAP
				|| op1temp->dataFlagBit == MULTIMAP)
				&& op2->dataFlagBit == COOL_LST
				&& pos < (Intg)(op2->content_lst->size()) - 1) {
				ERS(op1temp, op2, 1 + pos, type_a1, type_a2, type_res);

			}
			else {
				//don't call op1temp.clear(), its content_multimap not on heap.
				for (auto dt : *op1temp->content_multiset) {

					//clear the copy of element of op1temp->content_multiset,
					//then clear the op1temp.
					dt.clear();
				}
				Data keytemp = it->first;
				op1->content_multimap->erase(it);
				keytemp.clear();
			}

		}

	}


	Intg Executor::FND(DataPtr op1_, DataPtr op2_, Intg& type_a1, Intg& type_a2,
		Intg& type_res, DataPtr d2) {

		DataPtr op1 = op1_;
		DataPtr op2 = op2_;

		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}

		if (op1->dataFlagBit == Zf) {

			Intg pos = op1->content_s.find(op2->valueToStrg());

			return pos;

		}
		else if (op1->dataFlagBit == Sz) {
			if (op1->content_i == op2->valueToNumb()) {
				return 0;
			}
			else {
				return -1;
			}

		}
		else if (op1->dataFlagBit == COOL_LST) {
			Intg pos = 0;
			auto it = op1->content_lst->begin();
			while (it != op1->content_lst->end()) {
				if (it.operator *().operator ==(*op2)) {
					return pos;
				}
				++it;
				++pos;
			}
			return -1;

		}
		else if (op1->dataFlagBit == SET) {
			Intg pos = 0;
			auto itdes = op1->content_set->find(*d2);
			if (itdes == op1->content_set->end()) {
				return -1;
			}
			auto it = op1->content_set->begin();
			while (it != op1->content_set->end()) {
				if (it == itdes) {
					return pos;
				}
				++it;
				++pos;
			}
			return -1;

		}
		else if (op1->dataFlagBit == MULTISET) {
			Intg pos = 0;
			auto itdes = op1->content_multiset->find(*d2);
			if (itdes == op1->content_multiset->end()) {
				return -1;
			}
			auto it = op1->content_multiset->begin();
			while (it != op1->content_multiset->end()) {
				if (it == itdes) {
					return pos;
				}
				++it;
				++pos;
			}
			return -1;

		}
		else if (op1->dataFlagBit == MAP) {
			Intg pos = 0;
			auto itdes = op1->content_map->find(*d2);
			if (itdes == op1->content_map->end()) {
				return -1;
			}
			auto it = op1->content_map->begin();
			while (it != op1->content_map->end()) {
				if (it == itdes) {
					return pos;
				}
				++it;
				++pos;
			}
			return -1;

		}
		else if (op1->dataFlagBit == MULTIMAP) {
			Intg pos = 0;
			auto itdes = op1->content_multimap->find(*d2);
			if (itdes == op1->content_multimap->end()) {
				return -1;
			}
			auto it = op1->content_multimap->begin();
			while (it != op1->content_multimap->end()) {
				if (it == itdes) {
					return pos;
				}
				++it;
				++pos;
			}
			return -1;

		}
		else if (op1->dataFlagBit == S_AR && op2->dataFlagBit == S_AR) {
			if (op1->operator ==(*op2)) {
				return 0;
			}
			else {
				return -1;
			}
		}
		return -1;
	}

	/**
	 * @brief if you pass a list as the param , if the size of the list is two,
	 * the first element of the list will be regarded as the positon or key,
	 *  and the second element will be regarded as the value.this law is only suitable
	 *  for Strg or list/map/multimap, for other, the list whole will be regarded as an element
	 */
	void Executor::INS(DataPtr op1_, DataPtr op2_, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {

		DataPtr op1 = op1_;
		DataPtr op2 = op2_;

		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}

		if (op1->dataFlagBit == Sz || op1->dataFlagBit == S_AR) {
#if debug
			{
				static int ignorecount = 0;
				std::cerr << "Executor::executeCurrentCode()"
					<< " undefined operation NUM.INSERT(*), exit(-1) "
					<< "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")"
					<< std::endl;
			}
#endif
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
			assert(false); exit(-1);

		}
		else if (op1->dataFlagBit == Zf) {
			if (op2->dataFlagBit == COOL_LST && op2->content_lst->size() == 2) {
				op1->content_s.insert(
					(Intg)(op2->content_lst->at(0).valueToNumb()),
					op2->content_lst->at(1).valueToStrg());
			}
			else {
				op1->content_s.append(op2->valueToStrg());
			}

		}
		else if (op1->dataFlagBit == COOL_LST) {
			if (op2->dataFlagBit == COOL_LST && op2->content_lst->size() == 2) {
				op1->content_lst->insert(
					op1->content_lst->begin()
					+ (Intg)(op2->content_lst->at(0).valueToNumb()),
					op2->content_lst->at(1).deepAssign());
			}
			else {
				op1->content_lst->push_back(op2->deepAssign());
			}

		}
		else if (op1->dataFlagBit == MAP) {
			if (op2->dataFlagBit == COOL_LST && op2->content_lst->size() == 2) {
				op1->content_map->insert(
					make_pair(op2->content_lst->at(0).deepAssign(),
						op2->content_lst->at(1).deepAssign()));
			}
			else {
#if debug
				{
					static int ignorecount = 0;
					std::cerr << "Executor::executeCurrentCode()"
						<< " undefined operation MAP.INSERT({*}), exit(-1) "
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")"
						<< std::endl;
				}
#endif
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
				assert(false); exit(-1);
			}

		}
		else if (op1->dataFlagBit == MULTIMAP) {
			if (op2->dataFlagBit == COOL_LST && op2->content_lst->size() == 2) {
				auto it = op1->content_multimap->find(op2->content_lst->at(0));
				if (it == op1->content_multimap->end()) {
					op1->content_multimap->insert(
						make_pair(op2->content_lst->at(0).deepAssign(),
							multiset<Data> {
						op2->content_lst->at(1).deepAssign() }));
				}
				else {
					it->second.insert(op2->content_lst->at(1).deepAssign());
				}
			}
			else {
#if debug
				{
					static int ignorecount = 0;
					std::cerr << "Executor::executeCurrentCode()"
						<< " undefined operation MAP.INSERT({*}), exit(-1) "
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")"
						<< std::endl;
				}
#endif
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
				assert(false); exit(-1);
			}

		}
		else if (op1->dataFlagBit == SET) {

			auto dup = op2->deepAssign();
			if (op2->dataFlagBit == COOL_LST) {
				op1->content_set->insert(dup.content_lst->begin(),
					dup.content_lst->end());
			}
			else if (op2->dataFlagBit == SET) {

				op1->content_set->insert(dup.content_set->begin(),
					dup.content_set->end());

			}
			else if (op2->dataFlagBit == MULTISET) {

				op1->content_set->insert(dup.content_multiset->begin(),
					dup.content_multiset->end());

			}
			else {
				op1->content_set->insert(dup);

			}
		}
		else if (op1->dataFlagBit == MULTISET) {

			auto dup = op2->deepAssign();
			if (op2->dataFlagBit == COOL_LST) {
				op1->content_multiset->insert(dup.content_lst->begin(),
					dup.content_lst->end());
			}
			else if (op2->dataFlagBit == SET) {

				op1->content_multiset->insert(dup.content_set->begin(),
					dup.content_set->end());

			}
			else if (op2->dataFlagBit == MULTISET) {

				op1->content_multiset->insert(dup.content_multiset->begin(),
					dup.content_multiset->end());

			}
			else {
				op1->content_multiset->insert(dup);

			}

		}
	}
	;


	/**
	 * @brief for map/set/multiset, this function is the same as INSERT
	 */
	void Executor::PSB(DataPtr op1_, DataPtr op2_, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {

		DataPtr op1 = op1_;
		DataPtr op2 = op2_;

		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}

		if (op1->dataFlagBit == Sz || op1->dataFlagBit == S_AR) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid type");

			}

		}
		else if (op1->dataFlagBit == Zf) {

			op1->content_s.append(op2->valueToStrg());

		}
		else if (op1->dataFlagBit == COOL_LST) {

			op1->content_lst->push_back(op2->deepAssign());

		}
		else if (op1->dataFlagBit == MAP) {
			if (op2->dataFlagBit == COOL_LST && op2->content_lst->size() == 2) {
				op1->content_map->insert(
					make_pair(op2->content_lst->at(0).deepAssign(),
						op2->content_lst->at(1).deepAssign()));
			}
			else {
#if debug
				{
					static int ignorecount = 0;
					std::cerr << "Executor::executeCurrentCode()"
						<< " undefined operation MAP.PUSHBACK({*}), exit(-1) "
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")"
						<< std::endl;
				}
#endif
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
				assert(false); exit(-1);
			}

		}
		else if (op1->dataFlagBit == MULTIMAP) {
			if (op2->dataFlagBit == COOL_LST && op2->content_lst->size() == 2) {
				auto it = op1->content_multimap->find(op2->content_lst->at(0));
				if (it == op1->content_multimap->end()) {
					op1->content_multimap->insert(
						make_pair(op2->content_lst->at(0).deepAssign(),
							multiset<Data> {
						op2->content_lst->at(1).deepAssign() }));
				}
			}
			else {
#if debug
				{
					static int ignorecount = 0;
					std::cerr << "Executor::executeCurrentCode()"
						<< " undefined operation MAP.PUSHBACK({*}), exit(-1) "
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")"
						<< std::endl;
				}
#endif
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
				assert(false); exit(-1);
			}

		}
		else if (op1->dataFlagBit == SET) {
			if (op1->content_set->count(*op2) <= 0) {
				op1->content_set->insert(op2->deepAssign());
			}
		}
		else if (op1->dataFlagBit == MULTISET) {

			op1->content_set->insert(op2->deepAssign());

		}
	}
	void Executor::PSF(DataPtr op1_, DataPtr op2_, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {

		DataPtr op1 = op1_;
		DataPtr op2 = op2_;

		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}

		if (op1->dataFlagBit == Sz || op1->dataFlagBit == S_AR) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid type");

			}

		}
		else if (op1->dataFlagBit == Zf) {

			op1->content_s.insert(0, op2->valueToStrg());

		}
		else if (op1->dataFlagBit == COOL_LST) {

			op1->content_lst->push_front(op2->deepAssign());

		}
		else if (op1->dataFlagBit == MAP) {
			if (op2->dataFlagBit == COOL_LST && op2->content_lst->size() == 2) {
				op1->content_map->insert(
					make_pair(op2->content_lst->at(0).deepAssign(),
						op2->content_lst->at(1).deepAssign()));
			}
			else {
				if (this->mode == MODE_GROUNDING) {
					cout << "Ignore calculation errors in reasoning." << endl;
					return;
				}
				else {
					THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid type");

				}
			}

		}
		else if (op1->dataFlagBit == MULTIMAP) {
			if (op2->dataFlagBit == COOL_LST && op2->content_lst->size() == 2) {
				auto it = op1->content_multimap->find(op2->content_lst->at(0));
				if (it == op1->content_multimap->end()) {
					op1->content_multimap->insert(
						make_pair(op2->content_lst->at(0).deepAssign(),
							multiset<Data> {
						op2->content_lst->at(1).deepAssign() }));
				}
			}
			else {
				if (this->mode == MODE_GROUNDING) {
					cout << "Ignore calculation errors in reasoning." << endl;
					return;
				}
				else {
					THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid type");

				}
			}

		}
		else if (op1->dataFlagBit == SET) {
			if (op1->content_set->count(*op2) <= 0) {
				op1->content_set->insert(op2->deepAssign());
			}
		}
		else if (op1->dataFlagBit == MULTISET) {

			op1->content_set->insert(op2->deepAssign());

		}
	}
	void Executor::PPB(DataPtr op1_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {

		DataPtr op1 = op1_;
		DataPtr opres = opres_;

		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (opres->dataFlagBit == S_PTR) {
			opres = opres->content_ptr;
		}

		if (op1->dataFlagBit == Sz || op1->dataFlagBit == S_AR) {
			if (opres != op1) {

				opres->operator =(*op1);
				op1->content_ar = nullptr;
				op1->content_s = "";
			}
			else {
				return;
			}

		}
		else if (op1->dataFlagBit == Zf) {
			Strg popout;
			popout.operator +=(op1->content_s.back());
			opres->clear();
			opres->dataFlagBit = Zf;
			opres->content_s = popout;

			op1->content_s.pop_back();

		}
		else if (op1->dataFlagBit == COOL_LST) {

			Data popout = op1->content_lst->back();
			op1->content_lst->pop_back();
			opres->operator =(popout);

		}
		else if (op1->dataFlagBit == MAP) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid type");

			}

		}
		else if (op1->dataFlagBit == MULTIMAP) {

			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid type");

			}

		}
		else if (op1->dataFlagBit == SET) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid type");

			}

		}
		else if (op1->dataFlagBit == MULTISET) {

			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid type");

			}
		}
	}
	void Executor::PPF(DataPtr op1_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {

		DataPtr op1 = op1_;
		DataPtr opres = opres_;

		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (opres->dataFlagBit == S_PTR) {
			opres = opres->content_ptr;
		}

		if (op1->dataFlagBit == Sz || op1->dataFlagBit == S_AR) {
			if (opres != op1) {

				opres->operator =(*op1);
				op1->content_ar = nullptr;
				op1->content_s = "";
			}
			else {
				return;
			}

		}
		else if (op1->dataFlagBit == Zf) {
			Strg popout;
			popout.operator +=(op1->content_s.front());
			opres->clear();
			opres->dataFlagBit = Zf;
			opres->content_s = popout;

			op1->content_s.erase(0, 1);

		}
		else if (op1->dataFlagBit == COOL_LST) {

			Data popout = op1->content_lst->front();
			op1->content_lst->pop_front();
			opres->operator =(popout);

		}
		else if (op1->dataFlagBit == MAP) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid type");

			}

		}
		else if (op1->dataFlagBit == MULTIMAP) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid type");

			}

		}
		else if (op1->dataFlagBit == SET) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid type");

			}

		}
		else if (op1->dataFlagBit == MULTISET) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid type");

			}

		}
	}
	/**
	 * @brief this function try to return the inference of the back of the last element of the op1.
	 * if can't ,return the duplicate value of the last element.
	 */
	void Executor::BCK(DataPtr op1_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {

		DataPtr op1 = op1_;
		DataPtr opres = opres_;

		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (opres->dataFlagBit == S_PTR) {
			opres = opres->content_ptr;
		}

		if (op1->dataFlagBit == Sz || op1->dataFlagBit == S_AR) {
			if (opres != op1) {

				opres->operator =(*op1);
				op1->content_ar = nullptr;
				op1->content_s = "";
			}
			else {
				return;
			}

		}
		else if (op1->dataFlagBit == Zf) {
			Strg back_;
			back_.operator +=(op1->content_s.back());
			opres->clear();
			opres->dataFlagBit = Zf;
			opres->content_s = back_;

		}
		else if (op1->dataFlagBit == COOL_LST) {
			if (op1 == opres) {
				Data back_ = op1->content_lst->back();
				op1->content_lst->pop_back();
				op1->clear();
				opres = &back_;
			}
			else {

				opres->clear();
				opres->dataFlagBit = S_PTR;
				opres->content_ptr = &op1->content_lst->back();
			}

		}
		else if (op1->dataFlagBit == MAP) {
#if debug
			{
				static int ignorecount = 0;
				std::cerr << "Executor::executeCurrentCode()"
					<< " undefined operation MAP.BACK(), exit(-1) "
					<< "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")"
					<< std::endl;
			}
#endif
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
			assert(false); exit(-1);

		}
		else if (op1->dataFlagBit == MULTIMAP) {

#if debug
			{
				static int ignorecount = 0;
				std::cerr << "Executor::executeCurrentCode()"
					<< " undefined operation MULTIMAP.BACK(), exit(-1) "
					<< "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")"
					<< std::endl;
			}
#endif
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
			assert(false); exit(-1);

		}
		else if (op1->dataFlagBit == SET) {
#if debug
			{
				static int ignorecount = 0;
				std::cerr << "Executor::executeCurrentCode()"
					<< " undefined operation SET.BACK(), exit(-1) "
					<< "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")"
					<< std::endl;
			}
#endif
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
			assert(false); exit(-1);

		}
		else if (op1->dataFlagBit == MULTISET) {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Invalid type");

			}


		}
	}
	void Executor::MUL(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {
		DataPtr op1 = op1_;
		DataPtr op2 = op2_;
		DataPtr opres = opres_;
		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}
		while (opres->dataFlagBit == S_PTR) {
			opres = opres->content_ptr;
		}
		if (opres != op1) {
			opres->clear();
		}

		if (type_a1 == Sz) {
			if (opres != op1) {
				(*opres) = op1->deepAssign();
			}
			if (type_a2 == Sz) {
				(opres->content_i) = (op1->content_i) * (op2->content_i);
			}
			else if (type_a2 == Zf) {
				(opres->content_i) = (op1->content_i) * toNumb(op2->content_s);
			}
			else {
				Numb op2val = 1;
				try
				{
					op2val = op2->valueToNumb();
				}
				catch (const CalculationException& e)
				{
					if (this->mode == MODE_GROUNDING) {
						cout << "Ignore calculation errors in reasoning: " << e.what() << " use 1 as default." << endl;
						op2val = 1;
					}
					else {
						throw;
					}
				}
				(opres->content_i) = (op1->content_i) * op2val/*op2->valueToNumb()*/;
			}

		}
		else if (type_a1 == Zf) {
			if (opres != op1) {
				(*opres) = op1->deepAssign();
			}
			opres->clear();
			if (type_a2 == Zf || type_a2 == Sz) {
				Intg duptime = op2->valueToNumb();
				if (duptime == 0) {
					opres->clear();
					opres->dataFlagBit = Zf;
				}
				--duptime;
				auto strgdup = opres->content_s;
				while (duptime-- > 0) {
					op1->content_s.operator +=(strgdup);

				}

			}
			else if (type_a2 == COOL_LST) {
				vector<Strg> vd1;
				vector<Strg> vd2;
				vd2.push_back(op1->content_s);
				for (const auto& datait : *op2->content_lst) {
					Strg seg;
					if (datait.dataFlagBit == Sz) {
						seg = toStrg(datait.content_i);
					}
					else if (datait.dataFlagBit == Zf) {
						seg = datait.content_s;
					}
					if (seg == "") {
						continue;
					}
					vd1.swap(vd2);
					if (vd1.size() <= 0) {
						break;
					}
					for (auto& strgit : vd1) {
						if (strgit.size() <= 0) {
							continue;
						}
						else {
							auto&& splitv = split(strgit, seg);
							vd2.insert(vd2.end(), splitv.begin(), splitv.end());
						}

					}

					vd1.clear();

				}
				opres->clear();
				opres->dataFlagBit = Zf;
				for (auto& strgfrag : vd2) {
					opres->content_s.operator +=(strgfrag);
				}

			}
			else if (type_a2 == SET) {
				vector<Strg> vd1;
				vector<Strg> vd2;
				vd2.push_back(op1->content_s);
				for (const auto& datait : *op2->content_set) {
					Strg seg;
					if (datait.dataFlagBit == Sz) {
						seg = toStrg(datait.content_i);
					}
					else if (datait.dataFlagBit == Zf) {
						seg = datait.content_s;
					}
					if (seg == "") {
						continue;
					}
					vd1.swap(vd2);
					if (vd1.size() <= 0) {
						break;
					}
					for (auto& strgit : vd1) {
						if (strgit.size() <= 0) {
							continue;
						}
						else {
							auto&& splitv = split(strgit, seg);
							vd2.insert(vd2.end(), splitv.begin(), splitv.end());
						}

					}

					vd1.clear();

				}
				opres->clear();
				opres->dataFlagBit = Zf;
				for (auto& strgfrag : vd2) {
					opres->content_s.operator +=(strgfrag);
				}

			}
			else if (type_a2 == MULTISET) {
				vector<Strg> vd1;
				vector<Strg> vd2;
				vd2.push_back(op1->content_s);
				for (const auto& datait : *op2->content_multiset) {
					Strg seg;
					if (datait.dataFlagBit == Sz) {
						seg = toStrg(datait.content_i);
					}
					else if (datait.dataFlagBit == Zf) {
						seg = datait.content_s;
					}
					if (seg == "") {
						continue;
					}
					vd1.swap(vd2);
					if (vd1.size() <= 0) {
						break;
					}
					for (auto& strgit : vd1) {
						if (strgit.size() <= 0) {
							continue;
						}
						else {
							auto&& splitv = split(strgit, seg);
							vd2.insert(vd2.end(), splitv.begin(), splitv.end());
						}

					}

					vd1.clear();

				}
				opres->clear();
				opres->dataFlagBit = Zf;
				for (auto& strgfrag : vd2) {
					opres->content_s.operator +=(strgfrag);
				}

			}

		}
		else if (type_a1 == COOL_LST) {

			if (opres != op1) {

				(*opres) = op1->deepAssign();
			}
			if (type_a2 == Zf || type_a2 == Sz) {
				for (auto& elem : *opres->content_lst) {
					MUL(&elem, op2, &elem, type_a1, type_a2, type_res);
				}
			}
			else if (type_a2 == COOL_LST) {
				Intg len = min(opres->content_lst->size(),
					op2->content_lst->size());
				auto itopres = opres->content_lst->begin();
				auto itop2 = op2->content_lst->begin();
				while (len-- > 0) {
					MUL(DataPtr(&*itopres),
						DataPtr(&*itop2),
						DataPtr(&*itopres), type_a1, type_a2,
						type_res);
				}

			}
			else if (type_a2 == SET) {
				Intg len = min(opres->content_lst->size(),
					op2->content_set->size());
				auto itopres = opres->content_lst->begin();
				auto itop2 = op2->content_set->begin();
				while (len-- > 0) {
					MUL(DataPtr(&*itopres),
						DataPtr(&*itop2),
						DataPtr(&*itopres), type_a1, type_a2,
						type_res);
				}
			}
			else if (type_a2 == MULTISET) {
				Intg len = min(opres->content_lst->size(),
					op2->content_multiset->size());
				auto itopres = opres->content_lst->begin();
				auto itop2 = op2->content_multiset->begin();
				while (len-- > 0) {
					MUL(DataPtr(&*itopres),
						DataPtr(&*itop2),
						DataPtr(&*itopres), type_a1, type_a2,
						type_res);
				}
			}

		}
		else if (type_a1 == SET) {
#if debug
			{
				static int ignorecount = 0;
				std::cerr << "Executor::executeCurrentCode()"
					<< " undefined MUL*(set,*,*), exit(-1) "
					<< "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")"
					<< std::endl;
			}
#endif
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
			assert(false); exit(-1);

		}
		else if (type_a1 == MULTISET) {
#if debug
			{
				static int ignorecount = 0;
				std::cerr << "Executor::executeCurrentCode()"
					<< " undefined MUL*(multiset,*,*), exit(-1) "
					<< "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")"
					<< std::endl;
			}
#endif
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
			assert(false); exit(-1);

		}
		else if (type_a1 == MAP) {
#if debug
			{
				static int ignorecount = 0;
				std::cerr << "Executor::executeCurrentCode()"
					<< " undefined MUL*(map,*,*), exit(-1) "
					<< "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")"
					<< std::endl;
			}
#endif
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
			assert(false); exit(-1);

		}
		else if (type_a1 == MULTIMAP) {
#if debug
			{
				static int ignorecount = 0;
				std::cerr << "Executor::executeCurrentCode()"
					<< " undefined MUL*(multimap,*,*), exit(-1) "
					<< "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")"
					<< std::endl;
			}
#endif
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
			assert(false); exit(-1);

		}
		else {
			if (this->mode == MODE_GROUNDING) {
				cout << "Ignore calculation errors in reasoning." << endl;
				return;
			}
			else {
				THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");

			}
		}

	}
	Intg Executor::FIND_SUBEXPR(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {
		DataPtr op1 = op1_;
		DataPtr op2 = op2_;
		DataPtr opres = opres_;
		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}
		while (opres->dataFlagBit == S_PTR) {
			opres = opres->content_ptr;
		}
		if (opres != op1) {
			opres->clear();
		}

		if (op1->isExpression == false) {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "invalid lhs for FIND_EXPR, exit(-1)"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			cerr << "invalid lhs for FIND_SUBEXPR, exit(-1)" << endl;
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
			assert(false); exit(-1);
		}
		else {
			op1->expr->flatten((*currentAR)[Arg(S_Bs, "thisexpr")].expr, currentAR, currentAR->frmap, this->cdth, this->scpth);


		}
		if (op2->isExpression == false) {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "invalid rhs for FIND_SUBEXPR, exit(-1)"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			cerr << "invalid rhs for FIND_SUBEXPR, exit(-1)" << endl;
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
			assert(false); exit(-1);
		}
		else {
			op2->expr->flatten((*currentAR)[Arg(S_Bs, "thisexpr")].expr, currentAR, currentAR->frmap, this->cdth, this->scpth);
		}
		if (op1->expr->type == EXPRESSIONHANDLER_ARG) {
			if (op2->expr->type == EXPRESSIONHANDLER_ARG && op1->expr->expr_arg == op2->expr->expr_arg && op1->expr->expr_arg.asc == op2->expr->expr_arg.asc) {
				opres->deepAssignFrom(DataPtr::make(Arg(Sz, 1)));

				return true;
			}
			else {
				opres->deepAssignFrom(DataPtr::make(Arg(Sz, 0)));

				return false;
			}
		}
		// op1 is codetable
		if (op1->expr->expr_cdt.size() == 0) {
			opres->deepAssignFrom(DataPtr::make(Arg(Sz, 0)));
			return false;

		}
		FKA lastMatchedFKA = op1->expr->close_high_bound_exist ? op1->expr->close_high_bound : FKA(op1->expr->expr_cdt.fileKey, op1->expr->expr_cdt.addrdq.back());//root to top, the fka matched in the last time
		if (op1->expr->subtree_searching_progress_recording_map.count(op2->expr->serialNumber) > 0) {
			lastMatchedFKA = op1->expr->subtree_searching_progress_recording_map[op2->expr->serialNumber];
		}

		if (lastMatchedFKA == op1->expr->close_low_bound) {
			op1->expr->subtree_searching_progress_recording_map.erase(op2->expr->serialNumber);
			opres->deepAssignFrom(DataPtr::make(Arg(Sz, 0)));
			return 0;

		}

		auto startAddr = op1->expr->expr_cdt.getAboveCodeAddr(lastMatchedFKA.addr);
		auto placeholders = currentAR->getPlaceholders(op2->expr->expr_cdt);
		Matcher m = Matcher(&op1->expr->expr_cdt, &op2->expr->expr_cdt, this->tfth, startAddr, op2->expr->expr_cdt.addrdq.back(), MatchMode::SubExprSearchingPlaceholderBind, currentAR, cdth, placeholders);
		while (startAddr != addrnull) {
			if (m.match() == false && startAddr >= op1->expr->expr_cdt.addrdq.front()) {
				startAddr = op1->expr->expr_cdt.getAboveCodeAddr(startAddr);
				m.addrcmp = startAddr;
				m.reset();
			}
			else {
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

				//1 update searching progress map
				op1->expr->subtree_searching_progress_recording_map[op2->expr->serialNumber] = FKA(lastMatchedFKA.fileKey, startAddr);

				//2 update placeholders and their data
				for (auto& it : m.getMatchState().pcam) {
					Arg unbound_placeholder = currentAR->getKeyArg(it.first);
					Arg bound_placeholder = it.first;
					setFlagBits(bound_placeholder, unbound_placeholder);

					bound_placeholder.isPlaceholder = true;
					bound_placeholder.isExpression = true;
					bound_placeholder.isBound = true;

					currentAR->setKeyArg(bound_placeholder);

					DataPtr data = nullptr;
					Arg& cmp_arg = it.second;
					if (cmp_arg.isVar()) {
						if (currentAR->findall(cmp_arg)) {
							data = DataPtr::make();
							*data = (*currentAR)[cmp_arg];
						}
					}
					else {
						data = DataPtr::make(cmp_arg);
						data->isExpression = true;
						data->expr = ExpressionHandlerPtr::make(cmp_arg);
					}

					if (data->isExpression == false) {
						data->isExpression = true;
						if (cmp_arg.argFlag == S_Dz) {

							data->expr = ExpressionHandlerPtr::make(cdth->copyTreeBranch(cmp_arg.arg_fka));

						}
						else {
							data->expr = ExpressionHandlerPtr::make(cmp_arg);
						}
					}

					(*currentAR)[bound_placeholder] = *data;

				}
				opres->deepAssignFrom(DataPtr::make(Arg(Sz, 1)));
				return true;
			}

		}
		op1->expr->subtree_searching_progress_recording_map.erase(op2->expr->serialNumber);
		opres->deepAssignFrom(DataPtr::make(Arg(Sz, 0)));
		return false;


	}
	Intg Executor::EXIST_SUBEXPR(DataPtr op1_, DataPtr op2_, DataPtr opres_, Intg& type_a1, Intg& type_a2,
		Intg& type_res) {
		DataPtr op1 = op1_;
		DataPtr op2 = op2_;
		DataPtr opres = opres_;
		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}
		while (opres->dataFlagBit == S_PTR) {
			opres = opres->content_ptr;
		}
		if (opres != op1) {
			opres->clear();
		}

		if (op1->isExpression == false) {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "invalid lhs for FIND_EXPR, exit(-1)"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			cerr << "invalid lhs for FIND_SUBEXPR, exit(-1)" << endl;
			assert(false); exit(-1);
		}
		else {
			op1->expr->flatten((*currentAR)[Arg(S_Bs, "thisexpr")].expr, currentAR, currentAR->getFunctionCallFRMap(tfth, scpth), this->cdth, this->scpth);


		}
		if (op2->isExpression == false) {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "invalid rhs for FIND_SUBEXPR, exit(-1)"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			cerr << "invalid rhs for FIND_SUBEXPR, exit(-1)" << endl;
			assert(false); exit(-1);
		}
		else {
			op2->expr->flatten((*currentAR)[Arg(S_Bs, "thisexpr")].expr, currentAR, currentAR->getFunctionCallFRMap(tfth, scpth), this->cdth, this->scpth);
		}
		if (op1->expr->type == EXPRESSIONHANDLER_ARG) {
			if (op2->expr->type == EXPRESSIONHANDLER_ARG && op1->expr->expr_arg == op2->expr->expr_arg && op1->expr->expr_arg.asc == op2->expr->expr_arg.asc) {
				opres->deepAssignFrom(DataPtr::make(Arg(Sz, 1)));

				return true;
			}
			else {
				opres->deepAssignFrom(DataPtr::make(Arg(Sz, 0)));

				return false;
			}
		}

		// op1 is codetable
		if (op1->expr->expr_cdt.size() == 0) {
			opres->deepAssignFrom(DataPtr::make(Arg(Sz, 0)));
			return false;

		}

		if (op1->expr->open_high_bound_exist == false) {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "open high bound of the lhs in FIND_SUBEXPR must be true, exit(-1)."; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			cerr << "open high bound of the lhs in FIND_SUBEXPR must be true, exit(-1)." << endl;
			assert(false); exit(-1);
		}

		Addr startAddr = op1->expr->close_high_bound_exist ? op1->expr->close_high_bound.addr : op1->expr->expr_cdt.addrdq.back();//root to top, the fka matched in the last time
		auto placeholders = currentAR->getPlaceholders(op2->expr->expr_cdt);
		Matcher m = Matcher(&op1->expr->expr_cdt, &op2->expr->expr_cdt, this->tfth, startAddr, op2->expr->expr_cdt.addrdq.back(), MatchMode::SubExprSearchingPlaceholderBind, currentAR, cdth, placeholders);

		while (startAddr != addrnull) {

			if (m.match() == false) {
				startAddr = op1->expr->expr_cdt.getAboveCodeAddr(startAddr);
				m.addrcmp = startAddr;
				m.reset();
			}
			else {
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


				//1 update placeholders and their data
				for (auto& it : m.getMatchState().pcam) {
					Arg unbound_placeholder = currentAR->getKeyArg(it.first);
					Arg bound_placeholder = it.first;
					setFlagBits(bound_placeholder, unbound_placeholder);

					bound_placeholder.isPlaceholder = true;
					bound_placeholder.isExpression = true;
					bound_placeholder.isBound = true;

					currentAR->setKeyArg(bound_placeholder);

					DataPtr data = nullptr;
					Arg& cmp_arg = it.second;
					if (cmp_arg.isVar() && currentAR->findall(cmp_arg)) {
						data = DataPtr::make();
						*data = (*currentAR)[cmp_arg];
					}
					else {
						data = DataPtr::make(cmp_arg);
						data->isExpression = true;
						data->expr = ExpressionHandlerPtr::make(cmp_arg);
					}

					if (data->isExpression == false) {
						data->isExpression = true;
						if (cmp_arg.argFlag == S_Dz) {

							//data->expr = ExpressionHandlerPtr::make(cdth->copyTreeBranch(cmp_arg.arg_fka));
							Arg thisexpr_ = Arg(S_Bs, "thisexpr");
							auto& this_expr_cdt = (*currentAR)[thisexpr_].expr->expr_cdt;
							data->expr = ExpressionHandlerPtr::make(this_expr_cdt.copyTreeBranch(cmp_arg.arg_fka.addr));

						}
						else {
							data->expr = ExpressionHandlerPtr::make(cmp_arg);
						}
					}

					(*currentAR)[bound_placeholder] = *data;

				}
				opres->deepAssignFrom(DataPtr::make(Arg(Sz, 1)));
				return true;
			}

		}
		opres->deepAssignFrom(DataPtr::make(Arg(Sz, 0)));
		return false;


	}
	void Executor::RESET(const Arg& argexe1_, DataPtr op1_) {
		DataPtr op1 = op1_;


		/*while (opres->dataFlagBit == S_PTR) {
			opres = opres->content_ptr;
		}*/
		/*if (opres != op1) {
			opres->clear();
		}*/
		Arg arg = currentAR->getKeyArg(argexe1_);
		if (arg.isPlaceholder) {
			arg.isBound = false;
			op1->deepAssignFrom(DataPtr::make());
			currentAR->setKeyArg(arg);
		}
	}

	Executor::Executor() :executorAR(DataTablePtr::make(nullptr, nullptr, FKA(FILEKEYPRIMARY, Addr()))) {
		this->currentAR = this->executorAR;
	}


	Executor::Executor(HashCollection* hashCollection) :executorAR(DataTablePtr::make(nullptr, nullptr, FKA(FILEKEYPRIMARY, Addr()))) {

		this->cdth = &hashCollection->cdth;
		this->tfth = &hashCollection->tfth;
		this->scpth = &hashCollection->scpth;
		//Strg filekey = cdth->getMinFKA().fileKey;
		//Addr scopeaddr = this->scpth->file_scpt_map[filekey].addrdq.front();
		//executorAR = DataTablePtr::make(nullptr, nullptr, FKA(filekey, scopeaddr));
		this->currentAR = this->executorAR;
		this->systh = &hashCollection->systh;
		this->setting = &hashCollection->setting;
	}
	Executor::Executor(CodeTableHash* cdth,
		TemplateFunctionTableHash* tfth,
		ScopeTableHash* scpth,
		SystemTableHash* systh, Setting* setting) :executorAR(DataTablePtr::make(nullptr, nullptr, FKA(FILEKEYPRIMARY, Addr()))) {

		this->cdth = cdth;
		this->tfth = tfth;
		this->scpth = scpth;

		//Strg filekey = cdth->getMinFKA().fileKey;
		//Addr scopeaddr = this->scpth->file_scpt_map[filekey].addrdq.front();
		//executorAR = DataTablePtr::make(nullptr, nullptr, FKA(filekey, scopeaddr));


		this->currentAR = this->executorAR;
		this->systh = systh;
		this->setting = setting;
		/*this->matchStateSiloMaxCapacity = setting->matchStateSiloMaxCapacity;
		this->groundingStateSiloMaxCapacity = setting->groundingStateSiloMaxCapacity;
		this->maxSwitchTurn = setting->maxSwitchTurn;*/
	}
	Intg Executor::update_AR_stack() {
		// Step 1: Retrieve all ARs (including currentAR) and store them
		vector<DataTablePtr> arList;

		vector<FKA> arFKAList;
		DataTablePtr tempAR = currentAR;
		map<FKA, DataTablePtr> arDeleteNew;


		while (tempAR != nullptr) {
			arList.push_back(tempAR);
			arFKAList.push_back(tempAR->scopeStructureFKA);
			tempAR = tempAR->parentAR;
		}

		// Step 2: Clear the stack
		while (!arStack.empty()) {
			if (/*this->mode == MODE_GROUNDING && this->isExecuting == false*/true) {
				//erase this ar (and its name ar) from its parent ar if this ar is function call ar
				auto ar = arStack.top();
				auto arfka = ar->scopeStructureFKA;
				if (arfka != fkanull) {
					auto funcfka = this->tfth->getTFFKA(arfka, this->scpth);

					if (funcfka != fkanull) {
						if (arfka == tfth->operator[](funcfka).bodyScopeFKA) {
							if (find(arFKAList.begin(), arFKAList.end(), arfka) != arFKAList.end()) {
								for (auto s_ : arDeleteBuffer) {
									s_.erase(arfka);
									s_.erase(funcfka);
								}
							}
							else {
								arDeleteNew.insert(make_pair(arfka, ar));
								arDeleteNew.insert(make_pair(funcfka, ar));
							}
						}

					}
				}
			}
			arStack.pop();
		}




		// Step 3: Add new ARs to the stack
		for (auto ar : arList) {
			arStack.push(ar);
		}

		arDeleteBuffer.push_back(arDeleteNew);
		if (arDeleteBuffer.size() > 1) {

			auto& arDeleteExpired = arDeleteBuffer.front();
			for (auto& p : arDeleteExpired) {
				auto ar = p.second;
				auto fka = p.first;
				if (ar->parentAR != nullptr) {
					ar->parentAR->argdmap.erase(Arg(S_Dz, fka));
					ar->parentAR->argdmap.erase(Arg(S_AR, fka));
				}
			}

			arDeleteBuffer.pop_front();

		}
		// Return the depth of the stack
		return arStack.size();
	}
	Strg Executor::toBrief() const {
		stringstream ss;
		ss << hline;
		ss << "Executor::cdth:" << endl;
		ss << cdth->toStrg();
		ss << hline;
		ss << "Executor::currentAR:" << endl;
		ss << currentAR->toStrg();

		ss << hline;
		ss << "Executor::thisexpr:" << endl;
		if (currentAR->findall(Arg(S_Bs, "thisexpr")) == false) {
			ss << "Expr(null)" << endl;
		}
		else {
			ss << (*currentAR)[Arg(S_Bs, "thisexpr")].expr->toBrief();
		}
		ss << hline;
		ss << calign("CodeTrack:", 96) << endl;
		int tracklen = codetrack.size();
		int i = 0;
		while (i < tracklen) {
			ss << calign(toStrg(i), 10);
			ss << calign(ground_exe_track[i] + ":" + fkatrack[i].toStrg());
			ss << calign(codetrack[i].toStrg(), 72);
			ss << calign(artrack[i]);
			ss << "\n";
			++i;
		}
		ss << hline;

		return ss.str();

	}
	Strg Executor::ARtoBrief() const {
		return this->currentAR->toStrg();
	}
	/**
	 * @brief 在创建函数调用的ar时，利用此函数获得函数的parentAR
	 * @attention 此函数考虑了类函数的调用
	 * @param functionBodyScopeFKA
	 * @return
	 */
	DataTablePtr Executor::getParentAR(const FKA& functionBodyScopeFKA) {

#if debug
		cout << "Executor::getParentAR in, functionBodyScopeFKA:["
			<< functionBodyScopeFKA.toStrg() << "] in" << endl;
#endif

		if (functionBodyScopeFKA.fileKey == FILEKEYBUILTIN) {
			//内置函数不考虑
			return nullptr;
		}
		FKA& parentScopeFKA = (*scpth)[functionBodyScopeFKA].parentScopeFKA;
		Scope& parentScope = (*scpth)[parentScopeFKA];
		DataTablePtr parentAR = nullptr;
		if (parentScope.functionFlag != SYSB) {
			//非成员函数，直接递推寻找父ar
			parentAR = this->currentAR;
			while (parentAR != nullptr && parentAR->scopeStructureFKA != parentScopeFKA) {
				parentAR = parentAR->parentAR;
			}
			return parentAR;
		}
		else {
			//是成员函数，有可能是成员函数间内部互相调用，此时可以访问到的与父scope同addr的ar就是父ar
			parentAR = DataTable::getAccessibleAR(currentAR, parentScopeFKA);
			if (parentAR != nullptr) {
				return parentAR;
			}
			else {
				//是成员函数，且不是类内部互相调用的情况，需要在当前ar中寻找参与函数调用的类实例
				//从根节点向前推算，第一个遇到的引用将被作为函数调用的父ar
				FKI pos = cdth->getFKIByFKA(currentCodeFKA);
				FKI pos_start = pos;
				FKA& tffka = currentCode->assemblyFormula.flagBit.tfFKA;
				while (true) {
					Code& cd = cdth->operator [](pos);
					if (cd.assemblyFormula.flagBit.tfFKA != tffka
						|| (cd.assemblyFormula.flagBit.functionHandleFlag
							== true && pos != pos_start)) {
						break;
					}
					Arg&& arg1 = cd[0];
					Arg&& arg2 = cd[1];
					Arg&& argop = cd[2];
					Arg&& argres = cd[3];
					if (arg1.argFlag == Y_Dz) {
						arg1.ref_ar = currentAR;
						ReferenceData&& ref = globalRefTable.getReferenceData(arg1);
						parentAR = ref.getDestAR();
						return parentAR;
					}
					if (arg2.argFlag == Y_Dz) {
						arg2.ref_ar = currentAR;
						ReferenceData&& ref = globalRefTable.getReferenceData(arg2);
						parentAR = ref.getDestAR();
						return parentAR;
					}
					if (argop.argFlag == Y_Dz) {
						argop.ref_ar = currentAR;
						ReferenceData&& ref = globalRefTable.getReferenceData(
							argop);
						parentAR = ref.getDestAR();
						return parentAR;
					}
					if (argres.argFlag == Y_Dz) {
						argres.ref_ar = currentAR;
						ReferenceData&& ref = globalRefTable.getReferenceData(
							argres);
						parentAR = ref.getDestAR();
						return parentAR;
					}
					pos.SD(cdth);

				}
				return nullptr;

			}
		}

#if debug
		cout << "Executor::getParentAR in, functionBodyScopeFKA:["
			<< functionBodyScopeFKA.toStrg() << "] out" << endl;
#endif

	}

	/**
	 *@brief 根据currentCodeFKA 和 currentCodeFKA对应的tffka生成形参实参对照表
	 * @return 形式参数、实际参数对照表
	 */
	map<ArgFormal, ArgReal> Executor::getFRMap() {
		{
#if debug
			static Intg ignorecount = 0;
			cout << "Executor::getFRMap in, currentCodeFKA :["
				<< currentCodeFKA.toStrg() << "] in , ignorecount:["
				<< ignorecount++ << "]" << endl;
#endif
		}
		const FKA& tffka =
			(*cdth)[currentCodeFKA].assemblyFormula.flagBit.tfFKA;
		const FKA& fkaformal = cdth->getLast(COOL_M,
			FKA(tffka.fileKey, (*scpth)[tffka].scopeCoor2));
		CodeTable& cdtreal = cdth->operator [](currentCodeFKA.fileKey);
		CodeTable& cdtformal = cdth->operator [](tffka.fileKey);
		//FunctionHandleMatcher fhm(&cdtreal, &cdtformal);

		Matcher m = Matcher(&cdtreal, &cdtformal, this->tfth, currentCodeFKA.addr, fkaformal.addr, MatchMode::FunctionInvocationFormalRealBind, currentAR, cdth);
		map<ArgFormal, ArgReal> frmap;
		if (m.match() == T_) {
			for (auto& p : m.getMatchState().rcam) {
				frmap.insert(make_pair(p.first, p.second.first));
			}
			return frmap;
		}
		else {

			cout << "Executor::getFRMap err,exit(-1)" << endl;
			cerr << "Executor::getFRMap err,exit(-1)" << endl;
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
			assert(false); exit(-1);
		}

	}
	/**
	 * @attention 注意，此函数只是用于验证给定位置fki的代码是否可以成为一个有效的nextCode。
	 * 如果输入的fki是有效的，那么fki的值不会改变，否则，此函数会在此fki的基础上寻找下一个有效的pos
	 * @attention 有效的fki指的是对应代码可以被顺序执行！（因此不能被直接执行的代码会被跳过）
	 * @attention 当fki为fkinull或越过cdth的边界，则判定为无效并不再寻找下一个有效的pos
	 * @param fki,如果fki有效，其会更新nextcodefki、nextCodeFKA
	 * @return
	 */
	bool Executor::setNextCodeFKI() {
#if debug
		{
			static int ignorecount = 0;
			cout << "Executor::setNextCodePos(int&)" << " " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif
#if debug
		cout << "Executor::setNextCodeFKI 0, nextCodeFKA:[" << nextCodeFKA.toStrg()
			<< "]" << endl;
#endif

		//run out of code
		if (nextCodeFKI == fkinull
			|| nextCodeFKI.GT(cdth, cdth->getMaxFKI())
			== true) {
			nextCodeFKA = fkanull;
			nextCodeFKI = fkinull;
#if debug
			cout << "Executor::setNextCodeFKI 0.5, nextCodeFKA:["
				<< nextCodeFKA.toStrg() << "]" << endl;
#endif
			return false;
		}
		//executable code can be directly executed
		if ((*cdth)[nextCodeFKI].assemblyFormula.flagBit.execute == T_) {
			nextCodeFKA = (*cdth).getFKAByFKI(nextCodeFKI);
#if debug
			cout << "Executor::setNextCodeFKI 1, nextCodeFKA:["
				<< nextCodeFKA.toStrg() << "]" << endl;
#endif

			return true;
		}
		//dismiss disused code
		if ((*cdth)[nextCodeFKI].assemblyFormula.flagBit.execute == Du) {
#if debug
			cout << "Executor::setNextCodeFKI 3, nextCodeFKA:["
				<< nextCodeFKA.toStrg() << "]" << endl;
#endif
			nextCodeFKI.SI(cdth);
			return setNextCodeFKI();
		}

		//jump over EXPR scope
		if ((mode == MODE_EXECUTION || (mode == MODE_GROUNDING && isExecuting == true))
			&& (*cdth)[nextCodeFKI].assemblyFormula.flagBit.type
			== EXPRQS) {
			FKA& scopeFKAJump = (*cdth)[nextCodeFKI].scopeFKA;
			Addr& coor2 = (*scpth)[scopeFKAJump].scopeCoor2;
			nextCodeFKI = cdth->getFKIByFKA(
				FKA(scopeFKAJump.fileKey, coor2));
			nextCodeFKI.SI(cdth);

		}

		//jump over F_ under any circumstance, and jump over COOL_QS COOL_R when execute （call fact functions）
		if (

			(mode == MODE_EXECUTION || (mode == MODE_GROUNDING && isExecuting == true)) &&

			(((*cdth)[nextCodeFKI].assemblyFormula.flagBit.execute == COOL_R && (*cdth)[nextCodeFKI].assemblyFormula.flagBit.type == COOL_QS) ||
				(*cdth)[nextCodeFKI].assemblyFormula.flagBit.execute == F_)


			) {
#if debug
			cout << "Executor::setNextCodeFKI 4, nextCodeFKA:["
				<< nextCodeFKA.toStrg() << "]" << endl;
#endif
			FKA& scopeFKAJump = (*cdth)[nextCodeFKI].scopeFKA;
			Addr& coor2 = (*scpth)[scopeFKAJump].scopeCoor2;
			nextCodeFKI = cdth->getFKIByFKA(
				FKA(scopeFKAJump.fileKey, coor2));
			nextCodeFKI.SI(cdth);
#if debug
			cout << "Executor::setNextCodeFKI 5, nextCodeFKA:["
				<< nextCodeFKA.toStrg() << "]" << endl;
#endif
			return setNextCodeFKI();
		}



		nextCodeFKA = (*cdth).getFKAByFKI(nextCodeFKI);
#if debug
		cout << "Executor::setNextCodeFKI 5, nextCodeFKA:[" << nextCodeFKA.toStrg()
			<< "]" << endl;
#endif
		return true;
	}





	FKA Executor::getScopeQEFKA(const FKA& codeFKA) {
		return FKA(codeFKA.fileKey,
			(*scpth)[(*cdth)[codeFKA].scopeFKA].scopeCoor2);
	}
	FKA Executor::getScopeQSFKA(const FKA& codeFKA) {
		return FKA(codeFKA.fileKey,
			(*scpth)[(*cdth)[codeFKA].scopeFKA].scopeCoor1);
	}
	/**
	 * @brief 寻找与当前分支语句(COOL_B 1)匹配的下一个分支语句(COOL_B 2)所在的fka
	 * @param currentConditionCodeFKA
	 * @return
	 */
	FKA Executor::getNextAssociateBraceFKA(const FKA& currentConditionCodeFKA) {
		FKA possibleNextBranchCodeFKA;
		FKA nextQSFKA = cdth->getNext(COOL_QS, currentConditionCodeFKA);
		FKA nextQEFKA;
		FKA currentBranchCodeQEFKA = getScopeQEFKA(currentConditionCodeFKA);

		if (nextQSFKA == fkanull
			|| (currentBranchCodeQEFKA != fkanull
				&& nextQSFKA > currentBranchCodeQEFKA)) {
			return fkanull;
		}
		else {
			nextQEFKA = getScopeQEFKA(nextQSFKA);
			possibleNextBranchCodeFKA = cdth->getNext(COOL_B, nextQEFKA);
			if (possibleNextBranchCodeFKA != fkanull
				&& (*cdth)[possibleNextBranchCodeFKA][0]
				== (*cdth)[currentConditionCodeFKA][3]) {
				return possibleNextBranchCodeFKA;
			}
			else {
				return fkanull;
			}
		}

	}
	/**
	 * @attention 这个函数将根据（用户设定的）nextCodeFKI更新currentCode和currentCodeFKA，
	 * 同时，会自动寻找下一个可以被自动执行的代码将其设为新的nextCodeFKI以及对应的FKA与code供用户参考.
	 * 用户可以对nextCodeFKI其重新赋值来确定下一步执行的代码。
	 * @return
	 */
	bool Executor::getNewCode() {
#if debug
		cout << "Executor::getNewCode, currentCodeFKA:[" << currentCodeFKA.toStrg()
			<< "] in" << endl;
#endif

		//if (mode == MODE_GROUNDING && isExecuting == false) {//返回到grounding的位置
		//	nextCodeFKA = cdth->getFKAByFKI(nextCodeFKI);
		//	if (this->currentExpr->enclose(nextCodeFKA)) {

		//	}


		//}

		if (nextCodeFKI == fkinull
			|| nextCodeFKI.GT(cdth, cdth->getMaxFKI())) {
#if debug
			cout << "//////////////////////////////////////////////////\n"
				"///////           End of program!          ///////\n"
				"//////////////////////////////////////////////////" << endl;
#endif
			return false;

		}
		else {
			currentCodeFKI = nextCodeFKI;
		}

		if (currentCodeFKI.GT(cdth, cdth->getMaxFKI())) {
			return false;
		}
		nextCodeFKI.SI(cdth);
		setNextCodeFKI();

#if debug
		cout << "Executor::getNewCode nextCodeFKA 1:" << nextCodeFKA.toStrg()
			<< endl;
#endif

		if (currentCodeFKI.LT(cdth, cdth->getMaxFKI())) {
#if debug
			cout << "Executor::getNewCode nextCodeFKA 2:" << nextCodeFKA.toStrg()
				<< endl;
#endif
			currentCodeFKA = cdth->getFKAByFKI(currentCodeFKI);
			currentCode = &(*cdth)[currentCodeFKI];
#if debug
			cout << "Executor::getNewCode nextCodeFKA 3:" << nextCodeFKA.toStrg()
				<< endl;
#endif
			if (nextCodeFKI.LE(cdth, cdth->getMaxFKI())) {
#if debug
				cout << "Executor::getNewCode nextCodeFKA 4:"
					<< nextCodeFKA.toStrg() << endl;
#endif
				nextCode = &(*cdth)[nextCodeFKI];
#if debug
				cout << "Executor::getNewCode nextCodeFKA 5:"
					<< nextCodeFKA.toStrg() << endl;
#endif
			}

			return true;

		}
		else if (currentCodeFKI == cdth->getMaxFKI()) {
#if debug
			cout << "Executor::getNewCode nextCodeFKA 6:" << nextCodeFKA.toStrg()
				<< endl;
#endif
			currentCodeFKA = cdth->getFKAByFKI(currentCodeFKI);
			currentCode = &(*cdth)[currentCodeFKI];
			nextCodeFKA = fkanull;
			nextCode = nullptr;
#if debug
			cout << "Executor::getNewCode nextCodeFKA 7:" << nextCodeFKA.toStrg()
				<< endl;
#endif
			return true;
		}
		else {

			cerr << "Executor::getNewCode err,exit(-1)" << endl;

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "error exit"; // Replace with actual info
				info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			assert(false); exit(-1);
		}
		return false;
	}
	/// <summary>
	/// do not modify arg.asc here(argscope), modify it at setArgScopeFKA
	/// </summary>
	/// <returns></returns>
	bool Executor::addArgsToCurrentAR_ground() {

#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = bbhline + Strg("Executor::addArgsToCurrentAR_ground") + bbhline; // Replace with actual info
			info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
			std::cout << info << std::endl;
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif

			ignoreCount++;
		}
#endif

		if ((*currentCode).assemblyFormula.flagBit.type == COOL_C
			&& (*currentCode)[0].argFlag == S_Bs
			&& (*currentCode)[1].argFlag == X_Bs) {
			if ((*currentCode)[1].arg_s == "new"
				|| (*currentCode)[1].arg_s == "out") {
				(*currentAR).add((*currentCode)[0]);
			}
			else if ((*currentCode)[1].arg_s == "placeholder") {
				auto&& arg = (*currentCode)[0];
				arg.isPlaceholder = true;
				(*currentAR).add(arg);

			}
			else if ((*currentCode)[1].arg_s == "symbol" || (*currentCode)[1].arg_s == "sym") {
				auto&& arg = (*currentCode)[0];
				arg.isSymbol = true;
				(*currentAR).add(arg);

			}
			else {
				//创建类的实例，直接使用类的AR
				(*currentAR).add((*currentCode)[0]);
				System& sys = (*systh)[pair<Strg, FKA>(
					currentCode->assemblyFormula.quaternion.arg2_s,
					currentCode->scopeFKA)];
				(*currentAR)[(*currentCode)[0]] = (*currentAR)[Arg(S_Dz,
					sys.systemFKA)];
			}

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_LST
			&& (*currentCode).assemblyFormula.flagBit.execute != Du) {
			/**
			 * 不用管引用，引用已经被置于COOL_M之前，已经被处理过了
			 */
			if ((*currentCode).assemblyFormula.flagBit.operator_flag == M_Bs
				&& (*currentCode).assemblyFormula.quaternion.operator_s
				== "{null}") {
				(*currentAR).add((*currentCode)[3]);
				Data& dt = (*currentAR)[(*currentCode)[3]];
				dt = Data(COOL_LST, nullptr);
				dt.content_lst->clear();
			}
#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::setArgScopeFKA()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			Arg&& arg_1 = (*currentCode)[0];
			Arg&& arg_2 = (*currentCode)[1];
			Arg&& arg_op = (*currentCode)[2];
			Arg&& arg_res = (*currentCode)[3];
			if (arg_op.arg_s == "{null}") {
				return true;
			}
			else {
				if (arg_2.isVar() && arg_2.argFlag != Y_Dz) {
					(*currentAR).add(arg_2);
					(*currentAR)[arg_1].content_lst->push_back((*currentAR)[arg_2]);
				}
				else if (arg_2.argFlag == Sz) {
					auto lstptr = (*currentAR)[arg_1].content_lst;
					lstptr->push_back(Data(arg_2.arg_i));
				}
				else if (arg_2.argFlag == Zf) {
					auto lstptr = (*currentAR)[arg_1].content_lst;
					lstptr->push_back(Data(arg_2.arg_s));
				}

			}

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
			/**
			 * 不用管引用，引用已经被置于COOL_M之前，已经被处理过了
			 */

			Arg&& arg_1 = (*currentCode)[0];
			Arg&& arg_2 = (*currentCode)[1];
			Arg&& arg_op = (*currentCode)[2];
			Arg&& arg_res = (*currentCode)[3];
			if (arg_1.isDz() && arg_1.argFlag != Y_Dz) {
				(*currentAR).add((*currentCode)[0]);
			}
			if (arg_2.isDz() && arg_2.argFlag != Y_Dz) {
				(*currentAR).add((*currentCode)[1]);
			}
			if (arg_op.isDz() && arg_op.argFlag != Y_Dz) {
				(*currentAR).add((*currentCode)[2]);
			}
			if (arg_res.isDz() && arg_res.argFlag != Y_Dz) {
				(*currentAR).add((*currentCode)[3]);
			}
		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_Y
			&& (*currentCode).assemblyFormula.flagBit.execute != Du) {
			Arg&& arg_1 = (*currentCode)[0];
			Arg&& arg_2 = (*currentCode)[1];
			Arg&& arg_op = (*currentCode)[2];
			Arg&& arg_res = (*currentCode)[3];
			//引用的arg1、arg2无意义，故忽略其添加
			/*if (arg_1.isVar() && arg_1.argFlag != Y_Dz) {
			 (*currentAR).add((*currentCode)[0]);
			 } else if ((*currentCode).assemblyFormula.flagBit.arg1_flag == Y_Dz) {

			 arg_1.ref_ar = currentAR;
			 (*currentAR).add(arg_1);
			 (*currentAR)[arg_1].content_ar = currentAR;
			 }
			 if (arg_2.isVar() && arg_2.argFlag != Y_Dz) {
			 (*currentAR).add((*currentCode)[1]);
			 } else if ((*currentCode).assemblyFormula.flagBit.arg2_flag == Y_Dz) {

			 arg_2.ref_ar = currentAR;
			 (*currentAR).add(arg_2);
			 (*currentAR)[arg_2].content_ar = currentAR;
			 }
			 if ((*currentCode).assemblyFormula.flagBit.operator_flag == S_Dz) {
			 (*currentAR).add((*currentCode)[2]);
			 } else if ((*currentCode).assemblyFormula.flagBit.operator_flag == Y_Dz) {
			 globalRefTable.addRefData(this->currentAR, codeTable,
			 currentCodeAddr, 2);

			 arg_op.ref_ar = currentAR;
			 (*currentAR).add(arg_op);
			 (*currentAR)[arg_op].content_ar = currentAR;
			 }*/
			if (arg_res.isDz() && arg_res.argFlag != Y_Dz) {
				(*currentAR).add((*currentCode)[3]);
			}
			else if ((*currentCode).assemblyFormula.flagBit.result_flag == Y_Dz) {
				globalRefTable.addRefData(this->currentAR, cdth,
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
						cout << "Integrator::addArgsToCurrentAR()"
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
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M
			&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
			(*currentAR).add((*currentCode)[0]);
			(*currentAR).add((*currentCode)[1]);
			(*currentAR).add((*currentCode)[2]);
			(*currentAR).add((*currentCode)[3]);

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QS
			&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
			(*currentAR).add((*currentCode)[3]);
			(*currentAR)[Arg(S_AR, (*currentCode).scopeFKA)].dataFlagBit = S_AR;
			//safe_delete((*currentAR)[(*currentCode).scopeFKA].content_ar);
			(*currentAR)[Arg(S_AR, (*currentCode).scopeFKA)].content_ar =
				DataTablePtr::make(currentAR, currentAR, (*currentCode).scopeFKA);
			currentAR = (*currentAR)[Arg(S_AR, (*currentCode).scopeFKA)].content_ar;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
			//(*currentAR).add((*currentCode)[3]);
			(*currentAR)[(*currentCode)[3]].dataFlagBit = S_AR;

			currentAR = currentAR->parentAR;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == EXPRQS
			&& (*currentCode).assemblyFormula.flagBit.execute != Du) {
			(*currentAR).add(resultexe);
			FKA expraddr = cdth->getNext(COOL_M, currentCodeFKA);
			auto&& ofcdt = cdth->copyTree(expraddr);
			Data& dt = (*currentAR)[resultexe];
			dt.isExpression = true;
			dt.expr = ExpressionHandlerPtr::make(ofcdt);

			//DELETE_AR(scopeAR);

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == EXPRQE
			&& (*currentCode).assemblyFormula.flagBit.execute != Du) {


			//do nothing
			//        (*currentAR)[resultexe].dataFlagBit = S_AR;
			//        todo: delete heap content in this AR


			//DELETE_AR(deleteAR);

		}

		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_R) {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = bhline + Strg(__FUNCTION__) + "::CodeType==Return\n";
				info += "currentCodeFKA:\t" + currentCodeFKA.toStrg() + "\n";
				info += "currentCode:\t" + currentCode->toStrg() + "\n";
				info += "currentAR:\t" + currentAR->scopeStructureFKA.toStrg() + "\n";
				info += Strg(bhline); // Replace with actual info
				info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
				std::cout << info << std::endl;
				std::wstring winfo(info.begin(), info.end());
				OutputDebugString(winfo.c_str());
				ignoreCount++;
			}
#endif

			//Todo: 
			//if no return
			if (argexe1 == argnull) {
				//add nothing
				return true;
			}
			else {

				Data& ansdt = (*currentAR)["ans"];
				ExpressionHandlerPtr ansexpr = nullptr;
				if (ansdt.isExpression) {
					ansexpr = ansdt.expr;
				}
				else {
					auto ofdreturn = cdth->copyTreeBranch(currentCodeFKA);
					if (get<0>(ofdreturn).size() == 0) {
						ansexpr = ExpressionHandlerPtr::make(argexe1);
					}
					else {
						ansexpr = ExpressionHandlerPtr::make(ofdreturn);
					}
				}
				DataTablePtrW p = currentAR;
				//functions cannot be defined within loops or brances or functions
				set<Intg> function_scp{ FB, RFB,BRB,LOB };
				while (!(p->parentAR == nullptr || function_scp.count(this->scpth->operator[](p->parentAR->scopeStructureFKA).functionFlag) == 0)) {
					p = p->parentAR;
				}

				ansexpr->flatten(this->isExecuting ? (*currentAR)[Arg(S_Bs, "thisexpr")].expr : ExpressionHandlerPtr::make(), this->currentAR, p->frmap, this->cdth, this->scpth);

				ansdt.isExpression = true;
				ansdt.expr = ansexpr;
			}


		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QS
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R) {
			if ((*scpth)[currentCode->scopeFKA].functionFlag == SYSB) {
				//grounding过程中的继承处理
				(*currentAR).add((*currentCode)[3]);
				Data& sysData = (*currentAR)[Arg(S_AR, (*currentCode).scopeFKA)];
				sysData.dataFlagBit = S_AR;
				//safe_delete(sysData.content_ar);

				DataTablePtr sysAR = nullptr;
				sysData.content_ar = DataTablePtr::make(currentAR, currentAR, currentAR,
					(*currentCode).scopeFKA, SYSB);
				sysAR = sysData.content_ar;
				sysAR->scopeStructureFKA = currentCode->scopeFKA;
				currentAR = sysAR;

				System& currentSystem = (*systh)[currentCode->scopeFKA];
				//将继承类的预执行实例直接添加到querylist中完成继承
				for (auto& id : currentSystem.decedentSystemFKIList) {
					sysAR->queryARList.push_front(
						(*currentAR->parentAR)[Arg(S_AR, (*systh)[id].systemFKA)].content_ar);
					sysAR->queryARList_local.push_front(
						(*currentAR->parentAR)[Arg(S_AR, (*systh)[id].systemFKA)].content_ar);
				}

			}
			else {
				//预执行过程中的函数调用等处理


				(*currentAR).add((*currentCode)[3]);
				(*currentAR)[Arg(S_AR, (*currentCode).scopeFKA)].dataFlagBit = S_AR;

				//safe_delete((*currentAR)[(*currentCode).scopeFKA].content_ar);
				DataTablePtr queryAR_ = nullptr;
				if ((*scpth)[(*currentCode).scopeFKA].requireScopeFKA
					!= fkanull) {
					queryAR_ =
						(*currentAR)[Arg(S_AR, (*scpth)[(*currentCode).scopeFKA].requireScopeFKA)].content_ar;
				}
				else {
					queryAR_ = currentAR;
				}

				DataTablePtr funAR = nullptr;


				funAR = DataTablePtr::make(currentAR, queryAR_,
					(*currentCode).scopeFKA);
				(*currentAR)[Arg(S_AR, (*currentCode).scopeFKA)].content_ar = funAR;
				funAR->scopeStructureFKA = currentCode->scopeFKA;
				currentAR = funAR;
				//add dummy 'this expr' to mimic function invocation 
				currentAR->add(Arg(S_Bs, "thisexpr"));
				Data& thisexprdt = (*currentAR)[Arg(S_Bs, "thisexpr")];
				thisexprdt.isExpression = true;
				thisexprdt.expr = ExpressionHandlerPtr::make();
				thisexprdt.expr->type = EXPRESSIONHANDLER_CDT;
				//add dummy 'ans' to mimic function invocation
				currentAR->add(Arg(S_Bs, "ans"));
			}

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R) {
			Arg&& scopeArg = (*currentCode)[3];
			(*currentAR)[scopeArg].dataFlagBit = S_AR;

			if (currentAR->meetFKA != fkanull) {
				nextCodeFKA = currentAR->meetFKA;
				nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);
			}


			if (currentAR->meetFKA != fkanull && currentAR->returnAR != nullptr) {
				currentAR = currentAR->returnAR;

			}
			else  if (currentAR->parentAR != nullptr) {
				currentAR = currentAR->parentAR;
			}
			if (scpth->operator[](scopeArg.arg_fka).functionFlag != SYSB && scpth->operator[](scopeArg.arg_fka).functionFlag != EXPRB) {
				currentAR->argdmap.erase(scopeArg);
			}

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_TD
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
#if debug
				{
					static int ignorecount = 0;
					cout << "Integrator::addArgsToCurrentAR()" << " COOL_TD "
						<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
						<< ":" << __LINE__ << ":0" << ")" << endl;
				}
#endif

				//1
				//复制原函数的函数体SCOPE，将该scope添加至codeTable表中，并与函数进行绑定。
				//由于作用域内部地址不用管，只需要修改codeFKA
				//为了能合适地将scope添加到codeTable中，在compiler时已经在T类型后面连续添加了多个COOL_ME类型代码
				//因此,将这些COOL_ME代码删除后可以放心添加
				FKA& ffbScopeFKA =
					(*tfth)[currentCode->assemblyFormula.quaternion.arg2_fka].bodyScopeFKA;
				Addr ffbScopeAddrLowBound = (*scpth)[ffbScopeFKA].scopeCoor1;
				Addr ffbScopeAddrUpBound = (*scpth)[ffbScopeFKA].scopeCoor2;
				CodeTable& ffbCodeTable = cdth->operator [](
					ffbScopeFKA.fileKey);
				CodeTable&& ffbScopeCdt = get<0>(ffbCodeTable.copyCodeTable(
					ffbScopeAddrLowBound, ffbScopeAddrUpBound));

				Addr& bfbScopeAddrLowBound = currentCodeFKA.addr;
				const Addr&& bfbScopeAddrUpBound = ffbCodeTable.getNextNot(COOL_ME,
					currentCodeFKA.addr);
				//注意！此步骤中没有修改Z_Dz地址，后续一定要修改
				ffbScopeCdt.resetFKARange(bfbScopeAddrLowBound, bfbScopeAddrUpBound,
					ffbScopeCdt.fileKey);
				CodeTable& bfbScopeCdt = ffbScopeCdt;
				FKA bfnScopeFKA = (*currentCode).assemblyFormula.quaternion.arg1_fka;

				Intg pos = getAddrPosition(ffbCodeTable.addrdq, currentCodeFKA.addr);
				pos++;
				while ((ffbCodeTable)[pos].assemblyFormula.flagBit.type == COOL_ME) {
					ffbCodeTable.eraseCode(pos++, 0);

				}
				BackStepper bs(this);
				bs.bfbScopeCdt = &bfbScopeCdt;
				bs.bfnScopeFKA = bfnScopeFKA;
				//2 进行第一次正向执行，解除绑定，修改Z_Dz地址
				bs.backwardFunctionFirstForwardExecution();

				//3 第二次正向执行
				bs.backwardFunctionSecondForwardExecution();
				// 第三次正向执行
				bs.backwardFunctionThirdForwardExecution();
				//3 第一次反向执行
				bs.backwardFunctionFirstBackwardExecution();
				//4 整合
				bs.backwardFunctionIntegrate(currentCodeFKA, currentCodeFKA.addr,
					ffbCodeTable.getNextCodeAddr(currentCodeFKA.addr));

				//5 设置nextCodeAddr
				nextCodeFKA = cdth->getNextCodeFKA(currentCodeFKA);
				//        nextCodeAddr = bfbScopeAddrUpBound;

		}

		return true;
	}


	bool Executor::addArgsToCurrentAR_execute() {
#if debug
		{
			static int ignorecount = 0;
			cout << "Executor::addArgsToCurrentAR_execute()" << " " << "\tignorecount:["
				<< ignorecount++ << "\t]currentCodeFKA:["
				<< currentCodeFKA.toStrg() << "\t](" << __FILE__ << ":"
				<< __LINE__ << ":0" << ")" << endl;
		}
#endif
#if debug
		cout << "Executor::addArgsToCurrentAR, currentCodeAddr:["
			<< currentCodeFKA.toStrg() << "] in" << endl;
#endif
		if ((*currentCode).assemblyFormula.flagBit.type == COOL_C
			&& argexe1.argFlag == S_Bs && argexe2.argFlag == X_Bs
			&& argexe2.arg_s == "new") {

			(*currentAR).add(argexe1);

		}
		else if (
			mode == MODE_GROUNDING && isExecuting == true &&

			(*currentCode).assemblyFormula.flagBit.type == COOL_C
			&& argexe1.argFlag == S_Bs && argexe2.argFlag == X_Bs
			&& argexe2.arg_s == "placeholder") {

			argexe1.isPlaceholder = true;
			(*currentAR).add(argexe1);

		}
		else if (mode == MODE_GROUNDING && isExecuting == true &&
			(*currentCode).assemblyFormula.flagBit.type == COOL_C
			&& argexe1.argFlag == S_Bs && argexe2.argFlag == X_Bs
			&& (argexe2.arg_s == "symbol" || argexe2.arg_s == "sym")) {

			argexe1.isSymbol = true;
			(*currentAR).add(argexe1);

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_C
			&& argexe1.argFlag == S_Bs && argexe2.argFlag == X_Bs
			&& argexe2.arg_s != "new" && argexe2.arg_s != "out") {
			//此部分处理创建类对象实例
			(*currentAR).add(argexe1);
			Data& sysData = (*currentAR)[argexe1];
			/*safe_delete(sysData.content_ar);*/
			System& sys = (*systh)[pair<Strg, FKA>(argexe2.arg_s,
				currentCode->scopeFKA)];
			sysData.dataFlagBit = S_AR;
			FKA& systemScopeFKA = sys.systemFKA;
			DataTablePtr sysAR = nullptr;
			sysAR = DataTablePtr::make(currentAR, currentAR, currentAR,
				(*currentCode).scopeFKA, SYSB);
			sysData.content_ar = sysAR;
			sysAR->meetFKA = nextCodeFKA;
			sysAR->scopeStructureFKA = sys.systemFKA;
			//执行所继承的类
			nextCodeFKA = FKA(systemScopeFKA.fileKey,
				(*scpth)[systemScopeFKA].scopeCoor1);
			nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);
			currentAR = sysAR;
			/*DELETE_AR(sysAR);*/

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_Y
			&& (*currentCode).assemblyFormula.flagBit.execute != Du) {
			Arg& arg_1 = argexe1;
			Arg& arg_2 = argexe2;
			Arg& arg_op = operatorexe;
			Arg& arg_res = resultexe;
			//引用的arg1、arg2无意义，故忽略其添加
			/*if (arg_1.isVar() && arg_1.argFlag != Y_Dz) {
			 (*currentAR).add((*currentCode)[0]);
			 } else if ((*currentCode).assemblyFormula.flagBit.arg1_flag == Y_Dz) {

			 arg_1.ref_ar = currentAR;
			 (*currentAR).add(arg_1);
			 (*currentAR)[arg_1].content_ar = currentAR;
			 }
			 if (arg_2.isVar() && arg_2.argFlag != Y_Dz) {
			 (*currentAR).add((*currentCode)[1]);
			 } else if ((*currentCode).assemblyFormula.flagBit.arg2_flag == Y_Dz) {

			 arg_2.ref_ar = currentAR;
			 (*currentAR).add(arg_2);
			 (*currentAR)[arg_2].content_ar = currentAR;
			 }
			 if ((*currentCode).assemblyFormula.flagBit.operator_flag == S_Dz) {
			 (*currentAR).add((*currentCode)[2]);
			 } else if ((*currentCode).assemblyFormula.flagBit.operator_flag == Y_Dz) {
			 globalRefTable.addRefData(this->currentAR, codeTable,
			 currentCodeAddr, 2);

			 arg_op.ref_ar = currentAR;
			 (*currentAR).add(arg_op);
			 (*currentAR)[arg_op].content_ar = currentAR;
			 }*/
			if (arg_res.isVar() && arg_res.argFlag != Y_Dz) {
				resultexe.ref_ar = this->currentAR;
				(*currentAR).add(resultexe);
			}
			else if ((*currentCode).assemblyFormula.flagBit.result_flag == Y_Dz) {
				globalRefTable.addRefData(this->currentAR, cdth,
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
						cout << "Executor::addArgsToCurrentAR_execute()"
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
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M
			&& (*currentCode).assemblyFormula.flagBit.execute == true) {
			if (currentCode->assemblyFormula.flagBit.arg1_flag == S_Dz) {

				argexe1.ref_ar = this->currentAR;
				(*currentAR).add(argexe1);
			}
			else if (currentCode->assemblyFormula.flagBit.arg1_flag == Y_Dz) {
				globalRefTable.addRefData(this->currentAR, cdth,
					currentCodeFKA, 0);
				argexe1.ref_ar = this->currentAR;
				(*currentAR).add(argexe1);
			}
			if (currentCode->assemblyFormula.flagBit.arg2_flag == S_Dz) {
				argexe2.ref_ar = this->currentAR;
				(*currentAR).add(argexe2);
			}
			else if (currentCode->assemblyFormula.flagBit.arg2_flag == Y_Dz) {
				globalRefTable.addRefData(this->currentAR, cdth,
					currentCodeFKA, 1);
				argexe2.ref_ar = this->currentAR;
				(*currentAR).add(argexe2);
			}
			if (currentCode->assemblyFormula.flagBit.operator_flag == S_Dz) {
				operatorexe.ref_ar = this->currentAR;
				(*currentAR).add(operatorexe);
			}
			else if (currentCode->assemblyFormula.flagBit.operator_flag == Y_Dz) {
				globalRefTable.addRefData(this->currentAR, cdth,
					currentCodeFKA, 2);
				operatorexe.ref_ar = this->currentAR;
				(*currentAR).add(operatorexe);
			}
			if (currentCode->assemblyFormula.flagBit.result_flag == S_Dz) {
				resultexe.ref_ar = this->currentAR;
				(*currentAR).add(resultexe);
			}
			else if (currentCode->assemblyFormula.flagBit.result_flag == Y_Dz) {
				globalRefTable.addRefData(this->currentAR, cdth,
					currentCodeFKA, 3);
				resultexe.ref_ar = this->currentAR;
				(*currentAR).add(resultexe);
			}
		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_LST
			&& (*currentCode).assemblyFormula.flagBit.execute == true) {
			if ((*currentCode).assemblyFormula.flagBit.operator_flag == M_Bs
				&& (*currentCode).assemblyFormula.quaternion.operator_s
				== "{null}") {
				(*currentAR).add((*currentCode)[3]);
				(*currentAR)[(*currentCode)[3]] = Data(COOL_LST, nullptr);
			}
			else {
				Arg& arg_1 = argexe1;
				Arg& arg_2 = argexe2;
				Arg& arg_op = operatorexe;
				Arg& arg_res = resultexe;
				if (arg_2.isVar() && arg_2.argFlag != Y_Dz) {
					if (currentAR->findall(arg_2) == false) {
						(*currentAR).add(arg_2);
					}
					(*currentAR)[arg_1].content_lst->push_back((*currentAR)[arg_2]);
				}
				else if (arg_2.argFlag == Y_Dz) {
					globalRefTable.addRefData(this->currentAR, cdth,
						currentCodeFKA, 1);
					arg_2.ref_ar = currentAR;
					(*currentAR).add(arg_2);
				}
				else if (arg_2.argFlag == Sz) {
					auto lstptr = (*currentAR)[arg_1].content_lst;
					lstptr->push_back(Data(arg_2.arg_i));
				}
				else if (arg_2.argFlag == Zf) {
					auto lstptr = (*currentAR)[arg_1].content_lst;
					lstptr->push_back(Data(arg_2.arg_s));
				}

			}

		}
		/*else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M
		 && (*currentCode).assemblyFormula.flagBit.execute == F_) {
		 (*currentAR).add(argexe1);
		 (*currentAR).add(argexe2);
		 (*currentAR).add(operatorexe);
		 (*currentAR).add(resultexe);

		 }*/
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QS
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
			(*currentAR).add(resultexe);
			(*currentAR)[Arg(S_AR, (*currentCode).scopeFKA)].dataFlagBit = S_AR;
			(*currentAR)[Arg(S_AR, (*currentCode).scopeFKA)].content_ar = nullptr;
			//        safe_delete((*currentAR)[(*currentCode).scopeFKA].content_ar);
			DataTablePtr scopeAR = nullptr;
			scopeAR =
				DataTablePtr::make(currentAR, currentAR, (*currentCode).scopeFKA);
			(*currentAR)[Arg(S_AR, (*currentCode).scopeFKA)].content_ar = scopeAR;

			currentAR = scopeAR;
			//DELETE_AR(scopeAR);

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {

			//        (*currentAR)[resultexe].dataFlagBit = S_AR;
			//        todo: delete heap content in this AR
			if (currentAR->meetFKA != fkanull) {
				nextCodeFKA = currentAR->meetFKA;
				nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);
			}
			DataTablePtr deleteAR = currentAR;
			currentAR = currentAR->parentAR;
			currentAR->argdmap.erase(Arg(S_AR, deleteAR->scopeStructureFKA));

			//DELETE_AR(deleteAR);

		}

		else if ((*currentCode).assemblyFormula.flagBit.type == EXPRQS
			&& (*currentCode).assemblyFormula.flagBit.execute != Du) {
			(*currentAR).add(resultexe);
			FKA expraddr = cdth->getNext(COOL_M, currentCodeFKA);
			auto&& ofcdt = cdth->copyTree(expraddr);
			Data& dt = (*currentAR)[resultexe];
			dt.isExpression = true;
			dt.expr = ExpressionHandlerPtr::make(ofcdt);

			//DELETE_AR(scopeAR);

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == EXPRQE
			&& (*currentCode).assemblyFormula.flagBit.execute != Du) {


			//do nothing
			//        (*currentAR)[resultexe].dataFlagBit = S_AR;
			//        todo: delete heap content in this AR


			//DELETE_AR(deleteAR);

		}



		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QS
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R
			&& currentAR->type == SYSB
			&& currentAR->scopeStructureFKA == currentCode->scopeFKA) {
			//此部分仅仅考虑类继承中的处理，
			//无需考虑因函数调用或条件语句执行引起的COOL_QS R的情况，因为其已经在其他部分处理过了
			Scope& currentScope = (*scpth)[currentCode->scopeFKA];
			System& currentSystem = (*systh)[currentCode->scopeFKA];
			set<FKA> decedentSystemFKASet1;        //应该被继承的类
			set<FKA> decedentSystemFKASet2;        //已经被继承的类
			for (auto& id : currentSystem.decedentSystemFKIList) {
				decedentSystemFKASet1.insert((*systh)[id].systemFKA);
			}
			for (auto& decedentAR : currentAR->queryARList) {

				decedentSystemFKASet2.insert(decedentAR->scopeStructureFKA);
			}

			decedentSystemFKASet1 -= decedentSystemFKASet2;
			if (decedentSystemFKASet1.size() == 0) {
				//所有类均已继承，直接return，后续将自动执行此类中的代码
				return 0;

			}
			else {
				//还有类没被继承，那么创建这个类的AR添加到当前AR的queryARList中
				FKA decedentSysFKA = *decedentSystemFKASet1.begin();
				Scope& decedentSysScope = (*scpth)[decedentSysFKA];
				System& decedentSystem = (*systh)[decedentSysFKA];
				DataTablePtr parentARofDecedent = DataTable::getAccessibleAR(currentAR,
					decedentSysScope.parentScopeFKA);
				DataTablePtr decedentAR = nullptr;
				decedentAR
					= DataTablePtr::make(parentARofDecedent, parentARofDecedent,
						currentAR, decedentSysFKA, SYSB);
				decedentAR->meetFKA = currentCodeFKA;
				currentAR->queryARList.push_front(decedentAR);
				/*++(decedentAR->referenceCount);*/

				nextCodeFKA = FKA(decedentSystem.systemFKA.fileKey,
					(*scpth)[decedentSystem.systemFKA].scopeCoor1);
				nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);
				currentAR = decedentAR;
				/*DELETE_AR(decedentAR);*/
			}

			/*  (*currentAR).add(resultexe);
			 (*currentAR)[resultexe.arg_fka].dataFlagBit = S_AR;
			 (*currentAR)[resultexe.arg_fka].scopeStructureAddr =
			 currentCode->scopeAddr;
			 safe_delete((*currentAR)[(*currentCode).scopeAddr].content_ar);
			 DataTable *queryAR_ = nullptr;
			 if ((*scopeTable)[(*currentCode).scopeAddr].requireScopeFKA
			 != addrnull) {
			 queryAR_ =
			 (*currentAR)[(*scopeTable)[(*currentCode).scopeAddr].requireScopeFKA].content_ar;
			 } else {
			 queryAR_ = currentAR;
			 }

			 (*currentAR)[(*currentCode).scopeAddr].content_ar = new DataTable(
			 currentAR, queryAR_, (*currentCode).scopeAddr);
			 currentAR = (*currentAR)[(*currentCode).scopeAddr].content_ar;*/

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R
			&& (*scpth)[currentCode->scopeFKA].conditionFlag == 0) {
			//处理非条件语句作用域结尾
#if debug
				{
					static int ignorecount = 0;
					cout << "Executor::addArgsToCurrentAR_execute()" << " " << "\tignorecount:["
						<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
						<< ":0" << ")" << endl;
				}
#endif

				//(*currentAR)[resultexe].dataFlagBit = S_AR;
				if (currentAR->meetFKA != fkanull) {
					nextCodeFKA = currentAR->meetFKA;
					nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);
				}
				else {
					nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);
				}
				DataTablePtr ar_delete = currentAR;
				currentAR = currentAR->returnAR;
				if (ar_delete->type != SYSB) {
					//todo: delete heap content in ar_delete
					//@attention 调用COOL_QE退出ar需将父ar中对子ar的引用删除
					if (ar_delete->parentAR != nullptr) {
						ar_delete->parentAR->argdmap.erase(
							Arg(S_AR, ar_delete->scopeStructureFKA));
					}

					/*DELETE_AR(ar_delete);*/


				}

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R
			&& (*scpth)[currentCode->scopeFKA].conditionFlag == BRB) {
#if debug
				{
					static int ignorecount = 0;
					cout << "Executor::addArgsToCurrentAR_execute()" << " COOL_QE COOL_R BRB"
						<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
						<< ":" << __LINE__ << ":0" << ")" << endl;
				}
#endif

				//        (*currentAR)[resultexe].dataFlagBit = S_AR;

				Scope& conditionalBodyScope = (*scpth)[currentCode->scopeFKA];
				FKA& conditionCodeFKA = conditionalBodyScope.conditionCodeFKA;
				FKA possibleNextBraceCodeFKA = conditionCodeFKA;

				while (true) {
					FKA&& possibleNextBraceCodeFKAtmp = getNextAssociateBraceFKA(
						possibleNextBraceCodeFKA);
					if (possibleNextBraceCodeFKAtmp != fkanull) {
						possibleNextBraceCodeFKA = possibleNextBraceCodeFKAtmp;
					}
					else {
						break;
					}
				}

				//nextCodePos = currentCodePos + 1;
				nextCodeFKI = cdth->getFKIByFKA(conditionCodeFKA);
				nextCodeFKI.SI(cdth);
				setNextCodeFKI();
				nextCodeFKA = cdth->getFKAByFKI(nextCodeFKI);

				DataTablePtr ar_delete = currentAR;
				currentAR = currentAR->parentAR;
				//todo: delete heap content in ar_delete
				currentAR->argdmap.erase(Arg(S_AR, ar_delete->scopeStructureFKA));
				/*DELETE_AR(ar_delete);*/

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R
			&& (*scpth)[currentCode->scopeFKA].conditionFlag == LOB) {
#if debug
				{
					static int ignorecount = 0;
					cout << "Executor::addArgsToCurrentAR_execute()" << " COOL_QE COOL_R LOB"
						<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
						<< ":" << __LINE__ << ":0" << ")" << endl;
				}
#endif

				//        (*currentAR)[resultexe].dataFlagBit = S_AR;

				Scope& conditionalBodyScope = (*scpth)[currentCode->scopeFKA];
				FKA& conditionCodeFKA = conditionalBodyScope.conditionCodeFKA;
				Code& conditionCode = (*cdth)[conditionCodeFKA];
				Arg breakResult = conditionCode[0];
				Arg&& judgeResult = conditionCode[1];

				//if NOT BREAK , and judge result is pending or true, prepare for the next loop
				if ((breakResult.argFlag == S_Dz
					&& currentAR->operator [](breakResult).content_i <= 0)
					&& (

						(judgeResult.argFlag == Sz && judgeResult.arg_i > 0)
						|| (judgeResult.argFlag == Zf && judgeResult.arg_s != "")
						|| (judgeResult.argFlag == S_Dz/*donnot judge at now, the judge code haven't been run*/)
						|| (judgeResult.argFlag == S_Bs))

					) {
					FKA nextCodeFKAtmp = FKA(conditionalBodyScope.fileKey,
						conditionalBodyScope.scopeCoor1);
					if (judgeResult.argFlag == S_Dz || judgeResult.argFlag == S_Bs) {

						FKA&& lastFKA = cdth->getLastNot(COOL_QS, nextCodeFKAtmp);
						if (lastFKA != fkanull
							&& cdth->operator [](lastFKA).assemblyFormula.flagBit.type
							== COOL_M) {
							nextCodeFKAtmp = cdth->getLastNot(COOL_M, lastFKA);
							nextCodeFKAtmp = cdth->getNext(COOL_M, nextCodeFKAtmp);

						}
					}
					nextCodeFKA = nextCodeFKAtmp;
					nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);

					//            const FKA &&lastM = cdth->getLast(COOL_M,
					//                    FKA(conditionalBodyScope.fileKey,
					//                            conditionalBodyScope.scopeCoor1));
					//            if ((*cdth)[lastM][3] == conditionResult) {
					//                nextCodeFKA = cdth->getLastNot(COOL_M, lastM);
					//                nextCodeFKA = cdth->getNext(COOL_M, nextCodeFKA);
					//                nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);
					//            } else {
					//#if debug
					//                {
					//                    static int ignorecount = 0;
					//                    cerr << "Executor::addArgsToCurrentAR_execute()" << " err"
					//                            << "\tignorecount:[" << ignorecount++ << "\t]("
					//                            << __FILE__<<":" << __LINE__ << ":0" << ")"
					//                            << endl;
					//                }
					//#endif
					//                exit(-1);
					//            }

				}
				else {

					//if BREAK, quiet current loop, and provent break possible outer loop

					currentAR->operator [](breakResult).content_i = 0; //invalidate current "BREAK"

					nextCodeFKA = conditionCodeFKA;
					nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);
					nextCodeFKI.SI(cdth);
					nextCodeFKA = cdth->getFKAByFKI(nextCodeFKI);
					//            setNextCodeFKI();
					//            nextCodeFKA = cdth->getFKAByFKI(nextCodeFKI);
				}
				DataTablePtr ar_delete = currentAR;
				currentAR = currentAR->parentAR;
				//todo: delete heap content in ar_delete
				currentAR->argdmap.erase(Arg(S_AR, ar_delete->scopeStructureFKA));
				/*DELETE_AR(ar_delete);*/

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_CTN
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
			FKA conditionBodyScopeFKA = currentCode->scopeFKA;
			while ((*scpth)[conditionBodyScopeFKA].conditionFlag != LOB) {

				//            conditionBodyScopeFKA =
				//                    (*scpth)[conditionBodyScopeFKA].parentScopeFKA;
				DataTablePtr ar_delete = currentAR;
				currentAR = currentAR->parentAR;
				conditionBodyScopeFKA = currentAR->scopeStructureFKA;
				currentAR->argdmap.erase(Arg(S_AR, ar_delete->scopeStructureFKA));
				/*DELETE_AR(ar_delete);*/
				if (currentAR == nullptr) {

					{
						static int ignorecount = 0;
						std::cout << "Executor::addArgsToCurrentAR_execute()"
							<< " invalid CONTINUE, exit(-1) "
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
					assert(false); exit(-1);
				}
			}
			Scope& conditionalBodyScope = (*scpth)[conditionBodyScopeFKA];
			FKA& conditionCodeFKA = conditionalBodyScope.conditionCodeFKA;

			nextCodeFKA = FKA(conditionalBodyScope.fileKey,
				conditionalBodyScope.scopeCoor2);
			nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_BRK
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
			FKA conditionBodyScopeFKA = currentCode->scopeFKA;
			while ((*scpth)[conditionBodyScopeFKA].conditionFlag != LOB) {

				//            conditionBodyScopeFKA =
				//                    (*scpth)[conditionBodyScopeFKA].parentScopeFKA;
				DataTablePtr ar_delete = currentAR;
				currentAR = currentAR->parentAR;
				conditionBodyScopeFKA = currentAR->scopeStructureFKA;
				currentAR->argdmap.erase(Arg(S_AR, ar_delete->scopeStructureFKA));
				//DELETE_AR(ar_delete);
				if (currentAR == nullptr) {

					{
						static int ignorecount = 0;
						std::cout << "Executor::addArgsToCurrentAR_execute()"
							<< " invalid BREAK, exit(-1) " << "\tignorecount:["
							<< ignorecount++ << "\t](" << __FILE__ << ":"
							<< __LINE__ << ":0" << ")" << std::endl;
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
					assert(false); exit(-1);
				}
			}
			Scope& conditionalBodyScope = (*scpth)[conditionBodyScopeFKA];
			FKA& conditionCodeFKA = conditionalBodyScope.conditionCodeFKA;
			Code& conditionCode = (*cdth)[conditionCodeFKA];
			Arg&& breakResult = conditionCode[0];
			nextCodeFKA = FKA(conditionalBodyScope.fileKey,
				conditionalBodyScope.scopeCoor2);
			nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);
			Data& bkd = currentAR->operator [](breakResult);
			bkd = Data(true);
		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_R
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
			try
			{

				(*currentAR)["ans"] = (*currentAR)[argexe1];
			}
			catch (Arg& arg)
			{
				(*currentAR)["ans"] = Data(arg);
			}

			DataTablePtr ar_delete;
			while (currentAR->returnAR == nullptr
				|| tfth->count(
					(*scpth)[currentAR->scopeStructureFKA].requireScopeFKA)
				<= 0) {
				ar_delete = currentAR;
				currentAR = currentAR->parentAR;
				//todo:delete heap content in ar_delete
				currentAR->argdmap.erase(Arg(S_AR, ar_delete->scopeStructureFKA));
				//DELETE_AR(ar_delete);
				if (currentAR == nullptr) {

					{
						static int ignorecount = 0;
						std::cout << "Executor::addArgsToCurrentAR_execute()"
							<< " invalid RETURN, exit(-1) " << "\tignorecount:["
							<< ignorecount++ << "\t](" << __FILE__ << ":"
							<< __LINE__ << ":0" << ")" << std::endl;
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
					assert(false); exit(-1);
				}

			}
			nextCodeFKA = currentAR->meetFKA;
			if (nextCodeFKA == fkanull) {
				nextCodeFKI = cdth->getMaxFKI();
				nextCodeFKI.ID += 1;
#if debug
				cout << "Executor::addArgsToCurrentAR 2,meetAddr == addrnull ,"
					"program will finish\n" << endl;
#endif
			}
			else {
				nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);
			}
			ar_delete = currentAR;
			currentAR = currentAR->returnAR;

			//todo:delete heap content in ar_delete
			currentAR->argdmap.erase(Arg(S_AR, ar_delete->scopeStructureFKA));
			auto&& possibleFunctionNameFKA = scpth->operator [](
				ar_delete->scopeStructureFKA).requireScopeFKA;
			while (possibleFunctionNameFKA == fkanull
				|| (possibleFunctionNameFKA != fkanull
					&& tfth->count(
						possibleFunctionNameFKA) <= 0)) {
				ar_delete = ar_delete->parentAR;
				if (ar_delete == nullptr) {

					{
						static int ignorecount = 0;
						std::cout << "Executor::addArgsToCurrentAR_execute()"
							<< " invalid RETURN operation, exit(-1) "
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
					assert(false); exit(-1);
				}
				possibleFunctionNameFKA = scpth->operator [](
					ar_delete->scopeStructureFKA).requireScopeFKA;
			}
			DataTablePtr ar_delete_parent = ar_delete->parentAR;
			ar_delete_parent->argdmap.erase(Arg(S_AR, ar_delete->scopeStructureFKA));
			//DELETE_AR(ar_delete);

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_B
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
			bool exitBranch = false;
			if (argexe1.argFlag == S_Dz) {
				Arg&& foregoingBranchResultArg = cdth->operator [](
					argexe1.arg_fka)[3];
				exitBranch = (*currentAR)[foregoingBranchResultArg].valueToBool();

			}
			else if (argexe1.argFlag == Sz) {
				exitBranch = argexe1.arg_i;

			}
			else {
#if debug
				cerr
					<< "Executor::addArgsToCurrentAR err ,COOL_B type Code invalid argexe1, exit(-1)"
					<< endl;
#endif
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
				assert(false); exit(-1);
			}
			bool judgeResult = false;
			if (exitBranch == false) {
				if (argexe2.argFlag == S_Dz) {
					//get correct condition code FKA
					FKA possibleJudgeCodeFKA = argexe2.arg_fka;
					possibleJudgeCodeFKA = cdth->getNextNot(COOL_M,
						possibleJudgeCodeFKA);
					possibleJudgeCodeFKA = cdth->getLast(COOL_M,
						possibleJudgeCodeFKA);

					Arg&& conditionResultArg = cdth->operator [](
						possibleJudgeCodeFKA)[3];
					judgeResult = (*currentAR)[conditionResultArg].content_i;

				}
				else if (argexe2.argFlag == S_Bs) {

					judgeResult = (*currentAR)[argexe2].content_i;

				}
				else if (argexe2.argFlag == Sz) {
					judgeResult = (argexe2.arg_i > 0);

				}
				else if (argexe2.argFlag == Zf) {
					judgeResult = argexe2.arg_s != "";

				}
				else {
#if debug
					cerr
						<< "Executor::addArgsToCurrentAR err ,COOL_B type Code invalid argexe2, exit(-1)"
						<< endl;
#endif
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
					assert(false); exit(-1);
				}

			}
			else {
				if (resultexe.argFlag != Sz || resultexe.argFlag != Dz) {
					currentAR->add(resultexe);
					(*currentAR)[resultexe] = Data(true);

					FKA&& nextAssociateBraceFKA = getNextAssociateBraceFKA(
						currentCodeFKA);

					if (nextAssociateBraceFKA != fkanull
						&& (*cdth)[nextAssociateBraceFKA][0]
						== resultexe) {

						nextCodeFKA = nextAssociateBraceFKA;
						nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);
					}
					else {

						nextCodeFKI = currentCodeFKI;
						nextCodeFKI.SI(cdth);
						setNextCodeFKI();
						nextCodeFKA = cdth->getFKAByFKI(nextCodeFKI);

					}
				}
				else {
					nextCodeFKI = currentCodeFKI;
					nextCodeFKI.SI(cdth);
					setNextCodeFKI();
					nextCodeFKA = cdth->getFKAByFKI(nextCodeFKI);
				}
				return true;

			};
			bool exitNextBranch = false;
			if (judgeResult == true) {
				exitNextBranch = true;
				if (argexe1.argFlag == S_Dz) {
					//close the entrance of current branch
					Arg&& foregoingBranchResultArg = cdth->operator [](
						argexe1.arg_fka)[3];
					(*currentAR)[foregoingBranchResultArg] = Data(true);
				}

				Scope& conditionalBodyScope = (*scpth)[operatorexe.arg_fka];
				nextCodeFKA = FKA(conditionalBodyScope.fileKey,
					conditionalBodyScope.scopeCoor1);
				nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);
			}
			else {
				exitNextBranch = false;
			}
			currentAR->add(resultexe);
			(*currentAR)[resultexe] = Data(exitNextBranch);
			return true;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_L
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
			bool exitLoop = false;
			if (argexe1.argFlag == S_Dz) {
				if (currentAR->findcurrent(argexe1)) {
					exitLoop = (*currentAR)[argexe1].content_i;
				}
				else {
					currentAR->add(argexe1);
					auto& dt = currentAR->operator[](argexe1);
					dt.deepAssignFrom(DataPtr::make(Arg(Sz, 0)));
				}

			}
			else if (argexe1.argFlag == Sz) {
				exitLoop = argexe1.arg_i;
			}
			else {
#if debug
				cerr
					<< "Executor::addArgsToCurrentAR err ,COOL_L type Code invalid argexe1, exit(-1)"
					<< endl;
#endif
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
				assert(false); exit(-1);
			}
			bool judgeResult = false;
			if (exitLoop == false) {
				if (argexe2.argFlag == S_Dz) {
					//get correct condition code FKA
					FKA possibleJudgeCodeFKA = argexe2.arg_fka;
					possibleJudgeCodeFKA = cdth->getNextNot(COOL_M,
						possibleJudgeCodeFKA);
					possibleJudgeCodeFKA = cdth->getLast(COOL_M,
						possibleJudgeCodeFKA);

					Arg&& judgeResultArg = cdth->operator [](
						possibleJudgeCodeFKA)[3];
					judgeResult = (*currentAR)[judgeResultArg].content_i;

				}
				else if (argexe2.argFlag == S_Bs) {

					judgeResult = (*currentAR)[argexe2].content_i;

				}
				else if (argexe2.argFlag == Sz) {
					judgeResult = (argexe2.arg_i > 0);

				}
				else if (argexe2.argFlag == Zf) {
					judgeResult = argexe2.arg_s != "";

				}
				else {

					cerr
						<< "Executor::addArgsToCurrentAR err ,COOL_B type Code invalid argexe2, exit(-1)"
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
					assert(false); exit(-1);
				}

			}

			if (exitLoop == false && judgeResult == true) {

				Scope& conditionalBodyScope = (*scpth)[operatorexe.arg_fka];
				nextCodeFKA = FKA(conditionalBodyScope.fileKey,
					conditionalBodyScope.scopeCoor1);
				nextCodeFKI = cdth->getFKIByFKA(nextCodeFKA);
			}
			else {
				nextCodeFKI = currentCodeFKI;
				nextCodeFKI.SI(cdth);
				nextCodeFKA = cdth->getFKAByFKI(nextCodeFKI);

			}
		}
		/*else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
		 && (*currentCode).assemblyFormula.flagBit.execute == COOL_R) {

		 (*currentAR)[resultexe].dataFlagBit = S_AR;

		 currentAR = currentAR->parentAR;

		 }*/

		return true;
	}
	/**
		@attention: this function will convert op2 to the same type of op1 before comparison!!!!
	 * @brief compare two data objects, this function has practical signifacation.
	 * lst cmp lst  :compare counterpoint element
	 * lst cmp num/strg :all elements of vector compare with num/strg, vice versa
	 * set/multiset/map/multimap cmp num/strg/set/multiset/map/multimap,
	 *  judge whether the op1 is op2's subset or opposite.
	 * @attention
	 * 1  if op1 or op2's type is S_AR, the comparison makes no practical sense and will
	 *    use default comparison operation
	 * 2  if op1 is a num or strg and op2 is a lst/set/multiset, only when op2 has only one element
	 *    which is equal to op1, return equal(0);  if op2 is a set of more than one duplications of op1,
	 *    return (2).
	 * 3  a num/strg/lst/set/multiset can never be equal to a map/multimap
	 *
	 * @param op1
	 * @param op2
	 * @return return -1 if op1<op2, 0 if op1 == op2 , 1 if op1 > op2,
	 * 2 if op1 and op2 aren't mutual subset or
	 * not all elements in lst is greater than or less than the num/strg
	 */
	Intg CMP(const DataPtr op1_, const DataPtr op2_) {
		DataPtr op1 = DataPtr(op1_);
		DataPtr op2 = DataPtr(op2_);
		while (op1->dataFlagBit == S_PTR) {
			op1 = op1->content_ptr;
		}
		while (op2->dataFlagBit == S_PTR) {
			op2 = op2->content_ptr;
		}
		if (op1->dataFlagBit == S_AR || op2->dataFlagBit == S_AR) {
			if (op1->operator <(*op2)) {
				return -1;
			}
			else if (op1->operator ==(*op2)) {
				return 0;
			}
			else {
				return 1;
			}
		}
		if (op1->dataFlagBit == Sz) {
			if (op2->dataFlagBit == Sz) {
				if (op1->content_i < op2->content_i) {
					return -1;
				}
				else if (op1->content_i == op2->content_i) {
					return 0;
				}
				else {
					return 1;
				}

			}
			else if (op2->dataFlagBit == Zf) {
				if (op1->content_i < toNumb(op2->content_s)) {
					return -1;
				}
				else if (op1->content_i == toNumb(op2->content_s)) {
					return 0;
				}
				else {
					return 1;
				}

			}
			else if (op2->dataFlagBit == COOL_LST) {
				Intg cmpres = 0;
				if (op2->content_lst->size() == 0) {
					return 1;
				}
				else {
					cmpres = CMP(op1, &op2->content_lst->front());
				}
				auto it = op2->content_lst->begin();
				while (++it != op2->content_lst->end()) {
					if (cmpres != CMP(op1, &(*it))) {
						return 2;
					}

				}
				if (op2->content_lst->size() > 1 && cmpres == 0) {
					return 2;
				}
				return cmpres;

			}
			else if (op2->dataFlagBit == SET) {

				Intg cmpres = 0;

				if (op2->content_set->size() == 0) {
					return 1;
				}
				else {
					cmpres = CMP(op1, &(*op2->content_set->begin()));
				}
				auto it = op2->content_set->begin();
				while (++it != op2->content_set->end()) {
					if (cmpres != CMP(op1, &(*it))) {
						return 2;
					}

				}
				if (op2->content_set->size() > 1 && cmpres == 0) {
					return 2;
				}
				return cmpres;

			}
			else if (op2->dataFlagBit == MULTISET) {

				Intg cmpres = 0;

				if (op2->content_multiset->size() == 0) {
					return 1;
				}
				else {
					cmpres = CMP(op1, &op2->content_multiset->begin().operator *());
				}
				auto it = op2->content_multiset->begin();
				while (++it != op2->content_multiset->end()) {
					if (cmpres != CMP(op1, &(*it))) {
						return 2;
					}

				}
				if (op2->content_multiset->size() > 1 && cmpres == 0) {
					return 2;
				}
				return cmpres;

			}
			else if (op2->dataFlagBit == MAP) {
				Intg cmpres = 0;

				if (op2->content_map->size() == 0) {
					return 1;
				}
				else {
					cmpres = CMP(op1, &(op2->content_map->begin()->first));
				}
				auto it = op2->content_map->begin();
				while (++it != op2->content_map->end()) {
					if (cmpres != CMP(op1, &(it->first))) {
						return 2;
					}

				}
				if (cmpres == 0) {
					return 2;
				}
				return cmpres;

			}
			else if (op2->dataFlagBit == MULTIMAP) {

				Intg cmpres = 0;

				if (op2->content_multimap->size() == 0) {
					return 1;
				}
				else {
					cmpres = CMP(op1, &(op2->content_multimap->begin()->first));
				}
				auto it = op2->content_multimap->begin();
				while (++it != op2->content_multimap->end()) {
					if (cmpres != CMP(op1, &(it->first))) {
						return 2;
					}

				}
				if (cmpres == 0) {
					return 2;
				}
				return cmpres;

			}

		}
		else if (op1->dataFlagBit == Zf) {

			if (op2->dataFlagBit == Sz) {
				if (op1->content_s < toStrg(op2->content_i)) {
					return -1;
				}
				else if (op1->content_s == toStrg(op2->content_i)) {
					return 0;
				}
				else {
					return 1;
				}

			}
			else if (op2->dataFlagBit == Zf) {
				if (op1->content_s < (op2->content_s)) {
					return -1;
				}
				else if (op1->content_s == op2->content_s) {
					return 0;
				}
				else {
					return 1;
				}

			}
			else if (op2->dataFlagBit == COOL_LST) {
				Intg cmpres = 0;

				if (op2->content_lst->size() == 0) {
					return 1;
				}
				else {
					cmpres = CMP(op1, &op2->content_lst->front());
				}
				auto it = op2->content_lst->begin();
				while (++it != op2->content_lst->end()) {
					if (cmpres != CMP(op1, &(*it))) {
						return 2;
					}

				}
				if (op2->content_lst->size() > 1 && cmpres == 0) {
					return 2;
				}
				return cmpres;

			}
			else if (op2->dataFlagBit == SET) {

				Intg cmpres = 0;

				if (op2->content_set->size() == 0) {
					return 1;
				}
				else {
					cmpres = CMP(op1, &(*op2->content_set->begin()));
				}
				auto it = op2->content_set->begin();
				while (++it != op2->content_set->end()) {
					if (cmpres != CMP(op1, &(*it))) {
						return 2;
					}

				}
				if (op2->content_set->size() > 1 && cmpres == 0) {
					return 2;
				}
				return cmpres;

			}
			else if (op2->dataFlagBit == MULTISET) {

				Intg cmpres = 0;

				if (op2->content_multiset->size() == 0) {
					return 1;
				}
				else {
					cmpres = CMP(op1, &op2->content_multiset->begin().operator *());
				}
				auto it = op2->content_multiset->begin();
				while (++it != op2->content_multiset->end()) {
					if (cmpres != CMP(op1, &(*it))) {
						return 2;
					}

				}
				if (op2->content_multiset->size() > 1 && cmpres == 0) {
					return 2;
				}
				return cmpres;

			}
			else if (op2->dataFlagBit == MAP) {
				Intg cmpres = 0;

				if (op2->content_map->size() == 0) {
					return 1;
				}
				else {
					cmpres = CMP(op1, &(op2->content_map->begin()->first));
				}
				auto it = op2->content_map->begin();
				while (++it != op2->content_map->end()) {
					if (cmpres != CMP(op1, &(it->first))) {
						return 2;
					}

				}
				if (cmpres == 0) {
					return 2;
				}
				return cmpres;

			}
			else if (op2->dataFlagBit == MULTIMAP) {

				Intg cmpres = 0;

				if (op2->content_multimap->size() == 0) {
					return 1;
				}
				else {
					cmpres = CMP(op1, &(op2->content_multimap->begin()->first));
				}
				auto it = op2->content_multimap->begin();
				while (++it != op2->content_multimap->end()) {
					if (cmpres != CMP(op1, &(it->first))) {
						return 2;
					}

				}
				if (cmpres == 0) {
					return 2;
				}
				return cmpres;

			}

		}
		else if (op1->dataFlagBit == COOL_LST) {

			if (op2->dataFlagBit == Sz) {

				Intg cmpres = 0;

				if (op1->content_lst->size() == 0) {
					return -1;
				}
				else {
					cmpres = CMP(op2, &op1->content_lst->front());
				}
				auto it = op1->content_lst->begin();
				while (++it != op1->content_lst->end()) {
					if (cmpres != CMP(op2, &(*it))) {
						return 2;
					}

				}
				if (op2->content_lst->size() > 1 && cmpres == 0) {
					return 2;
				}
				return cmpres;

			}
			else if (op2->dataFlagBit == Zf) {

				Intg cmpres = 0;

				if (op1->content_lst->size() == 0) {
					return -1;
				}
				else {
					cmpres = CMP(op2, &op1->content_lst->front());
				}
				auto it = op1->content_lst->begin();
				while (++it != op1->content_lst->end()) {
					if (cmpres != CMP(op2, &(*it))) {
						return 2;
					}

				}
				if (op2->content_lst->size() > 1 && cmpres == 0) {
					return 2;
				}
				return cmpres;

			}
			else if (op2->dataFlagBit == COOL_LST) {

				Intg lenlst = min(op1->content_lst->size(),
					op2->content_lst->size());

				auto it1 = op1->content_lst->begin();
				auto it2 = op2->content_lst->begin();
				Intg cmpres = 0;
				while (lenlst-- > 0) {
					if ((cmpres = CMP(&(*it1), &(*it2))) != 0) {
						return cmpres;
					}
					++it1;
					++it2;

				}
				if (op1->content_lst->size() < op2->content_lst->size()) {
					return -1;
				}
				else if (op1->content_lst->size() == op2->content_lst->size()) {
					return 0;
				}
				else {
					return 1;
				}

			}
			else if (op2->dataFlagBit == SET) {

				Intg lenlst = min(op1->content_lst->size(),
					op2->content_set->size());

				auto it1 = op1->content_lst->begin();
				auto it2 = op2->content_set->begin();
				Intg cmpres = 0;
				while (lenlst-- > 0) {
					if ((cmpres = CMP(&(*it1), &(*it2))) != 0) {
						return cmpres;
					}
					++it1;
					++it2;

				}
				if (op1->content_lst->size() < op2->content_set->size()) {
					return -1;
				}
				else if (op1->content_lst->size() == op2->content_set->size()) {
					return 0;
				}
				else {
					return 1;
				}

			}
			else if (op2->dataFlagBit == MULTISET) {

				Intg lenlst = min(op1->content_lst->size(),
					op2->content_multiset->size());

				auto it1 = op1->content_lst->begin();
				auto it2 = op2->content_multiset->begin();
				Intg cmpres = 0;
				while (lenlst-- > 0) {
					if ((cmpres = CMP(&(*it1), &(*it2))) != 0) {
						return cmpres;
					}
					++it1;
					++it2;

				}
				if (op1->content_lst->size() < op2->content_multiset->size()) {
					return -1;
				}
				else if (op1->content_lst->size()
					== op2->content_multiset->size()) {
					return 0;
				}
				else {
					return 1;
				}

			}
			else if (op2->dataFlagBit == MAP) {

				Intg lenlst = min(op1->content_lst->size(),
					op2->content_map->size());

				auto it1 = op1->content_lst->begin();
				auto it2 = op2->content_map->begin();
				Intg cmpres = 0;
				while (lenlst-- > 0) {
					if ((cmpres = CMP(&(*it1), &(it2->first))) != 0) {
						return cmpres;
					}
					++it1;
					++it2;

				}

				if (op1->content_lst->size() < op2->content_map->size()) {
					return -1;
				}
				else if (op1->content_lst->size() == op2->content_map->size()) {
					return 2;
				}
				else {
					return 1;
				}

			}
			else if (op2->dataFlagBit == MULTIMAP) {

				Intg lenlst = min(op1->content_lst->size(),
					op2->content_multimap->size());

				auto it1 = op1->content_lst->begin();
				auto it2 = op2->content_multimap->begin();
				Intg cmpres = 0;
				while (lenlst-- > 0) {
					if ((cmpres = CMP(&(*it1), &(it2->first))) != 0) {
						return cmpres;
					}
					++it1;
					++it2;

				}

				if (op1->content_lst->size() < op2->content_multimap->size()) {
					return -1;
				}
				else if (op1->content_lst->size()
					== op2->content_multimap->size()) {
					return 2;
				}
				else {
					return 1;
				}

			}

		}
		else if (op1->dataFlagBit == SET) {

			if (op2->dataFlagBit == Sz) {

				if (op1->content_set->size() == 0) {
					return -1;
				}
				else if (op1->content_set->size() > 1) {
					if (op1->content_set->count(*op1) > 0) {
						return 1;

					}
					else {
						return 2;
					}
				}
				else {
					if (op1->content_set->count(*op1) > 0) {
						return 0;
					}
					else {
						return 2;
					}
				}

			}
			else if (op2->dataFlagBit == Zf) {

				if (op1->content_set->size() == 0) {
					return -1;
				}
				else if (op1->content_set->size() > 1) {
					if (op1->content_set->count(*op1) > 0) {
						return 1;

					}
					else {
						return 2;
					}
				}
				else {
					if (op1->content_set->count(*op1) > 0) {
						return 0;
					}
					else {
						return 2;
					}
				}

			}
			else if (op2->dataFlagBit == COOL_LST) {

				multiset<Data> op2_;
				op2_.insert(op2->content_lst->begin(), op2->content_lst->end());
				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1->content_set->begin(), op1->content_set->end(),
					op2_.begin(), op2_.end(), inserter(sd1, sd1.begin()));
				set_difference(op2_.begin(), op2_.end(), op1->content_set->begin(),
					op1->content_set->end(), inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						if (op1->content_set->size() == op2->content_lst->size()) {
							return 0;
						}
						else {
							return 2;
						}
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == SET) {

				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1->content_set->begin(), op1->content_set->end(),
					op2->content_set->begin(), op2->content_set->end(),
					inserter(sd1, sd1.begin()));
				set_difference(op2->content_set->begin(), op2->content_set->end(),
					op1->content_set->begin(), op1->content_set->end(),
					inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						return 0;
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == MULTISET) {

				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1->content_set->begin(), op1->content_set->end(),
					op2->content_multiset->begin(),
					op2->content_multiset->end(), inserter(sd1, sd1.begin()));
				set_difference(op2->content_multiset->begin(),
					op2->content_multiset->end(), op1->content_set->begin(),
					op1->content_set->end(), inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						if (op1->content_set->size()
							== op2->content_multiset->size()) {
							return 0;
						}
						else {
							return 2;
						}
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == MAP) {

				multiset<Data> op2_;
				for (auto& itmap : *op2->content_map) {
					op2_.insert(itmap.first);
				}
				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1->content_set->begin(), op1->content_set->end(),
					op2_.begin(), op2_.end(), inserter(sd1, sd1.begin()));
				set_difference(op2_.begin(), op2_.end(), op1->content_set->begin(),
					op1->content_set->end(), inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						return 2;
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == MULTIMAP) {

				multiset<Data> op2_;
				for (auto& itmultimap : *op2->content_multimap) {
					op2_.insert(itmultimap.first);
				}
				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1->content_set->begin(), op1->content_set->end(),
					op2_.begin(), op2_.end(), inserter(sd1, sd1.begin()));
				set_difference(op2_.begin(), op2_.end(), op1->content_set->begin(),
					op1->content_set->end(), inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						return 2;
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}

		}
		else if (op1->dataFlagBit == MULTISET) {

			if (op2->dataFlagBit == Sz) {

				if (op1->content_multiset->size() == 0) {
					return -1;
				}
				else if (op1->content_multiset->size() > 1) {
					if (op1->content_multiset->count(*op1) > 0) {
						return 1;

					}
					else {
						return 2;
					}
				}
				else {
					if (op1->content_multiset->count(*op1) > 0) {
						return 0;
					}
					else {
						return 2;
					}
				}

			}
			else if (op2->dataFlagBit == Zf) {

				if (op1->content_multiset->size() == 0) {
					return -1;
				}
				else if (op1->content_multiset->size() > 1) {
					if (op1->content_multiset->count(*op1) > 0) {
						return 1;

					}
					else {
						return 2;
					}
				}
				else {
					if (op1->content_multiset->count(*op1) > 0) {
						return 0;
					}
					else {
						return 2;
					}
				}

			}
			else if (op2->dataFlagBit == COOL_LST) {

				multiset<Data> op2_;
				op2_.insert(op2->content_lst->begin(), op2->content_lst->end());
				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1->content_multiset->begin(),
					op1->content_multiset->end(), op2_.begin(), op2_.end(),
					inserter(sd1, sd1.begin()));
				set_difference(op2_.begin(), op2_.end(),
					op1->content_multiset->begin(),
					op1->content_multiset->end(), inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						if (op1->content_multiset->size()
							== op2->content_lst->size()) {
							auto itmultiset1 = op1->content_multiset->begin();
							auto itmultiset2 = op2_.begin();
							while (itmultiset1 != op1->content_multiset->end()) {
								if (((*itmultiset1) == (*itmultiset2)) == false) {
									return 2;

								}
								++itmultiset1;
								++itmultiset2;

							}

							return 0;
						}
						else {
							return 2;
						}
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == SET) {

				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1->content_multiset->begin(),
					op1->content_multiset->end(), op2->content_set->begin(),
					op2->content_set->end(), inserter(sd1, sd1.begin()));
				set_difference(op2->content_set->begin(), op2->content_set->end(),
					op1->content_multiset->begin(),
					op1->content_multiset->end(), inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						if (op1->content_multiset->size()
							== op2->content_set->size()) {

							return 0;
						}
						else {
							return 2;
						}
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == MULTISET) {

				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1->content_multiset->begin(),
					op1->content_multiset->end(),
					op2->content_multiset->begin(),
					op2->content_multiset->end(), inserter(sd1, sd1.begin()));
				set_difference(op2->content_multiset->begin(),
					op2->content_multiset->end(),
					op1->content_multiset->begin(),
					op1->content_multiset->end(), inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						if (op1->content_multiset->size()
							== op2->content_multiset->size()) {
							if (op1->content_multiset->size()
								== op2->content_multiset->size()) {
								auto itmultiset1 = op1->content_multiset->begin();
								auto itmultiset2 = op1->content_multiset->begin();
								while (itmultiset1 != op1->content_multiset->end()) {
									if (((*itmultiset1) == (*itmultiset2))
										== false) {
										return 2;

									}
									++itmultiset1;
									++itmultiset2;

								}

								return 0;
							}
							else {
								return 2;
							}
						}
						else {
							return 2;
						}
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == MAP) {

				multiset<Data> op2_;
				for (auto& itmap : *op2->content_map) {
					op2_.insert(itmap.first);
				}
				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1->content_multiset->begin(),
					op1->content_multiset->end(), op2_.begin(), op2_.end(),
					inserter(sd1, sd1.begin()));
				set_difference(op2_.begin(), op2_.end(),
					op1->content_multiset->begin(),
					op1->content_multiset->end(), inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						return 2;
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == MULTIMAP) {

				multiset<Data> op2_;
				for (auto& itmultimap : *op2->content_multimap) {
					op2_.insert(itmultimap.first);
				}
				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1->content_multiset->begin(),
					op1->content_multiset->end(), op2_.begin(), op2_.end(),
					inserter(sd1, sd1.begin()));
				set_difference(op2_.begin(), op2_.end(),
					op1->content_multiset->begin(),
					op1->content_multiset->end(), inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						return 2;
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}

		}
		else if (op1->dataFlagBit == MAP) {

			if (op2->dataFlagBit == Sz) {

				if (op1->content_map->size() == 0) {
					return -1;
				}
				else if (op1->content_map->size() > 1) {
					if (op1->content_map->count(*op1) > 0) {
						return 1;

					}
					else {
						return 2;
					}
				}
				else {
					return 2;
				}

			}
			else if (op2->dataFlagBit == Zf) {

				if (op1->content_map->size() == 0) {
					return -1;
				}
				else if (op1->content_map->size() > 1) {
					if (op1->content_map->count(*op1) > 0) {
						return 1;

					}
					else {
						return 2;
					}
				}
				else {
					return 2;
				}

			}
			else if (op2->dataFlagBit == COOL_LST) {

				multiset<Data> op1_;
				multiset<Data> op2_;
				for (auto itmap : *op1->content_map) {
					op1_.insert(itmap.first);

				}

				op2_.insert(op2->content_lst->begin(), op2->content_lst->end());
				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1_.begin(), op1_.end(), op2_.begin(), op2_.end(),
					inserter(sd1, sd1.begin()));
				set_difference(op2_.begin(), op2_.end(), op1_.begin(), op1_.end(),
					inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						return 2;
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == SET) {
				multiset<Data> op1_;
				for (auto itmap : *op1->content_map) {
					op1_.insert(itmap.first);

				}
				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1_.begin(), op1_.end(), op2->content_set->begin(),
					op2->content_set->end(), inserter(sd1, sd1.begin()));
				set_difference(op2->content_set->begin(), op2->content_set->end(),
					op1_.begin(), op1_.end(), inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						return 2;
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == MULTISET) {

				set<Data> sd1;
				set<Data> sd2;
				multiset<Data> op1_;
				for (auto itmap : *op1->content_map) {
					op1_.insert(itmap.first);

				}
				set_difference(op1_.begin(), op1_.end(),
					op2->content_multiset->begin(),
					op2->content_multiset->end(), inserter(sd1, sd1.begin()));
				set_difference(op2->content_multiset->begin(),
					op2->content_multiset->end(), op1_.begin(), op1_.end(),
					inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						return 2;
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == MAP) {

				bool sd1notnull = false;
				bool sd2notnull = false;
				function<
					bool(const pair<const Data, Data>,
						const pair<const Data, Data>)> op1it_op2it_equal =
					[&](const pair<const Data, Data>& a,
						const pair<const Data, Data>& b) -> bool {
							if (a.first == b.first) {

								if (a.second == b.second) {
									return true;
								}

							}
							return false;
					};
				auto it1 = op1->content_map->begin();
				auto it2 = op2->content_map->begin();
				while ((sd1notnull == false) && (sd2notnull != false)) {
					if (it1 == op1->content_map->end()) {
						if (it2 != op2->content_map->end()) {
							sd2notnull = true;
						}
						else {
							break;
						}

					}
					else {
						if (it2 == op2->content_map->end()) {
							sd1notnull = true;
						}
					}

					if (sd1notnull == false) {
						auto it2tmp = op2->content_map->find(it1->first);
						if (it2tmp != op2->content_map->end()) {
							if (op1it_op2it_equal(*it1, *it2tmp)) {
								if (it2tmp == it2) {
									//two pairs are equal

								}
								else {
									sd2notnull = true;
									it2 = it2tmp;
								}

							}
							else {
								sd1notnull = true;
								sd2notnull = true;
							}
							++it1;
							++it2;

						}
						else {
							sd1notnull = true;
						}

					}
					if (sd2notnull == false) {
						auto it1tmp = op1->content_map->find(it2->first);
						if (it1tmp != op1->content_map->end()) {

							if (op1it_op2it_equal(*it1tmp, *it2)) {
								if (it1tmp == it1) {
									//two pairs are equal

								}
								else {
									sd1notnull = true;
									it1 = it1tmp;
								}

							}
							else {
								sd1notnull = true;
								sd2notnull = true;
							}
							++it1;
							++it2;

						}
						else {
							sd2notnull = true;
						}

					}

				}

				if (sd1notnull == true) {
					if (sd2notnull == true) {
						return 2;
					}
					else {
						return 1;
					}
				}
				else {
					if (sd2notnull == true) {
						return -1;
					}
					else {
						return 0;
					}
				}
			}
			else if (op2->dataFlagBit == MULTIMAP) {

				bool sd1notnull = false;
				bool sd2notnull = false;
				/*
					*this lambda used to tell difference between two different pairs.
					* @return  0 not equal; 1 equal;
				*@attention this function will compair key and value.
				*/
				auto op1it_op2it_equal =
					[](const pair<const Data, Data>& a,
						const pair<const Data,
						multiset<Data, less<Data>, allocator<Data>>>& b) -> bool {
							if (a.first == b.first) {
								if (b.second.size() == 1) {
									if (a.second == b.second.begin().operator *()) {
										return true;
									}
								}
							}
							return false;
					};
				auto it1 = op1->content_map->begin();
				auto it2 = op2->content_multimap->begin();
				while ((sd1notnull == false) && (sd2notnull != false)) {
					if (it1 == op1->content_map->end()) {
						if (it2 != op2->content_multimap->end()) {

							sd2notnull = true;
						}
						else {
							break;
						}

					}
					else {
						if (it2 == op2->content_multimap->end()) {
							sd1notnull = true;
						}
					}

					if (sd1notnull == false) {
						auto it2tmp = op2->content_multimap->find(it1->first);
						if (it2tmp != op2->content_multimap->end()) {
							if (op1it_op2it_equal(*it1, *it2tmp)) {
								if (it2tmp == it2) {
									//two pairs are equal

								}
								else {
									sd2notnull = true;
									it2 = it2tmp;
								}

							}
							else {
								sd1notnull = true;
								sd2notnull = true;
							}
							++it1;
							++it2;

						}
						else {
							sd1notnull = true;
						}

					}
					if (sd2notnull == false) {
						auto it1tmp = op1->content_map->find(it2->first);
						if (it1tmp != op1->content_map->end()) {
							if (op1it_op2it_equal(*it1tmp, *it2)) {
								if (it1tmp == it1) {
									//two pairs are equal

								}
								else {
									sd1notnull = true;
									it1 = it1tmp;
								}

							}
							else {
								sd1notnull = true;
								sd2notnull = true;
							}
							++it1;
							++it2;

						}
						else {
							sd2notnull = true;
						}

					}

				}

				if (sd1notnull == true) {
					if (sd2notnull == true) {
						return 2;
					}
					else {
						return 1;
					}
				}
				else {
					if (sd2notnull == true) {
						return -1;
					}
					else {
						return 0;
					}
				}
			}

		}
		else if (op1->dataFlagBit == MULTIMAP) {

			if (op2->dataFlagBit == Sz) {

				if (op1->content_multimap->size() == 0) {
					return -1;
				}
				else if (op1->content_multimap->size() > 1) {
					if (op1->content_multimap->count(*op1) > 0) {
						return 1;

					}
					else {
						return 2;
					}
				}
				else {
					return 2;
				}

			}
			else if (op2->dataFlagBit == Zf) {

				if (op1->content_multimap->size() == 0) {
					return -1;
				}
				else if (op1->content_multimap->size() > 1) {
					if (op1->content_multimap->count(*op1) > 0) {
						return 1;

					}
					else {
						return 2;
					}
				}
				else {
					return 2;
				}

			}
			else if (op2->dataFlagBit == COOL_LST) {

				//            Intg lenlst = min(op1->content_multimap->size(),
				//                    op2->content_lst->size());
				multiset<Data> op1_;
				multiset<Data> op2_;
				for (auto itmap : *op1->content_multimap) {
					op1_.insert(itmap.first);
				}

				op2_.insert(op2->content_lst->begin(), op2->content_lst->end());
				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1_.begin(), op1_.end(), op2_.begin(), op2_.end(),
					inserter(sd1, sd1.begin()));
				set_difference(op2_.begin(), op2_.end(), op1_.begin(), op1_.end(),
					inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						return 2;
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == SET) {
				multiset<Data> op1_;
				for (auto itmap : *op1->content_multimap) {
					op1_.insert(itmap.first);

				}
				set<Data> sd1;
				set<Data> sd2;
				set_difference(op1_.begin(), op1_.end(), op2->content_set->begin(),
					op2->content_set->end(), inserter(sd1, sd1.begin()));
				set_difference(op2->content_set->begin(), op2->content_set->end(),
					op1_.begin(), op1_.end(), inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						return 2;
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == MULTISET) {

				set<Data> sd1;
				set<Data> sd2;
				multiset<Data> op1_;
				for (auto itmap : *op1->content_multimap) {
					op1_.insert(itmap.first);

				}
				set_difference(op1_.begin(), op1_.end(),
					op2->content_multiset->begin(),
					op2->content_multiset->end(), inserter(sd1, sd1.begin()));
				set_difference(op2->content_multiset->begin(),
					op2->content_multiset->end(), op1_.begin(), op1_.end(),
					inserter(sd2, sd2.begin()));
				if (sd1.size() == 0) {
					if (sd2.size() > 0) {
						return -1;
					}
					else {
						return 2;
					}
				}
				else {
					if (sd2.size() > 0) {
						return 2;
					}
					else {
						return 1;
					}
				}
			}
			else if (op2->dataFlagBit == MAP) {

				bool sd1notnull = 0;
				bool sd2notnull = 0;
				/**
					*this lambda used to tell difference between two different pairs.
					* @return  0 not equal; 1 equal;
				*@attention this function will compair key and value.*/

				auto op1it_op2it_equal = [](
					const pair<const Data,
					multiset<Data, less<Data>, allocator<Data>>>& a,
					const pair<const Data, Data>& b) -> bool {
						if (a.first == b.first) {

							if (a.second.size() == 1
								&& a.second.begin().operator *() == b.second) {
								return true;
							}

						}
						return false;
					};
				auto it1 = op1->content_multimap->begin();
				auto it2 = op2->content_map->begin();
				while ((sd1notnull == false) && (sd2notnull != false)) {
					if (it1 == op1->content_multimap->end()) {
						if (it2 != op2->content_map->end()) {
							sd2notnull = true;
						}
						else {
							break;
						}

					}
					else {
						if (it2 == op2->content_map->end()) {
							sd1notnull = true;
						}
					}

					if (sd1notnull == false) {
						auto it2tmp = op2->content_map->find(it1->first);
						if (it2tmp != op2->content_map->end()) {
							if (op1it_op2it_equal(*it1, *it2tmp)) {
								if (it2tmp == it2) {
									//two pairs are equal

								}
								else {
									sd2notnull = true;
									it2 = it2tmp;
								}

							}
							else {
								sd1notnull = true;
								sd2notnull = true;
							}
							++it1;
							++it2;

						}
						else {
							sd1notnull = true;
						}

					}
					if (sd2notnull == false) {
						auto it1tmp = op1->content_multimap->find(it2->first);
						if (it1tmp != op1->content_multimap->end()) {
							if (op1it_op2it_equal(*it1tmp, *it2)) {
								if (it1tmp == it1) {
									//two pairs are equal

								}
								else {
									sd1notnull = true;
									it1 = it1tmp;
								}

							}
							else {
								sd1notnull = true;
								sd2notnull = true;
							}
							++it1;
							++it2;

						}
						else {
							sd2notnull = true;
						}

					}

				}

				if (sd1notnull == true) {
					if (sd2notnull == true) {
						return 2;
					}
					else {
						return 1;
					}
				}
				else {
					if (sd2notnull == true) {
						return -1;
					}
					else {
						return 0;
					}
				}
			}
			else if (op2->dataFlagBit == MULTIMAP) {

				bool sd1notnull = false;
				bool sd2notnull = false;
				/*	*
						*this lambda used to tell difference between two different pairs.
						* @return  0 not equal; 1 equal;
					*@attention this function will compair key and value.*/

				auto op1it_op2it_equal =
					[](
						const pair<const Data,
						multiset<Data, less<Data>, allocator<Data>>>& a,
						const pair<const Data,
						multiset<Data, less<Data>, allocator<Data>>>& b) -> bool {
							if (a.first == b.first) {

								if (a.second == b.second) {
									return true;
								}

							}
							return false;
					};
				auto it1 = op1->content_multimap->begin();
				auto it2 = op2->content_multimap->begin();
				while ((sd1notnull == false) && (sd2notnull != false)) {
					if (it1 == op1->content_multimap->end()) {
						if (it2 != op2->content_multimap->end()) {
							sd2notnull = true;
						}
						else {
							break;
						}

					}
					else {
						if (it2 == op2->content_multimap->end()) {
							sd1notnull = true;
						}
					}

					if (sd1notnull == false) {
						auto it2tmp = op2->content_multimap->find(it1->first);
						if (it2tmp != op2->content_multimap->end()) {
							if (op1it_op2it_equal(*it1, *it2tmp)) {
								if (it2tmp == it2) {
									//two pairs are equal

								}
								else {
									sd2notnull = true;
									it2 = it2tmp;
								}

							}
							else {
								sd1notnull = true;
								sd2notnull = true;
							}
							++it1;
							++it2;

						}
						else {
							sd1notnull = true;
						}

					}
					if (sd2notnull == false) {
						auto it1tmp = op1->content_multimap->find(it2->first);
						if (it1tmp != op1->content_multimap->end()) {
							if (op1it_op2it_equal(*it1tmp, *it2)) {
								if (it1tmp == it1) {
									//two pairs are equal

								}
								else {
									sd1notnull = true;
									it1 = it1tmp;
								}

							}
							else {
								sd1notnull = true;
								sd2notnull = true;
							}
							++it1;
							++it2;

						}
						else {
							sd2notnull = true;
						}

					}

				}

				if (sd1notnull == true) {
					if (sd2notnull == true) {
						return 2;
					}
					else {
						return 1;
					}
				}
				else {
					if (sd2notnull == true) {
						return -1;
					}
					else {
						return 0;
					}
				}
			}

		}


		THROW_CALCULATION_EXCEPTION(CalculationErrorType::Type, "Unknown type");




	}
	bool Executor::executeCurrentCode() {
		{
#if debug
			static Intg ignorecount = 0;
			cout << "Executor::executeCurrentCode, currentCodeAddr:["

				<< currentCodeFKA.toStrg() << "] in,ignorecount:[" << ignorecount++
				<< "]" << endl;
#endif
#if debug
			cout << "Executor::executeCurrentCode 1" << endl;
#endif
		}
		if (currentCode->assemblyFormula.flagBit.type == COOL_B) {
#if debug
			cout << "Executor::executeCurrentCode COOL_B " << endl;
#endif
			Code& nextCode = (*cdth)[nextCodeFKA];
			if (operatorexe.arg_fka == nextCode.scopeFKA) {

				DataTablePtr conditionalBodyAR = nullptr;
				conditionalBodyAR = DataTablePtr::make();
				conditionalBodyAR->parentAR = currentAR;
				conditionalBodyAR->returnAR = currentAR;
				conditionalBodyAR->meetFKA = currentCodeFKA;

				conditionalBodyAR->queryARList.push_back(currentAR);
				conditionalBodyAR->scopeStructureFKA = nextCode.scopeFKA;
				nextCodeFKI.SI(cdth);
				setNextCodeFKI();
				nextCodeFKA = cdth->getFKAByFKI(nextCodeFKI);
				currentAR = conditionalBodyAR;
			}
			else {

				return true;
			}

		}
		else if (currentCode->assemblyFormula.flagBit.type == COOL_L) {
#if debug
			cout << "Executor::executeCurrentCode COOL_L " << endl;
#endif
			Code& nextCode = (*cdth)[nextCodeFKA];
			if (operatorexe.arg_fka == nextCode.scopeFKA) {

				//register #bk in the parent AR of loop body first
				Arg&& breakResult = currentCode->operator [](0);
				if (currentAR->findall(breakResult) == false) {

					currentAR->add(breakResult);
					Data& bk = currentAR->operator [](breakResult);
					bk.dataFlagBit = Sz;
					bk.content_i = false;
				}

				DataTablePtr conditionalBodyAR = nullptr;
				conditionalBodyAR = DataTablePtr::make();
				conditionalBodyAR->parentAR = currentAR;
				conditionalBodyAR->returnAR = currentAR;
				conditionalBodyAR->meetFKA = currentCodeFKA;

				conditionalBodyAR->queryARList.push_back(currentAR);
				conditionalBodyAR->scopeStructureFKA = nextCode.scopeFKA;

				nextCodeFKI.SI(cdth);
				setNextCodeFKI();
				nextCodeFKA = cdth->getFKAByFKI(nextCodeFKI);
				currentAR = conditionalBodyAR;
			}
			else {

				return true;
			}

		}
		if (currentCode->assemblyFormula.flagBit.type == COOL_C) {
			//创建函数不做处理
		}
		////todo::从此处开始处理函数类型F
		//if (groundingPhaseOps.count(operatorexe.arg_s) > 0) {
		//	currentCode->assemblyFormula.flagBit.functionHandleFlag = T_;
		//	if (operatorexe.arg_s == "EXIST_SUBEXPR") {
		//		currentCode->assemblyFormula.flagBit.tfFKA = FKA(FILEKEYBUILTIN, S_S_EXIST_SUBEXPR);
		//	}
		//	else if (operatorexe.arg_s == "FIND_SUBEXPR") {
		//		currentCode->assemblyFormula.flagBit.tfFKA = FKA(FILEKEYBUILTIN, S_S_FIND_SUBEXPR);
		//	}
		//}
		if (currentCode->assemblyFormula.flagBit.functionHandleFlag == F_) {
#if debug
			cout << "Executor::executeCurrentCode 2 out" << endl;
#endif

			return true;
		}
		//todo::从此处开始处理函数调用的根节点 functionHandleFlag == true
		if (currentCode->assemblyFormula.flagBit.tfFKA == fkanull) {
			//the executor may meet a failed reasoning code, just jump over
#if debug
			cout << "Executor::executeCurrentCode 3 out" << endl;
#endif

			return true;
		}
		else if (currentCode->assemblyFormula.flagBit.tfFKA.fileKey
			== FILEKEYBUILTIN
			|| currentCode->assemblyFormula.flagBit.tfFKA.addr < addrnull) {
#if debug
			//todo::从此处开始处理函数调用的根节点 functionHandleFlag == true 内置函数
			cout << "Executor::executeCurrentCode 4,built-in fun" << endl;
#endif

			bool compareresult = false;

			Numb* a1_i = nullptr;
			Numb* a2_i = nullptr;
			Numb* res_i = nullptr;
			Strg* a1_s = nullptr;
			Strg* a2_s = nullptr;
			Strg* res_s = nullptr;
			DataPtr d1 = nullptr;
			DataPtr d2 = nullptr;
			DataPtr dres = nullptr;
			bool isImmediateData_a1 = false;
			bool isImmediateData_a2 = false;
			bool isImmediateData_res = false;
			if (argexe1.argFlag == Sz) {
				a1_i = &argexe1.arg_i;
			}
			else if (argexe1.argFlag == Zf) {
				a1_s = &argexe1.arg_s;
			}
			else if (argexe1 == argnull) {
#if debug
				{
					static int ignorecount = 0;
					std::cout << "Executor::executeCurrentCode()"
						<< " argexe1 is argnull, has no data."
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")"
						<< std::endl;
				}
#endif
			}
			else if (argexe1.isVar()) {
				if (argexe1.argFlag == Y_Dz) {
					argexe1.ref_ar = currentAR;
					Arg argtmp = globalRefTable[argexe1];
					DataTablePtr artmp =
						globalRefTable.getReferenceData(argexe1).getDestAR();
					if (argtmp.isVar()) {
						d1 = &(*artmp)[argtmp];
						while (d1->dataFlagBit == S_PTR) {
							d1 = d1->content_ptr;
						}
					}
					else {

						d1 = nullptr;
						isImmediateData_a1 = true;
						if (argtmp.argFlag == Sz) {
#if debug
							{
								static int ignorecount = 0;
								cout << "Executor::executeCurrentCode()" << " "
									<< "\tignorecount:[" << ignorecount++
									<< "\t](" << __FILE__ << ":" << __LINE__
									<< ":0" << ")" << endl;
							}
#endif
							a1_i = new Numb(argtmp.arg_i);
							d1 = new Data(argtmp.arg_i);

						}
						else if (argtmp.argFlag == Zf) {
							a1_s = new Strg(argtmp.arg_s);
							d1 = new Data(argtmp.arg_s);
						}

					}

				}
				else {
					try {
						d1 = &(*currentAR)[argexe1];
						while (d1->dataFlagBit == S_PTR) {
							d1 = d1->content_ptr;
						}
					}
					catch (Arg& argr) {
						d1 = nullptr;
						isImmediateData_a1 = true;
						if (argr.argFlag == Sz) {
#if debug
							{
								static int ignorecount = 0;
								cout << "Executor::executeCurrentCode()" << " "
									<< "\tignorecount:[" << ignorecount++
									<< "\t](" << __FILE__ << ":" << __LINE__
									<< ":0" << ")" << endl;
							}
#endif
							a1_i = new Numb(argr.arg_i);
							d1 = new Data(argr.arg_i);

						}
						else if (argr.argFlag == Zf) {
							a1_s = new Strg(argr.arg_s);
							d1 = new Data(argr.arg_s);
						}
					}
				}
				if (d1 != nullptr && !isImmediateData_a1) {
					if (d1->dataFlagBit == Sz) {
						a1_i = &d1->content_i;
					}
					else if (d1->dataFlagBit == Zf) {
						a1_s = &d1->content_s;
					}
					else if (d1->dataFlagBit == S_Bs || d1->dataFlagBit == S_Dz) {
						a1_i = &d1->content_i;
						a1_s = &d1->content_s;

					} //todo: a1_ar.....
				}

			}
			if (argexe2.argFlag == Sz) {
				a2_i = &argexe2.arg_i;
			}
			else if (argexe2.argFlag == Zf) {
				a2_s = &argexe2.arg_s;
			}
			else if (argexe2 == argnull) {
#if debug
				{
					static int ignorecount = 0;
					std::cout << "Executor::executeCurrentCode()"
						<< " argexe2 is argnull, has no data."
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")"
						<< std::endl;
				}
#endif
			}
			else if (argexe2.isVar()) {
				if (argexe2.argFlag == Y_Dz) {
					argexe2.ref_ar = currentAR;
					Arg argtmp = globalRefTable[argexe2];
					DataTablePtr artmp =
						globalRefTable.getReferenceData(argexe2).getDestAR();
					if (argtmp.isVar()) {
						d2 = &(*artmp)[argtmp];
						while (d2->dataFlagBit == S_PTR) {
							d2 = d2->content_ptr;
						}
					}
					else {

						d2 = nullptr;
						isImmediateData_a2 = true;
						if (argtmp.argFlag == Sz) {
#if debug
							{
								static int ignorecount = 0;
								cout << "Executor::executeCurrentCode()" << " "
									<< "\tignorecount:[" << ignorecount++
									<< "\t](" << __FILE__ << ":" << __LINE__
									<< ":0" << ")" << endl;
							}
#endif
							a2_i = new Numb(argtmp.arg_i);
							d2 = new Data(argtmp.arg_i);

						}
						else if (argtmp.argFlag == Zf) {
							a2_s = new Strg(argtmp.arg_s);
							d2 = new Data(argtmp.arg_s);
						}

					}

				}
				else {
					try {
						d2 = &(*currentAR)[argexe2];
						while (d2->dataFlagBit == S_PTR) {
							d2 = d2->content_ptr;
						}
					}
					catch (Arg& argr) {
						d2 = nullptr;
						isImmediateData_a2 = true;
						if (argr.argFlag == Sz) {
							a2_i = new Numb(argr.arg_i);
							d2 = new Data(argr.arg_i);

						}
						else if (argr.argFlag == Zf) {
							a2_s = new Strg(argr.arg_s);
							d2 = new Data(argr.arg_s);
						}
					}
				}

				if (d2 != nullptr && !isImmediateData_a2) {
					if (d2->dataFlagBit == Sz) {
						a2_i = &d2->content_i;
					}
					else if (d2->dataFlagBit == Zf) {
						a2_s = &d2->content_s;
					}
					else if (d2->dataFlagBit == S_Bs || d2->dataFlagBit == S_Dz) {
						a2_i = &d2->content_i;
						a2_s = &d2->content_s;

					} //todo: a2_ar.....
				}

			}
			if (resultexe.argFlag == Sz) {
				res_i = &resultexe.arg_i;
			}
			else if (resultexe.argFlag == Zf) {
				res_s = &resultexe.arg_s;
			}
			else if (resultexe == argnull) {
#if debug
				{
					static int ignorecount = 0;
					std::cout << "Executor::executeCurrentCode()"
						<< " resultexe is argnull, has no data."
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")"
						<< std::endl;
				}
#endif
			}
			else if (resultexe.isVar()) {
				if (resultexe.argFlag == Y_Dz) {
					resultexe.ref_ar = currentAR;
					Arg argtmp = globalRefTable[resultexe];
					DataTablePtr artmp =
						globalRefTable.getReferenceData(resultexe).getDestAR();
					if (argtmp.isVar()) {
						dres = &(*artmp)[argtmp];
						while (dres->dataFlagBit == S_PTR) {
							dres = dres->content_ptr;
						}
					}
					else {

						dres = nullptr;
						isImmediateData_res = true;
						if (argtmp.argFlag == Sz) {
#if debug
							{
								static int ignorecount = 0;
								cout << "Executor::executeCurrentCode()" << " "
									<< "\tignorecount:[" << ignorecount++
									<< "\t](" << __FILE__ << ":" << __LINE__
									<< ":0" << ")" << endl;
							}
#endif
							res_i = new Numb(argtmp.arg_i);
							dres = new Data(argtmp.arg_i);

						}
						else if (argtmp.argFlag == Zf) {
							res_s = new Strg(argtmp.arg_s);
							dres = new Data(argtmp.arg_s);
						}

					}

				}
				else {
					try {
						dres = &(*currentAR)[resultexe];
						while (dres->dataFlagBit == S_PTR) {
							dres = dres->content_ptr;
						}
					}
					catch (Arg& argr) {
						dres = nullptr;
						isImmediateData_res = true;
						if (argr.argFlag == Sz) {
							res_i = new Numb(argr.arg_i);
							dres = new Data(argr.arg_i);

						}
						else if (argr.argFlag == Zf) {
							res_s = new Strg(argr.arg_s);
							dres = new Data(argr.arg_s);
						}
					}
				}
				if (dres != nullptr && !isImmediateData_res) {
					if (dres->dataFlagBit == Sz) {
						res_i = &dres->content_i;
					}
					else if (dres->dataFlagBit == Zf) {
						res_s = &dres->content_s;
					}
					else if (dres->dataFlagBit == S_Bs
						|| dres->dataFlagBit == S_Dz) {
						res_i = &dres->content_i;
						res_s = &dres->content_s;

					} //todo: res_ar.....
				}

			}
			{
#if debug
				static Intg ignorecount = 0;
				cout << "Executor::executeCurrentCode 5,ignorecount:["
					<< ignorecount++ << "]" << endl;
#endif
			}
			if (d1 != nullptr) {
#if debug
				cout << "\td1:" << d1->toStrg() << endl;
#endif
			}
			if (d2 != nullptr) {
#if debug
				cout << "\td2:" << d2->toStrg() << endl;
#endif
			}
			if (dres != nullptr) {
#if debug
				cout << "\tdres:" << dres->toStrg() << endl;
#endif
			}
			//type of operand 1/2/res, the value can only be
			//Sz,Zf,S_LST/COOL_LST,S_MAP/MAP,S_MULTIAMP/MULTIAMP,S_SET/SET,S_MULTISET/MULTISET,S_AR
			Intg type_a1 = 0;
			Intg type_a2 = 0;
			Intg type_res = 0;
			if (d1 != nullptr) {
				type_a1 = d1->dataFlagBit;
				if (type_a1 == COOL_LST && d1->content_lst == nullptr) {
					d1->content_lst = DataLstPtr(new deque<Data>());
				}
				if (type_a1 == SET && d1->content_set == nullptr) {
					d1->content_set = DataSetPtr(new set<Data>());
				}
				if (type_a1 == MULTISET && d1->content_multiset == nullptr) {
					d1->content_multiset = DataMultiSetPtr(new multiset<Data>());
				}
				if (type_a1 == MAP && d1->content_map == nullptr) {
					d1->content_map = DataMapPtr(new map<Data, Data>());
				}
				if (type_a1 == MULTIMAP && d1->content_multimap == nullptr) {
					d1->content_multimap = DataMultiMapPtr(new map<Data, multiset<Data>>());
				}

			}
			else {
				if (a1_i) {
					type_a1 = Sz;
					d1 = new Data(*a1_i);
				}
				else if (a1_s) {
					type_a1 = Zf;
					d1 = new Data(*a1_s);
				}
			}
			if (d2 != nullptr) {
				type_a2 = d2->dataFlagBit;
				if (type_a2 == COOL_LST && d2->content_lst == nullptr) {
					d2->content_lst = DataLstPtr(new deque<Data>());
				}
				if (type_a2 == SET && d2->content_set == nullptr) {
					d2->content_set = DataSetPtr(new set<Data>());
				}
				if (type_a2 == MULTISET && d2->content_multiset == nullptr) {
					d2->content_multiset = DataMultiSetPtr(new multiset<Data>());
				}
				if (type_a2 == MAP && d2->content_map == nullptr) {
					d2->content_map = DataMapPtr(new map<Data, Data>());
				}
				if (type_a2 == MULTIMAP && d2->content_multimap == nullptr) {
					d2->content_multimap = DataMultiMapPtr(new map<Data, multiset<Data>>());
				}
			}
			else {
				if (a2_i) {
					type_a2 = Sz;
					d2 = new Data(*a2_i);
				}
				else if (a2_s) {
					type_a2 = Zf;
					d2 = new Data(*a2_s);
				}
			}
			if (dres != nullptr) {
				type_res = dres->dataFlagBit;
				if (type_res == COOL_LST && dres->content_lst == nullptr) {
					dres->content_lst = DataLstPtr(new deque<Data>());
				}
				if (type_res == SET && dres->content_set == nullptr) {
					dres->content_set = DataSetPtr(new set<Data>());
				}
				if (type_res == MULTISET && dres->content_multiset == nullptr) {
					dres->content_multiset = DataMultiSetPtr(new multiset<Data>());
				}
				if (type_res == MAP && dres->content_map == nullptr) {
					dres->content_map = DataMapPtr(new map<Data, Data>());
				}
				if (type_res == MULTIMAP && dres->content_multimap == nullptr) {
					dres->content_multimap = DataMultiMapPtr(new map<Data, multiset<Data>>());
				}
			}
			else {
#if debug
				{
					static int ignorecount = 0;
					std::cout << "Executor::executeCurrentCode()" << " dres == nullptr"
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")"
						<< std::endl;
				}
#endif
				if (res_i) {
					type_res = Sz;
					dres = new Data(*res_i);
				}
				else if (res_s) {
					type_res = Zf;
					dres = new Data(*res_s);
				}
			}

			/**
				*@brief this switch structure handle the basic operation of the vm_execute.
				* since one operator may depend on situation acts differently, here are the judgement process :
			*a1_type
				* a2_type
				* logic
				* if arg1 / arg2 / argres is modified, please apply the modification on bound data object.
				**/


			switch (currentCode->assemblyFormula.flagBit.tfFKA.addr[0]) {
			case S_S_JIA:
#if debug
				cout << "Executor::executeCurrentCode 5.1 in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "+");

				if (compareresult == true) {
					ADD(d1, d2, dres, type_a1, type_a2, type_res);
				}
#if debug
				cout << "Executor::executeCurrentCode 5.1 out" << endl;
#endif

				break;
			case S_S_JIAN:
#if debug
				cout << "Executor::executeCurrentCode 5.2 in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "-");

				if (compareresult == true) {
					SUB(d1, d2, dres, type_a1, type_a2, type_res);
				}
#if debug
				cout << "Executor::executeCurrentCode 5.2 out" << endl;
#endif
				break;
			case S_S_CHENG:
#if debug
				cout << "Executor::executeCurrentCode 5.3 in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "*");

				if (compareresult == true) {
					MUL(d1, d2, dres, type_a1, type_a2, type_res);

				}
#if debug
				cout << "Executor::executeCurrentCode 5.3 out" << endl;
#endif
				break;
			case S_S_CHU:
#if debug
				cout << "Executor::executeCurrentCode 5.4 in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "/");

				if (compareresult == true) {
					CHU(d1, d2, dres, type_a1, type_a2, type_res);
				}
#if debug
				cout << "Executor::executeCurrentCode 5.4 out" << endl;
#endif
				break;
			case S_S_FUZHI:
#if debug
				cout << "Executor::executeCurrentCode 5.5 in" << endl;
#endif
				compareresult =
					argexe1.isVar()
					&& (operatorexe.argFlag == M_Bs
						&& operatorexe.arg_s == "=");
				if (compareresult == true) {
					/*if (dres == d2) {
						break;
					}
					if (dres != nullptr) {
						dres->clear();

					}
					if (type_a1 == Sz) {
						dres->dataFlagBit = Sz;
						dres->content_i = *a2_i;
					}
					else if (type_a1 == Zf) {
						dres->dataFlagBit = Zf;
						dres->content_s = *a2_s;
					}
					else {*/

					dres->operator =(d2->deepAssign());
					//}

				}
#if debug
				cout << "Executor::executeCurrentCode 5.5 out" << endl;
#endif
				break;
			case S_S_DENGYU:
#if debug
				cout << "Executor::executeCurrentCode 5.6 in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "==");
				if (compareresult == true) {

					bool res = (CMP(d1, d2) == 0) && d1->dataFlagBit == d2->dataFlagBit;
					dres->clear();
					dres->dataFlagBit = Sz;
					dres->content_i = res;

				}
#if debug
				cout << "Executor::executeCurrentCode 5.6 out" << endl;
#endif
				break;
			case S_S_BUDENG:
#if debug
				cout << "Executor::executeCurrentCode BUDENG in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "!=");
				if (compareresult == true) {

					bool res = (CMP(d1, d2) != 0) || d1->dataFlagBit != d2->dataFlagBit;
					dres->clear();
					dres->dataFlagBit = Sz;
					dres->content_i = res;
				}
#if debug
				cout << "Executor::executeCurrentCode BUDENG out" << endl;
#endif
				break;
			case S_S_DAYU:
#if debug
				cout << "Executor::executeCurrentCode 5.6.1 in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == ">");
				if (compareresult == true) {
					if (dres != nullptr) {
						if (dres->dataFlagBit != Sz) {
							dres->clear();
						}
					}
					else {
						dres = DataPtr(new Data());
						isImmediateData_res = true;
					}
					dres->dataFlagBit = Sz;
					if (type_a1 == Sz) {
						if (type_a2 == Sz) {
							dres->content_i = (*a1_i) > (*a2_i);
						}
						else if (type_a2 == Zf) {
							dres->content_i = (*a1_i) > toNumb(*a2_s);
						}

					}
					else if (type_a1 == Zf) {
						if (type_a2 == Zf) {
							dres->content_i = (a1_s->compare(*a2_s) > 0);

						}
						else if (type_a2 == Sz) {
							dres->content_i = (a1_s->compare(toStrg(*a2_i)) > 0);
						}
					}
					else if (d1 != nullptr && d2 != nullptr) {
						dres->content_i = (CMP(d1, d2) == 1);

					}

				}
#if debug
				cout << "Executor::executeCurrentCode 5.6.1 out" << endl;
#endif
				break;
			case S_S_XIAOYU:
#if debug
				cout << "Executor::executeCurrentCode 5.6.2 in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "<");
				if (compareresult == true) {
					if (dres != nullptr) {
						if (dres->dataFlagBit != Sz) {
							dres->clear();
						}
					}
					else {
						dres = DataPtr(new Data());
						isImmediateData_res = true;
					}
					dres->dataFlagBit = Sz;
					if (type_a1 == Sz) {
						if (type_a2 == Sz) {
							dres->content_i = (*a1_i) < (*a2_i);
						}
						else if (type_a2 == Zf) {
							dres->content_i = (*a1_i) < toNumb(*a2_s);
						}

					}
					else if (type_a1 == Zf) {
						if (type_a2 == Zf) {
							dres->content_i = (a1_s->compare(*a2_s) < 0);

						}
						else if (type_a2 == Sz) {
							dres->content_i = (a1_s->compare(toStrg(*a2_i)) < 0);
						}
					}
					else if (d1 != nullptr && d2 != nullptr) {
						dres->content_i = (CMP(d1, d2) == -1);

					}

				}
#if debug
				cout << "Executor::executeCurrentCode 5.6.2 out" << endl;
#endif
				break;
			case S_S_BUDAYU:
#if debug
				cout << "Executor::executeCurrentCode BUDAYU in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "<=");
				if (compareresult == true) {
					if (dres != nullptr) {
						if (dres->dataFlagBit != Sz) {
							dres->clear();
						}
					}
					else {
						dres = DataPtr(new Data());
						isImmediateData_res = true;
					}
					dres->dataFlagBit = Sz;
					if (type_a1 == Sz) {
						if (type_a2 == Sz) {
							dres->content_i = (*a1_i) <= (*a2_i);
						}
						else if (type_a2 == Zf) {
							dres->content_i = (*a1_i) <= toNumb(*a2_s);
						}

					}
					else if (type_a1 == Zf) {
						if (type_a2 == Zf) {
							dres->content_i = (a1_s->compare(*a2_s) <= 0);

						}
						else if (type_a2 == Sz) {
							dres->content_i = (a1_s->compare(toStrg(*a2_i)) <= 0);
						}
					}
					else if (d1 != nullptr && d2 != nullptr) {
						dres->content_i = !(CMP(d1, d2) == 1);

					}

				}
#if debug
				cout << "Executor::executeCurrentCode BUDAYU out" << endl;
#endif
				break;
			case S_S_BUXIAOYU:
#if debug
				cout << "Executor::executeCurrentCode BUXIAOYU in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == ">=");
				if (compareresult == true) {
					if (dres != nullptr) {
						if (dres->dataFlagBit != Sz) {
							dres->clear();
						}
					}
					else {
						dres = DataPtr(new Data());
						isImmediateData_res = true;
					}
					dres->dataFlagBit = Sz;
					if (type_a1 == Sz) {
						if (type_a2 == Sz) {
							dres->content_i = (*a1_i) >= (*a2_i);
						}
						else if (type_a2 == Zf) {
							dres->content_i = (*a1_i) >= toNumb(*a2_s);
						}

					}
					else if (type_a1 == Zf) {
						if (type_a2 == Zf) {
							dres->content_i = (a1_s->compare(*a2_s) >= 0);

						}
						else if (type_a2 == Sz) {
							dres->content_i = (a1_s->compare(toStrg(*a2_i)) >= 0);
						}
					}
					else if (d1 != nullptr && d2 != nullptr) {
						dres->content_i = !(CMP(d1, d2) == -1);

					}

				}
#if debug
				cout << "Executor::executeCurrentCode BUXIAOYU out" << endl;
#endif
				break;
			case S_S_MI:
#if debug
				cout << "Executor::executeCurrentCode 5.7 in" << endl;
#endif

				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "^");

				if (compareresult) {

					POW(d1, d2, dres, type_a1, type_a2, type_res);
				}
#if debug
				cout << "Executor::executeCurrentCode 5.7 out" << endl;
#endif
				break;
			case CALL:
#if debug
				cout << "Executor::executeCurrentCode 5.8 in" << endl;
#endif
				compareresult =
					(operatorexe.argFlag == M_Bs
						&& (operatorexe.arg_s == "call"
							|| operatorexe.arg_s == "ca"));
#if debug
				cout << "Executor::executeCurrentCode 5.8 out" << endl;
#endif
				break;
			case SHUCHU: //后面自动换行
#if debug
				cout << "Executor::executeCurrentCode 5.9 in" << endl;
#endif
				std::cout.precision(16);
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "-->");
				if (compareresult == true) {

					if (debugMode) {
						cout
							<< "------------------------------------------"
							"debug output-----------------------------------------"
							<< endl;
					}
#if debug
					cout << "d1:[" << d1->toStrg() << "]" << endl;
					cout << "d2:[" << d2->toStrg() << "]" << endl;
#endif
					Strg leftOp;
					Strg rightOp;
					Strg inpath;
					Strg outpath;

					if (d1 != nullptr) {
						if (d1->dataFlagBit == S_AR) {

							leftOp =
								"{SystemName:"
								+ (*systh)[d1->content_ar->scopeStructureFKA].systemName
								+ "}";
						}
						else {
							leftOp = d1->valueToStrg();
						}

					}
					else if (a1_i) {
						leftOp = toStrg(*a1_i);
					}
					else if (a1_s) {
						leftOp = *a1_s;
					}
					if (d2 != nullptr) {
						if (d2->dataFlagBit == S_AR) {

							rightOp =
								"{SystemName:"
								+ (*systh)[d2->content_ar->scopeStructureFKA].systemName
								+ "}";
						}
						else {
							rightOp = d2->valueToStrg();
						}

					}
					else if (a2_i) {
						rightOp = toStrg(*a2_i);
					}
					else if (a2_s) {
						rightOp = *a2_s;
					}

					//以下内容曾导致“relocation truncated to fit”

					parseIOFileName(leftOp, rightOp, inpath, outpath);

					if (inpath != "") {
						if (inpath == IO_KEYBOARD) {
							if (outpath != "") {
								if (outpath == IO_SCREEN) {
									Strg s;
									cin >> s;
									cout << s;
									flush(cout);
								}
								else {
									Strg s;
									cin >> s;
									fstream fout(outpath, fstream::out | fstream::app);
									fout << s;
									fout.close();
								}
							}
							else {
								Strg s;
								cin >> s;
								if (d2 != nullptr) {
									d2->dataFlagBit = Zf;
									d2->content_s = s;
								}
							}

						}
						else {
							try {
								fstream fin(inpath, fstream::in);
								//                            Strg s;
								//                            fin.seekg(0, fin.end);
								//                            s.reserve((int)fin.tellg()+1);
								//                            fin.seekg(0, fin.beg);
								//                            fin.read(&s[0], fin.tellg());
								std::string s((std::istreambuf_iterator<char>(fin)),
									std::istreambuf_iterator<char>());
								//                            fin >> s;
								fin.close();
								if (outpath != "") {
									if (outpath == IO_SCREEN) {
										cout << s;
										flush(cout);
									}
									else {
										fstream fout(outpath, fstream::out | fstream::app);
										fout << s;
										fout.close();
									}
								}
								else {
									if (d2 != nullptr) {
										d2->dataFlagBit = Zf;
										d2->content_s = s;
									}
								}

							}
							catch (exception& e) {


								{
									static int ignorecount = 0;
									std::cerr << "Executor::executeCurrentCode()"
										<< "error in opening file , exit(-1) "
										<< "\tignorecount:[" << ignorecount++
										<< "\t](" << __FILE__ << ":" << __LINE__
										<< ":0" << ")" << std::endl;
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
								assert(false); exit(-1);
							}

						}

					}
					else {

						if (outpath != "") {
							if (outpath == IO_SCREEN) {
								cout << leftOp;
								flush(cout);
							}
							else {
								fstream fout(outpath, fstream::out | fstream::app);
								fout << leftOp;
								fout.close();
							}
						}
						else {
							if (d2 != nullptr) {
								d2->dataFlagBit = Zf;
								d2->content_s = leftOp;
							}
						}

					}

					//                cout << endl;
					std::flush(cout);

					if (debugMode) {
						cout
							<< "\n"
							"------------------------------------------"
							"-----------------------------------------------------"

							<< endl;

					}

#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = "------------------------------------------"
							"debug output-----------------------------------------\n" +
							leftOp + " --> " + rightOp + "\n"
							"------------------------------------------"
							"-----------------------------------------------------"; // Replace with actual info

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
				else {

					cerr
						<< "Executor::executeCurrentCode ,SHUCHU err,exit(-1),currentCode:["
						<< currentCode->toStrg() << "]" << endl;

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
					assert(false); exit(-1);
				}

#if debug
				cout << "Executor::executeCurrentCode 5.9 out" << endl;
#endif
				break;

			case LST_ACCESS:
#if debug
				cout << "Executor::executeCurrentCode LST_ACCESS in" << endl;
#endif

				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "[]");
				/*	*
						*@brief access a list*/


				if (compareresult == true) {

					ACC(d1, d2, dres, 0, type_a1, type_a2, type_res);
				}
#if debug
				cout << "Executor::executeCurrentCode LST_ACCESS out" << endl;
#endif
				break;
			case LENGTH:
#if debug
				cout << "Executor::executeCurrentCode LENGTH in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "LENGTH");
				if (compareresult == true) {
					dres->clear();
					dres->dataFlagBit = Sz;
					if (d1->dataFlagBit == S_LST) {
						dres->content_i = d1->content_lst->size();

					}
					else if (d1->dataFlagBit == S_SET) {

						dres->content_i = d1->content_set->size();
					}
					else if (d1->dataFlagBit == S_MULTISET) {

						dres->content_i = d1->content_multiset->size();
					}
					else if (d1->dataFlagBit == S_MAP) {

						dres->content_i = d1->content_map->size();
					}
					else if (d1->dataFlagBit == S_MULTIMAP) {

						dres->content_i = d1->content_multimap->size();
					}
					else if (d1->dataFlagBit == Zf) {

						dres->content_i = d1->content_s.size();
					}
					else if (d1->dataFlagBit == Zf) {

						dres->content_i = 0;
					}

				}
#if debug
				cout << "Executor::executeCurrentCode LENGTH out" << endl;
#endif
				break;
			case CLEAR:
#if debug
				cout << "Executor::executeCurrentCode CLEAR in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "CLEAR");
				if (compareresult == true) {
					bool isref = d1->isReferenceVar;
					Intg datatype = d1->dataFlagBit;
					d1->isReferenceVar = false;
					d1->clear();
					d1->isReferenceVar = isref;
					d1->dataFlagBit = datatype;
					if (d1->dataFlagBit == S_LST) {
						d1->content_lst = DataLstPtr(new deque<Data>());

					}
					else if (d1->dataFlagBit == S_SET) {
						d1->content_set = DataSetPtr(new set<Data>());
					}
					else if (d1->dataFlagBit == S_MULTISET) {
						d1->content_multiset = DataMultiSetPtr(new multiset<Data>());
					}
					else if (d1->dataFlagBit == S_MAP) {
						d1->content_map = DataMapPtr(new map<Data, Data>());
					}
					else if (d1->dataFlagBit == S_MULTIMAP) {
						d1->content_multimap = DataMultiMapPtr(new map<Data, multiset<Data>>());
					}

				}
#if debug
				cout << "Executor::executeCurrentCode CLEAR out" << endl;
#endif
				break;

			case ERASE:
#if debug
				cout << "Executor::executeCurrentCode ERASE in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "ERASE");

				if (compareresult == true) {
					ERS(d1, d2, 0, type_a1, type_a2, type_res);
				}
#if debug
				cout << "Executor::executeCurrentCode ERASE out" << endl;
#endif
				break;
			case TYPENAME:
#if debug
				cout << "Executor::executeCurrentCode TYPENAME in" << endl;
#endif
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "TYPENAME");
				if (compareresult == true) {
					dres->dataFlagBit = Zf;
					switch (d1->dataFlagBit) {

					case Dz:
						dres->content_s = "default";
						break;

					case Sz:

						dres->content_s = "number";
						break;
					case Zf:

						dres->content_s = "string";
						break;
					case COOL_LST:

						dres->content_s = "list";
						break;
					case S_AR:

						dres->content_s =
							(*systh)[d1->content_ar->scopeStructureFKA].systemName;
						break;
					case S_SET:
						dres->content_s = "set";
						break;
					case S_MULTISET:
						dres->content_s = "multiset";
						break;
					case S_MAP:
						dres->content_s = "map";
						break;
					case S_MULTIMAP:
						dres->content_s = "multimap";
						break;

					default:
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = "Unknown Type" + this->toBrief(); // Replace with actual info
						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
						std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
						OutputDebugString(winfo.c_str());
#endif
						cout << info << endl;
						ignoreCount++;
					}

					break;
					}

				}
#if debug
				cout << "Executor::executeCurrentCode LST_ACCESS out" << endl;
#endif
				break;
			case S_S_AND:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "&&");
#if debug
				cout << "Executor::executeCurrentCode S_S_AND in" << endl;
#endif
				if (compareresult) {
					dres->clear();
					dres->dataFlagBit = Sz;
					if (type_a1 == Sz) {
						if (type_a2 == Sz) {
							dres->content_i = *a1_i && *a2_i;
						}
						else if (type_a2 == Zf) {
							dres->content_i = *a1_i && *a2_s != "";
						}
						else {
							dres->content_i = *a1_i && d2->containerSize();
						}
					}
					else if (type_a1 == Zf) {
						if (type_a2 == Sz) {
							dres->content_i = *a1_s != "" && (*a2_i);
						}
						else if (type_a2 == Zf) {
							dres->content_i = *a1_s != "" && *a2_s != "";
						}
						else {
							dres->content_i = *a1_s != "" && d2->containerSize();
						}

					}
					else {
						dres->content_i = d1->containerSize()
							&& d2->containerSize();
					}

				}
#if debug
				cout << "Executor::executeCurrentCode S_S_AND out" << endl;
#endif
				break;
			case S_S_OR:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "||");
#if debug
				cout << "Executor::executeCurrentCode S_S_OR in" << endl;
#endif
				if (compareresult) {
					dres->clear();
					dres->dataFlagBit = Sz;
					if (type_a1 == Sz) {
						if (type_a2 == Sz) {
							dres->content_i = *a1_i || *a2_i;
						}
						else if (type_a2 == Zf) {
							dres->content_i = *a1_i || *a2_s != "";
						}
						else {
							dres->content_i = *a1_i || d2->containerSize();
						}
					}
					else if (type_a1 == Zf) {
						if (type_a2 == Sz) {
							dres->content_i = *a1_s != "" || (*a2_i);
						}
						else if (type_a2 == Zf) {
							dres->content_i = *a1_s != "" || *a2_s != "";
						}
						else {
							dres->content_i = *a1_s != "" || d2->containerSize();
						}

					}
					else {
						dres->content_i = d1->containerSize()
							|| d2->containerSize();
					}

				}
#if debug
				cout << "Executor::executeCurrentCode S_S_OR out" << endl;
#endif
				break;
			case S_S_NOT:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "!");
#if debug
				cout << "Executor::executeCurrentCode S_S_NOT in" << endl;
#endif
				if (compareresult) {

					dres->dataFlagBit = Sz;
					if (type_a2 == Sz) {
						dres->content_i = !((bool)*a2_i);
					}
					else if (type_a2 == Zf) {
						dres->content_i = !(*a2_s != "");
					}
					else {

						dres->content_i = !((bool)d2->containerSize());

					}

				}
#if debug
				cout << "Executor::executeCurrentCode S_S_NOT out" << endl;
#endif
				break;
			case S_S_MODULO:
				/**
					*string will be convert to number before operation.*/

				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "%");
#if debug
				cout << "Executor::executeCurrentCode S_S_MODULO in" << endl;
#endif
				if (compareresult) {
					if (type_a1 == Sz) {
						if (type_a2 == Sz) {
							dres->dataFlagBit = Sz;
							dres->content_i = fmod(*a1_i, *a2_i);
						}
						else if (type_a2 == Zf) {
							dres->dataFlagBit = Sz;
							try {
								dres->content_i = fmod(*a1_i, toNumb(*a2_s));
							}
							catch (exception& e) {

								{
									static int ignorecount = 0;
									std::cerr << "Executor::executeCurrentCode()"
										<< " modulo zero error, exit(-1) "
										<< "\tignorecount:[" << ignorecount++
										<< "\t](" << __FILE__ << ":" << __LINE__
										<< ":0" << ")" << std::endl;
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
								assert(false); exit(-1);;

							}
						}
					}
					else {

						{
							static int ignorecount = 0;
							std::cerr << "Executor::executeCurrentCode()"
								<< " undefined modulo operation, exit(-1) "
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
						assert(false); exit(-1);

					}

				}
#if debug
				cout << "Executor::executeCurrentCode S_S_MODULO out" << endl;
#endif
				break;
			case S_S_JIADENG:
				/**
					*if left operant is a number, operation result would be number, vice versa
					* if left operant is a set、multiset、list, same to insert.*/

				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "+=");
#if debug
				cout << "Executor::executeCurrentCode S_S_JIADENG in" << endl;
#endif
				if (compareresult) {
					ADD(d1, d2, d1, type_a1, type_a2, type_res);

				}
#if debug
				cout << "Executor::executeCurrentCode S_S_JIADENG out" << endl;
#endif
				break;
			case S_S_JIANDENG:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "-=");
#if debug
				cout << "Executor::executeCurrentCode S_S_JIANDENG in" << endl;
#endif
				if (compareresult) {
					SUB(d1, d2, d1, type_a1, type_a2, type_res);
				}
#if debug
				cout << "Executor::executeCurrentCode S_S_JIANDENG out" << endl;
#endif
				break;
			case S_S_CHENGDENG:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "*=");
#if debug
				cout << "Executor::executeCurrentCode S_S_CHENGDENG in" << endl;
#endif
				if (compareresult) {
					MUL(d1, d2, d1, type_a1, type_a2, type_res);
				}

#if debug
				cout << "Executor::executeCurrentCode S_S_CHENGDENG out" << endl;
#endif
				break;
			case S_S_CHUDENG:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "/=");
#if debug
				cout << "Executor::executeCurrentCode S_S_CHUDENG in" << endl;
#endif
				if (compareresult) {
					CHU(d1, d2, d1, type_a1, type_a2, type_res);
				}
#if debug
				cout << "Executor::executeCurrentCode S_S_CHUDENG out" << endl;
#endif
				break;
			case S_S_MODENG:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "%=");
#if debug
				cout << "Executor::executeCurrentCode S_S_MODENG in" << endl;
#endif
				if (compareresult) {
					if (type_a1 == Sz) {
						if (type_a2 == Sz) {
							d1->dataFlagBit = Sz;
							d1->content_i = fmod(*a1_i, *a2_i);
						}
						else if (type_a2 == Zf) {
							d1->dataFlagBit = Sz;
							try {
								d1->content_i = fmod(*a1_i, toNumb(*a2_s));
							}
							catch (exception& e) {

								{
									static int ignorecount = 0;
									std::cerr << "Executor::executeCurrentCode()"
										<< " modulo zero error, exit(-1) "
										<< "\tignorecount:[" << ignorecount++
										<< "\t](" << __FILE__ << ":" << __LINE__
										<< ":0" << ")" << std::endl;
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
								assert(false); exit(-1);

							}
						}
					}
					else {

						{
							static int ignorecount = 0;
							std::cerr << "Executor::executeCurrentCode()"
								<< " undefined modulo operation, exit(-1) "
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
						assert(false); exit(-1);

					}

				}
#if debug
				cout << "Executor::executeCurrentCode S_S_MODENG out" << endl;
#endif
				break;
			case S_S_MIDENG:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "^=");
#if debug
				cout << "Executor::executeCurrentCode S_S_MIDENG in" << endl;
#endif
				if (compareresult) {
					POW(d1, d2, d1, type_a1, type_a2, type_res);
				}
#if debug
				cout << "Executor::executeCurrentCode S_S_MIDENG out" << endl;
#endif
				break;
			case S_SI:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "++");
#if debug
				cout << "Executor::executeCurrentCode S_SI in" << endl;
#endif
				if (compareresult) {

					if (d1->dataFlagBit == Sz) {
						Intg orig = d1->content_i;
						dres->clear();
						dres->dataFlagBit = Sz;
						dres->content_i = orig;
						d1->content_i = orig + 1;

					}
					else {

						{
							static int ignorecount = 0;
							std::cerr << "Executor::executeCurrentCode()"
								<< " undefined op1++, exit(-1) "
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
						assert(false); exit(-1);
					}
				}
#if debug
				cout << "Executor::executeCurrentCode S_SI out" << endl;
#endif
				break;
			case S_SD:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "--");
#if debug
				cout << "Executor::executeCurrentCode S_SD in" << endl;
#endif
				if (compareresult) {

					if (d1->dataFlagBit == Sz) {
						Intg orig = d1->content_i;
						dres->clear();
						dres->dataFlagBit = Sz;
						dres->content_i = orig;
						d1->content_i = orig - 1;

					}
					else {

						{
							static int ignorecount = 0;
							std::cerr << "Executor::executeCurrentCode()"
								<< " undefined op1--, exit(-1) "
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
						assert(false); exit(-1);
					}
				}
#if debug
				cout << "Executor::executeCurrentCode S_SD out" << endl;
#endif
				break;
			case S_MAP:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "MAP");
#if debug
				cout << "Executor::executeCurrentCode S_SD in" << endl;
#endif
				if (compareresult) {

					dres->clear();
					dres->dataFlagBit = MAP;
					dres->content_map = DataMapPtr(new map<Data, Data>());
				}
#if debug
				cout << "Executor::executeCurrentCode S_SD out" << endl;
#endif
				break;
			case S_MULTIMAP:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "MULTIMAP");
#if debug
				cout << "Executor::executeCurrentCode MULTIMAP in" << endl;
#endif
				if (compareresult) {

					dres->clear();
					dres->dataFlagBit = MULTIMAP;
					dres->content_multimap = DataMultiMapPtr(new map<Data, multiset<Data>>());
				}
#if debug
				cout << "Executor::executeCurrentCode MULTIMAP out" << endl;
#endif
				break;
			case S_SET:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "SET");
#if debug
				cout << "Executor::executeCurrentCode SET in" << endl;
#endif
				if (compareresult) {
					dres->clear();
					dres->dataFlagBit = SET;
					dres->content_set = DataSetPtr(new set<Data>());

				}
#if debug
				cout << "Executor::executeCurrentCode SET out" << endl;
#endif
				break;
			case S_MULTISET:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "MULTISET");
#if debug
				cout << "Executor::executeCurrentCode MULTISET in" << endl;
#endif
				if (compareresult) {
					dres->clear();
					dres->dataFlagBit = MULTISET;
					dres->content_multiset = DataMultiSetPtr(new multiset<Data>());

				}
#if debug
				cout << "Executor::executeCurrentCode MULTISET out" << endl;
#endif
				break;
			case S_TONUM:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "TONUM");
#if debug
				cout << "Executor::executeCurrentCode TONUM in" << endl;
#endif
				if (compareresult) {

					auto num = d1->valueToNumb();

					dres->clear();
					dres->dataFlagBit = Sz;
					dres->content_i = num;

				}
#if debug
				cout << "Executor::executeCurrentCode TONUM out" << endl;
#endif
				break;
			case S_TOSTRG:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "TOSTRG");
#if debug
				cout << "Executor::executeCurrentCode TOSTRG in" << endl;
#endif
				if (compareresult) {

					auto&& valstr = d1->valueToStrg();

					dres->clear();
					dres->dataFlagBit = Zf;
					dres->content_s = std::move(valstr);

				}
#if debug
				cout << "Executor::executeCurrentCode TOSTRG out" << endl;
#endif
				break;
			case S_TOINT:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "TOINT");
#if debug
				cout << "Executor::executeCurrentCode TOINT in" << endl;
#endif
				if (compareresult) {

					Intg num = (Intg)d1->valueToNumb();

					dres->clear();
					dres->dataFlagBit = Sz;
					dres->content_i = num;

				}
#if debug
				cout << "Executor::executeCurrentCode TOINT out" << endl;
#endif
				break;
			case S_NONBLOCKEXECUTE:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "NONBLOCKEXECUTE");
#if debug
				cout << "Executor::executeCurrentCode NONBLOCKEXECUTE in" << endl;
#endif

				if (compareresult) {
					//                thread t(system, d1->valueToStrg().c_str());
				}

#if debug
				cout << "Executor::executeCurrentCode NONBLOCKEXECUTE out" << endl;
#endif
				break;
			case S_BLOCKEXECUTE:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "BLOCKEXECUTE");
#if debug
				cout << "Executor::executeCurrentCode BLOCKEXECUTE in" << endl;
#endif
				if (compareresult) {

					system(d1->valueToStrg().c_str());
				}
#if debug
				cout << "Executor::executeCurrentCode BLOCKEXECUTE out" << endl;
#endif
				break;
			case S_SLEEP:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "SLEEP");
#if debug
				cout << "Executor::executeCurrentCode SLEEP in" << endl;
#endif
				if (compareresult) {
					//                sleep();
					std::this_thread::sleep_for(std::chrono::microseconds((Intg)(d1->valueToNumb())));
				}
#if debug
				cout << "Executor::executeCurrentCode SLEEP out" << endl;
#endif
				break;

			case S_LOGICJUMP:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "LOGICJUMP");
#if debug
				cout << "Executor::executeCurrentCode LOGICJUMP in" << endl;
#endif
				if (compareresult) {
					this->logicStage = ((Intg)(d1->valueToNumb()));
				}
#if debug
				cout << "Executor::executeCurrentCode LOGICJUMP out" << endl;
#endif
				break;
			case S_FIND:
				//for lst/set/multiset/string/map/multimap return return an integer, -1 for not found, else return the position of ite.
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "FIND");
#if debug
				cout << "Executor::executeCurrentCode FIND in" << endl;
#endif

				if (compareresult) {
					Intg pos = FND(d1, d2, type_a1, type_a2, type_res, d2);
					dres->clear();
					dres->dataFlagBit = Sz;
					dres->content_i = pos;
				}

#if debug
				cout << "Executor::executeCurrentCode FIND out" << endl;
#endif
				break;
			case S_COUNT:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "COUNT");
#if debug
				cout << "Executor::executeCurrentCode COUNT in" << endl;
#endif
				if (compareresult) {
					dres->clear();
					dres->dataFlagBit = Sz;
					dres->content_i = 0;

					if (d1->dataFlagBit == Zf) {
						int pos = 0;
						while (pos != (int)string::npos) {
							pos = d1->content_s.find(d2->valueToStrg(), ++pos);
							if (pos != (int)string::npos) {
								dres->content_i++;
							}

						}

					}
					else if (d1->dataFlagBit == S_LST) {
						auto pos = d1->content_lst->begin();
						while (pos != d1->content_lst->end()) {

							if ((*pos).operator ==(*d2)) {
								dres->content_i++;
							}
							pos++;

						}
					}
					else if (d1->dataFlagBit == S_MAP) {
						dres->content_i = (FND(d1, d2, type_a1, type_a2, type_res,
							d2) >= 0);

					}
					else if (d1->dataFlagBit == S_MULTIMAP) {
						dres->content_i = d1->content_multimap->count(*d2);
						if (dres->content_i > 0) {
							dres->content_i =
								d1->content_multimap->operator [](*d2).size();

						}

					}
					else if (d1->dataFlagBit == S_SET) {
						dres->content_i = (FND(d1, d2, type_a1, type_a2, type_res,
							d2) >= 0);

					}
					else if (d1->dataFlagBit == S_MULTISET) {
						dres->content_i = d1->content_multiset->count(*d2);
					}

					else {

						dres->clear();
						dres->dataFlagBit = Sz;
						dres->content_i = false;
					}

				}

#if debug
				cout << "Executor::executeCurrentCode COUNT out" << endl;
#endif
				break;
			case S_INSERT:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "INSERT");
#if debug
				cout << "Executor::executeCurrentCode INSERT in" << endl;
#endif

				if (compareresult) {

					INS(d1, d2, type_a1, type_a2, type_res);
				}
#if debug
				cout << "Executor::executeCurrentCode INSERT out" << endl;
#endif
				break;
			case S_PUSHBACK:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "PUSHBACK");
#if debug
				cout << "Executor::executeCurrentCode PUSHBACK in" << endl;
#endif

				if (compareresult) {
					PSB(d1, d2, type_a1, type_a2, type_res);
				}

#if debug
				cout << "Executor::executeCurrentCode PUSHBACK out" << endl;
#endif
				break;
			case S_PUSHFRONT:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "PUSHFRONT");
#if debug
				cout << "Executor::executeCurrentCode PUSHFRONT in" << endl;
#endif

				if (compareresult) {
					PSF(d1, d2, type_a1, type_a2, type_res);

				}
#if debug
				cout << "Executor::executeCurrentCode PUSHFRONT out" << endl;
#endif
				break;
			case S_POPBACK:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "POPBACK");
#if debug
				cout << "Executor::executeCurrentCode POPBACK in" << endl;
#endif

				if (compareresult) {
					PPB(d1, dres, type_a1, type_a2, type_res);

				}
#if debug
				cout << "Executor::executeCurrentCode POPBACK out" << endl;
#endif
				break;
			case S_POPFRONT:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "POPFRONT");
#if debug
				cout << "Executor::executeCurrentCode POPFRONT in" << endl;
#endif

				if (compareresult) {
					PPF(d1, dres, type_a1, type_a2, type_res);

				}
#if debug
				cout << "Executor::executeCurrentCode POPFRONT out" << endl;
#endif
				break;
			case S_BACK:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "BACK");
#if debug
				cout << "Executor::executeCurrentCode BACK in" << endl;
#endif

				if (compareresult) {
					BCK(d1, dres, type_a1, type_a2, type_res);

				}
#if debug
				cout << "Executor::executeCurrentCode BACK out" << endl;
#endif
				break;
			case SI_S:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "++");
#if debug
				cout << "Executor::executeCurrentCode SI_S in" << endl;
#endif
				if (compareresult) {

					if (d2->dataFlagBit == Sz) {
						Intg orig = d2->content_i + 1;
						dres->clear();
						dres->dataFlagBit = Sz;
						dres->content_i = orig;
						d2->content_i = orig;

					}
					else {

						{
							static int ignorecount = 0;
							std::cout << "Executor::executeCurrentCode()"
								<< " undefined ++sop1, exit(-1) "
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
						assert(false); exit(-1);
					}
				}
#if debug
				cout << "Executor::executeCurrentCode SI_S out" << endl;
#endif
				break;
			case SD_S:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "--");
#if debug
				cout << "Executor::executeCurrentCode SD_S in" << endl;
#endif
				if (compareresult) {

					if (d2->dataFlagBit == Sz) {
						Intg orig = d2->content_i - 1;
						dres->clear();
						dres->dataFlagBit = Sz;
						dres->content_i = orig;
						d2->content_i = orig;

					}
					else {

						{
							static int ignorecount = 0;
							std::cerr << "Executor::executeCurrentCode()"
								<< " undefined --op1, exit(-1) "
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
						assert(false); exit(-1);
					}
				}
#if debug
				cout << "Executor::executeCurrentCode SD_S out" << endl;
#endif
				break;
			case S_S_EXIST_SUBEXPR:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "EXIST_SUBEXPR");
#if debug
				cout << "Executor::executeCurrentCode EXIST_SUBEXPR in" << endl;
#endif

				if (compareresult) {
					if (d1->isExpression == false) {
						DataPtr d1_tmp = DataPtr::make();
						d1_tmp->isExpression = true;
						d1_tmp->expr = ExpressionHandlerPtr::make(argexe1);
						EXIST_SUBEXPR(d1_tmp, d2, dres, type_a1, type_a2, type_res);

					}
					else {
						EXIST_SUBEXPR(d1, d2, dres, type_a1, type_a2, type_res);
					}
				}
#if debug
				cout << "Executor::executeCurrentCode EXIST_SUBEXPR out" << endl;
#endif
				break;
			case S_S_FIND_SUBEXPR:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "FIND_SUBEXPR");
#if debug
				cout << "Executor::executeCurrentCode FIND_SUBEXPR in" << endl;
#endif

				if (compareresult) {
					if (d1->isExpression == false) {
						DataPtr d1_tmp = DataPtr::make();
						d1_tmp->isExpression = true;
						d1_tmp->expr = ExpressionHandlerPtr::make(argexe1);
						FIND_SUBEXPR(d1_tmp, d2, dres, type_a1, type_a2, type_res);


					}
					else {
						FIND_SUBEXPR(d1, d2, dres, type_a1, type_a2, type_res);

					}
				}
#if debug
				cout << "Executor::executeCurrentCode FIND_SUBEXPR out" << endl;
#endif
				break;
			case S_RESET:
				compareresult = (operatorexe.argFlag == M_Bs
					&& operatorexe.arg_s == "RESET");
#if debug
				cout << "Executor::executeCurrentCode RESET in" << endl;
#endif

				if (compareresult) {

					RESET(argexe1, d1);
				}
#if debug
				cout << "Executor::executeCurrentCode RESET out" << endl;
#endif
				break;






			default:
				break;
			}
			if (isImmediateData_a1) {
#if debug
				{
					static int ignorecount = 0;
					cout << "Executor::executeCurrentCode()" << " "
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
				}
#endif
				safe_delete(a1_i);
#if debug
				{
					static int ignorecount = 0;
					cout << "Executor::executeCurrentCode()" << " "
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
				}
#endif
				safe_delete(a1_s);
#if debug
				{
					static int ignorecount = 0;
					cout << "Executor::executeCurrentCode()" << " "
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
				}
#endif
				//safe_delete(d1);
				d1 = nullptr;
#if debug
				{
					static int ignorecount = 0;
					cout << "Executor::executeCurrentCode()" << " "
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
				}
#endif
			}
			if (isImmediateData_a2) {
				safe_delete(a2_i);
				safe_delete(a2_s);
				//safe_delete(d2);
				d2 = nullptr;
			}
			if (isImmediateData_res) {
				safe_delete(res_i);
				safe_delete(res_s);
				//safe_delete(dres);
				dres = nullptr;
			}
			if (compareresult == true) {
#if debug
				cout << "Executor::executeCurrentCode 6 out" << endl;
#endif
				return true;
			}
			else {

				cerr << "Executor::executeCurrentCode tfaddr<addrnull err,exit(-1)"
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
				assert(false); exit(-1);
				return false;
			}

		}
		else if (currentCode->assemblyFormula.flagBit.tfFKA.addr > addrnull) {
			//todo:: 从此处开始处理非内置模板函数调用的根节点
#if debug
			{
				static int ignorecount = 0;
				cout << "Executor::executeCurrentCode()" << " " << "\tignorecount:["

					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__ << ":0"
					<< ")" << endl;
			}
#endif
#if debug
			cout << "Executor::executeCurrentCode 7 , tfaddr>addrnull" << endl;
#endif
			map<ArgFormal, ArgReal>&& frmap = getFRMap();

			FKA& functionNameScopeFKA = currentCode->assemblyFormula.flagBit.tfFKA;
			FKA& functionBodyScopeFKA =
				(*tfth)[functionNameScopeFKA].bodyScopeFKA;
			TemplateFunction& tf =
				(*tfth)[functionNameScopeFKA];

			auto& bfMeetFKAdq = currentAR->bfMeetFKAdq;
			auto& backwardExecute = currentAR->backwardExecute;
			FKA meetFKA = nextCodeFKA;
			//是否跳过当前function，（正向执行过程中遇到非末尾逆向函数时要跳过这个函数）
			bool skipfunction = false;

			//执行遇到末尾
			if (tf.isBackwardFunction == F_
				&& (*cdth)[nextCodeFKA].assemblyFormula.flagBit.type
				!= COOL_M
				&& (*cdth)[nextCodeFKA].assemblyFormula.flagBit.type
				!= COOL_FIL
				&& (*cdth)[nextCodeFKA].assemblyFormula.flagBit.type
				!= COOL_LIN && bfMeetFKAdq.size() > 0) {
				bfMeetFKAdq.push_front(nextCodeFKA);
				meetFKA = bfMeetFKAdq.back();
				bfMeetFKAdq.pop_back();
				if (meetFKA != nextCodeFKA) {
					backwardExecute = T_;
				}
				else {
					backwardExecute = F_;
				}
			}
			else if (tf.isBackwardFunction == T_
				&& (*cdth)[nextCodeFKA].assemblyFormula.flagBit.type
				!= COOL_M && bfMeetFKAdq.size() > 0) {
				bfMeetFKAdq.push_front(nextCodeFKA);
				meetFKA = bfMeetFKAdq.back();
				bfMeetFKAdq.pop_back();
				backwardExecute = T_;
			}
			//正向执行，未到末尾
			else if (tf.isBackwardFunction == F_
				&& (*cdth)[nextCodeFKA].assemblyFormula.flagBit.type
				== COOL_M) {
				meetFKA = nextCodeFKA;
				backwardExecute = F_;

			}
			else if (tf.isBackwardFunction == T_
				&& (*cdth)[nextCodeFKA].assemblyFormula.flagBit.type
				== COOL_M && backwardExecute == F_) {
				meetFKA = nextCodeFKA;
				bfMeetFKAdq.push_back(currentCodeFKA);
				//这里注意初始化逆向函数的返回值，因为逆推时其要作为上一步的实参出现。
				Arg&& argreturn = (*currentCode)[3];
				if (currentAR->findcurrent(argreturn) == false) {
					currentAR->add(argreturn);
				}
				//正向执行遇到非末尾逆向函数要跳过
				skipfunction = true;

			}
			//逆向执行
			else if (tf.isBackwardFunction == T_ && backwardExecute == T_) {
				meetFKA = bfMeetFKAdq.back();
				bfMeetFKAdq.pop_back();
				if (bfMeetFKAdq.size() > 0) {
					backwardExecute = T_;
				}
				else {
					//如果bfmeetAddrdq为空，说明此次函数执行完毕后跳转到了一非COOL_M类型code中，此时需要正向执行
					backwardExecute = F_;
				}

			}

			if (skipfunction) {
				return true;
			}
			//初始化函数调用生成的AR
			DataTablePtr functionBodyParentAR = getParentAR(functionBodyScopeFKA);
			DataTablePtr functionBodyAR = nullptr;
			functionBodyAR = DataTablePtr::make();
			functionBodyAR->parentAR = functionBodyParentAR;
			functionBodyAR->returnAR = currentAR;
			functionBodyAR->meetFKA = meetFKA;
			functionBodyAR->queryARList.push_back(currentAR);
			functionBodyAR->queryARList.push_back(functionBodyParentAR);
			functionBodyAR->scopeStructureFKA = functionBodyScopeFKA;
			functionBodyAR->setFRMap(frmap);

			nextCodeFKI =
				cdth->getFKIByFKA(
					(*tfth)[functionNameScopeFKA].bodyStartFKA);

			//将函数调用的AR添加到当前AR，转移至当前AR运行
			Arg arg_(Z_Dz, functionBodyScopeFKA);
			arg_.formalArg = true;
			currentAR->add(arg_);
			(*currentAR)[arg_].dataFlagBit = S_AR;
			(*currentAR)[arg_].content_ar = functionBodyAR;
			currentAR = functionBodyAR;
			nextCodeFKI.SI(cdth);
			setNextCodeFKI();
			nextCodeFKA = cdth->getFKAByFKI(nextCodeFKI);

		}
		else {

			cerr << "Executor::executeCurrentCode err, exit(-1)" << endl;

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
			assert(false); exit(-1);
		}

		return true;

	}
	/**
	 * @brief 解析指令字符串，如果没任何有效指令，则返回false，否则返回true
	 * @param orderStrg
	 * @param breakPointMap 断点表，
	 * @param clearMap 解析时是否清空breakPointMap
	 * @param STP
	 * @param SPD
	 * @param BEG
	 * @return
	 */
	bool Executor::parseOrderStrg(const Strg& orderStrg,
		map<Strg, set<Intg>>& breakPointMap, bool clearMap, bool& STP,
		bool& BEG, bool& SPD, bool& TRM) {
		if (clearMap) {
			breakPointMap.clear();
		}
		auto&& orderv = split(orderStrg, ";");
		Strg FILstrg;
		Intg validOrderNum = 0;
		for (auto& order_ : orderv) {
			++validOrderNum;
			auto&& option_value_v = split(order_, ":");

			if (option_value_v[0] == "COOL_FIL") {
				FILstrg = option_value_v[1];
			}
			else if (option_value_v[0] == "COOL_LIN") {
				Intg linenum = toIntg(option_value_v[1]);
				if (FILstrg != "") {
					if (breakPointMap.count(FILstrg) <= 0) {
						set<Intg> set_ = { linenum };
						breakPointMap[FILstrg] = set_;
					}
					else {
						breakPointMap[FILstrg].insert(linenum);
					}
				}
			}
			else if (option_value_v[0] == "STP") {
				STP = toIntg(option_value_v[1]);
			}
			else if (option_value_v[0] == "BEG") {
				BEG = toIntg(option_value_v[1]);
			}
			else if (option_value_v[0] == "SPD") {
				SPD = toIntg(option_value_v[1]);
			}
			else if (option_value_v[0] == "TRM") {
				TRM = toIntg(option_value_v[1]);
			}
			else {
				--validOrderNum;
			}

		}
		return validOrderNum > 0;

	}
	void Executor::updateTrack(bool inStepGround) {
		fkatrack.push_back(currentCodeFKA);
		ground_exe_track.push_back(inStepGround ? "grd" : "exe");
		codetrack.push_back(*currentCode);
		stringstream ss;
		auto itar = currentAR;
		auto i = 0;
		while (itar != nullptr) {
			if (i++ > 0) {
				ss << "<";
			}
			ss << itar->scopeStructureFKA.toStrg();
			itar = itar->parentAR;
		}
		artrack.push_back(ss.str());
	}
	void Executor::clearTrack() {
		this->codetrack.clear();
		this->fkatrack.clear();
		this->ground_exe_track.clear();
		this->artrack.clear();

	}
	void Executor::setMode(Intg mode) {
		this->executorAR = DataTablePtr::make(nullptr, nullptr, FKA(FILEKEYPRIMARY, Addr()));
		this->currentAR = executorAR;
		clearTrack();
		this->mode = mode;
	}
	/// <summary>
	/// set the asc of S_Bs. this function is not allowed to add any variable to currentAR. This function is valid for each line of code once.
	/// </summary>
	void Executor::setArgScopeFKA() {
		/*if ((*currentCode).assemblyFormula.flagBit.asc_locked == false) {
			(*currentCode).assemblyFormula.flagBit.asc_locked = true;
		}
		else {
			return;

		}*/
		if ((*currentCode).assemblyFormula.flagBit.type == COOL_C
			&& (*currentCode)[0].argFlag == S_Bs
			&& (*currentCode)[1].argFlag == X_Bs) {
			(*currentCode).assemblyFormula.flagBit.a1s = (*currentCode).scopeFKA;
			(*currentCode).assemblyFormula.flagBit.res = (*currentCode).scopeFKA;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M
			&& (*currentCode).assemblyFormula.flagBit.execute == F_) {

#if debug
			cout << "Integrator::setArgScopeFKA a1s" << endl;
#endif
			if ((*currentCode)[0].argFlag == S_Bs) {
				//currentAR->add((*currentCode)[0]);
				try {
					(*currentCode).assemblyFormula.flagBit.a1s =
						currentAR->getArgScopeFKA((*currentCode)[0]);
				}
				catch (Arg& var_not_exist) {
					(*currentCode).assemblyFormula.flagBit.a1s =
						(*currentCode).scopeFKA;
				}
			}

#if debug
			cout << "Integrator::setArgScopeFKA a2s" << endl;
#endif
			if ((*currentCode)[1].argFlag == S_Bs) {
				//currentAR->add((*currentCode)[1]);
				try {
					(*currentCode).assemblyFormula.flagBit.a2s =
						currentAR->getArgScopeFKA((*currentCode)[1]);
				}
				catch (Arg& var_not_exist) {
					(*currentCode).assemblyFormula.flagBit.a2s =
						(*currentCode).scopeFKA;
				}
			}

#if debug
			cout << "Integrator::setArgScopeFKA ops" << endl;
#endif
			if ((*currentCode)[2].argFlag == S_Bs) {
				//currentAR->add((*currentCode)[2]);
				try {
					(*currentCode).assemblyFormula.flagBit.ops =
						currentAR->getArgScopeFKA((*currentCode)[2]);
				}
				catch (Arg& var_not_exist) {
					(*currentCode).assemblyFormula.flagBit.ops =
						(*currentCode).scopeFKA;
				}
			}
#if debug
			cout << "Integrator::setArgScopeFKA res" << endl;
#endif
			if ((*currentCode)[3].argFlag == S_Bs) {
				//currentAR->add((*currentCode)[3]);
				try {
					(*currentCode).assemblyFormula.flagBit.res =
						currentAR->getArgScopeFKA((*currentCode)[3]);
				}
				catch (Arg& var_not_exist) {
					(*currentCode).assemblyFormula.flagBit.res =
						(*currentCode).scopeFKA;
				}
			}
#if debug
			cout << "Integrator::setArgScopeFKA end" << endl;
#endif

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_R
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
			if ((*currentCode).assemblyFormula.flagBit.asc_locked == false) {
				(*currentCode).assemblyFormula.flagBit.asc_locked = true;
			}
			else {
				return;

			}
			try {
				(*currentCode).assemblyFormula.flagBit.a1s = currentAR->getArgScopeFKA(
					(*currentCode)[0]);
			}
			catch (Arg& arg_) {

				auto tffka = tfth->getTFFKA((*currentCode).scopeFKA, scpth);
				auto arg_rt = (*currentCode)[0];
				arg_rt.asc = tffka;
				if (tffka != fkanull && currentAR->findall(arg_rt)) {
					(*currentCode).assemblyFormula.flagBit.a1s = tffka;
				}
				else
				{
					throw arg_;
				}

			}

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
			if ((*currentCode).assemblyFormula.flagBit.asc_locked == false) {
				(*currentCode).assemblyFormula.flagBit.asc_locked = true;
			}
			else {
				return;

			}
			/*if (((*currentCode).assemblyFormula.flagBit.result_flag == S_Dz)
				&& (*currentCode).assemblyFormula.flagBit.formalArgFlag[3]
				== true) {
				(*currentAR).add((*currentCode)[3]);
			}*/
#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::setArgScopeFKA()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			if ((*currentCode)[0].argFlag == S_Bs) {
				try {
					(*currentCode).assemblyFormula.flagBit.a1s =
						currentAR->getArgScopeFKA((*currentCode)[0]);
				}
				catch (Arg& var_not_exist) {
					(*currentCode).assemblyFormula.flagBit.a1s =
						(*currentCode).scopeFKA;
				}
			}

#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::setArgScopeFKA()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			if ((*currentCode)[1].argFlag == S_Bs) {
				try {
					(*currentCode).assemblyFormula.flagBit.a2s =
						currentAR->getArgScopeFKA((*currentCode)[1]);
				}
				catch (Arg& var_not_exist) {
					(*currentCode).assemblyFormula.flagBit.a2s =
						(*currentCode).scopeFKA;
				}
			}

#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::setArgScopeFKA()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			if ((*currentCode)[2].argFlag == S_Bs) {
				try {
					(*currentCode).assemblyFormula.flagBit.ops =
						currentAR->getArgScopeFKA((*currentCode)[2]);
				}
				catch (Arg& var_not_exist) {
					(*currentCode).assemblyFormula.flagBit.ops =
						(*currentCode).scopeFKA;
				}
			}
#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::setArgScopeFKA()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			if ((*currentCode)[3].argFlag == S_Bs) {
				try {
					(*currentCode).assemblyFormula.flagBit.res =
						currentAR->getArgScopeFKA((*currentCode)[3]);
				}
				catch (Arg& var_not_exist) {
					(*currentCode).assemblyFormula.flagBit.res =
						(*currentCode).scopeFKA;
				}
			}
#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::setArgScopeFKA()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif

				}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_LST
			&& (*currentCode).assemblyFormula.flagBit.execute != Du) {
			/*if ((*currentCode).assemblyFormula.flagBit.operator_flag == M_Bs
				&& (*currentCode).assemblyFormula.quaternion.operator_s
				== "{null}") {
				(*currentAR).add((*currentCode)[3]);
				Data& dt = (*currentAR)[(*currentCode)[3]];
				dt = Data(COOL_LST, nullptr);
				dt.content_lst->clear();
			}
#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::setArgScopeFKA()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif*/
			if ((*currentCode)[0].argFlag == S_Bs) {
				try {
					(*currentCode).assemblyFormula.flagBit.a1s =
						currentAR->getArgScopeFKA((*currentCode)[0]);
				}
				catch (Arg& var_not_exist) {
					(*currentCode).assemblyFormula.flagBit.a1s =
						(*currentCode).scopeFKA;
				}
			}

#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::setArgScopeFKA()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			if ((*currentCode)[1].argFlag == S_Bs) {
				try {
					(*currentCode).assemblyFormula.flagBit.a2s =
						currentAR->getArgScopeFKA((*currentCode)[1]);
				}
				catch (Arg& var_not_exist) {
					(*currentCode).assemblyFormula.flagBit.a2s =
						(*currentCode).scopeFKA;
				}
			}

#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::setArgScopeFKA()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			if ((*currentCode)[2].argFlag == S_Bs) {
				try {
					(*currentCode).assemblyFormula.flagBit.ops =
						currentAR->getArgScopeFKA((*currentCode)[2]);
				}
				catch (Arg& var_not_exist) {
					(*currentCode).assemblyFormula.flagBit.ops =
						(*currentCode).scopeFKA;
				}
			}
#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::setArgScopeFKA()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			if ((*currentCode)[3].argFlag == S_Bs) {
				try {
					(*currentCode).assemblyFormula.flagBit.res =
						currentAR->getArgScopeFKA((*currentCode)[3]);
				}
				catch (Arg& var_not_exist) {
					(*currentCode).assemblyFormula.flagBit.res =
						(*currentCode).scopeFKA;
				}
			}
#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::setArgScopeFKA()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif

				}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QS
			&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
			(*currentCode).assemblyFormula.flagBit.res =
				(*currentCode).assemblyFormula.quaternion.result_fka;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
			(*currentCode).assemblyFormula.flagBit.res =
				(*currentCode).assemblyFormula.quaternion.result_fka;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QS
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R) {
			(*currentCode).assemblyFormula.flagBit.res =
				(*currentCode).assemblyFormula.quaternion.result_fka;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R) {
			(*currentCode).assemblyFormula.flagBit.res =
				(*currentCode).assemblyFormula.quaternion.result_fka;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == EXPRQS) {
			(*currentCode).assemblyFormula.flagBit.res =
				(*currentCode).assemblyFormula.quaternion.result_fka;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == EXPRQE) {
			(*currentCode).assemblyFormula.flagBit.res =
				(*currentCode).assemblyFormula.quaternion.result_fka;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_FUN) {
			(*currentCode).assemblyFormula.flagBit.a1s =
				(*currentCode).assemblyFormula.quaternion.arg1_fka;
			(*currentCode).assemblyFormula.flagBit.a2s =
				(*currentCode).assemblyFormula.quaternion.arg2_fka;
			(*currentCode).assemblyFormula.flagBit.res =
				(*currentCode).assemblyFormula.quaternion.result_fka;
		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_TD) {
			(*currentCode).assemblyFormula.flagBit.a1s =
				(*currentCode).assemblyFormula.quaternion.arg1_fka;
			(*currentCode).assemblyFormula.flagBit.a2s =
				(*currentCode).assemblyFormula.quaternion.arg2_fka;
			(*currentCode).assemblyFormula.flagBit.res =
				(*currentCode).assemblyFormula.quaternion.result_fka;
		}

		return;
			}


	bool Executor::stepGround() {
		/*if (currentCodeFKA.addr.addrv == vector{ 812, 817 }) {
			debugflag = true;
		}*/
		if (currentCodeFKA.addr.addrv[0] >= debug_min_addr) {
			debugflag = true;

		}
		//#if debug
		{
			updateTrack(debugflag);
		}
		//#endif 
		//#if debug && _WIN32
		//		{
		//
		//			stringstream ss;
		//
		//			ss << "\n||||||||||||||||||||||||||||||stepGround||||||||||||||||||||||||||||\n";
		//			ss << this->toBrief();
		//			ss << bhline;
		//
		//			ss << "currentCode:(" << currentCodeFKA.toStrg() << ")" << endl;
		//			ss << currentCode->toBrief() << endl;
		//			ss << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
		//
		//			static int ignoreCount = 0;
		//			// Assuming info is a std::string, convert to std::wstring
		//			std::string info = ss.str(); // Replace with actual info
		//			info += std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
		//			std::wstring winfo(info.begin(), info.end());
		//#if WIN_DEBUG_OUTPUT
		//			OutputDebugString(winfo.c_str());
		//#endif
		//			cout << info << endl;
		//			ignoreCount++;
		//		}
		//#endif


#if (debug || filelog) && _WIN32 
		if (debugflag) {
			static int ignoreCount = 0;
			{
				stringstream ss;

				ss << "\n||||||||||||||||||||||||||||||stepGround||||||||||||||||||||||||||||\n";
				ss << this->toBrief();
				ss << bhline;

				ss << "currentCode:(" << currentCodeFKA.toStrg() << ")" << endl;
				ss << currentCode->toBrief() << endl;
				ss << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";


				// Assuming info is a std::string, convert to std::wstring
				std::string info = ss.str(); // Replace with actual info
				info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");

#if filelog
				cout << info << std::endl;
#endif
#if debug
				std::cout << info << std::endl;
#endif
#if WIN_DEBUG_OUTPUT
				std::wstring winfo(info.begin(), info.end());
				OutputDebugString(winfo.c_str());
#endif
			}
			ignoreCount++;
		}
#endif




		argexe1 = (*currentCode)[0];
		argexe2 = (*currentCode)[1];
		operatorexe = (*currentCode)[2];
		resultexe = (*currentCode)[3];

		if ((*currentCode).assemblyFormula.flagBit.type == COOL_C) {
			addArgsToCurrentAR_ground();
			setArgScopeFKA();

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M
			&& (*currentCode).assemblyFormula.flagBit.execute == F_) {

			addArgsToCurrentAR_ground();
			setArgScopeFKA();

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M && (*currentCode).assemblyFormula.flagBit.tfFKA == fkanull
			&& (*currentCode).assemblyFormula.flagBit.execute == T_ && isExecuting == false) {
			addArgsToCurrentAR_ground();
			setArgScopeFKA();

			if (nextCode == nullptr
				|| (*nextCode).assemblyFormula.flagBit.type != COOL_M) {
				//如果表达式是EXPRB，则不整合
				if (currentCode->scopeFKA != fkanull) {
					Scope& currentScope =
						(*scpth)[currentCode->scopeFKA];
					if ((currentScope.functionFlag == EXPRB)) {

						return true;

					}
				}
				//如果当前函数是rule function，且此代码是返回的表达式的一部分时，也不整合
				if ((*nextCode).assemblyFormula.flagBit.type == COOL_R) {
					if (currentCode->scopeFKA != fkanull) {
						FKA&& funcfka = tfth->getTFFKA(currentCode->scopeFKA, scpth);
						if (funcfka == fkanull) {
							throw exception("RETURN doesn't exist in function");
						}
						else {
							auto& func = (*tfth)[funcfka];

							if (func.integrate == true /*rule function*/) {
								return true;
							}
						}
					}
					else {
						throw exception("RETURN doesn't exist in scope");

					}

				}
				//否则进行整合
//                CodeTable cdtr = codeTable->copyTree(currentCodeAddr);


				//reserve last FKA to help find the currentFKI when when integrate finish.
				this->isExecuting = true;
				FKA rollbackFKA = this->cdth->getLastNot(COOL_M, currentCodeFKA);
				FKI rollbackFKI = this->cdth->getFKIByFKA(rollbackFKA);


				//all kinds of codes should be grounded
			/*	MultiTFIntegrator mtfi(cdth, currentCodeFKA,
					tfth, scpth,
					systh, currentAR, setting,
					pipe_writing_handler, pipe_reading_handler);*/
				MultiTFIntegrator mtfi(this);
				mtfi.setMaxCapacity(this->setting->matchStateSiloMaxCapacity,
					this->setting->groundingStateSiloMaxCapacity, this->setting->maxSwitchTurn);
#if debug
				cout << "Integrator::integrate 2s" << endl;
#endif
				try
				{
					if (mtfi.integrate() == false) {


#if debug && _WIN32
						{
							static int ignoreCount = 0;
							// Assuming info is a std::string, convert to std::wstring
							std::string info = "integrate failed, exit(-1)"; // Replace with actual info
							info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
							std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
							OutputDebugString(winfo.c_str());
#endif
							cout << info << endl;
							ignoreCount++;
			}
#endif

						cerr << "integrate failed, exit(-1)" << endl;
						assert(false); exit(-1);
					}
					else {

#if debug && _WIN32
						{
							static int ignoreCount = 0;
							// Assuming info is a std::string, convert to std::wstring
							std::string info = "integrate success!!! fka=[" + currentCodeFKA.toStrg() + "]"; // Replace with actual info
							info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
							std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
							OutputDebugString(winfo.c_str());
#endif
							cout << info << endl;
							ignoreCount++;
		}
#endif

						this->isExecuting = false;
						nextCodeFKI = rollbackFKI.SI(this->cdth);

	}
				}
				catch (const ReasoningException& e)
				{
					MultiTFIntegrator::reasoning_failure_times++;
					cout << "Reasoning Failure! Total abort tasks:" << MultiTFIntegrator::reasoning_failure_times << endl;
					this->isExecuting = false;
				}
#if debug
#endif
				}

			}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M && (*currentCode).assemblyFormula.flagBit.execute != F_ && (*currentCode).assemblyFormula.flagBit.tfFKA != fkanull) {
			// TODO: whether or not to invoke fact functions including loops during grounding? it has risks, but may prompt the reasoning preciosity. left for future improvements.
			stepExecute();
		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_Y
			&& (*currentCode).assemblyFormula.flagBit.execute != F_) {
			addArgsToCurrentAR_ground();
			setArgScopeFKA();
		}

		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M

			&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
			addArgsToCurrentAR_ground();
			setArgScopeFKA();

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_Y
			&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
			addArgsToCurrentAR_ground();
			setArgScopeFKA();

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QS
			&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
			addArgsToCurrentAR_ground();
			setArgScopeFKA();

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
			addArgsToCurrentAR_ground();
			setArgScopeFKA();

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QS
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R) {

			addArgsToCurrentAR_ground();
			setArgScopeFKA();

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R) {
			addArgsToCurrentAR_ground();
			setArgScopeFKA();

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_FUN) {
			addArgsToCurrentAR_ground();
			setArgScopeFKA();
		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_TD) {
			//在这里进行逆函数的推导
			addArgsToCurrentAR_ground();
			setArgScopeFKA();
		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_R) {
			addArgsToCurrentAR_ground();
			setArgScopeFKA();
		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_ABT) {
			if (isExecuting == false) {

				//nothing need to be done
			}
			else {
				if (currentAR->findall(Arg(S_Bs, "thisexpr"))) {
					throw (*currentAR)[Arg(S_Bs, "thisexpr")].expr;
				}
				throw ExpressionHandlerPtr::make();
			}
		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_N) {
			//什么都不用干
		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_LST) {
			addArgsToCurrentAR_ground();
			setArgScopeFKA();
		}
		else if (debugMode == true
			&& (*currentCode).assemblyFormula.flagBit.type == COOL_FIL) {
			currentFIL = currentCode->operator [](0).arg_s;
			cout << "location(" << currentFIL << ":" << currentLIN << ")"
				<< endl;

		}
		else if (debugMode == true
			&& (*currentCode).assemblyFormula.flagBit.type == COOL_LIN) {
			currentLIN = currentCode->operator [](0).arg_i;
			cout << "location(" << currentFIL << ":" << currentLIN << ")"
				<< endl;

		}

		else {
			addArgsToCurrentAR_ground();
			setArgScopeFKA();
		}

		update_AR_stack();
				}

	bool Executor::ground() {

		this->currentAR = this->executorAR;
		globalRefTable.clear();
		globalRefTable.mode = MODE_GROUNDING;
		nextCodeFKA = cdth->getMinFKA();
		nextCodeFKI = cdth->getMinFKI();
		if (debugMode) {
			cout << "=====================推理过程=====================" << endl;
		}
		while (getNewCode() == true) {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				stringstream ss;
				ss << bbhline;
				ss << "Executor::cdth" << endl;
				ss << this->cdth->toStrg();
				ss << bhline;
				ss << "Executor::ground::currentCode:" << endl;
				ss << calign(currentCodeFKA.toStrg(), 24) << calign(currentCode->toStrg(), 72) << endl;
				ss << bbhline;
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




			stepGround();


			}
		return true;

				}

	bool Executor::execute() {

		this->currentAR = this->executorAR;

		if (debugMode) {
			cout << "=====================执行过程=====================" << endl;
		}
		globalRefTable.clear();
		globalRefTable.mode = MODE_EXECUTION;


		if (setting->debugMode == true) { //debug模式未开发完整，此处跳过
			//(*fsInfo).open(setting->debugInfoOutputPath, (*fsInfo).out);
			//(*fsOrder).open(setting->debugOrderInputPath, (*fsInfo).in);
			//debug io currently replaced by cin & cout
		/*	(*fsOrder) = cin;
			(*fsInfo) = cout;*/
			/*while (true) {
				(*fsOrder) >> order;
				if ((*fsOrder).eof() && order.size() > 0) {
					if (parseOrderStrg(order, breakPointMap, false, STP, BEG, SPD,
						TRM) == true && BEG == true) {
						order.clear();
						break;
					}
					(*fsOrder).clear();
				}
				else {

				}
			}
			order.clear();*/
		}
		nextCodeFKA = this->cdth->getMinFKA();
		nextCodeFKI = this->cdth->getMinFKI();

		while (getNewCode() == true) {




			stepExecute();


		}

		return true;
	}

	bool Executor::stepExecute() {
		if (currentCodeFKA.addr.addrv[0] >= debug_min_addr) {
			debugflag = true;

		}
		//#if debug
		updateTrack(false);
		//#endif

#if (debug || filelog) && _WIN32 
		if (debugflag) {
			static int ignoreCount = 0;
			{
				stringstream ss;
				ss << "\n||||||||||||||||||||||||||||||stepExecute||||||||||||||||||||||||||||\n";
				ss << this->toBrief();
				ss << bhline;

				ss << "currentCode:(" << currentCodeFKA.toStrg() << ")" << endl;
				ss << currentCode->toBrief() << endl;
				ss << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
				// Assuming info is a std::string, convert to std::wstring
				std::string info = ss.str(); // Replace with actual info
				info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");

#if filelog
				cout << info << std::endl;
#endif
#if debug
				std::cout << info << std::endl;
#endif
#if WIN_DEBUG_OUTPUT
				std::wstring winfo(info.begin(), info.end());
				OutputDebugString(winfo.c_str());
#endif
			}
			ignoreCount++;
		}
#endif

		//#if debug && _WIN32
		//		{
		//
		//			stringstream ss;
		//
		//			ss << "|||||||||||||||||||||||||||||stepExecute||||||||||||||||||||||||||||\n";
		//			ss << this->toBrief();
		//			ss << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
		//
		//			static int ignoreCount = 0;
		//			// Assuming info is a std::string, convert to std::wstring
		//			std::string info = ss.str(); // Replace with actual info
		//			info += std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
		//			std::wstring winfo(info.begin(), info.end());
		//#if WIN_DEBUG_OUTPUT
		//			OutputDebugString(winfo.c_str());
		//#endif
		//			cout << info << endl;
		//			ignoreCount++;
		//		}
		//#endif

#if debug
		{
			static int ignorecount = 0;
			cout << "Executor::execute()" << " currentCodeFKA:["

				<< currentCodeFKA.toStrg() << "]" << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
	}
#endif

		/*if (setting->debugMode) {
			while (true) {
				(*fsOrder) >> order;
				if ((*fsOrder).eof()) {
					if (order.size() > 0) {

						if (parseOrderStrg(order, breakPointMap, true, STP, BEG,
							SPD, TRM) == true) {
							if (TRM) {
								TRM = false;
								return true;
							}
							else if (SPD) {
								SPD = false;
								BEG = false;
							}
							else if (STP && BEG) {
								BEG = false;
								break;
							}

						}
						else {
#if debug
							{
								static int ignorecount = 0;
								cout << "Executor::execute()"
									<< " receive unknown debug order:["
									<< order << "]" << "\tignorecount:["
									<< ignorecount++ << "\t](" << __FILE__
									<< ":" << __LINE__ << ":0" << ")"
									<< endl;
							}
#endif
							BEG = false;
						}
					}
					else {
						if (STP && BEG) {
							BEG = false;
							break;
						}
						break;
					}
					if (BEG == true) {
						break;
					}
					(*fsOrder).clear();
				}
			}
			order.clear();
		}*/


		argexe1 = (*currentCode)[0];
		argexe2 = (*currentCode)[1];
		operatorexe = (*currentCode)[2];
		resultexe = (*currentCode)[3];

		if ((*currentCode).assemblyFormula.flagBit.type == COOL_C) {
#if debug
			cout << "Executor::execute COOL_C in,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
			addArgsToCurrentAR_execute();

#if debug
			cout << "Executor::execute COOL_C out,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif

			}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_R) {
#if debug
			cout << "Executor::execute COOL_R in,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
			addArgsToCurrentAR_execute();
			executeCurrentCode();
#if debug
			cout << "Executor::execute COOL_R out,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_ABT) {
#if debug
			cout << "Executor::execute COOL_ABT in,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "execution abort"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
		}
#endif

			if (isExecuting == false) {

				//nothing need to be done
			}
			else {
				if (currentAR->findall(Arg(S_Bs, "thisexpr"))) {
					throw (*currentAR)[Arg(S_Bs, "thisexpr")].expr;
			}
				throw ExpressionHandlerPtr::make();
			}
#if debug
			cout << "Executor::execute COOL_ABT out,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
			}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M) {
#if debug
			cout << "Executor::execute COOL_M in,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
			addArgsToCurrentAR_execute();
			executeCurrentCode();
#if debug
			cout << "Executor::execute COOL_M out,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
				}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_LST) {
#if debug
			cout << "Executor::execute COOL_LST in,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
			addArgsToCurrentAR_execute();
			executeCurrentCode();
#if debug
			cout << "Executor::execute COOL_LST out,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
			}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_Y
			&& (*currentCode).assemblyFormula.flagBit.execute != Du) {
			addArgsToCurrentAR_execute();
			executeCurrentCode();

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QS
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R
			&& currentAR->type == SYSB) {
			//专门用于处理类继承问题
			addArgsToCurrentAR_execute();
				}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QS
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
#if debug
			cout << "Executor::execute COOL_QS in,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
				addArgsToCurrentAR_execute();
				executeCurrentCode();
#if debug
				cout << "Executor::execute COOL_QS out,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
			}

		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
#if debug
			cout << "Executor::execute COOL_QE T_ in,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
				addArgsToCurrentAR_execute();
				executeCurrentCode();
#if debug
				cout << "Executor::execute COOL_QE T_ out,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
				}
		else if ((*currentCode).assemblyFormula.flagBit.type == EXPRQS
			&& (*currentCode).assemblyFormula.flagBit.execute != Du) {

			addArgsToCurrentAR_execute();
			executeCurrentCode();

		}

		else if ((*currentCode).assemblyFormula.flagBit.type == EXPRQE
			&& (*currentCode).assemblyFormula.flagBit.execute != Du) {

			addArgsToCurrentAR_execute();
			executeCurrentCode();

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R) {
#if debug
			cout << "Executor::execute COOL_QE COOL_R in,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
				addArgsToCurrentAR_execute();
				executeCurrentCode();
#if debug
				cout << "Executor::execute COOL_QE COOL_R out,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
			}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_B
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
#if debug
			cout << "Executor::execute COOL_B T_ in,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
				addArgsToCurrentAR_execute();
				executeCurrentCode();
#if debug
				cout << "Executor::execute COOL_B T_ out,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
			}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_L
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
#if debug
			cout << "Executor::execute COOL_L T_ in,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
				addArgsToCurrentAR_execute();
				executeCurrentCode();
#if debug
				cout << "Executor::execute COOL_L T_ out,currentCode:["
				<< currentCode->toStrg() << "]" << endl;
#endif
		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_BRK
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {

			addArgsToCurrentAR_execute();
			executeCurrentCode();

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_CTN
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {

			addArgsToCurrentAR_execute();
			executeCurrentCode();

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_TD
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
			//needn't do anything
		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_FIL
			&& setting->debugMode == true) {
			currentFIL = currentCode->assemblyFormula.quaternion.arg1_s;
			//todo 将file信息按照指定格式写入fifo
			(*fsInfo) << "COOL_FIL:" << currentFIL << ";";
			(*fsInfo).flush();

			cout << "location:(" << currentFIL << ":" << currentLIN << ")"
				<< endl;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_LIN
			&& setting->debugMode == true) {
			currentLIN = currentCode->assemblyFormula.quaternion.arg1_i;
			//todo 将line信息按照指定格式写入fifo
			(*fsInfo) << "COOL_LIN:" << currentLIN << ";";
			map<Strg, set<Intg>>::iterator&& it = breakPointMap.find(
				currentFIL);
			if (it != breakPointMap.end()) {
				if (it->second.count(currentLIN) > 0) {
					BEG = false;
				}

			}

			cout << "location:(" << currentFIL << ":" << currentLIN << ")"
				<< endl;

		}
		update_AR_stack();
	}







}