#include"coolang_vm_execute.hpp"
using namespace std;
namespace COOLANG {
	/***********************************

	Common

	**********************************/

	Strg& strgReplace(Strg& source, const Strg& pattern, const Strg& insert) {

		auto pos = (int)source.find(pattern, 0);
		while (pos != (int)string::npos) {
			source.replace(pos, pattern.size(), insert);
			pos = (int)source.find(pattern, pos);
		}

		return source;

	}

	/**
	 * @attention fs必须已打开
	 * @param strg
	 * @param fs 当前结构info对应的value应当写入的位置，
	 * 此外，当前info作为其他info的值其写入位置由其上层info决定
	 * @return Intl 写入后文件指针位置
	 */
	Intl StrgBinaryInfo::parseAndWriteObject(const Strg& strg, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout
				<< "StrgBinaryInfo::parseAndWriteObject(const Strg&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		Intl object_value_output_position = fs.tellp();
		this->position = object_value_output_position;
		this->bytesize = strg.size() * sizeof(char);
		fs.write(strg.c_str(), bytesize);
		return fs.tellp();

	}
	Intl StrgBinaryInfo::constructObject(Strg& strg, fstream& fs) {
		if (bytesize > 0) {
			fs.seekg(position);
			strg.clear();
			strg.reserve(bytesize + 1);
			for (Intg i = 0; i < bytesize; ++i) {
				strg += fs.get();
			}
		}

		return fs.tellg();

	}

	Intg toIntg(Strg s) {
		if (s == "") {
			{
#if debug
				static int ignorecount = 0;
				cerr << "toIntg(Strg)" << " convert \"\" to Intg,return 0"
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << endl;
#endif
			}
		}
		return atoi(s.c_str());
	}

	Numb toNumb(Strg s) {
		if (s == "") {
			{
#if debug
				static int ignorecount = 0;
				cerr << "toNumb(Strg)" << " convert \"\" to Numb,return 0"
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << endl;
#endif
			}
			THROW_CALCULATION_EXCEPTION(CalculationErrorType::None, "Cannot convert Strg(\"\") to a Numb.\n");
		}
		stringstream ss;
		ss.precision(PRECISION);
		ss << s;
		Numb num;
		ss >> num;
		if (num < 1e-50 && num > -1e-50) {
			num = 0;
		}
		{
#if debug
			static int ignorecount = 0;
			cout << "toNumb(Strg)" << "s:[" << s << "]\tnum:[" << num << "]"
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << endl;
#endif
		}
		return num;
	}

	Strg toStrg(Intg i) {
		stringstream ss;
		ss.precision(PRECISION);
		ss << i;
		return ss.str();
	}

	Strg toStrg(Numb num) {
		stringstream ss;
		ss.precision(PRECISION);
		ss << num;
		return ss.str();
	}

	Addr link(const Addr& addr1, const Addr& addr2) {
		Addr a;
		a = addr1;
		for (int i = 0; i < addr2.size(); i++) {
			a.addrv.push_back(addr2[i]);

		}
		return a;

	}

	Addr link(const Addr& addr1, const Intg& addr2) {

		return link(addr1, Addr(addr2));

	}

	vector<Strg> split(const Strg& str_, const Strg& pattern) {
		vector<Strg> result;
		if (str_ == "") {
			result.push_back("");
			return result;
		}
		string str = str_;
		str += pattern;
		for (int i = 0; i < (int)str.size(); i++) {
			int pos = (int)str.find(pattern, i);
			if (pos != (int)string::npos && str.substr(i, pos - i) != "") {
				result.push_back(str.substr(i, pos - i));
				i = pos + (int)pattern.size() - 1;
			}
		}
		return result;
	}


	/***********************************


	Addr


	**********************************/

	Addr::Addr(Strg addrStrg) {
		vector<Strg> addrStrg_ = split(addrStrg, ".");
		for (Intg i = 0; i < (Intg)addrStrg_.size(); i++) {
			Intg val = toIntg(addrStrg_.at(i));
			if (val != 0) {
				addrv.push_back(val);

			}
			else {
				return;
			}
		}

	}

	Addr::Addr(Intg addri) {
		addrv.push_back(addri);
	}

	Intg Addr::size() const {
		return addrv.size();
	}


	Addr::Addr(vector<Intg> addr_) {
		addrv.assign(addr_.begin(), addr_.end());
	}


	Addr& Addr::operator=(const Addr& addr_) {
		addrv.assign(addr_.addrv.begin(), addr_.addrv.end());
		return *this;
	}

	Addr& Addr::operator=(const Intg addri) {
		if (addri != 0) {
			addrv.assign(1, addri);
		}
		else {
#if debug
			cout << "Addr::operator= err,try to assign 0 to addr" << endl;
#endif
		}
		return *this;
	}


	Addr& Addr::operator=(const Strg& addrs) {
		Addr addr_(addrs);
		addrv.assign(addr_.addrv.begin(), addr_.addrv.end());
		return *this;
	}


	Addr Addr::createNext(Intg offset) const {
		Addr addrNext = *this;
		addrNext.addrv.pop_back();
		addrNext.addrv.push_back(addrv.back() + offset);
		return addrNext;
	}

	Addr Addr::createNext() const {

		return createNext(1);
	}

	Addr Addr::createSub(const Addr& suffix) const {
		Addr sub = *this;
		for (int i = 0; i < suffix.size(); i++) {
			sub.addrv.push_back(suffix.addrv.at(i));
		}
		return sub;
	}

	Addr Addr::createSub(const Strg& suffix) const {
		return createSub(Addr(suffix));
	}

	Addr Addr::createSub() const {

		return createSub(Addr("1"));
	}

	bool Addr::operator==(const Addr& addr_) const {
		return addrv == addr_.addrv;
	}

	bool Addr::operator!=(const Addr& addr_) const {
		return addrv != addr_.addrv;
	}

	bool Addr::operator==(const Intg addri) const {
		return addrv == Addr(addri).addrv;
	}

	bool Addr::operator==(const Strg& addrs) const {
		return addrv == Addr(addrs).addrv;
	}

	bool Addr::operator<(const Addr& addr_) const {

		Intg m = min(this->size(), addr_.size());
		for (Intg i = 0; i < m; i++) {
			if (addrv.at(i) < addr_.addrv.at(i)) {
				return true;
			}
			else if (addrv.at(i) > addr_.addrv.at(i)) {
				return false;
			}

		}
		if (this->size() < addr_.size()) {
			if (addr_.addrv.at(m) > 0) {
				return true;
			}
			else {
				return false;
			}
			return true;
		}
		else if (this->size() == addr_.size()) {
			return false;
		}
		else if (this->size() > addr_.size()) {
			if (addrv.at(m) < 0) {
				return true;
			}
			else {
				return false;
			}
		}
		return false;
	}


	bool Addr::operator<=(const Addr& addr_) const {

		Intg m = min(this->size(), addr_.size());

		for (Intg i = 0; i < m; i++) {
			if (addrv.at(i) < addr_.addrv.at(i)) {
				return true;
			}
			else if (addrv.at(i) > addr_.addrv.at(i)) {
				return false;
			}

		}

		if (this->size() < addr_.size()) {

			if (addr_.addrv.at(m) > 0) {
				return true;
			}
			else {
				return false;
			}
			return true;
		}
		else if (this->size() == addr_.size()) {

			return true;
		}
		else if (this->size() > addr_.size()) {

			if (addrv.at(m) < 0) {

				return true;
			}
			else {

				return false;
			}
		}
		return false;
	}



	bool Addr::operator>=(const Addr& addr_) const {
		return !(*this < addr_);
	}

	bool Addr::operator>(const Addr& addr_) const {
		return !(*this <= addr_);
	}

	/**
			 * @name operator++()
			 * @brief ==createNext
			 * @return
			 */
	Addr& Addr::operator++() {
		addrv.back() += 1;
		return (*this);
	}

	/**
			 * @name operator++(int)
			 * @return
			 */
	Addr Addr::operator++(int) {
		auto ret = (*this);
		this->addrv.back() += 1;
		return ret;
	}

	/**
			* @name operator+(Intg offset)
			* @brief ==createNext()
			* @return
			*/
	Addr Addr::operator+(Intg offset) const {
		return createNext(offset);
	}
	Intg Addr::operator[](Intg pos) const {
		return this->addrv.at(pos);
	}
	Strg Addr::toStrg() const {
		stringstream addrss;
		addrss.precision(PRECISION);
		int sz = this->size();
		for (int i = 0; i < sz; i++) {
			if (i != 0) {
				addrss << ".";
			}
			addrss << ((*this)[i]);
		}
		if (*this < addrnull) {
			addrss << "(" << TemplateFunctionTableHash::getTFNameByFKA(FKA(FILEKEYBUILTIN, *this)) << ")";
		}
		return addrss.str();

	}



	/********************************


	AddrBinaryInfo


	********************************/
	Intl AddrBinaryInfo::parseAndWriteObject(const Addr& addr, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "parseAndWriteObject(const Addr&, fstream&)" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->position = fs.tellp();
		this->bytesize = addr.size() * sizeof(Intg);
		fs.write(reinterpret_cast<const char*>(addr.addrv.data()), bytesize);
		return fs.tellp();
	}

	Intl AddrBinaryInfo::constructObject(Addr& addr, fstream& fs) {
		addr.addrv.clear();
		if (bytesize > 0) {
			fs.seekg(position);
			Intg* intgv = new int[bytesize];
			fs.read(reinterpret_cast<char*>(intgv), bytesize);
			addr.addrv.reserve(bytesize + 1 * sizeof(Intg));
			for (Intg i = 0; i < (Intg)(bytesize / sizeof(Intg)); ++i) {
				addr.addrv.push_back(intgv[i]);
			}
			delete[] intgv;
		}
		return fs.tellg();
	}

	/************************************


	FileKeyAddr


	***********************************/

	//此函数仅仅提供一个排序方式，比较无实际意义。如要比较，请定义新的函数
	bool FileKeyAddr::operator>(const FileKeyAddr& fka) const {
		if (this->fileKey > fka.fileKey) {
			return true;
		}
		else if (this->fileKey == fka.fileKey) {
			return this->addr > fka.addr;
		}
		else {
			return false;
		}
	}

	//此函数仅仅提供一个排序方式，比较无实际意义。如要比较，请定义新的函数
	bool FileKeyAddr::operator<(const FileKeyAddr& fka) const {
		if (this->fileKey < fka.fileKey) {
			return true;
		}
		else if (this->fileKey == fka.fileKey) {
			return this->addr < fka.addr;
		}
		else {
			return false;
		}
	}



	bool FileKeyAddr::operator==(const FileKeyAddr& fka) const {
		/*if ((this->addr == addrnull && fka.addr == addrnull)
			|| (this->addr == addrmax && fka.addr == addrmax)) {
			return true;

		}*/
		return (this->fileKey == fka.fileKey) && (this->addr == fka.addr);
	}


	bool FileKeyAddr::operator!=(const FileKeyAddr& fka) const {
		return !(*this == fka);
	}


	bool FileKeyAddr::EQ(CodeTableHash* cdth, const FileKeyAddr& fka_) const {
		return *this == fka_;

	}


	bool FileKeyAddr::LT(CodeTableHash* cdth, const FileKeyAddr& fka_) const {
		return (!(*this == fka_)) && (!(GT(cdth, fka_)));

	}



	bool FileKeyAddr::GE(CodeTableHash* cdth, const FileKeyAddr& fka_) const {
		return !this->LT(cdth, fka_);

	}

	bool FileKeyAddr::LE(CodeTableHash* cdth, const FileKeyAddr& fka_) const {
		return !this->GT(cdth, fka_);

	}

	FileKeyAddr::FileKeyAddr(const Strg& fileKey, const Addr& addr) {
		this->fileKey = fileKey;
		this->addr = addr;
	}


	FileKeyAddr& FileKeyAddr::operator=(const FileKeyAddr& fka) {
		this->fileKey = fka.fileKey;
		this->addr = fka.addr;
		return *this;

	}


