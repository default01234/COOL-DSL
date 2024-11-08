#include"coolang_vm_execute.hpp"
using namespace std;
namespace COOLANG {
	/**
	 * @name split 返回[min,max)(n==1)或(min,max)(n>1)
	 * @attention 特别的，当max为null时，直接在min之后顺延n个地址
	 * @param min
	 * @param max
	 * @param n
	 * @return
	 */
	vector<Addr> split(const Addr& min, const Addr& max, const Intg n) {
		//special case
		//if (min.size() == max.size() && min.size() != 0 && max.addrv.back() - min.addrv.back() > n) {
		//	vector<Addr> avret;
		//	auto avit = min;
		//	for (int i = 0; i < n; i++) {

		//		avret.push_back(++avit);
		//	}
		//	return avret;
		//}
		//general case
		if ((min == max && n != 1) || n == 0) {

			cout << "split err,(min == max && n != 1) || n == 0 min:["
				<< min.toStrg() << "]\tmax:[" << max.toStrg() << "]\tn:[" << n
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
			assert(false); exit(-1);
		}

		vector<Addr> av;
		if (min == max && n == 1) {
			av.push_back(min);
		}
		Addr mid = min;

		Intg m = (min.size() <= max.size()) ? min.size() : max.size();
		for (Intg p = 0; p < m - 1; p++) {
			if (mid.addrv[p] < max.addrv[p]) {
				while ((Intg)av.size() < n) {
					av.push_back(++mid);

				}
				return av;
			}
		}

		if (mid.size() <= max.size() && mid < max) {
			while (mid.size() < max.size() && mid < max) {
				mid.addrv.push_back(1);

			}
			mid.addrv.push_back(2);
			while ((Intg)av.size() < n) {

				av.push_back(++mid);
				continue;

#if debug
				cerr << "split err 1" << endl;
#endif

			}
			return av;
		}

		//    if (mid.size() == max.size()) {
		//        mid.addrv.push_back(2);
		//        while ((Intg) av.size() < n) {
		//
		//            av.push_back(++mid);
		//            continue;
		//
		//            #if debug
		//    cerr << "split err 2" << endl;
		//    #endif
		//
		//        }
		//    }
		if (mid.size() > max.size()) {
			while ((Intg)av.size() < n) {

				av.push_back(++mid);
				continue;

#if debug
				cerr << "split err 3" << endl;
#endif

			}
			return av;
		}
#if debug
		cerr << "split err, can't split" << endl;
#endif
		return av;
	}


	/*********************************************

	ExpressionHandler

	*****************************************/

	ExpressionHandler::ExpressionHandler() :ruleFunBodyAR(DataTablePtr::make()) {
		expressionhandler_counter++;

		this->serialNumber = expressionhandler_counter;
	}
	/// <summary>
/// this function replace formal args and args with expression data in this expression with actual args or expr in data.
/// 	for formal args, replace them with corresponding realarg.
/// 	for local args, replace them with corresponding datavalue(number, string ...)
/// don't handle Arg EXPRZ_Dz seperately, because it should have already bound with data with isExpression == true.
/// Only replace all matched nodes (leaf nodes), no non-related addrs (inner nodes) are changed.
/// !!!!!
/// 	flatten can only be invoked when the ascs of relative code and args are correctly set!!
/// !!!!!
/// </summary>
/// <param name="realexpr">where the real/actual args from</param>
/// <param name="dt"></param>
/// <param name="frmap"></param>
	void ExpressionHandler::flatten(ExpressionHandlerPtrW realexpr, DataTablePtrW dt, map<ArgFormal, ArgReal, ArgComparatorLT> frmap, CodeTableHash* cdth /*= nullptr*/, ScopeTableHash* scpth) {
		//1 get arg-subexpr map
		map<ArgFormal, ExpressionHandlerPtr, ArgComparatorLT> argformal_expr_map(ArgComparatorLT(acm::name | acm::asc));
		map<ArgFormal, ArgReal, ArgComparatorLT> argformal_argreal_map(ArgComparatorLT(acm::name | acm::asc));
		argformal_argreal_map.insert(frmap.begin(), frmap.end());
#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			stringstream ss;
			ss << bbhline;
			ss << calign("ThisExpr", 96);
			ss << bhline;
			ss << this->toStrg();
			ss << bhline;
			ss << calign("realexpr", 96);
			ss << bhline;
			ss << realexpr->toStrg();
			ss << bhline;
			ss << calign("dt", 96);
			ss << bhline;
			ss << dt->toStrg();
			ss << bhline;
			ss << calign("frmap", 96);
			ss << bhline;
			ss << COOLANG::toStrg<ArgFormal, ArgReal>(frmap);
			ss << bhline;
			ss << calign("cdth", 96);
			ss << cdth->toStrg();
			ss << bbhline;

			std::string info = ss.str(); // Replace with actual info
			info += std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
			std::cout << info << endl;
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif

			ignoreCount++;
		}
#endif

#if (debug || filelog) && _WIN32 
		if (debugflag == true) {
			stringstream ss;
			ss << bhline;
			ss << __FUNCTION__ << "::\n";
			ss << calign("ThisExpr", 96);
			ss << bhline;
			ss << this->toStrg();
			ss << bhline;
			ss << calign("realexpr", 96);
			ss << bhline;
			ss << realexpr->toStrg();
			ss << bhline;
			ss << calign("dt", 96);
			ss << bhline;
			ss << dt->toStrg();
			ss << bhline;
			ss << calign("frmap", 96);
			ss << bhline;
			ss << COOLANG::toStrg<ArgFormal, ArgReal>(frmap);
			ss << bhline;
			if (cdth) {
				ss << calign("cdth", 96);
				ss << cdth->toStrg();
			}
			ss << bhline;
			static int ignoreCount = 0;
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
			ignoreCount++;
		}
#endif

		auto find_in_function_body = [&dt, &scpth](const Arg& arg, bool include_query_ar = false) ->bool {
			set<Intg> function_scp{ FB, RFB,BRB,LOB };

			auto p = dt;
			if (p->findcurrent(arg) == true) {
				return true;
			}
			if (scpth == nullptr) {
				return false;
			}
			while (!(p->parentAR == nullptr || function_scp.count(scpth->operator[](p->parentAR->scopeStructureFKA).functionFlag) == 0)) {
				p = p->parentAR;
				if (p->findcurrent(arg) == true) {
					return true;
				}
				else if (include_query_ar) {
					for (auto& ar_ : p->queryARList) {
						if (ar_->findcurrent(arg)) {
							return true;
						}
					}
				}
				else {
					continue;
				}
			}
			return false;
			};
		//if this->expr_arg
		if (this->type == EXPRESSIONHANDLER_ARG) {
			//is placeholder
			auto argkey = dt->getKeyArg(this->expr_arg);
			if (argkey.isPlaceholder && argkey.isBound == false) {
				return;
			}
			//is symbol
			if (this->expr_arg.isSymbol) {
				return;
			}
			//is formal arg, replace arg as realarg
			else if (argformal_argreal_map.count(this->expr_arg)) {
				auto* argreal = &argformal_argreal_map[this->expr_arg];
				if (argreal->isVar() == false) {
					this->expr_arg = (*argreal);
				}
				else if (argreal->isBs()) {
					this->expr_arg = (*argreal);
				}
				else { //Dz
					auto&& oacdt = realexpr->expr_cdt.copyTreeBranch(argreal->arg_fka.addr);
					auto exprtmp = ExpressionHandlerPtr::make(oacdt);
					this->deepAssign(exprtmp);

				}
			}
			//is local arg (only replace expression), replace arg as the value of its data
			else {
				if (this->expr_arg.isVar() && dt->findall(this->expr_arg)) {
					DataPtr data_;
					if (this->expr_arg.isDz()) {
						data_ = DataPtr(&(*dt)[this->expr_arg]);
					}
					else {
						if (this->expr_arg.asc != fkanull && dt->getAccessibleAR(dt, this->expr_arg.asc) != nullptr && dt->getAccessibleAR(dt, this->expr_arg.asc)->findall(this->expr_arg)) {

							auto dttmp = dt->getAccessibleAR(dt, this->expr_arg.asc);
							data_ = DataPtr(&(*dttmp)[this->expr_arg]);
						}
						else {
							data_ = DataPtr(&(*dt)[this->expr_arg]);
						}
					}
					while (data_->dataFlagBit == S_PTR) {
						data_ = data_->content_ptr;
					}
					if (data_->isExpression) {
						this->deepAssign(data_->expr);
						this->flatten(realexpr, dt, frmap, cdth);
					}
					else if (find_in_function_body(this->expr_arg, true)) {
						this->close_high_bound = fkanull;
						this->open_high_bound = fkanull;
						this->close_low_bound = fkanull;
						this->open_low_bound = fkanull;
						this->type = EXPRESSIONHANDLER_ARG;
						this->expr_arg = Arg();
						if (data_->dataFlagBit == Sz) {
							this->expr_arg.argFlag = Sz;
							this->expr_arg.arg_i = data_->content_i;
						}
						else if (data_->dataFlagBit == Zf) {
							this->expr_arg.argFlag = Zf;
							this->expr_arg.arg_s = data_->content_s;
						}
						else {
							//todo:: future design for return S_AR, LST, etc. 

						}
						return;
					}
					else {
						//Has been flatten as an outer function variable);
						return;
					}
				}
				else {
					return;
				}
			}
		}

		//if this->expr_cdt
		for (auto& cd : this->expr_cdt.codedq) {

#if (debug || filelog) && _WIN32 
			if (debugflag) {
				static int ignoreCount = 0;
				{
					stringstream ss;
					ss << bhline;
					ss << __FUNCTION__ << "::\n";
					ss << cd.toStrg();
					ss << bhline;

					// Assuming info is a std::string, convert to std::wstring
					std::string info = ss.str(); // Replace with actual info
					info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");

#if filelog
					std::cout << info << std::endl;
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

			for (int i : {0, 1, 2, 3}) {
				Arg&& arg = cd[i];
				if (arg.isVar()) {
					if (argformal_argreal_map.count(arg) > 0) {
						auto* argreal = &argformal_argreal_map[arg];
						if (argreal->isVar() == false) {

							argformal_expr_map[arg] = ExpressionHandlerPtr::make(*argreal);


						}
						else if (argreal->argFlag == S_Dz) {

							//s_dz 
							auto&& oacdt = realexpr->expr_cdt.copyTreeBranch(argreal->arg_fka.addr);
							argformal_expr_map[arg] = ExpressionHandlerPtr::make(oacdt);
						}

						else if (argreal->argFlag == S_Bs) {
							//if we can find the corresponding data of an identifier, we will replace the var in cd with the data with priority.
							if (dt->getAccessibleAR(dt, argreal->asc) != nullptr && dt->getAccessibleAR(dt, argreal->asc)->findall(*argreal)) {
								DataPtr data_ = DataPtr(&(*dt->getAccessibleAR(dt, argreal->asc))[*argreal]);
								while (data_->dataFlagBit == S_PTR) {
									data_ = data_->content_ptr;
								}
								if (data_->isExpression) {

									argformal_expr_map[arg] = data_->expr;
								}
								else if (data_->dataFlagBit == Sz) {
									argformal_expr_map[arg] = ExpressionHandlerPtr::make(*argreal);
								}
								else if (data_->dataFlagBit == Zf) {
									argformal_expr_map[arg] = ExpressionHandlerPtr::make(*argreal);
								}
								else {
									//todo:: future design for return S_AR, LST, etc. 
									argformal_expr_map[arg] = ExpressionHandlerPtr::make(*argreal);
								}
							}
							else {

								throw exception("Cannot use an undefined/ inaccessible identifier");

							}

						}

					}
					else if (dt->findall(arg)) {
						DataPtr data_;
						if (arg.argFlag == S_Dz) {
							continue;
							//data_ = DataPtr(&(*dt)[arg]);
						}
						else {
							if (this->expr_arg.asc != fkanull && dt->getAccessibleAR(dt, arg.asc) != nullptr && dt->getAccessibleAR(dt, arg.asc)->findall(arg)) {

								auto dttmp = dt->getAccessibleAR(dt, this->expr_arg.asc);
								data_ = DataPtr(&(*dttmp)[arg]);
							}
							else {
								try
								{
									data_ = DataPtr(&(*dt)[arg]);
								}
								catch (Arg& argr) {
									data_ = DataPtr::make();
									data_->dataFlagBit = argr.argFlag;
									if (argr.argFlag == Sz) {
										data_->content_i = argr.arg_i;
									}
									else if (argr.argFlag == Zf) {
										data_->content_s = argr.arg_s;
									}

								}
							}
						}

						while (data_->dataFlagBit == S_PTR) {
							data_ = data_->content_ptr;
						}
						if (data_->isExpression) {

							argformal_expr_map[arg] = data_->expr;
						}
						//if arg is a local variable in dt, use its value to replace this arg
						else if (find_in_function_body(arg, false) && arg.argFlag != S_Dz && dt->argdmap[arg] != datanull) {
							argformal_expr_map[arg] = ExpressionHandlerPtr::make();
							auto& expr_ = argformal_expr_map[arg];
							expr_->type = EXPRESSIONHANDLER_ARG;
							expr_->expr_arg = Arg();
							if (data_->dataFlagBit == Sz) {
								expr_->expr_arg.argFlag = Sz;
								expr_->expr_arg.arg_i = data_->content_i;
							}
							else if (data_->dataFlagBit == Zf) {
								expr_->expr_arg.argFlag = Zf;
								expr_->expr_arg.arg_s = data_->content_s;
							}
							else {
								//todo:: future design for return S_AR, LST, etc. 

							}
							continue;
						}
					}
					else {
						continue;
					}

				}

			}

		}
		//		for (auto& it : frmap) {
		//			auto* argformal = &(it.first);
		//			auto* argreal = &(it.second);
		//
		//			while (argreal->isPlaceholder && argreal->isBound) {
		//				if (argreal == nullptr) {
		//
		//#if debug && _WIN32
		//					{
		//						static int ignoreCount = 0;
		//						// Assuming info is a std::string, convert to std::wstring
		//						std::string info = "invalid binding-arg, exit(-1)"; // Replace with actual info
		//						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
		//						std::wstring winfo(info.begin(), info.end());
		//
//						#if WIN_DEBUG_OUTPUT
//		OutputDebugString(winfo.c_str());
//#endif
//		cout << info << endl;
		//						ignoreCount++;
		//					}
		//#endif
		//					cerr << "invalid binding-arg, exit(-1)" << endl;
		//					assert(false);exit(-1);
		//				}
		//			}
		//
		//			//bs or s_dz
		//			if (!argreal->isDz() || (argreal->isDz() && (argreal->argFlag == Z_Dz || argreal->argFlag == EXPRZ_Dz || argreal->argFlag == Y_Dz))) {
		//				argformal_expr_map[*argformal] = ExpressionHandlerPtr::make(*argreal);
		//
		//			}
		//			else {
		//				//s_dz 
		//				auto&& oacdt = realexpr->expr_cdt.copyTreeBranch(argreal->arg_fka.addr);
		//				argformal_expr_map[it.first] = ExpressionHandlerPtr::make(oacdt);
		//			}
		//
		//
		//
		//		}

				//2 replace arg with subexpr

#if debug && _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			stringstream ss;
			ss << bbhline;
			ss << calign("flatten::argformal_expr_map", 96) << endl;
			for (auto& p : argformal_expr_map) {
				ss << "key:" << calign(p.first.toBrief(), 48) << "\n" << "value:\n" << calign(p.second->toBrief(), 48) << endl;
			}
			ss << bbhline;
			std::string info = ss.str(); // Replace with actual info
			std::cout << info << endl;
			info += std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif

			ignoreCount++;
		}
#endif
		if (this->type == EXPRESSIONHANDLER_CDT) {
			auto placeholders = dt->getPlaceholders(this->expr_cdt);
			for (auto& holder : placeholders) {
				if (holder.isBound == false) {
					argformal_expr_map.erase(holder);
				}
			}
		}
		if (argformal_expr_map.size() > 0) {
			for (auto& argreal_expr_pair : argformal_expr_map) {
				this->replaceVar(argreal_expr_pair.first, argreal_expr_pair.second);

#if debug && _WIN32
				{
					static int ignoreCount = 0;
					// Assuming info is a std::string, convert to std::wstring
					stringstream ss;
					ss << bbhline;
					ss << "flatten::argformal:" << endl;
					ss << calign(argreal_expr_pair.first.toBrief()) << endl;
					ss << "subexpr:" << endl;
					ss << argreal_expr_pair.second->toBrief();
					ss << "this(expr):" << endl;
					ss << this->toBrief() << endl;
					ss << bbhline;
					std::string info = ss.str(); // Replace with actual info
					info += std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
					std::cout << info << endl;
					std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
					OutputDebugString(winfo.c_str());
#endif

					ignoreCount++;
				}
#endif

			}
			this->expr_cdt.checkFileKey();
			this->expr_cdt.deleteInaccessibleSubtree();
			this->expr_cdt.unfoldRingStruct();
			this->expr_cdt.checkChangeable();
#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				stringstream ss;
				ss << bbhline;
				ss << "flatten::raw return:\n";
				ss << this->toBrief();
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

			return this->flatten(realexpr, dt, frmap, cdth);
		}

		//3 do not modify addresses!


	}
	ExpressionHandler::ExpressionHandler(const CodeTable& expr_cdt_) :ruleFunBodyAR(DataTablePtr::make()) {
		this->type = EXPRESSIONHANDLER_CDT;
		this->expr_cdt = expr_cdt_;
		this->close_low_bound_exist = true;
		this->close_low_bound = FKA(expr_cdt_.fileKey, expr_cdt_.addrdq.front());
		this->close_high_bound_exist = true;
		this->close_high_bound = FKA(expr_cdt_.fileKey, expr_cdt_.addrdq.back());
		expressionhandler_counter++;
		this->serialNumber = expressionhandler_counter;
	}

	ExpressionHandler::ExpressionHandler(const OpenFKACDT& expr_fka_cdt_) :ruleFunBodyAR(DataTablePtr::make()) {
		this->type = EXPRESSIONHANDLER_CDT;
		this->expr_cdt = get<0>(expr_fka_cdt_);

		this->close_low_bound_exist = true;
		this->close_low_bound = FKA(expr_cdt.fileKey, expr_cdt.addrdq.front());

		this->close_high_bound_exist = true;
		this->close_high_bound = FKA(expr_cdt.fileKey, expr_cdt.addrdq.back());

		if (get<1>(expr_fka_cdt_) != fkanull) {
			this->open_low_bound_exist = true;
			this->open_low_bound = get<1>(expr_fka_cdt_);
		}

		if (get<2>(expr_fka_cdt_) != fkanull) {
			this->open_high_bound_exist = true;
			this->open_high_bound = get<2>(expr_fka_cdt_);
		}
		expressionhandler_counter++;
		this->serialNumber = expressionhandler_counter;
	}

	ExpressionHandler::ExpressionHandler(const OpenAddrCDT& expr_addr_cdt_) :ruleFunBodyAR(DataTablePtr::make()) {
		this->type = EXPRESSIONHANDLER_CDT;
		this->expr_cdt = get<0>(expr_addr_cdt_);

		this->close_low_bound_exist = true;
		this->close_low_bound = FKA(expr_cdt.fileKey, expr_cdt.addrdq.front());

		this->close_high_bound_exist = true;
		this->close_high_bound = FKA(expr_cdt.fileKey, expr_cdt.addrdq.back());

		if (get<1>(expr_addr_cdt_) != addrnull) {
			this->open_low_bound_exist = true;
			this->open_low_bound.addr = get<1>(expr_addr_cdt_);
			this->open_low_bound.fileKey = expr_cdt.fileKey;
		}

		if (get<2>(expr_addr_cdt_) != addrnull) {
			this->open_high_bound_exist = true;
			this->open_high_bound.addr = get<2>(expr_addr_cdt_);
			this->open_high_bound.fileKey = expr_cdt.fileKey;
		}
		expressionhandler_counter++;
		this->serialNumber = expressionhandler_counter;
	}

	ExpressionHandler::ExpressionHandler(const Arg& arg) :ruleFunBodyAR(DataTablePtr::make()) {
		this->type = EXPRESSIONHANDLER_ARG;
		this->expr_arg = arg;
		expressionhandler_counter++;
		this->serialNumber = expressionhandler_counter;
	}

	void ExpressionHandler::setAR(DataTablePtr ruleFunBodyAR_, DataTablePtrW baseAR) {
		/*ruleFunBodyAR_->deepAssign(this->ruleFunBodyAR, ruleFunBodyAR_, true);*/
		this->baseAR = baseAR;
		bool contain = false;
		for (auto& ptr : this->ruleFunBodyAR->queryARList) {
			if (ptr == baseAR) {
				contain = true;
				break;
			}
		}
		if (contain == false) {
			ruleFunBodyAR_->queryARList.push_back(baseAR);
		}
	}


	/*void ExpressionHandler::setAR(DataTablePtr& ruleFunBodyAR_, DataTablePtrW baseAR) {
		DataTable::deepAssign(this->ruleFunBodyAR, ruleFunBodyAR_, true);
		this->baseAR = baseAR;
		bool contain = false;
		for (auto& ptr : this->ruleFunBodyAR->queryARList) {
			if (ptr == baseAR) {
				contain = true;
				break;
			}
		}
		if (contain == false) {
			this->ruleFunBodyAR->queryARList.push_back(baseAR);
		}
	}*/

	FKA ExpressionHandler::getMinLowBound() {
		if (open_low_bound_exist) {
			return open_low_bound;
		}
		else if (close_low_bound_exist) {
			return close_low_bound;

		}
		else {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "invalid low bound, exit(-1)"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			std::cerr << "invalid low bound, exit(-1)" << endl;
			assert(false); exit(-1);

		}

	}

	FKA ExpressionHandler::getMaxHighBound() {
		if (open_high_bound_exist) {
			return open_high_bound;
		}
		else if (close_high_bound_exist) {
			return close_high_bound;

		}
		else {
#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "invalid high bound, exit(-1)"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			std::cerr << "invalid high bound, exit(-1)" << endl;
			assert(false); exit(-1);
		}
	}

	/**
		* clone from
		* don't copy serialNumber and subtree_searching_progress_recording_map. clone localAR, doesn't clone baseAR.
		*/
	void ExpressionHandler::deepAssign(ExpressionHandlerPtr expr_) {
		this->type = expr_->type;
		this->expr_cdt = expr_->expr_cdt;
		this->expr_arg = expr_->expr_arg;
		this->symbol_set = expr_->symbol_set;
		this->open_low_bound_exist = expr_->open_low_bound_exist;
		this->open_low_bound = expr_->open_low_bound;
		this->open_high_bound_exist = expr_->open_high_bound_exist;
		this->open_high_bound = expr_->open_high_bound;
		this->close_low_bound_exist = expr_->close_low_bound_exist;
		this->close_low_bound = expr_->close_low_bound;
		this->close_high_bound_exist = expr_->close_high_bound_exist;
		this->close_high_bound = expr_->close_high_bound;
		//this->subtree_searching_progress_recording_map = expr_->subtree_searching_progress_recording_map;

		//this->localAR = expr_->localAR;

		expr_->ruleFunBodyAR->deepAssign((this->ruleFunBodyAR), (expr_->ruleFunBodyAR), true);
		this->baseAR = expr_->baseAR;


	}

	Strg ExpressionHandler::toStrg() const {
		stringstream ss;
		ss << calign("ExprHandler(" + COOLANG::toStrg(serialNumber) + ")", 96);
		if (this->type == EXPRESSIONHANDLER_ARG) {
			ss << calign("Expr_arg:" + this->expr_arg.toStrg(), 96);
		}
		else {
			ss << hline;
			ss << calign("Expr_cdt", 96) << endl;
			ss << calign("open_low_bound(" + this->open_low_bound.toStrg() + ")\t\tclose_low_bound(" + this->close_low_bound.toStrg() + ")\t\tclose_high_bound(" + this->close_high_bound.toStrg() + ")\t\topen_high_bound(" + this->open_high_bound.toStrg() + ")", 96);
			ss << hline;
			ss << expr_cdt.toStrg();
			if (this->ruleFunBodyAR != nullptr) {
				ss << hline;
				ss << "Expr_localAR";
				ss << hline;
				ss << this->ruleFunBodyAR->toStrg();
			}
			if (this->baseAR != nullptr) {
				ss << hline;
				ss << "Expr_baseAR";
				ss << hline;
				ss << this->baseAR->toStrg();
			}
			ss << hline;

			if (subtree_searching_progress_recording_map.size() > 0) {
				ss << hline;
				ss << calign("subtree_searching_progress_recording_map", 96) << "\n";
				ss << calign("ExprSerialNumber", 24) << calign("LastMatchedFKA", 72) << endl;
				for (auto& p : subtree_searching_progress_recording_map) {

					ss << calign(COOLANG::toStrg(p.first), 24) << calign(p.second.toStrg(), 72) << endl;
				}

				ss << hline;
			}
		}
		return ss.str();

	}
	Strg ExpressionHandler::toBrief() const {
		stringstream ss;
		ss << calign("ExprHandler(" + COOLANG::toStrg(serialNumber) + ")", 96);
		if (this->type == EXPRESSIONHANDLER_ARG) {
			ss << calign("Expr_arg:" + this->expr_arg.toStrg(), 96);
		}
		else {
			ss << hline;
			ss << calign("Expr_cdt", 96) << endl;
			ss << calign("open_low_bound(" + this->open_low_bound.toStrg() + ")\t\tclose_low_bound(" + this->close_low_bound.toStrg() + ")\t\tclose_high_bound(" + this->close_high_bound.toStrg() + ")\t\topen_high_bound(" + this->open_high_bound.toStrg() + ")", 96);
			ss << hline;
			ss << expr_cdt.toStrg();

			ss << hline;
		}
		return ss.str();

	}