	Strg FileKeyAddr::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << fileKey << "-" << addr.toStrg();
		return ss.str();
	}

	/**
	 * @brief greaterThan 当fileKey在CodeTableHash中位置靠后时返回真，否则返回假
	 * @attention 当addr为addrnull时，fka最小；为addrmax时，fka最大
	 * @param cdth
	 * @param fka_
	 * @return
	 */
	bool FKA::GT(CodeTableHash* cdth, const FileKeyAddr& fka_) const {
		if (*this != fka_) {
			if (fka_.fileKey == FILEKEYEND) {
				return false;
			}
			else if (fka_.fileKey == "") {
				return true;
			}
			else if (this->fileKey == FILEKEYEND) {
				return true;
			}
			else if (this->fileKey == "") {
				return false;
			}

		}

		if (cdth) {
			if (fileKey != fka_.fileKey) {
				if (this->fileKey == FILEKEYBUILTIN && fka_.fileKey == "") {
					return true;
				}
				else if (this->fileKey == "" && fka_.fileKey == FILEKEYBUILTIN) {
					return false;
				}
				else if (this->fileKey == FILEKEYBUILTIN && fka_.fileKey != "") {
					return false;
				}
				else if (this->fileKey != "" && fka_.fileKey == FILEKEYBUILTIN) {
					return true;
				}
				auto this_fileKey_ad = cdth->file_addr_map.find(fileKey);
				auto FKA_fileKey_ad = cdth->file_addr_map.find(fka_.fileKey);
				if (this_fileKey_ad != cdth->file_addr_map.end()
					&& FKA_fileKey_ad != cdth->file_addr_map.end()) {
					return this_fileKey_ad->second > FKA_fileKey_ad->second;

				}
				else {

					{
						static int ignorecount = 0;
						cout << "GT(CodeTableHash*, const FileKeyAddr&)"
							<< " fileKeyNotFound, exit(-1) "
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
			}
			else {
				return this->addr > fka_.addr;
			}

		}
		else {

			{
				static int ignorecount = 0;
				cout << "GT(CodeTableHash*, const FileKeyAddr&)"
					<< " CodeTableHash == nullptr, exit(-1) " << "\tignorecount:["
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
			assert(false); exit(-1);
		}

	}


	/*********************************


	FKABinaryInfo


	*********************************/

	Intl FKABinaryInfo::parseAndWriteObject(const FileKeyAddr& fka, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout
				<< "FKABinaryInfo::parseAndWriteObject(const FileKeyAddr&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->fileKeyInfo.parseAndWriteObject(fka.fileKey, fs);
		this->addrInfo.parseAndWriteObject(fka.addr, fs);
		return fs.tellp();
	}
	Intl FKABinaryInfo::constructObject(FileKeyAddr& fka, fstream& fs) {
		this->fileKeyInfo.constructObject(fka.fileKey, fs);
		this->addrInfo.constructObject(fka.addr, fs);
		return fs.tellg();
	}

	/************************************


	FileKeyID


	**********************************/

	//此函数仅仅提供一个排序方式，比较无实际意义。如要比较，请定义新的函数
	bool FileKeyID::operator>(const FileKeyID& FKI) const {
		if (this->fileKey > FKI.fileKey) {
			return true;
		}
		else if (this->fileKey == FKI.fileKey) {
			return this->ID > FKI.ID;
		}
		else {
			return false;
		}
	}
	//此函数仅仅提供一个排序方式，比较无实际意义。如要比较，请定义新的函数
	bool FileKeyID::operator<(const FileKeyID& FKI) const {
		if (this->fileKey < FKI.fileKey) {
			return true;
		}
		else if (this->fileKey == FKI.fileKey) {
			return this->ID < FKI.ID;
		}
		else {
			return false;
		}
	}

	//若fileKey == "" || fileKey == FILEKEYEND，判定相等
	bool FileKeyID::operator==(const FileKeyID& FKI) const {
		if (this->fileKey == FKI.fileKey
			&& (this->fileKey == "" || this->fileKey == FILEKEYEND)) {
			return true;
		}
		return (this->fileKey == FKI.fileKey) && (this->ID == FKI.ID);
	}


	bool FileKeyID::operator!=(const FileKeyID& FKI) const {
		return !((this->fileKey == FKI.fileKey) && (this->ID == FKI.ID));
	}

	bool FileKeyID::GE(CodeTableHash* cdth, const FileKeyID& FKI_) const {
		return !this->LT(cdth, FKI_);
	}

	bool FileKeyID::EQ(CodeTableHash* cdth, const FileKeyID& FKI_) const {
		return *this == FKI_;

	}

	bool FileKeyID::LT(CodeTableHash* cdth, const FileKeyID& FKI_) const {
		return (!(*this == FKI_)) && (!(GT(cdth, FKI_)));

	}


	bool FileKeyID::LE(CodeTableHash* cdth, const FileKeyID& FKI_) const {
		return (!GT(cdth, FKI_));

	}

	FileKeyID::FileKeyID() {
	}


	FileKeyID::FileKeyID(const Strg& fileKey, const Intg& ID) {
		this->fileKey = fileKey;
		this->ID = ID;
	}

	FileKeyID& FileKeyID::operator=(const FileKeyID& FKI) {
		this->fileKey = FKI.fileKey;
		this->ID = FKI.ID;
		return *this;

	}

	Strg FileKeyID::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << fileKey << "-" << ID;
		return ss.str();
	}


	/**
 * @brief greaterThan 当fileKey在CodeTableHash中位置靠后时返回真，否则返回假
 * @param cdth
 * @param FKI_
 * @attention ""<FILEKEYBUILTIN
 * @return
 */
	bool FKI::GT(CodeTableHash* cdth, const FileKeyID& FKI_) const {
		if (*this != FKI_) {
			if (FKI_.fileKey == FILEKEYEND) {
				return false;
			}
			else if (FKI_.fileKey == "") {
				return true;
			}
			else if (this->fileKey == FILEKEYEND) {
				return true;
			}
			else if (this->fileKey == "") {
				return false;
			}

		}

		if (cdth) {
			if (fileKey != FKI_.fileKey) {
				if (this->fileKey == FILEKEYBUILTIN && FKI_.fileKey == "") {
					return true;
				}
				else if (this->fileKey == "" && FKI_.fileKey == FILEKEYBUILTIN) {
					return false;
				}
				else if (this->fileKey == FILEKEYBUILTIN && FKI_.fileKey != "") {
					return false;
				}
				else if (this->fileKey != "" && FKI_.fileKey == FILEKEYBUILTIN) {
					return true;
				}
				auto this_fileKey_ad = cdth->file_addr_map.find(fileKey);
				auto FKI_fileKey_ad = cdth->file_addr_map.find(FKI_.fileKey);
				if (this_fileKey_ad != cdth->file_addr_map.end()
					&& FKI_fileKey_ad != cdth->file_addr_map.end()) {
					return this_fileKey_ad->second > FKI_fileKey_ad->second;

				}
				else {

					{
						static int ignorecount = 0;
						cout << "GT(CodeTableHash*, const FileKeyID&)"
							<< " fileKeyNotFound, exit(-1) "
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
			}
			else {
				return this->ID > FKI_.ID;
			}

		}
		else {

			{
				static int ignorecount = 0;
				cout << "GT(CodeTableHash*, const FileKeyID&)"
					<< " CodeTableHash == nullptr, exit(-1) " << "\tignorecount:["
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
			assert(false); exit(-1);
		}

	}
	/**
	 * @brief self increase
	 * @param cdth
	 * @param FKI_
	 * @return 自增后的fki
	 * @attention 如果越过上边界，则fki置为fkinull
	 */
	FileKeyID& FKI::SI(CodeTableHash* cdth) {
		if (cdth->file_cdt_map.count(this->fileKey) <= 0) {
			return fkinull;
		}
		++ID;
		CodeTable& cdt = cdth->operator [](this->fileKey);
		if (this->ID > cdt.size() - 1) {
			this->fileKey = cdth->getNextCdtFileKey(fileKey);
			while (fileKey != "" && cdth->operator [](this->fileKey).size() <= 0) {
				this->fileKey = cdth->getNextCdtFileKey(fileKey);
				if (fileKey == "") {
					*this = fkinull;
					break;
				}
			}
			this->ID = 0;
		}
		return *this;

	}
	/**
	 * @brief self decrease
	 * @param cdth
	 * @param FKI_
	 * @return 自减后的fki
	 * @attention 如果越过下边界，则fki置为fkinull
	 */
	FileKeyID& FKI::SD(CodeTableHash* cdth) {
		if (cdth->file_cdt_map.count(this->fileKey) <= 0) {
			return fkinull;
		}
		--ID;
		CodeTable& cdt = cdth->operator [](this->fileKey);
		if (this->ID < 0) {
			this->fileKey = cdth->getLastCdtFileKey(fileKey);
			while (fileKey != "" && cdth->operator [](this->fileKey).size() <= 0) {
				this->fileKey = cdth->getLastCdtFileKey(fileKey);
				if (fileKey == "") {
					*this = fkinull;
					break;
				}
			}
			this->ID = cdt.size() - 1;
		}
		return *this;

	}

	/**********************************


	FKIBinaryInfo


	*************************************/
	Intl FKIBinaryInfo::parseAndWriteObject(const FKI& fki, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "FKIBinaryInfo::parseAndWriteObject(const FKI&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->fileKeyInfo.parseAndWriteObject(fki.fileKey, fs);
		this->ID = fki.ID;
		return fs.tellp();

	}
	Intl FKIBinaryInfo::constructObject(FKI& fki, fstream& fs) {
		this->fileKeyInfo.constructObject(fki.fileKey, fs);
		fki.ID = this->ID;
		return fs.tellg();

	}
	/********************************************


	ConstraintArg


	**********************************/

	Strg ConstraintArg::toStrg() const {
		stringstream ss;
		ss << "argFlag:[" << argFlag << "] value:[" << arg_i << "|" << arg_s << "|" << arg_fka.toStrg() << "]";
		return ss.str();

	}

	bool ConstraintArg::operator==(const ConstraintArg& constr_arg) const {
		return this->argFlag == constr_arg.argFlag && this->arg_i == constr_arg.arg_i && this->arg_s == constr_arg.arg_s && this->arg_fka == constr_arg.arg_fka;
	}



	bool ConstraintArg::operator!=(const ConstraintArg& constr_arg) const {
		return !(*this == constr_arg);
	}



	ConstraintArg& ConstraintArg::operator=(const ConstraintArg& constr_arg) {
		this->argFlag = constr_arg.argFlag; this->arg_i = constr_arg.arg_i; this->arg_s = constr_arg.arg_s; this->arg_fka = constr_arg.arg_fka;
		return *this;
	}

	bool ConstraintArg::isImmediate() const {
		return (argFlag == Sz || argFlag == Zf);
	}


	bool ConstraintArg::operator<(const ConstraintArg& constr_arg) const {
		if (*this == constr_arg) {
			return false;
		}
		//don't compare non immediate.
		if (this->argFlag != constr_arg.argFlag || this->isImmediate() == false || constr_arg.isImmediate() == false) {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "invalid compare argument"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			cout << "invalid compare argument for ConstraintArg, exit(-1)" << endl;
			assert(false); exit(-1);
		}
		if (this->argFlag == Sz) {
			return this->arg_i < constr_arg.arg_i;
		}
		else if (this->argFlag == Zf) {
			return this->arg_s < constr_arg.arg_s;
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
			cout << "unknown invalid compare argument for ConstraintArg, exit(-1)" << endl;
			assert(false); exit(-1);
		}
	}



	bool ConstraintArg::operator<=(const ConstraintArg& constr_arg) const {
		return *this == constr_arg || *this > constr_arg;
	}



	bool ConstraintArg::operator>(const ConstraintArg& constr_arg) const {
		return !(*this <= constr_arg);
	}

	bool ConstraintArg::operator>=(const ConstraintArg& constr_arg) const {
		return !(*this < constr_arg);
	}


	/**
			 * @brief 是否S_Bs、S_Dz、Z_Dz、S_AR、Y_Dz
			 * @return
			 */
	bool ConstraintArg::isVar() const {
		return (!(arg_i == 0 && arg_fka == fkanull && arg_s == ""
			))
			&& (argFlag == S_Bs || argFlag == S_Dz || argFlag == Z_Dz
				|| argFlag == S_AR || argFlag == Y_Dz);
	}


	/**
		 * @brief 是否S_Bs
		 * @return
		 */
	bool ConstraintArg::isBs() const {
		return (!(arg_i == 0 && arg_fka == fkanull && arg_s == ""
			)) && argFlag == S_Bs;
	}


	/**
			 * @brief 是否S_Dz、Z_Dz、Y_Dz
			 * @return
			 */
	bool ConstraintArg::isDz() const {
		return (!(arg_i == 0 && arg_fka == fkanull && arg_s == ""
			))
			&& (argFlag == S_Dz || argFlag == Z_Dz || argFlag == Y_Dz);

	}



	/************************************************


	ConstraintArgBinaryInfo



	*************************/


	Intl ConstraintArgBinaryInfo::parseAndWriteObject(const ConstraintArg& constr_arg, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout
				<< "FKABinaryInfo::parseAndWriteObject(const FileKeyAddr&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->argFlag = constr_arg.argFlag;
		this->arg_i = constr_arg.arg_i;
		this->arg_s_info.parseAndWriteObject(constr_arg.arg_s, fs);
		this->arg_fka_info.parseAndWriteObject(constr_arg.arg_fka, fs);
		return fs.tellp();
	}
	Intl ConstraintArgBinaryInfo::constructObject(ConstraintArg& constr_arg, fstream& fs) {
		constr_arg.argFlag = this->argFlag;
		constr_arg.arg_i = this->arg_i;
		this->arg_s_info.constructObject(constr_arg.arg_s, fs);
		this->arg_fka_info.constructObject(constr_arg.arg_fka, fs);
		return fs.tellg();
	}




	/**********************************

	Constraint

	*******************************/

	Strg Constraint::toStrg() const {
		stringstream ss;
		ss << "constr_instr:[" << instruction << "]";
		if (args.size() > 0) {
			ss << "constr:[";
			auto it = args.begin();
			while (it != args.end()) {
				ss << "[" << it->toStrg() << "],";
			}
			ss << "]";
		}
		return ss.str();
	}


	bool Constraint::operator==(const Constraint& constr) const {
		return this->instruction == constr.instruction && this->args == constr.args;
	}

	Constraint& Constraint::operator=(const Constraint& constr) {
		this->instruction = constr.instruction; this->args = constr.args;
		return *this;
	}



	Constraint::Constraint(const Strg& instruction/* = ""*/, const deque<ConstraintArg>& args/* = {}*/) {
		this->instruction = instruction;
		this->args = args;
	}

	//provided for sort
	bool Constraint::operator<(const Constraint& c) const {
		if (this->instruction != c.instruction) {
			return this->instruction < c.instruction;
		}
		else {
			return this->args < c.args;

		}
	}



	/************************************


	ConstraintBinaryInfo


	***********************************/


	Intl ConstraintBinaryInfo::parseAndWriteObject(const Constraint& constr_arg, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout
				<< "FKABinaryInfo::parseAndWriteObject(const FileKeyAddr&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->instruction_info.parseAndWriteObject(constr_arg.instruction, fs);
		args_info.parseAndWriteObject(constr_arg.args, fs);
		return fs.tellp();
	}
	Intl ConstraintBinaryInfo::constructObject(Constraint& constr_arg, fstream& fs) {
		this->instruction_info.constructObject(constr_arg.instruction, fs);
		this->args_info.constructObject(constr_arg.args, fs);
		return fs.tellg();
	}



	/*********************************


	FlagBit


	************************************/
	FlagBit& FlagBit::operator=(const FlagBit& flagBit) {
		this->execute = flagBit.execute;
		this->type = flagBit.type;
		//this->argFlag = flagBit.argFlag;
		this->arg1_flag = flagBit.arg1_flag;
		this->arg2_flag = flagBit.arg2_flag;
		this->operator_flag = flagBit.operator_flag;
		this->result_flag = flagBit.result_flag;

		this->functionHandleFlag = flagBit.functionHandleFlag;
		this->formalArgFlag[0] = flagBit.formalArgFlag[0];
		this->formalArgFlag[1] = flagBit.formalArgFlag[1];
		this->formalArgFlag[2] = flagBit.formalArgFlag[2];
		this->formalArgFlag[3] = flagBit.formalArgFlag[3];
		this->changeable[0] = flagBit.changeable[0];
		this->changeable[1] = flagBit.changeable[1];
		this->changeable[2] = flagBit.changeable[2];
		this->changeable[3] = flagBit.changeable[3];
		this->unknown[0] = flagBit.unknown[0];
		this->unknown[1] = flagBit.unknown[1];
		this->unknown[2] = flagBit.unknown[2];
		this->unknown[3] = flagBit.unknown[3];
		this->compatible[0] = flagBit.compatible[0];
		this->compatible[1] = flagBit.compatible[1];
		this->compatible[2] = flagBit.compatible[2];
		this->compatible[3] = flagBit.compatible[3];
		this->arg1_constraints = (flagBit.arg1_constraints);
		this->arg2_constraints = (flagBit.arg2_constraints);
		this->operator_constraints = (flagBit.operator_constraints);
		this->result_constraints = (flagBit.result_constraints);

		this->a1s = flagBit.a1s;
		this->a2s = flagBit.a2s;
		this->ops = flagBit.ops;
		this->res = flagBit.res;
		this->tfFKA = flagBit.tfFKA;
		return *this;
	}

	void FlagBit::addConstraint(Intg pos, const Constraint& constr) {
		set<Constraint>* constraints = nullptr;
		if (pos == 0) {
			constraints = &arg1_constraints;
		}
		else if (pos == 1) {
			constraints = &arg2_constraints;
		}
		else if (pos == 2) {
			constraints = &operator_constraints;
		}
		else if (pos == 3) {
			constraints = &result_constraints;
		}
		else {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "invalid position, exit(-1)"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
				OutputDebugString(winfo.c_str());
#endif
				cout << info << endl;
				ignoreCount++;
			}
#endif
			cout << "invalid position, exit(-1)" << endl;
			assert(false); exit(-1);

		}
		constraints->insert(constr);
		//bool no_need_to_add = false;
		//Intg l = constraints->size();
		//Intg i = 0;
		////bool conflict_exist = false;
		//while (i < l) {
		//	auto& constr_i = (*constraints)[i];
		//	if (constr_i == constr) {
		//		no_need_to_add = true;
		//		break;
		//	}
		//	if (constr_i.args.size() != constr.args.size()) {
		//		continue;
		//	}

		//	//dont compare non immediate in this block, only immediate data.
		//	bool constr_i_with_non_imm_in_args = false;
		//	bool constr_with_non_imm_in_args = false;

		//	for (auto& arg_ : constr_i.args) {
		//		if (arg_.isImmediate()) {
		//			constr_i_with_non_imm_in_args = true;
		//			break;
		//		}
		//	}
		//	if (constr_i_with_non_imm_in_args == true)
		//	{
		//		continue;
		//	}
		//	for (auto& arg_ : constr.args) {
		//		if (arg_.isImmediate()) {
		//			constr_with_non_imm_in_args = true;
		//			break;
		//		}
		//	}
		//	if (constr_with_non_imm_in_args == true)
		//	{
		//		continue;
		//	}

		//	bool incomparable = false;
		//	for (Intg j = constr.args.size() - 1; j >= 0; --j) {
		//		if (constr.args[j].argFlag != constr_i.args[j].argFlag) {
		//			incomparable = true;
		//			break;
		//		}
		//	}
		//	if (incomparable == true) {
		//		continue;
		//	}

		//	/*		if (constr_i.instruction == "eq" && constr_i.args.size() == 1) {
		//				if (constr.instruction == "eq"&& constr_i.args[0] != constr.args[0]) {
		//					conflict_exist == true;
		//					break;
		//				}
		//				else if (constr.instruction == "ne"&& constr_i.args[0] == constr.args[0]) {
		//					conflict_exist == true;
		//					break;
		//				}
		//				else if (constr.instruction == "ne") {

		//				}
		//			}*/
		//	++i;
		//}
		//if (no_need_to_add == false) {
		//	constraints->insert(constr);
		//}

		return;
	}

	Strg FlagBit::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << "exe:" << execute << "\ttype:" << type
			<< " tfFKA:[" << tfFKA.toStrg() << "]\t" << "asc:["
			<< a1s.toStrg() << "\t| " << a2s.toStrg() << "\t| "
			<< ops.toStrg() << "\t| " << res.toStrg() << "\t]"

			<< "form:[" << formalArgFlag[0] << " " << formalArgFlag[1]
			<< " " << formalArgFlag[2] << " " << formalArgFlag[3]
			<< "]\tfhf:[" << functionHandleFlag << "]\tchag:["
			<< changeable[0] << " " << changeable[1] << " " << changeable[2]
			<< " " << changeable[3] << "]\tcompatible:[" << compatible[0] << " " << compatible[1] << " "
			<< compatible[2] << " " << compatible[3] << "]\targFlag:[" << arg1_flag << " "
			<< arg2_flag << " " << operator_flag << " " << result_flag
			<< "] ";
		if (arg1_constraints.size() != 0) {
			ss << " arg1_constraints:[";
			auto it = arg1_constraints.begin();
			while (it != arg2_constraints.end()) {
				ss << "[" << it->toStrg() << "], ";
			}
			ss << "]";
		}
		if (arg2_constraints.size() != 0) {
			ss << " arg2_constraints:[";
			auto it = arg2_constraints.begin();
			while (it != arg2_constraints.end()) {
				ss << "[" << it->toStrg() << "], ";
			}
			ss << "]";
		}
		if (operator_constraints.size() != 0) {
			ss << " operator_constraints:[";
			auto it = operator_constraints.begin();
			while (it != operator_constraints.end()) {
				ss << "[" << it->toStrg() << "], ";
			}
			ss << "]";
		}
		if (result_constraints.size() != 0) {
			ss << " result_constraints:[";
			auto it = result_constraints.begin();
			while (it != result_constraints.end()) {
				ss << "[" << it->toStrg() << "], ";
			}
			ss << "]";
		}
		return ss.str();
	}

	/****************************************


	FlagBitBinaryInfo


	***********************************/


	Intl FlagBitBinaryInfo::parseAndWriteObject(const FlagBit& flagBit, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "parseAndWriteObject(const FlagBit&, fstream&)" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->execute = flagBit.execute;
		this->type = flagBit.type;
		//this->argFlag = flagBit.argFlag;
		this->arg1_flag = flagBit.arg1_flag;
		this->arg2_flag = flagBit.arg2_flag;
		this->operator_flag = flagBit.operator_flag;
		this->result_flag = flagBit.result_flag;
		this->asc_locked = flagBit.asc_locked;


		this->functionHandleFlag = flagBit.functionHandleFlag;
		this->formalArgFlag[0] = flagBit.formalArgFlag[0];
		this->formalArgFlag[1] = flagBit.formalArgFlag[1];
		this->formalArgFlag[2] = flagBit.formalArgFlag[2];
		this->formalArgFlag[3] = flagBit.formalArgFlag[3];
		this->changeable[0] = flagBit.changeable[0];
		this->changeable[1] = flagBit.changeable[1];
		this->changeable[2] = flagBit.changeable[2];
		this->changeable[3] = flagBit.changeable[3];
		this->unknown[0] = flagBit.unknown[0];
		this->unknown[1] = flagBit.unknown[1];
		this->unknown[2] = flagBit.unknown[2];
		this->unknown[3] = flagBit.unknown[3];
		this->compatible[0] = flagBit.compatible[0];
		this->compatible[1] = flagBit.compatible[1];
		this->compatible[2] = flagBit.compatible[2];
		this->compatible[3] = flagBit.compatible[3];
		this->arg1_constraints_info.parseAndWriteObject(flagBit.arg1_constraints, fs);
		this->arg2_constraints_info.parseAndWriteObject(flagBit.arg2_constraints, fs);
		this->operator_constraints_info.parseAndWriteObject(flagBit.operator_constraints, fs);
		this->result_constraints_info.parseAndWriteObject(flagBit.result_constraints, fs);

		this->a1sInfo.parseAndWriteObject(flagBit.a1s, fs);
		this->a2sInfo.parseAndWriteObject(flagBit.a2s, fs);
		this->opsInfo.parseAndWriteObject(flagBit.ops, fs);
		this->resInfo.parseAndWriteObject(flagBit.res, fs);
		this->tfFKAInfo.parseAndWriteObject(flagBit.tfFKA, fs);
		return fs.tellp();

	}


	Intl FlagBitBinaryInfo::constructObject(FlagBit& flagBit, fstream& fs) {
		flagBit.execute = this->execute;
		flagBit.type = this->type;
		//flagBit.argFlag = this->argFlag;
		flagBit.arg1_flag = this->arg1_flag;
		flagBit.arg2_flag = this->arg2_flag;
		flagBit.operator_flag = this->operator_flag;
		flagBit.result_flag = this->result_flag;
		flagBit.asc_locked = this->asc_locked;

		flagBit.functionHandleFlag = this->functionHandleFlag;
		flagBit.formalArgFlag[0] = this->formalArgFlag[0];
		flagBit.formalArgFlag[1] = this->formalArgFlag[1];
		flagBit.formalArgFlag[2] = this->formalArgFlag[2];
		flagBit.formalArgFlag[3] = this->formalArgFlag[3];
		flagBit.changeable[0] = this->changeable[0];
		flagBit.changeable[1] = this->changeable[1];
		flagBit.changeable[2] = this->changeable[2];
		flagBit.changeable[3] = this->changeable[3];
		flagBit.unknown[0] = this->unknown[0];
		flagBit.unknown[1] = this->unknown[1];
		flagBit.unknown[2] = this->unknown[2];
		flagBit.unknown[3] = this->unknown[3];
		flagBit.compatible[0] = this->compatible[0];
		flagBit.compatible[1] = this->compatible[1];
		flagBit.compatible[2] = this->compatible[2];
		flagBit.compatible[3] = this->compatible[3];
		this->arg1_constraints_info.constructObject(flagBit.arg1_constraints, fs);
		this->arg2_constraints_info.constructObject(flagBit.arg2_constraints, fs);
		this->operator_constraints_info.constructObject(flagBit.operator_constraints, fs);
		this->result_constraints_info.constructObject(flagBit.result_constraints, fs);
		this->a1sInfo.constructObject(flagBit.a1s, fs);
		this->a2sInfo.constructObject(flagBit.a2s, fs);
		this->opsInfo.constructObject(flagBit.ops, fs);
		this->resInfo.constructObject(flagBit.res, fs);
		this->tfFKAInfo.constructObject(flagBit.tfFKA, fs);
		return fs.tellg();

	}


	/************************************


	Quaternion


	**********************************/

	Quaternion& Quaternion::operator=(const Quaternion& quat) {
		this->arg1_i = quat.arg1_i;

		this->arg1_fka = quat.arg1_fka;

		this->arg1_s = quat.arg1_s;

		//todo:: 先不考虑symbol id 的写入问题，需要在实际用到时再进行斟酌
		//    DataTable *ref1_ar = nullptr;//无效（在不会使用）

		arg2_i = quat.arg2_i;

		this->arg2_fka = quat.arg2_fka;

		this->arg2_s = quat.arg2_s;

		//    DataTable *ref2_ar = nullptr;

		operator_i = quat.operator_i;

		this->operator_fka = quat.operator_fka;

		this->operator_s = quat.operator_s;

		//    DataTable *refoperator_ar = nullptr;

		result_i = quat.result_i;

		this->result_fka = quat.result_fka;

		this->result_s = quat.result_s;

		//    DataTable *refresult_ar = nullptr;

		return *this;


	}

	Strg Quaternion::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << " [" << arg1_i << " " << arg1_fka.toStrg() << " " << arg1_s << " " << arg1_symbolID
			<< "\t|" << arg2_i << " " << arg2_fka.toStrg() << " " << arg2_s
			<< " " << arg2_symbolID << "\t|" << operator_i << " " << operator_fka.toStrg() << " "
			<< operator_s << " " << operator_symbolID << "\t|" << result_i << " " << result_fka.toStrg()
			<< " " << result_s << " " << result_symbolID << "]";
		return ss.str();
	}

	/****************************************


	QuaternionBinaryInfo


	*************************************/


	Intl QuaternionBinaryInfo::parseAndWriteObject(const Quaternion& quat, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "parseAndWriteObject(const Quaternion&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->arg1_i = quat.arg1_i;

		this->arg1_fka_info.parseAndWriteObject(quat.arg1_fka, fs);

		this->arg1_s_info.parseAndWriteObject(quat.arg1_s, fs);

		//    DataTable *ref1_ar = nullptr;//无效（在不会使用）

		arg2_i = quat.arg2_i;

		this->arg2_fka_info.parseAndWriteObject(quat.arg2_fka, fs);

		this->arg2_s_info.parseAndWriteObject(quat.arg2_s, fs);

		//    DataTable *ref2_ar = nullptr;

		operator_i = quat.operator_i;

		this->operator_fka_info.parseAndWriteObject(quat.operator_fka, fs);

		this->operator_s_info.parseAndWriteObject(quat.operator_s, fs);

		//    DataTable *refoperator_ar = nullptr;

		result_i = quat.result_i;

		this->result_fka_info.parseAndWriteObject(quat.result_fka, fs);

		this->result_s_info.parseAndWriteObject(quat.result_s, fs);

		//    DataTable *refresult_ar = nullptr;

		return fs.tellp();
	}



	Intl QuaternionBinaryInfo::constructObject(Quaternion& quat, fstream& fs) {
		quat.arg1_i = this->arg1_i;

		this->arg1_fka_info.constructObject(quat.arg1_fka, fs);

		this->arg1_s_info.constructObject(quat.arg1_s, fs);

		//    DataTable *ref1_ar = nullptr;//无效（在不会使用）

		quat.arg2_i = this->arg2_i;

		this->arg2_fka_info.constructObject(quat.arg2_fka, fs);

		this->arg2_s_info.constructObject(quat.arg2_s, fs);

		//    DataTable *ref2_ar = nullptr;

		quat.operator_i = operator_i;

		this->operator_fka_info.constructObject(quat.operator_fka, fs);

		this->operator_s_info.constructObject(quat.operator_s, fs);

		//    DataTable *refoperator_ar = nullptr;

		quat.result_i = result_i;

		this->result_fka_info.constructObject(quat.result_fka, fs);

		this->result_s_info.constructObject(quat.result_s, fs);

		//    DataTable *refresult_ar = nullptr;

		return fs.tellg();
	}


	/***********************************

	AssemblyFormula

	********************************/

	AssemblyFormula& AssemblyFormula::operator=(const AssemblyFormula& other) {
		this->flagBit = other.flagBit;
		this->quaternion = other.quaternion;
		return *this;
	}

	Strg AssemblyFormula::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << " " << flagBit.toStrg() << " " << quaternion.toStrg() << " ";
		return ss.str();
	}



	/*************************************

	AssemblyFormulaBinaryInfo

	*********************************/

	Intl AssemblyFormulaBinaryInfo::parseAndWriteObject(const AssemblyFormula& asf, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "parseAndWriteObject(const AssemblyFormula&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->flagBitInfo.parseAndWriteObject(asf.flagBit, fs);
		this->quaternionInfo.parseAndWriteObject(asf.quaternion, fs);
		return fs.tellp();
	}

	Intl AssemblyFormulaBinaryInfo::constructObject(AssemblyFormula& asf, fstream& fs) {
		this->flagBitInfo.constructObject(asf.flagBit, fs);
		this->quaternionInfo.constructObject(asf.quaternion, fs);
		return fs.tellg();
	}






	/*********************************************


	Code


	****************************************/


	Code::Code(const Strg& locateFileKey, const FKA& scopeFKA_,
		const AssemblyFormula& assemblyFormula_) {
		this->fileKey = locateFileKey;
		scopeFKA = scopeFKA_;
		assemblyFormula = assemblyFormula_;
	}

	//Code& Code::clear() {
	//	this->fileKey = "";
	//	this->scopeFKA = FKA();
	//	this->assemblyFormula = AssemblyFormula();
	//	return *this;
	//}
	Code& Code::operator=(const Code& code_) {
		fileKey = code_.fileKey;
		scopeFKA = code_.scopeFKA;
		assemblyFormula = code_.assemblyFormula;

		return *this;
	}

	Strg Code::toStrg() const {
		/*stringstream ss;
		ss.precision(PRECISION);
		ss << toBrief()<<endl;
		ss << "Code::scop:[" << scopeFKA.toStrg() << "]\t " << assemblyFormula.toStrg();

		return ss.str();*/
		return toBrief();
	}
	Strg Code::toBrief() const {
		stringstream ss;
		ss.precision(PRECISION);
		Strg prefix;
		//execute
		if (this->assemblyFormula.flagBit.execute == false) {
			prefix += "F_:";
		}
		else if (this->assemblyFormula.flagBit.execute == Du) {
			prefix += "Du:";
		}
		else if (this->assemblyFormula.flagBit.execute == COOL_R) {
			prefix += "R_:";
		}
		//type
		if (this->assemblyFormula.flagBit.type == COOL_QS) {
			prefix += "QS";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_QE) {
			prefix += "QE";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_C) {
			prefix += "C";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_M) {
			prefix += "";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_ME) {
			prefix += "ME";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_FUN) {
			prefix += "FUN";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_R) {
			prefix += "RETURN";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_B) {
			prefix += "BRANCH";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_L) {
			prefix += "LOOP";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_TD) {
			prefix += "TD";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_S) {
			prefix += "CLASS";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_Y) {
			prefix += "Y";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_LST) {
			prefix += "LST";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_FIL) {
			prefix += "#file";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_LIN) {
			prefix += "#line";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_LOAD) {
			prefix += "#load";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_CTN) {
			prefix += "CONTINUE";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_BRK) {
			prefix += "BREAK";
		}
		else if (this->assemblyFormula.flagBit.type == COOL_ABT) {
			prefix += "ABORT";
		}
		else if (this->assemblyFormula.flagBit.type == EXPRQS) {
			prefix += "EXPRQS";
		}
		else if (this->assemblyFormula.flagBit.type == EXPRQE) {
			prefix += "EXPRQE";
		}
		//var
		ss << calign(prefix, 10);
		ss << "\t|" << calign(calign((*this)[0].toBrief(), 32) + calign((*this)[1].toBrief(), 32) + calign((*this)[2].toBrief(), 32) + calign((*this)[3].toBrief(), 32), 160) << "|\t";
		//tfaddr
		Strg tfaddrstr = "";
		if (this->assemblyFormula.flagBit.functionHandleFlag == true) {
			tfaddrstr += "root:";
		}
		tfaddrstr = tfaddrstr + TemplateFunctionTableHash::getTFNameByFKA(this->assemblyFormula.flagBit.tfFKA);
		ss << calign(tfaddrstr);

		//scope
		if (this->scopeFKA != fkanull) {
			ss << calign(this->scopeFKA.toStrg());

		}
		else {
			ss << calign("-");
		}

		////other 
		//ss << assemblyFormula.toStrg();
		return ss.str();
	}
	/**
	 * @brief 将code的指定位置（0,1,2,3,）的相应参数的“参数类型”、“参数的值”替换为arg的对应值，其他属性不变
	 * @param position
	 * @param arg
	 * @param setFlagBit whether or not assign the flagbits (constraints, changeable, unknown, compatible) of arg to the code.
	 */
	void Code::setArg(const Intg& position, const Arg& arg, bool setFlagBit, bool setArgScope /*= false*/) {
		switch (position) {
		case 0:
			assemblyFormula.flagBit.arg1_flag = arg.argFlag;
			assemblyFormula.quaternion.arg1_fka = arg.arg_fka;
			assemblyFormula.quaternion.arg1_s = arg.arg_s;
			assemblyFormula.quaternion.arg1_i = arg.arg_i;
			assemblyFormula.quaternion.arg1_symbolID = arg.symbolID;
			if (setFlagBit) {
				assemblyFormula.flagBit.arg1_constraints = arg.constraints;
				assemblyFormula.flagBit.changeable[0] = arg.changeable;
				assemblyFormula.flagBit.unknown[0] = arg.unknown;
				assemblyFormula.flagBit.compatible[0] = arg.compatible;
			}
			if (setArgScope) {
				assemblyFormula.flagBit.a1s = arg.asc;
			}
			break;
		case 1:
			assemblyFormula.flagBit.arg2_flag = arg.argFlag;
			assemblyFormula.quaternion.arg2_fka = arg.arg_fka;
			assemblyFormula.quaternion.arg2_s = arg.arg_s;
			assemblyFormula.quaternion.arg2_i = arg.arg_i;
			assemblyFormula.quaternion.arg2_symbolID = arg.symbolID;
			if (setFlagBit) {
				assemblyFormula.flagBit.arg2_constraints = arg.constraints;
				assemblyFormula.flagBit.changeable[1] = arg.changeable;
				assemblyFormula.flagBit.unknown[1] = arg.unknown;
				assemblyFormula.flagBit.compatible[1] = arg.compatible;
			}
			if (setArgScope) {
				assemblyFormula.flagBit.a2s = arg.asc;
			}
			break;
		case 2:
			assemblyFormula.flagBit.operator_flag = arg.argFlag;
			assemblyFormula.quaternion.operator_fka = arg.arg_fka;
			assemblyFormula.quaternion.operator_s = arg.arg_s;
			assemblyFormula.quaternion.operator_i = arg.arg_i;
			assemblyFormula.quaternion.operator_symbolID = arg.symbolID;
			if (setFlagBit) {
				assemblyFormula.flagBit.operator_constraints = arg.constraints;
				assemblyFormula.flagBit.changeable[2] = arg.changeable;
				assemblyFormula.flagBit.unknown[2] = arg.unknown;
				assemblyFormula.flagBit.compatible[2] = arg.compatible;
			}
			if (setArgScope) {
				assemblyFormula.flagBit.ops = arg.asc;
			}
			break;
		case 3:
			assemblyFormula.flagBit.result_flag = arg.argFlag;
			assemblyFormula.quaternion.result_fka = arg.arg_fka;
			assemblyFormula.quaternion.result_s = arg.arg_s;
			assemblyFormula.quaternion.result_i = arg.arg_i;
			assemblyFormula.quaternion.result_symbolID = arg.symbolID;
			if (setFlagBit) {
				assemblyFormula.flagBit.result_constraints = arg.constraints;
				assemblyFormula.flagBit.changeable[3] = arg.changeable;
				assemblyFormula.flagBit.unknown[3] = arg.unknown;
				assemblyFormula.flagBit.compatible[3] = arg.compatible;
			}
			if (setArgScope) {
				assemblyFormula.flagBit.res = arg.asc;
			}
			break;
		default:
			break;
		}

	}
	void Code::replaceFileKey(const Strg& fileKeyOrg, const Strg& fileKeyNew) {
		if (this->scopeFKA.fileKey == fileKeyOrg) {
			this->scopeFKA.fileKey = fileKeyNew;
		}
		/*if (this->assemblyFormula.flagBit.a1s.fileKey == fileKeyOrg) {
			this->assemblyFormula.flagBit.a1s.fileKey = fileKeyNew;
		}
		if (this->assemblyFormula.flagBit.a2s.fileKey == fileKeyOrg) {
			this->assemblyFormula.flagBit.a2s.fileKey = fileKeyNew;
		}
		if (this->assemblyFormula.flagBit.ops.fileKey == fileKeyOrg) {
			this->assemblyFormula.flagBit.ops.fileKey = fileKeyNew;
		}
		if (this->assemblyFormula.flagBit.res.fileKey == fileKeyOrg) {
			this->assemblyFormula.flagBit.res.fileKey = fileKeyNew;
		}*/

	}
	/**
	 * @brief 获得对应位置（arg1，arg2，op，res）位置的参数
	 * @attention 对于asc标志位，若代码中明确定义了asc，则以asc为准，
	 * 否则若asc为空且对应arg确实为一个变量，则asc取代码所在scopeAddr
	 * @param pos
	 * @return
	 */
	Arg Code::operator[](const Intg& pos) const {
		assert(pos >= 0 && pos <= 3);
		/*
		 #if debug
		 {
		 static int ignorecount = 0;
		 cout << "Code::operator [](const int&)" << " " << "\tignorecount:["
		 << ignorecount++ << "\t]("
		 << __FILE__<<":"
		 << __LINE__ << ":0" << ")" << endl;
		 }
		 #endif
		 */
		Intg enableFormalArgBit = false;
		// if (assemblyFormula.flagBit.execute == F_) {
		enableFormalArgBit = true;
		//}
		Arg arg;
		if (pos == 0) {
			arg.assign(
				Arg(assemblyFormula.flagBit.arg1_flag,
					assemblyFormula.quaternion.arg1_fka,
					assemblyFormula.quaternion.arg1_i,
					assemblyFormula.quaternion.arg1_s,
					assemblyFormula.quaternion.arg1_symbolID,
					assemblyFormula.quaternion.ref1_ar,
					enableFormalArgBit
					&& assemblyFormula.flagBit.formalArgFlag[0],
					assemblyFormula.flagBit.changeable[0],
					assemblyFormula.flagBit.unknown[0],
					assemblyFormula.flagBit.compatible[0],
					assemblyFormula.flagBit.a1s,
					assemblyFormula.flagBit.arg1_constraints
				));

		}
		else if (pos == 1) {
			arg.assign(
				Arg(assemblyFormula.flagBit.arg2_flag,
					assemblyFormula.quaternion.arg2_fka,
					assemblyFormula.quaternion.arg2_i,
					assemblyFormula.quaternion.arg2_s,
					assemblyFormula.quaternion.arg2_symbolID,
					assemblyFormula.quaternion.ref2_ar,
					enableFormalArgBit
					&& assemblyFormula.flagBit.formalArgFlag[1],
					assemblyFormula.flagBit.changeable[1],
					assemblyFormula.flagBit.unknown[1],
					assemblyFormula.flagBit.compatible[1],
					assemblyFormula.flagBit.a2s,
					assemblyFormula.flagBit.arg2_constraints

				));
		}
		else if (pos == 2) {
			arg.assign(
				Arg(assemblyFormula.flagBit.operator_flag,
					assemblyFormula.quaternion.operator_fka,
					assemblyFormula.quaternion.operator_i,
					assemblyFormula.quaternion.operator_s,
					assemblyFormula.quaternion.operator_symbolID,
					assemblyFormula.quaternion.refoperator_ar,
					enableFormalArgBit
					&& assemblyFormula.flagBit.formalArgFlag[2],
					assemblyFormula.flagBit.changeable[2],
					assemblyFormula.flagBit.unknown[2],
					assemblyFormula.flagBit.compatible[2],
					assemblyFormula.flagBit.ops,
					assemblyFormula.flagBit.operator_constraints
				));
		}
		else if (pos == 3) {
			arg.assign(
				Arg(assemblyFormula.flagBit.result_flag,
					assemblyFormula.quaternion.result_fka,
					assemblyFormula.quaternion.result_i,
					assemblyFormula.quaternion.result_s,
					assemblyFormula.quaternion.result_symbolID,
					assemblyFormula.quaternion.refresult_ar,
					enableFormalArgBit
					&& assemblyFormula.flagBit.formalArgFlag[3],
					assemblyFormula.flagBit.changeable[3],
					assemblyFormula.flagBit.unknown[3],
					assemblyFormula.flagBit.compatible[3],
					assemblyFormula.flagBit.res,
					assemblyFormula.flagBit.result_constraints
				));
		}

		/* if (arg.asc != addrnull
		 && (arg.argFlag == S_Bs
		 || (arg.argFlag == S_Dz && arg.arg_fka != addrnull)
		 || arg.argFlag == Y_Dz)) {
		 arg.asc = scopeAddr;
		 }*/
		return arg;
#if debug
		cerr << "Code[n] ouf of bound,should be 0~3 " << endl;
#endif
	}
	/**
	 * @brief 判断一行代码中是否包含另一个变量。
	 * @attention 不考虑引用所指向的变量，不考虑非变量类型标志位 (acm::name)
	 * @param arg
	 * @return
	 */
	bool Code::includeArg(const Arg& arg, acm mode) const {

		for (int i = 0; i < 4; ++i) {
			if (Arg::intersect(this->operator[](i), arg)) {
				return true;
			}
		}
		return false;



		/*(assemblyFormula.flagBit.arg1_flag == arg.argFlag
		&& assemblyFormula.quaternion.arg1_fka == arg.arg_fka
		&& assemblyFormula.quaternion.arg1_i == arg.arg_i
		&& assemblyFormula.quaternion.arg1_s == arg.arg_s)
		|| (assemblyFormula.flagBit.arg2_flag == arg.argFlag
			&& assemblyFormula.quaternion.arg2_fka == arg.arg_fka
			&& assemblyFormula.quaternion.arg2_i == arg.arg_i
			&& assemblyFormula.quaternion.arg2_s == arg.arg_s)

		|| (assemblyFormula.flagBit.operator_flag == arg.argFlag
			&& assemblyFormula.quaternion.operator_fka == arg.arg_fka
			&& assemblyFormula.quaternion.operator_i == arg.arg_i
			&& assemblyFormula.quaternion.operator_s == arg.arg_s)
		|| (assemblyFormula.flagBit.result_flag == arg.argFlag
			&& assemblyFormula.quaternion.result_fka == arg.arg_fka
			&& assemblyFormula.quaternion.result_i == arg.arg_i
			&& assemblyFormula.quaternion.result_s == arg.arg_s);*/

	}

	/************************************


	CodeBinaryInfo


	***********************************/

	Intl CodeBinaryInfo::parseAndWriteObject(const Code& code, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "parseAndWriteObject(const Code&, fstream&)" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->fileKeyInfo.parseAndWriteObject(code.fileKey, fs);
		this->scopeFKAInfo.parseAndWriteObject(code.scopeFKA, fs);
		this->assemblyFormulaInfo.parseAndWriteObject(code.assemblyFormula, fs);
		return fs.tellp();

	}


	Intl CodeBinaryInfo::constructObject(Code& code, fstream& fs) {
		this->fileKeyInfo.constructObject(code.fileKey, fs);
		this->scopeFKAInfo.constructObject(code.scopeFKA, fs);
		this->assemblyFormulaInfo.constructObject(code.assemblyFormula, fs);
		return fs.tellg();

	}




	/**********************************



	CodeTable (partial)



	************************************/

	Intg CodeTable::size() const {
		return this->addrdq.size();
	}

	CodeTable& CodeTable::operator=(const CodeTable& cdt) {
		this->fileKey = cdt.fileKey;
		this->cdth = cdt.cdth;
		this->addrdq = cdt.addrdq;
		this->codedq = cdt.codedq;
		return (*this);

	}

	const bool CodeTable::addCode(const Addr& codeAddr, const Code& code) {
		addrdq.push_back(codeAddr);
		codedq.push_back(code);
		return true;
	}

	Strg CodeTable::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		//ss << "CodeTable(brief):" << endl;

		Intg s = addrdq.size();
		for (Intg p = 0; p < s; p++) {
			ss << calign(COOLANG::toStrg(p), 8) << calign(codedq[p].assemblyFormula.flagBit.execute == Du ? "Du:" : "" + addrdq[p].toStrg(), 24) << calign(codedq[p].toBrief(), 72) << endl;

		}
		//ss << "CodeTable(full):" << endl;
		//for (Intg p = 0; p < s; p++) {
		//	ss << "Addr:" << addrdq[p].toStrg() << "\t" << codedq[p].toStrg() << endl;

		//}

		return ss.str();
	}

	bool CodeTable::find(const Addr& addr) {
		if (getAddrPosition(addrdq, addr) >= 0) {
			return true;
		}
		else {
			return false;
		}
	}
	Intg CodeTable::count(const Addr& addr) {
		if (getAddrPosition(addrdq, addr) >= 0) {
			return 1;
		}
		else {
			return 0;
		}
	}






	/*************************************

	CodeTableBinaryInfo

	**********************************/

	Intl CodeTableBinaryInfo::parseAndWriteObject(const CodeTable& cdt, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "parseAndWriteObject(const CodeTable&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->fileKeyInfo.parseAndWriteObject(cdt.fileKey, fs);
		this->codeForm = cdt.codeForm;
		this->completed = cdt.completed;
		this->dependentCdtFileKey_CodeForm_0or1_List_info.parseAndWriteObject(
			cdt.dependentCdtFileKey_CodeForm_0or1_List, fs);
		this->dependentCdtFileKey_CodeForm_2_List_info.parseAndWriteObject(
			cdt.dependentCdtFileKey_CodeForm_2_List, fs);
		this->dependentCdtFileKey_CodeForm_3_List_info.parseAndWriteObject(
			cdt.dependentCdtFileKey_CodeForm_3_List, fs);
		this->addrdqinfo.parseAndWriteObject(cdt.addrdq, fs);
		this->codedqinfo.parseAndWriteObject(cdt.codedq, fs);
		return fs.tellp();
	}



	Intl CodeTableBinaryInfo::constructObject(CodeTable& cdt, fstream& fs) {
		this->fileKeyInfo.constructObject(cdt.fileKey, fs);
		cdt.codeForm = this->codeForm;
		cdt.completed = this->completed;
		this->dependentCdtFileKey_CodeForm_0or1_List_info.constructObject(
			cdt.dependentCdtFileKey_CodeForm_0or1_List, fs);
		this->dependentCdtFileKey_CodeForm_2_List_info.constructObject(
			cdt.dependentCdtFileKey_CodeForm_2_List, fs);
		this->dependentCdtFileKey_CodeForm_3_List_info.constructObject(
			cdt.dependentCdtFileKey_CodeForm_3_List, fs);
		this->addrdqinfo.constructObject(cdt.addrdq, fs);
		this->codedqinfo.constructObject(cdt.codedq, fs);
		return fs.tellg();
	}


	/*******************************

	CodeTableHash

	********************************/
	Strg CodeTableHash::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << hline;
		for (auto& p : const_cast<CodeTableHash*>(this)->addr_file_map) {
			auto& cdt = const_cast<CodeTableHash*>(this)->file_cdt_map[p.second];
			ss << calign("CodeTable(" + cdt.fileKey + ")", 96) << "\n";
			ss << cdt.toStrg();
			ss << hline;
		}
		return ss.str();
	}

	Intg CodeTableHash::size() const {
		return file_cdt_map.size();
	}

	bool CodeTableHash::find(const FKA& fka) {
		if (this->file_addr_map.count(fka.fileKey) > 0) {
			if (this->operator [](fka.fileKey).count(fka.addr) > 0) {
				return true;
			}
		}
		return false;
	}



	/*************************************

	CodeTableHashBinaryInfo

	**********************************/


	Intg CodeTableHashBinaryInfo::parseAndWriteObject(const CodeTableHash& cdth, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "parseAndWriteObject(const CodeTableHash&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		file_cdt_map_info.parseAndWriteObject(cdth.file_cdt_map, fs);
		addr_file_map_info.parseAndWriteObject(cdth.addr_file_map, fs);
		file_addr_map_info.parseAndWriteObject(cdth.file_addr_map, fs);
		return fs.tellp();

	}


	Intg CodeTableHashBinaryInfo::constructObject(CodeTableHash& cdth, fstream& fs) {
		file_cdt_map_info.constructObject(cdth.file_cdt_map, fs);
		addr_file_map_info.constructObject(cdth.addr_file_map, fs);
		file_addr_map_info.constructObject(cdth.file_addr_map, fs);
		return fs.tellg();

	}









	/*********************************************

	CodeTable & CodeTableHash

	*****************************************/


	Addr CodeTable::getCodeAddrByOffset(const Addr& addr, const Intg offset) const {
		try {
			return addrdq.at(
				getAddrPosition<deque<Addr> >(this->addrdq, addr) + offset);
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTable::getCodeAddrByOffset:[" << e.what() << "] Addr:["
				<< addr.toStrg() << "] AddrPosition:["
				<< getAddrPosition<deque<Addr> >(this->addrdq, addr)
				<< "] offset:[" << offset << "]" << endl;
#endif
			return addrnull;
		}
	}
	FKA CodeTableHash::getCodeFKAByOffset(const FKA& fka, const Intg offset) const {
		CodeTable& cdt = (*const_cast<CodeTableHash*>(this))[fka.fileKey];
		Intg pos = getAddrPosition(cdt.addrdq, fka.addr);
		if (pos < 0) {

			{
				static int ignorecount = 0;
				cerr << "CodeTableHash::getCodeFKAByOffset(const FKA&, const int)"
					<< " can't find fka, exit(-1) " << "\tignorecount:["
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
			assert(false); exit(-1);
		}
		FKA fka_ = fka;
		if (pos + offset > 0 && pos + offset < cdt.size()) {
			Addr ad = cdt.getCodeAddrByOffset(fka.addr, offset);
			fka_.addr = ad;
			return fka_;

		}
		else if (pos + offset >= cdt.size()) {
			Intg offset_ = pos + offset - cdt.size() + 1;
			fka_.fileKey = this->getNextCdtFileKey(fka.fileKey);
			CodeTable& cdt_ = (*const_cast<CodeTableHash*>(this))[fka_.fileKey];
			fka_.addr = cdt_.addrdq.front();
			return getCodeFKAByOffset(fka_, offset_);

		}
		else {
			Intg offset_ = pos + offset + 1;
			fka_.fileKey = this->getLastCdtFileKey(fka.fileKey);
			CodeTable& cdt_ = (*const_cast<CodeTableHash*>(this))[fka_.fileKey];
			fka_.addr = cdt_.addrdq.back();
			return getCodeFKAByOffset(fka_, offset_);
		}

	}
	Addr CodeTable::getNextCodeAddr(const Addr& addr) const {
		try {
			return getCodeAddrByOffset(addr, 1);
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTable::getNextCodeAddr:[" << e.what() << "] Addr:["
				<< addr.toStrg() << "]" << endl;
#endif
			return addrnull;
		}
	}
	FKA CodeTableHash::getNextCodeFKA(const FKA& fka) const {
		try {
			return getCodeFKAByOffset(fka, 1);
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTableHash::getNextCodeFKA:[" << e.what() << "] FKA:["
				<< fka.toStrg() << "]" << endl;
#endif
			return fkanull;
		}
	}
	Addr CodeTable::getAboveCodeAddr(const Addr& addr) const {
		try {
			return getCodeAddrByOffset(addr, -1);
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTable::getNextCodeAddr:[" << e.what() << "] Addr:["
				<< addr.toStrg() << "]" << endl;
#endif
			return addrnull;
		}
	}
	FKA CodeTableHash::getAboveCodeFKA(const FKA& fka) const {
		try {
			return getCodeFKAByOffset(fka, -1);
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTableHash::getNextCodeFKA:[" << e.what() << "] FKA:["
				<< fka.toStrg() << "]" << endl;
#endif
			return fkanull;
		}
	}
	const Intg CodeTable::InsertCode(const Intg position, const Intg offset,
		const Addr& addr, const Code& code) {
		Intg pos = position + offset;
		try {
			deque<Addr>::iterator ita = (this->addrdq).begin() + pos;
			this->addrdq.insert(ita, addr);
			deque<Code>::iterator itc = (this->codedq).begin() + pos;
			this->codedq.insert(itc, code);
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTable::InsertCode:[" << e.what() << "] position:["
				<< position << "]" << endl;
#endif
		}
		return pos;

	}
	/**
	 * @brief 如果fka的fileKey可以检索到，则将代码插入fka对应file中的指定位置
	 * @brief 如果fka的fileKey不可以检索到且不为空，
	 * 则在fki对应fileKey的文件后面新增fileKey为fka的CodeTable。
	 * @param position
	 * @param offset
	 * @param fka
	 * @param code
	 * @return
	 */
	FKI CodeTableHash::InsertCode(const FKI& position, const Intg offset,
		const FKA& fka, const Code& code) {
		if (fka.fileKey != "") {

			if (this->file_cdt_map.count(fka.fileKey) > 0) {
				//如果fka的fileKey可以被检索，则直接插入
				Intg pos_ = file_cdt_map[fka.fileKey].InsertCode(position.ID,
					offset, fka.addr, code);
				return FKI(fka.fileKey, pos_);
			}
			else {
				if (this->file_cdt_map.count(position.fileKey) > 0) {
					//如果fka的fileKey不可以被检索而position可以被检索，则在position后新建cdt并插入
					Addr& addr_ = this->file_addr_map[position.fileKey];
					CodeTable cdt_;
					cdt_.fileKey = fka.fileKey;
					cdt_.InsertCode(0, 0, fka.addr, code);

					this->insertCdt(cdt_, addr_, false);
					return FKI(fka.fileKey, 0);
				}
				else {
					//如果position无法被检索到（或为空），则在cdth的尾端新建cdt并插入
					CodeTable cdt_;
					cdt_.fileKey = fka.fileKey;
					cdt_.InsertCode(0, 0, fka.addr, code);

					this->appendCdt(cdt_);
					return FKI(fka.fileKey, 0);
				}
			}

		}
		else {
			//在fka的fileKey为空的情况下
			if (this->file_cdt_map.count(position.fileKey) > 0) {
				//如果fka的fileKey为空则将其插入position对应fileKey
				CodeTable& cdt_ = this->operator [](position.fileKey);
				Intg pos_ = cdt_.InsertCode(position.ID, offset, fka.addr, code);

				return FKI(position.fileKey, pos_);
			}
			else {
				//如果position对应fileKey无法被检索到（或为空），则报错

				{
					static int ignorecount = 0;
					cerr
						<< "CodeTableHash::InsertCode(const FKI&, const int, const FKA&, const Code&)"
						<< " FileKeyNotFound, exit(-1) " << "\tignorecount:["
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

	}
	const Intg CodeTable::eraseCode(const Intg position, const Intg offset) {
		try {
			this->addrdq.erase(addrdq.begin() + position + offset);
			this->codedq.erase(codedq.begin() + position + offset);
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTable::eraseCode:[" << e.what() << "] position:["
				<< position << "]" << endl;
#endif
		}
		return position;
	}
	FKI CodeTableHash::eraseCode(const FKI& position, const Intg offset) {
		CodeTable& cdt = this->operator [](position.fileKey);
		FKI fki(position);

		if (position.ID + offset >= 0 && position.ID + offset < cdt.size()) {
			Intg pos_ = cdt.eraseCode(position.ID, offset);
			return FKI(position.fileKey, pos_);

		}
		else if (position.ID + offset >= cdt.size()) {
			Intg offset_ = position.ID + offset - cdt.size() + 1;
			fki.fileKey = this->getNextCdtFileKey(fki.fileKey);
			fki.ID = 0;
			return eraseCode(fki, offset_);

		}
		else {
			Intg offset_ = position.ID + offset + 1;
			fki.fileKey = this->getLastCdtFileKey(fki.fileKey);
			CodeTable& cdt_ = (*this)[fki.fileKey];
			fki.ID = cdt_.addrdq.size() - 1;
			return eraseCode(fki, offset_);
		}

	}
	FKI CodeTableHash::eraseCode(const FKA& fka, const Intg offset) {
		CodeTable& cdt = this->operator [](fka.fileKey);
		Intg pos = getAddrPosition(cdt.addrdq, fka.addr);
		if (pos < 0) {
			return fkinull;
		}
		FKI fki(fka.fileKey, pos);

		return eraseCode(fki, offset);
	}
	void CodeTableHash::eraseCdt(const Strg& fileKey) {
		Addr addr = this->file_addr_map[fileKey];
		this->file_addr_map.erase(fileKey);
		this->file_cdt_map.erase(fileKey);
		this->addr_file_map.erase(addr);
	}
	const Intg CodeTable::eraseCode(const Addr& addr, const Intg offset) {
		Intg position = getAddrPosition<deque<Addr> >(this->addrdq, addr);
		if (position < 0 || position >= (Intg)addrdq.size()) {
#if debug
			cout << "CodeTable::eraseCode err::can't find addr:[" << addr.toStrg()
				<< "]" << endl;
#endif
			return -1;
		}
		try {
			this->addrdq.erase(addrdq.begin() + position + offset);
			this->codedq.erase(codedq.begin() + position + offset);
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTable::eraseCode:[" << e.what() << "] position:["
				<< position << "]" << endl;
#endif
		}
		return position;
	}
	const FKA CodeTable::getScopeFKA(const Addr& codeAddr) const {
		try {
			return codedq.at(getAddrPosition<deque<Addr> >(this->addrdq, codeAddr)).scopeFKA;
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTable::getScopeFKA:[" << e.what() << "] FKA:["
				<< codeAddr.toStrg() << "]" << endl;
#endif
			return fkanull;
		}
	}
	FKA CodeTableHash::getScopeFKA(const FKA& codeFKA) const {
		try {
			CodeTable& cdt =
				(const_cast<CodeTableHash*>(this))->file_cdt_map[codeFKA.fileKey];
			return cdt.codedq.at(
				getAddrPosition<deque<Addr> >(cdt.addrdq, codeFKA.addr)).scopeFKA;
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTableHash::getScopeFKA:[" << e.what() << "] codeFKA:["
				<< codeFKA.toStrg() << "]" << endl;
#endif
			return fkanull;
		}
	}

	AssemblyFormula& CodeTable::getAssemblyFormula(const Addr& codeAddr) {
		try {
			return codedq.at(getAddrPosition<deque<Addr> >(this->addrdq, codeAddr)).assemblyFormula;
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTable::getAssemblyFormula:[" << e.what() << "] Addr:["
				<< codeAddr.toStrg() << "]" << endl;
#endif
			return asfnull;
		}
	}
	AssemblyFormula& CodeTableHash::getAssemblyFormula(const FKA& codeFKA) {
		try {

			return this->operator [](codeFKA).assemblyFormula;
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTableHash::getAssemblyFormula:[" << e.what()
				<< "] codeFKA:[" << codeFKA.toStrg() << "]" << endl;
#endif
			return asfnull;
		}
	}
	const bool CodeTable::sameScope(const Addr& addr1, const Addr& addr2) const {
		try {
			return getScopeFKA(addr1) == getScopeFKA(addr2);
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTable::sameScope:[" << e.what() << "] Addr:["
				<< addr1.toStrg() << " | " << addr2.toStrg() << "]" << endl;
#endif
			return false;
		}
	}
	const bool CodeTableHash::sameScope(const FKA& fka1, const FKA& fka2) const {
		try {
			return getScopeFKA(fka1) == getScopeFKA(fka2);
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTable::sameScope:[" << e.what() << "] Addr:["
				<< fka1.toStrg() << " | " << fka2.toStrg() << "]" << endl;
#endif
			return false;
		}
	}
	/**
	 * @brief 向codeTableHash中添加一行代码。如果codeFKA在cdth中已有对应的fileKey，则直接加入其末尾
	 * 若没有对应的fileKey，则在cdth末尾添加新的fileKey（对应的cdt），并将code加入其中。
	 * @param codeFKA
	 * @param code
	 * @return 是否添加成功
	 */
	const bool CodeTableHash::addCode(const FKA& codeFKA, const Code& code) {
		if (codeFKA.fileKey == "") {
			return false;

		}
		if (this->file_cdt_map.count(codeFKA.fileKey) > 0) {
			this->operator [](codeFKA.fileKey).addCode(codeFKA.addr, code);
		}
		else {
			CodeTable cdt_;
			cdt_.fileKey = codeFKA.fileKey;
			cdt_.addCode(codeFKA.addr, code);
			this->appendCdt(cdt_);

		}
		return true;

	}


	/**
	 * operator[ad]
	 * @attention 注意此函数当且仅当ad对应的code存在时才返回正确的引用，
	 * 当ad不存在时throw -1用，这样做是为了保证不被莫名插入代码。
	 * 因此在使用此函数之前一定要确定此地址正确
	 * @param addr
	 * @return
	 */
	Code& CodeTable::operator[](const Addr& addr) {
		if (addr == addrnull) {

			{
				static int ignorecount = 0;
				cerr << "CodeTable::operator [](const Addr&)"
					<< " input addrnull，throw(-1)" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
			throw - 1;
		}
		try {
			Intg pos = getAddrPosition<deque<Addr> >(this->addrdq, addr);
			if (pos < 0) {
				throw - 1;

			}
			return codedq[pos];
		}
		catch (exception& e) {
#if debug
			cerr << "CodeTable::operator[]:[" << e.what() << "] Addr:["
				<< addr.toStrg() << "]" << endl;
#endif
			throw - 1;
		}
	}
	Code& CodeTable::operator[](const Intg& pos) {
		if (pos < 0 || pos >= (Intg)(codedq.size())) {

			{
				static int ignorecount = 0;
				cerr << "CodeTable::operator [](const Intg&)" << " input pos<<["
					<< pos << "] out of range ，exit(-1)" << "\tignorecount:["
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
			cerr << "code index out of range, throw" << endl;
			throw exception("Code index out of range.");
		}

		return codedq[pos];

	}
	/**
	 * @brief 此函数用于将codeTable中addr对应所在语法树取消与函数的绑定。如果本来就没有绑定，则什么都不做
	 * @param addr
	 * @return pair<解绑的最小地址，解绑的最大地址>
	 */
	pair<Addr, Addr> CodeTable::unbindFunction(const Addr& addr) {
		Code& cd = (*this)[addr];
		FKA tffka = cd.assemblyFormula.flagBit.tfFKA;
		Addr unbindAddrMin = addr;
		Addr unbindAddrMax = addr;
		if (tffka.addr == addrnull) {
			return pair<Addr, Addr>();
		}
		else {

			Intg pos = getAddrPosition(this->addrdq, addr);
			Intg pos_to_top = pos;
			--pos_to_top;
			while (pos_to_top >= 0) {
				Code& cd1 = codedq[pos_to_top];

				if (cd1.assemblyFormula.flagBit.tfFKA
					== tffka && cd1.assemblyFormula.flagBit.functionHandleFlag == F_) {
					cd1.assemblyFormula.flagBit.tfFKA = fkanull;
					unbindAddrMin = addrdq[pos_to_top];
				}
				else {
					break;
				}
			}
			cd.assemblyFormula.flagBit.tfFKA = fkanull;
			if (cd.assemblyFormula.flagBit.functionHandleFlag == true) {
				cd.assemblyFormula.flagBit.functionHandleFlag = false;
				return pair<Addr, Addr>(unbindAddrMin, unbindAddrMax);
			}
			Intg pos_to_bottom = pos;
			++pos_to_bottom;
			while (pos_to_bottom < size()) {
				Code& cd2 = codedq[pos_to_bottom];
				if (cd2.assemblyFormula.flagBit.tfFKA == tffka) {
					unbindAddrMax = addrdq[pos_to_bottom];
					if (cd2.assemblyFormula.flagBit.functionHandleFlag == F_) {
						cd2.assemblyFormula.flagBit.tfFKA = fkanull;
					}
					else {
						cd2.assemblyFormula.flagBit.tfFKA = fkanull;
						cd.assemblyFormula.flagBit.functionHandleFlag = false;
						return pair<Addr, Addr>(unbindAddrMin, unbindAddrMax);
					}
				}
				else {
					return pair<Addr, Addr>(unbindAddrMin, unbindAddrMax);
				}
			}
		}
		return pair<Addr, Addr>(unbindAddrMin, unbindAddrMax);

	}
	/**
	 * @brief 此函数用于将codeTableHash中fka对应所在语法树取消与函数的绑定。如果本来就没有绑定，则什么都不做
	 * @attention 禁止跨CodeTable
	 * @param fka
	 * @return pair<解绑的最小地址，解绑的最大地址>
	 */
	pair<FKA, FKA> CodeTableHash::unbindFunction(const FKA& fka) {
		CodeTable& cdt = this->operator [](fka.fileKey);
		pair<Addr, Addr>&& paa = cdt.unbindFunction(fka.addr);
		return pair<FKA, FKA>(FKA(fka.fileKey, paa.first),
			FKA(fka.fileKey, paa.second));

	}

	/**
	 * @brief 复制addr所在区域的函数调用
	 * @param addr
	 * @return
	 */
	OpenAddrCDT CodeTable::copyFunctionCall(const Addr& addr) {
		Code& cd = (*this)[addr];
		FKA tffka = cd.assemblyFormula.flagBit.tfFKA;
		Addr functionCallAddrMin = addr;
		Addr functionCallAddrMax = addr;

		if (tffka == fkanull) {
			return OpenAddrCDT(CodeTable(), addrnull, addrnull);
		}
		else {

			Intg pos = getAddrPosition(this->addrdq, addr);
			Intg pos_to_top = pos;
			--pos_to_top;
			while (pos_to_top >= 0) {
				Code& cd1 = codedq[pos_to_top];

				if (cd1.assemblyFormula.flagBit.tfFKA
					== tffka && cd1.assemblyFormula.flagBit.functionHandleFlag == F_) {
					functionCallAddrMin = addrdq[pos_to_top];
					--pos_to_top;
				}
				else {
					break;
				}
			}
			if (cd.assemblyFormula.flagBit.functionHandleFlag == true) {
				return copyCodeTableIgnoreFILLIN(functionCallAddrMin,
					functionCallAddrMax);
			}
			Intg pos_to_bottom = pos;
			++pos_to_bottom;
			while (pos_to_bottom < size()) {
				Code& cd2 = codedq[pos_to_bottom];
				if (cd2.assemblyFormula.flagBit.tfFKA == tffka) {
					functionCallAddrMax = addrdq[pos_to_bottom];
					if (cd2.assemblyFormula.flagBit.functionHandleFlag == F_) {
						++pos_to_bottom;
					}
					else {
						return copyCodeTableIgnoreFILLIN(functionCallAddrMin,
							functionCallAddrMax);
					}
				}
				else {
					return copyCodeTableIgnoreFILLIN(functionCallAddrMin,
						functionCallAddrMax);
				}
			}
		}
		return copyCodeTableIgnoreFILLIN(functionCallAddrMin, functionCallAddrMax);
	}
	/**
	 * @brief 复制fka所在区域的函数调用
	 * @attention 禁止跨cdt
	 * @param fka
	 * @return
	 */
	OpenFKACDT CodeTableHash::copyFunctionCall(const FKA& fka) {
		CodeTable& cdt = this->operator [](fka.fileKey);
		auto&& open_addr_cdt = cdt.copyFunctionCall(fka.addr);
		return OpenFKACDT(get<0>(open_addr_cdt), FKA(fka.fileKey, get<1>(open_addr_cdt)), FKA(fka.fileKey, get<2>(open_addr_cdt)));
	}




	/**
	 * @brief 获得hash中位于指定位置的cdt的引用,禁止用此函数创建新的键值对，找不到对应的cdt throw-1
	 * @param pos （0,1,2,3）
	 * @return
	 */
	CodeTable& CodeTableHash::operator[](const Intg& pos) {
		if (pos >= size()) {
#if debug
			{
				static int ignorecount = 0;
				cout << "CodeTableHash::operator [](const int&)"
					<< " out of bound, throw(-1) " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			throw - 1;
		}
		auto&& it = this->addr_file_map.begin();
		Intg pos_ = pos;
		while (pos_-- > 0) {
			++it;
		}
		Strg& fileKey = it->second;
		return file_cdt_map[fileKey];

	}
	/**
	 * @brief 获得hash中位于指定addr的cdt的引用,禁止用此函数创建新的键值对
	 * @param pos （0,1,2,3）
	 * @return
	 */
	CodeTable& CodeTableHash::operator[](const Addr& addr) {
		if (addr_file_map.count(addr) <= 0) {

			{
				static int ignorecount = 0;
				cout << "CodeTableHash::operator [](const Addr &addr)"
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
			assert(false); exit(-1);
		}
		Strg& fileKey = addr_file_map[addr];
		return file_cdt_map[fileKey];

	}
	/**
	 * @brief 获得hash中位于指定fka的code的引用,禁止用此函数创建新的键值对,找不到throw-1
	 * @param
	 * @return
	 */
	Code& CodeTableHash::operator[](const FKA& fka) {
		if (file_cdt_map.count(fka.fileKey) <= 0) {

			{
				static int ignorecount = 0;
				cerr << "CodeTableHash::operator [](const FKA &fka)"
					<< " addr not found, exit(-1) " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}

			throw - 1;
		}
		auto& cdt = file_cdt_map[fka.fileKey];
		auto& code = cdt[fka.addr];
		return code;

	}
	/**
	 * @brief 获得hash中位于指定fki的code的引用,禁止用此函数创建新的键值对
	 * @param
	 * @return
	 */
	Code& CodeTableHash::operator[](const FKI& fki) {
		if (file_cdt_map.count(fki.fileKey) <= 0) {

			{
				static int ignorecount = 0;
				cout << "CodeTableHash::operator [](const FKI &fki)"
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
			assert(false); exit(-1);
		}

		return file_cdt_map[fki.fileKey][fki.ID];

	}
	/**
	 * @brief 获得hash中位于指定Strg#x的cdt的引用,禁止用此函数创建新的键值对
	 * @param pos （0,1,2,3）
	 * @return
	 */
	CodeTable& CodeTableHash::operator[](const Strg& fileKey) {
		if (file_cdt_map.count(fileKey) <= 0) {

			{
				static int ignorecount = 0;
				cout << "CodeTableHash::operator [](const Strg &fileKey)"
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
			assert(false); exit(-1);
		}
		return file_cdt_map[fileKey];

	}
	/**
	 * @brief 返回Asct对应addr在cdth中的上一个addr，如果不存在则返回addrnull
	 * @param addr
	 * @return
	 */
	Addr CodeTableHash::getLastCdtAddr(const Addr& addr) {
		auto&& it = addr_file_map.find(addr);
		if (it == addr_file_map.begin()) {
			return Addr();
		}
		else {
			--it;
			return it->first;
		}
	}
	Addr CodeTableHash::getNextCdtAddr(const Addr& addr) {
		auto&& it = addr_file_map.find(addr);
		if (it == addr_file_map.end()) {
			return Addr();
		}
		else {
			++it;
			return it->first;
		}
	}

	Strg CodeTableHash::getLastCdtFileKey(const Strg& filekey) const {
		auto&& it = file_addr_map.find(filekey);
		if (it == file_addr_map.end()) {
			return "";
		}
		else {
			Addr fileAddr = it->second;
			auto it2 = addr_file_map.find(fileAddr);
			if (it2 == addr_file_map.begin()) {
				return "";
			}
			--it2;
			return it2->second;
		}
	}
	Strg CodeTableHash::getNextCdtFileKey(const Strg& filekey) const {
		auto&& it = file_addr_map.find(filekey);
		if (it == file_addr_map.end()) {
			return "";
		}
		else {
			Addr fileAddr = it->second;
			auto it2 = addr_file_map.find(fileAddr);
			if (it2 == (--addr_file_map.end()) || it2 == addr_file_map.end()) {
				return "";
			}
			++it2;
			return it2->second;
		}
	}
	vector<Strg> CodeTableHash::getDependentFileKeyFileStem() {
		vector<Strg> fkfsv;
		set<Strg> fkfss;
		if (this->size() <= 0) {
			return fkfsv;
		}
		else {

			for (auto& afp : this->addr_file_map) {
				Strg& fileKey_ = afp.second;
				CodeTable& cd = this->operator [](fileKey_);
				for (auto& fileKey__ : cd.dependentCdtFileKey_CodeForm_3_List) {
					if (fkfss.count(fileKey__) > 0) {
						continue;
					}
					else {
						fkfss.insert(fileKey__);
						fkfsv.push_back(fileKey__);
					}

				}

			}
			return fkfsv;

		}

	}
	/**
	 * @brief 将cdt插入到当前addr之前或之后
	 * @param cdt
	 * @param currentAddr
	 * @param front 为真插入前，为假插入后面
	 * @return cdt所插入后对应的地址。
	 */
	Addr CodeTableHash::insertCdt(CodeTable& cdt, const Addr& currentAddr,
		const bool& front) {
		cdt.cdth = this;
		if (currentAddr == Addr()) {
			return appendCdt(cdt);
		}
		if (front == true) {

			Addr&& lastAddr = getLastCdtAddr(currentAddr);
			auto&& addrv = split(lastAddr, currentAddr, 2);
			Addr& addrInsert = addrv[1];
			this->file_addr_map[cdt.fileKey] = addrInsert;
			this->addr_file_map[addrInsert] = cdt.fileKey;
			this->file_cdt_map[cdt.fileKey] = cdt;
			return addrInsert;

		}
		else {
			Addr&& nextAddr = getNextCdtAddr(currentAddr);
			if (nextAddr != addrnull) {
				auto&& addrv = split(nextAddr, currentAddr, 2);
				Addr& addrInsert = addrv[1];
				this->file_addr_map[cdt.fileKey] = addrInsert;
				this->addr_file_map[addrInsert] = cdt.fileKey;
				this->file_cdt_map[cdt.fileKey] = cdt;
				return addrInsert;

			}
			else {
				return appendCdt(cdt);
			}

		}
	}
	/**
	 * @brief 在当前hash末尾添加一个cdt
	 * @param asct
	 * @return
	 */
	Addr CodeTableHash::appendCdt(CodeTable& cdt) {
		cdt.cdth = this;
		if (file_addr_map.size() <= 0) {
			Addr addrInsert(1000);
			this->addr_file_map[addrInsert] = cdt.fileKey;
			this->file_cdt_map[cdt.fileKey] = cdt;
			this->file_addr_map[cdt.fileKey] = addrInsert;
			return addrInsert;

		}
		else {
			Addr addrInsert = addr_file_map.rbegin()->first.addrv[0] + 3;
			this->addr_file_map[addrInsert] = cdt.fileKey;
			this->file_cdt_map[cdt.fileKey] = cdt;
			this->file_addr_map[cdt.fileKey] = addrInsert;
			return addrInsert;

		}

	}


	/**
 * @brief copy the max AST the node of specified addr in. the code at ad must not be Du
 * @attention copytree不会将Y/COOL_FIL/COOL_LIN类型代码一并拷贝，引用(Y)对应的代码段必须位于一段表达式对应代码段的前部。
 * @attention copytree不会将EXPRQS/EXPRQE与之间的代码一并拷贝，但会跳过相应区段选取相邻代码。同时选取最大的EXPRQE作为边界。EXPRQS、EXPRQE对禁止嵌套。
 * @attention copytree 会保留cdt的fileKey信息与cdth信息，但不会验证其有效性
 * @param ad
 * @return
 */
	OpenAddrCDT CodeTable::copyTree(const Addr& ad) const {

		Intg pos = getAddrPosition(addrdq, ad);

		if ((codedq[pos].assemblyFormula.flagBit.type == COOL_M)
			&& codedq[pos].assemblyFormula.flagBit.execute != Du) {

			CodeTable cdt = CodeTable();
			cdt.fileKey = this->fileKey;
			cdt.cdth = this->cdth;
			cdt.addrdq.push_back(addrdq[pos]);
			cdt.codedq.push_back(codedq[pos]);
			Intg pos_to_top = pos - 1;

			bool skipM = false;
			set<Addr> openLowAddrBound = {};
			set<Intg> specialTypes = { COOL_M, COOL_FIL, COOL_LIN, EXPRQS, EXPRQE };


			while (pos_to_top >= 0

				&& (specialTypes.count(codedq[pos_to_top].assemblyFormula.flagBit.type) > 0
					|| codedq[pos_to_top].assemblyFormula.flagBit.execute
					== Du)) {
				if (codedq[pos_to_top].assemblyFormula.flagBit.execute != Du) {
					if (codedq[pos_to_top].assemblyFormula.flagBit.type == COOL_M && skipM == false) {
						cdt.addrdq.push_front(addrdq[pos_to_top]);
						cdt.codedq.push_front(codedq[pos_to_top]);
					}
					else if (codedq[pos_to_top].assemblyFormula.flagBit.type == COOL_M && skipM == true) {
						//in expr scope (when copying outer cdt)
						//jump over
					}
					else if (codedq[pos_to_top].assemblyFormula.flagBit.type == EXPRQE && skipM == false) {
						//enter expr scope (when copying outer cdt)
						skipM = true;
						openLowAddrBound.insert(addrdq[pos_to_top]);
					}
					else if (codedq[pos_to_top].assemblyFormula.flagBit.type == EXPRQS && skipM == true) {
						//leave expr scope (when copying outer cdt)
						skipM = false;
					}
					else if (codedq[pos_to_top].assemblyFormula.flagBit.type == EXPRQS && skipM == false) {
						//leave expr scope (when copying inner cdt)
						break;
					}
					else {
						break;
					}
				}
				pos_to_top--;
			}

			skipM = false;
			Intg pos_to_root = pos + 1;
			while (pos_to_root < (Intg)addrdq.size()

				&& (specialTypes.count(codedq[pos_to_root].assemblyFormula.flagBit.type) > 0
					|| codedq[pos_to_root].assemblyFormula.flagBit.execute
					== Du)) {
				if (codedq[pos_to_root].assemblyFormula.flagBit.execute != Du) {
					if (codedq[pos_to_root].assemblyFormula.flagBit.type == COOL_M && skipM == false) {
						cdt.addrdq.push_back(addrdq[pos_to_root]);
						cdt.codedq.push_back(codedq[pos_to_root]);
					}
					else if (codedq[pos_to_root].assemblyFormula.flagBit.type == COOL_M && skipM == true) {
						//jump over
					}
					else if (codedq[pos_to_root].assemblyFormula.flagBit.type == EXPRQS && skipM == false) {
						skipM = true;
					}
					else if (codedq[pos_to_root].assemblyFormula.flagBit.type == EXPRQE && skipM == true) {
						openLowAddrBound.insert(addrdq[pos_to_root]);
						skipM = false;
					}
					else if (codedq[pos_to_root].assemblyFormula.flagBit.type == EXPRQE && skipM == false) {
						break;
					}
					else {
						break;
					}
				}
				pos_to_root++;
			}
			Intg posMin = getAddrPosition(this->addrdq, cdt.addrdq.front());
			Intg posMax = getAddrPosition(this->addrdq, cdt.addrdq.back());
			Addr openAddrMin = (posMin - 1 >= 0) ? addrdq[posMin - 1] : addrnull;
			openLowAddrBound.insert(openAddrMin);
			openAddrMin = openLowAddrBound.rbegin().operator*();
			Addr openAddrMax = (posMax + 1 <= addrdq.size() - 1) ? addrdq[posMax + 1] : addrnull;
			return OpenAddrCDT(cdt, openAddrMin, openAddrMax);
		}
		return OpenAddrCDT(codeTablenull, addrnull, addrnull);
	}


	/**
	 * @brief 此函数用于将语法树中不被访问到的子树删除。
	 * 删除的原则是：如果一行代码的四元式结果参数位在语法树中不被访问到，那么就删除此行代码
	 * @param cdt
	 */
	void CodeTable::deleteInaccessibleSubtree() {
#define debug_deleteInaccessibleSubtree true

#if debug && _WIN32 && debug_deleteInaccessibleSubtree
		{
			static int ignoreCount = 0;
			stringstream ss;
			ss << bhline;
			ss << __FUNCTION__ << "::cdt this: in" << endl;
			ss << this->toStrg();
			ss << bhline;

			// Assuming info is a std::string, convert to std::wstring
			std::string info = ss.str(); // Replace with actual info
			info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
			std::cout << info << std::endl;
			std::wstring winfo(info.begin(), info.end());
			OutputDebugString(winfo.c_str());
			ignoreCount++;
		}
#endif

		CodeTable& cdt = *this;
		set < Arg, ArgComparatorLT > argUseful(ArgComparatorLT(acm::asc | acm::name));
		set <Addr> addrErase;
		auto it = cdt.codedq.rbegin();
		Intg position = cdt.codedq.size() - 1;
		argUseful.insert(it->operator [](0));
		argUseful.insert(it->operator [](1));
		argUseful.insert(it->operator [](3));
		++it;
		position--;

		while (it != cdt.codedq.rend() && position >= 0) { //此处判断需考虑position，在cdt被修改后可能无法匹配rend
			if (argUseful.count(it->operator [](3))) {
				argUseful.insert(it->operator [](0));
				argUseful.insert(it->operator [](1));
				argUseful.insert(it->operator [](3));
			}
			else {
				addrErase.insert(cdt.addrdq[position]);
			}
			position--;
			++it;
		}

#if debug && _WIN32 && debug_deleteInaccessibleSubtree
		{
			static int ignoreCount = 0;
			stringstream ss;
			ss << bhline;
			ss << __FUNCTION__ << "::addrerase:" << endl;
			ss << COOLANG::toStrg(addrErase);
			ss << bhline;

			// Assuming info is a std::string, convert to std::wstring
			std::string info = ss.str(); // Replace with actual info
			info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
			std::cout << info << std::endl;
			std::wstring winfo(info.begin(), info.end());
			OutputDebugString(winfo.c_str());
			ignoreCount++;
		}
#endif

		for (auto& aderase : addrErase) {
			cdt.eraseCode(aderase, 0);
		}

#if debug && _WIN32 && debug_deleteInaccessibleSubtree
		{
			static int ignoreCount = 0;
			stringstream ss;
			ss << bhline;
			ss << __FUNCTION__ << "::cdt this: out" << endl;
			ss << this->toStrg();
			ss << bhline;

			// Assuming info is a std::string, convert to std::wstring
			std::string info = ss.str(); // Replace with actual info
			info += (std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n");
			std::cout << info << std::endl;
			std::wstring winfo(info.begin(), info.end());
			OutputDebugString(winfo.c_str());
			ignoreCount++;
		}
#endif
		return;
	}

	void CodeTable::unfoldRingStruct() {
		CodeTable& cdt = *this;
#define debug_unfoldRingStruct false


#if debug && _WIN32 && debug_unfoldRingStruct
		{
			stringstream ss;
			ss << bhline;
			ss << __FUNCTION__ << ":cdt this: in:\n";
			ss << this->toStrg();
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

		//todo:: logic here
		if (cdt.addrdq.size() == 0) {
			return;
		}

		auto currentAddrIt = cdt.addrdq.begin();
		Addr* currentCodeAddr = nullptr;
		auto currentCodeIt = cdt.codedq.begin();
		Code* currentCode = nullptr;


		//find ring root
		acm mode = acm::asc | acm::name;
		/*
		 * 查找形成环状结构的节点（即出现一个地址被多次访问的情况）。
		 * 这种情况是因为rule function出现了化繁步骤导致地址复制。
		 * 这一步需要找到重复的地址(S_Dz)及其位置。
		 *
		 * 需要说明的是，一个表达式的第三个参数，即结果参数，
		 * 在保证地址结果地址不重复的情况下不对其进行重复性检测，因为其只要被调用必是重复的
		 */
		set<Arg, ArgComparatorLT> argAlreadyExist(ArgComparatorLT(acm::asc | acm::name));

		while (currentAddrIt != cdt.addrdq.end()) {

#if debug && _WIN32 && debug_unfoldRingStruct
			{
				stringstream ss;
				ss << bhline;
				ss << __FUNCTION__ << ":cdt this: step 1:\n";
				ss << this->toStrg();
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
			currentCodeAddr = &*currentAddrIt;
			currentCode = &*currentCodeIt;
			++currentAddrIt;
			++currentCodeIt;
			Arg&& arg0 = (*currentCode)[0];
			Arg&& arg1 = (*currentCode)[1];
			/*	Arg&& arg2 = (*currentCode)[2];
				Arg&& arg3 = (*currentCode)[3];*/

			if (arg0.argFlag == S_Dz && arg0 != argnull) {
				if (argAlreadyExist.count(arg0) <= 0) {
					argAlreadyExist.insert(arg0);
				}
				else {
					Addr branchRoot = cdt.getSameTrBranchLastArgAddrIgnoreProperty(
						*currentCodeAddr, arg0, mode);

					auto&& openaddr_treebranchRef = cdt.copyTreeBranch(branchRoot);

					CodeTable& treeBranchRef = get<0>(openaddr_treebranchRef);

					const Addr&& insertAddrMin = cdt.getAboveCodeAddr(
						*currentCodeAddr);
					Addr& insertAddrMax = *currentCodeAddr;

					CodeTable& treeBranchInsert = treeBranchRef;
					map<FKA, FKA>&& onm = treeBranchInsert.resetFKARange(
						insertAddrMin, insertAddrMax, treeBranchInsert.fileKey);
					currentCode->assemblyFormula.quaternion.arg1_fka =
						onm[arg0.arg_fka];
					cdt.insertCodeTable(*currentCodeAddr, treeBranchInsert);
				}
			}
#if debug && _WIN32 && debug_unfoldRingStruct
			{
				stringstream ss;
				ss << bhline;
				ss << __FUNCTION__ << ":cdt this: step 2:\n";
				ss << this->toStrg();
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
			if (arg1.argFlag == S_Dz && arg1 != argnull) {
				if (argAlreadyExist.count(arg1) <= 0) {
					argAlreadyExist.insert(arg1);
				}
				else {
					Addr branchRoot = cdt.getSameTrBranchLastArgAddrIgnoreProperty(
						*currentCodeAddr, arg1, mode);
					auto&& openaddr_treebranchRef = cdt.copyTreeBranch(branchRoot);

					CodeTable& treeBranchRef = get<0>(openaddr_treebranchRef);

					const Addr&& insertAddrMin = cdt.getAboveCodeAddr(
						*currentCodeAddr);
					Addr& insertAddrMax = *currentCodeAddr;

					CodeTable& treeBranchInsert = treeBranchRef;
					map<FKA, FKA>&& onm = treeBranchInsert.resetFKARange(
						insertAddrMin, insertAddrMax, treeBranchInsert.fileKey);
					currentCode->assemblyFormula.quaternion.arg2_fka =
						onm[arg1.arg_fka];
					cdt.insertCodeTable(*currentCodeAddr, treeBranchInsert);

				}
			}

#if debug && _WIN32 && debug_unfoldRingStruct
			{
				stringstream ss;
				ss << bhline;
				ss << __FUNCTION__ << ":cdt this: step 3:\n";
				ss << this->toStrg();
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
			//			if (arg2.argFlag == S_Dz && arg2 != argnull) {
			//				if (argAlreadyExist.count(arg2) <= 0) {
			//					argAlreadyExist.insert(arg2);
			//				}
			//				else {
			//					Addr branchRoot = cdt.getSameTrBranchLastArgAddrIgnoreProperty(
			//						*currentCodeAddr, arg2);
			//					auto&& openaddr_treebranchRef = cdt.copyTreeBranch(branchRoot);
			//
			//					CodeTable& treeBranchRef = get<0>(openaddr_treebranchRef);
			//
			//					const Addr&& insertAddrMin = cdt.getAboveCodeAddr(
			//						*currentCodeAddr);
			//					Addr& insertAddrMax = *currentCodeAddr;
			//
			//					CodeTable& treeBranchInsert = treeBranchRef;
			//					map<FKA, FKA>&& onm = treeBranchInsert.resetFKARange(
			//						insertAddrMin, insertAddrMax, treeBranchInsert.fileKey);
			//					currentCode->assemblyFormula.quaternion.operator_fka =
			//						onm[arg2.arg_fka];
			//					cdt.insertCodeTable(*currentCodeAddr, treeBranchInsert);
			//
			//				}
			//			}
			//#if debug
			//			{
			//				static int ignorecount = 0;
			//				cout << "unfoldRingStruct(CodeTable&)" << " 8" << "\tignorecount:["
			//					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
			//					<< ":0" << ")" << endl;
			//			}
			//#endif

		}

		//replace ring root var
		//copy tree branch
		//replace tree branch var
		//insert tree branch

	}
	/// <summary>
	/// compare whether two codetables are the same by filekey, addrs, code flagbits(type, execute, scope, tffka, fhf), arg1,2,op,res (acm::name|acm::asc|acm::changeable)
	/// </summary>
	/// <param name="other_"></param>
	/// <returns></returns>
	bool CodeTable::operator==(const CodeTable& other_) const {
		if (this->fileKey == other_.fileKey && this->addrdq == other_.addrdq) {

			for (auto i = 0; i < other_.size(); ++i) {
				auto& cd1 = this->codedq[i];
				auto& cd2 = other_.codedq[i];
				if (cd1.scopeFKA == cd2.scopeFKA && cd1.assemblyFormula.flagBit.execute == cd2.assemblyFormula.flagBit.execute && cd1.assemblyFormula.flagBit.functionHandleFlag == cd2.assemblyFormula.flagBit.functionHandleFlag && cd1.assemblyFormula.flagBit.tfFKA == cd2.assemblyFormula.flagBit.tfFKA && cd1.assemblyFormula.flagBit.type == cd2.assemblyFormula.flagBit.type) {
					for (size_t i = 0; i < 4; i++)
					{
						if (Arg::intersect(cd1[i], cd2[i], acm::name | acm::asc | acm::changeable) == false) {
							return false;
						}
					}
				}
				else {
					return false;
				}


			}

			return true;
		}
		else {
			return false;
		}

	}


	/**
	 * @brief 将codeTable中出现的所有类型为Y_Dz的变量的ref_ar设置为ar
	 * @param ar
	 */
	void CodeTable::setRefArgAR(DataTablePtr ar) {

		for (Code& cd : codedq) {

			if (cd.assemblyFormula.flagBit.arg1_flag == Y_Dz) {
				cd.assemblyFormula.quaternion.ref1_ar = ar;
			}
			if (cd.assemblyFormula.flagBit.arg2_flag == Y_Dz) {
				cd.assemblyFormula.quaternion.ref2_ar = ar;
			}
			if (cd.assemblyFormula.flagBit.operator_flag == Y_Dz) {
				cd.assemblyFormula.quaternion.refoperator_ar = ar;
			}
			if (cd.assemblyFormula.flagBit.result_flag == Y_Dz) {
				cd.assemblyFormula.quaternion.refresult_ar = ar;
			}

		}

	}
	/// <summary>
	/// (this function only suitable for tree/expression)
	/// </summary>
	void CodeTable::checkChangeable() {
		auto setChangeableBit = [](Code& code, Intg pos, Intg bitvalue) {
			code.assemblyFormula.flagBit.changeable[pos] = bitvalue;
			return;
			};
		set<Arg, ArgComparatorLT > changeableArgs(ArgComparatorLT(acm::name | acm::asc)); //T_
		set<Arg, ArgComparatorLT > unChangeableArgs(ArgComparatorLT(acm::name | acm::asc)); // F_
		set<Arg, ArgComparatorLT > adaptiveChangeableArgs(ArgComparatorLT(acm::name | acm::asc)); // A_

		auto getChangeability = [&changeableArgs, &unChangeableArgs, &adaptiveChangeableArgs](const Arg& arg) ->Intg {
			if (changeableArgs.count(arg) > 0) {
				return true;
			}
			else if (unChangeableArgs.count(arg) > 0) {
				return false;
			}
			else if (adaptiveChangeableArgs.count(arg) > 0) {
				return A_;
			}
			else {
				return arg.changeable;
			}
			};
		//return true: need to recheck the changeablility from the beginning because one adaptive changeable arg has been determined to be changeable or unchangeable. 
		//return false: no need to recheck.
		auto noConflictInsert = [&changeableArgs, &unChangeableArgs, &adaptiveChangeableArgs](const Arg& arg, Intg changeable) ->bool {
			if (arg == argnull || (arg.argFlag == S_Dz && arg.arg_fka.addr == addrnull) || arg.isVar() == false) {
				return false;
			}
			if (changeable == true) {
				changeableArgs.insert(arg);
				if (unChangeableArgs.count(arg) > 0) {
					unChangeableArgs.erase(arg);
					return true;
					//throw exception(Strg("Conflict args exist! " + arg.toBrief() + " has been set to be unchangeable!").c_str());
				}
				if (adaptiveChangeableArgs.count(arg) > 0) {
					adaptiveChangeableArgs.erase(arg);
					return true;
				}
			}
			else if (changeable == false) {
				unChangeableArgs.insert(arg);
				if (changeableArgs.count(arg) > 0) {
					changeableArgs.erase(arg);
					return true;
					//throw exception(Strg("Conflict args exist! " + arg.toBrief() + " has been set to be changeable!").c_str());
				}
				if (adaptiveChangeableArgs.count(arg) > 0) {
					adaptiveChangeableArgs.erase(arg);
					return true;
				}
			}
			else if (changeable == A_) {
				/*if (unChangeableArgs.count(arg) == 0 && changeableArgs.count(arg) == 0) {
					adaptiveChangeableArgs.insert(arg);
				}*/
				adaptiveChangeableArgs.insert(arg);
				if (changeableArgs.count(arg) > 0) {
					changeableArgs.erase(arg);
					return true;
				}
				if (unChangeableArgs.count(arg) > 0) {
					unChangeableArgs.erase(arg);
					return true;
					//throw exception(Strg("Conflict args exist! " + arg.toBrief() + " has been set to be changeable!").c_str());
				}
			}
			return false;
			};



		bool recheck = false;
		do {
			recheck = false;
			for (auto& cd : this->codedq) {
				Arg&& arg1 = cd[0];
				Arg&& arg2 = cd[1];
				Arg&& argop = cd[2];
				Arg&& argres = cd[3];

				if (constantChangeableOps.count(argop.arg_s) > 0) {
					recheck += noConflictInsert(argres, true);
				}
				else if (constantUnchangeableOps.count(argop.arg_s) > 0) {
					recheck += noConflictInsert(argres, false);
				}
				else if (constantAdaptiveChangeableOps.count(argop.arg_s) > 0) {
					recheck += noConflictInsert(argres, A_);
				}
				arg1.changeable = getChangeability(arg1);
				arg2.changeable = getChangeability(arg2);
				argres.changeable = getChangeability(argres);
				Arg::infer_changeable(&arg1, &arg2, argop, &argres);

				recheck += noConflictInsert(arg1, arg1.changeable);
				recheck += noConflictInsert(arg2, arg2.changeable);
				recheck += noConflictInsert(argres, argres.changeable);

				setChangeableBit(cd, 0, getChangeability(arg1));
				setChangeableBit(cd, 1, getChangeability(arg2));
				setChangeableBit(cd, 3, getChangeability(argres));

			}
		} while (recheck);
		return;
	}
	/// <summary>
	/// check and modify formal flagbit based on "out" prefix (this function only suitable for tree/expression)
	/// </summary>
	void CodeTable::checkReal() {
		auto setFormalBit = [](Code& code, Intg pos, Intg bitvalue) {
			code.assemblyFormula.flagBit.formalArgFlag[pos] = bitvalue;
			return;
			};
		set<Arg, ArgComparatorLT > realArgs(ArgComparatorLT(static_cast<acm>(acm::name))); //T_

		auto getFormalBit = [&realArgs](const Arg& arg) ->Intg {
			if (realArgs.count(arg) > 0) {
				return false;
			}
			else {
				return arg.formalArg;
			}

			};
		//return true: need to recheck the changeablility from the beginning because one adaptive changeable arg has been determined to be changeable or unchangeable. 
		//return false: no need to recheck.
		auto noConflictInsert = [&realArgs](const Arg& arg) ->bool {
			if (arg == argnull || arg.isVar() == false) {
				return false;
			}
			if (realArgs.count(arg) == 0) {
				realArgs.insert(arg);
				return true;
			}
			return false;
			};


		bool recheck = false;
		do {
			recheck = false;
			for (auto& cd : this->codedq) {
				Arg&& arg1 = cd[0];
				Arg&& arg2 = cd[1];
				Arg&& argop = cd[2];
				Arg&& argres = cd[3];

				arg1.formalArg = getFormalBit(arg1);
				arg2.formalArg = getFormalBit(arg2);
				argres.formalArg = getFormalBit(argres);
				if (arg2.isBs() && arg2.arg_s == "out" && argop.isBs() && argop.arg_s == ":") {
					recheck = noConflictInsert(arg1);
				}

				setFormalBit(cd, 0, getFormalBit(arg1));
				setFormalBit(cd, 1, getFormalBit(arg2));
				setFormalBit(cd, 3, getFormalBit(argres));

			}
		} while (recheck);
		return;
	}
	/// <summary>
	/// check and modify compatible flagbit based on "out" prefix (this function only suitable for tree/expression)
	/// </summary>
	void CodeTable::checkCompatible() {
		auto setCompatibleBit = [](Code& code, Intg pos, Intg bitvalue) {
			code.assemblyFormula.flagBit.compatible[pos] = bitvalue;
			return;
			};
		set<Arg, ArgComparatorLT > compatibleArgs(ArgComparatorLT(static_cast<acm>(acm::name))); //T_

		auto getCompatibility = [&compatibleArgs](const Arg& arg) ->Intg {
			if (compatibleArgs.count(arg) > 0) {
				return true;
			}
			else {
				return arg.compatible;
			}

			};
		//return true: need to recheck the changeablility from the beginning because one adaptive changeable arg has been determined to be changeable or unchangeable. 
		//return false: no need to recheck.
		auto noConflictInsert = [&compatibleArgs](const Arg& arg) ->bool {
			if (arg == argnull || arg.isVar() == false) {
				return false;
			}
			if (compatibleArgs.count(arg) == 0) {
				compatibleArgs.insert(arg);
				return true;
			}
			return false;
			};


		bool recheck = false;
		do {
			recheck = false;
			for (auto& cd : this->codedq) {
				Arg&& arg1 = cd[0];
				Arg&& arg2 = cd[1];
				Arg&& argop = cd[2];
				Arg&& argres = cd[3];

				if (argop.isBs() && argop.arg_s == "?") {
					recheck = noConflictInsert(arg1);
				}
				arg1.compatible = getCompatibility(arg1);
				arg2.compatible = getCompatibility(arg2);
				argres.compatible = getCompatibility(argres);

				setCompatibleBit(cd, 0, getCompatibility(arg1));
				setCompatibleBit(cd, 1, getCompatibility(arg2));
				setCompatibleBit(cd, 3, getCompatibility(argres));

			}
		} while (recheck);
		return;
	}
	/// <summary>
	/// check and update constraints of the arg.(this function only suitable for tree/expression)
	/// todo: update this function to make type constraints reasoning better support run time reasoning and class inheriting.
	/// </summary>
	set<Addr> CodeTable::checkConstraints(bool allow_immediate_var) {
		auto setConstraints = [](Code& code, Intg pos, set<Constraint> constraints) {


			if (pos == 0)
			{
				code.assemblyFormula.flagBit.arg1_constraints = constraints;
			}
			else if (pos == 1)
			{
				code.assemblyFormula.flagBit.arg2_constraints = constraints;
			}
			else if (pos == 3)
			{
				code.assemblyFormula.flagBit.result_constraints = constraints;
			}
			return;
			};
		map<Arg, set<Constraint>, ArgComparatorLT > argConstraints(ArgComparatorLT(static_cast<acm>(acm::name | acm::asc))); //T_

		auto getConstraints = [&argConstraints](const Arg& arg) ->set<Constraint> {
			if (argConstraints.count(arg) > 0) {
				return argConstraints[arg];
			}
			else {
				auto argtmp = arg;
				argtmp.expandConstraints();
				return argtmp.constraints;
			}

			};
		//return true: need to recheck the changeablility from the beginning because one adaptive changeable arg has been determined to be changeable or unchangeable. 
		//return false: no need to recheck.
		auto noConflictInsert = [&argConstraints, allow_immediate_var](const Arg& arg_, set<Constraint> constraints) ->bool {
			auto arg = arg_;
			arg.expandConstraints();
			if (arg == argnull || arg.isVar() == false) {
				return false;
			}
			if (argConstraints.count(arg) == 0) {
				argConstraints[arg] = arg.constraints;
				return true;
			}
			else {

				set<Constraint>& c1 = argConstraints[arg];
				const set<Constraint> c2 = constraints;
				if (c1 == c2) {
					return false;
				}
				for (auto& c : c2) {
					if (arg.isVar() && c.instruction == "immediate" && allow_immediate_var == F_) {
						//while scanning, var names can be modified by "immediate"
						//while grounding, var names in newly formed codetables cannot be modified by "immediate"

						continue;
					}
					if (exclusiveConstraints.count(c.instruction) > 0)
					{
						for (auto& excluc : exclusiveConstraints[c.instruction]) {
							c1.erase(Constraint(excluc));
							//todo:: if some derived constraints are removed here, the constraints implying them need to be thought over.
						}

					}
					c1.insert(c);
				}

				return true;

			}
			return false;
			};


		//bool recheck = false;//never recheck
		//do {
			//recheck = false;
		Intg pos = 0;
		set<Addr> addrs;
		for (auto& cd : this->codedq) {
			Arg&& arg1 = cd[0];
			Arg&& arg2 = cd[1];
			Arg&& argop = cd[2];
			Arg&& argres = cd[3];
			arg1.expandConstraints();
			arg2.expandConstraints();
			argres.expandConstraints();
			noConflictInsert(arg1, arg1.constraints);
			noConflictInsert(arg2, arg2.constraints);
			noConflictInsert(argres, argres.constraints);

			arg1.constraints = getConstraints(arg1);
			arg2.constraints = getConstraints(arg2);
			argres.constraints = getConstraints(argres);
			if (argop.arg_s == ":" && ((arg2.isBs() && nonConstraintRelatedRhsNames.count(arg2.arg_s) == 0) || arg2.isDz())) {

				auto&& c = this->extractConstraint(this->addrdq[pos]);
				addrs.insert(c.second.begin(), c.second.end());
				if (c.first.instruction != "") {
					noConflictInsert(arg1, { c.first });
					arg1.constraints = getConstraints(arg1);
					arg1.expandConstraints();
					noConflictInsert(arg1, arg1.constraints);

					argres.constraints = getConstraints(argres);
					argres.expandConstraints();
					noConflictInsert(argres, argres.constraints);


				}

			}

			if (lhsConstraintInheritingOps.count(argop.arg_s)) {
				set<Constraint> cs;
				for (auto& c : arg1.constraints) {
					if (nonInheritableConstraints.count(c.instruction) == 0) {
						cs.insert(c);
					}
				}
				noConflictInsert(argres, cs);
				argres.constraints = getConstraints(argres);

			}
			else if (lhsConstraintCloningOps.count(argop.arg_s)) {
				argConstraints[argres].clear();
				noConflictInsert(argres, arg1.constraints);
				argres.constraints = getConstraints(argres);
			}
			else if (rhsConstraintCloningOps.count(argop.arg_s)) {
				argConstraints[argres].clear();
				noConflictInsert(argres, arg2.constraints);
				argres.constraints = getConstraints(argres);
			}
			else if (constantNumberOps.count(argop.arg_s)) {
				noConflictInsert(argres, { Constraint("number") });
				argres.constraints = getConstraints(argres);
			}
			else if (constantStringOps.count(argop.arg_s)) {
				noConflictInsert(argres, { Constraint("string") });
				argres.constraints = getConstraints(argres);
			}

			argres.expandConstraints();
			noConflictInsert(argres, argres.constraints);

			/*arg1.compatible = getCompatibility(arg1);
			arg2.compatible = getCompatibility(arg2);
			argres.compatible = getCompatibility(argres);
			if (argop.isBs() && argop.arg_s == "?") {
				recheck = noConflictInsert(arg1);
			}*/

			setConstraints(cd, 0, arg1.constraints);
			setConstraints(cd, 1, arg2.constraints);
			setConstraints(cd, 3, argres.constraints);

			pos++;
		}
		//} while (recheck);
		return addrs;
	}
	/// <summary>
	/// check and update filekey in the codetable to make them align with the codetable.filekey or the back line code's filekey. No changing on the asc of the S_Bs.
	/// (this function only suitable for tree/expression)
	/// </summary>
	void CodeTable::checkFileKey(const Strg& filekey_) {
		Strg filekey = filekey_;
		if (filekey == "") {
			filekey = this->fileKey;
			if (filekey == "" && this->codedq.size() > 0) {
				filekey = this->codedq.back().fileKey;
				if (filekey == "" && this->codedq.back().scopeFKA.fileKey != "") {
					filekey = this->codedq.back().scopeFKA.fileKey;
				}
			}
		}
		this->fileKey = filekey;
		if (filekey == "" && this->size() > 0) {
			throw exception("CodeTable without filekey detected!");
		}
		for (Code& cd : this->codedq) {

			cd.fileKey = filekey;
			if (cd.scopeFKA.fileKey != FILEKEYPRIMARY) {
				cd.scopeFKA.fileKey = filekey;
			}

			if (cd.assemblyFormula.flagBit.arg1_flag == S_Dz) {
				cd.assemblyFormula.quaternion.arg1_fka.fileKey = filekey;
			}
			if (cd.assemblyFormula.flagBit.arg2_flag == S_Dz) {
				cd.assemblyFormula.quaternion.arg2_fka.fileKey = filekey;
			}
			if (cd.assemblyFormula.flagBit.result_flag == S_Dz) {
				cd.assemblyFormula.quaternion.result_fka.fileKey = filekey;
			}
		}


	}
	/// <summary>
	/// this function can only be invoked before set bit asc! that is, in scanning process
	/// </summary>
	/// <param name="constraintAddr"></param>
	/// <returns>
	/// pair.first the extracted constraint
	/// pair.second the set of addrs that related to this constraint.
	/// </returns>
	pair<Constraint, set<Addr> > CodeTable::extractConstraint(const Addr& constraintAddr) {

		Constraint c;
		set<Addr> addrs;
		addrs.insert(constraintAddr);

		Code& base = (*this)[constraintAddr];

		acm mode = acm::name;

		Arg&& arg = base[0];
		Arg&& attr = base[1];
		if (attr.argFlag == S_Bs) {
			c.instruction = attr.arg_s;
		}
		else if (attr.argFlag == S_Dz || attr.argFlag == X_Dz) {
			Addr&& constraddr = (*this).getSameTrBranchLastArgAddr(constraintAddr, attr, mode);
			assert(constraddr != addrnull);
			addrs.insert(constraddr);
			Code& cd = (*this)[constraddr];
			/*if (cd.assemblyFormula.flagBit.type == COOL_M) {
				cd.assemblyFormula.flagBit.execute = Du;
			}*/
			Arg&& arg1 = cd[0];
			Arg&& arg2 = cd[1];
			Arg&& argop = cd[2];
			Arg&& argres = cd[3];
			if (argop.argFlag == M_Bs && argop.arg_s == "attr_call") {
				if (arg1.isBs()) {
					c.instruction = arg1.arg_s;
					if (arg2.isDz() == false) {

						c.args.push_front(arg2.toConstraintArg());

					}
					else {
						Addr argaddr = (*this).getSameTrBranchLastArgAddr(constraintAddr, attr, mode);
						assert(argaddr != addrnull);
						addrs.insert(argaddr);
						Code* cd_ = &(*this)[argaddr];
						/*	if (cd_->assemblyFormula.flagBit.type == COOL_M) {
								cd_->assemblyFormula.flagBit.execute = Du;
							}*/
						arg1 = (*cd_)[0];
						arg2 = (*cd_)[1];
						argop = (*cd_)[2];
						argres = (*cd_)[3];
						while (arg1.isDz() == true) {

							/*if (cd_->assemblyFormula.flagBit.type == COOL_M) {
								cd_->assemblyFormula.flagBit.execute = Du;
							}*/
							arg1 = (*cd_)[0];
							arg2 = (*cd_)[1];
							argop = (*cd_)[2];
							argres = (*cd_)[3];

							if ((argop.isBs()) && argop.arg_s == "COMMA") {
								if (arg2.isDz() == false) {

									c.args.push_front(arg2.toConstraintArg());

								}
								else {

#if debug && _WIN32
									{
										static int ignoreCount = 0;
										// Assuming info is a std::string, convert to std::wstring
										std::string info = "Unrecognized Constraint, throw(-1)"; // Replace with actual info
										info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
										std::wstring winfo(info.begin(), info.end());
#if WIN_DEBUG_OUTPUT
										OutputDebugString(winfo.c_str());
#endif
										cout << info << endl;
										ignoreCount++;
									}
#endif
									cout << "Unrecognized Constraint with expression as var, throw(-1)" << endl;
									throw(-1);

								}

								if (arg1.isDz() == false) {

									c.args.push_front(arg1.toConstraintArg());
									break;
								}



							}
							else {
								//todo::implement complex arg structures

							}
							argaddr = (*this).getSameTrBranchLastArgAddr(constraintAddr, arg1, mode);
							addrs.insert(argaddr);
							if (argaddr == addrnull) {
								break;
							}
							cd_ = &(*this)[argaddr];

						}

					}
				}
			}


		}
		else if (attr.argFlag == X_Bs) {
			c.instruction = attr.arg_s;

		}
		else {
			throw exception("invalid attr");
		}
		addrs.erase(addrnull);
		return make_pair(c, addrs);
	}

	OpenAddrCDT CodeTable::copyTreeBranch(const Addr& ad, acm mode) const {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "CodeTable::copyTreeBranch(const Addr&)" << " Addr:["
				<< ad.toStrg() << "]" << "\tignorecount:[" << ignorecount++
				<< "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")"
				<< std::endl;
		}
#endif

		Intg pos = getAddrPosition(addrdq, ad);
		if (pos < 0 || ad == addrnull) {
			return OpenAddrCDT(codeTablenull, addrnull, addrnull);
		}
		else if (codedq[pos].assemblyFormula.flagBit.type == COOL_R && codedq[pos].assemblyFormula.flagBit.execute != Du) {
			Arg ans = codedq[pos][0];
			pos--;
			while (pos > 0 && codedq[pos].assemblyFormula.flagBit.execute == Du) {
				pos--;
				if (pos == 0 && codedq[pos].assemblyFormula.flagBit.execute == Du) {
					return OpenAddrCDT(codeTablenull, addrnull, addrnull);
				}
			}
			if (!(ans != argnull && pos >= 0 && codedq[pos][3] == ans)) {

				return OpenAddrCDT(codeTablenull, addrnull, addrnull);
			}

		}


		if ((codedq[pos].assemblyFormula.flagBit.type == COOL_M)
			&& codedq[pos].assemblyFormula.flagBit.execute != Du) {

			CodeTable cdt = CodeTable();
			cdt.fileKey = this->fileKey;

			set<Addr> treeNode;
			set<Addr> cdtNode;
			treeNode.insert(this->addrdq[pos]);
			while (treeNode.size() > 0) {
				auto it = treeNode.begin();
				Addr currentCodeAddr = *it;
				Code& currentCode = (*const_cast<CodeTable*>(this))[currentCodeAddr];
				treeNode.erase(it);
				cdtNode.insert(currentCodeAddr);
				//            cdt.addrdq.push_front(currentCodeAddr);
				//            cdt.codedq.push_front(currentCode);
				Arg&& arg0 = (currentCode)[0];
				Arg&& arg1 = (currentCode)[1];
				Arg&& arg2 = (currentCode)[2];
				Arg&& arg3 = (currentCode)[3];
				if (arg0.argFlag == S_Dz) {
					pair<Addr, Intg>&& addr_intg_leaf =
						(*const_cast<CodeTable*>(this)).getSameTrBranchLastArgPositionIgnoreProperty(
							currentCodeAddr, arg0, mode);
					if (addr_intg_leaf.first != addrnull) {
						treeNode.insert(addr_intg_leaf.first);
					}

				}
				if (arg1.argFlag == S_Dz) {
					pair<Addr, Intg>&& addr_intg_leaf =
						(*const_cast<CodeTable*>(this)).getSameTrBranchLastArgPositionIgnoreProperty(
							currentCodeAddr, arg1, mode);
					if (addr_intg_leaf.first != addrnull) {
						treeNode.insert(addr_intg_leaf.first);
					}

				}
				if (arg2.argFlag == S_Dz) {
					pair<Addr, Intg>&& addr_intg_leaf =
						(*const_cast<CodeTable*>(this)).getSameTrBranchLastArgPositionIgnoreProperty(
							currentCodeAddr, arg2, mode);
					if (addr_intg_leaf.first != addrnull) {
						treeNode.insert(addr_intg_leaf.first);
					}

				}
				if (arg3.argFlag == S_Dz) {
					pair<Addr, Intg>&& addr_intg_leaf =
						(*const_cast<CodeTable*>(this)).getSameTrBranchLastArgPositionIgnoreProperty(
							currentCodeAddr, arg3, mode);
					if (addr_intg_leaf.first != addrnull) {
						treeNode.insert(addr_intg_leaf.first);
					}

				}

			}
			for (auto nodead : cdtNode) {
				cdt.addrdq.push_back(nodead);
				cdt.codedq.push_back((*const_cast<CodeTable*>(this))[nodead]);
			}
			Intg posMin = getAddrPosition(this->addrdq, cdt.addrdq.front());
			Intg posMax = getAddrPosition(this->addrdq, cdt.addrdq.back());
			Addr openAddrMin = (posMin - 1 >= 0) ? addrdq[posMin - 1] : addrnull;
			Addr openAddrMax = (posMax + 1 <= addrdq.size() - 1) ? addrdq[posMax + 1] : addrnull;
			return OpenAddrCDT(cdt, openAddrMin, openAddrMax);
		}
		return OpenAddrCDT(codeTablenull, addrnull, addrnull);
	}

	/**
	 * copy[adLow,admax]
	 * @param adLow
	 * @param adHigh
	 * @return
	 */
	OpenAddrCDT CodeTable::copyCodeTable(const Addr& closeAdLow, const Addr& closeAdHigh) const {

		Intg posMin = getAddrPosition(addrdq, closeAdLow);
		Intg posMax = getAddrPosition(addrdq, closeAdHigh);
		Addr openAddrMin = (posMin - 1 >= 0) ? addrdq[posMin - 1] : addrnull;
		Addr openAddrMax = (posMax + 1 <= addrdq.size() - 1) ? addrdq[posMax + 1] : addrnull;
		CodeTable cdt = CodeTable();
		cdt.fileKey = this->fileKey;
		cdt.cdth = this->cdth;
		while (posMin <= posMax) {
			cdt.addrdq.push_back(const_cast<CodeTable*>(this)->addrdq[posMin]);
			cdt.codedq.push_back(const_cast<CodeTable*>(this)->codedq[posMin]);
			posMin++;

		}
		return OpenAddrCDT(cdt, openAddrMin, openAddrMax);
	}
	/**
	 * copy[adLow,admax]
	 * @param adLow
	 * @param adUp
	 * @return
	 */
	OpenAddrCDT CodeTable::copyCodeTableIgnoreFILLIN(const Addr& adLow,
		const Addr& adUp) const {

		Intg posMin = getAddrPosition(addrdq, adLow);
		Intg posMax = getAddrPosition(addrdq, adUp);

		Addr OpenAddrLow = (posMin - 1 >= 0) ? addrdq[posMin - 1] : addrnull;
		Addr OpenAddrHigh = (posMax + 1 <= addrdq.size() - 1) ? addrdq[posMax + 1] : addrnull;

		CodeTable cdt = CodeTable();
		cdt.fileKey = this->fileKey;
		while (posMin <= posMax) {
			if ((this)->codedq[posMin].assemblyFormula.flagBit.type != COOL_FIL
				&& (this)->codedq[posMin].assemblyFormula.flagBit.type != COOL_LIN) {
				cdt.addrdq.push_back(const_cast<CodeTable*>(this)->addrdq[posMin]);
				cdt.codedq.push_back(const_cast<CodeTable*>(this)->codedq[posMin]);

			}
			posMin++;

		}
		return OpenAddrCDT(cdt, OpenAddrLow, OpenAddrHigh);
	}


	/**
 * @brief 将codeTableHash中出现的所有类型为Y_Dz的变量的ref_ar设置为ar
 * @param ar
 */
	void CodeTableHash::setRefArgAR(DataTablePtr ar) {

		for (auto& p : file_cdt_map) {
			p.second.setRefArgAR(ar);
		}
	}
	/**
 * @attention Branch 禁止跨cdt
 * @param ad
 * @return
 */
	OpenFKACDT CodeTableHash::copyTreeBranch(const FKA& fka, acm mode) const {
		const CodeTable& cdt = const_cast<CodeTableHash*>(this)->operator [](
			fka.fileKey);
		auto && (open_addr_cdt) = cdt.copyTreeBranch(fka.addr, mode);
		return OpenFKACDT(get<0>(open_addr_cdt), FKA(fka.fileKey, get<1>(open_addr_cdt)), FKA(fka.fileKey, get<2>(open_addr_cdt)));

	}
	/**
	 * copy[fkaLow,fkaUp]
	 * @attention 允许跨cdt，但跨cdt时抛出cdth作为异常,用户根据需要从cdth中提取cdt
	 * @param adLow
	 * @param adUp
	 * @return
	 */
	OpenFKACDT CodeTableHash::copyCodeTable(const FKA& fkaLow,
		const FKA& fkaUp) const {
		if (fkaLow.fileKey == fkaUp.fileKey) {
			auto cdt = const_cast<CodeTableHash*>(this)->operator [](fkaLow.fileKey);
			auto&& open_addr_cdt = cdt.copyCodeTable(
				fkaLow.addr, fkaUp.addr);
			return OpenFKACDT(get<0>(open_addr_cdt), FKA(fkaLow.fileKey, get<1>(open_addr_cdt)), FKA(fkaLow.fileKey, get<2>(open_addr_cdt)));
		}
		//以下逻辑抛出cdth

		auto&& itlow = this->file_cdt_map.find(fkaLow.fileKey);
		auto&& itup = this->file_cdt_map.find(fkaUp.fileKey);
		if (itlow == file_cdt_map.end()
			|| (itup == file_cdt_map.end() && fkaUp.fileKey != FILEKEYEND)) {
			return OpenFKACDT(codeTablenull, fkanull, fkanull);

		}

		CodeTableHash cdth;
		Addr adLow = fkaLow.addr;
		Addr adUp = itlow->second.addrdq.back();

		while (itlow->first != itup->first) {
			OpenAddrCDT&& openadcdt = itlow->second.copyCodeTable(adLow, adUp);
			cdth.appendCdt(get<0>(openadcdt));
			++itlow;
			adLow = itlow->second.addrdq.front();
			adUp = itlow->second.addrdq.back();
		}
		if (fkaUp.fileKey != FILEKEYEND) {
			adLow = itup->second.addrdq.front();
			adUp = fkaUp.addr;
			OpenAddrCDT&& openadcdt = itlow->second.copyCodeTable(adLow, adUp);
			cdth.appendCdt(get<0>(openadcdt));
		}
		throw cdth;
	}

	/***
 * @brief copy the max AST the node of specified addr in.
 * @attention copytree不会将Y类型代码一并拷贝，引用(Y)对应的代码段必须位于一段表达式对应代码段的前部。
 * @attention copytree不会将EXPRQS/EXPRQE与之间的代码一并拷贝，但会跳过相应区段选取相邻代码。同时选取最大的EXPRQE作为边界
 * @attention 禁止跨cdt（一棵树禁止跨cdt）
 * @param ad
 * @return
 */
	OpenFKACDT CodeTableHash::copyTree(const FKA& fka) const {
		CodeTable& cdt = const_cast<CodeTableHash*>(this)->operator [](fka.fileKey);
		auto&& open_addr_cdt = cdt.copyTree(fka.addr);
		return OpenFKACDT(get<0>(open_addr_cdt), FKA(fka.fileKey, get<1>(open_addr_cdt)), FKA(fka.fileKey, get<2>(open_addr_cdt)));
	}

	/**
	 * @brief 重置代码段的filekey以及其地址为min到max之间并修改语法树中出现的S_Dz地址参数的值以满足代码段地址范围。
	 * 具体值取决于split(min,max,n)函数的返回值
	 * @attention 此函数只适用于属于同一棵树的代码段(所有节点均连接于根节点)，此外，此函数会修改根节点的地址以使整个codetable的地址均位于（min，max）中
	 * @param min
	 * @param max
	 * @return Code中被替换的fka与原fka的对照表map<original,new>
	 */
	map<FKA, FKA> CodeTable::resetFKARange(const Addr& min, const Addr& max,
		const Strg& fileKey) {
		Strg fileKeyOrg = this->fileKey;
		this->fileKey = fileKey;
		if (this->size() == 0) {
			return map<FKA, FKA>();
		}
		//vector<Addr> addrvNew = split(min, max, size());
		typedef FKA FKAOriginal;
		typedef FKA FKANew;
		map<FKAOriginal, FKANew> onm;
		Intg currentPosition = 0;
		Intg maxPosition = size();
		Addr newlowbound = min;
		Addr highbound = max;
		while (currentPosition < maxPosition) {
			Code& currentCode = codedq[currentPosition];
			Arg&& arg1 = currentCode[0];
			Arg&& arg2 = currentCode[1];
			Arg&& argop = currentCode[2];
			Arg&& argres = currentCode[3];
			if (arg1.argFlag == S_Dz && arg1.arg_fka.addr > newlowbound && arg1.arg_fka.addr < highbound) {
				newlowbound = arg1.arg_fka.addr;
			}
			if (arg2.argFlag == S_Dz && arg2.arg_fka.addr > newlowbound && arg2.arg_fka.addr < highbound) {
				newlowbound = arg2.arg_fka.addr;
			}
			if (argres.argFlag == S_Dz && argres.arg_fka.addr > newlowbound && argres.arg_fka.addr < highbound) {
				newlowbound = argres.arg_fka.addr;
			}
			auto suggestCurrentCodeAddr = split(newlowbound, highbound, 1).back();




		L:for (auto& c : this->codedq) {
			if (suggestCurrentCodeAddr == c.assemblyFormula.quaternion.arg1_fka.addr || suggestCurrentCodeAddr == c.assemblyFormula.quaternion.arg2_fka.addr || suggestCurrentCodeAddr == c.assemblyFormula.quaternion.result_fka.addr) {
				suggestCurrentCodeAddr = split(suggestCurrentCodeAddr, highbound, 1).back();
				goto L;
			}
		}
		newlowbound = suggestCurrentCodeAddr;
		addrdq[currentPosition] = suggestCurrentCodeAddr;

		Addr& currentCodeAddr = addrdq[currentPosition];
		currentCode.replaceFileKey(fileKeyOrg, fileKey);

		if (currentCode.assemblyFormula.flagBit.arg1_flag == S_Dz
			&& currentCode.assemblyFormula.quaternion.arg1_fka.addr != addrnull) {
			FKAOriginal* fkaOriginal =
				&(currentCode.assemblyFormula.quaternion.arg1_fka);
			auto fkaNewIt = onm.find(*fkaOriginal);
			if (fkaNewIt != onm.end()) {
				*fkaOriginal = fkaNewIt->second;
			}
			else {
				throw exception("Cannot replace op1 with unknown new addr");
				onm[*fkaOriginal] = FKA(this->fileKey, currentCodeAddr);
				fkaOriginal->addr = currentCodeAddr;
			}
		}
		if (currentCode.assemblyFormula.flagBit.arg2_flag == S_Dz
			&& currentCode.assemblyFormula.quaternion.arg2_fka.addr != addrnull) {
			FKAOriginal* fkaOriginal =
				&(currentCode.assemblyFormula.quaternion.arg2_fka);
			auto fkaNewIt = onm.find(*fkaOriginal);
			if (fkaNewIt != onm.end()) {
				*fkaOriginal = fkaNewIt->second;
			}
			else {
				throw exception("Cannot replace op1 with unknown new addr");
				onm[*fkaOriginal] = FKA(this->fileKey, currentCodeAddr);
				fkaOriginal->addr = currentCodeAddr;
			}
		}
		/*if (currentCode.assemblyFormula.flagBit.operator_flag == S_Dz
			&& currentCode.assemblyFormula.quaternion.operator_fka
			!= fkanull) {
			FKAOriginal* fkaOriginal =
				&(currentCode.assemblyFormula.quaternion.operator_fka);
			auto fkaNewIt = onm.find(*fkaOriginal);
			if (fkaNewIt != onm.end()) {
				*fkaOriginal = fkaNewIt->second;
			}
			else {
				onm[*fkaOriginal] = FKA(this->fileKey, currentCodeAddr);
				fkaOriginal->addr = currentCodeAddr;
			}
		}*/
		if (currentCode.assemblyFormula.flagBit.result_flag == S_Dz
			&& currentCode.assemblyFormula.quaternion.result_fka
			!= fkanull) {
			FKAOriginal* fkaOriginal =
				&(currentCode.assemblyFormula.quaternion.result_fka);
			auto fkaNewIt = onm.find(*fkaOriginal);
			if (fkaNewIt != onm.end()) {
				*fkaOriginal = fkaNewIt->second;
			}
			else {
				onm[*fkaOriginal] = FKA(this->fileKey, currentCodeAddr);
				fkaOriginal->addr = currentCodeAddr;
			}
		}

		++currentPosition;
		}
		return onm;

	}

	/**
	 * @attention 将codeTableInsert插入到insertAddr “之前”，如果insertAddr的值为空，这将其插入最后面
	 * @param insertAddr
	 * @param codeTableInsert
	 * @attention 此步骤不会检查codeTableInsert与this的地址、fileKey是否匹配
	 */
	void CodeTable::insertCodeTable(const Addr& insertAddr,
		const CodeTable& codeTableInsert) {
		Intg posInsert = 0;
		if (this->addrdq.size() > 0 && insertAddr != addrnull
			&& insertAddr <= this->addrdq.back()) {
			posInsert = getAddrPosition(this->addrdq, insertAddr);
			if (posInsert == -1) {
				cout << "CodeTable::insertCodeTable err:: posInsert == -1, exit(-1)"
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

			posInsert = this->addrdq.size();
		}

#ifdef __linux__
		this->addrdq.insert<std::deque<Addr, allocator<Addr>>::const_iterator,
			std::deque<Addr, allocator<Addr>>::const_iterator>(
				this->addrdq.begin() + posInsert, codeTableInsert.addrdq.begin(),
				codeTableInsert.addrdq.end());
		this->codedq.insert<std::deque<Code, allocator<Code>>::const_iterator,
			std::deque<Code, allocator<Code>>::const_iterator>(
				this->codedq.begin() + posInsert, codeTableInsert.codedq.begin(),
				codeTableInsert.codedq.end());
#elif __WIN32__
		std::deque<COOLANG::Addr>::iterator addrit = this->addrdq.begin()
			+ posInsert;
		this->addrdq.insert(addrit, codeTableInsert.addrdq.begin(),
			codeTableInsert.addrdq.end());
		std::deque<COOLANG::Code>::iterator codeit = this->codedq.begin()
			+ posInsert;
		this->codedq.insert(codeit, codeTableInsert.codedq.begin(),
			codeTableInsert.codedq.end());
#endif
		return;

	}
	/**
	 * @attention 将codeTableInsert插入到insertFKA “之前”，如果insertFKA的值为空，这将其插入最后面
	 * @param insertAddr
	 * @param codeTableInsert
	 */
	void CodeTableHash::insertCodeTable(const FKA& insertFKA,
		const CodeTable& codeTableInsert) {
		CodeTable& cdt = this->operator [](insertFKA.fileKey);
		return cdt.insertCodeTable(insertFKA.addr, codeTableInsert);
	}
	/**
	 * @attention 将codeTableAppend插入到appendAddr “之后”,如果@arg{appendAddr} == addrnull、addrmax，则将其插入到最后面
	 * @param insertAddr
	 * @param codeTableInsert
	 */
	void CodeTable::appendCodeTable(const Addr& appendAddr,
		CodeTable& codeTableAppend) {
		Intg posAppend = 0;
		if (this->addrdq.size() > 0 && appendAddr != addrnull
			&& appendAddr != addrmax && appendAddr <= this->addrdq.back()) {
			posAppend = getAddrPosition(this->addrdq, appendAddr) + 1;
			if (posAppend == -1) {

				cout << "CodeTable::appendCodeTable err:: posAppend == -1, exit-1"
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

			posAppend = this->addrdq.size();
		}
		for (auto& dq_ : codeTableAppend.codedq) {
			dq_.fileKey = this->fileKey;
		}
#ifdef __linux__
		this->addrdq.insert<std::deque<Addr, allocator<Addr>>::const_iterator,
			std::deque<Addr, allocator<Addr>>::const_iterator>(
				this->addrdq.begin() + posAppend, codeTableAppend.addrdq.begin(),
				codeTableAppend.addrdq.end());
		this->codedq.insert<std::deque<Code, allocator<Code>>::const_iterator,
			std::deque<Code, allocator<Code>>::const_iterator>(
				this->codedq.begin() + posAppend, codeTableAppend.codedq.begin(),
				codeTableAppend.codedq.end());
#elif __WIN32__
		std::deque<COOLANG::Addr>::iterator addrit = this->addrdq.begin()
			+ posAppend;
		this->addrdq.insert(addrit, codeTableAppend.addrdq.begin(),
			codeTableAppend.addrdq.end());
		std::deque<COOLANG::Code>::iterator codeit = this->codedq.begin()
			+ posAppend;
		this->codedq.insert(codeit, codeTableAppend.codedq.begin(),
			codeTableAppend.codedq.end());
#endif
		return;

	}
	/**
	 * @attention 将codeTableAppend插入到appendFKA “之后”的相同Cdt后,如果@arg{appendAddr} == addrnull，则将其插入到最后面
	 * @param insertAddr
	 * @param codeTableInsert
	 */
	void CodeTableHash::appendCodeTableToCdt(const FKA& appendFKA,
		CodeTable& codeTableAppend) {
		CodeTable& cdt = this->operator [](appendFKA.fileKey);
		return cdt.appendCodeTable(appendFKA.addr, codeTableAppend);

	}
	/**
	 * @brief 删除指定间距内的所有Code，即[min+offset1,max+offset2],offset可以为负数
	 * @param eraseAddrMin
	 * @param offsetRight1
	 * @param eraseAddrMax(如果为addrmax则删除至末尾)
	 * @param offsetRight2
	 */
	void CodeTable::eraseCode(const Addr& eraseAddrMin, Intg offsetRight1,
		const Addr& eraseAddrMax, Intg offsetRight2) {
		if (eraseAddrMin == addrnull || eraseAddrMax == addrnull
			|| eraseAddrMin > eraseAddrMax) {

				{
					static int ignorecount = 0;
					cout << "CodeTable::eraseCode(const Addr&, int, const Addr&, int)"
						<< " err,exit(-1)" << "\tignorecount:[" << ignorecount++
						<< "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")"
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
		Intg positionMin = getAddrPosition(addrdq, eraseAddrMin) + offsetRight1;
		Intg positionMax = getAddrPosition(addrdq, eraseAddrMax) + offsetRight2 + 1;
		if (eraseAddrMax == addrmax) {
			positionMax = size() - 1;

		}
		addrdq.erase(addrdq.begin() + positionMin, addrdq.begin() + positionMax);
		codedq.erase(codedq.begin() + positionMin, codedq.begin() + positionMax);
	}
	/**
	 * @brief 删除指定间距内的所有Code，即[min+offset1,max+offset2],offset可以为负数
	 * @attention 允许跨cdt，不保留空的cdt
	 * @param eraseFKAMin
	 * @param offsetRight1
	 * @param eraseFKAMax
	 * @param offsetRight2
	 */
	void CodeTableHash::eraseCode(const FKA& eraseFKAMin, Intg offsetRight1,
		const FKA& eraseFKAMax, Intg offsetRight2) {
		FKA fkamin = this->getCodeFKAByOffset(eraseFKAMin, offsetRight1);
		FKA fkamax = this->getCodeFKAByOffset(eraseFKAMax, offsetRight2);

		if (fkamin.fileKey == fkamax.fileKey) {
			return this->operator [](fkamin.fileKey).eraseCode(fkamin.addr, 0,
				fkamax.addr, 0);
		}
		else {
			Strg fileKey = fkamin.fileKey;
			Strg fileKeyEnd = fkamax.fileKey;
			if (fileKeyEnd == FILEKEYEND) {
				fileKeyEnd = this->addr_file_map.rbegin()->second;

			}
			CodeTable* cdt = &(this->operator [](fileKey));
			Addr addrmin_ = fkamin.addr;
			Addr addrmax_ = cdt->addrdq.back();

			while (fileKey != fileKeyEnd) {

				cdt->eraseCode(addrmin_, 0, addrmax_, 0);
				fileKey = getNextCdtFileKey(fileKey);
				if (cdt->size() <= 0) {
					this->eraseCdt(cdt->fileKey);
					cdt = nullptr;
				}
				cdt = &(this->operator [](fileKey));
				addrmin_ = addrnull;
				addrmax_ = cdt->addrdq.back();

			}
			addrmax_ = fkamax.addr;
			return cdt->eraseCode(addrmin_, 0, addrmax_, 0);

		}

	}
	/**
	 * @attention
	 * @param codeTableAppend
	 */
	void CodeTable::appendCodeTable(CodeTable& codeTableAppend) {
#ifdef __linux__
		this->addrdq.insert<std::deque<Addr, allocator<Addr>>::const_iterator,
			std::deque<Addr, allocator<Addr>>::const_iterator>(
				this->addrdq.end(), codeTableAppend.addrdq.begin(),
				codeTableAppend.addrdq.end());
		this->codedq.insert<std::deque<Code, allocator<Code>>::const_iterator,
			std::deque<Code, allocator<Code>>::const_iterator>(
				this->codedq.end(), codeTableAppend.codedq.begin(),
				codeTableAppend.codedq.end());
#elif __WIN32__
		std::deque<COOLANG::Addr>::iterator addrit = this->addrdq.end();
		this->addrdq.insert(addrit, codeTableAppend.addrdq.begin(),
			codeTableAppend.addrdq.end());
		std::deque<COOLANG::Code>::iterator codeit = this->codedq.end();
		this->codedq.insert(codeit, codeTableAppend.codedq.begin(),
			codeTableAppend.codedq.end());
#endif
		return;
	}
	/**
	 * @brief 在末尾的cdt后面追加cdt
	 * @param codeTableAppend
	 */
	void CodeTableHash::appendCodeTableToLastCdt(CodeTable& codeTableAppend) {
		if (this->size() <= 0) {
			CodeTable cdt(codeTableAppend);
			this->appendCdt(cdt);
			return;
		}
		Strg fileKey = this->addr_file_map.rbegin()->second;
		this->operator [](fileKey).appendCodeTable(codeTableAppend);
	}

	/**
	 * @brief 获得当前地址之前的类型为type的代码的地址。
	 * @attention
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return
	 */
	Addr CodeTable::getLast(const Intg& type, const Addr& ad) const {
		Intg pos = getLastAddrPosition(addrdq, ad);
		Addr a;

		while (pos >= 0) {
			if (codedq[pos].assemblyFormula.flagBit.type
				!= type || codedq[pos].assemblyFormula.flagBit.execute == Du) {
				pos--;
			}
			else {
				a = addrdq[pos];
				return a;
			}

		}
		return a;
	}
	/**
	 * @brief 获得当前fka之前的类型为type的代码的fka。允许跨cdt（默认）
	 * @attention
	 * @param type
	 * @param ad
	 * @return
	 */
	FKA CodeTableHash::getLast(const Intg& type, const FKA& fka) const {

		Strg fileKey = fka.fileKey;
		Addr addr = fka.addr;

		for (;;) {
			if (this->file_cdt_map.count(fileKey) < 0) {
				fileKey = file_cdt_map.rbegin()->first;
			}
			CodeTable& cdt = const_cast<CodeTableHash*>(this)->operator [](fileKey);
			auto&& addr_ = cdt.getLast(type, addr);
			if (addr_ == addrnull) {
				addr = addrmax;
				fileKey = this->getLastCdtFileKey(fileKey);

			}
			else {
				return FKA(fileKey, addr_);
			}
			if (fileKey == "") {
				return fkanull;
			}

		}
		return fkanull;
	}
	/**
	 * @brief 获得当前地址之前的类型为types的代码的地址。
	 * @attention
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return
	 */
	Addr CodeTable::getLast(const vector<Intg>& types, const Addr& ad) const {
		Intg pos = getLastAddrPosition(addrdq, ad);
		if (pos < 0) {
#if debug
			cerr << "CodeTable::getLast(types) debug::get Array end,return addrnull"
				<< endl;
#endif
			return Addr();
		}
		Addr a;

		while (pos >= 0) {
			if (std::find(types.begin(), types.end(),
				codedq[pos].assemblyFormula.flagBit.type)
				== types.end() || codedq[pos].assemblyFormula.flagBit.execute == Du) {
				pos--;
			}
			else {
				a = addrdq[pos];
				return a;
			}

		}
		return a;
	}
	/**
	 * @brief 获得当前fka址之前的类型为types的代码的fka，允许跨cdt（默认）。
	 * @attention
	 * @param type
	 * @param ad
	 * @return
	 */
	FKA CodeTableHash::getLast(const vector<Intg>& types, const FKA& fka) const {
		Strg fileKey = fka.fileKey;
		Addr addr = fka.addr;

		for (;;) {
			if (this->file_cdt_map.count(fileKey) < 0) {
				fileKey = file_cdt_map.rbegin()->first;
			}
			CodeTable& cdt = const_cast<CodeTableHash*>(this)->operator [](fileKey);
			auto&& addr_ = cdt.getLast(types, addr);
			if (addr_ == addrnull) {
				addr = addrmax;
				fileKey = this->getLastCdtFileKey(fileKey);

			}
			else {
				return FKA(fileKey, addr_);
			}
			if (fileKey == "") {
				return fkanull;
			}

		}
		return fkanull;
	}
	/**
	 * @brief 获得当前地址之前的类型不为type的代码的地址。
	 * @attention
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return
	 */
	Addr CodeTable::getLastNot(const Intg& type, const Addr& ad) const {
		Intg pos = getLastAddrPosition(addrdq, ad);
		if (pos < 0) {
#if debug
			cerr << "CodeTable::getLastNot debug::get Array end,return addrnull"
				<< endl;
#endif
			return Addr();
		}
		Addr a;

		while (pos >= 0) {
			if (codedq[pos].assemblyFormula.flagBit.type
				== type || codedq[pos].assemblyFormula.flagBit.execute == Du) {
				pos--;
			}
			else {
				a = addrdq[pos];
				return a;
			}

		}
		return a;
	}
	/**
	 * @brief 获得当前地址之前的类型不为types的代码的地址。
	 * @attention
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return
	 */
	Addr CodeTable::getLastNot(const vector<Intg>& types, const Addr& ad) const {
		Intg pos = getLastAddrPosition(addrdq, ad);
		if (pos < 0) {
#if debug
			cerr
				<< "CodeTable::getLastNot(types) debug::get Array end,return addrnull"
				<< endl;
#endif
			return Addr();
		}
		Addr a;

		while (pos >= 0) {
			if (std::find(types.begin(), types.end(),
				codedq[pos].assemblyFormula.flagBit.type)
				!= types.end() || codedq[pos].assemblyFormula.flagBit.execute == Du) {
				pos--;
			}
			else {
				a = addrdq[pos];
				return a;
			}

		}
		return a;
	}
	/**
	 * @brief 获得当前fka之前的类型不为types的代码的fka，匀速跨cdt（默认）。
	 * @attention
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return
	 */
	FKA CodeTableHash::getLastNot(const vector<Intg>& types, const FKA& fka) const {
		Strg fileKey = fka.fileKey;
		Addr addr = fka.addr;

		for (;;) {
			if (this->file_cdt_map.count(fileKey) < 0) {
				fileKey = file_cdt_map.rbegin()->first;
			}
			CodeTable& cdt = const_cast<CodeTableHash*>(this)->operator [](fileKey);
			auto&& addr_ = cdt.getLastNot(types, addr);
			if (addr_ == addrnull) {
				addr = addrmax;
				fileKey = this->getLastCdtFileKey(fileKey);

			}
			else {
				return FKA(fileKey, addr_);
			}
			if (fileKey == "") {
				return fkanull;
			}

		}
		return fkanull;
	}
	/**
	 * @brief 获得当前地址之后的类型不为type的代码的地址。
	 * @attention
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return addr or addrnull
	 */
	Addr CodeTable::getNextNot(const Intg& type, const Addr& ad) const {
		Intg pos = getNextAddrPosition(addrdq, ad);
		if (pos < 0) {
#if debug
			cerr << "CodeTable::getNextNot debug::get Array end,return addrnull"
				<< endl;
#endif
			return Addr();
		}
		Addr a;

		while (pos < (Intg)addrdq.size()) {
			if (codedq[pos].assemblyFormula.flagBit.type
				== type || codedq[pos].assemblyFormula.flagBit.execute == Du) {
				pos++;
			}
			else {
				a = addrdq[pos];
				return a;
			}

		}
		return a;
	}
	/**
	 * @brief 获得当前fka之后的类型不为type的代码的fka。
	 * @attention 允许跨cdt（默认）
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return
	 */
	FKA CodeTableHash::getNextNot(const Intg& type, const FKA& fka) const {
		Strg fileKey = fka.fileKey;
		Addr addr = fka.addr;

		for (;;) {
			if (this->file_cdt_map.count(fileKey) < 0) {

				{
					static int ignorecount = 0;
					cerr << "CodeTableHash::getNextNot(const int&, const FKA&)"
						<< " fileKeyNotFound, exit(-1) " << "\tignorecount:["
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
			CodeTable& cdt = const_cast<CodeTableHash*>(this)->operator [](fileKey);
			auto&& addr_ = cdt.getNextNot(type, addr);
			if (addr_ == addrnull) {
				addr = addrnull;
				fileKey = this->getNextCdtFileKey(fileKey);

			}
			else {
				return FKA(fileKey, addr_);
			}
			if (fileKey == "") {
				return fkanull;
			}

		}
		return fkanull;
	}
	/**
	 * @brief 获得当前fka之前的类型不为type的代码的fka。
	 * @attention 允许跨cdt（默认）
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return
	 */
	FKA CodeTableHash::getLastNot(const Intg& type, const FKA& fka) const {
		Strg fileKey = fka.fileKey;
		Addr addr = fka.addr;

		for (;;) {
			if (this->file_cdt_map.count(fileKey) < 0) {

				{
					static int ignorecount = 0;
					cerr << "CodeTableHash::getNextNot(const int&, const FKA&)"
						<< " fileKeyNotFound, exit(-1) " << "\tignorecount:["
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
			CodeTable& cdt = const_cast<CodeTableHash*>(this)->operator [](fileKey);
			auto&& addr_ = cdt.getLastNot(type, addr);
			if (addr_ == addrnull) {
				addr = addrnull;
				fileKey = this->getLastCdtFileKey(fileKey);

			}
			else {
				return FKA(fileKey, addr_);
			}
			if (fileKey == "") {
				return fkanull;
			}

		}
		return fkanull;
	}
	/**
	 * @brief 获得当前地址之后的类型不为types的代码的地址。
	 * @attention
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return
	 */
	Addr CodeTable::getNextNot(const vector<Intg>& types, const Addr& ad) const {
		Intg pos = getNextAddrPosition(addrdq, ad);
		if (pos < 0) {
#if debug
			cerr
				<< "CodeTable::getNextNot(types) debug::get Array end,return addrnull"
				<< endl;
#endif
			return Addr();
		}
		Addr a;

		while (pos >= 0) {
			if (std::find(types.begin(), types.end(),
				codedq[pos].assemblyFormula.flagBit.type)
				!= types.end() || codedq[pos].assemblyFormula.flagBit.execute == Du) {
				pos++;
			}
			else {
				a = addrdq[pos];
				return a;
			}

		}
		return a;
	}
	/**
	 * @brief 获得当前fka之后的类型不为types的代码的fka。
	 * @attention 允许跨cdt（默认）
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return
	 */
	FKA CodeTableHash::getNextNot(const vector<Intg>& types, const FKA& fka) const {
		Strg fileKey = fka.fileKey;
		Addr addr = fka.addr;

		for (;;) {
			if (this->file_cdt_map.count(fileKey) < 0) {

				{
					static int ignorecount = 0;
					cerr
						<< "CodeTableHash::getNextNot(const vector<int>&, const FKA&)"
						<< " fileKeyNotFound, exit(-1) " << "\tignorecount:["
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
			CodeTable& cdt = const_cast<CodeTableHash*>(this)->operator [](fileKey);
			auto&& addr_ = cdt.getNextNot(types, addr);
			if (addr_ == addrnull) {
				addr = addrnull;
				fileKey = this->getNextCdtFileKey(fileKey);

			}
			else {
				return FKA(fileKey, addr_);
			}
			if (fileKey == "") {
				return fkanull;
			}

		}
		return fkanull;
	}
	/**
	 * @brief 获得当前地址之后的类型为type的代码的地址。
	 * @attention
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return
	 */
	Addr CodeTable::getNext(const Intg& type, const Addr& ad) const {
		Intg pos = getNextAddrPosition(addrdq, ad);
		if (pos < 0) {
#if debug
			cerr << "CodeTable::getNext debug::get Array end,return addrnull"
				<< endl;
#endif
			return Addr();
		}
		Addr a;

		while (pos < (Intg)addrdq.size()) {
			if (codedq[pos].assemblyFormula.flagBit.type
				!= type && codedq[pos].assemblyFormula.flagBit.execute != Du) {
				pos++;
			}
			else {
				a = addrdq[pos];
				return a;
			}

		}
		return a;
	}
	/**
	 * @brief 获得当前fka之后的类型为type的代码的fka。
	 * @attention 允许跨cdt（默认）
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return
	 */
	FKA CodeTableHash::getNext(const Intg& type, const FKA& fka) const {
		Strg fileKey = fka.fileKey;
		Addr addr = fka.addr;

		for (;;) {
			if (this->file_cdt_map.count(fileKey) < 0) {

				{
					static int ignorecount = 0;
					cerr << "CodeTableHash::getNext(const int&, const FKA&)"
						<< " fileKeyNotFound, exit(-1) " << "\tignorecount:["
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
			CodeTable& cdt = const_cast<CodeTableHash*>(this)->operator [](fileKey);
			auto&& addr_ = cdt.getNext(type, addr);
			if (addr_ == addrnull) {
				addr = addrnull;
				fileKey = this->getNextCdtFileKey(fileKey);

			}
			else {
				return FKA(fileKey, addr_);
			}
			if (fileKey == "") {
				return fkanull;
			}

		}
		return fkanull;
	}
	FKA CodeTableHash::getMaxFKA() const {
		if (this->size() > 0) {
			Strg& fileKey =
				const_cast<CodeTableHash*>(this)->addr_file_map.rbegin()->second;
			return FKA(fileKey,
				const_cast<CodeTableHash*>(this)->operator [](fileKey).addrdq.back());
		}
		else {
			return fkanull;
		}
	}
	FKA CodeTableHash::getMinFKA() const {
		if (this->size() > 0) {
			Strg& fileKey =
				const_cast<CodeTableHash*>(this)->addr_file_map.begin()->second;
			return FKA(fileKey,
				const_cast<CodeTableHash*>(this)->operator [](fileKey).addrdq.front());
		}
		else {
			return fkanull;
		}
	}
	FKI CodeTableHash::getMaxFKI() const {
		if (this->size() > 0) {
			Strg& fileKey =
				const_cast<CodeTableHash*>(this)->addr_file_map.rbegin()->second;
			return FKI(fileKey,
				const_cast<CodeTableHash*>(this)->operator [](fileKey).addrdq.size()
				- 1);
		}
		else {
			return fkinull;
		}
	}
	FKI CodeTableHash::getMinFKI() const {
		if (this->size() > 0) {
			Strg& fileKey =
				const_cast<CodeTableHash*>(this)->addr_file_map.begin()->second;
			return FKI(fileKey, 0);
		}
		else {
			return fkinull;
		}
	}
	/**
	 * @attention 请保证fki必有对应的fka
	 * @param fki
	 * @return
	 */
	FKA CodeTableHash::getFKAByFKI(const FKI& fki) const {
		FKA fka;
		fka.fileKey = fki.fileKey;
		fka.addr =
			const_cast<CodeTableHash*>(this)->operator [](fki.fileKey).addrdq[fki.ID];
		return fka;
	}
	/**
	 * @attention 请保证fka在cdth中存在
	 * @param fka
	 * @return
	 */
	FKI CodeTableHash::getFKIByFKA(const FKA& fka) const {
		FKI fki;
		fki.fileKey = fka.fileKey;
		fki.ID = getAddrPosition(
			const_cast<CodeTableHash*>(this)->operator [](fka.fileKey).addrdq,
			fka.addr);
		return fki;

	}
	/**
	 * @brief 获得当前地址之后的类型为types的代码的地址。
	 * @attention
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return addr 或 addrnull（找不到）
	 */
	Addr CodeTable::getNext(const vector<Intg>& types, const Addr& ad) const {
		Intg pos = getNextAddrPosition(addrdq, ad);
		if (pos < 0) {
#if debug
			cerr << "CodeTable::getNext(types) debug::get Array end,return addrnull"
				<< endl;
#endif
			return Addr();
		}
		Addr a;

		while (pos >= 0) {
			if (std::find(types.begin(), types.end(),
				codedq[pos].assemblyFormula.flagBit.type)
				== types.end() || codedq[pos].assemblyFormula.flagBit.execute == Du) {
				pos++;
			}
			else {
				a = addrdq[pos];
				return a;
			}

		}
		return a;
	}
	/**
	 * @brief 获得当前fka之后的类型为types的代码的fka。
	 * @attention 允许跨cdt（默认）
	 * @param type Y不被认为是COOL_M
	 * @param ad
	 * @return
	 */
	FKA CodeTableHash::getNext(const vector<Intg>& types, const FKA& fka) const {
		Strg fileKey = fka.fileKey;
		Addr addr = fka.addr;

		for (;;) {
			if (this->file_cdt_map.count(fileKey) < 0) {

				{
					static int ignorecount = 0;
					cerr
						<< "CodeTableHash::getNext(const vector<Intg>&, const FKA&)"
						<< " fileKeyNotFound, exit(-1) " << "\tignorecount:["
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
			CodeTable& cdt = const_cast<CodeTableHash*>(this)->operator [](fileKey);
			auto&& addr_ = cdt.getNext(types, addr);
			if (addr_ == addrnull) {
				addr = addrnull;
				fileKey = this->getNextCdtFileKey(fileKey);

			}
			else {
				return FKA(fileKey, addr_);
			}
			if (fileKey == "") {
				return fkanull;
			}

		}
		return fkanull;
	}

	/**
	 * @brief 将CodeTable中所有的argOld替换成argNew
	 * @attention 注意，不替换非参数类型标志位/ar
	 don't change flagbit
	 * @param argOld
	 * @param argNew
	 */
	void CodeTable::replaceVar(const Arg& argOld, const Arg& argNew, acm mode) {
		bool set_asc = (static_cast<int>(mode) & static_cast<int>(acm::asc)) > 0;
		for (auto& cd : codedq) {

			for (int i = 0; i < 4; i++) {
				if (Arg::intersect(cd[i], argOld, mode)) {
					cd.setArg(i, argNew, false, set_asc);
				}

			}

			/*if (it.assemblyFormula.flagBit.arg1_flag == argOld.argFlag
				&& it.assemblyFormula.quaternion.arg1_fka == argOld.arg_fka
				&& it.assemblyFormula.quaternion.arg1_s == argOld.arg_s
				&& it.assemblyFormula.quaternion.arg1_i == argOld.arg_i) {
				it.assemblyFormula.flagBit.arg1_flag = argNew.argFlag;
				it.assemblyFormula.quaternion.arg1_fka = argNew.arg_fka;
				it.assemblyFormula.quaternion.arg1_s = argNew.arg_s;
				it.assemblyFormula.quaternion.arg1_i = argNew.arg_i;
			}
			if (it.assemblyFormula.flagBit.arg2_flag == argOld.argFlag
				&& it.assemblyFormula.quaternion.arg2_fka == argOld.arg_fka
				&& it.assemblyFormula.quaternion.arg2_s == argOld.arg_s
				&& it.assemblyFormula.quaternion.arg2_i == argOld.arg_i) {
				it.assemblyFormula.flagBit.arg2_flag = argNew.argFlag;
				it.assemblyFormula.quaternion.arg2_fka = argNew.arg_fka;
				it.assemblyFormula.quaternion.arg2_s = argNew.arg_s;
				it.assemblyFormula.quaternion.arg2_i = argNew.arg_i;

			}
			if (it.assemblyFormula.flagBit.operator_flag == argOld.argFlag
				&& it.assemblyFormula.quaternion.operator_fka == argOld.arg_fka
				&& it.assemblyFormula.quaternion.operator_s == argOld.arg_s
				&& it.assemblyFormula.quaternion.operator_i == argOld.arg_i) {
				it.assemblyFormula.flagBit.operator_flag = argNew.argFlag;
				it.assemblyFormula.quaternion.operator_fka = argNew.arg_fka;
				it.assemblyFormula.quaternion.operator_s = argNew.arg_s;
				it.assemblyFormula.quaternion.operator_i = argNew.arg_i;

			}
			if (it.assemblyFormula.flagBit.result_flag == argOld.argFlag
				&& it.assemblyFormula.quaternion.result_fka == argOld.arg_fka
				&& it.assemblyFormula.quaternion.result_s == argOld.arg_s
				&& it.assemblyFormula.quaternion.result_i == argOld.arg_i) {
				it.assemblyFormula.flagBit.result_flag = argNew.argFlag;
				it.assemblyFormula.quaternion.result_fka = argNew.arg_fka;
				it.assemblyFormula.quaternion.result_s = argNew.arg_s;
				it.assemblyFormula.quaternion.result_i = argNew.arg_i;

			}*/
		}

	}
	/**
	 * @brief 将CodeTableHash中所有的argOld替换成argNew
	 * @attention 注意，不替换非参数类型标志位/ar
	 * @param argOld
	 * @param argNew
	 */
	void CodeTableHash::replaceVar(const Arg& argOld, const Arg& argNew, acm mode) {
		for (auto& p : file_cdt_map) {
			p.second.replaceVar(argOld, argNew, mode);

		}
		return;

	}
	/**
	 * @brief 重新设置codeTable中和argset有关的变量的可变属性。无视所有标志位
	 * @param arg
	 * @param changeable true(设置为可变) false（设置为不可变）
	 * @attention argset的changeable属性无效
	 */
	void CodeTable::resetChangeable(bool changeable, const set<Arg>& argset) {
		set<Arg> argsettemp = argset;
		if (changeable == true) {
			for (auto it : codedq) {
				Arg&& arg1 = it[0];
				Arg&& arg2 = it[1];
				Arg&& argop = it[2];
				Arg&& argres = it[3];
				if ((!arg1.changeable) && argsettemp.count(arg1)) {
					it.assemblyFormula.flagBit.changeable[0] = T_;

				}
				if ((!arg2.changeable) && argsettemp.count(arg2)) {
					it.assemblyFormula.flagBit.changeable[1] = T_;

				}
				if ((!argop.changeable) && argsettemp.count(argop)) {
					it.assemblyFormula.flagBit.changeable[2] = T_;

				}
				if (!argres.changeable) {
					if (argsettemp.count(argres)) {
						it.assemblyFormula.flagBit.changeable[3] = T_;
					}
					else if (it.assemblyFormula.flagBit.changeable[0] == T_
						|| it.assemblyFormula.flagBit.changeable[1] == T_
						|| it.assemblyFormula.flagBit.changeable[2] == T_) {
						it.assemblyFormula.flagBit.changeable[3] = T_;
						argsettemp.insert(argres);

					}

				}

			}

		}
		else {
			for (auto it : codedq) {
				Arg&& arg1 = it[0];
				Arg&& arg2 = it[1];
				Arg&& argop = it[2];
				Arg&& argres = it[3];
				if (arg1.changeable && argsettemp.count(arg1)) {
					it.assemblyFormula.flagBit.changeable[0] = F_;

				}
				if (arg2.changeable && argsettemp.count(arg2)) {
					it.assemblyFormula.flagBit.changeable[1] = F_;

				}
				if (argop.changeable && argsettemp.count(argop)) {
					it.assemblyFormula.flagBit.changeable[2] = F_;

				}

				if (argres.changeable) {
					if (argsettemp.count(argres)) {
						it.assemblyFormula.flagBit.changeable[3] = F_;
					}
					else if (it.assemblyFormula.flagBit.changeable[0] == F_
						&& it.assemblyFormula.flagBit.changeable[1] == F_
						&& it.assemblyFormula.flagBit.changeable[2] == F_) {
						it.assemblyFormula.flagBit.changeable[3] = F_;
						argsettemp.insert(argres);

					}

				}

			}

		}

	}
	/**
	 * @brief 重新设置codeTableHash中和argset有关的变量的可变属性。无视所有标志位
	 * @param arg
	 * @param changeable true(设置为可变) false（设置为不可变）
	 * @attention argset的changeable属性无效
	 */
	void CodeTableHash::resetChangeable(bool changeable, const set<Arg>& argset) {

		for (auto& p : this->file_cdt_map) {
			p.second.resetChangeable(changeable, argset);
		}
		return;
	}
	/**
	 * @brief 获得树 分支 上的上一个包括arg的代码的地址
	 * @attention 忽略Y中的变量（引用路径中的变量不被认为是此作用域中的变量）
	 * @param ad
	 * @param arg
	 * @return
	 */
	Addr CodeTable::getSameTrBranchLastArgAddr(const Addr& ad, const Arg& arg, acm mode) {
		Addr addr = getLastNot(vector<Intg> { COOL_LIN, COOL_FIL }, ad);
		while (this->count(addr)
			&& ((*this)[addr].assemblyFormula.flagBit.type == COOL_M
				|| (*this)[addr].assemblyFormula.flagBit.type == COOL_Y
				|| (*this)[addr].assemblyFormula.flagBit.execute == Du)) {

			if ((*this)[addr].assemblyFormula.flagBit.execute != Du
				&& (*this)[addr].assemblyFormula.flagBit.type != COOL_Y
				&& (*this)[addr].includeArg(arg, mode)) {
				return addr;
			}
			addr = getLastNot(vector<Intg> { COOL_LIN, COOL_FIL }, addr);
		}
		return addrnull;
	}
	/**
	 * @brief 获得树 分支 上的上一个包括arg的代码的地址
	 * @attention 忽略Y中的变量（引用路径中的变量不被认为是此作用域中的变量），禁止跨cdt
	 * @param ad
	 * @param arg
	 * @return
	 */
	FKA CodeTableHash::getSameTrBranchLastArgFKA(const FKA& fka, const Arg& arg, acm mode) {
		try {
			CodeTable& cdt = this->operator [](fka.fileKey);
			Addr&& addr = cdt.getSameTrBranchLastArgAddr(fka.addr, arg, mode);
			return FKA(fka.fileKey, addr);

		}
		catch (exception& e) {

			return fkanull;
		}
	}
	Addr CodeTable::getSameTrBranchNextArgAddr(const Addr& ad, const Arg& arg, acm mode) {
#if debug
		{
			static int ignorecount = 0;
			cout << "CodeTable::getSameTrBranchNextArgAddr(const Addr&, const Arg&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif
		if (ad == addrnull) {
#if debug
			{
				static int ignorecount = 0;
				cout
					<< "CodeTable::getSameTrBranchNextArgAddr(const Addr&, const Arg&)"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
			return addrnull;
		}
		Addr addr = getNextNot(vector<Intg> { COOL_LIN, COOL_FIL }, ad);
		while (this->count(addr)
			&& ((*this)[addr].assemblyFormula.flagBit.type == COOL_M
				|| (*this)[addr].assemblyFormula.flagBit.type == COOL_R
				|| (*this)[addr].assemblyFormula.flagBit.type == COOL_Y
				|| (*this)[addr].assemblyFormula.flagBit.execute == Du)) {
#if debug
					{
						static int ignorecount = 0;
						cout
							<< "CodeTable::getSameTrBranchNextArgAddr(const Addr&, const Arg&)"
							<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
							<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
					}
#endif
					if ((*this)[addr].assemblyFormula.flagBit.execute != Du
						&& (*this)[addr].assemblyFormula.flagBit.type != COOL_Y
						&& (*this)[addr].includeArg(arg, mode) == true) {
#if debug
							{
								static int ignorecount = 0;
								cout
									<< "CodeTable::getSameTrBranchNextArgAddr(const Addr&, const Arg&)"
									<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
									<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
							}
#endif
							return addr;
					}
#if debug
					{
						static int ignorecount = 0;
						cout
							<< "CodeTable::getSameTrBranchNextArgAddr(const Addr&, const Arg&)"
							<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
							<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
					}
#endif
					addr = getNextNot(vector<Intg> { COOL_LIN, COOL_FIL }, addr);
#if debug
					{
						static int ignorecount = 0;
						cout
							<< "CodeTable::getSameTrBranchNextArgAddr(const Addr&, const Arg&)"
							<< " " << "\tignorecount:[" << ignorecount << "\t]("
							<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
						ignorecount++;
					}
#endif
		}
		return Addr();
	}

	/**
	 * @attention 禁止跨cdt
	 * @param ad
	 * @param arg
	 * @return
	 */
	FKA CodeTableHash::getSameTrBranchNextArgFKA(const FKA& fka, const Arg& arg, acm mode) {
		try {
			CodeTable& cdt = this->operator [](fka.fileKey);
			Addr&& addr = cdt.getSameTrBranchNextArgAddr(fka.addr, arg, mode);
			return FKA(fka.fileKey, addr);

		}
		catch (exception& e) {

			return fkanull;
		}
	}
	/**
	 * @brief 寻找以一个以root为根节点的子树中的变量arg，root被省略。
	 * 因为其实无论root为何值，其都可以被确定。
	 * 当arg为标识符变量(M_Bs||S_Bs)时，只需要寻找父树中codeAddr比root的addr小的上一个arg；
	 * 当arg为数据地址(S_Dz)时，只需要寻找父树中codeAddr比root的addr小且arg为code[3]的arg（对应的addr）
	 * @attention 忽略Y中的变量（引用路径中的变量不被认为是此作用域中的变量）
	 * @param ad (root for the branch)
	 * @param arg
	 * @return
	 */
	Addr CodeTable::getSameTrBranchLastArgAddrIgnoreProperty(const Addr& ad,
		const Arg& arg, acm mode) {
			{
#if debug
				static int ignorecount = 0;
				cout
					<< "CodeTable::getSameTrBranchLastArgAddrIgnoreProperty(const Addr&, const Arg&)"
					<< "in addr:[" << ad.toStrg() << "\t]" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__ << ":0"
					<< ")" << endl;
#endif
			}
			Addr addr = getLastNot(vector<Intg> { COOL_LIN, COOL_FIL }, ad);
			if (addr == addrnull) {
#if debug
				cerr
					<< "CodeTable::getSameTrBranchLastArgAddrIgnoreProperty can't find above addr,return addrnull, ad:["
					<< ad.toStrg() << "]" << endl;
#endif
				return addrnull;
			}
			{
#if debug
				static int ignorecount = 0;
				cout
					<< "CodeTable::getSameTrBranchLastArgAddrIgnoreProperty(const Addr&, const Arg&)"
					<< "1 addr:[" << ad.toStrg() << "\t]" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__ << ":0"
					<< ")" << endl;
#endif
			}

			FKA scpfka = (*this)[ad].scopeFKA;
			set<Intg> specialTypes = { COOL_M , COOL_Y , EXPRQS, EXPRQE };
			while (this->count(addr)
				&& (specialTypes.count((*this)[addr].assemblyFormula.flagBit.type) > 0
					|| (*this)[addr].assemblyFormula.flagBit.execute == Du)) {

				if ((*this)[addr].assemblyFormula.flagBit.execute != Du
					&& (*this)[addr].assemblyFormula.flagBit.type != COOL_Y
					&& !(attributeRelatedOps.count((*this)[addr].assemblyFormula.quaternion.operator_s) > 0
						&& (*this)[addr].assemblyFormula.flagBit.operator_flag
						== M_Bs) && (*this)[addr].scopeFKA == scpfka


					&&(*this)[addr].includeArg(arg, mode)) {
						{
#if debug
							static int ignorecount = 0;
							cout
								<< "CodeTable::getSameTrBranchLastArgAddrIgnoreProperty(const Addr&, const Arg&)"
								<< "out addr:[" << ad.toStrg() << "\t]"
								<< "\tignorecount:[" << ignorecount++ << "\t]("
								<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
#endif
						}
						if ((*this)[addr][3] == arg) {
							//arg必须与code的结果位对应才能表明code为根节点的子叶结点，
							//否则只能证明code与根节点有相同的参数
							return addr;
						}

				}
				addr = getLastNot(vector<Intg> { COOL_LIN, COOL_FIL }, addr);
			}
			{
#if debug
				static int ignorecount = 0;
				cout
					<< "CodeTable::getSameTrBranchLastArgAddrIgnoreProperty(const Addr&, const Arg&)"
					<< "out addr:[" << ad.toStrg() << "\t]" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__ << ":0"
					<< ")" << endl;
#endif
			}
			return addrnull;
	}
	/**
	 * @brief 寻找以一个以root为根节点的子树中的变量arg，root被省略。
	 * 因为其实无论root为何值，其都可以被确定。
	 * 当arg为标识符变量(M_Bs||S_Bs)时，只需要寻找父树中codeAddr比root的addr小的上一个arg；
	 * 当arg为数据地址(S_Dz)时，只需要寻找父树中codeAddr比root的addr小且arg为code[3]的arg（对应的addr）
	 * @attention 忽略Y中的变量（引用路径中的变量不被认为是此作用域中的变量）
	 * @attention 禁止跨cdt
	 * @param ad (root)
	 * @param arg
	 * @return
	 */
	FKA CodeTableHash::getSameTrBranchLastArgFKAIgnoreProperty(const FKA& fka,
		const Arg& arg, acm mode) {
		try {
			CodeTable& cdt = this->operator [](fka.fileKey);
			Addr&& addr = cdt.getSameTrBranchLastArgAddrIgnoreProperty(fka.addr,
				arg, mode);
			return FKA(fka.fileKey, addr);

		}
		catch (exception& e) {

			return fkanull;
		}

	}
	/**
	 * @brief 寻找以一个以root为根节点的子树中的变量arg，root被省略。
	 * 因为其实无论root为何值，其都可以被确定。
	 * 当arg为标识符变量(M_Bs||S_Bs)时，只需要寻找父树中codeAddr比root的addr小的上一个arg；
	 * 当arg为数据地址(S_Dz)时，只需要寻找父树中codeAddr比root的addr小且arg为code[3]的arg（对应的addr）
	 * @param ad (root)
	 * @param arg
	 * @return Addr, PositionInCode(0,1,2,3)
	 */
	pair<Addr, Intg> CodeTable::getSameTrBranchLastArgPositionIgnoreProperty(
		const Addr& ad, const Arg& arg, acm mode) {
			{
#if debug
				static int ignorecount = 0;
				cout
					<< "CodeTable::getSameTrBranchLastArgPositionIgnoreProperty(const Addr&, const Arg&)"
					<< "in addr:[" << ad.toStrg() << "\t]" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__ << ":0"
					<< ")" << endl;
#endif
			}
			Addr addr = getLastNot(vector<Intg> { COOL_LIN, COOL_FIL }, ad);
			if (addr == addrnull) {
#if debug
				cerr
					<< "CodeTable::getSameTrBranchLastArgPositionIgnoreProperty can't find above addr,return addrnull, ad:["
					<< ad.toStrg() << "]" << endl;
#endif
				return pair<Addr, Intg>(addrnull, 0);
			}
			{
#if debug
				static int ignorecount = 0;
				cout
					<< "CodeTable::getSameTrBranchLastArgPositionIgnoreProperty(const Addr&, const Arg&)"
					<< "1 addr:[" << ad.toStrg() << "\t]" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__ << ":0"
					<< ")" << endl;
#endif
			}
			while (this->count(addr)
				&& ((*this)[addr].assemblyFormula.flagBit.type == COOL_M
					|| (*this)[addr].assemblyFormula.flagBit.execute == Du)) {

				if ((*this)[addr].assemblyFormula.flagBit.execute != Du
					&& !((*this)[addr].assemblyFormula.quaternion.operator_s == "$"
						&& (*this)[addr].assemblyFormula.flagBit.operator_flag
						== M_Bs)
					&& !((*this)[addr].assemblyFormula.quaternion.operator_s == "#"
						&& (*this)[addr].assemblyFormula.flagBit.operator_flag
						== M_Bs)
					&& !((*this)[addr].assemblyFormula.quaternion.operator_s == "?"
						&& (*this)[addr].assemblyFormula.flagBit.operator_flag
						== M_Bs)
					&& !((*this)[addr].assemblyFormula.quaternion.operator_s == ":"
						&& (*this)[addr].assemblyFormula.flagBit.operator_flag
						== M_Bs)
					&& !((*this)[addr].assemblyFormula.quaternion.operator_s
						== "out"
						&& (*this)[addr].assemblyFormula.flagBit.operator_flag
						== M_Bs)

					&& (*this)[addr].includeArg(arg, mode)) {
						{
#if debug
							static int ignorecount = 0;
							cout
								<< "CodeTable::getSameTrBranchLastArgPositionIgnoreProperty(const Addr&, const Arg&)"
								<< "out addr:[" << ad.toStrg() << "\t]"
								<< "\tignorecount:[" << ignorecount++ << "\t]("
								<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
#endif
						}

						if ((*this)[addr][3] == arg) {
							return pair<Addr, Intg>(addr, 3);
						}

				}
				addr = getLastNot(vector<Intg> { COOL_FIL, COOL_LIN }, addr);
			}
			{
#if debug
				static int ignorecount = 0;
				cout
					<< "CodeTable::getSameTrBranchLastArgPositionIgnoreProperty(const Addr&, const Arg&)"
					<< "out addr:[" << ad.toStrg() << "\t]" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__ << ":0"
					<< ")" << endl;
#endif
			}
			return pair<Addr, Intg>(addrnull, 0);
	}
	/**
	 * @brief 寻找以一个以root为根节点的子树中的变量arg，root被省略。
	 * 因为其实无论root为何值，其都可以被确定。
	 * 当arg为标识符变量(M_Bs||S_Bs)时，只需要寻找父树中codeAddr比root的addr小的上一个arg；
	 * 当arg为数据地址(S_Dz)时，只需要寻找父树中codeAddr比root的addr小且arg为code[3]的arg（对应的addr）
	 * @param ad (root)
	 * @attention 禁止跨cdt
	 * @param arg
	 * @return FKA, PositionInCode(0,1,2,3)
	 */
	pair<FKA, Intg> CodeTableHash::getSameTrBranchLastArgPositionIgnoreProperty(
		const FKA& fka, const Arg& arg, acm mode) {

		try {
			CodeTable& cdt = this->operator [](fka.fileKey);
			auto&& p = cdt.getSameTrBranchLastArgPositionIgnoreProperty(fka.addr,
				arg, mode);
			return pair<FKA, Intg>(FKA(fka.fileKey, p.first), p.second);

		}
		catch (exception& e) {

			return pair<FKA, Intg>(fkanull, 0);
		}

	}
	/**
	 * @brief 查找某个参数在当前代码后出现的位置，若不存在，返回addrnull
	 * @attention 忽略Y中的变量（引用路径中的变量不被认为是此作用域中的变量）
	 * @param ad (root)
	 * @param arg
	 * todo:: make distinguishing args of different asc or not as an input arg.
	 * @return Addr, PositionInCode(0,1,2,3)
	 */
	pair<Addr, Intg> CodeTable::getNextArgPosition(const Addr& ad, const Arg& arg, acm mode) {
		Intg pos = getNextAddrPosition(addrdq, ad);
		while (pos < size()) {
			Code& cd = codedq[pos];
			if (cd.assemblyFormula.flagBit.type == COOL_Y) {
				continue;
			}
			if (Arg::intersect(cd[0], arg, mode)) {

				return pair<Addr, Intg>(addrdq[pos], 0);

			}
			if (Arg::intersect(cd[1], arg, mode)) {

				return pair<Addr, Intg>(addrdq[pos], 1);

			}
			if (Arg::intersect(cd[2], arg, mode)) {

				return pair<Addr, Intg>(addrdq[pos], 2);

			}
			if (Arg::intersect(cd[3], arg, mode)) {

				return pair<Addr, Intg>(addrdq[pos], 3);

			}
			pos++;
		}
		return pair<Addr, Intg>(Addr(), 0);

	}
	/**
	 * @brief 查找某个参数在当前代码后出现的位置，若不存在，返回addrnull
	 * @attention 忽略Y中的变量（引用路径中的变量不被认为是此作用域中的变量）
	 * @attention 允许跨cdt（默认）
	 * @param fka (root)
	 * @param arg
	 * @return FKA, PositionInCode(0,1,2,3)
	 */
	pair<FKA, Intg> CodeTableHash::getNextArgPosition(const FKA& fka,
		const Arg& arg, acm mode) {

		try {
			FKA fka_(fka);
			while (true) {
				if (fka_.fileKey == "") {
					break;
				}

				CodeTable& cdt = this->operator [](fka_.fileKey);
				auto&& p = cdt.getNextArgPosition(fka_.addr, arg, mode);
				if (p.first == addrnull) {
					fka_.fileKey = getNextCdtFileKey(fka_.fileKey);
					fka_.addr = addrnull;

				}
				else {
					fka_.addr = p.first;
					return pair<FKA, Intg>(fka_, p.second);
				}
			}

			return pair<FKA, Intg>(fkanull, 0);

		}
		catch (exception& e) {

			return pair<FKA, Intg>(fkanull, 0);
		}

	}
	/**
	 * @attention 忽略Y，引用中的变量不属于本作用域
	 * @param ad
	 * @param arg
	 * @return
	 */
	Addr CodeTable::getSameTrBranchNextArgAddrIgnoreProperty(const Addr& ad,
		const Arg& arg, acm mode) {
#if debug
		cout << "CodeTable::getSameTrBranchNextArgAddr1 ad:[" << ad.toStrg()
			<< "]\targ:[" << arg.toStrg() << "]" << endl;
#endif
		if (ad == addrnull) {
#if debug
			cout << "CodeTable::getSameTrBranchNextArgAddr2 addrnull" << endl;
#endif
			return addrnull;
		}
		Addr addr = getNextNot(vector<Intg> { COOL_FIL, COOL_LIN }, ad);
		while (this->count(addr)
			&& ((*this)[addr].assemblyFormula.flagBit.type == COOL_M
				//            || (*this)[addr].assemblyFormula.flagBit.type == COOL_R
				|| (*this)[addr].assemblyFormula.flagBit.type == COOL_Y
				|| (*this)[addr].assemblyFormula.flagBit.execute == Du)) {
#if debug
			cout << "CodeTable::getSameTrBranchNextArgAddr3 " << endl;
#endif
			if ((*this)[addr].assemblyFormula.flagBit.type != COOL_Y
				&& (*this)[addr].includeArg(arg, mode) == true
				&& (*this)[addr].assemblyFormula.flagBit.execute != Du

				&& !((*this)[addr].assemblyFormula.quaternion.operator_s == "$"
					&& (*this)[addr].assemblyFormula.flagBit.operator_flag
					== M_Bs)
				&& !((*this)[addr].assemblyFormula.quaternion.operator_s == "#"
					&& (*this)[addr].assemblyFormula.flagBit.operator_flag
					== M_Bs)
				&& !((*this)[addr].assemblyFormula.quaternion.operator_s == ":"
					&& (*this)[addr].assemblyFormula.flagBit.operator_flag
					== M_Bs)

				) {
#if debug
				cout << "CodeTable::getSameTrBranchNextArgAddr3.5 includeArg"
					<< endl;
#endif
				return addr;
			}
#if debug
			cout << "CodeTable::getSameTrBranchNextArgAddr4 " << endl;
#endif
			addr = getNextNot(vector<Intg> { COOL_FIL, COOL_LIN }, addr);
#if debug
			cout << "CodeTable::getSameTrBranchNextArgAddr5 " << endl;
#endif
		}
		return Addr();
	}
	/**
	 * @attention 忽略Y，引用中的变量不属于本作用域
	 * @attention 禁止跨cdt
	 * @param ad
	 * @param arg
	 * @return
	 */
	FKA CodeTableHash::getSameTrBranchNextArgFKAIgnoreProperty(const FKA& fka,
		const Arg& arg, acm mode) {

		try {
			CodeTable& cdt = this->operator [](fka.fileKey);
			Addr&& addr = cdt.getSameTrBranchNextArgAddrIgnoreProperty(fka.addr,
				arg, mode);
			return FKA(fka.fileKey, addr);

		}
		catch (exception& e) {

			return fkanull;
		}

	}


	/************************************


	NumVec2


	**********************************/

	NumVec2& NumVec2::operator=(const NumVec2& op2) {
		this->list2 = op2.list2;
		return *this;
	}


	NumVec2& NumVec2::operator=(const deque<deque<Numb>>& op2) {
		this->list2 = op2;
		return *this;
	}



	bool NumVec2::operator<(const NumVec2& vector2_) const {
		auto& vector1 = this->list2;
		auto& vector2 = vector2_.list2;
		for (size_t i = 0; i < (std::min)(vector1.size(), vector2.size()); ++i) {
			for (size_t j = 0;
				j < (std::min)(vector1[i].size(), vector2[i].size()); ++j) {
				if (vector1[i][j] < vector2[i][j])
					return true;
				if (vector1[i][j] > vector2[i][j])
					return false;
			}
			if (vector1[i].size() < vector2[i].size())
				return true;
			if (vector1[i].size() > vector2[i].size())
				return false;
		}

		if (vector1.size() < vector2.size())
			return true;
		if (vector1.size() > vector2.size())
			return false;

		return false; // The two vectors are equal.

	}


	bool NumVec2::operator==(const NumVec2& op2) const {
		return this->list2 == op2.list2;

	}



	bool NumVec2::operator!=(const NumVec2& op2) const {
		return this->list2 != op2.list2;

	}


	int NumVec2::size() const {
		return (int)list2.size();
	}



	deque<Numb>& NumVec2::operator[](const int& i) {
		int i_ = i;
		while (i_ > (int)list2.size() - 1) {
			list2[i_] = { };
			--i_;

		}
		return list2[i];
	}

	/**
			 * @convert "xx,xx;xx,xx..." into a 2-dimension matrix in std::vec
			 * @param input
			 */
	NumVec2::NumVec2(const Strg& input) {
		auto& result = list2;
		std::stringstream ss(input);
		std::string item;

		while (std::getline(ss, item, ';')) { // Split by semicolons
			std::deque<Numb> row;
			std::stringstream rowStream(item);
			std::string number;

			while (std::getline(rowStream, number, ',')) { // Split by commas
				row.push_back(toNumb(number)); // Convert to integer and add to row
			}

			result.push_back(row); // Add row to result
		}

	}


	Strg NumVec2::toStrg() const {
		stringstream ss;
		ss << "{";

		int i = 0;
		for (const auto& innerlist : list2) {
			int j = 0;
			ss << "{";
			for (const auto& elem : innerlist) {
				ss << elem;
				if (++j < (int)innerlist.size()) {
					ss << ",";

				}
			}
			ss << "}";
			if (++i < (int)list2.size()) {
				ss << ",";
			}
		}

		ss << "}";
		return ss.str();
	}

	/************************************

	NumVec2BinaryInfo

	********************************/


	Intl NumVec2BinaryInfo::parseAndWriteObject(const NumVec2& nv2, fstream& fs) {

		this->list2Info.parseAndWriteObject(nv2.list2, fs);
		return fs.tellp();

	}


	Intl NumVec2BinaryInfo::constructObject(NumVec2& nv2, fstream& fs) {

		this->list2Info.constructObject(nv2.list2, fs);
		return fs.tellg();

	}

	/*********************************************


	TemplateFunction


	*******************************/



	TemplateFunction::TemplateFunction(const Scope& functionNameScope) {
		nameStartFKA = FKA(functionNameScope.fileKey, functionNameScope.scopeCoor1);
		nameEndFKA = FKA(functionNameScope.fileKey, functionNameScope.scopeCoor2);

	}
	TemplateFunction::TemplateFunction(FKA fka_built_in) {
		nameStartFKA = fka_built_in;
		nameEndFKA = fka_built_in;
		templateHandleFKA = fka_built_in;
		bodyScopeFKA = fka_built_in;
		bodyEndFKA = fka_built_in;
		returnHandle.push_back(fka_built_in);
		integrate = F_;
		organization = TFInfoMat(BUILTIN_FUN_INFO);
	}

	Strg TemplateFunction::toStrg() const {

		stringstream ss;
		ss.precision(PRECISION);
		ss << "integrate:" << integrate << "\t" << "organization:"
			<< organization.toStrg() << "\t" << "isBackwardFunction:"
			<< isBackwardFunction << "\t" << "matchLevel:" << matchLevel
			<< "\t" << "forwardFunctionFKA:" << forwardFunctionFKA.toStrg()
			<< "\t" << "nameStartFKA:" << nameStartFKA.toStrg() << "\t"
			<< "nameEndFKA:" << nameEndFKA.toStrg() << "\t"
			<< "bodyScopeFKA:" << bodyScopeFKA.toStrg() << "\t"
			<< "bodyStartFKA:" << bodyStartFKA.toStrg() << "\t"
			<< "bodyEndFKA:" << bodyEndFKA.toStrg() << "\t"
			<< "templateHandleFKA:" << templateHandleFKA.toStrg() << "\t";
		for (const auto& fka_ : returnHandle) {
			ss << fka_.toStrg() << "\t";
		}
		return ss.str();

	}




	/****************************************


	TemplateFunctionBinaryInfo


	***********************************/

	Intl TemplateFunctionBinaryInfo::parseAndWriteObject(const TemplateFunction& tf, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout
				<< "parseAndWriteObject(const TemplateFunction&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->integrate = tf.integrate;

		this->isBackwardFunction = tf.isBackwardFunction;
		this->matchLevel = tf.matchLevel;
		this->forwardFunctionFKAInfo.parseAndWriteObject(tf.forwardFunctionFKA,
			fs);
		this->nameStartFKAInfo.parseAndWriteObject(tf.nameStartFKA, fs);
		this->nameEndFKAInfo.parseAndWriteObject(tf.nameEndFKA, fs);
		this->bodyScopeFKAInfo.parseAndWriteObject(tf.bodyScopeFKA, fs);
		this->bodyStartFKAInfo.parseAndWriteObject(tf.bodyStartFKA, fs);
		this->bodyEndFKAInfo.parseAndWriteObject(tf.bodyEndFKA, fs);
		this->templateHandleFKAInfo.parseAndWriteObject(tf.templateHandleFKA,
			fs);
		this->returnHandleInfo.parseAndWriteObject(tf.returnHandle, fs);
		this->organizationInfo.parseAndWriteObject(tf.organization, fs);
		return fs.tellp();

	}


	Intl TemplateFunctionBinaryInfo::constructObject(TemplateFunction& tf, fstream& fs) {
		tf.integrate = this->integrate;

		tf.isBackwardFunction = this->isBackwardFunction;
		tf.matchLevel = this->matchLevel;
		this->forwardFunctionFKAInfo.constructObject(tf.forwardFunctionFKA, fs);
		this->nameStartFKAInfo.constructObject(tf.nameStartFKA, fs);
		this->nameEndFKAInfo.constructObject(tf.nameEndFKA, fs);
		this->bodyScopeFKAInfo.constructObject(tf.bodyScopeFKA, fs);
		this->bodyStartFKAInfo.constructObject(tf.bodyStartFKA, fs);
		this->bodyEndFKAInfo.constructObject(tf.bodyEndFKA, fs);
		this->templateHandleFKAInfo.constructObject(tf.templateHandleFKA, fs);
		this->returnHandleInfo.constructObject(tf.returnHandle, fs);
		this->organizationInfo.constructObject(tf.organization, fs);
		return fs.tellg();

	}



	/**************************************************

	TemplateFunctionTable

	*************************************************/

	TemplateFunction& TemplateFunctionTable::operator[](const Addr& tfAddr) {
		return tfdq[getAddrPosition<deque<Addr> >(addrdq, tfAddr)];
	}

	Addr TemplateFunctionTable::lastTfAddr(const TFAddr& tfAddr) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "lastTfAddr(const TFAddr&)" << " in, tfaddr:[ "
				<< tfAddr.toStrg() << "]" << "\tignorecount:["
				<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
				<< ":0" << ")" << std::endl;
		}
#endif
		Intg i = getLastAddrPosition<deque<Addr> >(addrdq, tfAddr);
		if (i < 0) {
			return Addr();
		}
		if (tfdq[i].nameEndFKA.addr >= tfAddr
			&& tfdq[i].nameStartFKA.addr <= tfAddr) {
			i -= 1;
		}
		if (i < 0) {
			return Addr();
		}
		Addr a = addrdq[i];
#if debug
		cout << "TemplateFunctionTable::lastTfAddr addr out:[" << a.toStrg()
			<< "]" << endl;
#endif
		return a;
	}


	Addr TemplateFunctionTable::nextTfAddr(const Addr& codeTableAddr) {
		Intg i = getNextAddrPosition<deque<Addr> >(addrdq, codeTableAddr);
		if (i > (Intg)addrdq.size() - 1 || i < 0) {
			return Addr();
		}
		Addr a =
			addrdq[getLastAddrPosition<deque<Addr> >(addrdq, codeTableAddr)];
		return a;
	}


	bool TemplateFunctionTable::add(const Addr& tfAddr, const TemplateFunction& tf) {
		addrdq.push_back(tfAddr);
		tfdq.push_back(tf);
		return true;
	}


	TemplateFunctionTable::TemplateFunctionTable(TemplateFunctionTableHash* tfth,
		const Strg& fileKey) {
		this->tfth = tfth;
		this->fileKey = fileKey;

	}


	FKA TemplateFunctionTable::lastAccessibleTFFKA(
		ScopeTableHash* const scopeTableHash,
		CodeTableHash* const codeTableHash,
		SystemTableHash* const systemTableHash, const FKA& upbound,
		const FKA& codeOrScopeFKA) {
		return this->tfth->lastAccessibleTFFKA(scopeTableHash, codeTableHash,
			systemTableHash, upbound, codeOrScopeFKA);

	}



	Strg TemplateFunctionTable::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << "fileKey:" << fileKey << "\n";
		int len = addrdq.size();
		TemplateFunctionTable* tft = const_cast<TemplateFunctionTable*>(this);
		for (Intg i = 0; i < len; ++i) {
			ss << "Addr:" << tft->addrdq[i].toStrg() << "\tTemplateFun:"
				<< tft->tfdq[i].toStrg() << endl;
		}
		return ss.str();
	}

	/*************************************

	TemplateFunctionTableBinaryInfo

	*******************************/

	Intl TemplateFunctionTableBinaryInfo::parseAndWriteObject(
		const TemplateFunctionTable& tft, fstream& fs) {
#if debug
			{
				static int ignorecount = 0;
				std::cout
					<< "TemplateFunctionTableBinaryInfo::parseAndWriteObject(const TemplateFunctionTable&, fstream&)"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
			}
#endif
			this->fileKeyInfo.parseAndWriteObject(tft.fileKey, fs);
			this->addrdqinfo.parseAndWriteObject(tft.addrdq, fs);
			this->tfdqinfo.parseAndWriteObject(tft.tfdq, fs);
			return fs.tellp();
	}
	Intl TemplateFunctionTableBinaryInfo::constructObject(
		TemplateFunctionTable& tft, fstream& fs) {
		this->fileKeyInfo.constructObject(tft.fileKey, fs);
		this->addrdqinfo.constructObject(tft.addrdq, fs);
		this->tfdqinfo.constructObject(tft.tfdq, fs);
		return fs.tellg();
	}


	/****************************************


	TemplateFunctionTableHash


	**************************************/


	TemplateFunctionTableHash::TemplateFunctionTableHash(CodeTableHash* cdth) {
		this->cdth = cdth;
		const Strg& s = FILEKEYBUILTIN;
		TemplateFunctionTable tft(this, s);
		Intg p = ADDR_NEG_LOWBOUND;
		while (p < 0) {
			tft.add(Addr(p), TemplateFunction(FKA(s, Addr(p))));
			p++;
		}
		file_tft_map.insert(make_pair(FILEKEYBUILTIN, tft));
	}

	/**
	 * @brief 查询codeORscopeFKA可访问到，且在upbound之前的函数（在函数表中）的地址
	 * @attention 当codeORscopeFKA是作用域fka时，不会检索该作用域内的函数。
	 * @attention 对无效输入的代码地址报错，没有合适的结果返回fkanull。
	 * @param scopeTable
	 * @param codeTable
	 * @param upbound 查询的tf地址必须小于此fka,此地址为scopeFKA时会取用scope的coor1
	 * @param codeORscopeFKA 可以输入代码表地址或函数表、作用域表地址，都行的,查询此处地址可访问到的tf地址
	 * @param
	 * @return
	 */
	FKA TemplateFunctionTableHash::lastAccessibleTFFKA(
		ScopeTableHash* const scopeTableHash,
		CodeTableHash* const codeTableHash,
		SystemTableHash* const systemTableHash, const FKA& upbound,
		const FKA& codeORscopeFKA) {
#if debug
			{
				static int ignorecount = 0;
				cout
					<< "TemplateFunctionTableHash::lastAccessibleTFFKA(ScopeTable* const, CodeTable* const, const Addr&)"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif

			FKA origin; // codeFKA ，tf可以被此地址访问，此地址是codeFKA而非TFfka
			if (codeTableHash->find(codeORscopeFKA) == true) {
				origin = codeORscopeFKA;
			}
			else if (scopeTableHash->find(codeORscopeFKA) == true) {
				Scope& scp = (*scopeTableHash)[codeORscopeFKA];
				origin.addr = scp.scopeCoor1;
				origin.fileKey = scp.fileKey;
			}
			else {
				//当此地址不是codeTable地址也不是scope地址时，
				//直接采用代码表中最接近的上一行代码的地址
				CodeTable& cdt = (*codeTableHash)[codeORscopeFKA.fileKey];
				origin.addr = cdt.addrdq[getLastAddrPosition(cdt.addrdq,
					codeORscopeFKA.addr)];
				origin.fileKey = codeORscopeFKA.fileKey;

			}
			FKA dest; //目标tf地址
			if (codeTableHash->find(upbound) == true) {
				dest = upbound;
			}
			else if (scopeTableHash->find(upbound) == true) {
				Scope& scp = (*scopeTableHash)[upbound];
				dest.addr = scp.scopeCoor1;
				dest.fileKey = scp.fileKey;
			}
			else if (upbound.addr < addrnull) {
				dest = upbound;
			}
			else if (upbound.LT(cdth, codeORscopeFKA) == false) {
				dest = codeORscopeFKA;
			}
			dest = this->lastTfFKA(dest);
			if (dest.addr > addrnull) {
				//以下代码，从其父作用域开始递归查询上一个可访问tf

				if (scopeTableHash->accessible(origin,
					(*codeTableHash)[origin].scopeFKA, dest)) {
					return dest;
				}
				else {
					//判断
					FKA dest_parentScopeFKA = (*scopeTableHash)[dest].parentScopeFKA;
					if (dest_parentScopeFKA == fkanull) {
						return lastAccessibleTFFKA(scopeTableHash, codeTableHash,
							systemTableHash, dest, codeORscopeFKA);
					}
					Scope& dest_parentScope =
						(*scopeTableHash)[(*scopeTableHash)[dest].parentScopeFKA];
					if (dest_parentScope.functionFlag == SYSB) {
						FKA origin_systemFKA;
						try {
							origin_systemFKA =
								systemTableHash->getDirectSystemByCodeFKA(origin).systemFKA;

						}
						catch (int& e) {

							origin_systemFKA = fkanull;
						}
						if (origin_systemFKA == fkanull) {
							return lastAccessibleTFFKA(scopeTableHash, codeTableHash,
								systemTableHash, dest, codeORscopeFKA);
						}
						else if (systemTableHash->isChild(origin_systemFKA,
							dest_parentScopeFKA)) {
							return dest;
						}
						else {
							return lastAccessibleTFFKA(scopeTableHash, codeTableHash,
								systemTableHash, dest, codeORscopeFKA);
						}

					}

				}

			}

			else if (dest.addr < addrnull) {
#if debug
				cout << "TemplateFunctionTable::lastAccessibleTFFKA 6" << endl;
#endif

				return dest;

			}
			else {
#if debug
				cout << "TemplateFunctionTable::lastAccessibleTFFKA 7" << endl;
#endif
#if debug
				{
					static int ignorecount = 0;
					cout
						<< "TemplateFunctionTable::lastAccessibleTFFKA(ScopeTable* const, CodeTable* const, const Addr&)"
						<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
				}
#endif
				return fkanull;
			}
#if debug
			cout << "TemplateFunctionTable::lastAccessibleTFFKA 8" << endl;
#endif
			return fkanull;
	}



	void TemplateFunctionTableHash::add(TemplateFunctionTable& tft) {
		assert(tft.fileKey != "");
		file_tft_map.insert(make_pair(tft.fileKey, tft));
	}



	/**
			 * @attention 不检查tf有关信息是否与scopeTableHash匹配
			 * @param fka 为函数声明作用域的scopeFKA
			 * @param tf
			 */
	void TemplateFunctionTableHash::add(const FKA& fka, TemplateFunction& tf) {
		assert(fka.fileKey != "");
		if (file_tft_map.count(fka.fileKey) <= 0) {
			TemplateFunctionTable tft(this, fka.fileKey);
			tft.add(fka.addr, tf);
			this->add(tft);
		}
		else {
			TemplateFunctionTable& tft = file_tft_map[fka.fileKey];
			tft.add(fka.addr, tf);
			this->add(tft);
		}
	}


	TemplateFunction& TemplateFunctionTableHash::operator[](const FKA& tffka) {
		assert(tffka.fileKey != "");
		TemplateFunctionTable& tft = this->file_tft_map[tffka.fileKey];
		return tft.tfdq[getAddrPosition<deque<Addr> >(tft.addrdq, tffka.addr)];
	}



	TemplateFunctionTable& TemplateFunctionTableHash::operator[](const Strg& fileKey) {
		assert(fileKey != "");
		return this->file_tft_map[fileKey];
	}




	Strg TemplateFunctionTableHash::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		cout << "TemplateFunctionTableHash:\n";
		for (const auto& it : this->file_tft_map) {
			ss << "FileKey:" << it.first << "\n";
			ss << it.second.toStrg() << "\n";
		}
		return ss.str();
	}


	Intg TemplateFunctionTableHash::count(const FKA& fka) {
		auto it = file_tft_map.find(fka.fileKey);
		if (it != file_tft_map.end()) {
			return getAddrPosition(it->second.addrdq, fka.addr) >= 0;
		}
		else {
			return 0;
		}
	}

	/// <summary>
		/// copy the codetable snippet (COOL_M) of the function name.
		/// </summary>
		/// <param name="tffka"></param>
	CodeTable TemplateFunctionTableHash::getTFNameCdt(const FKA& tffka, ScopeTableHash* scpth) {
		assert(tffka != fkanull);
		if (tffka.fileKey == FILEKEYBUILTIN) {
			CodeTable cdt;
			Code cd;
			cd.setArg(2, Arg(M_Bs, TemplateFunctionTableHash::getTFOperatorByFKA(tffka)), false, false);
			cdt.addCode(tffka.addr, cd);
			return cdt;
		}
		else {
			assert(scpth->cdth != nullptr);
			Scope& scp = (*scpth)[tffka];
			FKA&& tfnameroot = scpth->cdth->getLast(COOL_M,
				FKA(scp.fileKey, scp.scopeCoor2));

			auto&& ofc = scpth->cdth->copyTree(tfnameroot);
			return get<0>(ofc);
		}

	}

	/**
	 * @attention 检查tf有关作用域信息是否与scopeTableHash匹配,若不匹配会修改至匹配
	 * @param fka 为函数声明作用域的scopeFKA
	 * @param tf
	 */
	void TemplateFunctionTableHash::add(const FKA& fka, TemplateFunction& tf,
		ScopeTableHash* scpth) {
		assert(fka.fileKey != "");
		if (file_tft_map.count(fka.fileKey) <= 0) {
			TemplateFunctionTable tft(this, fka.fileKey);
			Scope& scp = (*scpth)[fka];
			tf.nameStartFKA = FKA(scp.fileKey, scp.scopeCoor1);
			tf.nameEndFKA = FKA(scp.fileKey, scp.scopeCoor2);
			tft.add(fka.addr, tf);
			this->add(tft);
		}
		else {
			TemplateFunctionTable& tft = this->file_tft_map[fka.fileKey];
			Scope& scp = (*scpth)[fka];
			tf.nameStartFKA = FKA(scp.fileKey, scp.scopeCoor1);
			tf.nameEndFKA = FKA(scp.fileKey, scp.scopeCoor2);
			tft.add(fka.addr, tf);
			//this->add(tft);
		}
	}




	/**
		 * @attention 此函数接受的fka如果是tffka，则返回nexttffka，
		 * 如果fka是codefka，则返回tffka>codefka的下一个tffka
		 * @param fka tffka或codefka均可
		 * @return
		 */
	FKA TemplateFunctionTableHash::lastTfFKA(const FKA& tffka) {
		if (tffka.fileKey == "" || tffka == fkanull) {
			throw exception(("Invalid tffka: " + tffka.toStrg()).c_str());
		}
		FKA fka_ = tffka;
		//tffka is a built in func
		if (fka_.fileKey == FILEKEYBUILTIN) {
			if (fka_.addr.addrv.back() > ADDR_NEG_LOWBOUND) {
				fka_.addr.addrv.back() -= 1;
				return fka_;
			}
			else {
				return fkanull;
			}
		}
		else {
			//tffka is not a built in func
			TemplateFunctionTable& tft = this->file_tft_map[fka_.fileKey];
			Addr&& addr_ = tft.lastTfAddr(fka_.addr);
			if (addr_ != addrnull) {
				//tffka isn't the top func in its tffunc table
				fka_.addr = addr_;
				return fka_;
			}
			else {
				//tffka is the top func in its tffunc table
				Strg fileKey_ = this->cdth->getLastCdtFileKey(fka_.fileKey);
				if (fileKey_ == "") {
					fileKey_ = FILEKEYBUILTIN;
					return FKA(fileKey_, Addr(-1));
				}
				else {
					while (fileKey_ != "") {
						TemplateFunctionTable& tft_ = this->file_tft_map.operator [](
							fileKey_);
						if (tft_.addrdq.size() > 0) {
							fka_.fileKey = fileKey_;
							fka_.addr = tft_.addrdq.back();
							return fka_;

						}
						else {
							fileKey_ = this->cdth->getLastCdtFileKey(fileKey_);
						}

					}
					fileKey_ = FILEKEYBUILTIN;
					return FKA(fileKey_, Addr(-1));

				}

			}

		}


	}
	FKA TemplateFunctionTableHash::nextTfFKA(const FKA& tffka) {
		if (tffka.fileKey == "" || tffka == fkanull) {
			throw exception(("Invalid tffka: " + tffka.toStrg()).c_str());
		}
		FKA fka_ = tffka;
		//fka is built in func
		if (fka_.fileKey == FILEKEYBUILTIN) {
			if (fka_.addr.addrv.back() < -1) {
				fka_.addr.addrv.back() += 1;
				return fka_;
			}
			else {
				if (this->cdth->addr_file_map.size() > 0) {
					Strg fileKey_ = this->cdth->addr_file_map.begin()->second;
					while (fileKey_ != "") {

						TemplateFunctionTable& tft_ = this->file_tft_map.operator [](
							fileKey_);
						if (tft_.addrdq.size() > 0) {
							fka_.fileKey = fileKey_;
							fka_.addr = tft_.addrdq.front();
							return fka_;

						}
						else {
							fileKey_ = this->cdth->getNextCdtFileKey(fileKey_);
						}

					}

					return fkanull;
				}
				else {
					return fkanull;
				}

			}
		}
		else {
			//fka isn't built in func 
			TemplateFunctionTable& tft = this->file_tft_map[fka_.fileKey];
			auto&& addr_ = tft.nextTfAddr(fka_.addr);
			if (addr_ != addrnull) {
				//tffka isn't the last func in its tffunc table
				fka_.addr = addr_;
				return fka_;
			}
			else {
				//tffka is the last func in its tffunc table
				Strg fileKey_ = this->cdth->getNextCdtFileKey(fka_.fileKey);
				while (fileKey_ != "") {

					TemplateFunctionTable& tft_ = this->file_tft_map.operator [](
						fileKey_);
					if (tft_.addrdq.size() > 0) {
						fka_.fileKey = fileKey_;
						fka_.addr = tft_.addrdq.front();
						return fka_;

					}
					else {
						fileKey_ = this->cdth->getNextCdtFileKey(fileKey_);
					}

				}

				return fkanull;
			}

		}




	}
	/// <summary>
	/// if the fka is in a template function, return the template function name scope, else return fkanull
	/// </summary>
	/// <param name="fka"></param>
	/// <returns></returns>
	FKA TemplateFunctionTableHash::getTFFKA(const FKA& scopefka, ScopeTableHash* scpth) {
		if (scpth == nullptr) {
			throw exception("invalid ScopeTableHash.");
		}
		FKA currentscopefka = scopefka;
		while (currentscopefka != fkanull) {
			auto& currentscp = (*scpth)[currentscopefka];

			if (currentscp.functionFlag == FB || currentscp.functionFlag == RFB) {
				if (currentscp.requireScopeFKA != fkanull) {
					currentscopefka = currentscp.requireScopeFKA;
				}
				else {
					currentscopefka = currentscp.parentScopeFKA;

				}

			}
			else if (currentscp.functionFlag == FN) {
				return currentscopefka;

			}
			else {
				currentscopefka = currentscp.parentScopeFKA;

			}
		}
		return fkanull;

	}












	/************************************

	TemplateFunctionTableHashBinaryInfo

	***********************************/

	Intl TemplateFunctionTableHashBinaryInfo::parseAndWriteObject(const TemplateFunctionTableHash& tfth,
		fstream& fs) {
#if debug
			{
				static int ignorecount = 0;
				std::cout
					<< "parseAndWriteObject(const TemplateFunctionTableHash&, fstream&)"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
			}
#endif
			this->file_tft_map_info.parseAndWriteObject(tfth.file_tft_map, fs);
			return fs.tellp();
	}



	Intl TemplateFunctionTableHashBinaryInfo::constructObject(TemplateFunctionTableHash& tfth, fstream& fs) {
		this->file_tft_map_info.constructObject(tfth.file_tft_map, fs);
		return fs.tellg();
	}














	/**************************************

	Scope

	***********************************/

	Strg Scope::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << "functionFlag:" << functionFlag << "\t" << "conditionFlag:"
			<< conditionFlag << "\t" << "matched:" << matched << "\t"
			<< "fileKey:" << fileKey << "\t" << "scopeCoor1:"
			<< scopeCoor1.toStrg() << "\t" << "scopeCoor2:"
			<< scopeCoor2.toStrg() << "\t" << "parentScopeFKA:"
			<< parentScopeFKA.toStrg() << "\t" << "requireScopeFKA:"
			<< requireScopeFKA.toStrg() << "\t" << "conditionCodeFKA:"
			<< conditionCodeFKA.toStrg() << "\t"
			<< "validTemplateFunctionNameFKA:"
			<< validTemplateFunctionNameFKA.toStrg() << "\t";
		for (const auto& fka_ : returnHandle) {
			ss << fka_.toStrg() << "\t";
		}
		ss << "scopeProperty:" << " " << "\t";
		return ss.str();
	}

	/*************************************

	ScopeBinaryInfo

	**********************************/


	Intl ScopeBinaryInfo::parseAndWriteObject(const Scope& scp, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout
				<< "ScopeBinaryInfo::parseAndWriteObject(const Scope&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->functionFlag = scp.functionFlag;
		this->conditionFlag = scp.conditionFlag;
		this->matched = scp.matched;
		this->fileKeyInfo.parseAndWriteObject(scp.fileKey, fs);
		this->scopeCoor1Info.parseAndWriteObject(scp.scopeCoor1, fs);
		this->scopeCoor2Info.parseAndWriteObject(scp.scopeCoor2, fs);
		this->parentScopeFKAInfo.parseAndWriteObject(scp.parentScopeFKA, fs);
		this->requireScopeFKAInfo.parseAndWriteObject(scp.requireScopeFKA, fs);
		this->conditionCodeFKAInfo.parseAndWriteObject(scp.conditionCodeFKA, fs);
		this->validTemplateFunctionNameFKAInfo.parseAndWriteObject(
			scp.validTemplateFunctionNameFKA, fs);
		this->returnHandleInfo.parseAndWriteObject(scp.returnHandle, fs);
		//暂时不处理property
		return fs.tellp();

	}
	Intl ScopeBinaryInfo::constructObject(Scope& scp, fstream& fs) {
		scp.functionFlag = this->functionFlag;
		scp.conditionFlag = this->conditionFlag;
		scp.matched = this->matched;
		this->fileKeyInfo.constructObject(scp.fileKey, fs);
		this->scopeCoor1Info.constructObject(scp.scopeCoor1, fs);
		this->scopeCoor2Info.constructObject(scp.scopeCoor2, fs);
		this->parentScopeFKAInfo.constructObject(scp.parentScopeFKA, fs);
		this->requireScopeFKAInfo.constructObject(scp.requireScopeFKA, fs);
		this->conditionCodeFKAInfo.constructObject(scp.conditionCodeFKA, fs);
		this->validTemplateFunctionNameFKAInfo.constructObject(
			scp.validTemplateFunctionNameFKA, fs);
		this->returnHandleInfo.constructObject(scp.returnHandle, fs);
		//暂时不处理property
		return fs.tellg();

	}


	/************************************

	ScopeTable (partial)

	******************************/


	/*
			 * @attention 不能通过此[] 运算符创建新的scope。
			 */
	Scope& ScopeTable::operator[](const Addr& addr) {
		if (addr == addrnull) {
			return scopenull;
		}
		return scopedq[getAddrPosition<deque<Addr> >(addrdq, addr)];
	}


	Addr ScopeTable::getNextScopeAddr(const Addr& codeTableAddr) {
		return addrdq[getNextAddrPosition(addrdq, codeTableAddr)];
	}

	Addr ScopeTable::getLastScopeAddr(const Addr& codeTableAddr) {
		return addrdq[getLastAddrPosition(addrdq, codeTableAddr)];
	}

	ScopeTable::ScopeTable(ScopeTableHash* scpth, const Strg& fileKey) {
		this->scpth = scpth;
		this->fileKey = fileKey;
	}


	Strg ScopeTable::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << "fileKey:" << fileKey << "\n";
		int len = addrdq.size();
		ScopeTable* scpt = const_cast<ScopeTable*>(this);
		for (Intg i = 0; i < len; ++i) {
			ss << "Addr:" << scpt->addrdq[i].toStrg() << "\tScope:"
				<< scpt->scopedq[i].toStrg() << endl;
		}
		return ss.str();
	}



	/*******************************

	ScopeTableBinaryInfo

	******************************/


	Intl ScopeTableBinaryInfo::parseAndWriteObject(const ScopeTable& scpt,
		fstream& fs) {
#if debug
			{
				static int ignorecount = 0;
				std::cout
					<< "ScopeTableBinaryInfo::parseAndWriteObject(const ScopeTable&, fstream&)"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
			}
#endif
			this->fileKeyInfo.parseAndWriteObject(scpt.fileKey, fs);
			this->addrdqlist.parseAndWriteObject(scpt.addrdq, fs);
			this->scopedqlist.parseAndWriteObject(scpt.scopedq, fs);
			return fs.tellp();
	}
	Intl ScopeTableBinaryInfo::constructObject(ScopeTable& scpt, fstream& fs) {
		this->fileKeyInfo.constructObject(scpt.fileKey, fs);
		this->addrdqlist.constructObject(scpt.addrdq, fs);
		this->scopedqlist.constructObject(scpt.scopedq, fs);
		return fs.tellg();
	}

	/***********************************

	ScopeTableHash (partial)

	***********************************/

	ScopeTableHash::ScopeTableHash(CodeTableHash& cdth) {
		this->cdth = &cdth;
	}

	void ScopeTableHash::add(ScopeTable& scpt) {
		this->file_scpt_map.insert(make_pair(scpt.fileKey, scpt));
	}


	FKA ScopeTableHash::getNextScopeFKA(const FKA& codeTableFKA) {
		ScopeTable& scpt = this->operator [](codeTableFKA.fileKey);
		Intg pos = getNextAddrPosition(scpt.addrdq, codeTableFKA.addr);
		if (pos <= 0) {
			FKA fka_;
			fka_.addr = addrnull;
			fka_.fileKey = this->cdth->getNextCdtFileKey(codeTableFKA.fileKey);

			return getNextScopeFKA(fka_);

		}
		return FKA(codeTableFKA.fileKey, scpt.addrdq[pos]);

	}


	FKA ScopeTableHash::getLastScopeFKA(const FKA& codeTableFKA) {
		ScopeTable& scpt = this->operator [](codeTableFKA.fileKey);
		Intg pos = getLastAddrPosition(scpt.addrdq, codeTableFKA.addr);
		if (pos <= 0) {
			FKA fka_;
			fka_.addr = addrmax;
			fka_.fileKey = this->cdth->getLastCdtFileKey(codeTableFKA.fileKey);

			return getLastScopeFKA(fka_);

		}
		return FKA(codeTableFKA.fileKey, scpt.addrdq[pos]);
	}


	Strg ScopeTableHash::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		cout << "ScopeTableHash:\n";
		for (const auto& it : this->file_scpt_map) {
			ss << "FileKey:" << it.first << "\n";
			ss << it.second.toStrg() << "\n";
		}
		return ss.str();
	}



	/**
		 * @brief 根据fileKey返回scopeTable,禁止利用此函数禁止创建新的键值对
		 * @param fileKey
		 * @return
		 */
	ScopeTable& ScopeTableHash::operator[](const Strg& fileKey) {
		if (file_scpt_map.count(fileKey) > 0) {
			return file_scpt_map[fileKey];
		}
		else {

			{
				static int ignorecount = 0;
				cerr << "ScopeTableHash::operator [](const Strg&)"
					<< " fileKeyNotFound, exit(-1) " << "\tignorecount:["
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
			assert(false); exit(-1);
		}
	}
	Scope& ScopeTableHash::operator[](const FKA& fka) {
		ScopeTable& scp = (*this)[fka.fileKey];
		return scp[fka.addr];
	}
	bool ScopeTableHash::find(const FKA& fka) {
		if (file_scpt_map.count(fka.fileKey) > 0) {
			if (getAddrPosition(file_scpt_map[fka.fileKey].addrdq, fka.addr) >= 0) {

				return true;
			}
		}
		return false;
	}
	/**
	 * @brief 用于判断是否能从作用域origin访问到作用域dest(scope to scope)，适用于类与函数
	 * @param origin 起始作用域的COOL_QS地址，
	 * @param dest 目标作用域的COOL_QS地址
	 * @return
	 */
	bool ScopeTableHash::accessible(const FKA& origin, const FKA& dest) {
		//不能访问在后面定义的作用域
		if (origin.LT(this->cdth, dest)) {
			return false;
		}
		//两者相同可以访问
		if (origin == dest) {
			return true;
		}
		//dest是公共域也可以访问
		if (dest == fkanull) {
			return true;
		}
		//dest是origin上层scope或上层scope的直接子scope的话可以访问
		FKA dest_parent_fka = this->operator [](dest).parentScopeFKA;
		if (dest_parent_fka == fkanull) {
			return true;
		}
		FKA orig_parent_fka = this->operator [](origin).parentScopeFKA;
		while (orig_parent_fka != fkanull) {
			if (orig_parent_fka == dest_parent_fka) {
				return true;
			}
			orig_parent_fka = this->operator [](orig_parent_fka).parentScopeFKA;
		}

		//其他情况无法访问
		return false;

	}
	/**
	 * @brief 用于判断是否能从代码地址origin访问到作用域dest(fka to scope)，适用于类与函数
	 * @attention 仅仅从作用域角度考虑是否可以访问，不考虑类之间的继承关系引起的访问
	 * @param origin 起始代码codeTable地址（FKA）（不能是scope地址）
	 * @param parent 起始代码所在scope的fka
	 * @param dest 目标作用域的scope的fka
	 * @return
	 */
	bool ScopeTableHash::accessible(const FKA& origin, const FKA& parent,
		const FKA& dest) {
		//不能访问在后面定义的作用域
		if (origin.LT(this->cdth, dest)) {
			return false;
		}
		//可以访问父作用域
		if (parent == dest) {
			return true;
		}
		//dest是公共域也可以访问
		if (dest == fkanull) {
			return true;
		}
		//dest与origin有相同的父作用域
		if (this->operator [](dest).parentScopeFKA == parent) {
			return true;
		}
		//dest是origin上层scope或上层scope的直接子scope的话可以访问
		FKA dest_parent_fka = this->operator [](dest).parentScopeFKA;
		if (dest_parent_fka == fkanull) {
			return true;
		}
		FKA orig_parent_fka = this->operator [](parent).parentScopeFKA;
		while (orig_parent_fka != fkanull) {
			if (orig_parent_fka == dest_parent_fka) {
				return true;
			}
			orig_parent_fka = this->operator [](orig_parent_fka).parentScopeFKA;
		}

		//其他情况无法访问
		return false;

	}






	/************************************

	ScopeTableHashBinaryInfo

	********************************/


	Intl ScopeTableHashBinaryInfo::parseAndWriteObject(const ScopeTableHash& scpth, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "parseAndWriteObject(const ScopeTableHash&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->file_scpt_map_info.parseAndWriteObject(scpth.file_scpt_map, fs);
		return fs.tellp();

	}


	Intl ScopeTableHashBinaryInfo::constructObject(ScopeTableHash& scpth, fstream& fs) {
		this->file_scpt_map_info.constructObject(scpth.file_scpt_map, fs);
		return fs.tellg();

	}


	/********************************


	ScopeTable & ScopeTableHash


	*********************************/



	Intg ScopeTable::newScopeStart(Addr& scopeAddr, Scope& scope) {
		addrdq.push_back(scopeAddr);
		scopedq.push_back(scope);
		return 0;
	}
	Intg ScopeTableHash::newScopeStart(FKA& scopeFKA, Scope& scope) {
		if (this->file_scpt_map.count(scopeFKA.fileKey) <= 0) {
			this->file_scpt_map[scopeFKA.fileKey] = ScopeTable(this,
				scopeFKA.fileKey);
		}
		scope.fileKey = scopeFKA.fileKey;
		ScopeTable& scpt = file_scpt_map[scopeFKA.fileKey];
		scpt.addrdq.push_back(scopeFKA.addr);
		scpt.scopedq.push_back(scope);
		return 0;
	}
	Intg ScopeTable::scopeEnd(Addr& scopeAddr, Scope& scope) {
		scopedq[getAddrPosition<deque<Addr> >(addrdq, scopeAddr)] = scope;
		return 0;
	}
	Intg ScopeTableHash::scopeEnd(FKA& scopeFKA, Scope& scope) {
		ScopeTable& scpt = file_scpt_map[scopeFKA.fileKey];
		scpt.scopedq[getAddrPosition<deque<Addr> >(scpt.addrdq, scopeFKA.addr)] =
			scope;
		return 0;
	}















	/***************************************


	System Class

	***************************************/



	System::System(const Strg& name) {
		systemName = name;
	}
	System::System(const Strg& name, const FKA& parentScopeFKA) {
		systemName = name;
		this->parentScopeFKA = parentScopeFKA;
		this->fki.fileKey = parentScopeFKA.fileKey;
	}
	System::System(const Strg& name, const FKA& systemScopeFKA,
		const FKA& parentScopeFKA) {
		systemName = name;
		systemFKA = systemScopeFKA;
		this->parentScopeFKA = parentScopeFKA;
		this->fki.fileKey = systemScopeFKA.fileKey;
	}
	Strg System::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << systemName << "\t" << systemFKA.toStrg() << "\tconstructorFKAList:";
		for (auto& fka : constructorFKAList) {
			ss << fka.toStrg() << "\t";
		}
		ss << "decedentSystemFKIList:[";
		for (auto& fki : decedentSystemFKIList) {
			ss << fki.toStrg() << "\t";
		}
		ss << "]";

		return ss.str();
	}





	/***************************************

	SystemBinaryInfo

	*******************************/

	Intl SystemBinaryInfo::parseAndWriteObject(const System& sys, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "parseAndWriteObject(const System&, fstream&)" << " "
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->fkiInfo.parseAndWriteObject(sys.fki, fs);
		this->systemNameInfo.parseAndWriteObject(sys.systemName, fs);
		this->systemFKAInfo.parseAndWriteObject(sys.systemFKA, fs);
		this->parentScopeFKAInfo.parseAndWriteObject(sys.parentScopeFKA, fs);
		this->constructorFKAListInfo.parseAndWriteObject(sys.constructorFKAList,
			fs);
		this->decedentSystemFKIListInfo.parseAndWriteObject(
			sys.decedentSystemFKIList, fs);
		return fs.tellp();
	}


	Intl SystemBinaryInfo::constructObject(System& sys, fstream& fs) {
		this->fkiInfo.constructObject(sys.fki, fs);
		this->systemNameInfo.constructObject(sys.systemName, fs);
		this->systemFKAInfo.constructObject(sys.systemFKA, fs);
		this->parentScopeFKAInfo.constructObject(sys.parentScopeFKA, fs);
		this->constructorFKAListInfo.constructObject(sys.constructorFKAList,
			fs);
		this->decedentSystemFKIListInfo.constructObject(
			sys.decedentSystemFKIList, fs);
		return fs.tellg();
	}


	/**********************************

	SystemTable (partial)

	***********************************/


	SystemTable::SystemTable(SystemTableHash* systh, Strg fileKey) {
		this->systh = systh;
		this->fileKey = fileKey;
	}


	Strg SystemTable::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << "fileKey:" << fileKey << "\n";

		for (const auto& it : id_system_map) {
			ss << "ID:" << it.first << "\tSystem:" << it.second.toStrg()
				<< endl;
		}
		return ss.str();
	}

	/**
		 * @brief 相当于reset，当sys不存在时，创建sys，存在时，则令已存在的sys等于新的sys
		 * @param sys
		 */
	void SystemTable::add(const System& sys) {
#if debug
		{
			static int ignorecount = 0;
			cout << "SystemTable::add(const System&)" << "\tsysName:["
				<< sys.systemName << "]" << "\tignorecount:[" << ignorecount++
				<< "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif

		if (this->systh->systemExisted(sys)) {
			FKI sysfki = this->systh->getSystemFKI(
				std::make_pair(sys.systemName, sys.parentScopeFKA));
			SystemTable& syst = this->systh->operator [](sysfki.fileKey);
			System& sysexist = syst[sysfki.ID];

			sysexist.constructorFKAList.insert(sys.constructorFKAList.begin(),
				sys.constructorFKAList.end());
			sysexist.systemName = sys.systemName;
			sysexist.decedentSystemFKIList.insert(sys.decedentSystemFKIList.begin(),
				sys.decedentSystemFKIList.end());
			sysexist.parentScopeFKA = sys.parentScopeFKA;
			if (sys.systemFKA != fkanull) {
				sysexist.systemFKA = sys.systemFKA;
				syst.fka_id_map[sysexist.systemFKA] = sysexist.fki.ID;
			}
			return;
		}
		++id;
		id_system_map[id] = sys;
		id_system_map[id].fki.ID = id;
		id_system_map[id].fki.fileKey = this->fileKey;
		name_iddq_map[sys.systemName].push_back(id);
		if (sys.systemFKA != fkanull) {
			fka_id_map[sys.systemFKA] = id;
		}
	}
	/**
	 * @brief 判断system是否已经存在（声明过）同名、同父作用域
	 * @param sys
	 * @return
	 */
	bool SystemTable::systemExisted(const System& sys) {
		if (name_iddq_map.count(sys.systemName) > 0) {
			for (auto& id : name_iddq_map[sys.systemName]) {
				if (id_system_map[id].parentScopeFKA == sys.parentScopeFKA) {
					return true;
				}
			}

		}
		else {
			return false;
		}
		return false;
	}


	/*******************************************

	SystemTableBinaryInfo

	*********************************/
	Intl SystemTableBinaryInfo::parseAndWriteObject(const SystemTable& syst, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "parseAndWriteObject(const SystemTable&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->id = syst.id;
		this->fileKeyInfo.parseAndWriteObject(syst.fileKey, fs);
		this->id_system_map_info.parseAndWriteObject(syst.id_system_map, fs);
		this->name_iddq_map_info.parseAndWriteObject(syst.name_iddq_map, fs);
		this->fka_id_map_info.parseAndWriteObject(syst.fka_id_map, fs);
		return fs.tellp();

	}


	Intl SystemTableBinaryInfo::constructObject(SystemTable& syst, fstream& fs) {
		syst.id = this->id;
		this->fileKeyInfo.constructObject(syst.fileKey, fs);
		this->id_system_map_info.constructObject(syst.id_system_map, fs);
		this->name_iddq_map_info.constructObject(syst.name_iddq_map, fs);
		this->fka_id_map_info.constructObject(syst.fka_id_map, fs);
		return fs.tellg();

	}



	/************************************

	SystemTableHash (partial)

	*********************************/

	/**
	 * @brief 判断system是否在某个库中已经存在（声明过的）同名、同父作用域的sys
	 * @param sys
	 * @return
	 */
	bool SystemTableHash::systemExisted(const System& sys) {
		for (auto& systp : this->file_syst_map) {
			if (systp.second.systemExisted(sys) == true) {
				return true;
			}
		}
		return false;
	}
	/**
	 *
	 * @param maychild (SYSB fka)
	 * @param mayparent(SYSB fka)
	 * @return
	 */
	bool SystemTableHash::isChild(const FKA& maychild, const FKA& mayparent) {
		auto fkichild = (*this)[maychild].fki;
		auto fkiparent = (*this)[mayparent].fki;
		if (maychild.LT(cdth, mayparent) || maychild == fkanull
			|| fkichild == fkinull || fkiparent == fkinull) {
			return false;
		}

		if ((*this)[fkichild].decedentSystemFKIList.count(fkiparent)) {
			return true;
		}
		else {
			for (auto fki_ : (*this)[fkichild].decedentSystemFKIList) {
				FKA& maychild_ = (*this)[fki_].systemFKA;
				if (isChild(maychild_, mayparent) == true) {
					return true;
				}
			}
			return false;
		}

	}



	SystemTableHash::SystemTableHash(CodeTableHash& cdth, ScopeTableHash& scpth) {
		this->cdth = &cdth;
		this->scpth = &scpth;

	}


	void SystemTableHash::add(SystemTable& syst) {
		this->file_syst_map.insert(make_pair(syst.fileKey, syst));
	}


	void SystemTableHash::add(System& sys) {
		if (sys.fki.fileKey == "") {

			{
				static int ignorecount = 0;
				std::cerr << "add(System&)"
					<< " system fki.fileKey == null, exit(-1) "
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
		if (this->file_syst_map.count(sys.fki.fileKey) <= 0) {
			this->file_syst_map[sys.fki.fileKey] = SystemTable(this,
				sys.fki.fileKey);
		}
		this->file_syst_map[sys.fki.fileKey].add(sys);
	}



	Strg SystemTableHash::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		cout << "SystemTableHash:\n";
		for (const auto& it : this->file_syst_map) {
			ss << "FileKey:" << it.first << "\n";
			ss << it.second.toStrg() << "\n";
		}
		return ss.str();
	}




	/**************************************


	SystemTableHashBinaryInfo


	*************************************/

	Intl SystemTableHashBinaryInfo::parseAndWriteObject(const SystemTableHash& systh, fstream& fs) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "parseAndWriteObject(const SystemTableHash&, fstream&)"
				<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
				<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		this->file_syst_map_info.parseAndWriteObject(systh.file_syst_map, fs);
		return fs.tellp();
	}


	Intl SystemTableHashBinaryInfo::constructObject(SystemTableHash& systh, fstream& fs) {
		this->file_syst_map_info.constructObject(systh.file_syst_map, fs);
		return fs.tellg();
	}


	/*****************************************


	SystemTable & SystemTableHash


	*************************************/




	/**
		 * @attention 禁止通过此函数在表中创建system实例。
		 */
	System& SystemTable::operator[](const FKA& fka) {
		if (fka_id_map.count(fka) > 0) {
			return id_system_map[fka_id_map[fka]];
		}
		else {

			{
				static int ignorecount = 0;
				cerr << "operator [](const Addr&)"
					<< " cant find system by addr,assert(false); exit(-1);"
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
			assert(false); exit(-1);
		}

	}
	/**
	 * @attention 禁止通过此函数在表中创建system实例。
	 */
	System& SystemTableHash::operator[](const FKA& fka) {

		for (auto& systp : this->file_syst_map) {

			if (systp.second.fka_id_map.count(fka) > 0) {
				return systp.second.id_system_map[systp.second.fka_id_map[fka]];
			}
		}

		{
			static int ignorecount = 0;
			cerr << "SystemTableHash::operator [](const FKA&)"
				<< " sysNotFound, exit(-1) " << "\tignorecount:["
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
		assert(false); exit(-1);

	}
	System& SystemTable::operator[](const Intg& id) {
		return id_system_map[id];
	}

	/**
	 * @attention 禁止通过此函数在表中创建system实例。
	 * @param name_parentScopeAddr，给出当前所在父活动域以及system名称，返回满足要求的作用域的引用。
	 * @return
	 */
	System& SystemTable::operator[](const pair<Strg, FKA>& name_parentScopeFKA) {
		return this->systh->operator [](name_parentScopeFKA);

	}
	System& SystemTableHash::operator[](
		const pair<Strg, FKA>& name_parentScopeFKA) {
		FKI fki = getSystemFKI(name_parentScopeFKA);
		if (fki != fkinull) {
			return this->operator [](fki);

		}

		else {

			{
				static int ignorecount = 0;
				cerr << "SystemTableHash::operator [](const pair<Strg,FKA>&)"
					<< "cant find system , exit(-1) " << "\tignorecount:["
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
			assert(false); exit(-1);
		}

	}
	/**
	 * @brief 返回fileKey对应的SystemTable
	 * @param fileKey
	 * @return
	 */
	SystemTable& SystemTableHash::operator[](const Strg& fileKey) {
		if (file_syst_map.count(fileKey) > 0) {
			return file_syst_map[fileKey];
		}
		else {

			{
				static int ignorecount = 0;
				cerr << "SystemTableHash::operator [](const Strg&)"
					<< " FileNotFound, exit(-1) " << "\tignorecount:["
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
			assert(false); exit(-1);
		}

	}
	System& SystemTableHash::operator[](const FKI& fki) {
		return file_syst_map[fki.fileKey][fki.ID];

	}
	/**
	 * @brief 如果找不到会 throw -1
	 * @param codeFKA
	 * @return
	 */
	System& SystemTableHash::getDirectSystemByCodeFKA(const FKA& codeFKA) {
		if (codeFKA.addr < addrnull) {
			throw - 1; //built in function
		}
		FKA scopeFKA = (*cdth)[codeFKA].scopeFKA;

		if (scopeFKA == fkanull) {
#if debug
			{
				static int ignorecount = 0;
				cerr << "SystemTableHash::getDirectSystemByCodeFKA(const FKA&)"
					<< " codeNotInScope" << "\tignorecount:[" << ignorecount++
					<< "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")"
					<< endl;
			}
#endif
			throw - 1;
		}
		else {
			while (scopeFKA != fkanull) {

				if ((*scpth)[scopeFKA].functionFlag == SYSB) {
					return this->operator [](scopeFKA);
				}
				scopeFKA = (*scpth)[scopeFKA].parentScopeFKA;
			}
			//找不到说明也不在system中
			throw - 1;
		}
	}
	/**
	 * @brief 根据sys名称和当前代码所在父作用域，查询sys的id（仅在当前scopeTable中查询）
	 * @attention 如果不存在则返回-1
	 * @param name_parentScopeFKA
	 * @return
	 */
	Intg SystemTable::getSystemID(const pair<Strg, FKA>& name_parentScopeFKA) {
		const Strg& name = name_parentScopeFKA.first;
		const FKA& fka = name_parentScopeFKA.second;
		bool exist = name_iddq_map.count(name);
		if (exist) {
			for (auto& id : name_iddq_map[name]) {
				System& sys = id_system_map[id];
				if (systh->scpth->accessible(fkamax, fka, sys.systemFKA)) {
					return id;
				}
			}
#if debug
			{
				static int ignorecount = 0;
				cout << "getSystemID(const pair<Strg, FKA> &name_parentScopeFKA)"
					<< " system unaccessible,return -1" << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << endl;
			}
#endif
			return -1;

		}

		else {
#if debug
			{
				static int ignorecount = 0;
				cerr << "getSystemID(const pair<Strg, FKA> &name_parentScopeFKA)"
					<< " cant find system by addr,return -1;"
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << endl;
			}
#endif
			return -1;
		}

	}
	/**
	 * @brief 根据sys名称和当前代码所在父作用域，查询sys的fki（在整个scopeTableHash中查询）
	 * @param name_parentScopeFKA
	 * @return
	 */
	FKI SystemTableHash::getSystemFKI(const pair<Strg, FKA>& name_parentScopeFKA) {
		FKI fki;
		for (auto& systp : this->file_syst_map) {
			Intg id = systp.second.getSystemID(name_parentScopeFKA);
			if (id > 0) {
				fki.fileKey = systp.first;
				fki.ID = id;
				return fki;

			}

		}
#if debug
		{
			static int ignorecount = 0;
			cout << "SystemTableHash::getSystemFKI(const pair<Strg,FKA>&)"
				<< " sysNotFound " << "\tignorecount:[" << ignorecount++
				<< "\t](" << __FILE__ << ":" << __LINE__ << ":0" << ")" << endl;
		}
#endif
		return fki;

	}
	void SystemTable::erase(Intg id) {
		id_system_map.erase(id);
	}
	Strg SystemTable::toStrg() {
		stringstream ss;
		ss.precision(PRECISION);
		for (auto& sys : id_system_map) {
			ss << "[" << sys.first << "]\t" << sys.second.toStrg() << endl;
		}
		ss << endl;
		return ss.str();
	}
	/**
	 * @brief 这个函数用于添加继承关系
	 * @param childName 子名称
	 * @param childParentScopeAddr 子系统体的父作用域
	 * @param parentName 父名称
	 */
	void SystemTableHash::inherit(const Strg& childName,
		const FKA& childParentScopeFKA, const Strg& parentName) {
		System& child = this->operator [](
			make_pair(childName, childParentScopeFKA));
		FKI fki = getSystemFKI(make_pair(parentName, childParentScopeFKA));
		child.decedentSystemFKIList.insert(fki);

	}





	/**********************************

	HashCollection

	**********************************/




	//Strg HashCollection::saveToSingleFile() {
		//    Strg &&path_ = getenv(COOLANG_PROJECT_PATH);
		//    return saveToSingleFile(path_);
		//}
	pathStr HashCollection::saveToSingleFile(const pathStr& dir_path) {
#if debug
		{
			static int ignorecount = 0;
			std::cout << "HashCollection::saveToSingleFile(const Strg&)"
				<< "\tignorecount:[" << ignorecount++ << "\t]"
				<< " HashCollection:" << this->toStrg() << "(" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		pathStr path_ = dir_path;

		pathStr fileName = StringToWString(
			split(fileKey, "#").front() + COOLANG_BINARY_CODE_SUFFIX);

		filesystem::path path__(path_.c_str());
		if (std::filesystem::is_directory(path__) == false) {
			path__ = path__.parent_path();
		}
		if (std::filesystem::exists(path__) == false) {
			if (path__.empty())
			{
				path__ = ".";
			}
			else
			{

				std::filesystem::create_directories(path__);

			}

		}
		path__.append(fileName);
#if debug
		{
			static int ignorecount = 0;
			std::cout << "HashCollection::saveToSingleFile(const Strg&)"
				<< " binary output file path: " << path__.string()
				<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
				<< ":" << __LINE__ << ":0" << ")" << std::endl;
		}
#endif
		fstream fs;
		fs.open(path__, fs.out | fs.trunc | fs.binary);
		try {

			HashCollectionBinaryInfo hcbi;
			hcbi.parseAndWriteObject(*this, fs);

		}
		catch (exception& e) {

			{
				static int ignorecount = 0;
				cerr << "HashCollection::saveToSingleFile()"
					<< " write error, exit(-1) ,e.what:" << e.what()
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
			fs.close();
			assert(false); exit(-1);
		}
		fs.close();
		return pathToPathStr(path__);

	}
	/**
	 * @attention 一个hashCollection对应一个文件，hashcollection的fileKey代表无后缀文件名
	 * @attention 一个hashCollection可能包含多个codeTable，codeTable中的fileKey为“文件名#起始行数”
	 * @return
	 */
	map<Strg, HashCollection> HashCollection::splitSelfToSimpleHashs() {
		map<Strg, HashCollection> hcv;

		for (auto&& it = cdth.addr_file_map.begin(); it != cdth.addr_file_map.end();
			++it) {
			Strg fileKey = it->second;
			auto&& vector_ = split(fileKey, "#");
			Strg fileKeyName = vector_.front();
			[[maybe_unused]] Intg fileKeyLine = 0;
			if (vector_.size() > 1) {
				fileKeyLine = toIntg(vector_.back());
			}
			if (hcv.count(fileKey) <= 0) {
				hcv[fileKeyName] = HashCollection();
			}
			HashCollection& hc = hcv[fileKeyName];
			hc.fileKey = fileKeyName;
			hc.cdth.appendCdt(this->cdth[fileKey]);
			if (this->scpth.file_scpt_map.count(fileKey) > 0) {
				hc.scpth.add(this->scpth[fileKey]);
			}
			if (this->tfth.file_tft_map.count(fileKey) > 0) {
				hc.tfth.add(this->tfth.file_tft_map[fileKey]);
			}
			if (this->systh.file_syst_map.count(fileKey) > 0) {
				hc.systh.add(this->systh.file_syst_map[fileKey]);
			}
		}
		return hcv;
	}
	HashCollection HashCollection::extractSimpleHash(const Strg& fileKey_) {
		Strg fileKey = fileKey_;
		auto&& vector_ = split(fileKey, "#");
		Strg fileKeyName = vector_.front();
		HashCollection hc = HashCollection();
		hc.fileKey = fileKeyName;
		for (auto&& it = cdth.addr_file_map.begin(); it != cdth.addr_file_map.end();
			++it) {
			Strg fileKeyIt = it->second;
			auto&& vectorIt = split(fileKeyIt, "#");
			Strg fileKeyNameIt = vectorIt.front();
			if (fileKeyNameIt == fileKeyName) {

				hc.cdth.appendCdt(this->cdth[fileKeyIt]);
				if (this->scpth.file_scpt_map.count(fileKeyIt) > 0) {
					hc.scpth.add(this->scpth[fileKeyIt]);
				}
				if (this->tfth.file_tft_map.count(fileKeyIt) > 0) {
					hc.tfth.add(this->tfth.file_tft_map[fileKeyIt]);
				}
				if (this->systh.file_syst_map.count(fileKeyIt) > 0) {
					hc.systh.add(this->systh.file_syst_map[fileKeyIt]);
				}
			}
		}
		return hc;

	}
	//Strg HashCollection::saveToMultiFiles() {
	//    auto &&hcv = splitSelfToSimpleHashs();
	//    Strg savePath;
	//    for (auto &shcvp : hcv) {
	//        savePath = shcvp.second.saveToSingleFile();
	//    }
	//    return savePath;
	//
	//}
	pathStr HashCollection::saveToMultiFiles(const pathStr& dir_path) {
		auto&& hcv = splitSelfToSimpleHashs();
		for (auto& shcvp : hcv) {
			shcvp.second.saveToSingleFile(dir_path);
		}
		return dir_path;

	}
	void HashCollection::loadFromBinaryFile(const pathStr& path_) {
		filesystem::path path__(path_.c_str());
		fstream fs;
		fs.open(path__, fs.in | fs.binary);
		try {
			HashCollectionBinaryInfo hcbi;
			hcbi.constructObject(*this, fs);

		}
		catch (exception& e) {

			{
				static int ignorecount = 0;
				cerr << "HashCollection::loadFromBinaryFile(const Strg &path_)"
					<< " write error, exit(-1) ,e.what:" << e.what()
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
			fs.close();
			assert(false); exit(-1);
		}
		fs.close();
		return;
	}





	/**
 * @brief 载入全部库文件
 */
	void HashCollection::completeAll() {
		set<Strg> loadedFileKey;        //FKF

		set<Strg> set1;
		set<Strg> set2;
		set<Strg>* currentUFKSet = &set1; //current Unloaded FileKey set,内部fileKey对应file名称
		set<Strg>* nextUFKSet = &set2;

		map<Strg, set<Strg>> FKF_FKN_map;

		deque<Strg> fkdq;        //此deque将被用来生成最后的addr_file_map,
		//依赖树越靠近顶层的fk位于前面
		for (auto& afp : this->cdth.addr_file_map) {
			fkdq.push_back(afp.second);
		}
		loadedFileKey.insert(this->fileKey);
		auto&& dependentFK = this->cdth.getDependentFileKeyFileStem();
		currentUFKSet->insert(dependentFK.begin(), dependentFK.end());

		while (currentUFKSet->size() > 0 || nextUFKSet->size() > 0) {
			if (currentUFKSet->size() <= 0) {
				set<Strg>* swapset = currentUFKSet;
				currentUFKSet = nextUFKSet;
				nextUFKSet = swapset;

			}
			for (auto& fileKeyIt : *currentUFKSet) {
				auto&& fkv = split(fileKeyIt, "#");
				//删除fkdq中原有的fileKeyit，然后在fkdq前端添加fileKeyit
				//此步骤不重复加载已有fk对应hashcollection，只会修改fkdq
				if (fkv.size() > 1) {
					if (loadedFileKey.count(fkv.front()) > 0) {
						if (FKF_FKN_map[fkv.front()].count(fileKeyIt) > 0) {
							for (auto&& it = fkdq.begin(); it != fkdq.end(); ++it) {
								if (it.operator *() == fileKeyIt) {
									fkdq.erase(it);
									fkdq.push_front(fileKeyIt);
									break;
								}

							}
							//大循环
							continue;
						}
						else {
							FKF_FKN_map[fkv.front()].insert(fileKeyIt);
						}
					}
					else {
						loadedFileKey.insert(fkv.front());
						FKF_FKN_map[fkv.front()].insert(fileKeyIt);

					}
				}
				else {
					if (loadedFileKey.count(fkv.front()) > 0) {
						if (loadedFileKey.count(fileKeyIt) <= 0) {

							for (auto&& it = fkdq.begin(); it != fkdq.end(); ++it) {
								if (it.operator *() == fileKeyIt) {
									fkdq.erase(it);
									fkdq.push_front(fileKeyIt);
									break;
								}

							}
						}
						else {
							for (auto& fileKeyIt_ : FKF_FKN_map[fileKeyIt]) {

								for (auto&& it = fkdq.begin(); it != fkdq.end();
									++it) {
									if (it.operator *() == fileKeyIt_) {
										fkdq.erase(it);
										fkdq.push_front(fileKeyIt_);
										break;
									}

								}
							}

						}
						//大循环
						continue;
					}
					else {
						loadedFileKey.insert(fkv.front());

					}
				}

				auto pathv1 = getFilePath(coolang_exec_path.c_str(),
					fileKey + COOLANG_BINARY_CODE_SUFFIX, true);
				if (pathv1.size() <= 0) {
					pathv1 = getFilePath(coolang_project_path,
						fileKey + COOLANG_BINARY_CODE_SUFFIX, true);
					if (pathv1.size() <= 0) {
						pathv1 = getFilePath(coolang_home_path,
							fileKey + COOLANG_BINARY_CODE_SUFFIX, true);
						if (pathv1.size() <= 0) {

							{
								static int ignorecount = 0;
								cerr << "HashCollection::completeAll()"
									<< " FileNotFound, exit(-1) "
									<< "\tignorecount:[" << ignorecount++
									<< "\t](" << __FILE__ << ":" << __LINE__
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
							assert(false); exit(-1);
						}
					}
				}

				//读入hashcollection
				HashCollection hc;
				hc.loadFromBinaryFile(pathStr(pathv1.front()));
				//为下一轮循环nextUFKSet做准备
				auto&& dependentFK_ = hc.cdth.getDependentFileKeyFileStem();
				nextUFKSet->insert(dependentFK_.begin(), dependentFK_.end());
				//添加hc中的所有cdt至this,
				for (auto& afp : hc.cdth.addr_file_map) {
					if (this->cdth.file_addr_map.count(afp.second) > 0) {
						for (auto&& it = fkdq.begin(); it != fkdq.end(); ++it) {
							if (it.operator *() == afp.second) {
								fkdq.erase(it);
								fkdq.push_front(afp.second);
								break;
							}

						}
						continue;
					}
					else {
						Strg& fk_ = afp.second;
						CodeTable& cdt_ = hc.cdth.operator [](fk_);
						this->cdth.appendCdt(cdt_);
						if (this->scpth.file_scpt_map.count(fk_) <= 0
							&& hc.scpth.file_scpt_map.count(fk_) > 0) {
							this->scpth.add(hc.scpth.file_scpt_map[fk_]);
						}
						if (this->tfth.file_tft_map.count(fk_) <= 0
							&& hc.tfth.file_tft_map.count(fk_) > 0) {
							this->tfth.add(hc.tfth.file_tft_map[fk_]);
						}
						if (this->systh.file_syst_map.count(fk_) <= 0
							&& hc.systh.file_syst_map.count(fk_) > 0) {
							this->systh.add(hc.systh.file_syst_map[fk_]);
						}

					}

				}

			}
			currentUFKSet->clear();

		}

		//重新构造cdth的索引目录
		this->cdth.addr_file_map.clear();
		this->cdth.file_addr_map.clear();
		auto&& addrv = split(Addr(1), Addr(fkdq.size() + 4), fkdq.size());
		for (Intg i = 0; i < (Intg)fkdq.size(); ++i) {
			this->cdth.addr_file_map.insert(make_pair(addrv[i], fkdq[i]));
			this->cdth.file_addr_map.insert(make_pair(fkdq[i], addrv[i]));

		}
		return;

	}
	Strg HashCollection::toStrg() const {
		stringstream ss;
		ss.precision(PRECISION);
		ss << "HashCollection FileKey:" << this->fileKey << "\n";
		ss << "HashCollection cdth:\n" << this->cdth.toStrg() << "\n";
		ss << "HashCollection scpth:\n" << this->scpth.toStrg() << "\n";
		ss << "HashCollection tfth:\n" << this->tfth.toStrg() << "\n";
		ss << "HashCollection systh:\n" << this->systh.toStrg() << "\n";

		return ss.str();

	}




















	/*************************************

	HashCollectionBinaryInfo

	*********************************/


	Intl HashCollectionBinaryInfo::parseAndWriteObject(const HashCollection& hashCollection,
		fstream& fs) {
#if debug
			{
				static int ignorecount = 0;
				std::cout << "parseAndWriteObject(const HashCollection&, fstream&)"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
			}
#endif
			this->completed = hashCollection.completed;
			//当前对象结构对应info应当被写入的位置,此位置由其上层info决定。
			//当没有上层info时由自己决定。此即为没有上层info时的情况
			Intl object_info_output_position = fs.tellp();
			Intl object_value_output_position = object_info_output_position
				+ sizeof(HashCollectionBinaryInfo);  //与当前对象结构info对应的值应当从此处开始记录；
			//写入value
			fs.seekp(object_value_output_position, fs.beg);
			settingInfo.parseAndWriteObject(hashCollection.setting, fs);
			fileKeyInfo.parseAndWriteObject(hashCollection.fileKey, fs);
			cdthInfo.parseAndWriteObject(hashCollection.cdth, fs);
			scpthInfo.parseAndWriteObject(hashCollection.scpth, fs);
			tfthInfo.parseAndWriteObject(hashCollection.tfth, fs);
			systhInfo.parseAndWriteObject(hashCollection.systh, fs);

			//写入info
			fs.seekp(object_info_output_position, fs.beg);
			fs.write(reinterpret_cast<const char*>(this),
				sizeof(HashCollectionBinaryInfo));
			fs.seekp(0, fs.end);

			return fs.tellp();
	}

	Intl HashCollectionBinaryInfo::constructObject(HashCollection& hashCollection, fstream& fs) {
		//当前对象结构对应info本应由其上层info读取
		//当没有上层info时必须自己读入自己的info。

		//读入自身info
		fs.read(reinterpret_cast<char*>(this),
			sizeof(HashCollectionBinaryInfo));

		hashCollection.completed = this->completed;

		//读入自身value
		settingInfo.constructObject(hashCollection.setting, fs);
		fileKeyInfo.constructObject(hashCollection.fileKey, fs);
		cdthInfo.constructObject(hashCollection.cdth, fs);
		scpthInfo.constructObject(hashCollection.scpth, fs);
		tfthInfo.constructObject(hashCollection.tfth, fs);
		systhInfo.constructObject(hashCollection.systh, fs);

		//关联内部指针
		for (auto& p_file_cdt : hashCollection.cdth.file_cdt_map) {
			p_file_cdt.second.cdth = &hashCollection.cdth;
		}
		for (auto& p_file_scpt : hashCollection.scpth.file_scpt_map) {
			p_file_scpt.second.scpth = &hashCollection.scpth;
		}
		for (auto& p_file_tft : hashCollection.tfth.file_tft_map) {
			p_file_tft.second.tfth = &hashCollection.tfth;
		}
		for (auto& p_file_syst : hashCollection.systh.file_syst_map) {
			p_file_syst.second.systh = &hashCollection.systh;
		}

		return fs.tellg();
	}











}