	/**
			 * @brief utilize groundingState n to generate ExpressionHandler
			 * for groundingState n+1 under the specified environment
			 *
			 * @param gs
			 * @return codetabel n+1
			 * @throw -1 when can't generate code based on gs n.
			 * @attention: you must use a move constructor to capture the return.
			 */
	ExpressionHandlerPtr MultiTFIntegrator::generateExpressionHandlerByGroundingState(const GroundingState* gs) {



#if (debug || filelog) && _WIN32 
		static int ignoreCount = 0;
		if (debugflag) {
			{
				stringstream ss;
				ss << bbhline;
				ss << __FUNCTION__ << "::\n";
				ss << "generateCdtByGroundingState(GroundingState*)::" << "\n";
				ss << "last_gs:" << (gs->lastgs ? COOLANG::toStrg(gs->lastgs->id) : "null") << "\n";

				ss << "BDDB_value:" << gs->BDDB_value << "\n";
				ss << "position:" << gs->position << "\n";
				ss << "TFFKA:" << tfth->getTFNameByFKA(gs->tffka) << "\n";
				ss << bhline;
				ss << "gs(" << gs->id << "):" << endl;
				ss << gs->toStrg(this->scpth) << std::endl;
				ss << bhline;
				ss << "gs::ruleFunBodyAR:";
				ss << (gs->expr->ruleFunBodyAR != nullptr ? gs->expr->ruleFunBodyAR->toStrg() : "");
				ss << bhline;
				ss << "gs::ruleFunNameAR:";
				ss << (gs->expr->ruleFunNameAR != nullptr ? gs->expr->ruleFunNameAR->toStrg() : "");
				ss << bhline;
				ss << "gs::baseAR:";
				ss << (gs->expr->baseAR != nullptr ? gs->expr->baseAR->toStrg() : "");
				ss << bbhline;

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

		AST_transformation_count++;

		/*FKA mindeslimit = cdth->getLastNot(COOL_M, fkades);
		FKA maxdeslimit = cdth->getNextNot(COOL_M, fkades);*/
		//        FKA funIndexFKA = getFunIndexFKA(fkades);
		//
		//        MatchState &ms = gs->matchState;
		//        CodeTable cdtmid_clone(gs->codeTable);
		//        Addr mindes = cdtmid_clone.addrdq[posmindes];

		//        Intg posmindes = gs->position;
		Intg pos = gs->position;
		auto& matchedTFFKA = gs->tffka;

		auto exprGenerated = ExpressionHandlerPtr::make();//the returned expr of this function
		exprGenerated->deepAssign(gs->expr);


		if (exprGenerated->type == EXPRESSIONHANDLER_ARG) {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "invalid grounding state with ExpressionHandler including an arg, exit(-1)"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			cerr << "invalid grounding state with ExpressionHandler including an arg, exit(-1)" << endl;
			assert(false); exit(-1);

		}
		Addr addrnode = (exprGenerated->expr_cdt).addrdq[pos];
		FKA fkaref;
		if (matchedTFFKA == fkanull) {

			//not matching any rule function, just return original epxr.
			return exprGenerated;


		}
		else if (matchedTFFKA.fileKey == FILEKEYBUILTIN) {
			//matching built-in function, just set flagbit then return
			fkaref = matchedTFFKA;
			exprGenerated->expr_cdt[addrnode].assemblyFormula.flagBit.tfFKA = fkaref;
			exprGenerated->expr_cdt[addrnode].assemblyFormula.flagBit.functionHandleFlag = true;
			exprGenerated->expr_cdt.checkFileKey();
			exprGenerated->expr_cdt.deleteInaccessibleSubtree();
			exprGenerated->expr_cdt.unfoldRingStruct();
			return exprGenerated;
		}
		else if (matchedTFFKA.fileKey != FILEKEYBUILTIN) {
			Scope& scp = (*scpth)[matchedTFFKA];
			fkaref = cdth->getLast(COOL_M, FKA(scp.fileKey, scp.scopeCoor2));
			auto cdtref =
				(fkaref.addr > addrnull) ?
				&cdth->operator [](fkaref.fileKey) : &(exprGenerated->expr_cdt);
			//CodeTable* cdtdes = &(exprGenerated->expr_cdt);


			//matching rule function, go to ruleFunAR and call the executor, then integrate the returned expr
			if ((*tfth)[matchedTFFKA].integrate == true) {

				//CodeTable& cdtmid = (*cdtdes);

				//以下代码将操作匹配到的 函数的返回的表达式储存到cdtfrom中


				//invoke rule function will return an expr with code to substitude original code
				ExpressionHandlerPtr exprRuleFunReturn;
				//MultiTFIntegrator会认为以下几种类型的代码不属于语法树。其不会被用来替换原代码段
			/*	vector<Intg> exclude_types{ COOL_QS, COOL_QE, COOL_C, COOL_ME, COOL_FUN, COOL_R, COOL_FIL, COOL_LIN, EXPRQE, EXPRQS };

				const FKA& returnHandle =
					(*scpth)[(*tfth)[matchedTFFKA].bodyScopeFKA].returnHandle[0];

				const FKA&& aboveCodeFKA = cdth->getAboveCodeFKA(returnHandle);

				bool&& notexclude = (find(exclude_types.begin(),
					exclude_types.end(),
					(*cdth)[aboveCodeFKA].assemblyFormula.flagBit.type)
					== exclude_types.end());*/


					//if (notexclude) {


						//if no problems in the code, invoke the rule function and get the true function
						//cdtFunReturn =
						//	cdth->copyTree(
						//		cdth->getLast(COOL_M,
						//			(*scpth)[(*tfth)[matchedTFFKA].bodyScopeFKA].returnHandle[0]));



						//1 gain the formal arg, real arg (cmp arg) from the gs.
				/*map<ArgRef, pair<ArgCmp, AddrCmp> >& rcam = gs->getMatchState().rcam;
				map<ArgFormal, ArgReal> frmap;
				for (auto& it : rcam) {
					frmap.insert(make_pair(it.first, it.second.first));
				}*/




				//auto& bfMeetFKAdq = gs->expr->ruleFunBodyAR->bfMeetFKAdq; //不会执行inverse functions， this is useless
				//auto& backwardExecute = currentAR->backwardExecute; 

				//1 executor recovery point设置executor恢复点
				DataTablePtr executor_rollbackARPtr = executor->currentAR;
				DataTablePtr executor_rollbackAR = DataTablePtr::make();
				executor->currentAR->deepAssign(executor_rollbackAR, executor->currentAR, true);
				/*ExpressionHandlerPtr executor_rollbackExprPtr = executor->currentExpr;
				ExpressionHandlerPtr executor_rollbackExpr = ExpressionHandlerPtr::make();
				executor_rollbackExpr->deepAssign(executor->currentExpr);*/

				FKA executor_rollback_currentCodeFKA = executor->currentCodeFKA;
				FKI executor_rollback_currentCodeFKI = executor->currentCodeFKI;
				FKA executor_rollback_nextCodeFKA = executor->nextCodeFKA;
				FKI executor_rollback_nextCodeFKI = executor->nextCodeFKI;

				Code* executor_rollback_currentCode = executor->currentCode;

				//2 配置gs中 rule function 调用所需的AR环境
				DataTablePtr functionBodyAR = gs->expr->ruleFunBodyAR;
				const FKA& functionNameScopeFKA = matchedTFFKA;
				FKA& functionBodyScopeFKA =
					(*tfth)[functionNameScopeFKA].bodyScopeFKA;
				TemplateFunction& tf =
					(*tfth)[functionNameScopeFKA];
				//DataTablePtr functionBodyParentAR = executor->getParentAR(functionBodyScopeFKA);
				//functionBodyAR->parentAR = functionBodyParentAR;
				functionBodyAR->returnAR = gs->expr->baseAR;
				FKA meetFKA = executor->currentCodeFKA;// 直接返回至当前代码
				functionBodyAR->meetFKA = meetFKA;
				//functionBodyAR->queryARList.push_back(functionBodyParentAR);
				functionBodyAR->scopeStructureFKA = functionBodyScopeFKA;
				functionBodyAR->add(Arg(S_Bs, "thisexpr"));
				Data& thisexprdt = (*functionBodyAR)[Arg(S_Bs, "thisexpr")];
				thisexprdt.isExpression = true;
				thisexprdt.expr = exprGenerated;
				//functionBodyAR->setFRMap(frmap);


				//3 替换executor中的currentAR 为gs中的ruleFunBodyAR， 并设置与nextCodeFKI并启动执行。
				executor->currentAR = gs->expr->ruleFunBodyAR;
				executor->nextCodeFKI =
					cdth->getFKIByFKA(
						(*tfth)[functionNameScopeFKA].bodyStartFKA);
				executor->nextCodeFKI.SI(cdth);
				executor->setNextCodeFKI();
				executor->nextCodeFKA = cdth->getFKAByFKI(executor->nextCodeFKI);


				//4 执行Loop 直到退出（下一步代码地址为meetFKA或当前步指令类型为RETURN）
				while (executor->getNewCode()) {
					if (executor->currentCode->assemblyFormula.flagBit.type == COOL_R || executor->nextCodeFKA == meetFKA) {
						//before left function body AR, gain the ans expr first.
						executor->stepGround();
						exprRuleFunReturn = (*(executor->currentAR))[Arg(Bs, "ans")].expr; //提取rulefunction返回的表达式
						(*(executor->currentAR))[Arg(Bs, "ans")].expr = nullptr; //重置 reset the returned 'ans' expression to nullptr
						(*(executor->currentAR))[Arg(Bs, "ans")].isExpression = false;
						exprRuleFunReturn->flatten(gs->expr, gs->expr->ruleFunBodyAR, gs->expr->ruleFunBodyAR->frmap, this->cdth);
						if (exprRuleFunReturn->type == EXPRESSIONHANDLER_CDT) {
							exprRuleFunReturn->expr_cdt.checkFileKey();
							exprRuleFunReturn->expr_cdt.deleteInaccessibleSubtree();
							exprRuleFunReturn->expr_cdt.unfoldRingStruct();
							exprRuleFunReturn->expr_cdt.checkChangeable();
						}
						//正常退出无需恢复executor->currentAR的内容。
						executor->currentAR = executor_rollbackARPtr;
						executor->currentCodeFKA = executor_rollback_currentCodeFKA;
						executor->currentCodeFKI = executor_rollback_currentCodeFKI;
						executor->nextCodeFKA = executor_rollback_nextCodeFKA;
						executor->nextCodeFKI = executor_rollback_nextCodeFKI;
						executor->currentCode = executor_rollback_currentCode;
						break;
					}
					try {
						executor->stepExecute();
					}
					catch (ExpressionHandlerPtr& e) {

#if debug && _WIN32
						{
							static int ignoreCount = 0;
							// Assuming info is a std::string, convert to std::wstring
							std::string info = "Grounding aborted, rolling back	..."; // Replace with actual info
							info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
							std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
							OutputDebugString(winfo.c_str());
#endif
							cout << info << endl;
							ignoreCount++;
						}
#endif
						// 如果遇到ABORT指令，则rollback至executor的还原点
						executor->currentAR = executor_rollbackARPtr;
						*executor->currentAR = *executor_rollbackAR;
						executor->currentCodeFKA = executor_rollback_currentCodeFKA;
						executor->currentCodeFKI = executor_rollback_currentCodeFKI;
						executor->nextCodeFKA = executor_rollback_nextCodeFKA;
						executor->nextCodeFKI = executor_rollback_nextCodeFKI;
						executor->currentCode = executor_rollback_currentCode;

						/*executor->currentExpr = executor_rollbackExprPtr;
						*executor->currentExpr = *executor_rollbackExpr;*/

						throw;
					}

				}


				//				}
				//				else {
				//					{
				//#if debug
				//						static int ignorecount = 0;
				//						cout << "GenerateCdtByGroundingState()" << "no tree found"
				//							<< "\tignorecount:[" << ignorecount++ << "\t]("
				//							<< __FILE__ << ":" << __LINE__ << ":0" << ")"
				//							<< endl;
				//#endif
				//					}
				//				}





				//directly return executor->currentExpr if exprFunReturn is null, or the user has modified variable "thisexpr"
				if (exprGenerated->expr_cdt != gs->expr->expr_cdt || (exprRuleFunReturn->type == EXPRESSIONHANDLER_CDT && exprRuleFunReturn->expr_cdt.size() == 0)) {
					exprGenerated->flatten(gs->expr, gs->expr->ruleFunBodyAR, gs->expr->ruleFunBodyAR->frmap, this->cdth);
					//return exprGenerated;//thisexpr

				}

				else if (exprRuleFunReturn->type == EXPRESSIONHANDLER_CDT && exprRuleFunReturn->expr_cdt.size() != 0) {

					const map<AddrReal, AddrFormal>& addrm = gs->getMatchState().addrm;
					Intg pos_ = gs->position;
					Addr mindes;
					Addr maxdes;
					Intg leng = gs->expr->expr_cdt.size();
					if (pos_ == leng - 1) {
						maxdes = gs->expr->getMaxHighBound().addr;
					}
					else {
						maxdes = gs->expr->expr_cdt.addrdq[pos_ + 1];
					}


					while (pos_ >= 0) {
						auto& addrArgReal = gs->expr->expr_cdt.addrdq[pos_];

						if (addrm.count(addrArgReal) == 0) {
							mindes = addrArgReal;
							break;
						}
						--pos_;
					}
					if (mindes == addrnull) {
						mindes = gs->expr->getMinLowBound().addr;
					}

					SingleTFIntegrator stfi(&exprGenerated->expr_cdt, &exprRuleFunReturn->expr_cdt, executor->scpth,
						mindes, maxdes, exprRuleFunReturn->expr_cdt.addrdq[0], gs->getMatchState().cram,
						gs->getMatchState().addrm);
#if debug && _WIN32
					{
						stringstream ss;
						ss << bbhline;
						ss << "MultiTFIntegrator::generateExpressionHandlerByGroundingState::" << endl;
						ss << "exprRuleFunReturn:" << endl;
						ss << exprRuleFunReturn->toBrief();
						ss << bhline;
						ss << "exprGenerated:" << endl;
						ss << exprGenerated->toBrief();
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
					stfi.integrate();

				}
				else if (exprRuleFunReturn->type == EXPRESSIONHANDLER_ARG) {
					const map<AddrReal, AddrFormal>& addrm = gs->getMatchState().addrm;
					CodeTable& cdtdes = exprGenerated->expr_cdt;
					Arg&& rootArg = cdtdes[gs->position][3];
					Addr rootAddr = cdtdes.addrdq[gs->position];

					auto&& posReplace = cdtdes.getNextArgPosition(rootAddr, rootArg, acm::name | acm::asc);
					cdtdes[posReplace.first].setArg(posReplace.second, exprRuleFunReturn->expr_arg, true, false);

					for (auto& it : addrm) {
						auto& addrReal = it.first;
						Intg pos_ = getAddrPosition(cdtdes.addrdq, addrReal);
						if (pos_ >= 0) {
							cdtdes.addrdq.erase(cdtdes.addrdq.begin() + pos_);
							cdtdes.codedq.erase(cdtdes.codedq.begin() + pos_);
						}
					}


				}




			}
			//matching fact function, 
			else {
				CodeTable& cdtGenerated = exprGenerated->expr_cdt;

				CodeTable cdtFunAnnounce = get<0>(cdth->copyTree(
					(*tfth)[matchedTFFKA].templateHandleFKA));
				const MatchState& ms = gs->getMatchState();

				//calculate lowbound
				Intg posmin = pos;
				Addr addrmin = cdtGenerated.addrdq[posmin];
				for (; ms.addrm.count(addrmin); --posmin) {
					if (posmin >= 0) {
						addrmin = cdtGenerated.addrdq[posmin];
					}
					else {
						posmin = 0;
						break;
					}
				}

				//calculate upbound
				Intg posmax = pos;
				Addr addrmax = cdtGenerated.addrdq[posmax];
				if (pos < cdtGenerated.size() - 1) {
					posmax++;
					addrmax = cdtGenerated.addrdq[posmax];

				}

				SingleTFIntegrator stfi(&cdtGenerated, &cdtFunAnnounce, scpth,
					addrmin, addrmax, cdtFunAnnounce.addrdq[0], ms.cram,
					ms.addrm);

				stfi.integrate();



			}






#if debug && _WIN32
			{
				stringstream ss;
				ss << bbhline;
				ss << "COOLANG::MultiTFIntegrator::generateExpressionHandlerByGroundingState::returnvalue" << "\n" <<
					"exprGenerated" << "\n";
				ss << exprGenerated->toBrief();
				ss << "groundingState::in\n";
				ss << gs->toStrg();
				ss << bbhline;

				// Assuming info is a std::string, convert to std::wstring
				std::string info = ss.str(); // Replace with actual info
				info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
				std::cout << info << std::endl;
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif

			}
#endif


			try {
				exprGenerated->expr_cdt.checkFileKey();
				exprGenerated->expr_cdt.deleteInaccessibleSubtree();
				exprGenerated->expr_cdt.unfoldRingStruct();
				exprGenerated->expr_cdt.checkChangeable();
				exprGenerated->expr_cdt.checkConstraints();
			}
			catch (exception& e) {
				cout << bbhline;
				cout << e.what() << endl;
				cout << __FUNCTION__ << endl;
				cout << gs->toStrg();
				cout << bbhline;
				cout.flush();
				assert(false);

			}
			return exprGenerated;

		}


	}


	/*
	@param argreplace: be replaced arg
	*/
	void ExpressionHandler::replaceVar(const Arg& argreplace, ExpressionHandlerPtr subexpr) {
		// 4.1 If the subexpr is an arg, replace argreplace in this expr with subexpr's arg
		if (subexpr->type == EXPRESSIONHANDLER_ARG) {
			for (Code& code : this->expr_cdt.codedq) {
				for (int i = 0; i < 4; ++i) {
					if (Arg::intersect(code[i], argreplace, acm::name | acm::asc)) {
						code.setArg(i, subexpr->expr_arg, true, true);
					}
				}
			}
		}
		// 4.2 If the subexpr is a codetable
		else if (subexpr->type == EXPRESSIONHANDLER_CDT) {
			// Find and replace next argreplace in this expr_cdt with argroot2
			for (Intg pos = 0; pos < this->expr_cdt.codedq.size(); pos++) {
				Code& code = this->expr_cdt.codedq[pos];
				for (int i = 0; i < 4; ++i) {
					if (Arg::intersect(code[i], argreplace, acm::name | acm::asc)) {

						// Copy the codeTable of subexpr
						CodeTable copiedTable = subexpr->expr_cdt;
						if (this->open_low_bound_exist == true) {
							copiedTable.resetFKARange(this->open_low_bound.addr, this->expr_cdt.addrdq.front(), this->open_low_bound.fileKey); // Reset FKARange of copied table

						}
						else {

#if debug && _WIN32
							{
								static int ignoreCount = 0;
								// Assuming info is a std::string, convert to std::wstring
								std::string info = "invalid expr for flatten, openlowbound and closelowbound is needed, exit(-1)"; // Replace with actual info
								info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
								std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
								OutputDebugString(winfo.c_str());
#endif
								cout << info << endl;
								ignoreCount++;
							}
#endif
							cerr << "invalid expr for flatten, openlowbound and closelowbound is needed, exit(-1)" << endl;

						}

						// Find argroot2 in the copied table
						Arg argroot2 = copiedTable.codedq.back()[3];



						code.setArg(i, argroot2, true, true);
						//break; // Assuming only one replacement per Code object

						// Append the copiedTable ahead of this->expr_cdt each time argreplace is found.
						this->expr_cdt.addrdq.insert(this->expr_cdt.addrdq.begin(), copiedTable.addrdq.begin(), copiedTable.addrdq.end());
						this->expr_cdt.codedq.insert(this->expr_cdt.codedq.begin(), copiedTable.codedq.begin(), copiedTable.codedq.end());
						pos += copiedTable.size();
					}
				}
			}

		}
	}

	/**
fkades 由currentCodeFKA决定，调用此函数创建MultiTFIntegrator时务必确定executor处于grounding状态且currentCodeFKA指向一个未绑定的TF 的COOL_M类型Code

*/
	MultiTFIntegrator::MultiTFIntegrator(Executor* executor) :executor(executor), groundingStateSilo(executor->setting, executor->scpth), mw(executor->cdth, executor->systh), setting(executor->setting) {
		this->cdth = executor->cdth;
		this->fkades = executor->currentCodeFKA;
		this->tfth = executor->tfth;
		this->scpth = executor->scpth;
		this->systh = executor->systh;
		this->currentAR = executor->currentAR;
		this->setting = setting;
		this->train = setting->train;
		this->neuralNetworkEnabled = setting->neuralNetworkEnabled;
		this->pipe_writing_handler = executor->pipe_writing_handler;
		this->pipe_reading_handler = executor->pipe_reading_handler;

		if (this->train || this->neuralNetworkEnabled) {
			this->DSP_domains = getAllAccessiableClassAndFile();
		}
	}

	/***
		 *
		 * @param cdth
		 * @param fkades 需要进行整合匹配的目标位置fka
		 * @param tfth
		 * @param scpth
		 */
	MultiTFIntegrator::MultiTFIntegrator(CodeTableHash* cdth, const FKA& fkades,
		TemplateFunctionTableHash* tfth, ScopeTableHash* scpth,
		SystemTableHash* systh, DataTablePtr currentAR, Setting* setting,
		PipeCommunicationHandler* pipe_writing_handler_,
		PipeCommunicationHandler* pipe_reading_handler_) :
		groundingStateSilo(setting, scpth), mw(cdth, systh) {
		this->cdth = cdth;
		this->fkades = fkades;
		this->tfth = tfth;
		this->scpth = scpth;
		this->systh = systh;
		this->currentAR = currentAR;
		this->setting = setting;
		this->train = setting->train;
		this->neuralNetworkEnabled = setting->neuralNetworkEnabled;
		this->pipe_writing_handler = pipe_reading_handler_;
		this->pipe_reading_handler = pipe_reading_handler_;

		if (this->train || this->neuralNetworkEnabled) {
			this->DSP_domains = getAllAccessiableClassAndFile();
		}

	}

	/**
	 * 将工作模式设置为MODE_SEPARATE，其效果是匹配时不再将结果直接整合到cdtmain中，
	 * 但需提供整合代码段目标以及来源
	 * @arg
	 */
	void MultiTFIntegrator::setModeSeparate(CodeTable& seperateCdtDes) {
		mode = MODE_SEPARATE;
		this->seperateCdtDes = &seperateCdtDes;
	}


	void MultiTFIntegrator::setMaxCapacity(Intg matchStateSiloMaxCapacity,
		Intg groundingStateSiloMaxCapacity, Intg maxSwitchTurn) {
		this->matchStateSiloMaxCapacity = matchStateSiloMaxCapacity;
		this->groundingStateSiloMaxCapacity = groundingStateSiloMaxCapacity;
		this->maxSwitchTurn = maxSwitchTurn;
		this->groundingStateSilo.setMaxCapacity(groundingStateSiloMaxCapacity);
	}
	/**
		 * @brief 获得指定表达式匹配函数时从何处开始寻找可访问函数
		 * @brief
		 * @param expFKA表达式中的任意一个codeFKA地址
		 * @attention 任何时候表达式都不能跨cdt
		 * @return 通常而言，输入的参数expfka就满足要求，
		 * 但如果其所在表达式中存在成员引用，例如a.b()，
		 * 则输入的参数expfka就不满足要求，将选取类内部上一行代码的fka。
		 */
	FKA MultiTFIntegrator::getFunIndexFKA(const FKA& expFKA) {
		FKA mindeslimit = cdth->getLastNot(vector<Intg> { COOL_M, COOL_FIL, COOL_LIN },
			fkades);
		FKA maxdeslimit = cdth->getNextNot(vector<Intg> { COOL_M, COOL_FIL, COOL_LIN },
			fkades);

		CodeTable& expCdt = cdth->operator [](fkades.fileKey);
		Intg posmax = getAddrPosition(expCdt.addrdq, maxdeslimit.addr);
		Intg posmin = getAddrPosition(expCdt.addrdq, mindeslimit.addr);
		if (fkades.fileKey != mindeslimit.fileKey) {
			posmin = -1;
		}
		if (fkades.fileKey != maxdeslimit.fileKey) {
			posmax = cdth->operator [](expFKA.fileKey).size();

		}

		while (--posmax > posmin) {
			if (expCdt.codedq[posmax].assemblyFormula.flagBit.operator_flag
				== M_Bs
				|| expCdt.codedq[posmax].assemblyFormula.quaternion.operator_s
				== "call") {
				Arg&& funNameArg = (expCdt.codedq[posmax])[0];
				if (funNameArg.argFlag == Y_Dz) {
					funNameArg.ref_ar = currentAR;

					FKA&& sysScopeFKA = globalRefTable[funNameArg].asc;
					Scope& sysScope = (*scpth)[sysScopeFKA];
					//需要返回类内的一行代码才能访问类内的函数
					FKA&& expAddr_ = cdth->getAboveCodeFKA(
						FKA(sysScope.fileKey, sysScope.scopeCoor2));
					return expAddr_;

				}
				break;
			}
		}
		return expFKA;

	}
	/**
		 * this function is the api to interact with the python program.
		 * @param gs
		 * @return
		 */
	json MultiTFIntegrator::getNeuralNetworkAgentPrediction(const GroundingState* gs) {
		if (/*gs->expr->expr_cdt.size() <= 1 || */this->DSP_domains.size() == 0) {

			json j;
			j = json::parse(R"({
				"prediction": []
			})");
			/*j["policies"] = -1;
			j["ac_policies"] = 0;
			j["advance"] = -1;
			j["ac_advance"] = 0;
			j["rpsign"] = -1;
			j["ac_rpsign"] = 0;
			j["rpval"] = -1;
			j["ac_rpval"] = 0;
			j["rule"] = 0;
			j["ac_rule"] = 0;
			j["ac"] = 0;
			j["ci"] = 0;*/
			return j;

		}


		Strg&& strwrite = mw.wrapMessage("predict", this->DSP_domains,
			*gs);

		pipe_writing_handler->writeMessage(strwrite);

		Strg&& strread = pipe_reading_handler->readJsonMessage();

		json&& reply = mw.unwrapMessage(strread);

		/*accumulated_ac += reply["ac"].get<Numb>();
		accumulated_ci += reply["ci"].get<Numb>();*/
		neural_network_assistance_count++;

		return reply;
		//测试用
	}


	/**
		 * @brief 返回表达式(由this->fkades标识)可以访问到的所有的类名和文件名组成的集合，并根据集合内容生成一个hash code
		 * 可以从getLastAccessibleTFFKA那里获得
		 * @return
		 */
	set<Strg> MultiTFIntegrator::getAllAccessiableClassAndFile() {
		set<Strg> DSP_domains;
		Strg currentFileKey = fkades.fileKey;
		//add file name
		//DSP_domains.insert(FILEKEYBUILTIN);
		while (currentFileKey != "") {
			if (this->tfth->file_tft_map.count(currentFileKey) > 0 && this->tfth->file_tft_map[currentFileKey].addrdq.size() > 0)
			{
				DSP_domains.insert(currentFileKey);
			}
			currentFileKey = this->cdth->getLastCdtFileKey(currentFileKey);
		}

		//add "$file $class" name
		try {
			System& sys = this->systh->getDirectSystemByCodeFKA(this->fkades);
			function<void(const System& sys_)> addDSPDomains;
			addDSPDomains = [&DSP_domains, &systh = this->systh,
				&addDSPDomains](const System& sys_) {
				Strg file_class_name = sys_.systemFKA.fileKey + " "
					+ sys_.systemName;
				DSP_domains.insert(file_class_name);
				for (const auto& fki_ : sys_.decedentSystemFKIList) {
					System& sys__ = (*systh)[fki_];
					addDSPDomains(sys__);
				}
				};
			addDSPDomains(sys);

		}
		catch (Intg& i) {
			//Not in a class, do nothing.

		}
		return DSP_domains;
	}


	bool MultiTFIntegrator::integrate() {

		CodeTable* cdtdes = nullptr;
		OpenFKACDT&& ofcdes =
			(mode != MODE_SEPARATE) ? (cdth->copyTree(fkades)) : OpenFKACDT(*seperateCdtDes, fkanull, fkanull); //remember, expr_cdt in ofcdes not at the same memory address.
		if (mode == MODE_SEPARATE) {
			cdtdes = seperateCdtDes;

			//remember to assign the codetable in of cdtdes to the "codeTableDes" as the tuple only copies the instance rather than references to them.
		}
		else {
			cdtdes = &get<0>(ofcdes);
		}
		cdtdes->setRefArgAR(currentAR);

#if debug
		{
			static int ignorecount = 0;
			std::cout << "(" << __FILE__ << ":" << __LINE__ << ":0"
				<< ")MultiTFIntegrator::integrate() in" << " (*cdtdes):["
				<< (*cdtdes).toStrg() << "]" << "\tignorecount:["
				<< ignorecount++ << "\t]" << std::endl;
		}
#endif
		//这一步往silo中添加了一个初始的GroundingState。
		ExpressionHandlerPtr exprdes = ExpressionHandlerPtr::make(ofcdes);
		//executor->currentExpr = exprdes;
		exprdes->setAR(DataTablePtr::make(), currentAR);


		groundingStateSilo.reset(exprdes, false, nullptr);
		Intg turns = 0;                    //recording loop times.

		//以下两个变量规定了代码替换的范围，但必须明确代码替换必须在原cdt中执行，不能跨cdt
		/*FKA mindeslimit = cdth->getLastNot(COOL_M, fkades);
		FKA maxdeslimit = cdth->getNextNot(COOL_M, fkades);*/
		FKA mindeslimit = exprdes->getMinLowBound();
		FKA maxdeslimit = exprdes->getMaxHighBound();
		FKA funIndexFKA = getFunIndexFKA(fkades); //检索函数时，从这个地址开始向上检索,如果此地址为空则无效
		if (maxdeslimit == fkanull) {
			maxdeslimit = fkamax;
		}

		bool reasoning_success = true;
		for (; groundingStateSilo.resultSilo.size() == 0 && reasoning_success == true; ++turns) {




			auto&& p = groundingStateSilo.getNext();
			Numb BDDB_value = p.first;
			const GroundingState* gs = p.second; //n-1时刻生成，n，用于生成n+1时刻的gs的codetable（exprdes）
			pair<Strg, Intg> logicjump_filekey_stage = make_pair((gs != nullptr) ? gs->tffka.fileKey : "", -1);
			if (gs == nullptr) {
				reasoning_success = false;
				break;
			}
			/*	if (gs == nullptr) {
					return false;
				}*/
			if (turns == 0) {
				exprdes->type = gs->expr->type;
				exprdes->expr_cdt = gs->expr->expr_cdt;
				exprdes->expr_arg = gs->expr->expr_arg;
				exprdes->symbol_set = gs->expr->symbol_set;
				exprdes->open_low_bound_exist = gs->expr->open_low_bound_exist;
				exprdes->open_low_bound = gs->expr->open_low_bound;
				exprdes->open_high_bound_exist = gs->expr->open_high_bound_exist;
				exprdes->open_high_bound = gs->expr->open_high_bound;
				exprdes->close_low_bound_exist = gs->expr->close_low_bound_exist;
				exprdes->close_low_bound = gs->expr->close_low_bound;
				exprdes->close_high_bound_exist = gs->expr->close_high_bound_exist;
				exprdes->close_high_bound = gs->expr->close_high_bound;

			}
			else {
				try {
					exprdes = generateExpressionHandlerByGroundingState(gs);
					logicjump_filekey_stage = make_pair(gs->tffka.fileKey, this->executor->logicStage);
					this->executor->logicStage = -1;
					//const_cast<GroundingState*>(gs)->expr = exprdes;

				}
				catch (ExpressionHandlerPtr& expr) {


#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = "GroundingState with non-execuble expr, continue"; // Replace with actual info
						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
						std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
						OutputDebugString(winfo.c_str());
#endif
						cout << info << endl;
						ignoreCount++;
					}
#endif
					cout << "GroundingState with non-executable expr, continue" << endl;
					gs->expr->ruleFunBodyAR = expr->ruleFunBodyAR;

					groundingStateSilo.abortedSilo.insert(make_pair(gs->BDDB_value, gs));

					continue;

				}

			}

			//exprdes = 
			//	(turns == 0) ?
			//	gs->expr : generateExpressionHandlerByGroundingState(gs); //n 时刻的expr

#if debug && _WIN32
			{
				stringstream ss;
				ss << bbhline;
				ss << "MultiTFIntegrator::integrate::while resultsilo null:" << endl;
				ss << "exprdes:" << endl;
				ss << exprdes->toBrief();
				ss << bbhline;
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = ss.str(); // Replace with actual info
				info += std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif


			cdtdes = &(exprdes->expr_cdt);

			Intg leng = (*cdtdes).addrdq.size();
			int ungroundedCodeNum = leng; //一个ast中未被绑定的code的数目
			Intg pos = leng - 1;
			//{'advance': 0, 'ac_advance' : 0, 'rpsign' : 0, 'ac_rpsign' : 0, 'rpval' : 0, 'ac_rpval' : 0, 'policies' : 0 , 'ac_policies' : 0, 'ac' : 0, 'ci' : 1}
			GroundingState tmpgs(false);
			tmpgs.statemat = gs->statemat;
			tmpgs.lastgs = gs;
			tmpgs.BDDB_value = gs->BDDB_value;
			tmpgs.tffka = gs->tffka;
			tmpgs.expr = exprdes;
			json jprediction = this->setting->neuralNetworkEnabled ? getNeuralNetworkAgentPrediction(&tmpgs) : json::parse(R"({
				"prediction": []
			})");


			map<set<Strg>, map<Strg, Numb>, StrgSetComparator> predictionMap;
			Intg domain_resolved = 0;

			// Iterate through the JSON array
			for (const auto& item : jprediction["prediction"]) {
				try {
					int indomain = item.at("indomain").get<Intg>();
					if (indomain > 0) {
						++domain_resolved;
					}
					int succeed = item.at("succeed").get<Intg>();
					if (indomain <= 0 || succeed == -1) {
						// Skip invalid predictions
						continue;
					}


					std::set<std::string> file_class_name_set(item.at("file_class_name").begin(), item.at("file_class_name").end());

					std::map<std::string, Numb> value_map;

					// Initialize values with invalid values
					value_map["ac_policies"] = 0.0;
					value_map["policies"] = -1;
					value_map["ac_advance"] = 0.0;
					value_map["advance"] = -1;
					value_map["ac_rpsign"] = 0.0;
					value_map["rpsign"] = -1;
					value_map["ac_rpval"] = 0.0;
					value_map["rpval"] = 0;
					value_map["ac_rule"] = 0.0;
					value_map["rule"] = 0;
					value_map["ac"] = 0.0;

					for (const auto& el : item.items()) {
						const std::string& key = el.key();
						const json& value = el.value();

						// Skip "file_class_name" as it is used as the key
						if (key == "file_class_name") {
							continue;
						}

						// Add to value_map only if the value is valid
						if (!value.is_null()) {
							if (value.is_number()) {
								value_map[key] = value.get<Numb>();
							}
							else if (value.is_boolean()) {
								value_map[key] = static_cast<Numb>(value.get<bool>());
							}
						}
					}

					if (succeed == 0) {
						// Set other flag bits to -1 if succeed is 0
						value_map["policies"] = -1;
						value_map["advance"] = -1;
						value_map["rpsign"] = -1;
						value_map["rpval"] = 0;
						value_map["rule"] = 0;
					}
					else {
						// Check if any flag bits are -1 and set succeed to 0 if true
						if (value_map["policies"] == -1 || value_map["advance"] == -1 || value_map["rpsign"] == -1) {
							value_map["succeed"] = 0;
							value_map["rule"] = 0;

						}
					}

					// Add the pair to predictionMap
					predictionMap[file_class_name_set] = value_map;
				}
				catch (const std::exception& e) {
					std::cerr << "Error processing prediction item: " << e.what() << std::endl;
					continue;  // Skip the current item and continue with the next
				}
			}

			//Numb policies = this->setting->neuralNetworkEnabled ? prediction["policies"].get<Numb>() : 0;
			//Numb ac_policies = this->setting->neuralNetworkEnabled ? prediction["ac_policies"].get<Numb>() : 0;

			//Numb advance = this->setting->neuralNetworkEnabled ? prediction["advance"].get<Numb>() : 0;
			//Numb ac_advance = this->setting->neuralNetworkEnabled ? prediction["ac_advance"].get<Numb>() : 0;

			//Numb rpsign = this->setting->neuralNetworkEnabled ? prediction["rpsign"].get<Numb>() : 0;
			//Numb ac_rpsign = this->setting->neuralNetworkEnabled ? prediction["ac_rpsign"].get<Numb>() : 0;

			//Numb rpval = this->setting->neuralNetworkEnabled ? prediction["rpval"].get<Numb>() : 0;
			//Numb ac_rpval = this->setting->neuralNetworkEnabled ? prediction["ac_rpval"].get<Numb>() : 0;

			//Numb ac = this->setting->neuralNetworkEnabled ? prediction["ac"].get<Numb>() : 0;
			//Numb ci = 1;//this->setting->neuralNetworkEnabled ? prediction["ci"].get<Numb>() : 0;

			//std::vector<Numb> policies;
			//if (this->setting->neuralNetworkEnabled) {
			//	if (prediction["policies"].is_array()) {
			//		for (const auto& item : prediction["policies"]) {
			//			policies.push_back(item.get<Numb>());
			//		}
			//	}
			//	// Fill the rest of the vector with default values if needed
			//	while (policies.size() < leng) {
			//		policies.push_back(Numb());
			//	}

			//}
			//else {
			//	policies = std::vector<Numb>(leng);
			//}

			bool addsuccess = false;
			while (pos >= 0) {

#if debug && _WIN32
				{
					static int ignoreCount = 0;
					// Assuming info is a std::string, convert to std::wstring
					std::string info = "groundingStateSilo.add pos = " + toStrg(pos) + "\ncdtdes:\n" + cdtdes->toStrg(); // Replace with actual info
					info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
					std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
					OutputDebugString(winfo.c_str());
#endif
					cout << info << endl;
					ignoreCount++;
				}
#endif




				if ((*cdtdes)[pos].assemblyFormula.flagBit.tfFKA != fkanull) {
					pos--;
					ungroundedCodeNum--;
					try {

						if (ungroundedCodeNum == 0) {
							//grounding success

							auto succeed = true;
							auto ac_policies = 0;
							auto policies = -1;
							auto ac_advance = 0;
							auto advance = -1;
							auto ac_rpsign = 0;
							auto rpsign = -1;
							auto ac_rpval = 0;
							auto rpval = 0;
							auto ac_rule = 0;
							auto rule = 0;
							auto ac = 0;
							auto addresult = groundingStateSilo.add(pair<Strg, Strg>(), logicjump_filekey_stage, this->tfth, fkanull,
								TFInfoMat(), (std::max)(pos, 0), false, false, succeed, ac_policies, policies, ac_advance, advance, ac_rpsign, rpsign, ac_rpval, rpval, ac_rule, rule, ac,
								gs->getMatchState(), exprdes, true, gs);
							addsuccess = (addsuccess || addresult);
						}


					}
					catch (const ReasoningException& e) {
						reasoning_success = false;
						break;
					}

					continue;

				}
				Addr addrnode = (*cdtdes).addrdq[pos]; //遍历cdt每个节点进行匹配

#if debug
				{
					static int ignorecount = 0;
					std::cout << "integrate()" << " 2" << "\tignorecount:["
						<< ignorecount++ << "\t](" << __FILE__ << ":"
						<< __LINE__ << ":0" << ")" << std::endl;
				}
#endif
				FKA lastTFFKA = tfth->lastAccessibleTFFKA(scpth, cdth, systh,
					maxdeslimit, funIndexFKA);




				while (lastTFFKA != fkanull) {
#if debug
					{
						static int ignorecount = 0;
						std::cout << "(" << __FILE__ << ":" << __LINE__ << ":0"
							<< ")MultiTFIntegrator::integrate()"
							<< " in loop of matching lastTFFKA, gs_n-1_id:["
							<< gs->id << "]\tpos:[" << pos << "]\t"
							<< "lastTFFKA:[" << lastTFFKA.toStrg() << "],\t"
							<< "\tignorecount:[" << ignorecount++ << "\t]"
							<< std::endl;
					}
#endif
#if (debug || filelog) && _WIN32 
					if (debugflag) {
						static int ignoreCount = 0;
						auto&& tfnamecdt = TemplateFunctionTableHash::getTFNameCdt(lastTFFKA, this->scpth);
						if (tfnamecdt.codedq.back()[2].arg_s == cdtdes->codedq[pos][2].arg_s) {
							stringstream ss;
							ss << bhline;
							ss << __FUNCTION__ << "::\n";
							ss << "match pos:\t" << pos << "\tmatching tffka:\t" << lastTFFKA.toStrg() << endl;
							ss << "cdtdes::\n" << cdtdes->toStrg() << endl;
							ss << "tfnamecdt(cdtref)::\n" << tfnamecdt.toStrg();
							ss << bhline;
							// Assuming info is a std::string, convert to std::wstring
							std::string info = ss.str(); // Replace with actual info
							info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");

#if filelog
							std::cout << info << std::endl;
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
					FKA fkaref;

					if (lastTFFKA.addr < addrnull
						|| lastTFFKA.fileKey == FILEKEYBUILTIN) {
						fkaref = lastTFFKA;

					}
					else if (lastTFFKA.addr > addrnull) {
						Scope& scp = (*scpth)[lastTFFKA];
						fkaref = cdth->getLast(COOL_M,
							FKA(scp.fileKey, scp.scopeCoor2));
					}
					else {

						cout
							<< "MultiTFIntegrator::integrator err: no suitable template"
							<< endl;

						return false;
					}
					//如果fkaref是内置函数fka，那么传入的cdtref无效，直接传入cdtres即可（不可传入null参数）
					Matcher m(cdtdes,
						(fkaref.addr > addrnull) ?
						&cdth->operator [](fkaref.fileKey) : cdtdes,
						tfth, addrnode, fkaref.addr);
					//m.mss.setMaxCapacity(matchStateSiloMaxCapacity);
					if (m.match() == true) {

#if (debug || filelog) && _WIN32 
						if (debugflag && lastTFFKA.addr > addrnull) {
							auto&& tfnamecdt = TemplateFunctionTableHash::getTFNameCdt(lastTFFKA, this->scpth);
							static int ignoreCount = 0;
							{
								stringstream ss;
								ss << bbhline;
								ss << __FUNCTION__ << "::match success:\n";
								ss << "match pos:\t" << pos << "\tmatching tffka:\t" << lastTFFKA.toStrg() << endl;
								ss << "cdtdes::\n" << cdtdes->toStrg() << endl;
								ss << "tfnamecdt(cdtref)::\n" << tfnamecdt.toStrg();
								ss << bbhline;

								// Assuming info is a std::string, convert to std::wstring
								std::string info = ss.str(); // Replace with actual info
								info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");

#if filelog
								std::cout << info << std::endl;
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

						if (lastTFFKA.addr > addrnull && tfth->operator[](lastTFFKA).integrate == true) {

#if debug && _WIN32
							{
								static int ignoreCount = 0;
								// Assuming info is a std::string, convert to std::wstring
								stringstream ss;
								ss << bbhline;
								ss << __FUNCTION__ << ":non built-in fact function " << lastTFFKA.toStrg() << " matched at " << addrnode.toStrg() << "\n";

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

						}
						/*for (auto it : m.mss.resmsm)*/ { //这个循环执行最多一次，因为只有一个pair
							MatchState& ms = m.getMatchState();
							//const TFInfoMat& organization = it.first;
							//                            CodeTable cdtmid_clone(*cdtdes);
							Intg posmindes = pos;
							Addr mindes = (*cdtdes).addrdq[posmindes];
							//                            groundingStateSilo.add(organization, pos,
							//                                    polices[pos], ms, cdtmid_clone, gs);


							Strg className; //systemname
							try {
								className = systh->getDirectSystemByCodeFKA(
									lastTFFKA).systemName;
							}
							catch (Intg& e) {
								//can't find the correspounding class(system)
								className = "";
							}
							TFInfoMat organization;
							if (fkaref.fileKey != FILEKEYBUILTIN) {
								const FKA& tfFKA = cdth->getScopeFKA(fkaref);
								organization = (*tfth)[tfFKA].organization;
							}
							else {
								organization = TFInfoMat(BUILTIN_FUN_INFO);
							}

							bool added = false;
							for (const auto& map_entry : predictionMap) {
								if (map_entry.first.find(split(lastTFFKA.fileKey, "#")[0]) != map_entry.first.end()) {
									const auto& value_map = map_entry.second;
									auto indomain = value_map.at("indomain");
									if (indomain < 0.8) {
										continue;
									}
									added = true;
									auto succeed = value_map.at("succeed");
									auto ac_policies = value_map.at("ac_policies");
									auto policies = value_map.at("policies");
									auto ac_advance = value_map.at("ac_advance");
									auto advance = value_map.at("advance");
									auto ac_rpsign = value_map.at("ac_rpsign");
									auto rpsign = value_map.at("rpsign");
									auto ac_rpval = value_map.at("ac_rpval");
									auto rpval = value_map.at("rpval");
									auto ac_rule = value_map.at("ac_rule");
									auto rule = value_map.at("rule");
									auto ac = value_map.at("ac");

									// Call the add function
									try {
										auto addresult = groundingStateSilo.add(
											std::make_pair(lastTFFKA.fileKey, className), logicjump_filekey_stage, tfth, lastTFFKA, organization,
											pos,
											domain_resolved, indomain,
											succeed, ac_policies, policies, ac_advance, advance, ac_rpsign, rpsign,
											ac_rpval, rpval, ac_rule, rule, ac, ms, exprdes, true, gs
										);
										addsuccess = (addsuccess || addresult);
									}
									catch (const ReasoningException& e) {
										reasoning_success = false;
										break;
									}

								}
							}

							if (added == false) {
								auto succeed = 0;
								auto ac_policies = 0;
								auto policies = -1;
								auto ac_advance = 0;
								auto advance = -1;
								auto ac_rpsign = 0;
								auto rpsign = -1;
								auto ac_rpval = 0;
								auto rpval = 0;
								auto ac_rule = 0;
								auto rule = 0;
								auto ac = 0;

								try {
									auto addresult = groundingStateSilo.add(
										pair<Strg, Strg>(lastTFFKA.fileKey,
											className), logicjump_filekey_stage, this->tfth, lastTFFKA, organization,
										pos,
										domain_resolved, false, //the template function is not from desired knowledge domain
										succeed, ac_policies, policies, ac_advance, advance, ac_rpsign, rpsign, ac_rpval, rpval, ac_rule, rule, ac, ms, exprdes, true, gs);

									addsuccess = addsuccess || addresult;
								}
								catch (const ReasoningException& e) {
									reasoning_success = false;
									break;
								}

							}
						}
					}
#if debug
					cout << "lastTFFKA 1:[" << lastTFFKA.toStrg() << "]"
						<< endl;
#endif
					lastTFFKA = tfth->lastAccessibleTFFKA(scpth, cdth, systh,
						lastTFFKA, funIndexFKA);
#if debug
					cout << "lastTFFKA 2:[" << lastTFFKA.toStrg() << "]"
						<< endl;
#endif
				}
				pos--;

			}

			if (addsuccess == false) {

				groundingStateSilo.garbageSilo.insert(p);
			}
		}

		//executor->currentExpr = nullptr;

		//整合模式下
		if ((mode == MODE_DEFAULT || mode == MODE_INTEGRATE) && reasoning_success == true) {

			CodeTable& cdtdes_ = cdth->operator [](fkades.fileKey);
			CodeTable& cdtfrom_ =
				(groundingStateSilo.resultSilo.rbegin()->second->expr->expr_cdt);

			auto replaceinfo = cdtdes_.copyTree(fkades.addr);
			Addr mindes_ = get<1>(replaceinfo);
			Addr maxdes_ = get<2>(replaceinfo);
			if (cdtfrom_.addrdq.size() > 0 && (cdtfrom_.addrdq.front() <= mindes_ || (maxdes_ != addrnull && cdtfrom_.addrdq.back() >= maxdes_))) {

				cdtfrom_.resetFKARange(mindes_, maxdes_, fkades.fileKey);


				CodeTable& cdt_orginal = get<0>(replaceinfo);
				if (cdtfrom_.size() > 0 && cdtfrom_.codedq.back().assemblyFormula.flagBit.result_flag == S_Dz) {
					cdtfrom_.codedq.back().setArg(3, cdt_orginal.codedq.back()[3], false, false);
				}

				for (int i = 0; i < cdt_orginal.size(); ++i) {
					cdtdes_.eraseCode(cdt_orginal.addrdq[i], 0);
				}
			}



			SingleTFIntegrator stfi(&cdtdes_, &cdtfrom_, scpth,
				mindes_, maxdes_,
				cdtfrom_.addrdq[0],
				map<ArgForm, deque<ArgReal> >(), map<AddrCmp, AddrRef>());
			stfi.integrate();
		}
		//分离模式下
		else if (mode == MODE_SEPARATE) {
			*(this->seperateCdtDes) =
				groundingStateSilo.resultSilo.rbegin()->second->expr->expr_cdt;
		}


		//data collection process
		if (setting->train || setting->exportModelingData) {
			if (reasoning_success == false) {
				//未推理出的问题只标记绝对错误的部分，不迭代标记整个路径。因为这些路径的分支可能导向正确结果。
				for (auto& p_ : this->groundingStateSilo.garbageSilo) {
					GroundingState* gs_ = const_cast<GroundingState*>(p_.second);
					gs_->succeed = 0;

				}

				for (auto& p_ : this->groundingStateSilo.abortedSilo) {
					GroundingState* gs_ = const_cast<GroundingState*>(p_.second);
					gs_->succeed = 0;

				}

			}

			Strg dataset_filename;
			auto&& knowledge_domains = this->groundingStateSilo.getKnowledgeDomains();
			//knowledge_domains.erase(FILEKEYBUILTIN);

			for (auto knowledge_domain_ : knowledge_domains) {
				Strg knowledge_domain = split(knowledge_domain_, "#")[0];
				if (knowledge_domains__dataset_filename__map.count(
					{ knowledge_domain }) == 0) {

					if (this->setting->CollectionCycleSerialNumber < 0)
					{
						Strg s_;

						s_ += knowledge_domain;

						stringstream ss;
						hash<std::string> hasher;

						ss << getFormattedDate("yymmdd") << hasher(s_) << ".json";
						dataset_filename = ss.str();
						knowledge_domains__dataset_filename__map[{knowledge_domain}] =
							dataset_filename;

					}
					else
					{

						Strg s_;
						s_ += ("__" + knowledge_domain);
						//during test, hash is not needed
						/*stringstream ss;
						hash<std::string> hasher;

						ss << getFormattedDate("yymmdd") << hasher(s_) << ".json";
						dataset_filename = ss.str();*/
						dataset_filename = toStrg(this->setting->CollectionCycleSerialNumber) + s_ + ".json";
						knowledge_domains__dataset_filename__map[{knowledge_domain}] =
							dataset_filename;


					}

				}
				else {
					dataset_filename =
						knowledge_domains__dataset_filename__map[{knowledge_domain}];
				}

				this->groundingStateSilo.bufferDataset(dataset_filename, { knowledge_domain },
					setting->exportDuplicatedTrainingData,
					setting->exportSuccessTrainingData, setting->exportFailureTrainingData, cdth, systh);
			}

		}
		if (reasoning_success == false) {

#if _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				stringstream ss;
				auto rit = groundingStateSilo.garbageSilo.rbegin();
				auto ritaborted = groundingStateSilo.abortedSilo.rbegin();
				ss << bbhline;
				ss << "Error exit:" << __FUNCTION__ << ":\n";
				ss << "CodeTable:\n:" << this->scpth->cdth->toStrg() << bhline;
				ss << "groundingStateSiloMaxCapacity:" << groundingStateSiloMaxCapacity << endl;
				ss << "siloAll:size:" << groundingStateSilo.siloAll.size() << "\n";
				int i = 0;
				while (++i < min(100, groundingStateSilo.garbageSilo.size()) && ((++rit)->second) != nullptr) {
					ss << bhline;
					ss << "garbageGroundingState:" << rit->first << "\n";
					ss << rit->second->toStrg(this->scpth) << endl;
				}
				if (groundingStateSilo.abortedSilo.size() > 0) {
					ss << "abortedSilo:" << ritaborted->first << "\n";
					ss << bhline;
					ss << "abortedGroundingState:" << ritaborted->first << "\n";
					ss << tfth->operator[](ritaborted->second->tffka).toStrg() << endl;
					ss << ritaborted->second->toStrg(this->scpth) << endl;
					ss << "abortedGSAR:\n";
					ss << ritaborted->second->expr->ruleFunBodyAR->toStrg();
					i = 0;
					while (++i < min(100, groundingStateSilo.abortedSilo.size()) && ((++ritaborted)->second) != nullptr) {
						ss << bhline;
						ss << "abortedGroundingState:" << ritaborted->first << "\n";
						ss << tfth->operator[](ritaborted->second->tffka).toStrg() << endl;
						ss << ritaborted->second->toStrg(this->scpth) << endl;
						ss << "abortedGSAR:\n";
						ss << ritaborted->second->expr->ruleFunBodyAR->toStrg();
					}

				}
				ss << bbhline;
				std::string info = ss.str(); // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
				OutputDebugString(winfo.c_str());
				cout << info << endl;
				fstream f;
				f.open("important.txt");
				if (f.is_open()) {
					f << info << endl;
					f.close();
				}
				ignoreCount++;
			}
#endif
			executor->toBrief();
			THROW_REASONING_EXCEPTION(ReasoningErrorType::NoMoreSpacesToExplore, "Reasoning failure! No more space to explore! Task abort!");
			assert(false);

		}
		return true;
	}


	StateMatrix& StateMatrix::operator=(const StateMatrix& sm) {
		rule_domain_add_stack = sm.rule_domain_add_stack;
		tfinfo_add_stack = sm.tfinfo_add_stack;
		knowledge_domain_list = sm.knowledge_domain_list;
		step_list = sm.step_list;
		matrix = sm.matrix;
		BDDB_o_t_list = sm.BDDB_o_t_list;
		accumulated_steps = sm.accumulated_steps;
		return *this;
	}
	set<Strg> StateMatrix::getknowledgeDomains() const
	{
		return set<Strg>(knowledge_domain_list.begin(), knowledge_domain_list.end());

	}
	Intg StateMatrix::getStep(const Strg& knowledge_domain) const {
		// Find the position of the knowledge domain in the list
		auto it = std::find(knowledge_domain_list.begin(), knowledge_domain_list.end(), knowledge_domain);

		// If found, return the corresponding step
		if (it != knowledge_domain_list.end()) {
			size_t index = std::distance(knowledge_domain_list.begin(), it);
			return step_list[index];
		}

		// If not found, return -1
		return -1;
	}

	/**
		 *
		 * @param rule domain, file_class_name p1 函数所在文件名，p2 函数所在类名, only the most accurate values are recorded.(if a rule is in one class, only the filename& classname combined name will be recorded)
		 * @param tfinfo 即为 与函数绑定的info。注意当前只用info的第一层
		 * @return bool 1是否成功，int 2 BDDB_o_t的值（相同阶段逗留步数）, Numb 3 rp(reward provided by PCP)， intg 4 对应知识领域所到达的阶段
		 */
	tuple<bool, Intg, Numb, Intg> StateMatrix::add(const pair<Strg, Strg>& file_class_name,
		const TFInfoMat& tfinfo) {
		bool addsucceed = false;
		Intg advance = 0;
		Numb rp = toNumb(BUILTIN_FUN_INFO);
		auto&& knowledge_domain = split(file_class_name.first, "#")[0]
			+ (file_class_name.second != "" ? " " : "")
			+ file_class_name.second;
		const auto& funrewardv = tfinfo.list2[0];
		step_list;
		auto addtrack = [this, &knowledge_domain, &funrewardv]() {
			this->rule_domain_add_stack.push_back(knowledge_domain);
			this->tfinfo_add_stack.push_back(funrewardv); };

		Intg i = -1;
		auto it_knowledge_domain = knowledge_domain_list.begin();
		for (; it_knowledge_domain != knowledge_domain_list.end(); ++it_knowledge_domain) {
			++i;
			if (it_knowledge_domain.operator *() == knowledge_domain) {

				break;
			}
		}
		//之前没有属于knowledge domain的func被匹配
		if (it_knowledge_domain == knowledge_domain_list.end()) {
			//            matrix.list2[0] = tfinfo.list2[0];

			knowledge_domain_list.push_back(knowledge_domain);
			Intg step = 0;
			auto it = funrewardv.begin();
			for (; it != funrewardv.end(); ++it) {
				if (it.operator *() != 0) {
					rp = it.operator *();
					break;
				}
				++step;
			}
			matrix.list2.push_back(deque<Numb>(funrewardv.size(), 0));
			matrix.list2.back()[step] = funrewardv[step];
			if (it != funrewardv.end()) {
				step_list.push_back(step);
			}
			else {
				step_list.push_back(0);
			}
			BDDB_o_t_list.push_back(0);
			this->accumulated_steps += 1;

			addtrack();
			return make_tuple(true, step, rp, step_list.back());
		}
		else
		{
			//之前已经有属于knowledge domain 的函数被匹配 时

			Intg step = step_list[i];
			Intg& BDDB_o_t = BDDB_o_t_list[i];
			if (funrewardv.size() > 0 && step >= funrewardv.size()) {
				//the function cannot be applied at the step out of its chain length
				return make_tuple(false, 0, 0, 0);

			}
			auto it = funrewardv.begin() + step;
			for (; it != funrewardv.end(); ++it) {
				if (it.operator *() != 0) {
					rp = it.operator *();
					break;
				}
				++step;

			}
			if (it == funrewardv.end()) {
				if (funrewardv.size() == 0) {
					//the function has no reward, therefore can be applied at any step
					BDDB_o_t += 1;
					this->accumulated_steps += 1;
					addtrack();
					return make_tuple(true, BDDB_o_t, rp, step_list[i]);

				}
				else {
					//the function cannot applied at this step, add failure!!
					return make_tuple(false, 0, 0, 0);
				}

			}
			else {
				//add successfully



				if (step > step_list[i]) {
					//making progress 
					step_list[i] = step;
					BDDB_o_t = 0;

				}
				else {
					//not making progress
					BDDB_o_t++;

				}

				auto& matrix_i = matrix[i];
				if (matrix_i.size() < funrewardv.size()) {
					matrix_i.insert(matrix_i.end(), funrewardv.size() - matrix_i.size(), 0);
				}
				matrix_i[step] += funrewardv[step];
			}

			accumulated_steps += 1;
			addtrack();
			return make_tuple(true, BDDB_o_t, rp, step_list[i]);
		}
	}

	Strg StateMatrix::toStrg() const {
		/*json j;
		j["StateMatrix"]["n"] = n;
		j["StateMatrix"]["matrix"] = json::array();
		for (int i = 0; i < (int)knowledge_domain_list.size(); i++) {
			json jit;
			jit["knowledge_domain"] = knowledge_domain_list[i];
			auto vc = matrix.list2[i];
			jit["vector"] = vc;
			j["StateMatrix"]["matrix"].push_back(jit);
		}
		j["BDDB_o_t"] = BDDB_o_t;
		j["accumulated_steps"] = accumulated_steps;
		return j.dump(4);*/

		stringstream ss;
		ss << calign("knowledge domain", 24) << calign("matrix", 32) << calign("step", 16) << calign("BDDB_o_t", 16) << endl;
		for (int i = 0; i < knowledge_domain_list.size(); ++i) {
			ss << calign(knowledge_domain_list[i], 24) << calign(COOLANG::toStrg<deque<Numb> >(matrix.list2[i]), 32) << calign(COOLANG::toStrg(step_list[i]), 16) << calign(COOLANG::toStrg(BDDB_o_t_list[i]), 16) << endl;
		}
		ss << calign("accumulated_step", 16) << calign("tf knowledge domain", 24) << calign("tfInfoMat", 32) << endl;
		for (int i = 1; i <= accumulated_steps; ++i) {
			ss << calign(COOLANG::toStrg(i), 16) << calign(this->rule_domain_add_stack[i - 1], 24) << calign(COOLANG::toStrg<>(this->tfinfo_add_stack[i - 1]), 32) << endl;

		}
		ss << hdot;
		return ss.str();

	}
	/*******************************

		MatchState

	****************************/
	MatchState::MatchState(CodeTable* cdtcmp, const Addr& addrcmp, CodeTable* cdtref,
		const Addr& addrref, MatchMode md /*= MatchMode::RuleMatchingFormalRealBind*/, DataTablePtrW dt/* = nullptr*/, CodeTableHash* cdth/* = nullptr*/, set<Arg, ArgComparatorLT> placeholders) {
		this->cdtcmp = cdtcmp;
		this->cdtref = cdtref;
		this->addrcmporg = addrcmp;
		this->addrreforg = addrref;
		this->md = md;
		this->dt = dt;
		this->cdth = cdth;
		this->placeholders = placeholders;
	}
	MatchState::MatchState(const MatchState& ms) {
		cdtcmp = ms.cdtcmp;
		cdtref = ms.cdtref;
		addrcmpv = ms.addrcmpv;
		argposcmpv = ms.argposcmpv;
		addrrefv = ms.addrrefv;
		argposrefv = ms.argposrefv;
		cram = ms.cram;
		rcam = ms.rcam;
		pcam = ms.pcam;
		addrm = ms.addrm;
		addrcmporg = ms.addrcmporg;
		addrreforg = ms.addrreforg;
		md = ms.md;
		dt = ms.dt;
		cdth = ms.cdth;
		placeholders = ms.placeholders;
	}

	MatchState& MatchState::operator=(const MatchState& ms) {
		cdtcmp = ms.cdtcmp;
		cdtref = ms.cdtref;
		addrcmpv = ms.addrcmpv;
		argposcmpv = ms.argposcmpv;
		addrrefv = ms.addrrefv;
		argposrefv = ms.argposrefv;
		cram = ms.cram;
		rcam = ms.rcam;
		pcam = ms.pcam;
		addrm = ms.addrm;
		addrcmporg = ms.addrcmporg;
		addrreforg = ms.addrreforg;
		md = ms.md;
		dt = ms.dt;
		cdth = ms.cdth;
		placeholders = ms.placeholders;
		return (*this);
	}

	bool MatchState::finished() const {
		return argposrefv.size() == 0 && addrm.size() > 0;
	}

	void MatchState::pop(AddrCmp& addrcmp_, Intg& argposcmp, Addr& addrref_,
		Intg& argposref) {
		addrcmp_ = addrcmpv.back();
		argposcmp = argposcmpv.back();
		addrref_ = addrrefv.back();
		argposref = argposrefv.back();
		addrcmpv.pop_back();
		argposcmpv.pop_back();
		addrrefv.pop_back();
		argposrefv.pop_back();
	}

	void MatchState::push(const AddrCmp& addrcmp, const Intg& argposcmp,
		const AddrRef& addrref, const Intg& argposref) {
		addrcmpv.push_back(addrcmp);
		argposcmpv.push_back(argposcmp);
		addrrefv.push_back(addrref);
		argposrefv.push_back(argposref);

	}
	Intg MatchState::size() const {
		return argposcmpv.size();
	}


	/**
		 * @brief 比较两个参数是否在事实上等价
		 * @param argcmp
		 * @param argref
		 * @return
		 */
	bool MatchState::sameOneArg(const ArgCmp& argcmp, const ArgRef& argref) const {
		if (/*argref.formalArg == argref.formalArg &&*/argcmp.asc == argref.asc
			&& argcmp == argref) {
			return true;
		}
		return false;
	}
	bool MatchState::add(const AddrCmp& addrcmp, const AddrRef& addrref) {
		if (addrref > addrnull) {
			if (addrm[addrcmp] == addrref) {
				return true;
			}
			else if (addrm[addrcmp] == addrnull) {
				addrm[addrcmp] = addrref;
				return true;
			}
			else {
				return false;
			}
		}
		else if (addrref < addrnull) {
			addrm[addrcmp] = addrref;
			return true;
		}
		else {
			return false;
		}
	}

	bool MatchState::sameTreeStruct(const ArgCmp& root1, const ArgCmp& root2,
		const AddrCmp& rootaddr1, const AddrCmp& rootaddr2) {
		//todo:: logic here
		if (root1.argFlag != root2.argFlag) {
			return false;
		}
		if (root1.argFlag == M_Bs && root2.argFlag == M_Bs) {
			return root1 == root2;
		}
		if (root1.argFlag == Sz && root2.argFlag == Sz) {
			return root1.arg_i == root2.arg_i;
		}
		if (root1.argFlag == Zf && root2.argFlag == Zf) {
			return root1.arg_s == root2.arg_s;
		}
		if (root1.argFlag == S_Bs && root2.argFlag == S_Bs) {
			return sameOneArg(root1, root2);
		}
		else {
			//root1.argFlag ==S_Dz && root2.argFlag == S_Dz
			if (root1.arg_fka.addr == addrnull && root2.arg_fka.addr == addrnull) {
				return true;
			}
			Addr leafaddr1 = rootaddr1;
			while (!((*cdtcmp)[leafaddr1][3] == root1)) {
				leafaddr1 = cdtcmp->getSameTrBranchLastArgAddrIgnoreProperty(
					leafaddr1, root1, acm::name | acm::asc);
				if (leafaddr1 == addrnull) {

					cout
						<< "sameTreeStruct err: can't find leafaddr1 definition, exit(-1)\n"
						"\trootaddr1:[" << rootaddr1.toStrg()
						<< "\t]\n"
						"\troot1:[" << root1.toStrg() << "\t]\n"
						"rootaddr2:[" << rootaddr2.toStrg()
						<< "\t]\n"
						"root2:[" << root2.toStrg() << "\t]\n"
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
			Addr leafaddr2 = rootaddr2;
			while (!((*cdtcmp)[leafaddr2][3] == root2)) {
				leafaddr2 = cdtcmp->getSameTrBranchLastArgAddrIgnoreProperty(
					leafaddr2, root2, acm::name | acm::asc);
				if (leafaddr2 == addrnull) {

					cout
						<< "sameTreeStruct err: can't find leafaddr2's definition\n"
						"\trootaddr1:[" << rootaddr1.toStrg()
						<< "\t]\n"
						"\troot1:[" << root1.toStrg() << "\t]\n"
						"rootaddr2:[" << rootaddr2.toStrg()
						<< "\t]\n"
						"root2:[" << root2.toStrg() << "\t]\n"
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
			Code& leafcode1 = (*cdtcmp)[leafaddr1];
			Arg&& leaf1_0 = leafcode1[0];
			Arg&& leaf1_1 = leafcode1[1];
			Arg&& leaf1_2 = leafcode1[2];
			Code& leafcode2 = (*cdtcmp)[leafaddr2];
			Arg&& leaf2_0 = leafcode2[0];
			Arg&& leaf2_1 = leafcode2[1];
			Arg&& leaf2_2 = leafcode2[2];
			bool same0 = false;
			same0 = sameTreeStruct(leaf1_0, leaf2_0, leafaddr1, leafaddr2);
			if (!same0) {
#if debug
				{
					static int ignorecount = 0;
					cout
						<< "sameTreeStruct(const ArgCmp&, const ArgCmp&, const AddrCmp&, const AddrCmp&)"
						<< " "
						"same1 false \n"
						"leaf1_0:[" << leaf1_0.toStrg() << "\t]\n"
						"leaf2_0:[" << leaf2_0.toStrg() << "\t]\n"
						"leafaddr1:[" << leafaddr1.toStrg()
						<< "\t]\tleafaddr2:[" << leafaddr2.toStrg()
						<< "\t]\t\n" << "\tignorecount:[" << ignorecount++
						<< "\t](" << __FILE__ << ":" << __LINE__ << ":0"
						<< ")" << endl;
				}
#endif
				return false;
			}
			bool same1 = false;
			same1 = sameTreeStruct(leaf1_1, leaf2_1, leafaddr1, leafaddr2);
			if (!same1) {
#if debug
				{
					static int ignorecount = 0;
					cout
						<< "sameTreeStruct(const ArgCmp&, const ArgCmp&, const AddrCmp&, const AddrCmp&)"
						<< " "
						"same1 false \n"
						"leaf1_0:[" << leaf1_1.toStrg() << "\t]\n"
						"leaf2_0:[" << leaf2_1.toStrg() << "\t]\n"
						"leafaddr1:[" << leafaddr1.toStrg()
						<< "\t]\tleafaddr2:[" << leafaddr2.toStrg()
						<< "\t]\t\n" << "\tignorecount:[" << ignorecount++
						<< "\t](" << __FILE__ << ":" << __LINE__ << ":0"
						<< ")" << endl;
				}
#endif
				return false;
			}
			bool same2 = false;
			same2 = sameTreeStruct(leaf1_2, leaf2_2, leafaddr1, leafaddr2);
			if (!same2) {
#if debug
				{
					static int ignorecount = 0;
					cout
						<< "sameTreeStruct(const ArgCmp&, const ArgCmp&, const AddrCmp&, const AddrCmp&)"
						<< " "
						"same1 false \n"
						"leaf1_0:[" << leaf1_2.toStrg() << "\t]\n"
						"leaf2_0:[" << leaf2_2.toStrg() << "\t]\n"
						"leafaddr1:[" << leafaddr1.toStrg()
						<< "\t]\tleafaddr2:[" << leafaddr2.toStrg()
						<< "\t]\t\n" << "\tignorecount:[" << ignorecount++
						<< "\t](" << __FILE__ << ":" << __LINE__ << ":0"
						<< ")" << endl;
				}
#endif
				return false;
			}
			return true;

		}

		return false;
	}
	/**
		 * @brief 这个函数用以检查形参的一致性,即在添加argcmp与argref之前确保argref对应的argcmp为空。
		 * 即添加后每个非兼容（?arg/arg.compatible == true）形参(ArgRef)最多只能对应一个实参(ArgCmp)。
		 * 如果一个形参对应多个实参，那么这些多的实参是相同的参数，那么将删除未重复的（未被引用的树）树。
		 * 如果实参不相同则返回假，如果重复树被引用则不删除。
		 * @change 此函数单独维护rcam，不会修改cram
		 * @param
		 * @return
		 */
	bool MatchState::checkArgRefConsistency(const ArgCmp& argcmp,
		const ArgRef& argref, const Addr& addrcmp, const Addr& addrref) {
#if debug
			{
				static int ignorecount = 0;
				cout
					<< "checkArgRefConsistency(const ArgCmp&, const ArgRef&, const Addr&, const Addr&)"
					<< " in " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
			auto argRefIt = rcam.find(argref);
			if (argRefIt == rcam.end() || argRefIt->second.first == argnull) {
				rcam[argref] = pair<ArgCmp, AddrCmp>(argcmp, addrcmp);
				return true;
			}
			else {
				ArgCmp& argcmp2 = argRefIt->second.first;
				AddrCmp& addrcmp2 = argRefIt->second.second;
				if (sameOneArg(argcmp, argcmp2) == true) {
					return true;

				}
				else if (sameTreeStruct(argcmp, argcmp2, addrcmp, addrcmp2)
					== true) {
#if debug
						{
							static int ignorecount = 0;
							cout
								<< "checkArgRefConsistency(const ArgCmp&, const ArgRef&, const Addr&, const Addr&)"
								<< " sameTreeStruct(argcmp, argref, addrcmp, addrref)== true"
								<< "\tignorecount:[" << ignorecount++ << "\t]("
								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
								<< endl;
						}
#endif
						argRefIt->second = pair<ArgCmp, AddrCmp>(argcmp, addrcmp);
						return true;

				}
				else if (argref.compatible) {
					//the same argref can match different argcmp sharing the same constraint
					return true;
				}
				else {
					return false;

				}
			}

			return false;

	}
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
	const bool MatchState::add(const ArgCmp& argcmp, const ArgRef& argref,
		const Addr& addrcmp, const Addr& addrref) {

		if (argcmp.argFlag == M_Bs || argref.argFlag == M_Bs) {

			return argcmp.argFlag == argref.argFlag && argcmp.arg_s == argref.arg_s;
		}

		if ((static_cast<int>(md) & static_cast<int>(MatchMode::RuleMatchingFormalRealBind)) > 0) {


			acm argcmpmd = acm::ref | acm::placeholder | acm::constraint | acm::or_data | acm::changeable;

			if (Arg::intersect(argcmp, argref, argcmpmd, this->dt, this->cdth) && checkArgRefConsistency(argcmp, argref, addrcmp,
				addrref)) {
				if (find(argref.constraints.begin(), argref.constraints.end(), Constraint("not", { ConstraintArg(X_Bs,"immediate") })) != argref.constraints.end() &&
					find(argcmp.constraints.begin(), argcmp.constraints.end(), Constraint("immediate")) != argcmp.constraints.end()
					) {
					return false;
				}

				if (find(argref.constraints.begin(), argref.constraints.end(), Constraint("immediate")) != argref.constraints.end() &&
					find(argcmp.constraints.begin(), argcmp.constraints.end(), Constraint("immediate")) == argcmp.constraints.end()
					) {
					return false;
				}
				if (cram[argcmp].size() == 0) {
					cram[argcmp].push_back(Arg());
					cram[argcmp][0] = argref;
				}
				else {
					cram[argcmp].push_back(argref);
				}
				if (argref.isPlaceholder == true && argref.isBound == false) {
					if (pcam.find(argcmp) != pcam.end()) {
						auto& argcmporg = pcam[argref];
						if (Arg::intersect(argcmporg, argcmp)) {
							return true;
						}
						else {
							return false;
						}
					}
					else {
						pcam[argref] = argcmp;
					}
				}
				return true;

			}
			else {
				return false;
			}
#if debug
			{
				static int ignorecount = 0;
				cout
					<< "add(const ArgCmp&, const ArgRef&, const Addr&, const Addr&)"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
#if debug
			cout << "Matcher::add \n\targcmp:[" << argcmp.toStrg() << "\targref:["
				<< argref.toStrg() << endl;
#endif

		}
		else if ((static_cast<int>(md) & static_cast<int>(MatchMode::FunctionInvocationFormalRealBind)) > 0) {


			acm argcmpmd = acm::ref | acm::placeholder | acm::constraint | acm::or_data | acm::changeable;

			if (Arg::intersect(argcmp, argref, argcmpmd, this->dt, this->cdth) && checkArgRefConsistency(argcmp, argref, addrcmp,
				addrref)) {
				/*if (find(argref.constraints.begin(), argref.constraints.end(), Constraint("not", { ConstraintArg(X_Bs,"immediate") })) != argref.constraints.end() &&
					find(argcmp.constraints.begin(), argcmp.constraints.end(), Constraint("immediate")) != argcmp.constraints.end()
					) {
					return false;
				}

				if (find(argref.constraints.begin(), argref.constraints.end(), Constraint("immediate")) != argref.constraints.end() &&
					find(argcmp.constraints.begin(), argcmp.constraints.end(), Constraint("immediate")) == argcmp.constraints.end()
					) {
					return false;
				}*/
				if (cram[argcmp].size() == 0) {
					cram[argcmp].push_back(Arg());
					cram[argcmp][0] = argref;
				}
				else {
					cram[argcmp].push_back(argref);
				}
				if (argref.isPlaceholder == true && argref.isBound == false) {
					if (pcam.find(argcmp) != pcam.end()) {
						auto& argcmporg = pcam[argref];
						if (Arg::intersect(argcmporg, argcmp)) {
							return true;
						}
						else {
							return false;
						}
					}
					else {
						pcam[argref] = argcmp;
					}
				}
				return true;

			}
			else {
				return false;
			}


		}
		else if ((static_cast<int>(md) & static_cast<int>(MatchMode::SubExprSearchingPlaceholderBind)) > 0)
		{

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "Mode::SubExprSearchingPlaceholderBind"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif

#if debug
			cout << "Matcher::add \n\targcmp:[" << argcmp.toStrg() << "\targref:["
				<< argref.toStrg() << endl;
#endif


			acm argcmpmd = acm::ref | acm::placeholder | acm::constraint | acm::identifier | acm::symbol | acm::or_data;

			if (Arg::intersect(argcmp, argref, argcmpmd, this->dt, this->cdth) && checkArgRefConsistency(argcmp, argref, addrcmp,
				addrref)) {
				if (cram[argcmp].size() == 0) {
					cram[argcmp].push_back(Arg());
					cram[argcmp][0] = argref;
				}
				else {
					cram[argcmp].push_back(argref);
				}
				if (argref.isPlaceholder == true && argref.isBound == false) {
					if (pcam.find(argcmp) != pcam.end()) {
						auto& argcmporg = pcam[argref];
						if (Arg::intersect(argcmporg, argcmp)) {
							return true;
						}
						else {
							return false;
						}
					}
					else {
						pcam[argref] = argcmp;
					}
				}
				return true;
			}
			else {
				return false;
			}


		}







	}
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
	bool MatchState::compareSingleCode(const AddrCmp& addrcmp, const Code& cdcmp,
		const AddrRef& addrref, const Code& cdref) {
		if ((static_cast<int>(this->md) & static_cast<int>(MatchMode::RuleMatchingFormalRealBind)) > 0) {
			if (cdcmp.assemblyFormula.flagBit.tfFKA != fkanull) {
				return false;
			}
		}

#if debug && _WIN32
		{
			stringstream ss;
			ss << bbhline;
			ss << __FUNCTION__ << endl;
			ss << "codecmp:\t" << calign(addrcmp.toStrg()) << cdcmp.toBrief() << endl;
			ss << "coderef:\t" << calign(addrref.toStrg()) << cdref.toBrief() << endl;
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

		if (addrref == addrnull) {
			return true;
		}
		else if (addrref < addrnull) {

#if debug
			static int ignorecount = 0;
			cout << "Matcher::compareSingleCode ignorecount:[" << ignorecount++
				<< "]\taddrref < addrnull,addrcmp:[" << addrcmp.toStrg()
				<< "]\taddrref:[" << addrref.toStrg() << "]" << endl;
#endif
		}
		else if (addrref > addrnull) {

			if (addrm[addrcmp] == addrref) {
				return true;

			}
			else if (add(addrcmp, addrref) == false) {
#if debug
				cout << "Matcher::compareSingleCode err,\naddrcmp:["
					<< addrcmp.toStrg() << "]\tcdcmp:[" << cdcmp.toStrg()
					<< "]\naddrref:[" << addrref.toStrg() << "]\tcdref:["
					<< cdref.toStrg() << "]" << endl;
#endif
				return false;
			}

		}
		else {

			cout << "MatchState::compareSingleCode err::addrref compare err"
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

		ArgCmp&& argcmp1 = cdcmp[0];
		ArgCmp&& argcmp2 = cdcmp[1];
		ArgCmp&& operatorcmp = cdcmp[2];
		ArgCmp&& resultcmp = cdcmp[3];
		argcmp1.isPlaceholder = this->placeholders.count(argcmp1);
		argcmp2.isPlaceholder = this->placeholders.count(argcmp2);
		resultcmp.isPlaceholder = this->placeholders.count(resultcmp);

		//        if (argcmp1.argFlag == Y_Dz) {
		//            argcmp1 = globalRefTable[argcmp1];
		//        }
		//        if (argcmp2.argFlag == Y_Dz) {
		//            argcmp2 = globalRefTable[argcmp2];
		//        }
		//        if (operatorcmp.argFlag == Y_Dz) {
		//            operatorcmp = globalRefTable[operatorcmp];
		//        }
		//        if (resultcmp.argFlag == Y_Dz) {
		//            resultcmp = globalRefTable[resultcmp];
		//        }
#if (debug || filelog) && _WIN32 
		if (debugflag && TemplateFunctionTableHash::getTFNameByFKA(FKA(FILEKEYBUILTIN, addrref)) == operatorcmp.arg_s) {
			static int ignoreCount = 0;
			{
				stringstream ss;
				ss << bhline;
				ss << __FUNCTION__ << "::addrref < addrnull\n";
				ss << "addrcmp:" << addrcmp.toStrg() << "\t\t" << "addrref:" << addrref.toStrg() << "\n";
				ss << "cdcmp:\t" << cdcmp.toStrg() << endl;
				ss << "cdref:\t" << cdref.toStrg() << endl;
				ss << bhline;

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
		if (addrref < addrnull) {
			bool compareresult = false;
			switch (addrref[0]) {
			case S_S_JIA:
				compareresult =
					(operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "+")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_JIAN:
				compareresult =
					(operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "-")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_CHENG:
				compareresult =
					(operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "*")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_CHU:
				compareresult =
					(operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "/")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_FUZHI:
				compareresult =
					argcmp1.isVar()
					&& (operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "=")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_DENGYU:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "==")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_BUDENG:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "!=")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_DAYU:
				compareresult =
					(operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == ">")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_XIAOYU:
				compareresult =
					(operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "<")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_BUDAYU:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "<=")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_BUXIAOYU:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == ">=")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_MI:
				compareresult =
					(operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "^")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
				//            //CALL is an operator that has no practical meaning.
				//            case CALL:
				//
				//                compareresult =
				//                        ((operatorcmp.arg_s == "call"
				//                                || operatorcmp.arg_s == "ca"))
				//                                && ((argcmp1.changeable != T_
				//                                        && argcmp2.changeable != T_));
				//                break;
			case SHUCHU:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "-->")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case LST_ACCESS:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "[]")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case LENGTH:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "LENGTH")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case CLEAR:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "CLEAR")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case ERASE:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "ERASE")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case TYPENAME:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "TYPENAME")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_AND:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "&&")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_OR:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "||")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_NOT:
				compareresult =
					(operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "!"
						&& argcmp2.changeable != T_);
				break;
			case S_S_MODULO:
				compareresult =
					(operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "%")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_JIADENG:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "+=")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_JIANDENG:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "-=")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_CHENGDENG:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "*=")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_CHUDENG:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "/=")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_MODENG:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "%=")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_S_MIDENG:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "^=")
					&& (argcmp1.changeable != T_
						&& argcmp2.changeable != T_);
				break;
			case S_SI:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "++" && argcmp1.argFlag == S_Bs
					&& argcmp1.changeable != T_);
				break;
			case S_SD:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "--" && argcmp1.argFlag == S_Bs
					&& argcmp1.changeable != T_);
				break;
			case S_MAP:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "MAP");
				break;
			case S_MULTIMAP:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "MULTIMAP");
				break;
			case S_SET:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "SET");
				break;
			case S_MULTISET:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "MULTISET");
				break;
			case S_TONUM:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "TONUM"
					&& argcmp1.changeable != T_);
				break;
			case S_TOSTRG:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "TOSTRG"
					&& argcmp1.changeable != T_);
				break;
			case S_TOINT:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "TOINT"
					&& argcmp1.changeable != T_);
				break;
			case S_NONBLOCKEXECUTE:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "NONBLOCKEXECUTE"
					&& argcmp1.changeable != T_);
				break;
			case S_BLOCKEXECUTE:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "BLOCKEXECUTE"
					&& argcmp1.changeable != T_);
				break;
			case S_SLEEP:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "SLEEP"
					&& argcmp1.changeable != T_);
				break;
			case S_LOGICJUMP:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "LOGICJUMP"
					&& argcmp1.changeable != T_);
				break;
			case S_FIND:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "FIND"
					&& argcmp1.changeable != T_);
				break;
			case S_COUNT:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "COUNT"
					&& argcmp1.changeable != T_);
				break;
			case S_INSERT:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "INSERT"
						&& (argcmp1.changeable != T_
							&& argcmp2.changeable != T_));
				break;
			case S_PUSHBACK:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "PUSHBACK"
						&& (argcmp1.changeable != T_
							&& argcmp2.changeable != T_));
				break;
			case S_PUSHFRONT:
				compareresult =
					(operatorcmp.argFlag == M_Bs
						&& operatorcmp.arg_s == "PUSHFRONT"
						&& (argcmp1.changeable != T_
							&& argcmp2.changeable != T_));
				break;
			case S_POPBACK:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "POPBACK"
					&& argcmp1.changeable != T_);
				break;
			case S_POPFRONT:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "POPFRONT"
					&& argcmp1.changeable != T_);
				break;
			case S_BACK:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "BACK"
					&& argcmp1.changeable != T_);
				break;
			case SI_S:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "++" && argcmp2.argFlag == S_Bs
					&& argcmp2.changeable != T_);
				break;
			case SD_S:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "--" && argcmp2.argFlag == S_Bs
					&& argcmp2.changeable != T_);
				break;
			case S_S_FIND_SUBEXPR:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "FIND_SUBEXPR");
				break;
			case S_S_EXIST_SUBEXPR:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "EXIST_SUBEXPR");
				break;
			case S_RESET:
				compareresult = (operatorcmp.argFlag == M_Bs
					&& operatorcmp.arg_s == "RESET");
				break;

				//case COMMA:
				//	/**todo::如果要支持多态，请在预编译时对函数名称进行变形
				//	 * 将逗号表达式纳入匹配项会导致原来对多态的自动支持不可用
				//	 */
				// 由于逗号表达式无实际意义，故不将其作为可以匹配的内置函数
				//	compareresult = (operatorcmp.argFlag == M_Bs
				//		&& operatorcmp.arg_s == "COMMA");
				break;
			default:
				break;
			}
			if (compareresult == true) {
				add(addrcmp, addrref);
				return true;
			}
			else {
				return false;
			}

		}
		ArgRef&& argref1 = cdref[0];
		ArgRef&& argref2 = cdref[1];
		ArgRef&& operatorref = cdref[2];
		ArgRef&& resultref = cdref[3];
		argref1.isPlaceholder = this->placeholders.count(argref1);
		argref2.isPlaceholder = this->placeholders.count(argref2);
		resultref.isPlaceholder = this->placeholders.count(resultref);

		if (addrcmp != addrnull && addrref > addrnull) {

#if (debug || filelog) && _WIN32 
			if (debugflag && operatorref.arg_s == operatorcmp.arg_s) {
				static int ignoreCount = 0;
				{
					stringstream ss;
					ss << bhline;
					ss << __FUNCTION__ << "::addrref > addrnull\n";
					ss << "addrcmp:" << addrcmp.toStrg() << "\t\t" << "addrref:" << addrref.toStrg() << "\n";
					ss << "cdcmp:\t" << cdcmp.toStrg() << endl;
					ss << "cdref:\t" << cdref.toStrg() << endl;
					ss << bhline;

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

			bool add1 = add(argcmp1, argref1, addrcmp, addrref);
			if (add1 == false) {
#if debug
				cout << "add1false:addrcmp:[" << addrcmp.toStrg()
					<< "]\tadref:[" << addrref.toStrg() << "]"
					<< "\n\targcmp1:[" << argcmp1.toStrg() << "]"
					<< "\n\targref1:[" << argref1.toStrg() << "]"

					<< endl;
#endif

			}

			bool add2 = add(argcmp2, argref2, addrcmp, addrref);
			if (add2 == false) {
#if debug
				cout << "add2false:addrcmp:[" << addrcmp.toStrg()
					<< "]\tadref:[" << addrref.toStrg() << "]"
					<< "\n\targcmp2:[" << argcmp2.toStrg() << "]"
					<< "\n\targref2:[" << argref2.toStrg() << "]" << endl;
#endif
			}

			bool addop = add(operatorcmp, operatorref, addrcmp, addrref);

			if (addop == false) {
#if debug
				cout << "addopfalse:addrcmp:[" << addrcmp.toStrg()
					<< "]\tadref:[" << addrref.toStrg() << "]"
					<< "\n\toperatorcmp:[" << operatorcmp.toStrg() << "]"
					<< "\n\toperatorref:[" << operatorref.toStrg() << "]"
					<< endl;
#endif
			}
			bool addre = add(resultcmp, resultref, addrcmp, addrref);
			if (addre == false) {
#if debug
				cout << "addrefalse:addrcmp:[" << addrcmp.toStrg()
					<< "]\tadref:[" << addrref.toStrg() << "]"

					<< "\n\tresultcmp:[" << resultcmp.toStrg() << "]"
					<< "\n\tresultref:[" << resultref.toStrg() << "]"
					<< endl;
#endif
			}
			if (add1 && add2 && addop && addre) {
				push(addrcmp, 3, addrref, 3);
				push(addrcmp, 1, addrref, 1);
				push(addrcmp, 0, addrref, 0);

				return true;
			}
			else {
#if debug
				static Intg ignorecount = 0;
				cout << "addfalse:addrcmp:[" << addrcmp.toStrg()
					<< "]\taddrref:[" << addrref.toStrg() << "]"

					<< "\n\tresultcmp:[" << resultcmp.toStrg() << "]"
					<< "\n\tresultref:[" << resultref.toStrg() << "]"
					<< "\n\tignorecount:[" << ignorecount++ << "]" << endl;
#endif
				return false;
			}

		}
		//{

		/*cout << "MatchState::compareSingleCode err"
			"\n\t addrcmp:[" << addrcmp.toStrg() << "]\taddrref:["
			<< addrref.toStrg() << "]"
			"\n\tcdcmp::" << cdcmp.toStrg() << "\n\tcdref::"
			<< cdref.toStrg() << endl;*/
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
		throw(-1);

		//}
	}


	void GroundingState::checkExprCdt() {
		GroundingState& gs = *this;
		if (gs.expr != nullptr && gs.expr->type == EXPRESSIONHANDLER_CDT && gs.expr->expr_cdt.size() > 0 && gs.tffka != fkanull) {
			Addr tfroot = gs.expr->expr_cdt.addrdq[gs.position];
			gs.expr->expr_cdt.checkFileKey();
			gs.expr->expr_cdt.deleteInaccessibleSubtree();
			gs.expr->expr_cdt.unfoldRingStruct();
			gs.expr->expr_cdt.checkChangeable();
			auto it = gs.expr->expr_cdt.addrdq.begin();
			for (; it != gs.expr->expr_cdt.addrdq.end(); ++it) {
				if (it->operator==(tfroot)) {
					gs.position = getAddrPosition(gs.expr->expr_cdt.addrdq, tfroot);
					break;
				}
			}
			if (it == gs.expr->expr_cdt.addrdq.end()) {
				throw exception("root position for tffka in GroundingState is missing");
			}

		}
	}
	GroundingState::GroundingState(bool assignNewID) {
		if (assignNewID == true) {
			id = getNewID();
			checkExprCdt();
		}
	}
	GroundingState::GroundingState() {
		id = getNewID();
		checkExprCdt();
	}
	GroundingState::~GroundingState() {

	}

	GroundingState::GroundingState(ExpressionHandlerPtr expr_, bool duplicate, const Numb& policy,
		const Intg& position_, const GroundingState* lastgs_) {
		if (duplicate) {
			this->expr = ExpressionHandlerPtr::make();
			this->expr->deepAssign(expr_);
			/*exprHandlerPool.addPointer(this->expr);*/
		}
		else {
			this->expr = expr_;
		}
		this->policy = policy;
		this->position = position_;
		this->lastgs = const_cast<GroundingState*>(lastgs_);
		id = getNewID();
		checkExprCdt();

	}
	GroundingState::GroundingState(ExpressionHandlerPtr expr_, bool duplicate) {
		if (duplicate) {
			this->expr = ExpressionHandlerPtr::make();
			this->expr->deepAssign(expr_);
		}
		else {

			this->expr = expr_;
		}
		id = getNewID();
		checkExprCdt();

	}


	set<Strg> GroundingState::getknowledgeDomains()//return{filename与file-classname的集合}
		const {

		return statemat.getknowledgeDomains();
	}
	/// <summary>
	/// todo:: here i use a lazy way and may be unsafe. in the future, move this function to the GroundingStateSilo and use extract() to get the GroundingState object and modify its .succeed.
	/// success can override failure and invalid, and failure can override invalid
	/// 
	/// </summary>
	/// <param name="succeed_"></param>
	void GroundingState::setGroundingResult(const Intg& succeed_, const set<CodeTable, function<bool(const CodeTable&, const CodeTable&)>>* successCodeTableSet) const {
		if (this->succeed == succeed_) {
			return;
		}
		if (succeed_ == 0 && this->succeed == 1) {
			return;
		}
		if (succeed_ == 0 && this->lastgs && (this->lastgs->initialState == true || this->lastgs->succeed == 1)) {
			return;
		}
		if (succeed_ == 0 && successCodeTableSet != nullptr && successCodeTableSet->count(this->expr->expr_cdt) > 0) {
			return;
		}
		Intg* this_succeed = const_cast<Intg*>(&this->succeed);
		*this_succeed = succeed_;
		/*if (succeed_ == true && lastgs && garbage.count(lastgs) == 0) {
			lastgs->setGroundingResult(succeed_);
		}*/
		if (lastgs) {
			lastgs->setGroundingResult(succeed_, successCodeTableSet);
		}

	}
	/// <summary>
	/// set MatchState of the grounding state and rulefunNameAR, ruleFunBodyAR based on the match state.
	/// </summary>
	/// <param name="ms_"></param>
	/// <param name="tfth"></param>
	/// <param name="tffka_"></param>
	void GroundingState::setMatchState(const MatchState& ms_, TemplateFunctionTableHash* tfth, const FKA& tffka_) {

		this->matchState = ms_;

		if (tffka_.addr == addrnull || tffka_.fileKey == FILEKEYBUILTIN) {
			return;
		}
		assert(this->expr->baseAR != nullptr);
		assert(this->expr->ruleFunBodyAR != nullptr);

		this->tffka = tffka_;
		TemplateFunction& tf = tfth->operator[](tffka);


		if (tf.integrate == true) {
			this->expr->ruleFunNameAR = DataTablePtr::make();
			this->expr->ruleFunNameAR->scopeStructureFKA = tffka;
			DataTablePtr ruleFunNameARInBaseAR = nullptr;
			DataTablePtr ruleFunBodyARInBaseAR = nullptr;

			/// if possible, copy the existing rule fun name ar and rule fun body ar and do the reasoning in these two ars. the two ar work as sandboxes to avoid contaminating common ar. clean earlier data (don't clear its relation to other ars.)
			if (this->expr->baseAR->findall(Arg(S_AR, tffka)))
			{	//if we find an ar sharing the same-scope-fka with rule fun name ar in the base ar(even cannot be accessed from the ruleFunBody ar), we copy it to the rule fun name ar.
				ruleFunNameARInBaseAR = this->expr->baseAR->operator[](Arg(S_AR, tffka)).content_ar;//global ar
				ruleFunNameARInBaseAR->deepAssign(this->expr->ruleFunNameAR, ruleFunNameARInBaseAR, true);
				this->expr->ruleFunNameAR->argdmap.clear();
				this->expr->ruleFunNameAR->frmap.clear();
			}
			else {
				//if we cannot find a same-scope-fka ar, just create it.
				this->expr->ruleFunNameAR->scopeStructureFKA = this->tffka;

			}

			//
			if (this->expr->baseAR->findall(Arg(S_AR, tf.bodyScopeFKA)))
			{
				ruleFunBodyARInBaseAR = this->expr->baseAR->operator[](Arg(S_AR, tffka)).content_ar;//global ar
				ruleFunBodyARInBaseAR->deepAssign(this->expr->ruleFunBodyAR, ruleFunBodyARInBaseAR, true);
				this->expr->ruleFunBodyAR->argdmap.clear();
				this->expr->ruleFunBodyAR->frmap.clear();
			}
			else {
				//if we cannot find a same-scope-fka ar, just create it.
				this->expr->ruleFunBodyAR->scopeStructureFKA = tf.bodyScopeFKA;
			}

			//make the two sandbox ars' parent ar consistant.
			if (ruleFunNameARInBaseAR != nullptr && ruleFunBodyARInBaseAR == nullptr) {
				this->expr->ruleFunBodyAR->parentAR = this->expr->ruleFunNameAR->parentAR;

			}
			else if (ruleFunNameARInBaseAR == nullptr && ruleFunBodyARInBaseAR != nullptr) {
				this->expr->ruleFunNameAR->parentAR = this->expr->ruleFunBodyAR->parentAR;
			}

			//cut the connection between rulefunBody ar (sandbox) and rulefunName ar in base ar
			if (ruleFunBodyARInBaseAR != nullptr && ruleFunNameARInBaseAR != nullptr) {
				deque<DataTablePtrW>& queryarlist = this->expr->ruleFunBodyAR->queryARList;
				/*queryarlist.erase(remove_if(queryarlist.begin(), queryarlist.end(), [&ruleFunNameARInBaseAR](const auto& query_ar) ->bool {
					return query_ar == nullptr || query_ar == ruleFunNameARInBaseAR; }));*/

				deque<DataTablePtrW> queryarlist_new;
				for (DataTablePtrW& query_ar : queryarlist) {
					if (query_ar == nullptr || query_ar == ruleFunNameARInBaseAR) {
						continue;

					}
					queryarlist_new.push_back(query_ar);

				}
				queryarlist.clear();
				queryarlist.insert(queryarlist.begin(), queryarlist_new.begin(), queryarlist_new.end());
			}

			//no existing ar found.
			if (ruleFunBodyARInBaseAR == nullptr && ruleFunNameARInBaseAR == nullptr)
			{
				this->expr->ruleFunBodyAR->queryARList.push_back(this->expr->baseAR);
				this->expr->ruleFunBodyAR->parentAR = this->expr->baseAR;

				this->expr->ruleFunNameAR->parentAR = this->expr->ruleFunBodyAR->parentAR;
			}


			//add name ar to the query ar list of body ar.
			this->expr->ruleFunBodyAR->queryARList.push_back(this->expr->ruleFunNameAR);

			//add base AR as the query ar for rule fun name ar and clean all existing "ans".
			this->expr->ruleFunNameAR->queryARList.push_back(this->expr->baseAR);
			while (this->expr->baseAR->findall(Arg(S_Bs, "ans"))) {
				auto ar_with_ans = this->expr->baseAR->getArgAR(Arg(S_Bs, "ans"));
				if (ar_with_ans != nullptr) {
					//ar_with_ans->argdmap.erase(Arg(S_Bs, "ans"));
					ar_with_ans->frmap.erase(Arg(S_Bs, "ans"));
				}
			}

			//make the sand box return to base ar.
			this->expr->ruleFunBodyAR->returnAR = this->expr->baseAR;

			//make up missing cmp/real args to rule fun name ar.
			for (auto& p : matchState.rcam) {
				ArgCmp& argcmp = p.second.first;
				auto& argref = p.first;
				if (argref == Arg(S_Bs, "ans")) {
					//don't make "ans" points to an actual tree(expr) outside the rule function. it will be used to store the return expr of the rule function. so we just need to add it to the rule fun name ar.
					this->expr->ruleFunNameAR->add(Arg(S_Bs, "ans"));
				}
				else {

					//add rcam from matchstate into ruleFunBodyAR as frmap.
					this->expr->ruleFunBodyAR->frmap[argref] = argcmp;


					if (this->expr->ruleFunNameAR->findall(argcmp) == false) {

						this->expr->ruleFunNameAR->add(argcmp);

						//throw exception("unknown real args for fact functions.");

					}

				}


			}
			if (this->expr->ruleFunNameAR->findall(Arg(S_Bs, "ans")) == false) {
				this->expr->ruleFunNameAR->add(Arg(S_Bs, "ans"));
			}

		}
		else {

			//fact function only need a dummy rule fun body ar.
			this->expr->ruleFunBodyAR->scopeStructureFKA = tf.bodyScopeFKA;
			this->expr->ruleFunBodyAR->queryARList.push_back(this->expr->baseAR);
			this->expr->ruleFunBodyAR->parentAR = this->expr->baseAR;
			this->expr->ruleFunBodyAR->returnAR = this->expr->baseAR;
			for (auto& p : matchState.rcam) {
				ArgCmp& argcmp = p.second.first;
				auto& argref = p.first;
				//add rcam from matchstate into ruleFunBodyAR as frmap.
				this->expr->ruleFunBodyAR->frmap[argref] = argcmp;


				if (this->expr->ruleFunBodyAR->findall(argcmp) == false) {

					//Only argcmps of S_Dz can be missing because of tree operations
					assert(argcmp.argFlag == S_Dz);
					//if real arg can not be found, create data in ruleFunBodyAR.
					this->expr->ruleFunBodyAR->add(argcmp);

				}


			}
		}
		return;
	}
	//此处的cdth/systh_用于在导出时查询信息用，不要在内部修改！
			/**
			 * @attention export GroundingState(except final states and initial states)
			 * @param outputFileName,
			 * @param expected_exported_konwledge_domains, 期望导出的knowledgedomain类型，
				attention当前不处理复合的domain，因此只包含一个元素（非此domain的gs indomain为false）
			 * @param problem_knowledge_domains, 该problem归属的knowledgedomain
			 * @param duplicated 是否允许重复输出
			 * @param export_succeed_ 是否输出成功的
			 * @param export_failured_ 是否输出成功的
			 * @param cdth 用来查类型名称
			 * @param systh_ 用来查询类型名称
			 * @return
			 */
	bool GroundingState::bufferTrainingData(const Strg& outputFileName, const set<Strg>& expected_exported_knowledge_domains, const set<Strg>& problem_knowledge_domains, const bool& duplicated,
		const bool& export_succeed_, const bool& export_failed_, const CodeTableHash* cdth,
		const SystemTableHash* systh_) const {

		if (problem_knowledge_domains.size() == 0) {
			return false;
		}

		if (GroundingStateSilo::trainingDataBuffer.count(outputFileName) == 0) {
			GroundingStateSilo::trainingDataBuffer[outputFileName] = json::array();
		}


		std::set<Strg> intersection;
		std::set_intersection(expected_exported_knowledge_domains.begin(), expected_exported_knowledge_domains.end(), problem_knowledge_domains.begin(), problem_knowledge_domains.end(),
			std::inserter(intersection, intersection.begin()));

		bool indomain = intersection.size() > 0;
		if (this->succeed == true) {
			if (expected_exported_knowledge_domains.count(split(this->tffka.fileKey, "#")[0]) <= 0) {
				indomain = false;
			}
		}
		auto& output_buffer_json = GroundingStateSilo::trainingDataBuffer[outputFileName];

		if (indomain == true) {
			if (this->succeed == -1) {
				return false;
			}
			if (export_succeed_ == false && this->succeed == true) {
				return false;
			}
			if (export_failed_ == false && this->succeed == false) {
				return false;
			}
		}

		if (exported) {
			if (!duplicated) {
				return true;
			}
		}
		if (finalState) {
			if (this->lastgs) {
				return lastgs->bufferTrainingData(outputFileName, expected_exported_knowledge_domains, problem_knowledge_domains, duplicated, export_succeed_, export_failed_, cdth,
					systh_);
			}
			return true;
		}
		if (initialState) {
			return true;
		}
		expr->expr_cdt.cdth = const_cast<CodeTableHash*>(cdth);
		SystemTableHash* systh = const_cast<SystemTableHash*>(systh_);
		json j;
		j = this->toJson(cdth, systh_, indomain);
		Strg gstfdomain = expected_exported_knowledge_domains.begin().operator*();
		j["gstfdomain"] = gstfdomain;
		json domain_step_map = j["domainstepmap"];
		if (gstfdomain != "" && domain_step_map.contains(gstfdomain)) {
			j["step"] = domain_step_map[gstfdomain];
		}
		else {
			j["step"] = 0;
		}


		output_buffer_json.push_back(j);
		if (this->lastgs) {
			return lastgs->bufferTrainingData(outputFileName, expected_exported_knowledge_domains, problem_knowledge_domains, duplicated, export_succeed_, export_failed_, cdth, systh_);
		}
		//		try {
		//			auto&& jstr = j.dump(4);
		//			ofs << jstr;
		//		}
		//		catch (exception& e) {
		//			{
		//				static int ignorecount = 0;
		//				std::cerr << "(" << __FILE__ << ":" << __LINE__ << ":0"
		//					<< ")\texport_(ofstream&, const bool&, const bool&, const CodeTableHash*, const SystemTableHash*)"
		//					<< " cannot export json data, exit(-1) "
		//					<< "\tignorecount:[" << ignorecount++ << "\t]"
		//					<< std::endl;
		//	}
		//#if debug && _WIN32
		//			{
		//				static int ignoreCount = 0;
		//				// Assuming info is a std::string, convert to std::wstring
		//				std::string info = "error exit"; // Replace with actual info
		//				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
		//				std::wstring winfo(info.begin(), info.end());
		//#if WIN_DEBUG_OUTPUT
		//				OutputDebugString(winfo.c_str());
		//#endif
		//				cout << info << endl;
		//				ignoreCount++;
		//			}
		//#endif
		//			assert(false); exit(-1);
		//
		//		}
		//		if (this->lastgs) {
		//			ofs << ",";
		//			return lastgs->bufferTrainingData(ofs, duplicated, only_succeed_, cdth, systh_);
		//		}
		return true;
	}
	/**
	@attention if the grounding state need to be exported as training data, this function can only be called after determining tf applied to the cdt and tf has been added to the statemat.
		if exported as predicting data, the applied tf neednot determined and added before calling this function, but the advance/ reward is invalid.

	*/
	json GroundingState::toJson(const CodeTableHash* cdth,
		const SystemTableHash* systh_, const bool& indomain) const {

		//        codeTable.cdth = const_cast<CodeTableHash*>(cdth);
		SystemTableHash* systh = const_cast<SystemTableHash*>(systh_);
		json j;
		j["indomain"] = indomain;
		j["succeed"] = this->succeed;
		j["initialstate"] = this->initialState;
		j["position"] = this->position;
		Strg gstfdomain = split(this->tffka.fileKey, "#")[0];
		j["gstfdomain"] = gstfdomain;
		j["BDDB_value"] = this->BDDB_value;
		j["rule"] = this->rule;//tf return expr(rule fun) or value(fact fun)?

		j["relatedFilesAndClasses"] = this->statemat.knowledge_domain_list;
		//todo: modify it to suit multi-domain policy making
		json domain_step_map = {};
		if (this->lastgs != nullptr && this->lastgs->statemat.knowledge_domain_list.size() > 0) {
			for (size_t i = 0; i < this->lastgs->statemat.knowledge_domain_list.size(); ++i) {
				domain_step_map[this->lastgs->statemat.knowledge_domain_list[i]] = this->lastgs->statemat.step_list[i];
			}
		}

		if (gstfdomain != "" && domain_step_map.contains(gstfdomain)) {
			j["step"] = domain_step_map[gstfdomain];
		}
		else {
			j["step"] = 0;
		}
		Intg advance = 0;//the step after adopting current tf
		if (gstfdomain != "" && this->statemat.knowledge_domain_list.size() > 0) {
			for (size_t i = 0; i < this->statemat.knowledge_domain_list.size(); ++i) {
				if (this->statemat.knowledge_domain_list[i] == gstfdomain) {
					advance = this->statemat.step_list[i];
					break;
				}
			}
		}
		j["advance"] = advance;

		j["domainstepmap"] = domain_step_map;
		bool rewardsign = 0; //negative()
		Numb rewardval = 0;
		if (this->tffka.fileKey == FILEKEYBUILTIN) {
			rewardval = (toNumb(BUILTIN_FUN_INFO));
			rewardsign = rewardval > 0;
		}
		else {
			GroundingState* this_ = const_cast<GroundingState*>(this);
			if (this_->statemat.accumulated_steps >= 1 && this_->statemat.tfinfo_add_stack[this_->statemat.accumulated_steps - 1].size() > 0) {

				rewardsign = (this_->statemat.tfinfo_add_stack[this_->statemat.accumulated_steps - 1][advance] > 0);
				rewardval = (this_->statemat.tfinfo_add_stack[this_->statemat.accumulated_steps - 1][advance]);
			}
		}
		j["rpsign"] = rewardsign; // the sign of the rp
		j["rpval"] = rewardval;
		j["codeTable"] = json::array();
		for (auto& code : expr->expr_cdt.codedq) {

			Arg&& op1 = code[0];
			Arg&& op2 = code[1];
			Arg&& opr = code[2];
			Arg&& res = code[3];

			json jop1;
			json jop2;
			json joperator;
			json jres;
			json jcode;
			//op1
			if (op1.isDz()) {
				jop1["argName"] = op1.arg_fka.addr.toStrg();
				jop1["argType"] = to_json_ID_;

			}
			else if (op1.isBs()) {
				if (opr.arg_s == "call") {
					jop1["argName"] = op1.arg_s;
					jop1["argType"] = to_json_FUNN_;
				}
				else {

					jop1["argName"] = op1.arg_s;
					jop1["argType"] = to_json_ID_;

				}
			}
			else if (op1.argFlag == Sz) {
				jop1["argName"] = COOLANG::toStrg(op1.arg_i);
				jop1["argType"] = to_json_NUMB_;
			}
			else if (op1.argFlag == Zf) {
				jop1["argName"] = op1.arg_s;
				jop1["argType"] = to_json_STRG_;
			}
			if (op1.argFlag == S_AR) {
				jop1["isClass"] = to_json_TRUE_;

				jop1["className"] =
					res.ref_ar->scopeStructureFKA.fileKey + " "
					+ systh->operator [](
						res.ref_ar->scopeStructureFKA).systemName;
			}
			else {
				jop1["isClass"] = to_json_FALSE_;

				jop1["className"] = "";
			}
			jop1["changeable"] = op1.changeable;

			//op2
			if (op2.isDz()) {
				jop2["argName"] = op2.arg_fka.addr.toStrg();
				jop2["argType"] = to_json_ID_;

			}
			else if (op2.isBs()) {

				jop2["argName"] = op2.arg_s;
				jop2["argType"] = to_json_ID_;

			}
			else if (op2.argFlag == Sz) {
				jop2["argName"] = COOLANG::toStrg(op2.arg_i);
				jop2["argType"] = to_json_NUMB_;
			}
			else if (op2.argFlag == Zf) {
				jop2["argName"] = op2.arg_s;
				jop2["argType"] = to_json_STRG_;
			}

			if (op2.argFlag == S_AR) {
				jop2["isClass"] = to_json_TRUE_;

				jop2["className"] =
					res.ref_ar->scopeStructureFKA.fileKey + " "
					+ systh->operator [](
						res.ref_ar->scopeStructureFKA).systemName;
			}
			else {
				jop2["isClass"] = to_json_FALSE_;

				jop2["className"] = "";
			}
			jop2["changeable"] = op2.changeable;
			//opr
			if (opr.arg_s == ",") {
				joperator["argName"] = ",";
				joperator["argType"] = to_json_OPR_COMMA_;

			}
			else if (opr.arg_s == "call") {
				joperator["argName"] = "call";
				joperator["argType"] = to_json_OPR_CALL_;

			}
			else {
				joperator["argName"] = opr.arg_s;
				joperator["argType"] = to_json_OPR_OTHER_;

			}

			//res
			if (res.isDz()) {
				jres["argName"] = res.arg_fka.addr.toStrg();
				jres["argType"] = to_json_ID_;

			}
			else if (res.isBs()) {

				jres["argName"] = res.arg_s;
				jres["argType"] = to_json_ID_;

			}
			else if (res.argFlag == Sz) {
				jres["argName"] = COOLANG::toStrg(res.arg_i);
				jres["argType"] = to_json_NUMB_;
			}
			else if (res.argFlag == Zf) {
				jres["argName"] = res.arg_s;
				jres["argType"] = to_json_NUMB_;
			}

			if (res.argFlag == S_AR) {
				jres["isClass"] = to_json_TRUE_;

				jres["className"] =
					res.ref_ar->scopeStructureFKA.fileKey + " "
					+ systh->operator [](
						res.ref_ar->scopeStructureFKA).systemName;
			}
			else {
				jres["isClass"] = to_json_FALSE_;

				jres["className"] = "";
			}
			jres["changeable"] = res.changeable;

			//use jop1 jop2 joperator jres compose the jcode
//            json jcode = { jop1, jop2, joperator, jres,  };
			jcode["operand1"] = jop1;
			jcode["operand2"] = jop2;
			jcode["operator"] = joperator;
			jcode["result"] = jres;
			jcode["grounded"] = (code.assemblyFormula.flagBit.tfFKA != fkanull);
			jcode["boundtfdomain"] = (split(code.assemblyFormula.flagBit.tfFKA.fileKey, "#")[0]);//domain of the template function
			jcode["root"] = (code.assemblyFormula.flagBit.functionHandleFlag);//the template function rooted here

			j["codeTable"].push_back(jcode);

		}


		return j;
	}


	Strg GroundingState::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << "GroundingState(" << this->id << "):\t" << "position:[" << position << "]\t";
		ss << "BDDB_value:" << this->BDDB_value << "\n";
		ss << "BDDB_o_t:" << this->bddb_o_t << endl;
		ss << "tffka:\t" << this->tffka.toStrg();
		ss << "\tstateMatrix:\n" << statemat.toStrg();
		ss << "\n\tcodeTable:\n" << this->expr->expr_cdt.toStrg();
		if (this->lastgs) {
			ss << hdot;
			ss << "lastGroundingState:<<\n";
			ss << this->lastgs->toStrg();
		}
		return ss.str();

	}
	Strg GroundingState::toStrg(ScopeTableHash* scpth) const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << "BDDB_value:" << this->BDDB_value << "\n";
		ss << "ID:" << this->id << "\n";
		ss << "BDDB_o_t:" << this->bddb_o_t << endl;
		ss << "\tstateMatrix:\n" << statemat.toStrg();
		ss << "tffka:\t" << this->tffka.toStrg() << "position:[" << position << "]" << endl;
		ss << "\n\tcodeTable:\n" << this->expr->expr_cdt.toStrg();
		if (this->tffka != fkanull && scpth != nullptr) {
			ss << "\n\ttfnamecdt:\n" << TemplateFunctionTableHash::getTFNameCdt(this->tffka, scpth).toStrg();
		}
		if (this->lastgs) {
			ss << hdot;
			ss << "lastGroundingState:<<\n";
			ss << this->lastgs->toStrg(scpth);
		}

		return ss.str();

	}
	/// <summary>
	/// this function is used to compare two GroundingStates based on their position/ tffka/ and expr(arg/ cdt), used for avoiding duplicated addition into GroundingStateSilo
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	bool GroundingStateComparatorLT::operator()(const GroundingState& lhs, const GroundingState& rhs) const {
		if (lhs.position < rhs.position) {
			return true;
		}
		else if (lhs.position > rhs.position) {
			return false;
		}//pos == pos

		if (lhs.tffka < rhs.tffka) {
			return true;
		}
		else if (lhs.tffka > rhs.tffka) {
			return false;
		}//fka==fka

		auto& lexpr = lhs.expr;
		auto& rexpr = rhs.expr;
		if (lexpr->type < rexpr->type) {

			return true;
		}
		else if (lexpr->type > rexpr->type) {

			return false;
		}
		else {//lexpr == rexpr
			if (lexpr->type == EXPRESSIONHANDLER_ARG) {
				ArgComparatorLT aclt(acm::name | acm::asc | acm::changeable | acm::compatible | acm::constraint | acm::immediate);
				return aclt(lexpr->expr_arg, rexpr->expr_arg);
			}
			else {
				//type == EXPRESSIONHANDLER_CDT
				auto& lcdt = lexpr->expr_cdt;
				auto& rcdt = rexpr->expr_cdt;
				if (lcdt.addrdq < rcdt.addrdq) {
					return true;
				}
				else if (lcdt.addrdq > rcdt.addrdq) {
					return false;
				}// addrdq == addrdq

				ArgComparatorLT aclt(acm::name | acm::asc | acm::changeable | acm::compatible | acm::constraint | acm::immediate);
				auto codeComparatorLT = [&aclt](const Code& lcd, const Code& rcd) -> bool {
					if (lcd.assemblyFormula.flagBit.tfFKA < rcd.assemblyFormula.flagBit.tfFKA) {
						return true;
					}
					else if (lcd.assemblyFormula.flagBit.tfFKA > rcd.assemblyFormula.flagBit.tfFKA) {
						return false;
					}

					if (lcd.assemblyFormula.flagBit.functionHandleFlag < rcd.assemblyFormula.flagBit.functionHandleFlag) {
						return true;
					}
					else if (lcd.assemblyFormula.flagBit.functionHandleFlag > rcd.assemblyFormula.flagBit.functionHandleFlag) {
						return false;
					}

					for (int i = 0; i < 4; ++i) {
						auto&& lcdarg = lcd[i];
						auto&& rcdarg = rcd[i];
						if (aclt(lcdarg, rcdarg)) {
							return true;
						}
						else if (aclt(rcdarg, lcdarg)) {
							return false;
						}
						else {

							//lcdarg == rcdarg
							continue;
						}

					}

					//lcd == rcd
					return false;

					};
				for (int i = 0; i < lcdt.size(); ++i) {
					auto& lcd = lcdt[i];
					auto& rcd = rcdt[i];

					if (codeComparatorLT(lcd, rcd)) {
						return true;
					}
					else if (codeComparatorLT(rcd, lcd)) {
						return false;
					}
					else {
						//lcd == rcd
						continue;

					}
				}
				//lcdt == rcdt
				return false;

			}




		}


	}

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
	Matcher::Matcher(CodeTable* cdtcmp, CodeTable* cdtref,
		TemplateFunctionTableHash* tfth, const AddrCmp& addrcmp,
		const AddrRef& addrref, MatchMode md /*= MatchMode::RuleMatchingFormalRealBind*/, DataTablePtrW dt /*= nullptr*/, CodeTableHash* cdth /*= nullptr*/, set<Arg, ArgComparatorLT> placeholders) :ms(cdtcmp, addrcmp, cdtref, addrref, md, dt, cdth, placeholders) {
		if (cdtcmp == nullptr) {

			cout << "Matcher::Matcher err::cdtcmp == null" << endl;
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

		if (cdtref == nullptr) {

			cout << "Matcher::Matcher err::cdtref == null" << endl;
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

		this->cdtcmp = cdtcmp;
		this->cdtref = cdtref;
		this->tfth = tfth;
		this->addrcmp = addrcmp;
		this->addrref = addrref;
		this->md = md;
		this->currentAR = dt;
		this->cdth = cdth;
		this->placeholders = placeholders;

	}
	/**
		 * @name compareMultiCode
		 * @brief compare two trees, 但是这个函数更专用，主要用在比较函数头和约束对应代码段。
		 * @param ms
		 * @return
		 */
	const bool Matcher::compareMultiCode() {

		while (ms.size() > 0) {

			/*if (ms.size() <= 0) {
				return true;
			}*/
			AddrCmp addrcmp;
			Intg argposcmp;
			AddrRef addrref;
			Intg argposref;
			ms.pop(addrcmp, argposcmp, addrref, argposref);

#if debug
			static Intg ignorecount = 0;
			cout << "Matcher::compareMultiCode start, addrcmp:[" << addrcmp.toStrg()
				<< "]\targposcmp:[" << argposcmp << "]"
				"\taddrref:[" << addrref.toStrg() << "]\targposref:["
				<< argposref << "]\tignorecount:[" << ignorecount++ << "]"
				<< endl;
#endif

			if (addrref > addrnull && addrcmp > addrnull) {
				ArgCmp argcmp((*cdtcmp)[addrcmp][argposcmp]);
				AddrCmp cdaddrcmp =
					cdtcmp->getSameTrBranchLastArgAddrIgnoreProperty(addrcmp,
						argcmp, acm::name | acm::asc);

				ArgRef argref((*cdtref)[addrref][argposref]);
				AddrRef cdaddrref =
					cdtref->getSameTrBranchLastArgAddrIgnoreProperty(addrref,
						argref, acm::name | acm::asc);


				while (cdaddrcmp != addrnull && cdaddrref != addrnull) {
#if debug
					static Intg ignorecount = 0;
					cout
						<< "Matcher::compareMultiCode 2, cdaddrcmp != addrnull && cdaddrref != addrnull , "
						"addrcmp:[" << addrcmp.toStrg()
						<< "]\targposcmp:[" << argposcmp << "]"
						"\taddrref:[" << addrref.toStrg()
						<< "]\targposcmp:[" << argposref << "]\tignorecount:["
						<< ignorecount++ << "]" << endl;
#endif
					/*MatchState msmid(ms);
					if (msmid.compareSingleCode(cdaddrcmp, (*cdtcmp)[cdaddrcmp],
						cdaddrref, (*cdtref)[cdaddrref]) == true) {

						TFInfoMat organization;
						if (tfth) {
							const FKA& tfFKA = cdtref->getScopeFKA(addrref);
							organization = (*tfth)[tfFKA].organization;
						}
						mss.push(organization, msmid);
					}*/
					if (ms.compareSingleCode(cdaddrcmp, (*cdtcmp)[cdaddrcmp],
						cdaddrref, (*cdtref)[cdaddrref]) == true) {
						cdaddrcmp = cdtcmp->getSameTrBranchLastArgAddrIgnoreProperty(
							cdaddrcmp, argcmp, acm::name | acm::asc);
						cdaddrref = cdtref->getSameTrBranchLastArgAddrIgnoreProperty(
							cdaddrref, argcmp, acm::name | acm::asc);


						if ((static_cast<int>(this->md) & static_cast<int>(MatchMode::RuleMatchingFormalRealBind)) > 0) {
							if ((cdaddrcmp == addrnull || cdtcmp->operator[](cdaddrcmp).assemblyFormula.flagBit.tfFKA != fkanull) && cdaddrref != addrnull) {
								return false;
							}
						}

					}
					else {
						return false;
					}
				}

				if (cdaddrcmp == addrnull && cdaddrref != addrnull) {
					//code cmp matches part of code ref, error
					return false;
				}
				else {
					//code ref matches part/ whole of code cmp, when a expression has many potential function invocations.
					continue;

				}

			}
			else {

				cout << "Matcher::compareMultiCode err,exit-1. addrcmp:["
					<< addrcmp.toStrg() << "]\t"
					"addrref:[" << addrref.toStrg() << "]" << endl;
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

		return true;



	}
	void Matcher::reset() {
		this->ms = MatchState(this->cdtcmp, this->addrcmp, this->cdtref, this->addrref, this->md, this->currentAR, this->cdth, this->placeholders);
	}
	const bool Matcher::match() {

#if debug && _WIN32
		{
			stringstream ss;
			ss << bbhline;
			ss << __FUNCTION__;
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


		if (addrref > addrnull && addrcmp > addrnull) {
#if debug
			static Intg ignorecount = 0;
			cout << "Matcher::match addrref>addrnull \t"
				"addrref:[" << addrref.toStrg() << "]\taddrcmp:["
				<< addrcmp.toStrg() << "]\tignorecount:[" << ignorecount++
				<< "]" << endl;
#endif

			if (ms.compareSingleCode(addrcmp, cdtcmp->operator [](addrcmp),
				addrref, cdtref->operator [](addrref)) == true) {

#if debug
				static Intg ignorecount = 0;
				cout
					<< "Matcher::match addrref>addrnull compareSingleCode == true\t"
					"addrref:[" << addrref.toStrg()
					<< "]\taddrcmp:[" << addrcmp.toStrg()
					<< "]\tignorecount:[" << ignorecount++ << "]" << endl;
#endif
				return compareMultiCode();
				/*TFInfoMat organization;
				if (tfth) {
					const FKA& tfFKA = cdtref->getScopeFKA(addrref);
					organization = (*tfth)[tfFKA].organization;
				}
				mss.push(organization, ms);*/
			}
			else {
				return false;
			}
		}
		else if (addrref < addrnull && addrcmp > addrnull) {

			if (ms.compareSingleCode(addrcmp, cdtcmp->operator [](addrcmp),
				addrref, Code()) == true) {
				//mss.push(TFInfoMat(BUILTIN_FUN_INFO), ms);
				return true;
			}
			else {
				return false;
			};
		}
		else {

#if debug
			cerr << "Matcher::match err::invalid addrref:[" << addrref.toStrg()
				<< "]\t"
				"addrcmp:[" << addrcmp.toStrg() << "]" << endl;
#endif

		}

	}

	inline MatchState& Matcher::getMatchState() {

		return this->ms;
	}


	/**
		 *
		 * @param cdtdes 此代码段的部分片段被替换
		 * @param cdtfrom 次代码段用于替换cdtdes代码片段所在代码段
		 * @param sth
		 * @param mindes 替换部分下限
		 * @param maxdes 替换部分上限 (mindes,maxdes)//mindes、maxdes可以为addrnull
		 * @param trfrom cdtfrom代码段中，用于替换cdtdes代码片段的语法树分支根地址
		 * @param am_cmp_ref 实参（cdtdes中参数）形参（cdtfrom中参数）对照表
		 * @param addrm cdtdes的addrdq中被替换的code的addr与用于替换的代码片段的对照表
		 */
	SingleTFIntegrator::SingleTFIntegrator(CodeTable* cdtdes, CodeTable* cdtfrom,
		ScopeTableHash* sth, const Addr& mindes, const Addr& maxdes,
		const Addr& trfrom, const map<ArgReal, deque<ArgForm> >& am_cmp_ref,
		const map<AddrFormal, AddrReal>& addrm) :am_ref_cmp(ArgComparatorLT(acm::name | acm::asc)) {
		this->cdtdes = cdtdes;
		this->cdtfrom = cdtfrom;
		this->sth = sth;
		this->mindes = mindes;
		this->maxdes = maxdes;
		if (cdtdes->getLast(COOL_M, maxdes) != addrnull) {
			this->scopeFKAdes = (*cdtdes)[cdtdes->getLast(COOL_M, maxdes)].scopeFKA;
		}
		else {
			this->scopeFKAdes = cdtdes->getScopeFKA(maxdes);
		}
		if (addrm.size() > 0) {

			this->attachmentRootNode = cdtdes->operator[](addrm.rbegin()->first)[3];
			if (attachmentRootNode.argFlag == S_Dz) {
				this->maxdes = min(attachmentRootNode.arg_fka.addr, maxdes);
			}


		}

		this->trfrom = trfrom;
		this->addrm = addrm;
		cdtmid = get<0>(cdtfrom->copyTree(trfrom));
		this->am_ref_cmp.clear();
		for (auto iter = am_cmp_ref.begin(); iter != am_cmp_ref.end(); iter++) {
			for (const ArgReal& ades : iter->second) {
				this->am_ref_cmp.insert(pair<ArgForm, ArgReal>(ades, iter->first));
			}

		}


		//if the root arg of cdtfrom is not a key in ref_cmp_am, add the root args of cdtfrom and the replaced part of the cdtdes into the am_ref_cmp.
		if (cdtfrom->size() > 0 && addrm.size() > 0 && am_ref_cmp.count(cdtfrom->codedq.back()[3]) == 0) {
			Arg&& rootcmp = cdtdes->operator[](addrm.rbegin()->first)[3];
			am_ref_cmp[cdtfrom->codedq.back()[3]] = rootcmp;

		}


		//if "ans" is a key in ref_cmp pair, means that the cdtfrom is the returned codetable of a rule function, 
		// therefore, we should replace "ans" with the root arg of the cdtfrom. 
		if (am_ref_cmp.count(Arg(S_Bs, "ans")) > 0) {
			auto&& nodeHandler = am_ref_cmp.extract(Arg(S_Bs, "ans"));
			auto& key = nodeHandler.key();
			if (cdtfrom->size() > 0) {
				key = cdtfrom->codedq.back()[3];
			}
			am_ref_cmp.insert(move(nodeHandler));
		}

		am_ref_cmp.erase(argnull);
	}


	/**
		 *
		 * @param code 被修改的code
		 * @param scopeFKAdes 被修改code所在scopefka
		 * @param
		 * @return codeFKAdes  被修改code所生成的新的codefka（min，max）
		 */
	Addr SingleTFIntegrator::replaceLine(CodeTable& cdt, Intg pos, const FKA& scopeFKAdes,
		const Addr& addrlowbound, const Addr& addrhighbound) {
			{
#if debug
				static int ignorecount = 0;
				cout << "SingleTFIntegrator::replaceLine 1.0 ignorecount:["
					<< ignorecount++ << "\t]" << endl;
#endif
			}

			Code& code = cdt[pos];

			Arg&& ref1 = code[0];
			Arg&& ref2 = code[1];
			Arg&& refop = code[2];
			Arg&& refres = code[3];

			ArgDec* arg1 = ref1.isVar() ? &am_ref_cmp[ref1] : nullptr;
			ArgDec* arg2 = ref2.isVar() ? &am_ref_cmp[ref2] : nullptr;
			ArgDec* argres = refres.isVar() ? &am_ref_cmp[refres] : nullptr;

#if debug && _WIN32
			{
				stringstream ss;
				ss << bbhline;
				ss << __FUNCTION__ << "::cdt:" << endl;
				ss << cdt.toStrg();
				ss << bhline;
				ss << "code:" << endl;
				ss << code.toStrg() << endl;
				ss << bhline;
				ss << "am_ref_cmp" << endl;
				ss << COOLANG::toStrg(am_ref_cmp) << endl;
				ss << bhline;
				ss << calign("ref1:" + ref1.toBrief() + "\targ1:" + (arg1 ? arg1->toBrief() : ""), 24) << calign("ref2:" + ref2.toBrief() + "\targ2:" + (arg2 ? arg2->toBrief() : ""), 24) << calign("refres:" + refres.toBrief() + "\targres:" + (argres ? argres->toBrief() : ""), 24) << "\n";
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
			Addr newaddrlowbound = addrlowbound;
			if (arg1 && *arg1 != argnull && arg1->argFlag == S_Dz && arg1->arg_fka.addr > newaddrlowbound && arg1->arg_fka.addr < addrhighbound) {
				newaddrlowbound = arg1->arg_fka.addr;

			}
			if (arg2 && *arg2 != argnull && arg2->argFlag == S_Dz && arg2->arg_fka.addr > newaddrlowbound && arg2->arg_fka.addr < addrhighbound) {
				newaddrlowbound = arg2->arg_fka.addr;

			}
			if (argres && *argres != argnull && argres->argFlag == S_Dz && argres->arg_fka.addr > newaddrlowbound && argres->arg_fka.addr < addrhighbound) {
				newaddrlowbound = argres->arg_fka.addr;

			}
			if (ref1 != argnull && ref1.argFlag == S_Dz && ref1.arg_fka.addr > newaddrlowbound && ref1.arg_fka.addr < addrhighbound) {
				newaddrlowbound = ref1.arg_fka.addr;

			}
			if (ref2 != argnull && ref2.argFlag == S_Dz && ref2.arg_fka.addr > newaddrlowbound && ref2.arg_fka.addr < addrhighbound) {
				newaddrlowbound = ref2.arg_fka.addr;

			}
			if (refres != argnull && refres.argFlag == S_Dz && refres.arg_fka.addr > newaddrlowbound && refres.arg_fka.addr < addrhighbound) {
				newaddrlowbound = refres.arg_fka.addr;

			}

			auto codeFKAdes = FKA(scopeFKAdes.fileKey, split(newaddrlowbound, addrhighbound, 1).back());
			//new created addr should never be the same with used addrs.
		L:for (auto p : am_ref_cmp) {
			if (p.first.isDz() && p.first.arg_fka.addr == codeFKAdes.addr && codeFKAdes.addr != addrhighbound) {
				codeFKAdes.addr = split(codeFKAdes.addr, addrhighbound, 1).back();
				goto L;
			}
			if (p.second.isDz() && p.second.arg_fka.addr == codeFKAdes.addr && codeFKAdes.addr != addrhighbound) {
				codeFKAdes.addr = split(codeFKAdes.addr, addrhighbound, 1).back();
				goto L;
			}

		}
		for (auto& c : cdt.codedq) {
			if ((codeFKAdes.addr == c.assemblyFormula.quaternion.arg1_fka.addr || codeFKAdes.addr == c.assemblyFormula.quaternion.arg2_fka.addr || codeFKAdes.addr == c.assemblyFormula.quaternion.result_fka.addr) && codeFKAdes.addr != addrhighbound) {
				codeFKAdes.addr = split(codeFKAdes.addr, addrhighbound, 1).back();
				goto L;
			}
		}
		if (code.assemblyFormula.flagBit.execute == F_) {
			code.assemblyFormula.flagBit.execute = T_;
		}

		set<Strg> exclude_op = { "new","out","$","#","?" };
		if (code.assemblyFormula.flagBit.type == COOL_M
			&& code.assemblyFormula.flagBit.operator_flag == M_Bs
			&& (
				exclude_op.count(code.assemblyFormula.quaternion.operator_s) == 0

				/*code.assemblyFormula.quaternion.operator_s == "+"
				|| code.assemblyFormula.quaternion.operator_s == "-"
				|| code.assemblyFormula.quaternion.operator_s == "*"
				|| code.assemblyFormula.quaternion.operator_s == "/"
				|| code.assemblyFormula.quaternion.operator_s == "^"
				|| code.assemblyFormula.quaternion.operator_s == "%"
				|| code.assemblyFormula.quaternion.operator_s == "=="
				|| code.assemblyFormula.quaternion.operator_s == "="
				|| code.assemblyFormula.quaternion.operator_s == "!"
				|| code.assemblyFormula.quaternion.operator_s == "--"
				|| code.assemblyFormula.quaternion.operator_s == "++"
				|| code.assemblyFormula.quaternion.operator_s == "+="
				|| code.assemblyFormula.quaternion.operator_s == "-="
				|| code.assemblyFormula.quaternion.operator_s == "*="
				|| code.assemblyFormula.quaternion.operator_s == "/="
				|| code.assemblyFormula.quaternion.operator_s == "%="
				|| code.assemblyFormula.quaternion.operator_s == "^="
				|| code.assemblyFormula.quaternion.operator_s == "&&"
				|| code.assemblyFormula.quaternion.operator_s == "||"
				|| code.assemblyFormula.quaternion.operator_s == "!"
				|| code.assemblyFormula.quaternion.operator_s == "%"
				|| code.assemblyFormula.quaternion.operator_s == "-->"
				|| code.assemblyFormula.quaternion.operator_s == "call"
				|| code.assemblyFormula.quaternion.operator_s == "ca"
				|| code.assemblyFormula.quaternion.operator_s == "COMMA"*/

				/*todo:add new common operator here*/
				)) {

					{
#if debug
						static int ignorecount = 0;
						cout << "SingleTFIntegrator::replaceLine 1.1 ignorecount:["
							<< ignorecount++ << "\t]" << endl;
#endif
					}

					Intg argNullNum = 0;
					if (arg1 &&

						(*arg1 == argnull ||
							(arg1->argFlag == S_Dz &&

								(arg1->arg_fka.addr > addrhighbound ||
									arg1->arg_fka.addr < min(this->mindes, cdtdes->size() > 0 ? this->cdtdes->addrdq.front() : this->mindes))

								)
							)

						) {
						if (ref1.isDz() && ref1.argFlag != EXPRZ_Dz && ref1.argFlag != Z_Dz

							) {
							arg1->argFlag = S_Dz;
							arg1->arg_fka = codeFKAdes;
							arg1->formalArg = T_;
							arg1->changeable =
								code.assemblyFormula.flagBit.changeable[0];
							//arg1->asc = scopeFKAdes;
							++argNullNum;
						}
						else {
							(*arg1) = ref1;
						}

					}
					if (arg2 &&

						(*arg2 == argnull ||
							(arg2->argFlag == S_Dz &&

								(arg2->arg_fka.addr > addrhighbound ||
									arg2->arg_fka.addr < min(this->mindes, cdtdes->size() > 0 ? this->cdtdes->addrdq.front() : this->mindes))

								)
							)

						) {
						if (ref2.isDz() && ref2.argFlag != EXPRZ_Dz) {
							arg2->argFlag = S_Dz;
							arg2->arg_fka = codeFKAdes;
							arg2->formalArg = T_;
							arg2->changeable =
								code.assemblyFormula.flagBit.changeable[1];
							//arg2->asc = scopeFKAdes;
							++argNullNum;
						}
						else {
							*arg2 = ref2;
						}
					}
					if (argres &&

						(*argres == argnull ||
							(argres->argFlag == S_Dz /*&&

								(argres->arg_fka.addr > addrhighbound ||
									argres->arg_fka.addr < addrlowbound)*/

								)
							)

						) {
						if (refres.isDz() && refres.argFlag != EXPRZ_Dz) {
							argres->argFlag = S_Dz;
							argres->arg_fka = codeFKAdes;
							argres->formalArg = T_;
							argres->changeable =
								code.assemblyFormula.flagBit.changeable[3];
							//argres->asc = scopeFKAdes;
							++argNullNum;
						}
						else {
							*argres = refres;
						}
					}
					if (argNullNum > 1) {
						//we don't need it now
	//					cout
	//						<< "replaceLine err::more than one argnull to replace. argNullNum:["
	//						<< argNullNum << "]" << endl;
	//#if debug && _WIN32
	//					{
	//						static int ignoreCount = 0;
	//						// Assuming info is a std::string, convert to std::wstring
	//						std::string info = "error exit"; // Replace with actual info
	//						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
	//						std::wstring winfo(info.begin(), info.end());
	//						#if WIN_DEBUG_OUTPUT
//						OutputDebugString(winfo.c_str());
//#endif
//						cout << info << endl;
						//						ignoreCount++;
						//					}
						//#endif
						//					assert(false);exit(-1);
					}
					Arg::infer_changeable(arg1, arg2, refop, argres);
					/*if (constantChangeableOps.count(refop.arg_s) && argres) {
						argres->changeable = true;
					}
					else if (constantUnchangeableOps.count(refop.arg_s) && argres) {
						argres->changeable = false;
					}
					else if (inheritedChangeableOps.count(refop.arg_s) && argres) {
						argres->changeable = max(arg1 ? arg1->changeable : 0, arg2 ? arg2->changeable : 0);
					}*/
					code.scopeFKA = scopeFKAdes;

					if (arg1)code.setArg(0, *arg1, true, true);
					if (arg2)code.setArg(1, *arg2, true, true);
					if (argres)code.setArg(3, *argres, true, true);

					/*code.assemblyFormula.flagBit.arg1_flag = arg1->argFlag;
					code.assemblyFormula.quaternion.arg1_fka = arg1->arg_fka;
					code.assemblyFormula.quaternion.arg1_i = arg1->arg_i;
					code.assemblyFormula.quaternion.arg1_s = arg1->arg_s;
					code.assemblyFormula.flagBit.formalArgFlag[0] = arg1->formalArg;
					code.assemblyFormula.flagBit.changeable[0] = arg1->changeable;
					code.assemblyFormula.flagBit.a1s = arg1->asc;
					code.assemblyFormula.flagBit.arg2_flag = arg2->argFlag;
					code.assemblyFormula.quaternion.arg2_fka = arg2->arg_fka;
					code.assemblyFormula.quaternion.arg2_i = arg2->arg_i;
					code.assemblyFormula.quaternion.arg2_s = arg2->arg_s;
					code.assemblyFormula.flagBit.formalArgFlag[1] = arg2->formalArg;
					code.assemblyFormula.flagBit.changeable[1] = arg2->changeable;
					code.assemblyFormula.flagBit.a2s = arg2->asc;
					code.assemblyFormula.flagBit.result_flag = argres->argFlag;
					code.assemblyFormula.quaternion.result_fka = argres->arg_fka;
					code.assemblyFormula.quaternion.result_i = argres->arg_i;
					code.assemblyFormula.quaternion.result_s = argres->arg_s;
					code.assemblyFormula.flagBit.formalArgFlag[3] = argres->formalArg;
					code.assemblyFormula.flagBit.changeable[3] = argres->changeable;
					code.assemblyFormula.flagBit.res = argres->asc;*/
					return codeFKAdes.addr;
		}
		else if (code.assemblyFormula.flagBit.type == COOL_M
			&& ((code.assemblyFormula.flagBit.operator_flag == M_Bs
				&& (code.assemblyFormula.quaternion.operator_s == ":"
					&& (code.assemblyFormula.quaternion.arg2_s
						== "out"
						/*todo:add new attribute exclude here*/

						)))
				|| (code.assemblyFormula.flagBit.operator_flag == M_Bs
					&& (code.assemblyFormula.quaternion.operator_s
						== "$"
						|| code.assemblyFormula.quaternion.operator_s
						== "#")

					/*todo:add new attribute operator exclude here*/
					))) {

			throw exception("this part shouldn't have been accessed, check why attribute assignment code not be discarded");
			/**
			 * 代码替换过程中将cdtfrom中的out、#、$属性赋值语句去掉
			 */
			code.assemblyFormula.flagBit.execute = Du;
			ArgDec& arg1 = am_ref_cmp[code[0]];
			ArgDec& argres = am_ref_cmp[code[3]];

			Intg argNullNum = 0;
			if (arg1 == argnull) {
				arg1.argFlag = S_Dz;
				arg1.arg_fka = codeFKAdes;
				arg1.formalArg = T_;
				arg1.changeable = code.assemblyFormula.flagBit.changeable[0];
				//arg1.asc = scopeFKAdes;
				++argNullNum;
			}

			if (argres == argnull) {
				argres.argFlag = S_Dz;
				argres.arg_fka = codeFKAdes;
				argres.formalArg = T_;
				argres.changeable = code.assemblyFormula.flagBit.changeable[3];
				//argres.asc = scopeFKAdes;
				++argNullNum;
			}
			if (argNullNum > 1) {

				//					cout
				//						<< "replaceLine err::more than one argnull to replace. argNullNum:["
				//						<< argNullNum << "]" << endl;
				//#if debug && _WIN32
				//					{
				//						static int ignoreCount = 0;
				//						// Assuming info is a std::string, convert to std::wstring
				//						std::string info = "error exit"; // Replace with actual info
				//						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				//						std::wstring winfo(info.begin(), info.end());
				//						#if WIN_DEBUG_OUTPUT
//				OutputDebugString(winfo.c_str());
//#endif
//				cout << info << endl;
				//						ignoreCount++;
				//					}
				//#endif
				//					assert(false);exit(-1);
			}
			Arg::infer_changeable(&arg1, nullptr, refop, &argres);
			code.scopeFKA = scopeFKAdes;

			code.assemblyFormula.flagBit.arg1_flag = arg1.argFlag;
			code.assemblyFormula.quaternion.arg1_fka = arg1.arg_fka;
			code.assemblyFormula.quaternion.arg1_i = arg1.arg_i;
			code.assemblyFormula.quaternion.arg1_s = arg1.arg_s;
			code.assemblyFormula.flagBit.formalArgFlag[0] = arg1.formalArg;
			code.assemblyFormula.flagBit.changeable[0] = arg1.changeable;
			code.assemblyFormula.flagBit.a1s = arg1.asc;

			code.assemblyFormula.flagBit.result_flag = argres.argFlag;
			code.assemblyFormula.quaternion.result_fka = argres.arg_fka;
			code.assemblyFormula.quaternion.result_i = argres.arg_i;
			code.assemblyFormula.quaternion.result_s = argres.arg_s;
			code.assemblyFormula.flagBit.formalArgFlag[3] = argres.formalArg;
			code.assemblyFormula.flagBit.changeable[3] = argres.changeable;
			code.assemblyFormula.flagBit.res = argres.asc;
			return codeFKAdes.addr;
		}
		return codeFKAdes.addr;
	}
	bool SingleTFIntegrator::replaceVar() {
		Intg s = cdtmid.addrdq.size();
		/*vector<Addr> av = split(mindes, maxdes, s);
		Addr maxaddr_in_argcmp;
		for (auto& p : am_ref_cmp) {
			if (p.second.argFlag == S_Dz && p.second.arg_fka.addr > maxaddr_in_argcmp) {
				maxaddr_in_argcmp = p.second.arg_fka.addr;
			}
		}
		if (av.size() > 0 && av.back() < maxaddr_in_argcmp) {
			av.back() = maxaddr_in_argcmp;
		}*/


		/*Addr cdtdesTreeHandleAddr = cdtdes->getLast(COOL_M, maxdes);
		Arg cdtdesTreeHandleArg((*cdtdes)[cdtdesTreeHandleAddr][3]);
		Arg cdtmidTreeHandleArg(cdtmid.codedq.back()[3]);
		am_ref_cmp[cdtmidTreeHandleArg] = cdtdesTreeHandleArg;*/
		auto lowbound = mindes;
		for (Intg pos = 0; pos < s; pos++) {

			Addr newCodeAddr = replaceLine(cdtmid, pos, scopeFKAdes,
				lowbound, maxdes);
			cdtmid.addrdq[pos] = newCodeAddr;
			lowbound = newCodeAddr;
		}
		return true;
	}
	/// <summary>
	/// integrate one code snippet into another. It will correctly set all the flagbits(changeable, asc, template function, and so on).
	/// </summary>
	/// <returns></returns>
	bool SingleTFIntegrator::integrate() {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "SingleTFIntegrator::integrate()" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif

#if debug && _WIN32
		{
			stringstream ss;
			ss << bbhline;
			ss << "SingleTFIntegrator::integrate()::cdtdes:" << endl;
			ss << cdtdes->toStrg();
			ss << bhline;
			ss << "cdtfrom:" << endl;
			ss << cdtfrom->toStrg();
			ss << bhline;
			ss << calign("Addr::mindes(" + mindes.toStrg() + ")", 24) << calign("Addr::maxdes(" + maxdes.toStrg() + ")", 24) << calign("Addr::trfrom(" + trfrom.toStrg() + ")", 24) << "\n";
			ss << bhline;
			ss << "am_ref_cmp:" << "\n";
			ss << toStrg<Arg, Arg>(this->am_ref_cmp) << endl;
			ss << bhline;
			ss << "addrm" << endl;
			ss << toStrg<Addr, Addr>(this->addrm) << endl;
			ss << bbhline;



			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = ss.str(); // Replace with actual info
			info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
			std::cout << info << endl;
			std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
			OutputDebugString(winfo.c_str());
#endif

			ignoreCount++;
		}
#endif


		//delete replaced code
		if (addrm.size() == 0) {

			Intg posmin = 0;
			if (mindes != addrnull && mindes >= cdtdes->addrdq.front()) {

				posmin = getAddrPosition(cdtdes->addrdq, mindes);
				if (posmin == -1) {

					{
						static int ignorecount = 0;
						std::cout << "SingleTFIntegrator::integrate()"
							<< " err::posmin == -1, exit(-1) "
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
			else {
				posmin = -1;
			}
			Intg posmax = INT_MAX - 10000;
			if (maxdes != addrnull && maxdes <= cdtdes->addrdq.back()) {
				posmax = getAddrPosition(cdtdes->addrdq, maxdes);
				if (posmax == -1) {

					cout
						<< "SingleTFIntegrator::integrate err::posmax == -1,exit-1"
						<< "(" << __FILE__ << ":" << __LINE__ << ")"
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
			else {
				posmax = cdtdes->addrdq.size();
			}

			cdtdes->addrdq.erase(cdtdes->addrdq.begin() + posmin + 1,
				cdtdes->addrdq.begin() + posmax);
			cdtdes->codedq.erase(cdtdes->codedq.begin() + posmin + 1,
				cdtdes->codedq.begin() + posmax);
		}
		else {

			for (auto& it : addrm) {
#if debug
				static Intg ignorecount = 0;
				cout << "SingleTFIntegrator::integrate2.4 it:["
					<< it.first.toStrg() << "]\tignorecount:["
					<< ignorecount++ << "]" << endl;
#endif
				cdtdes->eraseCode(it.first, 0);
			}
		}


		Intg pos_ = getLastAddrPosition(cdtdes->addrdq, maxdes);
		if (pos_ > 0) {
			auto mindes_ = cdtdes->addrdq[pos_ - 1];
			if (mindes < mindes_) {
				mindes = mindes_;

			}
		}



		//replace vars to get code snippet intert (cdtmid)
		if (am_ref_cmp.size() != 0) {
			replaceVar();
			FKA& scopeFrom = (*cdtfrom)[trfrom].scopeFKA;
			if (sth && (*sth)[scopeFrom].functionFlag == FN) {
				for (auto& cdmid : cdtmid.codedq) {
					cdmid.scopeFKA = scopeFKAdes;
					cdmid.assemblyFormula.flagBit.execute = T_;
					cdmid.assemblyFormula.flagBit.tfFKA = scopeFrom;
					cdmid.assemblyFormula.flagBit.functionHandleFlag = F_;
				}
				cdtmid.codedq.back().assemblyFormula.flagBit.functionHandleFlag =
					T_;

			}
			else if (sth
				&& ((*sth)[scopeFrom].functionFlag == FB
					|| (*sth)[scopeFrom].functionFlag == RFB)) {
				for (auto& cdmid : cdtmid.codedq) {
					cdmid.scopeFKA = scopeFKAdes;
					cdmid.assemblyFormula.flagBit.functionHandleFlag = F_;
				}
			}
			else {
#if debug
				cout << "SingleTFIntegrator::integrate ,sth==nullptr" << endl;
#endif
			}
		}




		//calculate the position insert.
		Intg posInsert = 0;
		if (cdtdes->addrdq.size() > 0 && maxdes != addrnull
			&& maxdes <= cdtdes->addrdq.back()) {
			posInsert = getAddrPosition(cdtdes->addrdq, maxdes) >= 0 ? getAddrPosition(cdtdes->addrdq, maxdes) : getNextAddrPosition(cdtdes->addrdq, maxdes);
			if (posInsert == -1) {

				cout
					<< "SingleTFIntegrator::integrate err:: posInsert == -1,exit-1"
					<< "(" << __FILE__ << ":" << __LINE__ << ")" << endl;
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

			posInsert = cdtdes->addrdq.size();
		}










#if debug && _WIN32
		{
			stringstream ss;
			ss << bhline;
			ss << __FUNCTION__ << "::raw stfi integrated codetable(cdtfrom --replacevars --> cdtmid):" << endl;
			ss << cdtmid.toStrg();
			ss << "cdtdes->\n";
			ss << cdtdes->toStrg();
			ss << bhline;
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = ss.str(); // Replace with actual info
			info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
			std::cout << info << std::endl;
			std::wstring winfo(info.begin(), info.end());
			OutputDebugString(winfo.c_str());
			ignoreCount++;
		}
#endif



		//check code flags and modify codefilekey of code insert(cdtmid)
		cdtmid.checkFileKey();
		cdtmid.deleteInaccessibleSubtree();
		cdtmid.checkConstraints();
		cdtmid.checkChangeable();
		for (auto& cd_ : cdtmid.codedq) {
			cd_.fileKey = cdtdes->fileKey;
		}


		//modify the root node of cdtmid to match attachment requirement.
		if (attachmentRootNode != argnull) {
			//插入的树有给定的插入位置，则需要替换该树的根节点为attachmentRootNode

			cdtmid.codedq.back().setArg(3, this->attachmentRootNode, false, true);
			if (attachmentRootNode.argFlag == S_Dz && cdtmid.addrdq.back() < attachmentRootNode.arg_fka.addr) {
				cdtmid.addrdq.back() = attachmentRootNode.arg_fka.addr;
			}

		}

		//insert new code snippet (cdtmid)


#ifdef __linux__
		cdtdes->addrdq.insert<std::deque<Addr, allocator<Addr>>::iterator,
			std::deque<Addr, allocator<Addr>>::iterator>(
				cdtdes->addrdq.begin() + posInsert, cdtmid.addrdq.begin(),
				cdtmid.addrdq.end());
		cdtdes->codedq.insert<std::deque<Code, allocator<Code>>::iterator,
			std::deque<Code, allocator<Code>>::iterator>(
				cdtdes->codedq.begin() + posInsert, cdtmid.codedq.begin(),
				cdtmid.codedq.end());
#elif __WIN32__ || _WIN32
		std::deque<COOLANG::Addr>::iterator addrit = cdtdes->addrdq.begin()
			+ posInsert;
		cdtdes->addrdq.insert(addrit, cdtmid.addrdq.begin(),
			cdtmid.addrdq.end());
		std::deque<COOLANG::Code>::iterator codeit = cdtdes->codedq.begin()
			+ posInsert;
		cdtdes->codedq.insert(codeit, cdtmid.codedq.begin(),
			cdtmid.codedq.end());
#endif


#if debug && _WIN32
		{
			static int ignoreCount = 0;
			stringstream ss;
			ss << bbhline;
			ss << __FUNCTION__ << "::cdtdes return:\n";
			ss << cdtdes->toStrg();
			ss << bbhline;
			// Assuming info is a std::string, convert to std::wstring
			std::string info = ss.str(); // Replace with actual info
			info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
			std::cout << info << std::endl;
			std::wstring winfo(info.begin(), info.end());
			OutputDebugString(winfo.c_str());
			ignoreCount++;
		}
#endif

		return true;
	}
	/**
			 *
			 * @return return the BDDB_value n and groundingstate n
			 */
	pair<Numb, const GroundingState*> GroundingStateSilo::getNext() {

		if (activeSilo.size() > 0) {
			auto p = pair<Numb, const GroundingState*>(activeSilo.rbegin()->first,
				activeSilo.rbegin()->second);
			//garbageSilo.insert(p);
			activeSilo.erase(prev(activeSilo.end()));
			return p;
		}
		else {

			return pair<Numb, const GroundingState*>(
				std::numeric_limits<double>::lowest(), nullptr);

		}


	}
	/**
		 * @brief get BDDB_value n and groundingstate n, by function vector n, codeTable n and grounding state n-1.
		 * then insert the pair<BDDB_value n, grounding state n> into activesilo.
		 * codeTable n is yielded outside this function.
		 * this function is focusing on
		 * @param tffka n
		 * @param organization function vector n
		 * @param position, the position to apply function
		 *
		 * @param domain_resolved: the number of domains of the cdt in the gs are determined (at least one output from nns with indomain == true, more than one may indicating some predictions are wrong. )
		 * @param indomain: the cdt in gs is at corresponding domain indicated by tffka.filekey.
		 * @param policy, the position that nn suggests to apply a function to.
		 * @param expr_ n
		 * @param ms n
		 * @param ac n
		 * @param advance, from nn, whether this step should advance (step for next gs)
		 * @param rpsign, from nn, this step should receive a negative reward or positive one
		 * @param rule, from nn, this function should be a
		 * @param lastGroundingState n-1 groundingState
		 * @param biasCoefficient: an exceptionally small number, used when two grounding states share a same BDDB_value, the grounding state with bigger biasEnergy(biasCoefficient*position) tackled first.
		 * to be exactly, the grounding state whose position is smaller, or may closer to root in AST, is tackled preferentially.
		 * @return
		 */
	bool GroundingStateSilo::add(const pair<Strg, Strg>& file_class_name, const pair<Strg, Intg>& filekey_stage, TemplateFunctionTableHash* tfth, const FKA& tffka,
		const TFInfoMat& organization, const Intg& position,
		const Intg& domain_resolved, const bool& indomain,
		const bool& succeed, const Numb& ac_policies, const Numb& policy, const Numb& ac_advance, const Numb& advance, const Numb& ac_rpsign, const Numb& rpsign, const Numb& ac_rpval, const Numb& rpval, const Numb& ac_rule, const bool& rule,
		const Numb& ac, const MatchState& ms, ExpressionHandlerPtr expr_, bool duplicate_expr_,
		const GroundingState* lastGroundingState, const Numb& biasCoefficient /*=
		1e-10*/) {
		const Numb E0 = 1.0;
		const Numb ci = 1.0;

#if debug
		{
			static int ignorecount = 0;
			std::cout
				<< "add(const pair<Strg,Strg>&, const FKA&, const TFInfoMat&, const int&, const double&, const double&, const double&, const double&, const MatchState&, const CodeTable&, GroundingState*)"
				<< " in " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif

#if debug && _WIN32
		{
			stringstream ss;
			ss << bbhline;
			ss << "GroundingStateSilo::add::" << "\n";
			ss << "position:" << position << "\tFKA:" << expr_->expr_cdt.addrdq[position].toStrg() << "\n";
			ss << "matchedfunction:" << tffka.toStrg() << endl;
			ss << bhline;
			ss << "expr_:" << endl;
			ss << expr_->toBrief();
			ss << bhline;
			ss << "lastGroundingState::" << endl;
			if (lastGroundingState != nullptr) {
				ss << lastGroundingState->toStrg();

			}
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


		GroundingState gs(expr_, duplicate_expr_); //create grounding state for n;


		gs.statemat = lastGroundingState->statemat;
		gs.BDDB_value = lastGroundingState->BDDB_value;
		gs.tffka = tffka;
		gs.rule = (tffka.addr != addrnull && tfth != nullptr) ? (*tfth)[tffka].integrate : false;
		gs.policy = policy;
		gs.setMatchState(ms, tfth, tffka);
		gs.lastgs = lastGroundingState;
		gs.position = position;
		gs.initialState = !lastGroundingState;

		if (filekey_stage.second != -1) {//handle logicjump
			// Find the position of the first element of filekey_stage in knowledge_domain_list
			auto it = find(gs.statemat.knowledge_domain_list.begin(), gs.statemat.knowledge_domain_list.end(), filekey_stage.first);

			if (it != gs.statemat.knowledge_domain_list.end()) {
				// Get the index
				size_t index = distance(gs.statemat.knowledge_domain_list.begin(), it);

				// Set the second element of filekey_stage to the corresponding position in step_list
				gs.statemat.step_list[index] = filekey_stage.second;
			}
		}
		//处理最初的groundingState
		if (gs.initialState == true) {
			gs.BDDB_value = 0;
			auto&& insert_res = siloAll.insert(move(gs));


			if (insert_res.second == true) {
				activeSilo.insert(
					make_pair(insert_res.first->BDDB_value, (&(*insert_res.first)))
				);
			}
			return true;
		}
		//处理成功的Groundings(空表不算成功)
		gs.finalState = gs.expr->expr_cdt.size() > 0;
		for (auto& cd_ : gs.expr->expr_cdt.codedq) {
			if (cd_.assemblyFormula.flagBit.tfFKA == fkanull) {
				gs.finalState = false;
				break;
			}
		}
		if (gs.finalState == true) {
			gs.succeed = true;
			// mark the gs path to success, and add all success gs to set
			function<bool(const CodeTable&, const CodeTable&)> codeTableComparatorLT = [](const CodeTable& lhs, const CodeTable& rhs) -> bool {
				auto& lcdt = const_cast<CodeTable&>(lhs);
				auto& rcdt = const_cast<CodeTable&>(rhs);
				if (lcdt.addrdq < rcdt.addrdq) {
					return true;
				}
				else if (lcdt.addrdq > rcdt.addrdq) {
					return false;
				}// addrdq == addrdq

				ArgComparatorLT aclt(acm::name | acm::asc | acm::changeable | acm::compatible | acm::constraint | acm::immediate);
				auto codeComparatorLT = [&aclt](const Code& lcd, const Code& rcd) -> bool {
					if (lcd.assemblyFormula.flagBit.tfFKA < rcd.assemblyFormula.flagBit.tfFKA) {
						return true;
					}
					else if (lcd.assemblyFormula.flagBit.tfFKA > rcd.assemblyFormula.flagBit.tfFKA) {
						return false;
					}

					if (lcd.assemblyFormula.flagBit.functionHandleFlag < rcd.assemblyFormula.flagBit.functionHandleFlag) {
						return true;
					}
					else if (lcd.assemblyFormula.flagBit.functionHandleFlag > rcd.assemblyFormula.flagBit.functionHandleFlag) {
						return false;
					}

					for (int i = 0; i < 4; ++i) {
						auto&& lcdarg = lcd[i];
						auto&& rcdarg = rcd[i];
						if (aclt(lcdarg, rcdarg)) {
							return true;
						}
						else if (aclt(rcdarg, lcdarg)) {
							return false;
						}
						else {

							//lcdarg == rcdarg
							continue;
						}

					}

					//lcd == rcd
					return false;

					};
				for (int i = 0; i < lcdt.size(); ++i) {
					auto& lcd = lcdt[i];
					auto& rcd = rcdt[i];

					if (codeComparatorLT(lcd, rcd)) {
						return true;
					}
					else if (codeComparatorLT(rcd, lcd)) {
						return false;
					}
					else {
						//lcd == rcd
						continue;

					}
				}
				//lcdt == rcdt
				return false;

				};
			set<CodeTable, decltype(codeTableComparatorLT)> success_gs_cdt(codeTableComparatorLT);
			auto* lsgs = gs.lastgs;
			while (lsgs && lsgs->initialState == false) {
				lsgs->setGroundingResult(true);
				success_gs_cdt.insert(lsgs->expr->expr_cdt);
				lsgs = lsgs->lastgs;
			}
			gs.BDDB_value = (numeric_limits<Numb>::max)();
			auto&& insert_res = siloAll.insert(move(gs));
			if (insert_res.second == true) {
				resultSilo.insert(
					make_pair(insert_res.first->BDDB_value, &(*insert_res.first)));
			}
			//return if doesnt need to export modeling data
			if (setting->exportModelingData == false) {
				return true;
			}
			// mark the failure gs
			for (auto failure_gs_ : this->garbageSilo) {
				failure_gs_.second->setGroundingResult(false, &success_gs_cdt);
			}
			for (auto failure_gs_ : this->abortedSilo) {
				failure_gs_.second->setGroundingResult(false, &success_gs_cdt);
			}
			// Mark other step chains in activeSilo that have skipped steps relative to the successful step chain as wrong
			map<const GroundingState*, set<const GroundingState*>> parent_children_gs_map;
			set<const GroundingState*> success_gs_set;
			set<const GroundingState*> failure_gs_set;
			//if gs2 have skipped steps relative to gs1
			auto jumpstep = [&](const GroundingState* gs1, const GroundingState* gs2)->bool {
				if (this->setting->exportStepSkippingTrainingData == false) {
					return false;
				}
				if (gs1 == gs2) {
					return false;
				}
				for (auto kd : gs2->statemat.knowledge_domain_list) {
					/*if (kd == FILEKEYBUILTIN) {
						continue;
					}*/
					auto stp1 = gs1->statemat.getStep(kd);
					auto stp2 = gs2->statemat.getStep(kd);
					if (stp2 > stp1) {
						return true;
					}
				}

				return false;
				};
			set<const GroundingState*>  recur_parent_failure_gs_set;
			function<void(const GroundingState*)> set_children_failure_recur;
			/**
			set the children of the gs failure, not the parent self.
			*/
			set_children_failure_recur = [&](const GroundingState* parent)->void {
				if (parent_children_gs_map.count(parent) == 0) {
					return;
				}
				auto children_set = parent_children_gs_map[parent];
				set<const GroundingState*> pending_children_gs_set;
				for (auto child : children_set) {
					if (child->succeed == 1) {
						return;
					}
					if (child->succeed == 0 && recur_parent_failure_gs_set.count(child) > 0) {
						continue;
					}
					else {
						pending_children_gs_set.insert(child);
					}
				}

				for (auto child : pending_children_gs_set) {
					const_cast<GroundingState*>(child)->succeed = 0;
					set_children_failure_recur(child);
					recur_parent_failure_gs_set.insert(child);
				}

				};
			for (auto& pending_gs_ : this->siloAll) {
				auto parent = pending_gs_.lastgs;
				auto child = &pending_gs_;
				if (parent != nullptr) {
					parent_children_gs_map[parent].insert(child);
				}
				if (child->succeed == 1) {
					success_gs_set.insert(child);
				}
				else if (child->succeed == 0) {
					failure_gs_set.insert(child);
				}
			}
			for (auto gs_suc : success_gs_set) {
				if (gs_suc->lastgs != nullptr && parent_children_gs_map.count(gs_suc->lastgs) != 0) {
					auto& sibling_gs_set = parent_children_gs_map[gs_suc->lastgs];
					for (auto& sibling : sibling_gs_set) {
						if (sibling->succeed != 1 && jumpstep(gs_suc, sibling) == true) {
							set_children_failure_recur(sibling);
							recur_parent_failure_gs_set.insert(sibling);
						}
					}
				}
			}

			return true;
		}

		//这里处理statemat n-1与tfinfo（organization/ reward）n的合并，works only when userPromptEnabled
		bool addsucceed = true;
		Intg bddb_o_t = 0;
		Numb rp = 0;
		Intg actual_advance = 0;
		Numb ra = 0;
		Numb bddb_o = 0;

		if (setting->userPromptEnabled && setting->chainEnabled) {
			//from 1 to b
			auto geometricSeriesSum = [](double a, int b) {
				if (a == 1) {
					return static_cast<double>(b);
				}
				else {
					return a * (1 - std::pow(a, b)) / (1 - a);
				}
				};

			auto&& addsucceed__bddb_o_t__rp = gs.statemat.add(file_class_name,
				organization);
			addsucceed = get<0>(addsucceed__bddb_o_t__rp);
			bddb_o_t = get<1>(addsucceed__bddb_o_t__rp);
			rp = get<2>(addsucceed__bddb_o_t__rp);
			actual_advance = get<3>(addsucceed__bddb_o_t__rp);
			/*ra = max(setting->BDDB__r_a__k_0 * abs(rp), setting->BDDB__r_a_base);*/
			bddb_o = bddb_o_t > 0 ?
				(-setting->BDDB__o__k_1 * pow(setting->BDDB__o__k_2, bddb_o_t))
				: (
					(lastGroundingState != nullptr && setting->BDDB__o__k_2 != 0 && lastGroundingState->bddb_o_t != 0) ?
					(setting->BDDB__o__k_0 / pow(setting->BDDB__o__k_2, lastGroundingState->bddb_o_t) + setting->BDDB__o__k_1 * geometricSeriesSum(setting->BDDB__o__k_2, lastGroundingState->bddb_o_t)) : setting->BDDB__o__k_0
					);
		}
		else if (/*setting->userPromptEnabled &&*/ setting->chainEnabled == false) {
			//disable chain
			addsucceed = true;
			actual_advance = advance;
			auto&& knowledge_domain = split(file_class_name.first, "#")[0]
				+ (file_class_name.second != "" ? " " : "")
				+ file_class_name.second;

			bool domain_adopt = false;
			auto it_knowledge_domain = gs.statemat.knowledge_domain_list.begin();
			for (; it_knowledge_domain != gs.statemat.knowledge_domain_list.end(); ++it_knowledge_domain) {
				if (it_knowledge_domain.operator *() == knowledge_domain) {
					domain_adopt = true;
					break;
				}
			}

			if (domain_adopt == false) {
				gs.statemat.knowledge_domain_list.push_back(knowledge_domain);
				gs.statemat.step_list.push_back(0);
				gs.statemat.matrix.list2.push_back(deque<Numb>{rp});
				gs.statemat.BDDB_o_t_list.push_back(0);
			}
			gs.statemat.rule_domain_add_stack.push_back(knowledge_domain);

			for (auto& step_ : gs.statemat.step_list) {
				step_ = 0;
			}
			if (tffka.addr < addrnull) {
				rp = toNumb(BUILTIN_FUN_INFO);
			}
			for (auto reward_ : organization.list2[0]) {
				if (abs(reward_) > abs(rp)) {
					rp = reward_;
				}
			}
			gs.statemat.accumulated_steps += 1;
			gs.statemat.tfinfo_add_stack.push_back(deque<Numb>{rp});
		}
		if (setting->neuralNetworkEnabled) {
			ra = setting->BDDB__r_a_base;
		}


		//成功gs计算BDDB_value
		bool experiment = true;
		if (experiment) {
			if (setting->userPromptEnabled) {

				if (addsucceed == false) {
					return false;
				}


				if (setting->neuralNetworkEnabled && domain_resolved == true) {//if the neural network is seriously undertrained, don't use it

					//user y neural y


					if (setting->nnRewardEnabled && succeed == true && policy == position && rpval != 0) {
						//rewardEnabled

						bool align = (rpval != 0 && (rpval / rp) < 1.1 && (rpval / rp) > 0.9 && abs(rpval - rp) < 0.1 && (actual_advance == advance)) && ac > 0.8 && rule == gs.rule;
						auto counter = MultiTFIntegrator::AST_transformation_count;

						if (align == true) {
							// more accurate, more reward
							gs.BDDB_value = gs.BDDB_value
								+ rp + ac * (abs(rpval));
						}
						else if (align == false && tffka.addr > addrnull) {
							//if more than one domains are determined (include this one), and this one doesn't align with the tf, disable both user reward and neural's positive reward.
							if (rpval < 0) {
								gs.BDDB_value = gs.BDDB_value + rpval;
							}
							else {
								gs.BDDB_value = gs.BDDB_value;
							}
						}
						else {
							// built-in functions have no nn, so don't punish them
							MultiTFIntegrator::eliminated_constraints_count++;
							gs.BDDB_value = gs.BDDB_value
								+ rp;
						}
					}
					else if (setting->nnPenaltyEnabled && succeed == false && tffka.addr > addrnull) { // don't punish built-in function application
						//penaltyEnabled
						MultiTFIntegrator::punished_constraints_count++;

						if (indomain == true && succeed == false) {
							//not success penalty
							gs.BDDB_value = gs.BDDB_value
								- abs(rp) - 10;
						}
						else {

							if (domain_resolved == true && indomain == false) {
								//not indomain penalty
								gs.BDDB_value = gs.BDDB_value
									- abs(rp) - 10;
							}
							else {
								//unknown domain
								gs.BDDB_value = gs.BDDB_value
									+ rp;
							}

						}

					}
					else {
						gs.BDDB_value = gs.BDDB_value
							+ rp;
					}

				}
				else {
					//user Y neural N (or domain unresolved)
					gs.BDDB_value = gs.BDDB_value
						+ rp;
				}



			}
			else {
				if (setting->neuralNetworkEnabled && domain_resolved == true) {//if the neural network is seriously undertrained, don't use it

					//user n neural y

					if (setting->nnRewardEnabled && succeed == true) {
						//rewardEnabled
						bool align = (rpval != 0 && (rpval / rp) < 1.1 && (rpval / rp) > 0.9 && abs(rpval - rp) < 0.1 && (actual_advance == advance)) && ac > 0.8 && rule == gs.rule;

						auto counter = MultiTFIntegrator::AST_transformation_count;

						if (align) {
							// more accurate, more reward
							gs.BDDB_value = gs.BDDB_value
								+ 5;
						}
						else if (align == false && tffka.addr > addrnull) {
							//if more than one domains are determined (include this one), and this one doesn't align with the tf, disable the user reward and neural reward
							gs.BDDB_value = gs.BDDB_value - 1;
						}
						else {
							//built-in functions dont generate punish or reward
							gs.BDDB_value = gs.BDDB_value + 0.01;
							MultiTFIntegrator::eliminated_constraints_count++;
							//do nothing
						}
					}
					else if (setting->nnPenaltyEnabled && succeed == false && tffka.addr > addrnull) { // don't punish built-in function application
						//penaltyEnabled
						MultiTFIntegrator::punished_constraints_count++;

						if (indomain == true && succeed == false) {
							//not success penalty
							gs.BDDB_value = gs.BDDB_value
								- 10;
						}
						else {

							if (domain_resolved == true && indomain == false) {
								//not indomain penalty
								gs.BDDB_value = gs.BDDB_value
									- 10;
							}
							else {
								//do nothing
							}

						}

					}


				}
				else {

					//user n neural n

					//dijkstra

					gs.BDDB_value = gs.BDDB_value - 1;

				}

			}
		}
		else {



		}

		gs.bddb_o_t = bddb_o_t;
		//添加其他未被淘汰的中间gs
		if (siloAll.size() < groundingStateSiloMaxCapacity) {
			auto&& insert_res = siloAll.insert(move(gs));
			if (insert_res.second == true) {
				activeSilo.insert(
					make_pair(insert_res.first->BDDB_value, &insert_res.first.operator*()));
				MultiTFIntegrator::explored_space_count++;
			}
			return true;
		}

		{
			static int ignorecount = 0;
			std::cerr
				<< "add(const pair<Strg,Strg>&, const FKA&, const TFInfoMat&, const int&, const double&, const double&, const double&, const double&, const MatchState&, const CodeTable&, const GroundingState*)"
				<< "run out off groundingStateSiloMaxCapacity, exit(-1) "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#if _WIN32
		{
			static int ignoreCount = 0;
			// Assuming info is a std::string, convert to std::wstring
			stringstream ss;
			auto rit = activeSilo.rbegin();
			ss << bbhline;
			ss << "Error exit:" << __FUNCTION__ << ":\n";
			ss << "CodeTable:\n:" << this->scpth->cdth->toStrg() << bhline;
			ss << "groundingStateSiloMaxCapacity:" << groundingStateSiloMaxCapacity << endl;
			ss << "siloAll:size:" << siloAll.size() << "\n";
			ss << "activeSilo:" << rit->first << "\n";
			ss << rit->second->toStrg(this->scpth) << endl;
			int i = 0;
			while (++i < min(100, activeSilo.size()) && ((++rit)->second) != nullptr) {
				ss << bhline;
				ss << "activeSilo:" << rit->first << "\n";
				ss << rit->second->toStrg(this->scpth) << endl;
			}
			ss << bbhline;
			std::string info = ss.str(); // Replace with actual info
			info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
			std::wstring winfo(info.begin(), info.end());
			OutputDebugString(winfo.c_str());
			std::cout << info << endl;
			ignoreCount++;
			fstream f;
			f.open("important.txt", ios::app);
			if (f.is_open()) {
				f << info << endl;
				f.close();
			}
			ignoreCount++;
		}
#endif
		THROW_REASONING_EXCEPTION(ReasoningErrorType::RunOutOfResources, "Reasoning Failure! Run out of reasoning resources! Task abort!");

		assert(false); exit(-1);
	}

	/**
		 *
		 * @param organization
		 * @param gs
		 * @param onIntegrateFinish
		 * @return
		 */
	GroundingStateSilo& GroundingStateSilo::reset(ExpressionHandlerPtr expr_, bool duplicate_expr_,
		const bool (*onIntegrateFinish)(GroundingStateSilo* p)) {

		activeSilo.clear();
		resultSilo.clear();
		garbageSilo.clear();
		siloAll.clear();
		//处理最初的grouindingState
		GroundingState gs(expr_, duplicate_expr_);
		gs.initialState = true;
		gs.lastgs = nullptr;
		gs.BDDB_value = 0;
		auto&& insert_res = siloAll.insert(move(gs));
		activeSilo.insert(make_pair(insert_res.first->BDDB_value, &insert_res.first.operator*()));

		return (*this);
	}


	GroundingStateSilo::GroundingStateSilo(Setting* setting, ScopeTableHash* scpth) {
		this->setting = setting;
		this->scpth = scpth;
	}

	void GroundingStateSilo::setMaxCapacity(Intg groundingStateSiloMaxCapacity) {
		this->groundingStateSiloMaxCapacity = groundingStateSiloMaxCapacity;
	}

	set<Strg> GroundingStateSilo::getKnowledgeDomains()
	{

		set<Strg> domains = {};
		set<Strg> filekeys = {};
		for (auto& p : this->siloAll) {
			filekeys.insert(p.tffka.fileKey);
		}
		for (auto fk_ : filekeys) {
			domains.insert(split(fk_, "#")[0]);
		}
		domains.erase("");
		return domains;


	}
	/*
		@attention: call after setting all gs bits in this silo.
	*/
	void GroundingStateSilo::bufferDataset(const Strg& outputFileName, const set<Strg>& knowledge_domains, const bool& duplicated,
		const bool& export_succeed, const bool& export_failed, const CodeTableHash* cdth,
		const SystemTableHash* systh_) {
		if (setting->exportModelingData == false) {
			return;
		}
		if (siloAll.size() == 0) {
			return;
		}
		auto get_problem_domains = [&]() {

			set<Strg> problem_domains;
			if (this->resultSilo.size() > 0) {
				problem_domains = this->resultSilo.begin()->second->getknowledgeDomains();
			}
			else if (this->siloAll.size() >= this->groundingStateSiloMaxCapacity) {
				map<Strg, Intg> function_invocation_distribution;
				Intg total_invocation = 0;
				for (auto& gs_ : this->siloAll) {
					if (gs_.succeed != 0 && gs_.tffka.fileKey != "") {
						if (function_invocation_distribution.count(gs_.tffka.fileKey) == 0) {
							function_invocation_distribution[gs_.tffka.fileKey] = 0;
						}
						++function_invocation_distribution[gs_.tffka.fileKey];
						++total_invocation;
					}
				}

				for (auto p : function_invocation_distribution) {
					if (p.second / total_invocation > 0.8) {
						problem_domains.insert(split(p.first, "#")[0]);
					}

				}

			}


			return problem_domains; };
		auto problem_domains = get_problem_domains();
		for (auto& gs : siloAll) {
			// export success gs first, then export failure ones.
			gs.bufferTrainingData(outputFileName, knowledge_domains, problem_domains, duplicated, export_succeed, export_failed, cdth, systh_);
		}
		//fstream fs(outputFileName, std::ios::in | std::ios::app);
		//fs.seekp(0, fs.end);

		//if (fs.tellg() > 0) {

		//	fs.seekg(-1, fs.end);
		//	char lastCharacter;
		//	fs.get(lastCharacter);
		//	if (lastCharacter != ']') {
		//		fs.close();
		//		fs.open(outputFileName, std::ios::out);
		//		fs.put('[');
		//	}
		//	else {
		//		fs.close();
		//		ofstream ofs(outputFileName, std::ios::in); //only in this way can you overwrite the file without loading all file
		//		ofs.seekp(-1, fs.end);
		//		ofs.put(',');
		//		ofs.close();
		//		fs.open(outputFileName, std::ios::in | std::ios::app);
		//	}
		//}
		//else {
		//	fs.put('[');
		//}
		//for (auto& gs : siloAll) {
		//	if (gs.bufferTrainingData(fs, duplicated, only_succeed, cdth, systh_)) {

		//		fs.put(',');
		//	}

		//}

		//fs.seekg(0, fs.end);

		//char lastCharacter;
		//fs.seekg(-1, fs.end);
		//fs.get(lastCharacter);
		//if (lastCharacter == ',') {
		//	fs.close();
		//	ofstream ofs(outputFileName, std::ios::in); //only in this way can you overwrite the file without loading all file
		//	ofs.seekp(-1, ofs.end);
		//	ofs.put(']');
		//	ofs.close();
		//}
		//else {

		//	fs.put(']');
		//	fs.close();
		//}

	}

	bool GroundingStateSilo::flush() {
		bool success = (trainingDataBuffer.size() > 0);
		json built_in_data = json::array();//data set of invoking built-in functions
		Strg built_in_filename = "";//file name of data set of invoking built-in functions

		map<Strg, set<Strg>> dataset_filename__knowledge_domains__map;
		for (const auto& pair : MultiTFIntegrator::knowledge_domains__dataset_filename__map) {

			dataset_filename__knowledge_domains__map.insert(make_pair(pair.second, pair.first));
		}
		if (MultiTFIntegrator::knowledge_domains__dataset_filename__map.count({ FILEKEYBUILTIN }) > 0) {
			built_in_filename = MultiTFIntegrator::knowledge_domains__dataset_filename__map[{FILEKEYBUILTIN}];
			const json& built_in_set = trainingDataBuffer[{built_in_filename}];
			if (built_in_set.is_array()) {
				size_t built_in_size = built_in_set.size();
				// Create a vector of indices
				std::vector<size_t> built_in_indices(built_in_size);
				std::iota(built_in_indices.begin(), built_in_indices.end(), 0);

				// Randomly shuffle the indices
				std::random_device rd;
				std::mt19937 g(rd());
				std::shuffle(built_in_indices.begin(), built_in_indices.end(), g);

				for (size_t i = 0; i < built_in_size; ++i) {
					json btijs = built_in_set[built_in_indices[i]];
					if (btijs["indomain"] == true) {
						btijs["indomain"] = false;
						built_in_data.push_back(btijs);
					}
				}
			}

		}

		for (const auto& pair : trainingDataBuffer) {
			//if (pair.first == built_in_filename) {
			//	continue;//dont export built-in data for training
			//}

			auto outputFilePath = coolang_home_path;
			auto export_knowledge_domain = dataset_filename__knowledge_domains__map[pair.first];
			outputFilePath.append(pair.first);
			const json& newData = pair.second;


			json combinedData = json::array();
			for (const auto& pair_ : trainingDataBuffer) {
				if (export_knowledge_domain.count(FILEKEYBUILTIN) > 0) {
					break;
				}
				if (pair_.first != pair.first && pair_.first != built_in_filename) {
					const json& conflict_domain_data = pair_.second;
					if (conflict_domain_data.is_array()) {
						size_t conflict_domain_data_size = conflict_domain_data.size();
						for (size_t i = 0; i < conflict_domain_data_size; ++i) {
							json cjs = conflict_domain_data[i];
							if (cjs["gstfdomain"] != "" && cjs["gstfdomain"] != FILEKEYBUILTIN &&
								export_knowledge_domain.count(cjs["gstfdomain"]) == 0) {
								if (cjs["indomain"] == true) {
									cjs["indomain"] = false;
									combinedData.push_back(cjs);
								}

							}
						}
					}
				}

			}

			// Check if the file exists and is not empty
			if (std::filesystem::exists(outputFilePath) && std::filesystem::file_size(outputFilePath) > 0) {
				std::ifstream inputFile(outputFilePath);
				if (inputFile.is_open()) {
					json existingData;
					inputFile >> existingData;

					// Check if the existing data is a JSON array
					if (existingData.is_array()) {
						size_t halfSize = min(existingData.size() / 2, newData.size() / 2);


						// Create a vector of indices
						std::vector<size_t> indices(existingData.size());
						std::iota(indices.begin(), indices.end(), 0);

						// Randomly shuffle the indices
						std::random_device rd;
						std::mt19937 g(rd());
						std::shuffle(indices.begin(), indices.end(), g);

						json randomHalf = json::array();;
						for (size_t i = 0; i < halfSize; ++i) {
							randomHalf.push_back(existingData[indices[i]]);
						}

						// Combine the random half of the existing data with the new data
						combinedData.insert(combinedData.end(), randomHalf.begin(), randomHalf.end());
						combinedData.insert(combinedData.end(), newData.begin(), newData.end());
					}
					else {

						combinedData.insert(combinedData.end(), newData.begin(), newData.end()); // Fallback in case the existing data is not an array

					}

					inputFile.close();
				}
				else {
					std::cerr << "Error opening file for reading: " << outputFilePath.string() << std::endl;
					success = false;
					continue; // Skip the current file if reading fails
				}
			}
			else {
				// If the file does not exist or is empty, just use the new data
				combinedData.insert(combinedData.end(), newData.begin(), newData.end());
			}

			size_t halfSizeBuiltIn = min(built_in_data.size(), int(1.33 * newData.size()));
			for (size_t i = 0; i < halfSizeBuiltIn; ++i) {
				combinedData.push_back(built_in_data[i]);
			}


			std::ofstream file(outputFilePath);
			if (file.is_open()) {
				file << combinedData.dump(4);  // Writing JSON data to file with indentation
				file.close();
			}
			else {
				// Handle the error, set success to false
				std::cerr << "Error opening file: " << outputFilePath.string() << std::endl;
				success = false;
			}
		}

		return success;
	}

	MessageWrapper::MessageWrapper(const CodeTableHash* cdth_, const SystemTableHash* systh_) {
		this->cdth = cdth_;
		this->systh = systh_;
	}
	Strg MessageWrapper::wrapMessage(const Strg& order_,
		const map<set<Strg>, Strg>& training_data_map) {
		if (order_ != "train") {
#if debug
			{
				static int ignorecount = 0;
				std::cerr << "(" << __FILE__ << ":" << __LINE__ << ":0"
					<< ")\twrapMessage(const Strg&, const map<set<Strg>,Strg>&)"
					<< " Invalid arg, throw(-1) " << "\tignorecount:["
					<< ignorecount++ << "\t]" << std::endl;
			}
#endif
			throw(-1);
		}
		json j;
		j["order"] = "train";
		j["training_data"] = json::array();
		function<set<Strg>(const set<Strg>& file_class_name)> serialNumberEraser = [](const set<Strg>& file_class_name)-> set<Strg> {
			set<Strg> converted_file_class_name;
			for (const auto& name : file_class_name) {
				auto v = split(name, "#");
				converted_file_class_name.insert(v.front());
			}
			return converted_file_class_name;
			};
		for (const auto& pair_ : training_data_map) {
			json jpair;

			jpair["file_class_name"] = serialNumberEraser(pair_.first);
			jpair["training_data_file_name"] = pair_.second;
			j["training_data"].push_back(jpair);

		}

		return j.dump();

	}


	Strg MessageWrapper::wrapMessage(const Strg& order_, const set<Strg>& file_class_name_set,
		const GroundingState& gs) {
		if (order_ != "predict") {

#if debug
			{
				static int ignorecount = 0;
				std::cerr << "(" << __FILE__ << ":" << __LINE__ << ":0"
					<< ")\twrapMessage(const Strg&, const set<Strg>&, GroundingState&)"
					<< " Invalid value, throw(-1) " << "\tignorecount:["
					<< ignorecount++ << "\t]" << std::endl;
			}
#endif
			throw(-1);

		}

		json j;
		function<set<Strg>(const set<Strg>& file_class_name)> serialNumberEraser = [](const set<Strg>& file_class_name)-> set<Strg> {
			set<Strg> converted_file_class_name;
			for (const auto& name : file_class_name) {
				auto v = split(name, "#");
				converted_file_class_name.insert(v.front());
			}
			return converted_file_class_name;
			};
		j["order"] = order_;
		j["file_class_name"] = serialNumberEraser(file_class_name_set);
		j["grounding_state"] = gs.toJson(cdth, systh, false);
		return j.dump();
	}

	json MessageWrapper::unwrapMessage(const Strg& reply_) {
		try {
			json j = json::parse(reply_);
			return j;
		}
		catch (const std::exception& e) {

			{
				static int ignorecount = 0;
				std::cerr << "(" << __FILE__ << ":" << __LINE__ << ":0"
					<< ")\tunwrapMessage(const Strg&) " << e.what() << ":[" << reply_ << "]\n"
					<< reply_ << ", exit(-1) " << "\tignorecount:["
					<< ignorecount++ << "\t]" << std::endl;
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

			std::cerr << "Error parsing JSON: " << e.what() << std::endl;
		}

	}


	CodeBlock::CodeBlock(Intg type, bool dependent, Intg id, Intg QSid,
		const Strg& cdtFileKey) {
		this->type = type;
		this->dependent = dependent;
		this->id = id;
		this->codeTable.fileKey = cdtFileKey;

	}
	CodeBlock::CodeBlock() {
	}
	void CodeBlock::addUnknownArg(const Arg& arg) {
		argUnknown.insert(arg);
	}
	Strg CodeBlock::toStrg() {
		stringstream ss;
		ss.precision(PRECISION);
		ss << "CodeBlock\t" << "id:[" << id << "]\t" << "dependent:["
			<< dependent << "]\t" << "QSid:[" << QSid << "]\t" << "QEid:["
			<< QEid << "]\n\t\n";
		ss << "argUnknown:\n";
		for (auto& arg : argUnknown) {
			ss << arg.toStrg();
		}
		ss << "\t\n";
		ss << codeTable.toStrg() << "\t\n";
		return ss.str();

	}

	void CodeBlockTable::addBlock(Intg id, const CodeBlock& cb) {
		iddq.push_back(id);
		icbmap[id] = cb;
	}
	/**
		 * @attention 这个函数可以向CodeBlockTable中添加codeBlock。
		 * 当id在icbmap中不存在时，会在返回引用的同时向iddq中pushback id。
		 * @param id
		 * @return
		 */
	CodeBlock& CodeBlockTable::operator[](Intg id) {
		if (icbmap.count(id) <= 0) {
			iddq.push_back(id);
		}
		return icbmap[id];
	}
	void CodeBlockTable::erase(Intg id) {
		for (auto it = iddq.begin(); it != iddq.end(); it++) {
			if (*it == id) {
				iddq.erase(it);
				icbmap.erase(id);
				break; //必须加break，否则it是最后一个元素，会陷入死循环
			}
		}

	}
	Strg CodeBlockTable::toStrg() {
		stringstream ss;
		ss.precision(PRECISION);
		for (auto it : iddq) {
			ss << icbmap[it].toStrg();
		}
		return ss.str();
	}

	BackStepper::~BackStepper() {
#if debug
		{
			static int ignorecount = 0;
			cout << "~BackStepper()" << " " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif
		//integrator = nullptr;
		executor = nullptr;
		codeTableHash = nullptr;
		scopeTableHash = nullptr;
		currentAR = nullptr;
		templateFunctionTableHash = nullptr;
		systemTableHash = nullptr;
		argKnown.clear();

		argUnknown.clear();

		mustKnown.clear();

		return_id.clear();
		scopeStack.clear();
		unknown_id_map.clear();
		step_map_dq.clear();

#if debug
		{
			static int ignorecount = 0;
			cout << "~BackStepper()" << " " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif
	}

	/**
		 * 删除指定id的codeBlock，同时清理new_id_map
		 * @param id
		 */
	void BackStepper::eraseBlock(Intg id) {
		CodeBlock& blockremove = codeBlockTable[id];
		for (auto it : blockremove.argUnknown) {
			unknown_id_map[it].erase(id);
			if (unknown_id_map[it].size() == 0) {
				unknown_id_map.erase(it);
			}
		}
		codeBlockTable.erase(id);

	}

	/**
		 * 当idset中的所有式子互相代入后，还有多少个未知变量（估计值，未计算）
		 * @param idset
		 * @return
		 */
	Intg BackStepper::getUnknownArgNum(set<Intg> idset) {
		set<Arg> argset;
#if debug
		{
			static int ignorecount = 0;
			cout << "getUnknownArgNum(set<int>)" << " " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif
		//合并所有block的未知变量至argset
		for (auto it : idset) {
			argset.insert(codeBlockTable[it].argUnknown.begin(),
				codeBlockTable[it].argUnknown.end());
		}
		//如果求解一个未知变量的所依赖block均在idset中，
		//那么对这个idset而言这各变量是已知的，可以从argset中移除

		Intg num = argset.size();
		for (auto it : argset) {
			if (includes(idset.begin(), idset.end(), unknown_id_map[it].begin(),
				unknown_id_map[it].end())) {
				--num;
			}
		}
		return num;
	}


	/**
		 * 当idset中的所有式子互相代入后，还有多少个未知变量（估计值，未计算）
		 * @param idset
		 * @return
		 */
	set<Arg> BackStepper::getUnknownArgSet(set<Intg> idset) {

		set<Arg> argset;
		for (auto it : idset) {

			argset.insert(codeBlockTable[it].argUnknown.begin(),
				codeBlockTable[it].argUnknown.end());
		}
		//        Intg num = argset.size();
		auto argsetTemp = argset;
		for (auto it : argset) {
			if (includes(idset.begin(), idset.end(), unknown_id_map[it].begin(),
				unknown_id_map[it].end())) {
				argsetTemp.erase(it);

			}
		}
		return argsetTemp;
	}

	/**
		 * @brief 将id1代表的codeBlock整合到id2代表的block中。所述操作的block均在cbt中。
		 * @param id1
		 * @param id2
		 * @param cbt
		 * @return 代入成功/失败
		 */
	bool BackStepper::integrateTwoBlock(Intg id1, Intg id2, CodeBlockTable* cbt) {
		CodeBlock& cb1 = (*cbt)[id1];
		CodeBlock& cb2 = (*cbt)[id2];
		Code& cb1root = cb1.codeTable.codedq.back();
		Arg&& arg1 = cb1root[0];
		Arg&& arg2 = cb1root[1];
		Arg&& argop = cb1root[2];
		Arg&& argres = cb1root[3];
		if (cb2.argUnknown.count(argres) == 0) {
			//如果一个block的根节点不是另一个block的未知数，则两个block不能代入
			return false;
		}
		else {
			if (argop.argFlag == M_Bs && argop.arg_s == "=") {
				//如果一个block的根节点是未知数，且block的最后一个最小表达式为赋值式，那么
				//1 获得赋值变量
				Arg& argassignR = arg2;
				Arg& argassignL = arg1;
				//2 删去赋值表达式
				cb1.codeTable.addrdq.pop_back();
				cb1.codeTable.codedq.pop_back();
				//3 将cb2中的参数换成赋值变量
				cb2.codeTable.replaceVar(argassignL, argassignR, acm::name | acm::asc);
				//4 合并cdt，合并并更新argUnknown
				cb2.codeTable.addrdq.insert(cb2.codeTable.addrdq.begin(),
					cb1.codeTable.addrdq.begin(),
					cb1.codeTable.addrdq.end());
				cb2.codeTable.codedq.insert(cb2.codeTable.codedq.begin(),
					cb1.codeTable.codedq.begin(),
					cb1.codeTable.codedq.end());
				set<Arg> argUnknown2temp;

				set_union(cb1.argUnknown.begin(), cb1.argUnknown.end(),
					cb2.argUnknown.begin(), cb2.argUnknown.end(),
					inserter(argUnknown2temp, argUnknown2temp.begin()));
				argUnknown2temp.erase(arg1);
				cb2.argUnknown = argUnknown2temp;

			}
			else {
				//如果一个block的根节点是未知数，且block的最后一个最小表达式不为赋值式，直接将两个block合并
				cb2.codeTable.addrdq.insert(cb2.codeTable.addrdq.begin(),
					cb1.codeTable.addrdq.begin(),
					cb1.codeTable.addrdq.end());
				cb2.codeTable.codedq.insert(cb2.codeTable.codedq.begin(),
					cb1.codeTable.codedq.begin(),
					cb1.codeTable.codedq.end());
				set<Arg> argUnknown2temp;

				set_union(cb1.argUnknown.begin(), cb1.argUnknown.end(),
					cb2.argUnknown.begin(), cb2.argUnknown.end(),
					inserter(argUnknown2temp, argUnknown2temp.begin()));
				cb2.argUnknown = argUnknown2temp;

			}
			return true;
		}

	}



	void BackStepper::setBackwardFunctionInfo(FKA& bfnScopeFKA, CodeTable* bfbScopeCdt) {
		this->bfbScopeCdt = bfbScopeCdt;
		this->bfnScopeFKA = bfnScopeFKA;
	}

	/**
		 * 以arg为模板返回一个新的arg，这个arg的标识符除argFlag外继承原arg
		 * @param arg
		 * @return
		 */
	Arg BackStepper::getNewArg(const Arg& arg) {
		Arg arg_(arg);
		arg_.argFlag = S_Bs;
		arg_.arg_fka = fkanull;
		arg_.arg_i = 0;
		arg_.arg_s = "_ARG_" + toStrg(++argID);
		return arg_;
	}
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
	CodeBlock BackStepper::setUnknownAttributeAndBlock(CodeTable& cdt) {
#if debug
		{
			static int ignorecount = 0;
			cout << "setUnknownAttributeAndBlock(CodeTable&)" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif
		numID += 2;
		CodeBlock cb(COOL_M, false, numID, currentScopeID, cdt.fileKey);
		cb.codeTable = cdt;
		//先进行代码参数替换，如果原值没有change（或赋值号），则直接用新值替换；
		//如果原值change了（或赋值了），说明原新值已经变旧，用新的新值替换原新值
		//注意！用新的新值替换掉旧值后，新的新值不可再被替换！！！！
		//所有值在一个表达式中最多change一次!!!!
		set<Arg> changedarg;
		for (auto& it : cb.codeTable.codedq) {
			Arg arg1 = it[0];
			Arg arg2 = it[1];
			Arg argop = it[2];
			Arg argres = it[3];

			Arg& newarg1 = old_new_map[arg1];
			if ((arg1.changeable
				|| (argop.argFlag == M_Bs && argop.arg_s == "="))
				&& changedarg.count(arg1) <= 0) {
				if (temporaryArg_occ_map.count(arg1) <= 0) {
					newarg1 = getNewArg(arg1);
					changedarg.insert(arg1);
					changedarg.insert(newarg1);
					newarg.insert(newarg1);        //告诉stepper有新创建的参数。
				}
				else if (temporaryArg_occ_map[arg1] == 1) {
					temporaryArg_occ_map[arg1] = 2;
					temporaryArg_id_map[arg1].insert(cb.id);
				}
				else {

					{
						static int ignorecount = 0;
						cout << "setUnknownAttributeAndBlock(CodeTable&)"
							<< " temporaryArg err arg1" << "\tignorecount:["
							<< ignorecount++ << "\t](" << __FILE__ << ":"
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
					assert(false); exit(-1);
				}

			}
			else {
				if (newarg1 == argnull) {
					newarg1 = arg1;
				}
			}
			it.setArg(0, newarg1, true);

			Arg& newarg2 = old_new_map[arg2];
			if (arg2.changeable && changedarg.count(arg2) <= 0) {
				if (temporaryArg_occ_map.count(arg2) <= 0) {
					newarg2 = getNewArg(arg2);
					changedarg.insert(arg2);
					changedarg.insert(newarg2);
					newarg.insert(newarg2);
				}
				else if (temporaryArg_occ_map[arg2] == 1) {
					temporaryArg_occ_map[arg2] = 2;
					temporaryArg_id_map[arg2].insert(cb.id);
				}
				else {

					{
						static int ignorecount = 0;
						cout << "setUnknownAttributeAndBlock(CodeTable&)"
							<< " temporaryArg err arg2" << "\tignorecount:["
							<< ignorecount++ << "\t](" << __FILE__ << ":"
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
					assert(false); exit(-1);
				}

			}
			else {
				if (newarg2 == argnull) {
					newarg2 = arg2;
				}
			}
			it.setArg(1, newarg2, true);

			Arg& newargop = old_new_map[argop];
			if (argop.changeable && changedarg.count(argop) <= 0) {
				if (temporaryArg_occ_map.count(argop) <= 0) {
					newargop = getNewArg(argop);
					changedarg.insert(argop);
					changedarg.insert(newargop);
					newarg.insert(newargop);
				}
				else if (temporaryArg_occ_map[argop] == 1) {
					temporaryArg_occ_map[argop] = 2;
					temporaryArg_id_map[argop].insert(cb.id);
				}
				else {

					{
						static int ignorecount = 0;
						cout << "setUnknownAttributeAndBlock(CodeTable&)"
							<< " temporaryArg err op" << "\tignorecount:["
							<< ignorecount++ << "\t](" << __FILE__ << ":"
							<< __LINE__ << ":0" << ")" << endl;

					}

				}

			}
			else {

				if (newargop == argnull) {
					newargop = argop;
				}
			}
			it.setArg(2, newargop, true);

			Arg& newargres = old_new_map[argres];
			if ((argres.changeable
				|| (argop.argFlag == M_Bs && argop.arg_s == "="))
				&& changedarg.count(argres) <= 0) {
				//赋值情况
				if (argop.argFlag == M_Bs && argop.arg_s == "=") {
					newargres = newarg1;
				}
				else if (argres.argFlag == S_Dz
					&& temporaryArg_occ_map.count(argres) <= 0) {
					//添加为临时变量，出现次数置为1
					temporaryArg_occ_map[argres] = 1;
					newargres = argres;
					cb.argTemporary.insert(argres);
					temporaryArg_id_map[argres].insert(cb.id);
				}
				else if (temporaryArg_occ_map.count(argres) > 0) {
					//临时变量不可能作为res出现两次，报错

					{
						static int ignorecount = 0;
						cout << "setUnknownAttributeAndBlock(CodeTable&)"
							<< " temporaryArg err res" << "\tignorecount:["
							<< ignorecount++ << "\t](" << __FILE__ << ":"
							<< __LINE__ << ":0" << ")" << endl;

					}

				}
				else {
					//一般情况(这一步虽然比较冗余，但先这么写吧，结果变量可能不需要创建)
					newargres = getNewArg(argres);
					changedarg.insert(argres);
					changedarg.insert(newargres);
					newarg.insert(newargres);
				}
			}
			else {
				if (newargres == argnull) {
					newargres = argres;
				}
			}
			it.setArg(3, newargres, true);

		}
		//推导参数可知性
		//一些参数在替换后由不可知变成了可知，
		//这是因为他们的值被改变了，本质上与原参数不是同一个参数了，属于正常现象。
		//如果在首个表达式中修改了所有参数，甚至可能导致所有变量已知（即与输入待定量无关）
		for (auto& it : cb.codeTable.codedq) {
			Arg arg1 = it[0];
			Arg arg2 = it[1];
			Arg argop = it[2];
			Arg argres = it[3];
			if (argUnknown.count(arg1)) {
				arg1.unknown = T_;
				it.assemblyFormula.flagBit.unknown[0] = T_;
			}
			if (argUnknown.count(arg2)) {
				arg2.unknown = T_;
				it.assemblyFormula.flagBit.unknown[1] = T_;
			}
			if (argUnknown.count(argop)) {
				argop.unknown = T_;
				it.assemblyFormula.flagBit.unknown[2] = T_;
			}
			if (argUnknown.count(argres)) {
				cb.dependent = true;
				argres.unknown = T_;
				it.assemblyFormula.flagBit.unknown[3] = T_;
			}
			else if (arg1.unknown || arg2.unknown || argop.unknown) {
				cb.dependent = true;
				argres.unknown = T_;
				it.assemblyFormula.flagBit.unknown[3] = T_;

			}

		}
		//存在不可知情况dependent = true（依赖未知值）：
		//对于不可知情况，应将所有标志位为不可知的变量的changeable置为true，
		//同时所有changeable变量的unknown置为true。
		if (cb.dependent) {
			cb.codeTable.unbindFunction(cb.codeTable.addrdq.front());
			map<Arg, Arg> old_new_map_temp;
			for (auto& it : cb.codeTable.codedq) {
				Arg arg1 = it[0];
				Arg arg2 = it[1];
				Arg argop = it[2];
				Arg argres = it[3];

				if (arg1.changeable || arg1.unknown) {

					it.assemblyFormula.flagBit.changeable[0] = T_;
					it.assemblyFormula.flagBit.unknown[0] = T_;
					argUnknown.insert(arg1);
					cb.addUnknownArg(arg1);
					unknown_id_map[arg1].insert(cb.id);
				}
				if (arg2.changeable || arg2.unknown) {

					it.assemblyFormula.flagBit.changeable[1] = T_;
					it.assemblyFormula.flagBit.unknown[1] = T_;

					argUnknown.insert(arg2);
					cb.addUnknownArg(arg2);
					unknown_id_map[arg2].insert(cb.id);
				}
				if (argop.changeable || argop.unknown) {

					it.assemblyFormula.flagBit.changeable[2] = T_;
					it.assemblyFormula.flagBit.unknown[2] = T_;

					argUnknown.insert(argop);
					cb.addUnknownArg(argop);
					unknown_id_map[argop].insert(cb.id);
				}
				if (argres.changeable || argres.unknown || arg1.changeable
					|| arg1.unknown || arg2.changeable || arg2.unknown
					|| argop.changeable || argop.unknown) {

					it.assemblyFormula.flagBit.changeable[3] = T_;
					it.assemblyFormula.flagBit.unknown[3] = T_;

					cb.addUnknownArg(argres);
					argUnknown.insert(argres);
					unknown_id_map[argres].insert(cb.id);
				}

			}

			//todo:: 此作用域内代码可以优化，即去除对中间变量的新建变量。
		}
		return cb;
	}
	void BackStepper::analysisBFN() {

		//backward function name scope 所在的cdt
		CodeTable& bfnCdt = codeTableHash->operator [](bfnScopeFKA.fileKey);
		mustKnown.insert(Arg(S_Bs, "ans"));
		Scope& bfnScope = (*scopeTableHash)[bfnScopeFKA];
		Intg posBegin = getAddrPosition(bfnCdt.addrdq, bfnScope.scopeCoor1);
		Intg posEnd = getAddrPosition(bfnCdt.addrdq, bfnScope.scopeCoor2);
		++posBegin;
		while (posBegin < posEnd) {
			Code& fncd = (bfnCdt)[posBegin];
			Arg&& arg1 = fncd[0];
			Arg&& arg2 = fncd[1];
			Arg&& argop = fncd[2];
			Arg&& argres = fncd[3];
			if (arg1.argFlag == S_Bs) {
				if (arg1.changeable) {
					argUnknown.insert(arg1);
					argKnown.erase(arg1);
				}
				else if (argUnknown.count(arg1) <= 0) {
					argKnown.insert(arg1);
				}
			}
			if (arg2.argFlag == S_Bs) {
				if (arg2.changeable) {
					argUnknown.insert(arg2);
					argKnown.erase(arg2);
				}
				else if (argUnknown.count(arg2) <= 0) {
					argKnown.insert(arg2);
				}
			}
			if (argop.argFlag == S_Bs) {
				if (argop.changeable) {
					argUnknown.insert(argop);
					argKnown.erase(argop);
				}
				else if (argUnknown.count(argop) <= 0) {
					argKnown.insert(argop);
				}
			}
			if (argres.argFlag == S_Bs) {
				if (argres.changeable) {
					argUnknown.insert(argres);
					argKnown.erase(argres);
				}
				else if (argUnknown.count(argres) <= 0) {
					argKnown.insert(argres);
				}
			}
			++posBegin;
		}
		for (auto it : argUnknown) {
			old_new_map.insert(pair<Arg, Arg>(it, argnull));
		}
		argUnknown.erase(*(mustKnown.begin()));
		argKnown.insert(mustKnown.begin(), mustKnown.end());
	}

	/**
	 * @brief 将blockIDset中的所有代码段进行整合，其基本思路是，从前向后依次合并：
	 * 找到最小id对应的block，先将其与和其有公共未知变量的block合并，
	 * 然后逐步推进到根节点
	 * @param integrateIDblock
	 * @return 整合完成后的节点[root]
	 */
	CodeBlock BackStepper::integrateBlock(set<Intg> blockIDset) {
#if debug
		{
			static int ignorecount = 0;
			cout << "integrateBlock(set<int>)" << " " << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << endl;
		}
#endif
		if (blockIDset.size() == 0) {
			return CodeBlock();
		}
		else if (blockIDset.size() > 0) {
			set<Intg> blockIDsetRemain = blockIDset;
			deque<pair<Intg, Intg>> integrateOrder;
			CodeBlockTable codeBlockTableTemp;
			//此循环进行合并
			//1。1 从小到大获得block的id
			for (auto it : blockIDset) {
				codeBlockTableTemp.addBlock(it, codeBlockTable[it]);
				blockIDsetRemain.erase(it);

				//1.2遍历依赖该block对应的未知量的block的集合block1
				for (auto arg_unknown : codeBlockTable[it].argUnknown) {
					set<Intg>& intersection_1 = unknown_id_map[arg_unknown];

					set<Intg> intersection_res;

					//1.3如果block1中的元素同时也是blockidset中除去block的元素（blockIDsetRemain），
					//那么这个元素可以与该block组合
					//通过每循环删除blockIDsetRemain中的it使得不重复匹配
					set_intersection(intersection_1.begin(), intersection_1.end(),
						blockIDsetRemain.begin(), blockIDsetRemain.end(),
						inserter(intersection_res, intersection_res.begin()));
					//1.4 将该block与1.3所得元素组对后填入整合顺序中
					for (auto id_ : intersection_res) {
						integrateOrder.push_back(make_pair(it, id_));

					}

				}

			}
			//2 根据integrateOrder（尝试）合并一个个的codeBlock.合并的codeBlock单独储存。
			//注意，合并后不需要重整地址范围。地址范围最后再整理

			set<Intg> substitudeID;            //已经代入的block
			for (auto it : integrateOrder) {

				bool substituteSuccess = integrateTwoBlock(it.first, it.second,
					&codeBlockTableTemp);
				if (substituteSuccess) {
					substitudeID.insert(it.first);
				}

			}
			//3 正常情况下，除了最后一个block（root），其他block都会被整合（代入），
			//也就是说，如果原block集合与其中被整合的block的集合做差，肯定只剩一个block根
			set<Intg> substitude_result;            //代入后的结果
			set_difference(blockIDset.begin(), blockIDset.end(),
				substitudeID.begin(), substitudeID.end(),
				inserter(substitude_result, substitude_result.begin()));
			if (substitude_result.size() != 1) {
				//todo::匹配有点问题

				{
					static int ignorecount = 0;
					cout << "integrateBlock(set<int>)" << " err ,exit(-1)"
						<< "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;

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
			else {
				//无需为代码段重新分配地址，因为地址顺序本就是正确的，（虽然地址不正确）
				//直接用integrator对block中的代码段进行匹配
				CodeBlock& resultcb =
					codeBlockTableTemp[(*substitude_result.begin())];
				CodeTable& cdt = resultcb.codeTable;
				cdt.unfoldRingStruct();
				cdt.checkChangeable();
				FKA&& fkades = FKA(cdt.fileKey, cdt.addrdq[0]);

				MultiTFIntegrator mi(codeTableHash, fkades,
					templateFunctionTableHash, scopeTableHash, systemTableHash,
					currentAR, setting, executor->pipe_writing_handler,
					executor->pipe_writing_handler);
				mi.setModeSeparate(cdt);
				if (mi.integrate()) {
					//匹配成功
					return codeBlockTableTemp[(*substitude_result.begin())];
				}
				else {
					return CodeBlock();
				}

			}

		}
		return CodeBlock();

	}

	/**
	 * @brief 根据逆函数的输入重新逐步确定可变标志位（直接根据基本表达式的四元式用到的操作符来确定），
	 * 而且受输入变量中影响的待定系数的标志位会始终向后传递,不会被“；”所终止。
	 * 使用到待定变量的与函数绑定的代码段会取消其绑定效果，同时在执行过程中注意修改内部代码的地址。
	 * @param bfnScopeAddr
	 * @param bfnScopeCdt
	 */
	void BackStepper::backwardFunctionFirstForwardExecution() {

		analysisBFN();
		//2 根据逆函数的输入重新逐步确定可变标志位.这个循环会遍历所有Code，请在这一步中完成
		//2.1对作用域地址的重新赋值并将产生的新作用域添加到（局部）作用域表中，
		//内层作用域可能不需要添加，但最外层一定要添加
		//2.2

		currentFBAd = (*bfbScopeCdt).addrdq.front();
		currentFBCd = &(*bfbScopeCdt)[currentFBAd];
		Addr nextFBAd = currentFBAd;
		for (; currentFBAd != addrnull; currentFBAd = nextFBAd) {
#if debug
			{
				static int ignorecount = 0;
				cout << "BackStepper::backwardFunctionFirstForwardExecution()"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
			if (currentFBAd != addrnull) {
				currentFBCd = &(*bfbScopeCdt)[currentFBAd];
			}
			if (nextFBAd != addrnull) {
				nextFBAd = (*bfbScopeCdt).getNextCodeAddr(nextFBAd);
			}
			currentScopeFKA = currentFBCd->scopeFKA;

			//2.1 将bfb拆解为block
			if ((*currentFBCd).assemblyFormula.flagBit.type == COOL_QS) {

				++numID;
				currentScopeID = numID;
				codeBlockTable[numID] = CodeBlock(COOL_QS, F_, numID, currentScopeID,
					bfbScopeCdt->fileKey);
				codeBlockTable[numID].QSid = numID;
				codeBlockTable[numID].codeTable.addCode(currentFBAd, *currentFBCd);
				scopeIDStack.push_back(currentScopeID);
				continue;

			}
			else if ((*currentFBCd).assemblyFormula.flagBit.type == COOL_QE) {

				++numID;
				codeBlockTable[numID] = CodeBlock(COOL_QE, F_, numID, currentScopeID,
					bfbScopeCdt->fileKey);
				codeBlockTable[numID].codeTable.addCode(currentFBAd,
					(*currentFBCd));
				codeBlockTable[currentScopeID].QEid = numID;
				scopeIDStack.pop_back();
				currentScopeID = scopeIDStack.back();

				continue;

			}
			else if ((*currentFBCd).assemblyFormula.flagBit.type == COOL_ME
				|| (*currentFBCd).assemblyFormula.flagBit.execute == Du) {
				(*bfbScopeCdt).eraseCode(currentFBAd, 0);
				continue;
			}
			else if (currentFBCd->assemblyFormula.flagBit.type == COOL_M) {
				CodeTable&& cdtmid = get<0>(bfbScopeCdt->copyFunctionCall(currentFBAd));
				CodeBlock&& cb = setUnknownAttributeAndBlock(cdtmid);
				codeBlockTable[cb.id] = cb;
				currentFBAd = cb.codeTable.addrdq.back();
				nextFBAd = bfbScopeCdt->getNextCodeAddr(currentFBAd);
				continue;

			}
			else if (currentFBCd->assemblyFormula.flagBit.type == COOL_R) {

				CodeTable cdt;

				cdt.addCode(currentFBAd, (*currentFBCd));
				cdt[currentFBAd].setArg(1, Arg(S_Bs, "ans"), false);
				cdt[currentFBAd].setArg(2, Arg(M_Bs, "="), false);
				cdt[currentFBAd].setArg(3, cdt[currentFBAd][0], false);
				cdt[currentFBAd].assemblyFormula.flagBit.type = COOL_M;
				cdt[currentFBAd].assemblyFormula.flagBit.tfFKA = FKA(FILEKEYBUILTIN,
					S_S_FUZHI);
				cdt[currentFBAd].assemblyFormula.flagBit.functionHandleFlag = true;
				numID += 2;
				//这里虽然将cdt的type设置为COOL_M，但是codeBlock的类型依然需要被设置为R，用以确定从何处逆向运行
				CodeBlock cb(COOL_R, false, numID, currentScopeID,
					bfbScopeCdt->fileKey);
				cb.codeTable = cdt;
				codeBlockTable[numID] = cb;
				return_id.insert(numID);

				continue;
			}

		}
#if debug
		{
			static int ignorecount = 0;
			cout << "enclosing_method(enclosing_method_arguments)" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif

	}
	/**todo::（预执行主要用于处理全局变量（域外变量），
	 * 但是，我们这里暂且禁用全局变量、同时要求各作用域的变量不得重复，
	 * 就可以跳过2次执行）
	 * 我们暂且忽略这一方法，先跑起来再说 嘻嘻嘻~~~
	 * @brief 这一步用于创建适合的AR
	 */
	void BackStepper::backwardFunctionSecondForwardExecution() {

		return;

	}
	void BackStepper::backwardFunctionThirdForwardExecution() {
		return;
	}
	/**
	 * @brief 这一步完成每一个block内部的代码匹配，同时，重整codeBlockTable中Block的排列顺序。
	 * @attention 这一步不对每一个block中的地址进行处理！处理还需一步！
	 */
	void BackStepper::backwardFunctionFirstBackwardExecution() {
		CodeBlockTable blockTableDependent; //仅仅包含已知变量的block，注意此block内biddq无效！
		CodeBlockTable blockTableIndependent; //仅仅包含未知变量的block，注意此block内biddq无效！
		deque<CodeBlock> blockIntegrate;

#if debug
		{
			static int ignorecount = 0;
			cout << "BackStepper::backwardFunctionFirstBackwardExecution()" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << endl;
			cout << codeBlockTable.toStrg() << endl;
		}
#endif
		for (auto it : codeBlockTable.iddq) {
			if (codeBlockTable[it].dependent) {
				blockTableDependent.addBlock(it, codeBlockTable[it]);
			}
			else {
				blockTableIndependent.addBlock(it, codeBlockTable[it]);
			}
		}

		//删除return后面的Block(emm...好像原函数匹配过程中已经删过了)
		//    Intg returnBlockID = (*return_id.rbegin());
		//    CodeBlock &returnBlock = codeBlockTable[returnBlockID];

			/**
			 * @brief 当仍然存在包含未知值的模块还未确定时，重复执行此循环，至所有未知模块均匹配。
			 * 匹配后的模块储存在@arg blockIntegrate 中
			 */
		while (blockTableDependent.icbmap.size() > 0) {
			//获得所有可能可以融合的组合（基于最大逆推步数决定）我这个方法有点太暴力了，影响速度
			//这一步可能有更优秀的算法,大佬来补(我想设计一个回收的东西，用来排除掉那些已经比较过的，优化时候再搞)
			step_map_dq.clear();
#if debug
			{
				static int ignorecount = 0;
				cout << "BackStepper::backwardFunctionFirstBackwardExecution()"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif

			//对于dq ，out_of_range lookups are not defined.
			step_map_dq.push_back(T_num_idset_map());
#if debug
			{
				static int ignorecount = 0;
				cout << "BackStepper::backwardFunctionFirstBackwardExecution()"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
			//向step0中放入距离返回表达式最近的需要逆推的表达式
			T_num_idset_map& step0_map = step_map_dq[0];
			auto step0_map_idset = set<Intg>();
			step0_map_idset.insert(blockTableDependent.iddq.back());
			auto step0_map_num = getUnknownArgNum(step0_map_idset);
			step0_map.insert(make_pair(step0_map_num, step0_map_idset));

			//从step0依次递推step1，step2.。。。。
			for (Intg currentStep = 0; currentStep < maxBackStep; currentStep++) {
				step_map_dq.push_back(T_num_idset_map());
				auto& nextStepMap = step_map_dq[currentStep + 1];
				for (auto it : step_map_dq[currentStep]) {
					set<Intg>& idset = it.second; //这个idset是已经代入（联立）的代码块的id集合
					set<Arg>&& unKnownArgSet = getUnknownArgSet(idset);
					for (auto unknownarg : unKnownArgSet) {

						for (Intg substituteBlockID : unknown_id_map[unknownarg]) {
							if (idset.count(substituteBlockID) > 0) { //重复的模块不代入
								continue;
							}
							else {    //代入不重复的模块。
								set<Intg> nextidset_temp = idset;
								nextidset_temp.insert(substituteBlockID);
								Intg nextnum_temp = getUnknownArgNum(
									nextidset_temp);
								//不重复添加模块
								bool exist = false;
								for (auto it = nextStepMap.lower_bound(
									nextnum_temp);
									it != nextStepMap.upper_bound(nextnum_temp)
									&& it != nextStepMap.end(); ++it) {
									if (nextidset_temp == it->second) {
										exist = true;
										break;
									}

								}
								if (!exist) {
									nextStepMap.insert(
										make_pair(nextnum_temp,
											nextidset_temp));
								}

							}

						}
					}

				}
			}

			//代入并整合
			//处理原则：未知量个数越小越优先>step越小越优先
			//unknownnum:用以显示这个表达式还依赖多少外部未知变量,越少越好
			for (Intg unknownnum = 0, cantIntegrateAny = false, integrateFinish =
				false; cantIntegrateAny == false && integrateFinish == false;
				unknownnum++) {
				cantIntegrateAny = true;
				for (auto num_idset_map : step_map_dq) {
					if (unknownnum >= 0) {
						if (num_idset_map.count(unknownnum)) {
							cantIntegrateAny = false;
							break;
						}
					}

				}
				if (cantIntegrateAny) {

					{
						static int ignorecount = 0;
						cout
							<< "BackStepper::backwardFunctionFirstBackwardExecution()"
							<< " cant integrate any,exit(-1)"
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
					assert(false); exit(-1);
				}

				for (auto num_idset_map : step_map_dq) {

					for (auto it = num_idset_map.lower_bound(unknownnum);
						it != num_idset_map.upper_bound(unknownnum); it++) {

						set<Intg>& integrateIDset = it->second;
						//这里对integrateIDset进行匹配与整合
						CodeBlock&& resultBlock = integrateBlock(integrateIDset);
						if (resultBlock.id == 0) {
							//整合失败
							continue;
						}
						else {
							//整合成功
							//整合成功意味着所有参与整合的式子的未知值都将已知，
							//因此在下一步整合前需要
							//1 将所有已经已知的值从argUnknown中去掉，并更新依赖这些值的CodeBlock
							//2 将参与匹配的Block从blockDependentIDdq（依赖值的iddq）中移除
							//2? 为什么可以移除？难道不担心其他式子引用吗？不用担心。如果有式子引用以被删除的模块，说明
							//这个式子脱离了逆推的语法树，那么这个式子也可以一并删除（事实是我们的匹配机制过于冗余，脱离逆推语法树
							//也可以正常匹配）

							set<Arg> argRecentlyKnown;

							for (auto it : integrateIDset) {
								set<Arg>& argUnknown_ =
									blockTableDependent[it].argUnknown;
								argRecentlyKnown.insert(argUnknown_.begin(),
									argUnknown_.end());

								blockTableDependent.erase(it);

							}
							argKnown += argRecentlyKnown;
							argUnknown -= argKnown;
							for (auto arg_ : argRecentlyKnown) {
								for (auto id_ : unknown_id_map[arg_]) {
									if (integrateIDset.count(id_)) {
										continue;
									}
									auto cb = blockTableDependent[id_];
									cb.argUnknown -= argKnown;
									cb.codeTable.resetChangeable(false, argKnown);

								}

							}
							blockIntegrate.push_back(resultBlock);
							integrateFinish = true;

						}

					}

				}

			}
		}

		//在所有模块均已经匹配后，开始进行代码的拼接，具体为
			/* 1 从blockIndependentTable中顺序读取一个block，其第一个block必须为COOL_QS类型，将其codeTable压入CodeTableTemp中
			 * 2 在压入COOL_QS类型后，初始化arg_new中的变量，以C类型创建
			 * 3 继续压入blockIndependentTable中的变量至倒数第二个
			 * 4 将blockIntegrate中的所有block的codeTable全部依次压入
			 * 5 将blockIndependentTable中最后一个block压入
			 * 6 重新分配地址
			 * 7 校正作用域
			 * 8 @attention 每压入一个COOL_M或R类型的block，一定要在后面补一个COOL_ME，将其隔断
			 */
		auto blockTableIndependent_it = blockTableIndependent.iddq.begin();

		CodeTable cdt_ME = CodeTable();
		cdt_ME.fileKey = bfbScopeCdt->fileKey;
		cdt_ME.addrdq.push_back(addrnull); //注意addrnull不能出现在实际的CodeTable中，这里在经过代码替换后会被消掉
		Code cd_ME = Code();

		cd_ME.assemblyFormula.flagBit.type = COOL_ME;
		cdt_ME.codedq.push_back(cd_ME);
		bfbScopeCdt->addrdq.clear();
		bfbScopeCdt->codedq.clear();
		//1
		bfbScopeCdt->appendCodeTable(
			blockTableIndependent[*(blockTableIndependent_it)].codeTable);
		++blockTableIndependent_it;
		bfbScopeCdt->appendCodeTable(cdt_ME);

		//2 初始化变量(这些变量是冗余的，尤其是地址变量，在进行地址变换后其值会变，因此在这里先插入一些COOL_ME占位)
		for (int menum = 10; menum > 0; menum--) {
			bfbScopeCdt->appendCodeTable(cdt_ME);

		}

		//3 继续压入blockIndependentTable中的block至倒数第二个(即最后一个COOL_QE不压入)
		//注意！不要在其中穿插cdt_ME,会破坏临时变量的创建与调用顺序
		Intg maxTemporaryArgOccID = 0;    //用以记录下一个TemporaryArg出现的ID的最大值
		for (;
			blockTableIndependent_it
			!= blockTableIndependent.iddq.begin()
			+ blockTableIndependent.iddq.size() - 1;) {
			bfbScopeCdt->appendCodeTable(
				blockTableIndependent[*(blockTableIndependent_it)].codeTable);
			if (blockTableIndependent[*(blockTableIndependent_it)].argTemporary.size()
					> 0) {

				//将共享临时变量的block之间不能用CdtME断开
				for (auto temporaryarg : blockTableIndependent[*(blockTableIndependent_it)].argTemporary) {
					for (auto id : temporaryArg_id_map[temporaryarg]) {
						if (blockTableIndependent.icbmap.count(id)) {
						}
						maxTemporaryArgOccID = max(maxTemporaryArgOccID, id);
					}

				}
				//不共享临时变量的block之间需要用CdtME断开以确保非临时变量的创建与调用顺序
				if (maxTemporaryArgOccID <= *(blockTableIndependent_it)) {
					bfbScopeCdt->appendCodeTable(cdt_ME);
				}

			}
			++blockTableIndependent_it;
		}
		//4 将blockIntegrate中的所有block的codeTable全部依次压入
		//注意！整合后的block不要在函数间填充
		for (auto cb : blockIntegrate) {
			bfbScopeCdt->appendCodeTable(cb.codeTable);

		}

		//5 压入最后的block
		bfbScopeCdt->appendCodeTable(
			blockTableIndependent[*(blockTableIndependent_it)].codeTable);

		//接下来我们开始整合。。。。。

	}
	//4 将代码整合到程序中
	/**
	 * 代码段实际地址取决于 split函数
	 * @param appendFKA 修改的代码段将插入appendFKA之后
	 * @param addrmin 修改后的代码段的起始地址
	 * @param addrmax 修改后代码段终止地址
	 */
	void BackStepper::backwardFunctionIntegrate(const FKA& appendFKA,
		const Addr& addrmin, const Addr& addrmax) {

		//这一步要完成bfb的完全整合到codeTable以及更新codeTable的scopeTable。
		// 1 重置地址（不重置Z_Dz）
		// 2 修改newarg（因为重置地址改变了部分S_Dz）
		// 3 将重置的地址整合到bfb中
		// 4 将bfb从头到尾扫描，修改scopeTable和templateFunctionTable
		// 5 将bfb插入到codeTable中
		// 最后请在创建BackSteper的地方修改currentCodeAddr和nextCodeAddr（如果需要的话）的地址

		//1
		map<FKA, FKA> map_original_new = bfbScopeCdt->resetFKARange(addrmin,
			addrmax, bfbScopeCdt->fileKey);
		//2
		set<Arg> newarg_temp = newarg;
#if debug
		{
			static int ignorecount = 0;
			cout
				<< "BackStepper::backwardFunctionIntegrate(const Addr&, const Addr&, const Addr&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif
		for (auto& arg_ : newarg) {
			if (arg_.argFlag == S_Dz && map_original_new.count(arg_.arg_fka)) {
				newarg_temp.erase(arg_);
				newarg_temp.insert(Arg(S_Dz, map_original_new[arg_.arg_fka]));

			}
		}
#if debug
		{
			static int ignorecount = 0;
			cout
				<< "BackStepper::backwardFunctionIntegrate(const Addr&, const Addr&, const Addr&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif
		//3 将重置的地址整合到bfb中
		//3.1 创建经过地址替换的cdt
		CodeTable cdt_newarg;
		cdt_newarg.fileKey = bfbScopeCdt->fileKey;
		Code cd_newarg;
		cd_newarg.assemblyFormula.flagBit.type = COOL_C;
		cd_newarg.setArg(1, Arg(X_Bs, "new"), true);
		cd_newarg.setArg(2, Arg(M_Bs, ":"), true);
		Addr ad = Addr(1);
		for (auto arg__ = newarg.begin(); arg__ != newarg.end(); ++arg__) {
#if debug
			{
				static int ignorecount = 0;
				cout
					<< "BackStepper::backwardFunctionIntegrate(const Addr&, const Addr&, const Addr&)"
					<< " arg__:[" << (*arg__).toStrg() << "]"
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
			cd_newarg.setArg(0, *arg__, true);
			cd_newarg.setArg(3, *arg__, true);
			cdt_newarg.addrdq.push_back(++ad);
			cdt_newarg.codedq.push_back(cd_newarg);
		}
		//3.2 获取所要插入的位置，亦即之前在作用域内部开头处所插入的连续COOL_ME。现将之删除，并获取其上下地址边界
		Addr cdtNewArgLowBoundAddr = bfbScopeCdt->addrdq[0];
		Addr cdtNewArgUpBoundAddr = bfbScopeCdt->getNextNot(COOL_ME,
			cdtNewArgLowBoundAddr);
		bfbScopeCdt->eraseCode(cdtNewArgLowBoundAddr, +1, cdtNewArgUpBoundAddr, -1);
		cdt_newarg.resetFKARange(cdtNewArgLowBoundAddr, cdtNewArgUpBoundAddr,
			cdt_newarg.fileKey);
		//3 将进行地址替换的cdt整合到bfb中
		bfbScopeCdt->insertCodeTable(cdtNewArgUpBoundAddr, cdt_newarg);
		//4 将bfb从头到尾扫描，修改scopeTable和templateFunctionTable(同时修改Z_Dz)
		map<FKA, FKA> scope_org_new_map;
		deque<FKA> currentScopeStack_;
		FKA currentScopeFKA_;
		for (Intg codePos = 0, len = bfbScopeCdt->size(); codePos < len;
			codePos++) {
			Code& code_ = (*bfbScopeCdt)[codePos];
			if (code_.assemblyFormula.flagBit.type == COOL_QS) {
				//获取currentScopeFKA(这里直接使用codeTable中代码地址作为scope地址)
				currentScopeFKA_ = FKA(bfbScopeCdt->fileKey,
					bfbScopeCdt->addrdq[codePos]);

				//获取原Z_Dz
				FKA scopeFKAOrg = code_.assemblyFormula.quaternion.result_fka;

				//修改Code的地址
				code_.assemblyFormula.quaternion.result_fka = currentScopeFKA_;
				code_.scopeFKA = currentScopeFKA_;
				code_.assemblyFormula.flagBit.res = currentScopeFKA_;

				//修改scopeTableHash
				scopeTableHash->operator [](currentScopeFKA_.fileKey).addrdq.push_back(
					currentScopeFKA_.addr);
				scopeTableHash->operator [](currentScopeFKA_.fileKey).scopedq.push_back(
					(*scopeTableHash)[scopeFKAOrg]);
				(*scopeTableHash)[currentScopeFKA_].scopeCoor1 =
					(*bfbScopeCdt).addrdq[codePos];

				if (currentScopeStack_.size() == 0) {
					(*scopeTableHash)[currentScopeFKA_].requireScopeFKA =
						bfnScopeFKA;
				}
				else {
					(*scopeTableHash)[currentScopeFKA_].requireScopeFKA =
						currentScopeStack_.back();
					(*scopeTableHash)[currentScopeFKA_].parentScopeFKA =
						currentScopeStack_.back();
				}
				(*scopeTableHash)[currentScopeFKA_].scopeCoor1 =
					(*bfbScopeCdt).addrdq[codePos];

				currentScopeStack_.push_back(currentScopeFKA_);

				scope_org_new_map[scopeFKAOrg] = currentScopeFKA_;
			}
			else if (code_.assemblyFormula.flagBit.type == COOL_QE) {

				code_.scopeFKA = currentScopeFKA_;

				//修改Code的地址
				code_.assemblyFormula.quaternion.result_fka = currentScopeFKA_;
				code_.scopeFKA = currentScopeFKA_;
				code_.assemblyFormula.flagBit.res = currentScopeFKA_;

				//修改scope
				(*scopeTableHash)[currentScopeFKA_].scopeCoor2 =
					(*bfbScopeCdt).addrdq[codePos];

				currentScopeStack_.pop_back();
				if (currentScopeStack_.size() > 0) {
					currentScopeFKA_ = currentScopeStack_.back();
				}
				else {
					currentScopeFKA_ = currentAR->scopeStructureFKA;
				}

			}
			else {
				code_.scopeFKA = currentScopeFKA_;
				FKA& a1s_ = code_.assemblyFormula.flagBit.a1s;
				FKA& a2s_ = code_.assemblyFormula.flagBit.a2s;
				FKA& ops_ = code_.assemblyFormula.flagBit.ops;
				FKA& res_ = code_.assemblyFormula.flagBit.res;
				if (scope_org_new_map.count(a1s_)) {
					a1s_ = scope_org_new_map[a1s_];
				}
				if (scope_org_new_map.count(a2s_)) {
					a2s_ = scope_org_new_map[a2s_];
				}
				if (scope_org_new_map.count(ops_)) {
					ops_ = scope_org_new_map[ops_];
				}
				if (scope_org_new_map.count(res_)) {
					res_ = scope_org_new_map[res_];
				}
			}
		}
		//修改templateFunctionTableHash
		TemplateFunction& bfntf = (*templateFunctionTableHash)[bfnScopeFKA];
		bfntf.isBackwardFunction = true;
		bfntf.bodyStartFKA = FKA(bfbScopeCdt->fileKey, bfbScopeCdt->addrdq.front());
		bfntf.bodyEndFKA = FKA(bfbScopeCdt->fileKey, bfbScopeCdt->addrdq.back());
		bfntf.bodyScopeFKA = (*bfbScopeCdt)[0].scopeFKA;
		bfntf.returnHandle.clear();

		// 5 将bfb插入到codeTable中
		codeTableHash->operator [](bfbScopeCdt->fileKey).insertCodeTable(addrmax,
			*bfbScopeCdt);
		//注意，此时虽然完成了整合，随后需要将nextCodeFKA/pos设置为新插入的scope的开头
		//如此的狠角色跟我比还是不够看

	}
	BackStepper::BackStepper(Executor* executor) {
		this->executor = executor;
		this->codeTableHash = executor->cdth;
		this->scopeTableHash = executor->scpth;
		this->currentAR = executor->currentAR;
		this->templateFunctionTableHash = executor->tfth;
		this->systemTableHash = executor->systh;
		this->maxBackStep = executor->setting->maxBackStep;
	}


	/**
	 * 虽然名字为setNextCodeFKI，但是这个函数并不主动寻找nextCodeFKI，而是判断输入的fki是否有效
	 * 而是检测传入的nextCodePos是否有效，无效的话寻找下一个有效的FKI，否则不变
	 * @attention 跳过无效代码、跳过返回表达式函数的函数体
	 * @param fki 如果fki有效，返回true，否则尝试修改fki使其有效，否则返回假
	 * @return 是否将fki设置成了有效的fki
	 */
	template<class class_deprecated>
	const bool Integrator<class_deprecated>::setNextCodeFKI(FKI& fki) {
		if (fki.GT(codeTableHash, codeTableHash->getMaxFKI())) {
			fki = fkinull;
			return false;
		}
		else if (fki == fkinull) {
			return false;
		}

		if (
			//            (*codeTable)[fki].assemblyFormula.flagBit.execute == F_
			//            ||
			(*codeTableHash)[fki].assemblyFormula.flagBit.execute == Du) {
			fki.SI(codeTableHash);
			return setNextCodeFKI(fki);
		}
		else if ((*codeTableHash)[fki].assemblyFormula.flagBit.execute == COOL_R
			&& (*codeTableHash)[fki].assemblyFormula.flagBit.type == COOL_QS) {
				{
#if debug
					static int ignorecount = 0;
					cout << "Integrator::setNextCodeFKI(fki&)" << " "
						<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
						<< ":" << __LINE__ << ":0" << ")" << endl;
#endif
				}
				FKA& scopeFKAJump = (*codeTableHash)[fki].scopeFKA;
				FKA& functionNameScope = (*scopeTableHash)[scopeFKAJump].requireScopeFKA;
				if (functionNameScope == fkanull
					|| (*scopeTableHash)[scopeFKAJump].conditionFlag != 0) {
					//这是条件语句条件体，需要进入匹配
					return true;
				}
				if ((*templateFunctionTableHash)[functionNameScope].integrate == true) {
					//这是返回表达式的函数的函数体，不需要进入匹配
					Addr& coor2 = (*scopeTableHash)[scopeFKAJump].scopeCoor2;
					fki.ID = getAddrPosition(
						codeTableHash->operator [](fki.fileKey).addrdq, coor2);
					fki.SI(codeTableHash);
#if debug
					cout << "Executor::setNextCodePos 5, nextCodeAddr:["
						<< nextCodeFKI.toStrg() << "]" << endl;
#endif
					return setNextCodeFKI(fki);
				}
				else {
					//这是返回运算值的函数的函数体，需要进入匹配
					return true;
				}

		}
		return true;
	}
	/**
	 * @brief 此函数根据nextCodeFKA更新currentCodeFKI、currentCodeFKA、currentCode、
	 * nextCodeFKI、nextCodeFKA、nextCode。
	 * @return
	 */
	template<class class_deprecated>
	const bool Integrator<class_deprecated>::getNewCode() {
		if (nextCodeFKA != fkanull) {
			nextCodeFKI = codeTableHash->getFKIByFKA(nextCodeFKA);
		}

		if (nextCodeFKI == fkinull) {
			return false;
		}
		currentCodeFKI = nextCodeFKI;
		nextCodeFKI.SI(codeTableHash);
		setNextCodeFKI(nextCodeFKI);
		if (currentCodeFKI.LT(codeTableHash, codeTableHash->getMaxFKI())) {

			currentCodeFKA = codeTableHash->getFKAByFKI(currentCodeFKI);
			currentCode = &(*codeTableHash)[currentCodeFKI];

			if (nextCodeFKI.LE(codeTableHash, codeTableHash->getMaxFKI())) {
				nextCodeFKA = codeTableHash->getFKAByFKI(nextCodeFKI);
				nextCode = &(*codeTableHash)[nextCodeFKI];

			}

			return true;

		}
		else if (currentCodeFKI == codeTableHash->getMaxFKI()) {
			currentCodeFKA = codeTableHash->getFKAByFKI(currentCodeFKI);
			currentCode = &(*codeTableHash)[currentCodeFKI];
			nextCodeFKA = fkanull;
			nextCode = nullptr;
			nextCodeFKI = fkinull;
			return true;
		}
		return false;
	}
	template<class class_deprecated>
	const bool Integrator<class_deprecated>::setArgScopeFKA() {
		if ((*currentCode).assemblyFormula.flagBit.type == COOL_C
			&& (*currentCode)[0].argFlag == S_Bs
			&& (*currentCode)[1].argFlag == X_Bs) {
			(*currentCode).assemblyFormula.flagBit.a1s = (*currentCode).scopeFKA;
			(*currentCode).assemblyFormula.flagBit.res = (*currentCode).scopeFKA;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M
			&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
			if (((*currentCode).assemblyFormula.flagBit.result_flag == S_Dz)
				&& (*currentCode).assemblyFormula.flagBit.formalArgFlag[3]
				== true) {
				(*currentAR).add((*currentCode)[3]);
			}
#if debug
			cout << "Integrator::setArgScopeFKA a1s" << endl;
#endif
			if ((*currentCode)[0].isVar()) {
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
			if ((*currentCode)[1].isVar()) {
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
			if ((*currentCode)[2].isVar()) {
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
			if ((*currentCode)[3].isVar()) {
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
			(*currentCode).assemblyFormula.flagBit.a1s = currentAR->getArgScopeFKA(
				(*currentCode)[0]);

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M
			&& (*currentCode).assemblyFormula.flagBit.execute == T_) {
			if (((*currentCode).assemblyFormula.flagBit.result_flag == S_Dz)
				&& (*currentCode).assemblyFormula.flagBit.formalArgFlag[3]
				== true) {
				(*currentAR).add((*currentCode)[3]);
			}
#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::setArgScopeFKA()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			if ((*currentCode)[0].isVar()) {
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
			if ((*currentCode)[1].isVar()) {
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
			if ((*currentCode)[2].isVar()) {
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
			if ((*currentCode)[3].isVar()) {
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
			if ((*currentCode)[0].isVar()) {
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
			if ((*currentCode)[1].isVar()) {
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
			if ((*currentCode)[2].isVar()) {
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
			if ((*currentCode)[3].isVar()) {
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

		return true;
	}
	template<class class_deprecated>
	const bool Integrator<class_deprecated>::addArgsToCurrentAR() {
		if ((*currentCode).assemblyFormula.flagBit.type == COOL_C
			&& (*currentCode)[0].argFlag == S_Bs
			&& (*currentCode)[1].argFlag == X_Bs) {
			if ((*currentCode)[1].arg_s == "new"
				|| (*currentCode)[1].arg_s == "out") {
				(*currentAR).add((*currentCode)[0]);
			}
			else {
				//创建类的实例，直接使用类的AR
				(*currentAR).add((*currentCode)[0]);
				System& sys = (*systemTableHash)[pair<Strg, FKA>(
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
			&& (*currentCode).assemblyFormula.flagBit.execute != Du) {
			/**
			 * 不用管引用，引用已经被置于COOL_M之前，已经被处理过了
			 */

			Arg&& arg_1 = (*currentCode)[0];
			Arg&& arg_2 = (*currentCode)[1];
			Arg&& arg_op = (*currentCode)[2];
			Arg&& arg_res = (*currentCode)[3];
			if (arg_1.isVar() && arg_1.argFlag != Y_Dz) {
				(*currentAR).add((*currentCode)[0]);
			}
			if (arg_2.isVar() && arg_2.argFlag != Y_Dz) {
				(*currentAR).add((*currentCode)[1]);
			}
			if (arg_op.isVar() && arg_op.argFlag != Y_Dz) {
				(*currentAR).add((*currentCode)[2]);
			}
			if (arg_res.isVar() && arg_res.argFlag != Y_Dz) {
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
			if (arg_res.isVar() && arg_res.argFlag != Y_Dz) {
				(*currentAR).add((*currentCode)[3]);
			}
			else if ((*currentCode).assemblyFormula.flagBit.result_flag == Y_Dz) {
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
			(*currentAR)[(*currentCode).scopeFKA].dataFlagBit = S_AR;
			//safe_delete((*currentAR)[(*currentCode).scopeFKA].content_ar);
			(*currentAR)[(*currentCode).scopeFKA].content_ar =
				DataTablePtr::make(currentAR, currentAR, (*currentCode).scopeFKA);
			currentAR = (*currentAR)[(*currentCode).scopeFKA].content_ar;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == F_) {

			(*currentAR)[(*currentCode)[3]].dataFlagBit = S_AR;

			currentAR = currentAR->parentAR;

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_R
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R) {

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QS
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R) {
			if ((*scopeTableHash)[currentCode->scopeFKA].functionFlag == SYSB) {
				//预执行过程中的继承处理
				(*currentAR).add((*currentCode)[3]);
				Data& sysData = (*currentAR)[(*currentCode).scopeFKA];
				sysData.dataFlagBit = S_AR;
				//safe_delete(sysData.content_ar);

				DataTablePtr sysAR = nullptr;
				sysData.content_ar = DataTablePtr::make(currentAR, currentAR, currentAR,
					(*currentCode).scopeFKA, SYSB);
				sysAR = sysData.content_ar;
				sysAR->scopeStructureFKA = currentCode->scopeFKA;
				currentAR = sysAR;

				System& currentSystem = (*systemTableHash)[currentCode->scopeFKA];
				//将继承类的预执行实例直接添加到querylist中完成继承
				for (auto& id : currentSystem.decedentSystemFKIList) {
					sysAR->queryARList.push_front(
						(*currentAR->parentAR)[(*systemTableHash)[id].systemFKA].content_ar);
				}

			}
			else {
				//预执行过程中的函数调用等处理
				(*currentAR).add((*currentCode)[3]);
				(*currentAR)[(*currentCode).scopeFKA].dataFlagBit = S_AR;

				//safe_delete((*currentAR)[(*currentCode).scopeFKA].content_ar);
				DataTablePtr queryAR_ = nullptr;
				if ((*scopeTableHash)[(*currentCode).scopeFKA].requireScopeFKA
					!= fkanull) {
					queryAR_ =
						(*currentAR)[(*scopeTableHash)[(*currentCode).scopeFKA].requireScopeFKA].content_ar;
				}
				else {
					queryAR_ = currentAR;
				}

				DataTablePtr funAR = nullptr;

				funAR = DataTablePtr::make(currentAR, queryAR_,
					(*currentCode).scopeFKA);
				(*currentAR)[(*currentCode).scopeFKA].content_ar = funAR;
				funAR->scopeStructureFKA = currentCode->scopeFKA;
				currentAR = funAR;
			}

		}
		else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
			&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R) {
			Arg&& scopeArg = (*currentCode)[3];
			(*currentAR)[scopeArg].dataFlagBit = S_AR;

			currentAR = currentAR->parentAR;

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
					(*templateFunctionTableHash)[currentCode->assemblyFormula.quaternion.arg2_fka].bodyScopeFKA;
				Addr ffbScopeAddrLowBound = (*scopeTableHash)[ffbScopeFKA].scopeCoor1;
				Addr ffbScopeAddrUpBound = (*scopeTableHash)[ffbScopeFKA].scopeCoor2;
				CodeTable& ffbCodeTable = codeTableHash->operator [](
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
				//BackStepper bs(this);
				BackStepper bs(nullptr);
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
				nextCodeFKA = codeTableHash->getNextCodeFKA(currentCodeFKA);
				//        nextCodeAddr = bfbScopeAddrUpBound;

		}

		return true;
	}


	/**
	 * @本阶段的框架，一行一行代码进行处理
	 * @return
	 */
	template<class class_deprecated>
	const bool Integrator<class_deprecated>::integrate() {
		string currentFIL;
		string currentLIN;
		globalRefTable.clear();
		globalRefTable.mode = MODE_GROUNDING;
		nextCodeFKA = codeTableHash->getMinFKA();
		nextCodeFKI = codeTableHash->getMinFKI();
		if (debugMode) {
			cout << "=====================推理过程=====================" << endl;
		}
		while (getNewCode() == true) {


#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "|||||||||||||||||||||\n" + codeTableHash->toStrg(); // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif

#if debug
			cout << "|||||||||||||||||||||" << endl;
#endif
#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::integrate()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
#if debug
			auto&& cdtstrtemp = codeTableHash->toStrg();
			cout << cdtstrtemp << endl;
#endif
#if debug
			{
				static int ignorecount = 0;
				cout << "Integrator::integrate()" << " " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			if ((*currentCode).assemblyFormula.flagBit.type == COOL_C) {
				setArgScopeFKA();
				addArgsToCurrentAR();

			}
			else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M
				&& (*currentCode).assemblyFormula.flagBit.execute != F_) {
				setArgScopeFKA();
				addArgsToCurrentAR();

				if (nextCode == nullptr
					|| (*nextCode).assemblyFormula.flagBit.type != COOL_M) {
					//如果表达式是EXPRB，则不整合
					if (currentCode->scopeFKA != fkanull) {
						Scope& currentScope =
							(*scopeTableHash)[currentCode->scopeFKA];
						if ((currentScope.functionFlag == EXPRB)) {

							continue;

						}
					}
					//否则进行整合
	//                CodeTable cdtr = codeTable->copyTree(currentCodeAddr);

					//all kinds of codes should be grounded
					MultiTFIntegrator mtfi(codeTableHash, currentCodeFKA,
						templateFunctionTableHash, scopeTableHash,
						systemTableHash, currentAR, setting,
						pipe_writing_handler, pipe_reading_handler);
					mtfi.setMaxCapacity(matchStateSiloMaxCapacity,
						groundingStateSiloMaxCapacity, maxSwitchTurn);
#if debug
					cout << "Integrator::integrate 2s" << endl;
#endif
					if (mtfi.integrate() == false) {

						{
							static int ignorecount = 0;
							cout << "Integrator::integrate()"
								<< " integrate failed,exit(-1)"
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
						assert(false); exit(-1);
					};
#if debug
					cout << "Integrator::integrate 2e" << endl;
#endif
				}

			}
			else if ((*currentCode).assemblyFormula.flagBit.type == COOL_Y
				&& (*currentCode).assemblyFormula.flagBit.execute != F_) {
				setArgScopeFKA();
				addArgsToCurrentAR();
			}

			else if ((*currentCode).assemblyFormula.flagBit.type == COOL_M

				&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
				addArgsToCurrentAR();
				setArgScopeFKA();

			}
			else if ((*currentCode).assemblyFormula.flagBit.type == COOL_Y
				&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
				addArgsToCurrentAR();
				setArgScopeFKA();

			}
			else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QS
				&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
				setArgScopeFKA();
				addArgsToCurrentAR();

			}
			else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
				&& (*currentCode).assemblyFormula.flagBit.execute == F_) {
				setArgScopeFKA();
				addArgsToCurrentAR();

			}
			else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QS
				&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R) {

				setArgScopeFKA();
				addArgsToCurrentAR();

			}
			else if ((*currentCode).assemblyFormula.flagBit.type == COOL_QE
				&& (*currentCode).assemblyFormula.flagBit.execute == COOL_R) {
				setArgScopeFKA();
				addArgsToCurrentAR();

			}
			else if ((*currentCode).assemblyFormula.flagBit.type == COOL_FUN) {
				setArgScopeFKA();
				addArgsToCurrentAR();
			}
			else if ((*currentCode).assemblyFormula.flagBit.type == COOL_TD) {
				setArgScopeFKA();
				//在这里进行逆函数的推导
				addArgsToCurrentAR();
			}
			else if ((*currentCode).assemblyFormula.flagBit.type == COOL_R) {
				setArgScopeFKA();
				addArgsToCurrentAR();
			}
			else if ((*currentCode).assemblyFormula.flagBit.type == COOL_N) {
				//什么都不用干
			}
			else if ((*currentCode).assemblyFormula.flagBit.type == COOL_LST) {
				setArgScopeFKA();
				addArgsToCurrentAR();
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
				setArgScopeFKA();
				addArgsToCurrentAR();
			}

		}
		return true;

	}



}