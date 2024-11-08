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

#ifndef COOLANG_VM_HPP_
#define COOLANG_VM_HPP_


#include "coolang_setting.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <list>
#include <vector>
#include <deque>
#include <stdint.h>
#include <limits.h>
#include <set>
#include <algorithm>
#include <iterator>
#include <ranges>
#include <filesystem>
#include <io.h>
#include <memory>
#include <type_traits>
#include "coolang_define.hpp"
#include "coolang_exception.hpp"
#include "pipeCommunication.hpp"
#include <tuple>

using namespace std;
namespace COOLANG {
	inline filesystem::path coolang_home_path(charpToPath(getenv(COOLANG_HOME)));
	inline filesystem::path coolang_project_path; //thie var shows the directory of the project, assigned in main();
	inline filesystem::path coolang_exec_path; //thie var shows the directory of the coolang_vm.exe, assigned in main();

	/************************

	memory management

	*************************/
	template <typename T, typename = void>
	struct has_fun_member_function_on_heap : std::false_type {};

	template <typename T>
	struct has_fun_member_function_on_heap<T, std::void_t<decltype(std::declval<T>().onHeap())>> : std::true_type {};



	template <typename T, typename = void>
	struct has_fun_member_function_set_on_heap : std::false_type {};

	template <typename T>
	struct has_fun_member_function_set_on_heap<T, std::void_t<decltype(std::declval<T>().onHeap())>> : std::true_type {};


	template <typename T, typename Smtptr, typename = void>
	struct has_fun_member_function_set_this_ptr : std::false_type {};

	template <typename T, typename Smtptr>
	struct has_fun_member_function_set_this_ptr<T, Smtptr,
		std::void_t<decltype(std::declval<T>().setThisPtr(std::declval<Smtptr>()))>> : std::true_type {};



	template <typename T, typename = void>
	struct has_fun_member_function_get_this_ptr : std::false_type {};

	template <typename T>
	struct has_fun_member_function_get_this_ptr<T,
		std::void_t<decltype(std::declval<T>().getThisPtr())>> : std::true_type {};

	/**
	*@brief a deleter for smart pointer, only delete the objects onheap,
	*  if T is a class and T has member function: bool onHeap(), judge if it is on heap first.
	*
	*/
	template <class T>
	struct Deleter {
		void operator()(T* ptr) const {
			if constexpr (has_fun_member_function_on_heap<T>::value) {

				if (ptr->onHeap() == true) {
					delete ptr;
				}
				else {
					return;
				}
			}
			else {
				delete ptr;
			}
		}
	};

	/*
		don't directly call this function, only customized smart ptr is allowed.

	*/
	/*template <class T, class... Args>
	std::shared_ptr<T> make_shared_ptr(Args&&... args) {
		T* t = new T(std::forward<Args>(args)...);
		if constexpr (has_fun_member_function_set_on_heap<T>::value) {
			t->setOnHeap();
		}
		return std::shared_ptr<T>(t, Deleter<T>());
	}*/




	template <typename T>
	class WeakPtr;

	template <typename T>
	class SharedPtr {
		// Declare WeakPtr as a friend class
		friend class WeakPtr<T>;

	private:
		std::shared_ptr<T> _sharedPtr;
		template < class... Args>
		static std::shared_ptr<T> make_shared_ptr(Args&&... args) {
			T* t = new T(std::forward<Args>(args)...);
			if constexpr (has_fun_member_function_set_on_heap<T>::value) {
				t->setOnHeap();
			}
			return std::shared_ptr<T>(t, Deleter<T>());
		}

		//Constructor using std::shared_ptr is banned
		SharedPtr(const std::shared_ptr<T>& ptr) : _sharedPtr(ptr) {}

	public:
		SharedPtr() : _sharedPtr(nullptr) {}


		SharedPtr(nullptr_t) : _sharedPtr(nullptr) {}

		/*
			Const and non-const version of the same pointer share the same reference counter.
		*/
		SharedPtr(const T* t);

		// Constructor using myWeakPtr
		SharedPtr(const WeakPtr<T>& weakPtr);


		//Constructor using SharedPtr
		SharedPtr(const SharedPtr<T>& return_) : _sharedPtr(return_._sharedPtr) {}

		//Move Constructor to capture return value
		SharedPtr(SharedPtr<T>&& return_) {
			if (this != &return_) {
				_sharedPtr = std::move(return_._sharedPtr);
			}
		}

		SharedPtr<T>& operator=(const SharedPtr<T>& return_) {
			this->_sharedPtr = return_._sharedPtr;
			return *this;
		}
		//move assign
		SharedPtr<T>& operator=(SharedPtr<T>&& return_) {
			if (this != &return_) {
				this->_sharedPtr = move(return_._sharedPtr);
			}
			return *this;
		}

		SharedPtr<T>& operator=(nullptr_t) {
			this->_sharedPtr.reset();
			return *this;
		}
		template < class... Args>
		static SharedPtr<T> make(Args&& ...args);

		bool operator==(const SharedPtr<T>& sharedPtr) const {
			return !(*this != sharedPtr);
		}
		bool operator!=(const SharedPtr<T>& sharedPtr) const {
			return this->_sharedPtr.owner_before(sharedPtr._sharedPtr) || sharedPtr._sharedPtr.owner_before(this->_sharedPtr);
		}
		bool operator==(const WeakPtr<T>& weakPtr) const {
			return !(*this != weakPtr);
		}
		bool operator!=(const WeakPtr<T>& weakPtr) const {
			return this->_sharedPtr.owner_before(weakPtr._weakPtr) || weakPtr._weakPtr.owner_before(this->_sharedPtr);
		}


		bool operator==(nullptr_t) const {
			return _sharedPtr == nullptr;
		}
		bool operator!=(nullptr_t) const {
			return !(_sharedPtr == nullptr);
		}

		bool operator<(const SharedPtr<T>& other) const {
			return std::less<T*>()(_sharedPtr.get(), other._sharedPtr.get());
		}


		T* operator->() const {
			return _sharedPtr.get();
		}

		T& operator*() const {
			return *_sharedPtr;
		}

		// Other member functions using default interfaces gradually added here

		T* get() const {
			return this->_sharedPtr.get();
		}
	};

	template <typename T>
	class WeakPtr {
		friend class SharedPtr<T>;
	private:
		std::weak_ptr<T> _weakPtr;

		// Constructor using std::weak_ptr is banned
		WeakPtr(const std::weak_ptr<T>& ptr) : _weakPtr(ptr) {}
	public:
		WeakPtr() : _weakPtr() {}

		WeakPtr(nullptr_t) : _weakPtr() {}

		// Constructor using SharedPtr
		WeakPtr(const SharedPtr<T>& sharedPtr) : _weakPtr(sharedPtr._sharedPtr) {}

		// Constructor using WeakPtr
		WeakPtr(const WeakPtr<T>& weakPtr) : _weakPtr(weakPtr._weakPtr) {}

		WeakPtr<T>& operator=(const WeakPtr<T>& weakPtr) {
			this->_weakPtr = weakPtr._weakPtr;
			return *this;
		}
		WeakPtr<T>& operator=(nullptr_t) {
			this->_weakPtr.reset();
			return *this;
		}

		bool operator==(const SharedPtr<T>& sharedPtr) const {
			return !(*this != sharedPtr);
		}
		bool operator!=(const SharedPtr<T>& sharedPtr) const {
			return this->_weakPtr.owner_before(sharedPtr._sharedPtr) || sharedPtr._sharedPtr.owner_before(this->_weakPtr);
		}
		bool operator==(const WeakPtr<T>& weakPtr) const {
			return !(*this != weakPtr);
		}
		bool operator!=(const WeakPtr<T>& weakPtr) const {
			return this->_weakPtr.owner_before(weakPtr._weakPtr) || weakPtr._weakPtr.owner_before(this->_weakPtr);
		}
		bool operator==(nullptr_t) const {
			return _weakPtr.expired();
		}
		bool operator!=(nullptr_t) const {
			return !_weakPtr.expired();
		}

		bool operator<(const WeakPtr<T>& other) const {
			auto spThis = _weakPtr.lock(); // Lock this weak pointer
			auto spOther = other._weakPtr.lock(); // Lock the other weak pointer
			return std::less<std::shared_ptr<T>>()(spThis, spOther);
		}


		T* operator->() const {
			std::shared_ptr<T> sharedPtr = _weakPtr.lock();
			if (sharedPtr) {
				return sharedPtr.get();
			}
			else {
				// Handle the case when the object is not alive
				return nullptr;
			}
		}

		T& operator*() const {
			std::shared_ptr<T> sharedPtr = _weakPtr.lock();
			if (sharedPtr) {
				return *sharedPtr;
			}
			else {
				// Handle the case when the object is not alive
				throw std::runtime_error("Attempted to dereference an expired weak pointer");
			}
		}

		T* get() const {
			return this->_weakPtr.lock().get();
		}



	};


	// Constructor using myWeakPtr
		// Constructor using myWeakPtr
	template <class T>
	SharedPtr<T>::SharedPtr(const WeakPtr<T>& weakPtr) : _sharedPtr(weakPtr._weakPtr.lock()) {}

	template <class T>
	template <class... Args>
	SharedPtr<T> SharedPtr<T>::make(Args&& ...args) {
		SharedPtr<T> sptr = make_shared_ptr(forward<Args>(args)...);
		if constexpr (has_fun_member_function_set_this_ptr<T, WeakPtr<T> >::value) {
			sptr->setThisPtr(sptr);
		}
		return sptr;

	}
	/*
	Const and non-const version of the same pointer share the same reference counter.
*/
	template <typename T>
	SharedPtr<T>::SharedPtr(const T* t) {

		if constexpr (has_fun_member_function_on_heap<T>::value) {
			if (t->onHeap() == true) {

				if constexpr (has_fun_member_function_get_this_ptr<T>::value) {
					SharedPtr<T> tw = t->getThisPtr();
					this->_sharedPtr = tw._sharedPtr;
				}
				else {

#if debug && _WIN32
					{
						static int ignoreCount = 0;
						// Assuming info is a std::string, convert to std::wstring
						std::string info = "Object doesn't provide interface for direct conversion to SharedPtr var raw pointer"; // Replace with actual info
						info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
						std::wstring winfo(info.begin(), info.end());
						OutputDebugString(winfo.c_str()); cout << info << endl;
						ignoreCount++;
					}
#endif

					cerr << "Object doesn't provide interface for direct conversion to SharedPtr var raw pointer" << endl;
					exception e("Object doesn't provide interface for direct conversion to SharedPtr var raw pointer");
					throw e;


				}

			}
			else {

				this->_sharedPtr = std::shared_ptr<T>(const_cast<T*>(t), Deleter<T>());
			}
			return;
		}
		else {

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "cannot directly create SharedPtr from pointer of a class without memberfunction onHeap(), exit(-1)"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
				OutputDebugString(winfo.c_str()); cout << info << endl;
				ignoreCount++;
			}
#endif
			cerr << "cannot directly create SharedPtr from pointer of a class without memberfunction onHeap(), exit(-1)" << endl;
			exit(-1);
		}
	}
	/**
	 * @brief 获取列表类容器二进制存储所需的信息list/deque
	 * @tparam ListType 容器类型,必须实现函数push_back，size；
	 * @tparam InnerListInfoType 元素对应的信息类型
	 * @tparam InnerListValueType 容器中储存的元素类型
	 */
	template<class ListType, class InnerListInfoType, class InnerListValueType>
	class ListBinaryInfo {
	public:
		Intl position = 0; //文件指针位置
		Intg listlen = 0;

		/**
		 * @attention
		 * @param list2
		 * @param fs（fs必须保证在文件输入流末端）
		 */
		Intl parseAndWriteObject(const ListType& list2, fstream& fs) {
#if debug
			{
				static int ignorecount = 0;
				std::cout << "parseAndWriteObject(const ListType&, fstream&)" << " "
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << std::endl;
			}
#endif
			this->position = fs.tellp();
			this->listlen = list2.size();
			Intl element_info_object_position = fs.tellp();
			Intl element_value_object_position;
			InnerListInfoType elei;
			for (/*const InnerListValueType */auto& elev : list2) {
				//存储位置（elei，elev）
				//写入顺序（elev，elei）

				//1 写入elev
				element_value_object_position = element_info_object_position
					+ sizeof(InnerListInfoType);
				fs.seekp(element_value_object_position, fs.beg);
				elei.parseAndWriteObject(elev, fs);

				//2 写入elei
				fs.seekp(element_info_object_position, fs.beg);
				fs.write(reinterpret_cast<char*>(&elei), sizeof(InnerListInfoType));
				fs.seekp(0, fs.end);
				element_info_object_position = fs.tellp();
			}

			return fs.tellp();

		}
		/**
		 * @attention
		 * @param list2
		 * @param fs（fs必须为文件输出流且以打开）
		 * @return Intl 已经解析到的位置。亦即，Object value在二进制文件中存储部分的末尾。
		 */
		Intl constructObject(ListType& list2, fstream& fs) {
			list2.clear();

			InnerListInfoType* eleip = new InnerListInfoType();

			if (listlen > 0) {
				fs.seekg(position, fs.beg);
			}
			for (Intg i = 0; i < listlen; ++i) {
				InnerListValueType elev = InnerListValueType();
				//1 读入elei
				fs.read(reinterpret_cast<char*>(eleip), sizeof(InnerListInfoType));

				//2 读入elev
				eleip->constructObject(elev, fs);

				list2.push_back(elev);

			}
			return fs.tellg();
		}
		/**
		 * @attention fs必须已打开，传入指针无需分配内存，此处会自动分配内存。其内存释放需在外部手动进行
		 * @param t
		 */
		void parseInfo(ListType*& containerp, fstream& fs) {
			containerp = new ListType();
			InnerListInfoType* elep = new InnerListInfoType();
			for (Intg i = 0; i < listlen; ++i) {
				fs.seekp(position + i * sizeof(InnerListInfoType));
				fs.read(reinterpret_cast<char*>(elep), sizeof(InnerListInfoType));
				containerp->push_back(*elep);
			}
			delete elep;

		}
	};
	/**
	 * @brief 获取列表类容器二进制存储所需的信息list/deque
	 * @tparam ListType 容器类型,必须实现函数push_back，size；
	 * @tparam InnerListInfoType 元素对应的信息类型
	 * @tparam InnerListValueType 容器中储存的元素类型
	 */
	template<class ListType>
	class ListBinaryInfo<ListType, Intg, Intg> {
	public:
		Intl position = 0; //文件指针位置
		Intg listlen = 0;
		/**
		 *
		 * @param position object写入的位置
		 * @param list2
		 */
		void parseObject(const Intl& position, const ListType& list2) {
			this->position = position;
			this->listlen = list2.size();

		}

		/**
		 * @attention
		 * @param list2
		 * @param fs（fs必须保证在文件输入流末端）
		 */
		Intl parseAndWriteObject(const ListType& list2, fstream& fs) {
#if debug
			{
				static int ignorecount = 0;
				std::cout << "parseAndWriteObject(const ListType&, fstream&)" << " "
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << std::endl;
			}
#endif
			this->position = fs.tellp();
			this->listlen = list2.size();

			for (const Intg& ele : list2) {
				//1 写入ele
				fs.write(reinterpret_cast<const char*>(&ele), sizeof(Intg));
			}

			return fs.tellp();

		}
		/**
		 * @attention
		 * @param list2
		 * @param fs
		 */
		Intl constructObject(ListType& list2, fstream& fs) {
			list2.clear();
			if (listlen > 0) {
				fs.seekg(position);
			}

			Intg ele = 0;
			for (Intg i = 0; i < listlen; ++i) {
				//1 写入ele
				fs.read(reinterpret_cast<char*>(&ele), sizeof(Intg));
				list2.push_back(ele);

			}
			return fs.tellg();

		}

	};
	/**
	 * @brief 获取列表类容器二进制存储所需的信息list/deque
	 * @tparam ListType 容器类型,必须实现函数push_back，size；
	 * @tparam InnerListInfoType 元素对应的信息类型
	 * @tparam InnerListValueType 容器中储存的元素类型
	 */
	template<class ListType>
	class ListBinaryInfo<ListType, Numb, Numb> {
	public:
		Intl position = 0; //文件指针位置
		Intg listlen = 0;
		/**
		 *
		 * @param position object写入的位置
		 * @param list2
		 */
		void parseObject(const Intl& position, const ListType& list2) {
			this->position = position;
			this->listlen = list2.size();

		}

		/**
		 * @attention
		 * @param list2
		 * @param fs（fs必须保证在文件输入流末端）
		 */
		Intl parseAndWriteObject(const ListType& list2, fstream& fs) {
#if debug
			{
				static int ignorecount = 0;
				std::cout << "parseAndWriteObject(const ListType&, fstream&)" << " "
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << std::endl;
			}
#endif
			this->position = fs.tellp();
			this->listlen = list2.size();

			for (const Numb& ele : list2) {
				//1 写入ele
				fs.write(reinterpret_cast<const char*>(&ele), sizeof(Numb));
			}

			return fs.tellp();

		}
		/**
		 * @attention
		 * @param list2
		 * @param fs
		 */
		Intl constructObject(ListType& list2, fstream& fs) {
			list2.clear();
			if (listlen > 0) {
				fs.seekg(position);
			}

			Intg ele = 0;
			for (Intg i = 0; i < listlen; ++i) {
				//1 写入ele
				fs.read(reinterpret_cast<char*>(&ele), sizeof(Numb));
				list2.push_back(ele);

			}
			return fs.tellg();

		}

	};
	/*
	 * extension of ListBinaryInfo
	 * info of a 2 dimension list2 (support operator[], like vector or deque)
	 * @param
	 * List2Type a<b<x>>
	 * InnerListInfoType info of b<x>
	 * InnerListValueType b<x>
	 * InnerListInfoType info of x
	 * InnerListValueType x
	 *
	 */
	template<class List2Type, class InnerListInfoType, class InnerListValueType/*,
	 class ElementInfoType, class ElementValueType*/>
	class List2BinaryInfo {

	public:
		Intl position = 0; //文件指针位置
		Intg listlen = 0;

		/**
		 * @attention
		 * @param list2
		 * @param fs（fs必须保证在文件输入流末端）
		 */
		Intl parseAndWriteObject(const List2Type& list2, fstream& fs) {
#if debug
			{
				static int ignorecount = 0;
				std::cout << "parseAndWriteObject(const List2Type&, fstream&)"
					<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
					<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
			}
#endif
			this->position = fs.tellp();
			this->listlen = list2.size();
			Intl innerList_info_object_position = fs.tellp();
			Intl innerList_value_object_position;
			InnerListInfoType inli;
			//write in all inner list of list2
			for (const InnerListValueType/*auto*/& inlv : list2) {
				//存储位置（inli，inlv）
				//写入顺序（inlv，inli）

				//1 写入inlv
				innerList_value_object_position = innerList_info_object_position
					+ sizeof(InnerListInfoType);
				fs.seekp(innerList_value_object_position, fs.beg);
				inli.parseAndWriteObject(inlv, fs);

				//2 写入inli
				fs.seekp(innerList_info_object_position, fs.beg);
				fs.write(reinterpret_cast<char*>(&inli), sizeof(InnerListInfoType));
				fs.seekp(0, fs.end);
				innerList_info_object_position = fs.tellp();
			}

			return fs.tellp();

		}
		/**
		 * @attention
		 * @param list2
		 * @param fs（fs必须为文件输出流且以打开）
		 * @return Intl 已经解析到的位置。亦即，Object value在二进制文件中存储部分的末尾。
		 */
		Intl constructObject(List2Type& list2, fstream& fs) {
			list2.clear();

			InnerListInfoType* inlip = new InnerListInfoType();

			if (listlen > 0) {
				fs.seekg(position, fs.beg);
			}
			for (Intg i = 0; i < listlen; ++i) {
				InnerListValueType inlv = InnerListValueType();
				//1 读入inli
				fs.read(reinterpret_cast<char*>(inlip), sizeof(InnerListInfoType));

				//2 读入inlv
				inlip->constructObject(inlv, fs);

				list2.push_back(inlv);

			}
			return fs.tellg();
		}
		/**
		 * @attention fs必须已打开，传入指针无需分配内存，此处会自动分配内存。其内存释放需在外部手动进行
		 * @param t
		 */
		void parseInfo(List2Type*& containerp, fstream& fs) {
			containerp = new List2Type();
			InnerListInfoType* inlp = new InnerListInfoType();
			for (Intg i = 0; i < listlen; ++i) {
				fs.seekp(position + i * sizeof(InnerListInfoType));
				fs.read(reinterpret_cast<char*>(inlp), sizeof(InnerListInfoType));
				containerp->push_back(*inlp);
			}
			delete inlp;

		}

	};
	/**
	 * @brief 获取列表类容器二进制存储所需的信息set/deque
	 * @tparam SetType 容器类型,必须实现函数insert；
	 * @tparam InnerListInfoType 元素对应的信息类型
	 * @tparam InnerListValueType 容器中储存的元素类型
	 */
	template<class SetType, class InnerListInfoType, class InnerListValueType>
	class SetBinaryInfo {
	public:
		Intl position = 0; //文件指针位置
		Intg setlen = 0;
		/**
		 *
		 * @param position object写入的位置
		 * @param set
		 */
		void parseObject(const Intl& position, const SetType& set) {
			this->position = position;
			this->setlen = set.size();

		}

		/**
		 * @attention
		 * @param set
		 * @param fs（fs必须保证在文件输入流末端）
		 */
		Intl parseAndWriteObject(const SetType& set, fstream& fs) {
#if debug
			{
				static int ignorecount = 0;
				std::cout << "parseAndWriteObject(const SetType&, fstream&)" << " "
					<< "\tignorecount:[" << ignorecount++ << "\t](" << __FILE__
					<< ":" << __LINE__ << ":0" << ")" << std::endl;
			}
#endif
			this->position = fs.tellp();
			this->setlen = set.size();
			Intl element_info_object_position = fs.tellp();
			Intl element_value_object_position;
			InnerListInfoType elei;
			for (const InnerListValueType& elev : set) {
				//存储位置（elei，elev）
				//写入顺序（elev，elei）

				//1 写入elev
				element_value_object_position = element_info_object_position
					+ sizeof(InnerListInfoType);
				fs.seekp(element_value_object_position, fs.beg);
				elei.parseAndWriteObject(elev, fs);

				//2 写入elei
				fs.seekp(element_info_object_position, fs.beg);
				fs.write(reinterpret_cast<char*>(&elei), sizeof(InnerListInfoType));
				fs.seekp(0, fs.end);
				element_info_object_position = fs.tellp();
			}

			return fs.tellp();

		}
		/**
		 * @attention
		 * @param set
		 * @param fs（fs必须已打开）
		 */
		Intl constructObject(SetType& set, fstream& fs) {
			set.clear();
			InnerListInfoType* eleip = new InnerListInfoType();

			if (setlen > 0) {
				fs.seekg(position);
			}
			Intg len = this->setlen;
			while (len-- > 0) {
				InnerListValueType elev = InnerListValueType();
				//1 读入elei

				fs.read(reinterpret_cast<char*>(eleip), sizeof(InnerListInfoType));
				eleip->constructObject(elev, fs);
				set.insert(elev);
			}
			delete eleip;

			return fs.tellg();

		}
		/**
		 * @attention fs必须已打开，传入指针无需分配内存，此处会自动分配内存。其内存释放需在外部手动进行
		 * @param t
		 */
		void parseInfo(SetType*& containerp, fstream& fs) {
			containerp = new SetType();
			InnerListInfoType* elep = new InnerListInfoType();
			for (Intg i = 0; i < setlen; ++i) {
				fs.seekp(position + i * sizeof(InnerListInfoType));
				fs.read(reinterpret_cast<char*>(elep), sizeof(InnerListInfoType));
				containerp->push_back(*elep);
			}
			delete elep;

		}
	};
	/**
	 * @attention 获得map、multimap存储二进制文件所需信息，必须实现size、insert
	 * @tparam InfoKeyType map的键对应的信息类
	 * @tparam ObjectKeyType map的键对应的数据类
	 * @tparam InfoValueType map的值对应的信息类
	 * @tparam ObjectValueType map的值对应的数据类
	 */
	template<class InfoKeyType, class ObjectKeyType, class InfoValueType,
		class ObjectValueType>
	class mapBinaryInfo {
	public:
		Intl position = 0;
		Intg maplen = 0;

		Intl parseAndWriteObject(const map<ObjectKeyType, ObjectValueType>& mapobj,
			fstream& fs) {
#if debug
				{
					static int ignorecount = 0;
					std::cout
						<< "parseAndWriteObject(const map<ObjectKeyType,ObjectValueType>&, fstream&)"
						<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
				}
#endif
				Intl object_value_output_position = fs.tellp();
				this->position = object_value_output_position;
				this->maplen = mapobj.size();
				Intl pair_key_object_info_output_position = object_value_output_position;
				Intl pair_key_object_value_output_position = 0;
				Intl pair_value_object_info_output_position = 0;
				Intl pair_value_object_value_output_position = 0;
				for (/*pair<ObjectKeyType, ObjectValueType>*/auto& opkv : mapobj) {
					//存储顺序
					InfoKeyType ik;
					const ObjectKeyType& ok = opkv.first;
					InfoValueType iv;
					const ObjectValueType& ov = opkv.second;

					//写入顺序(key's value,key's info ,value's value,value's info)

					//写入ok
					pair_key_object_value_output_position =
						pair_key_object_info_output_position + sizeof(InfoKeyType);
					fs.seekp(pair_key_object_value_output_position, fs.beg);
					pair_value_object_info_output_position = ik.parseAndWriteObject(ok,
						fs);

					//写入ik
					fs.seekp(pair_key_object_info_output_position, fs.beg);
					fs.write(reinterpret_cast<char*>(&ik), sizeof(InfoKeyType));
					fs.seekp(0, fs.end);
					pair_value_object_info_output_position = fs.tellp();

					//写入ov
					pair_value_object_value_output_position =
						pair_value_object_info_output_position
						+ sizeof(InfoValueType);
					fs.seekp(pair_value_object_value_output_position, fs.beg);
					pair_key_object_info_output_position = iv.parseAndWriteObject(ov,
						fs); //为下一轮的pair_key_object_info_output_position赋值

					//写入iv
					fs.seekp(pair_value_object_info_output_position, fs.beg);
					fs.write(reinterpret_cast<char*>(&iv), sizeof(InfoValueType));
					fs.seekp(0, fs.end); //将文件指针置于末位
#if debug
					{
						if (fs.tellp() != pair_key_object_info_output_position) {

							static int ignorecount = 0;
							cerr
								<< "parseAndWriteObject(map<ObjectKeyType,ObjectValueType>&, fstream&)"
								<< " fileByteSize error exit(-1) "
								<< "\tignorecount:[" << ignorecount++ << "\t]("
								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
								<< endl;
							exit(-1);
						}
					}
#endif

				}

				return fs.tellp();
		}
		Intl constructObject(map<ObjectKeyType, ObjectValueType>& mapobj,
			fstream& fs) {
			mapobj.clear();
			InfoKeyType* ikp = new InfoKeyType();

			InfoValueType* ivp = new InfoValueType();

			if (maplen > 0) {
				fs.seekg(position);
			}
			for (Intg i = 0; i < maplen; ++i) {
				ObjectKeyType ok = ObjectKeyType(); //必须实时刷新，否则构造的值可能残留之前的值
				ObjectValueType ov = ObjectValueType();
				//读入ikp
				fs.read(reinterpret_cast<char*>(ikp), sizeof(InfoKeyType));
				//读入ok
				ikp->constructObject(ok, fs);
				//读入 ivp
				fs.read(reinterpret_cast<char*>(ivp), sizeof(InfoValueType));
				//读入 ov
				ivp->constructObject(ov, fs);

				mapobj.insert(make_pair(ok, ov));
			}

			return fs.tellg();
		}

	};
	/**
	 * @attention 获得map、multimap存储二进制文件所需信息，必须实现size、insert
	 * @tparam InfoKeyType map的键对应的信息类
	 * @tparam ObjectKeyType map的键对应的数据类
	 * @tparam InfoValueType map的值对应的信息类
	 * @tparam ObjectValueType map的值对应的数据类
	 */
	template<class InfoKeyType, class ObjectKeyType>
	class mapBinaryInfo<InfoKeyType, ObjectKeyType, Intg,
		Intg> {
	public:
		Intl position = 0;
		Intg maplen = 0;

		Intl parseAndWriteObject(const map<ObjectKeyType, Intg>& mapobj,
			fstream& fs) {
#if debug
				{
					static int ignorecount = 0;
					std::cout
						<< "parseAndWriteObject(const map<ObjectKeyType,int>&, fstream&)"
						<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
				}
#endif
				Intl object_value_output_position = fs.tellp();
				this->position = object_value_output_position;
				this->maplen = mapobj.size();
				Intl pair_key_object_info_output_position = object_value_output_position;
				Intl pair_key_object_value_output_position = 0;
				Intl pair_value_output_position = 0;
				for (const pair<ObjectKeyType, Intg>& opkv : mapobj) {
					//存储顺序
					InfoKeyType ik;
					const ObjectKeyType& ok = opkv.first;
					const Intg& v = opkv.second;

					//写入顺序(key's value,key's info ,value's value,value's info)

					//写入ok
					pair_key_object_value_output_position =
						pair_key_object_info_output_position + sizeof(InfoKeyType);
					fs.seekp(pair_key_object_value_output_position, fs.beg);
					pair_value_output_position = ik.parseAndWriteObject(ok, fs);

					//写入ik
					fs.seekp(pair_key_object_info_output_position, fs.beg);
					fs.write(reinterpret_cast<char*>(&ik), sizeof(InfoKeyType));
					fs.seekp(0, fs.end);
					pair_value_output_position = fs.tellp();

					//写入v
					fs.seekp(pair_value_output_position, fs.beg);
					fs.write(reinterpret_cast<const char*>(&v), sizeof(Intg));
					fs.seekp(0, fs.end); //将文件指针置于末位
					pair_key_object_info_output_position = fs.tellp();

				}

				return fs.tellp();
		}
		Intl constructObject(map<ObjectKeyType, Intg>& mapobj, fstream& fs) {

			mapobj.clear();
			InfoKeyType* ikp = new InfoKeyType();

			Intg v = 0;
			if (maplen > 0) {
				fs.seekg(position);
			}

			for (Intg i = 0; i < maplen; ++i) {
				ObjectKeyType ok = ObjectKeyType();
				//读入ik
				fs.read(reinterpret_cast<char*>(ikp), sizeof(InfoKeyType));
				//读入ok
				ikp->constructObject(ok, fs);
				//读入v
				fs.read(reinterpret_cast<char*>(&v), sizeof(Intg));

				mapobj.insert(make_pair(ok, v));

			}
			delete ikp;
			return fs.tellg();
		}

	};
	/**
	 * @attention 获得map、multimap存储二进制文件所需信息，必须实现size、insert
	 * @tparam InfoKeyType map的键对应的信息类
	 * @tparam ObjectKeyType map的键对应的数据类
	 * @tparam InfoValueType map的值对应的信息类
	 * @tparam ObjectValueType map的值对应的数据类
	 */
	template<class InfoValueType, class ObjectValueType>
	class mapBinaryInfo< Intg, Intg, InfoValueType, ObjectValueType> {
	public:
		Intl position = 0;
		Intg maplen = 0;

		Intl parseAndWriteObject(const map<Intg, ObjectValueType>& mapobj,
			fstream& fs) {
#if debug
				{
					static int ignorecount = 0;
					std::cout
						<< "parseAndWriteObject(const map<int,ObjectValueType>&, fstream&)"
						<< " " << "\tignorecount:[" << ignorecount++ << "\t]("
						<< __FILE__ << ":" << __LINE__ << ":0" << ")" << std::endl;
				}
#endif
				Intl object_value_output_position = fs.tellp();
				this->position = object_value_output_position;
				this->maplen = mapobj.size();
				//基本类型不需要区分info与value
				Intl pair_key_output_position = object_value_output_position;

				Intl pair_value_object_info_output_position = 0;
				Intl pair_value_object_value_output_position = 0;
				for (const pair<Intg, ObjectValueType>& opkv : mapobj) {
					//存储顺序
					const Intg& k = opkv.first;
					InfoValueType iv;
					const ObjectValueType& ov = opkv.second;

					//写入顺序(key's value,key's info ,value's value,value's info)

					//写入k

					fs.seekp(pair_key_output_position, fs.beg);
					fs.write(reinterpret_cast<const char*>(&k), sizeof(Intg));
					pair_value_object_info_output_position = fs.tellp();

					//写入ov
					pair_value_object_value_output_position =
						pair_value_object_info_output_position
						+ sizeof(InfoValueType);
					fs.seekp(pair_value_object_value_output_position, fs.beg);
					pair_key_output_position = iv.parseAndWriteObject(ov, fs); //为下一轮的pair_key_object_info_output_position赋值

					//写入iv
					fs.seekp(pair_value_object_info_output_position, fs.beg);
					fs.write(reinterpret_cast<char*>(&iv), sizeof(InfoValueType));
					fs.seekp(0, fs.end); //将文件指针置于末位
#if debug
					{
						if (fs.tellp() != pair_key_output_position) {

							static int ignorecount = 0;
							cerr
								<< "parseAndWriteObject(map<ObjectKeyType,ObjectValueType>&, fstream&)"
								<< " fileByteSize error exit(-1) "
								<< "\tignorecount:[" << ignorecount++ << "\t]("
								<< __FILE__ << ":" << __LINE__ << ":0" << ")"
								<< endl;
							exit(-1);
						}
					}
#endif

				}

				return fs.tellp();
		}
		Intl constructObject(map<Intg, ObjectValueType>& mapobj, fstream& fs) {
			mapobj.clear();
			Intg k = 0;
			InfoValueType* ivp = new InfoValueType();

			if (maplen > 0) {
				fs.seekg(position);
			}
			for (Intg i = 0; i < maplen; ++i) {
				ObjectValueType ov = ObjectValueType();
				//读入k
				fs.read(reinterpret_cast<char*>(&k), sizeof(Intg));
				//读入iv
				fs.read(reinterpret_cast<char*>(ivp), sizeof(InfoValueType));
				//读入ov
				ivp->constructObject(ov, fs);

				mapobj.insert(make_pair(k, ov));
			}
			delete ivp;

			return fs.tellg();
		}

	};

	typedef std::string Strg;
	Strg& strgReplace(Strg& source, const Strg& pattern, const Strg& insert);

	class StrgBinaryInfo;
	class StrgBinaryInfo {
	public:
		Intl position; //文件指针位置
		int bytesize;
		Intl parseAndWriteObject(const Strg& strg, fstream& fs);
		Intl constructObject(Strg& strg, fstream& fs);
	};
	//class Strg: public string {
	//public:
	//    Strg() :
	//            string("") {
	//
	//    }
	//    Strg(const string &s) :
	//            string(s) {
	//    }
	//    Strg(const char *s) :
	//            string(s) {
	//    }
	//    Intg size() const {
	//        return this->string::size();
	//    }
	//    Strg& operator+=(const string &s) {
	//        this->string::operator+=(s);
	//        return (*this);
	//    }
	//    Strg& operator+=(const string &&s) {
	//        this->string::operator+=(s);
	//        return (*this);
	//    }
	//    Strg& operator=(const string &s) {
	//        this->string::operator =(s);
	//        return (*this);
	//    }
	//    Strg& operator=(const char *s) {
	//        this->string::operator =(s);
	//        return (*this);
	//    }
	//
	//};

	/**
	 * union two set（s）
	 * @tparam T
	 * @param argL
	 * @param argR
	 * @return
	 */
	template<typename T = Intg>
	set<T>& operator+=(set<T>& argL, set<T>& argR) {
		set<T> temp;
		set_union(argL.begin(), argL.end(), argR.begin(), argR.end(),
			inserter(temp, temp.begin()));
		argL = temp;
		return argL;

	}
	/**
	 * COOL_L = exit in COOL_L not in COOL_R
	 * @tparam T
	 * @param argL
	 * @param argR
	 * @return
	 */
	template<typename T = Intg>
	set<T>& operator-=(set<T>& argL, set<T>& argR) {
		set<T> temp;

		set_difference(argL.begin(), argL.end(), argR.begin(), argR.end(),
			insert_iterator<set<T>>(temp, temp.begin()));
		argL = temp;
		return argL;
	}
	/**
	 * COOL_L = exit in COOL_L && in COOL_R
	 * @tparam T
	 * @param argL
	 * @param argR
	 * @return
	 */
	template<typename T = Intg>
	set<T>& operator^=(set<T>& argL, const set<T>& argR) {
		set<T> temp;
		set_intersection(argL.begin(), argL.end(), argR.begin(), argR.end(),
			insert_iterator<set<T>>(temp, temp.begin()));
		argL = temp;
		return argL;
	}

	vector<Strg> split(const Strg& str_, const Strg& pattern);

	template <typename T, typename = void>
	struct has_fun_member_function_toStrg : std::false_type {};

	template <typename T>
	struct has_fun_member_function_toStrg<T, std::void_t<decltype(std::declval<T>().toStrg())>> : std::true_type {};


	//Check for operator*() and T*


	template <typename T, typename = void>
	struct has_dereference : std::false_type {};

	// Specialization for types with operator*()
	template <typename T>
	struct has_dereference<T, std::void_t<decltype(std::declval<T>().operator*())>> : std::true_type {};

	// Additional check for pointer types
	template <typename T>
	struct is_pointer : std::false_type {};

	template <typename T>
	struct is_pointer<T*> : std::true_type {};

	// Combined trait for checking both conditions
	template <typename T>
	struct has_dereference_or_is_pointer : std::integral_constant<bool, has_dereference<T>::value || is_pointer<T>::value> {};

	Intg toIntg(Strg s);

	Numb toNumb(Strg s);

	Strg toStrg(Intg i);

	Strg toStrg(Numb num);

	template<typename key_type, typename value_type>
	Strg toStrg(const map<key_type, value_type>& map) {
		stringstream ss;
		ss.precision(PRECISION);
		int i = 0;
		ss << "{";
		for (const auto& it : map) {
			if (i++ != 0) {
				ss << ", ";
			}
			if constexpr (has_dereference_or_is_pointer<key_type>::value) {

				if constexpr (has_fun_member_function_toStrg<decltype(*it.first)>::value) {
					ss << it.first->toStrg();
				}
				else {
					ss << COOLANG::toStrg(*it.first);
				}

			}
			else {

				if constexpr (has_fun_member_function_toStrg<key_type>::value) {
					ss << it.first.toStrg();
				}
				else {
					ss << COOLANG::toStrg(it.first);
				}
			}

			ss << ":";

			if constexpr (has_dereference_or_is_pointer<value_type>::value) {
				if constexpr (has_fun_member_function_toStrg<decltype(*it.second)>::value) {
					ss << it.second->toStrg();
				}
				else {
					ss << COOLANG::toStrg(*it.second);
				}
			}
			else {

				if constexpr (has_fun_member_function_toStrg<value_type>::value) {
					ss << it.second.toStrg();
				}
				else {
					ss << COOLANG::toStrg(it.second);
				}


			}
		}
		ss << "}";
		return ss.str();

	}
	template<typename key_type, typename value_type, typename comparator>
	Strg toStrg(const map<key_type, value_type, comparator>& map) {
		stringstream ss;
		ss.precision(PRECISION);
		int i = 0;
		ss << "{";
		for (const auto& it : map) {
			if (i++ != 0) {
				ss << ", ";
			}
			if constexpr (has_dereference_or_is_pointer<key_type>::value) {

				if constexpr (has_fun_member_function_toStrg<decltype(*it.first)>::value) {
					ss << it.first->toStrg();
				}
				else {
					ss << COOLANG::toStrg(*it.first);
				}

			}
			else {

				if constexpr (has_fun_member_function_toStrg<key_type>::value) {
					ss << it.first.toStrg();
				}
				else {
					ss << COOLANG::toStrg(it.first);
				}
			}

			ss << ":";

			if constexpr (has_dereference_or_is_pointer<value_type>::value) {
				if constexpr (has_fun_member_function_toStrg<decltype(*it.second)>::value) {
					ss << it.second->toStrg();
				}
				else {
					ss << COOLANG::toStrg(*it.second);
				}
			}
			else {

				if constexpr (has_fun_member_function_toStrg<value_type>::value) {
					ss << it.second.toStrg();
				}
				else {
					ss << COOLANG::toStrg(it.second);
				}


			}
		}
		ss << "}";
		return ss.str();

	}
	template<typename key_type, typename value_type>
	Strg toStrg(const multimap<key_type, value_type>& map) {
		stringstream ss;
		ss.precision(PRECISION);
		int i = 0;
		ss << "{";
		for (const auto& it : map) {
			if (i++ != 0) {
				ss << ", ";
			}
			if constexpr (has_dereference_or_is_pointer<key_type>::value) {

				if constexpr (has_fun_member_function_toStrg<decltype(*it.first)>::value) {
					ss << it.first->toStrg();
				}
				else {
					ss << COOLANG::toStrg(*it.first);
				}

			}
			else {

				if constexpr (has_fun_member_function_toStrg<key_type>::value) {
					ss << it.first.toStrg();
				}
				else {
					ss << COOLANG::toStrg(it.first);
				}
			}

			ss << ":";

			if constexpr (has_dereference_or_is_pointer<value_type>::value) {
				if constexpr (has_fun_member_function_toStrg<decltype(*it.second)>::value) {
					ss << it.second->toStrg();
				}
				else {
					ss << COOLANG::toStrg(*it.second);
				}
			}
			else {

				if constexpr (has_fun_member_function_toStrg<value_type>::value) {
					ss << it.second.toStrg();
				}
				else {
					ss << COOLANG::toStrg(it.second);
				}


			}
		}
		ss << "}";
		return ss.str();
	}
	template<typename List>
	Strg toStrg(const List& lst) {
		stringstream ss;
		ss.precision(PRECISION);
		int i = 0;
		ss << "{";
		for (const auto& it : lst) {
			if (i++ != 0) {
				ss << ", ";
			}
			if constexpr (has_dereference_or_is_pointer<decltype(it)>::value) {

				if constexpr (has_fun_member_function_toStrg<decltype(*it)>::value) {
					ss << it->toStrg();
				}
				else {
					ss << COOLANG::toStrg(*it);

				}

			}
			else {

				if constexpr (has_fun_member_function_toStrg<decltype(it)>::value) {
					ss << it.toStrg();
				}
				else {
					ss << COOLANG::toStrg(it);

				}


			}
		}
		ss << "}";
		return ss.str();

	}
	class CodeTableHash;
	class ScopeTableHash;
	class TemplateFunctionTableHash;
	class SystemTableHash;
	class CodeTableHash;
	class CodeTable;
	class Code;
	class CodeBinaryInfo;
	class Addr;

	class Addr {
	public:

		vector<Intg> addrv;
		Addr() {}
		Addr(Strg addrStrg);

		Addr(Intg addri);

		Intg size() const;

		Addr(vector<Intg> addr_);


		Addr& operator=(const Addr& addr_);

		Addr& operator=(const Intg addri);

		Addr& operator=(const Strg& addrs);


		Addr createNext(Intg offset) const;

		Addr createNext() const;

		Addr createSub(const Addr& suffix) const;

		Addr createSub(const Strg& suffix) const;

		Addr createSub() const;

		bool operator==(const Addr& addr_) const;

		bool operator!=(const Addr& addr_) const;

		bool operator==(const Intg addri) const;

		bool operator==(const Strg& addrs) const;

		bool operator<(const Addr& addr_) const;

		bool operator<=(const Addr& addr_) const;

		bool operator>=(const Addr& addr_) const;

		bool operator>(const Addr& addr_) const;

		/**
		 * @name operator++()
		 * @brief ==createNext
		 * @return
		 */
		Addr& operator++();

		/**
		 * @name operator++(int)
		 * @return
		 */
		Addr operator++(int);

		/**
		 * @name operator+(Intg offset)
		 * @brief ==createNext()
		 * @return
		 */
		Addr operator+(Intg offset) const;

		Intg operator[](Intg pos) const;

		Strg toStrg() const;


	};
	inline Addr addrnull = Addr();
	inline Addr addrmax = Addr(/*INT32_MAX*/INT_MAX);
	class AddrBinaryInfo {
	public:
		Intl position;
		Intg bytesize;
		Intl parseAndWriteObject(const Addr& addr, fstream& fs);

		Intl constructObject(Addr& addr, fstream& fs);

	};
	class FileKeyAddr;

	/**
	 * @brief 这个类用于展示具有文件名标识的地址
	 */
	class FileKeyAddr {
	public:
		Strg fileKey;
		Addr addr;
		//此函数仅仅提供一个排序方式，比较无实际意义。如要比较，请定义新的函数
		bool operator>(const FileKeyAddr& fka) const;

		//此函数仅仅提供一个排序方式，比较无实际意义。如要比较，请定义新的函数
		bool operator<(const FileKeyAddr& fka) const;

		//fka的addr为addrnull或addrmax时，忽略fileKey的影响
		bool operator==(const FileKeyAddr& fka) const;

		bool operator!=(const FileKeyAddr& fka) const;

		/**
		 * @brief greaterThan 当fileKey在CodeTableHash中位置靠后时返回真，否则返回假
		 * @attention 当addr为addrnull时，fka最小；为addrmax时，fka最大
		 * @param cdth
		 * @param fka_
		 * @return
		 */
		bool GT(CodeTableHash* cdth, const FileKeyAddr& fka_) const;

		bool EQ(CodeTableHash* cdth, const FileKeyAddr& fka_) const;

		bool LT(CodeTableHash* cdth, const FileKeyAddr& fka_) const;

		bool GE(CodeTableHash* cdth, const FileKeyAddr& fka_) const;

		bool LE(CodeTableHash* cdth, const FileKeyAddr& fka_) const;

		FileKeyAddr() {}
		FileKeyAddr(const Strg& fileKey, const Addr& addr);

		FileKeyAddr& operator=(const FileKeyAddr& fka);

		Strg toStrg() const;


	};
	typedef FileKeyAddr FKA;
	inline FKA fkanull = FKA();
	inline FKA fkamax = FKA(FILEKEYEND, INT_MAX);
	class FKABinaryInfo {
	public:
		StrgBinaryInfo fileKeyInfo;
		AddrBinaryInfo addrInfo;
		Intl parseAndWriteObject(const FileKeyAddr& fka, fstream& fs);
		Intl constructObject(FileKeyAddr& fka, fstream& fs);
	};

	class FileKeyID;

	/**
	 * @brief 这个类用于展示附带文件名称的ID
	 */
	class FileKeyID {
	public:
		Strg fileKey;
		Intg ID = 0;
		//此函数仅仅提供一个排序方式，比较无实际意义。如要比较，请定义新的函数

		bool operator>(const FileKeyID& FKI) const;

		//此函数仅仅提供一个排序方式，比较无实际意义。如要比较，请定义新的函数
		bool operator<(const FileKeyID& FKI) const;

		//若fileKey == "" || fileKey == FILEKEYEND，判定相等
		bool operator==(const FileKeyID& FKI) const;

		bool operator!=(const FileKeyID& FKI) const;

		/**
		 * @brief greaterThan 当fileKey在CodeTableHash中位置靠后时返回真，否则返回假
		 * @param cdth
		 * @param FKI_
		 * @attention ""<FILEKEYBUILTIN
		 * @return
		 */
		bool GT(CodeTableHash* cdth, const FileKeyID& FKI_) const;

		bool GE(CodeTableHash* cdth, const FileKeyID& FKI_) const;

		bool EQ(CodeTableHash* cdth, const FileKeyID& FKI_) const;

		bool LT(CodeTableHash* cdth, const FileKeyID& FKI_) const;

		bool LE(CodeTableHash* cdth, const FileKeyID& FKI_) const;

		/**
		 * @brief self increase
		 * @param cdth
		 * @param FKI_
		 * @return 自增后的fki
		 */
		FileKeyID& SI(CodeTableHash* cdth);
		/**
		 * @brief self decrease
		 * @param cdth
		 * @param FKI_
		 * @return 自减后的fki
		 */
		FileKeyID& SD(CodeTableHash* cdth);

		FileKeyID();

		FileKeyID(const Strg& fileKey, const Intg& ID);

		FileKeyID& operator=(const FileKeyID& FKI);

		Strg toStrg() const;

	};
	typedef FileKeyID FKI;
	inline  FKI fkinull = FKI();
	inline FKI fkimax = FKI(FILEKEYEND, INT_MAX);
	class FKIBinaryInfo {
	public:
		StrgBinaryInfo fileKeyInfo;
		Intg ID = 0;
		Intl parseAndWriteObject(const FileKeyID& fki, fstream& fs);
		Intl constructObject(FileKeyID& fki, fstream& fs);
	};
	/**
	 * @name split 返回[min,max)(n==1)或(min,max)(n>1)
	 * @param min
	 * @param max
	 * @param n
	 * @return
	 */
	vector<Addr> split(const Addr& min, const Addr& max, const Intg n);
	class AssemblyFormula;
	class Arg;
	class DataTable;
	typedef SharedPtr<DataTable> DataTablePtr;

	typedef WeakPtr<DataTable> DataTablePtrW;

	class CodeTableHash;
	class CodeTableBinaryInfo;

	class AddrList;
	class AssemblyFormula;
	class FlagBit;
	class Quaternion;
	class ResultProperty;

#define Ints int
	class ConstraintArg {
	public:
		Intg argFlag = 0;//type
		Numb arg_i = 0;
		Strg arg_s;
		FKA arg_fka;
		Strg toStrg() const;

		//complete equal, by element.
		bool operator==(const ConstraintArg& constr_arg) const;

		bool operator!=(const ConstraintArg& constr_arg) const;

		ConstraintArg& operator=(const ConstraintArg& constr_arg);

		bool isImmediate() const;

		bool operator<(const ConstraintArg& constr_arg) const;

		bool operator<=(const ConstraintArg& constr_arg) const;

		bool operator>(const ConstraintArg& constr_arg) const;

		bool operator>=(const ConstraintArg& constr_arg) const;

		ConstraintArg() = default;
		inline ConstraintArg(Intg argflag, Strg args) :argFlag(argflag), arg_s(args) {};


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


	};

	class ConstraintArgBinaryInfo {
	public:
		Intg argFlag = 0;//type
		Numb arg_i = 0;
		StrgBinaryInfo arg_s_info;
		FKABinaryInfo arg_fka_info;
		Intl parseAndWriteObject(const ConstraintArg& fka, fstream& fs);
		Intl constructObject(ConstraintArg& fka, fstream& fs);
	};

	class Constraint {
	public:
		Strg instruction;
		deque<ConstraintArg> args;
		Strg toStrg() const;

		//complete consistant, by member element.
		bool operator==(const Constraint& constr) const;

		Constraint& operator=(const Constraint& constr);
		Constraint(const Strg& instruction = "", const deque<ConstraintArg>& args = {});

		//provided for sort
		bool operator<(const Constraint& c) const;


	};
	class ConstraintBinaryInfo {
	public:
		StrgBinaryInfo instruction_info;
		ListBinaryInfo<deque<ConstraintArg>, ConstraintArgBinaryInfo, ConstraintArg> args_info;
		Intl parseAndWriteObject(const Constraint& constr, fstream& fs);
		Intl constructObject(Constraint& constr, fstream& fs);
	};

	class FlagBit {
	public:
		Ints execute = T_;

		//   Ints forward = 0;

		Ints type = 0;

		//Ints argFlag = 0;



		//    Ints operationOrderFlag = 0;

		//    Ints resultFlag = 0;

		Intg arg1_flag = 0;
		Intg arg2_flag = 0;
		Intg operator_flag = 0;
		Intg result_flag = 0;
		
		bool asc_locked = false;


		bool functionHandleFlag = false;

		bool formalArgFlag[4] = { T_, T_, T_, T_ };

		Intg changeable[4] = { F_, F_, F_, F_ };

		Intg unknown[4] = { F_, F_, F_, F_ };

		bool compatible[4] = { F_, F_, F_, F_ };



		set<Constraint> arg1_constraints;
		set<Constraint> arg2_constraints;
		set<Constraint> operator_constraints;
		set<Constraint> result_constraints;


		FKA a1s;
		FKA a2s;
		FKA ops;
		FKA res;
		FKA tfFKA;

		void addConstraint(Intg pos, const Constraint& constr);

		FlagBit& operator=(const FlagBit& other);

		Strg toStrg() const;

	};
	class FlagBitBinaryInfo {
	public:
		Intg execute;
		Ints type;
		//Ints argFlag;
		Intg arg1_flag = 0;
		Intg arg2_flag = 0;
		Intg operator_flag = 0;
		Intg result_flag = 0;
		bool asc_locked = false;


		bool functionHandleFlag = false;
		bool formalArgFlag[4] = { T_, T_, T_, T_ };
		Intg changeable[4] = { F_, F_, F_, F_ };
		Intg unknown[4] = { F_, F_, F_, F_ };
		bool compatible[4] = { F_, F_, F_, F_ };

		SetBinaryInfo<set<Constraint>, ConstraintBinaryInfo, Constraint> arg1_constraints_info;
		SetBinaryInfo<set<Constraint>, ConstraintBinaryInfo, Constraint> arg2_constraints_info;
		SetBinaryInfo<set<Constraint>, ConstraintBinaryInfo, Constraint> operator_constraints_info;
		SetBinaryInfo<set<Constraint>, ConstraintBinaryInfo, Constraint> result_constraints_info;


		FKABinaryInfo a1sInfo;
		FKABinaryInfo a2sInfo;
		FKABinaryInfo opsInfo;
		FKABinaryInfo resInfo;
		FKABinaryInfo tfFKAInfo;
		Intl parseAndWriteObject(const FlagBit& flagBit, fstream& fs);

		Intl constructObject(FlagBit& flagBit, fstream& fs);

	};

	class Quaternion {
	public:
		Numb arg1_i = 0;

		FKA arg1_fka;

		Strg arg1_s;

		Intg arg1_symbolID = 0;

		DataTablePtr ref1_ar = nullptr;

		Numb arg2_i = 0;

		FKA arg2_fka;

		Strg arg2_s;

		Intg arg2_symbolID = 0;

		DataTablePtr ref2_ar = nullptr;

		Numb operator_i = 0;

		FKA operator_fka;

		Strg operator_s;

		Intg operator_symbolID = 0;

		DataTablePtr refoperator_ar = nullptr;

		Numb result_i = 0;

		FKA result_fka;

		Strg result_s;

		Intg result_symbolID = 0;

		DataTablePtr refresult_ar = nullptr;

		Quaternion& operator=(const Quaternion& other);

		Strg toStrg() const;

	};
	class QuaternionBinaryInfo {
	public:
		Numb arg1_i = 0;

		FKABinaryInfo arg1_fka_info;

		StrgBinaryInfo arg1_s_info;

		//    DataTable *ref1_ar = nullptr;//无效（在不会使用）

		Numb arg2_i = 0;

		FKABinaryInfo arg2_fka_info;

		StrgBinaryInfo arg2_s_info;

		//    DataTable *ref2_ar = nullptr;

		Numb operator_i = 0;

		FKABinaryInfo operator_fka_info;

		StrgBinaryInfo operator_s_info;

		//    DataTable *refoperator_ar = nullptr;

		Numb result_i = 0;

		FKABinaryInfo result_fka_info;

		StrgBinaryInfo result_s_info;

		//    DataTable *refresult_ar = nullptr;
		Intl parseAndWriteObject(const Quaternion& quat, fstream& fs);

		Intl constructObject(Quaternion& quat, fstream& fs);

	};
	class Property;
	class PropertyList;
	//PropertyList getPropertyList(const CodeTable* cdt, Addr codeAddr, Intg pos); //replaced by constraint

	class AssemblyFormula;

	class AssemblyFormula {
	public:
		FlagBit flagBit;
		Quaternion quaternion;
		AssemblyFormula& operator=(const AssemblyFormula& other);

		Strg toStrg() const;
	};
	inline AssemblyFormula asfnull = AssemblyFormula();
	class AssemblyFormulaBinaryInfo {
	public:
		FlagBitBinaryInfo flagBitInfo;
		QuaternionBinaryInfo quaternionInfo;
		Intl parseAndWriteObject(const AssemblyFormula& asf, fstream& fs);

		Intl constructObject(AssemblyFormula& asf, fstream& fs);

	};
	class Scope;
	class Code {
	public:
		Strg fileKey;
		FKA scopeFKA;
		AssemblyFormula assemblyFormula;
		Code() {		}
		Code(const Strg& locateFileKey, const FKA& scopeFKA_,
			const AssemblyFormula& assemblyFormula_);

		Code& null();
		Code& operator=(const Code& code_);
		Arg operator[](const Intg& pos) const;
		bool includeArg(const Arg& arg, acm mode) const;
		Strg toStrg() const;
		Strg toBrief() const;
		/**
		 * @brief 将code的指定位置（0,1,2,3）的相应参数的“参数类型”、“参数的值”替换为arg的对应值，其他属性不变
		 * @param position
		 * @param arg
		 */
		void setArg(const Intg& position, const Arg& arg, bool setFlagBit, bool setArgScope = false);
		void replaceFileKey(const Strg& fileKeyOrg, const Strg& fileKeyNew);

	};
	class CodeBinaryInfo;
	typedef tuple<CodeTable, Addr, Addr>  OpenAddrCDT; //codetable, open lowbound of codetable, open high bound of codetable, like (codetable)
	typedef tuple<CodeTable, FKA, FKA>  OpenFKACDT;
	class CodeTable {
	public:
		Strg fileKey;
		Intg codeForm = 0; //0,1（源代码），2（kc），3（cb）//表明代码形式
		bool completed = true; //0无效（不完整）（起占位作用）,1有效（这就是源代码表）
		deque<Strg> dependentCdtFileKey_CodeForm_0or1_List;
		deque<Strg> dependentCdtFileKey_CodeForm_2_List;
		deque<Strg> dependentCdtFileKey_CodeForm_3_List;
		deque<Addr> addrdq;
		deque<Code> codedq;
		CodeTableHash* cdth = nullptr;
		Addr getCodeAddrByOffset(const Addr& addr, const Intg offset) const;
		Addr getNextCodeAddr(const Addr& addr) const;
		Addr getAboveCodeAddr(const Addr& addr) const;
		const Intg InsertCode(const Intg position, const Intg offset,
			const Addr& addr, const Code& code);
		const Intg eraseCode(const Intg position, const Intg offset);
		const Intg eraseCode(const Addr& addr, const Intg offset);
		const FKA getScopeFKA(const Addr& codeAddr) const;
		AssemblyFormula& getAssemblyFormula(const Addr& codeAddr);
		/*const bool compareAssemblyFormula(const Addr& codeAddr) const;
		const bool compareAssemblyFormula(
			const AssemblyFormula& assemblyFormula) const;*/
		const bool sameScope(const Addr& addr1, const Addr& addr2) const;
		const bool addCode(const Addr& codeAddr, const Code& code);
		Code& operator[](const Addr& addr);
		Code& operator[](const Intg& pos);
		Addr getLast(const Intg& type, const Addr& ad) const;
		Addr getLast(const vector<Intg>& types, const Addr& ad) const;
		Addr getLastNot(const Intg& type, const Addr& ad) const;
		Addr getLastNot(const vector<Intg>& types, const Addr& ad) const;
		Addr getNextNot(const Intg& type, const Addr& ad) const;
		Addr getNextNot(const vector<Intg>& types, const Addr& ad) const;
		Addr getNext(const std::vector<Intg>& types, const Addr& ad) const;
		Addr getNext(const Intg& type, const Addr& ad) const;
		Addr getSameTrBranchLastArgAddr(const Addr& ad, const Arg& arg, acm mode);
		Addr getSameTrBranchNextArgAddr(const Addr& ad, const Arg& arg, acm mode);
		Addr getSameTrBranchLastArgAddrIgnoreProperty(const Addr& ad,
			const Arg& arg, acm mode);
		pair<Addr, Intg> getSameTrBranchLastArgPositionIgnoreProperty(
			const Addr& ad, const Arg& arg, acm mode);
		pair<Addr, Intg> getNextArgPosition(const Addr& ad, const Arg& arg, acm mode);
		Addr getSameTrBranchNextArgAddrIgnoreProperty(const Addr& ad,
			const Arg& arg, acm mode);
		OpenAddrCDT copyTree(const Addr& ad) const;
		void setRefArgAR(DataTablePtr ar);
		/// <summary>
		/// check and fix potential conflictions in attribution changeable in the codetable
		/// </summary>
		void checkChangeable();
		void checkReal();
		void checkCompatible();
		set<Addr> checkConstraints(bool allow_immediate_var = false);
		void checkFileKey(const Strg& filekey_ = "");
		pair<Constraint, set<Addr> > extractConstraint(const Addr& constraintAddr);
		/**
		 * @attention copy[adlow,adUp]
		 * @param adLow
		 * @param adUp
		 * @return
		 */
		OpenAddrCDT copyCodeTable(const Addr& adLow, const Addr& adUp) const;
		OpenAddrCDT copyCodeTableIgnoreFILLIN(const Addr& adLow,
			const Addr& adUp) const;
		OpenAddrCDT copyTreeBranch(const Addr& ad, acm mode = acm::name | acm::asc) const;
		map<FKA, FKA> resetFKARange(const Addr& min, const Addr& max,
			const Strg& fileKey);
		void insertCodeTable(const Addr& insertAddr,
			const CodeTable& codeTableInsert);
		/**
		 * @brief 删除指定间距内的所有Code，即[min+offset,max+offset]
		 * @param eraseAddrMin
		 * @param offsetRight1
		 * @param eraseAddrMax
		 * @param offsetRight2
		 */
		void eraseCode(const Addr& eraseAddrMin, Intg offsetRight1,
			const Addr& eraseAddrMax, Intg offsetRight2);
		void appendCodeTable(const Addr& appendAddr, CodeTable& codeTableAppend);
		void appendCodeTable(CodeTable& codeTableAppend);
		pair<Addr, Addr> unbindFunction(const Addr& addr);
		/**
		 * @brief 将CodeTable中所有的argOld替换成argNew
		 * @param argOld
		 * @param argNew
		 */
		void replaceVar(const Arg& argOld, const Arg& argNew, acm mode);
		/**
		 * 复制ad所在区域的函数调用
		 * @param ad
		 * @return
		 */
		OpenAddrCDT copyFunctionCall(const Addr& ad);
		Strg toStrg() const;
		Intg size() const;

		CodeTable& operator=(const CodeTable& cdt);
		bool find(const Addr& addr);
		Intg count(const Addr& addr);
		/**
		 * @brief 重新设置codeTable中和argset有关的变量的可变属性。无视所有标志位
		 * @param arg
		 * @param changeable true(设置为可变) false（设置为不可变）
		 * @attention argset的changeable属性无效
		 */
		void resetChangeable(bool changeable, const set<Arg>& argset);

		/**
		 * @brief 此函数用于将语法树中不被访问到的子树删除。
		 * 删除的原则是：如果一行代码的四元式结果参数位在语法树中不被访问到，那么就删除此行代码
		 * @param cdt
		 */
		void deleteInaccessibleSubtree();

		void unfoldRingStruct();
		/// <summary>
		/// whether two codeTables are the same (fka, scope, code)
		/// </summary>
		/// <param name="other_"></param>
		/// <returns></returns>
		bool operator==(const CodeTable& other_) const;
		inline bool operator!=(const CodeTable& other_) const {
			return !(*this == other_);
		}



		CodeTable() {		}
	};
	class CodeTableBinaryInfo {
	public:
		StrgBinaryInfo fileKeyInfo;
		Intg codeForm;
		bool completed;
		ListBinaryInfo<deque<Strg>, StrgBinaryInfo, Strg> dependentCdtFileKey_CodeForm_0or1_List_info;
		ListBinaryInfo<deque<Strg>, StrgBinaryInfo, Strg> dependentCdtFileKey_CodeForm_2_List_info;
		ListBinaryInfo<deque<Strg>, StrgBinaryInfo, Strg> dependentCdtFileKey_CodeForm_3_List_info;
		ListBinaryInfo<deque<Addr>, AddrBinaryInfo, Addr> addrdqinfo;
		ListBinaryInfo<deque<Code>, CodeBinaryInfo, Code> codedqinfo;
		Intl parseAndWriteObject(const CodeTable& cdt, fstream& fs);

		Intl constructObject(CodeTable& cdt, fstream& fs);

	};

	class CodeTableHashBinaryInfo;
	/// <summary>
	/// CodeTableHash is not allowed to add dummy files' codetable, including "built-in".
	/// </summary>
	class CodeTableHash {

	public:
		map<Strg, CodeTable> file_cdt_map;
		map<Addr, Strg> addr_file_map;
		map<Strg, Addr> file_addr_map;
		Addr getLastCdtAddr(const Addr& addr);
		Addr getNextCdtAddr(const Addr& addr);
		Strg getLastCdtFileKey(const Strg& filekey) const;
		Strg getNextCdtFileKey(const Strg& filekey) const;
		Addr insertCdt(CodeTable& cdt, const Addr& currentAddr, const bool& front);
		Addr appendCdt(CodeTable& cdt);
		vector<Strg> getDependentFileKeyFileStem();
		Strg toStrg() const;

		CodeTable& operator[](const Intg& pos);
		CodeTable& operator[](const Addr& addr);
		Code& operator[](const FKA& fka);
		Code& operator[](const FKI& fki);
		CodeTable& operator[](const Strg& fileKey);
		Intg size() const;

		bool find(const FKA& fka);


		//以下功能为CodeTable功能向cdth的拓展
		FKA getCodeFKAByOffset(const FKA& fka, const Intg offset) const;
		FKA getNextCodeFKA(const FKA& fka) const;
		FKA getAboveCodeFKA(const FKA& fka) const;
		FKI InsertCode(const FKI& position, const Intg offset, const FKA& fka,
			const Code& code);
		FKI eraseCode(const FKI& position, const Intg offset);
		FKI eraseCode(const FKA& fka, const Intg offset);
		void eraseCdt(const Strg& fileKey);
		FKA getScopeFKA(const FKA& codeFKA) const;
		AssemblyFormula& getAssemblyFormula(const FKA& codeFKA);
		/*const bool compareAssemblyFormula(const FKA& codeFKA) const;
		const bool compareAssemblyFormula(
			const AssemblyFormula& assemblyFormula) const;*/
		const bool sameScope(const FKA& fka1, const FKA& fka2) const;
		const bool addCode(const FKA& codeFKA, const Code& code);

		FKA getLast(const Intg& type, const FKA& fka) const;
		FKA getLast(const vector<Intg>& types, const FKA& fka) const;
		FKA getLastNot(const Intg& type, const FKA& fka) const;
		FKA getLastNot(const vector<Intg>& types, const FKA& fka) const;
		FKA getNextNot(const Intg& type, const FKA& fka) const;
		FKA getNextNot(const vector<Intg>& types, const FKA& fka) const;
		FKA getNext(const std::vector<Intg>& types, const FKA& fka) const;
		FKA getNext(const Intg& type, const FKA& fka) const;
		FKA getMaxFKA() const;
		FKA getMinFKA() const;
		FKI getMaxFKI() const;
		FKI getMinFKI() const;
		FKA getFKAByFKI(const FKI& fki) const;
		FKI getFKIByFKA(const FKA& fka) const;
		FKA getSameTrBranchLastArgFKA(const FKA& fka, const Arg& arg, acm mode);
		FKA getSameTrBranchNextArgFKA(const FKA& fka, const Arg& arg, acm mode);
		FKA getSameTrBranchLastArgFKAIgnoreProperty(const FKA& fka, const Arg& arg, acm mode);
		FKA getSameTrBranchNextArgFKAIgnoreProperty(const FKA& fka, const Arg& arg, acm mode);
		pair<FKA, Intg> getSameTrBranchLastArgPositionIgnoreProperty(const FKA& fka,
			const Arg& arg, acm mode);
		pair<FKA, Intg> getNextArgPosition(const FKA& fka, const Arg& arg, acm mode);
		OpenFKACDT copyTree(const FKA& fka) const;
		void setRefArgAR(DataTablePtr ar);

		/**
		 * @attention copy[adlow,adUp]
		 * @param adLow
		 * @param adUp
		 * @return
		 */
		OpenFKACDT copyCodeTable(const FKA& fkaLow, const FKA& fkaUp) const;
		OpenFKACDT copyTreeBranch(const FKA& fka, acm mode = acm::name | acm::asc) const;
		//map<FKA, FKA> resetFKARange(const FKA& min, const FKA& max);
		void insertCodeTable(const FKA& insertAddr,
			const CodeTable& codeTableInsert);
		/**
		 * @brief 删除指定间距内的所有Code，即[min+offset,max+offset]
		 * @param eraseAddrMin
		 * @param offsetRight1
		 * @param eraseAddrMax
		 * @param offsetRight2
		 */
		void eraseCode(const FKA& eraseFKAMin, Intg offsetRight1,
			const FKA& eraseFKAMax, Intg offsetRight2);
		void appendCodeTableToCdt(const FKA& appendFKA, CodeTable& codeTableAppend);
		void appendCodeTableToLastCdt(CodeTable& codeTableAppend);
		pair<FKA, FKA> unbindFunction(const FKA& FKA);
		/**
		 * @brief 将CodeTable中所有的argOld替换成argNew
		 * @param argOld
		 * @param argNew
		 */
		void replaceVar(const Arg& argOld, const Arg& argNew, acm mode);
		void resetChangeable(bool changeable, const set<Arg>& argset);

		/**
		 * 复制ad所在区域的函数调用
		 * @param ad
		 * @return
		 */
		OpenFKACDT copyFunctionCall(const FKA& fka);
		//Intg count(const FKA& fka);
	};
	class CodeTableHashBinaryInfo {
	public:
		mapBinaryInfo<StrgBinaryInfo, Strg, CodeTableBinaryInfo, CodeTable> file_cdt_map_info;
		mapBinaryInfo<AddrBinaryInfo, Addr, StrgBinaryInfo, Strg> addr_file_map_info;
		mapBinaryInfo<StrgBinaryInfo, Strg, AddrBinaryInfo, Addr> file_addr_map_info;
		Intg parseAndWriteObject(const CodeTableHash& cdth, fstream& fs);

		Intg constructObject(CodeTableHash& cdth, fstream& fs);

	};

	Addr link(const Addr& addr1, const Addr& addr2);

	Addr link(const Addr& addr1, const Intg& addr2);

	template<typename T>
	bool push_back(vector<T>& arr1, const vector<T> arr2) {

		Intg s = arr2.size();
		Intg pos = 0;
		while (pos <= s) {
			arr1.push_back(arr2[pos]);
		}
		return true;
	}




	inline Code codenull = Code();
	class CodeBinaryInfo {
	public:
		StrgBinaryInfo fileKeyInfo;
		FKABinaryInfo scopeFKAInfo;
		AssemblyFormulaBinaryInfo assemblyFormulaInfo;
		Intl parseAndWriteObject(const Code& code, fstream& fs);

		Intl constructObject(Code& code, fstream& fs);

	};



	template<typename Array_, typename Addr_>
	const Intg getAddrPosition(const Array_& arr, const Addr_& addr) {
		if (arr.size() <= 0) {

			{
				static int ignorecount = 0;
				std::cerr << "getAddrPosition(const Array_ &arr, const Addr_ &addr)"
					<< "  arr size<=0 err ,return -1 " << "\tignorecount:["
					<< ignorecount++ << "\t](" << __FILE__ << ":" << __LINE__
					<< ":0" << ")" << std::endl;
			}

			return -1;
		}
		Intg left = 0;
		Intg right = arr.size() - 1;
		Intg mid;
		while (left <= right) {
			mid = (left + right) / 2;

			//要找的数在中点左边
			if (addr < (arr.operator[](mid))) {
				right = mid - 1;

			}
			//要找的数在中点右边
			else if (addr > (arr.operator[](mid))) {
				left = mid + 1;

			}
			//否则找到了，直接跳出
			else {

				return mid;
				break;
			}

		}
#if debug
		cerr << "getAddrPosition: can't found Addr position,return -1" << endl;
#endif
		mid = -1;
		return mid;

	}
	template<typename Array_, class Addr_>
	const Intg getLastAddrPosition(const Array_& arr, const Addr_& addr) {
		Intg left = 0;
		Intg right = arr.size() - 1;
		Intg mid;
		if (arr.size() <= 0 || addr <= arr[0]) {
			//#if debug
			//        cerr << "getLastAddrPosition err::addr<=arr[0]" << endl;
			//#endif

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "getLastAddrPosition err::addr<=arr[0]"; // Replace with actual info
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
				OutputDebugString(winfo.c_str()); cout << info << endl;
				ignoreCount++;
			}
#endif

			return -1;
		}
		if (addr > arr.back()) {
			return right;
		}
		while (left <= right) {
			mid = (left + right) / 2;

			//要找的数在中点左边
			if (addr < arr[mid]) {
				right = mid - 1;

			}
			//要找的数在中点右边
			else if (addr > arr[mid]) {
				left = mid + 1;

			}
			//否则找到了，直接跳出
			else {

				return mid - 1;
				break;
			}

		}
		return (left + right) / 2;

	}

	/**
	 * \brief overload version of const Intg getLastAddrPosition(const Array_ &arr, const Addr_ &addr), with detailed output
	 * \tparam Array_
	 * \param arr
	 * \param addr
	 * \return
	 */
	template<typename Array_>
	const Intg getLastAddrPosition(const Array_& arr, const Addr& addr) {
		Intg left = 0;
		Intg right = arr.size() - 1;
		Intg mid;
		if (arr.size() <= 0 || addr <= arr[0]) {
			//#if debug
			//        cerr << "getLastAddrPosition err::addr<=arr[0]" << endl;
			//#endif

#if debug && _WIN32
			{
				static int ignoreCount = 0;
				// Assuming info is a std::string, convert to std::wstring
				std::string info = "getLastAddrPosition err::addr<=arr[0]"; // Replace with actual info
				info = info + "addr:[" + addr.toStrg() + "]";
				info = std::string() + "(" + __FILE__ + ":" + std::to_string(__LINE__) + ":" + __FUNCTION__ ")\n" + info + "\tignoreCount:[" + std::to_string(ignoreCount) + "]\n";
				std::wstring winfo(info.begin(), info.end());
				OutputDebugString(winfo.c_str()); cout << info << endl;
				ignoreCount++;
			}
#endif

			return -1;
		}
		if (addr > arr.back()) {
			return right;
		}
		while (left <= right) {
			mid = (left + right) / 2;

			//要找的数在中点左边
			if (addr < arr[mid]) {
				right = mid - 1;

			}
			//要找的数在中点右边
			else if (addr > arr[mid]) {
				left = mid + 1;

			}
			//否则找到了，直接跳出
			else {

				return mid - 1;
				break;
			}

		}
		return (left + right) / 2;

	}



	/**
	 * @attention 允许addr不存在于arr
	 * @tparam Array_
	 * @param arr
	 * @param addr
	 * @return 不存在返回-1
	 */
	template<typename Array_>
	const Intg getNextAddrPosition(const Array_& arr, const Addr& addr) {
		Intg left = 0;
		Intg right = arr.size() - 1;
		Intg mid;
		if (arr.size() <= 0 || addr >= arr.back()) {
#if debug
			cerr << "getNextAddrPosition err::addr>=arr[max]" << endl;
#endif

			return -1;
		}
		if (addr < arr[0]) {
			return 0;
		}
		while (left <= right) {
			mid = (left + right) / 2;

			//要找的数在中点左边
			if (addr < arr[mid]) {
				right = mid - 1;

			}
			//要找的数在中点右边
			else if (addr > arr[mid]) {
				left = mid + 1;

			}
			//否则找到了，直接跳出
			else {

				return mid + 1;
				break;
			}

		}
		return (left + right) / 2 + 1;

	}

	template<typename Array_, typename Addr_>
	const bool erase(Array_& arr, const Addr_& addr) {
		arr.erase(arr.begin() + getAddrPosition<Array_, Addr_>(arr, addr));
		return true;
	}


	/*
	 * a 2 dimension list2 (support operator[], like vector or deque)
	 * @param
	 * List2Type a<b<x>>
	 * InnerListType b<x>
	 * InnerListType x
	 */
	class NumVec2;
	class NumVec2 {
	public:
		deque<deque<Numb>> list2;
		NumVec2& operator=(const NumVec2& op2);

		NumVec2& operator=(const deque<deque<Numb>>& op2);

		bool operator<(const NumVec2& vector2_) const;

		bool operator==(const NumVec2& op2) const;

		bool operator!=(const NumVec2& op2) const;

		int size() const;

		deque<Numb>& operator[](const int& i);

		//    NumVec2&& operator+(const NumVec2 &op2) {
		//
		//    }
		//    NumVec2&& operator-(const NumVec2 &op2) {
		//    }
		//    NumVec2& operator+=(const NumVec2 &op2) {
		//    }
		//    NumVec2& operator-=(const NumVec2 &op2) {
		//    }

		NumVec2() {	}
		/**
		 * @convert "xx,xx;xx,xx..." into a 2-dimension matrix in std::vec
		 * @param input
		 */
		NumVec2(const Strg& input);


		~NumVec2() {
		}
		Strg toStrg() const;


	};
	class NumVec2BinaryInfo {
	public:
		List2BinaryInfo<deque<deque<Numb>>,
			ListBinaryInfo<deque<Numb>, Numb, Numb>, deque<Numb> > list2Info;
		Intl parseAndWriteObject(const NumVec2& nv2, fstream& fs);

		Intl constructObject(NumVec2& nv2, fstream& fs);


	};
	//the control info of Functions（rules） start
	typedef NumVec2BinaryInfo TFInfoMatInfo;
	typedef NumVec2 TFInfoMat;
	//the control info of Functions（rules） end
	class ScopeTable;
	class SystemTable;
	class TemplateFunctionTableHash;
	class TemplateFunctionTable;
	class TemplateFunction;
	class TemplateFunction {
	public:
		Intg integrate = F_; // the function returns an expr (rule function)

		bool isBackwardFunction = false;
		Intg matchLevel = 0;

		FKA forwardFunctionFKA;

		FKA nameStartFKA;
		FKA nameEndFKA;
		//vector<Addr> templateHandleAddrList;
		FKA bodyScopeFKA;
		FKA bodyStartFKA;
		FKA bodyEndFKA;

		FKA templateHandleFKA; //函数声明表达式的根节点
		vector<FKA> returnHandle;
		TFInfoMat organization;
		TemplateFunction(const Scope& functionNameScope);
		TemplateFunction() {
		}
		TemplateFunction(FKA fka_built_in);

		Strg toStrg() const;

	};
	class TemplateFunctionBinaryInfo {
	public:
		Intg integrate = F_;

		bool isBackwardFunction = false;
		Intg matchLevel = 0;
		FKABinaryInfo forwardFunctionFKAInfo;

		FKABinaryInfo nameStartFKAInfo;
		FKABinaryInfo nameEndFKAInfo;
		FKABinaryInfo bodyScopeFKAInfo;
		FKABinaryInfo bodyStartFKAInfo;
		FKABinaryInfo bodyEndFKAInfo;

		FKABinaryInfo templateHandleFKAInfo; //函数声明表达式的根节点
		ListBinaryInfo<vector<FKA>, FKABinaryInfo, FKA> returnHandleInfo;
		TFInfoMatInfo organizationInfo;
		Intl parseAndWriteObject(const TemplateFunction& tf, fstream& fs);

		Intl constructObject(TemplateFunction& tf, fstream& fs);

	};
	class TemplateFunctionTable {
	public:
		Strg fileKey;
		TemplateFunctionTableHash* tfth = nullptr;
		deque<Addr> addrdq;
		deque<TemplateFunction> tfdq;

		TemplateFunctionTable() {
		}
		const Intg addTemplateFunction(const Addr& addr,
			const TemplateFunction& templateFunction);

		TemplateFunction& operator[](const Addr& tfAddr);


		typedef Addr TFAddr;
		Addr lastTfAddr(const TFAddr& tfAddr);


		Addr nextTfAddr(const Addr& codeTableAddr);


		bool add(const Addr& tfAddr, const TemplateFunction& tf);

		FKA lastAccessibleTFFKA(ScopeTableHash* const scopeTableHash,
			CodeTableHash* const codeTableHash,
			SystemTableHash* const systemTableHash, const FKA& lowbound,
			const FKA& codeOrScopeFKA);
		TemplateFunctionTable(TemplateFunctionTableHash* tfth,
			const Strg& fileKey);
		Strg toStrg() const;

	};
	class TemplateFunctionTableBinaryInfo {
	public:
		StrgBinaryInfo fileKeyInfo;
		ListBinaryInfo<deque<Addr>, AddrBinaryInfo, Addr> addrdqinfo;
		ListBinaryInfo<deque<TemplateFunction>, TemplateFunctionBinaryInfo,
			TemplateFunction> tfdqinfo;
		Intl parseAndWriteObject(const TemplateFunctionTable& tft, fstream& fs);
		Intl constructObject(TemplateFunctionTable& tft, fstream& fs);
	};
	class ScopeTableHash;
	class TemplateFunctionTableHash {
	public:
		map<Strg, TemplateFunctionTable> file_tft_map;
		CodeTableHash* cdth = nullptr;
		TemplateFunctionTableHash(CodeTableHash* cdth);
		void add(TemplateFunctionTable& tft);

		/**
		 * @attention 不检查tf有关信息是否与scopeTableHash匹配
		 * @param fka 为函数声明作用域的scopeFKA
		 * @param tf
		 */
		void add(const FKA& fka, TemplateFunction& tf);

		/**
		 * @attention 检查tf有关信息是否与scopeTableHash匹配,若不匹配会修改至匹配
		 * @param fka 为函数声明作用域的scopeFKA
		 * @param tf
		 */
		void add(const FKA& fka, TemplateFunction& tf, ScopeTableHash* scpth);


		FKA lastAccessibleTFFKA(ScopeTableHash* const scopeTableHash,
			CodeTableHash* const codeTableHash,
			SystemTableHash* const systemTableHash, const FKA& lowbound,
			const FKA& codeOrScopeFKA);
		FKA lastTfFKA(const FKA& fka);
		FKA nextTfFKA(const FKA& fka);
		FKA getTFFKA(const FKA& fka, ScopeTableHash* scpth);
		TemplateFunction& operator[](const FKA& tffka);

		TemplateFunctionTable& operator[](const Strg& fileKey);

		Strg toStrg() const;

		Intg count(const FKA& fka);

		/// <summary>
		/// copy the codetable (COOL_M) of the function name.
		/// </summary>
		/// <param name="tffka"></param>
		static CodeTable getTFNameCdt(const FKA& tffka, ScopeTableHash* scpth);

		static Strg getTFNameByFKA(const FKA& funfka) {
			if (funfka.fileKey != FILEKEYBUILTIN) {
				return funfka.toStrg();
			}
			else {
				Strg name;
				switch (funfka.addr.addrv.back()) {
				case S_S_JIA: name = "S_S_JIA(+)"; break;
				case S_S_JIAN: name = "S_S_JIAN(-)"; break;
				case S_S_CHENG: name = "S_S_CHENG(*)"; break;
				case S_S_CHU: name = "S_S_CHU(/)"; break;
				case S_S_FUZHI: name = "S_S_FUZHI(=)"; break;
				case S_S_DENGYU: name = "S_S_DENGYU(==)"; break;
				case S_S_MI: name = "S_S_MI(^)"; break;
				case CALL: name = "CALL"; break;
				case SHUCHU: name = "SHUCHU(-->)"; break;
				case S_S_DAYU: name = "S_S_DAYU(>)"; break;
				case S_S_XIAOYU: name = "S_S_XIAOYU(<)"; break;
				case COMMA: name = "COMMA(,)"; break;
				case TUIDAO: name = "TUIDAO(=>)"; break;
				case LST_ACCESS: name = "LST_ACCESS([])"; break;
				case LENGTH: name = "LENGTH"; break;
				case ERASE: name = "ERASE"; break;
				case CLEAR: name = "CLEAR"; break;
				case TYPENAME: name = "TYPENAME"; break;
				case S_S_AND: name = "S_S_AND(&&)"; break;
				case S_S_OR: name = "S_S_OR(||)"; break;
				case S_S_NOT: name = "S_S_NOT(!)"; break;
				case S_S_MODULO: name = "S_S_MODULO(%)"; break;
				case S_S_JIADENG: name = "S_S_JIADENG(+=)"; break;
				case S_S_JIANDENG: name = "S_S_JIANDENG(-=)"; break;
				case S_S_CHENGDENG: name = "S_S_CHENGDENG(*=)"; break;
				case S_S_CHUDENG: name = "S_S_CHUDENG(/=)"; break;
				case S_S_MODENG: name = "S_S_MODENG(%=)"; break;
				case S_S_MIDENG: name = "S_S_MIDENG(^=)"; break;
				case S_SI: name = "S_SI(lhs++)"; break;
				case S_SD: name = "S_SD(lhs--)"; break;
				case S_MAP: name = "S_MAP"; break;
				case S_MULTIMAP: name = "S_MULTIMAP"; break;
				case S_SET: name = "S_SET"; break;
				case S_MULTISET: name = "S_MULTISET"; break;
				case S_TONUM: name = "S_TONUM"; break;
				case S_TOSTRG: name = "S_TOSTRG"; break;
				case S_TOINT: name = "S_TOINT"; break;
				case S_NONBLOCKEXECUTE: name = "S_NONBLOCKEXECUTE"; break;
				case S_BLOCKEXECUTE: name = "S_BLOCKEXECUTE"; break;
				case S_SLEEP: name = "S_SLEEP"; break;
				case S_LOGICJUMP: name = "S_LOGICJUMP"; break;
				case S_FIND: name = "S_FIND"; break;
				case S_COUNT: name = "S_COUNT"; break;
				case S_INSERT: name = "S_INSERT"; break;
				case S_PUSHBACK: name = "S_PUSHBACK"; break;
				case S_PUSHFRONT: name = "S_PUSHFRONT"; break;
				case S_POPBACK: name = "S_POPBACK"; break;
				case S_POPFRONT: name = "S_POPFRONT"; break;
				case S_BACK: name = "S_BACK"; break;
				case SI_S: name = "SI_S(++rhs)"; break;
				case SD_S: name = "SD_S(--rhs)"; break;
				case S_S_BUDENG: name = "S_S_BUDENG(!=)"; break;
				case S_S_BUDAYU: name = "S_S_BUDAYU(<=)"; break;
				case S_S_BUXIAOYU: name = "S_S_BUXIAOYU(>=)"; break;
				case S_S_EXIST_SUBEXPR: name = "S_S_EXIST_SUBEXPR"; break;
				case S_S_FIND_SUBEXPR: name = "S_S_FIND_SUBEXPR"; break;
				case S_RESET: name = "S_RESET"; break;
					//case ADDR_NEG_LOWBOUND: name = "ADDR_NEG_LOWBOUND"; break;

				default: name = "Unknown"; break;
				}

				return name;
			}

		}


		static Strg getTFOperatorByFKA(const FKA& funfka) {
			if (funfka.fileKey != FILEKEYBUILTIN) {
				return funfka.toStrg();
			}
			else {
				Strg name;
				switch (funfka.addr.addrv.back()) {
				case S_S_JIA: name = "+"; break;
				case S_S_JIAN: name = "-"; break;
				case S_S_CHENG: name = "*"; break;
				case S_S_CHU: name = "/"; break;
				case S_S_FUZHI: name = "="; break;
				case S_S_DENGYU: name = "=="; break;
				case S_S_MI: name = "^"; break;
				case CALL: name = "call"; break;
				case SHUCHU: name = "-->"; break;
				case S_S_DAYU: name = ">"; break;
				case S_S_XIAOYU: name = "<"; break;
				case COMMA: name = "COMMA"; break;
				case TUIDAO: name = "=>"; break;
				case LST_ACCESS: name = "[]"; break;
				case LENGTH: name = "LENGTH"; break;
				case ERASE: name = "ERASE"; break;
				case CLEAR: name = "CLEAR"; break;
				case TYPENAME: name = "TYPENAME"; break;
				case S_S_AND: name = "&&"; break;
				case S_S_OR: name = "||"; break;
				case S_S_NOT: name = "!"; break;
				case S_S_MODULO: name = "%"; break;
				case S_S_JIADENG: name = "+="; break;
				case S_S_JIANDENG: name = "-="; break;
				case S_S_CHENGDENG: name = "*="; break;
				case S_S_CHUDENG: name = "/="; break;
				case S_S_MODENG: name = "%="; break;
				case S_S_MIDENG: name = "^="; break;
				case S_SI: name = "++"; break;
				case S_SD: name = "--"; break;
				case S_MAP: name = "MAP"; break;
				case S_MULTIMAP: name = "MULTIMAP"; break;
				case S_SET: name = "SET"; break;
				case S_MULTISET: name = "MULTISET"; break;
				case S_TONUM: name = "TONUM"; break;
				case S_TOSTRG: name = "TOSTRG"; break;
				case S_TOINT: name = "TOINT"; break;
				case S_NONBLOCKEXECUTE: name = "NONBLOCKEXECUTE"; break;
				case S_BLOCKEXECUTE: name = "BLOCKEXECUTE"; break;
				case S_SLEEP: name = "SLEEP"; break;
				case S_LOGICJUMP: name = "S_LOGICJUMP"; break;
				case S_FIND: name = "FIND"; break;
				case S_COUNT: name = "COUNT"; break;
				case S_INSERT: name = "INSERT"; break;
				case S_PUSHBACK: name = "PUSHBACK"; break;
				case S_PUSHFRONT: name = "PUSHFRONT"; break;
				case S_POPBACK: name = "POPBACK"; break;
				case S_POPFRONT: name = "POPFRONT"; break;
				case S_BACK: name = "BACK"; break;
				case SI_S: name = "++"; break;
				case SD_S: name = "--"; break;
				case S_S_BUDENG: name = "!="; break;
				case S_S_BUDAYU: name = "<="; break;
				case S_S_BUXIAOYU: name = ">="; break;
				case S_S_EXIST_SUBEXPR: name = "EXIST_SUBEXPR"; break;
				case S_S_FIND_SUBEXPR: name = "FIND_SUBEXPR"; break;
				case S_RESET: name = "RESET"; break;
					//case ADDR_NEG_LOWBOUND: name = "ADDR_NEG_LOWBOUND"; break;

				default: name = "Unknown"; break;
				}

				return name;
			}

		}



	};



	//[[deprecated("the class is useless, please use Constraint")]]
	class Property;
	class Property {
	public:
		Addr propertyNameStartAddr;
		Addr propertyNameEndAddr;
		vector<Addr> propertyHandleAddrList;
		vector<Addr> propertyArgAddrList;
		Addr propertyStartAddr;
		Addr propertyEndAddr;

	};
	class PropertyTable;
	class PropertyTable {
	public:
		deque<Addr> addrdq;
		deque<Property> propdq;

		/*	const Intg addProperty(const Addr& addr, const Property& property) {
				addrdq.push_back(addr);
				propdq.push_back(property);
				return 1;
			}*/

	};
	class Data;
	class DataTable;

	class ScopeTable;
	class Scope;

	class Scope {
	public:
		Ints functionFlag = 0; //showcase the functionality of the scope, not meaning the scope must be a part of a function.
		Ints conditionFlag = 0;
		bool matched = false;
		Strg fileKey;
		Addr scopeCoor1;
		Addr scopeCoor2;
		FKA parentScopeFKA;
		FKA requireScopeFKA;  //a function name's requireScope is its function body.
		FKA conditionCodeFKA; //COOL_B or COOL_L type
		FKA validTemplateFunctionNameFKA; //当为函数声明作用域时有效，指向最初声明的scope的fka，同时此scope失效
		vector<FKA> returnHandle;
		vector<Property> scopeProperty; //暂时不用2022年7月28日15:32:01
		Strg toStrg() const;


	};
	static Scope scopenull = Scope();
	class ScopeBinaryInfo {
	public:
		Ints functionFlag = 0;
		Ints conditionFlag = 0;
		bool matched = false;
		StrgBinaryInfo fileKeyInfo;
		AddrBinaryInfo scopeCoor1Info;
		AddrBinaryInfo scopeCoor2Info;
		FKABinaryInfo parentScopeFKAInfo;
		FKABinaryInfo requireScopeFKAInfo;
		FKABinaryInfo conditionCodeFKAInfo;
		FKABinaryInfo validTemplateFunctionNameFKAInfo;
		ListBinaryInfo<vector<FKA>, FKABinaryInfo, FKA> returnHandleInfo;
		//ListBinaryInfo<PropertyBinaryInfo> scopeProperty;//暂时不用2022年7月28日15:32:01
		Intl parseAndWriteObject(const Scope& scp, fstream& fs);
		Intl constructObject(Scope& scp, fstream& fs);
	};

	class ScopeTableHash;

	class ScopeTable {
	public:
		ScopeTableHash* scpth = nullptr;
		Strg fileKey;
		deque<Addr> addrdq;
		deque<Scope> scopedq;
		Intg newScopeStart();
		Intg newScopeStart(Addr& scopeAddr, Scope& scope);
		Intg addScopeProperty();
		Intg scopeEnd();
		Intg scopeEnd(Addr& scopeAddr, Scope& scope);
		/*
		 * @attention 不能通过此[] 运算符创建新的scope。
		 */
		Scope& operator[](const Addr& addr);

		Addr getNextScopeAddr(const Addr& codeTableAddr);

		Addr getLastScopeAddr(const Addr& codeTableAddr);

		ScopeTable() {
		}
		ScopeTable(ScopeTableHash* scpth, const Strg& fileKey);

		Strg toStrg() const;


	};
	class ScopeTableBinaryInfo {
	public:
		StrgBinaryInfo fileKeyInfo;
		ListBinaryInfo<deque<Addr>, AddrBinaryInfo, Addr> addrdqlist;
		ListBinaryInfo<deque<Scope>, ScopeBinaryInfo, Scope> scopedqlist;
		Intl parseAndWriteObject(const ScopeTable& scpt, fstream& fs);
		Intl constructObject(ScopeTable& scpt, fstream& fs);
	};

	class ScopeTableHashBinaryInfo;

	/**
	 *  @brief scope禁止跨cdt
	 */
	class ScopeTableHash {
	public:
		map<Strg, ScopeTable> file_scpt_map;
		CodeTableHash* cdth;
		ScopeTableHash(CodeTableHash& cdth);

		void add(ScopeTable& scpt);

		ScopeTable& operator[](const Strg& fileKey);
		Scope& operator[](const FKA& fka);
		bool accessible(const FKA& origin, const FKA& dest);
		bool accessible(const FKA& origin, const FKA& parent, const FKA& dest);
		bool find(const FKA& fka);
		Intg newScopeStart();
		Intg newScopeStart(FKA& scopeFKA, Scope& scope);
		Intg addScopeProperty();
		Intg scopeEnd();
		Intg scopeEnd(FKA& scopeFKA, Scope& scope);
		/*
		 * @attention 不能通过此[] 运算符创建新的scope。
		 */
		FKA getNextScopeFKA(const FKA& codeTableFKA);
		FKA getLastScopeFKA(const FKA& codeTableFKA);
		Strg toStrg() const;

	};
	class ScopeTableHashBinaryInfo {
	public:
		mapBinaryInfo<StrgBinaryInfo, Strg, ScopeTableBinaryInfo, ScopeTable> file_scpt_map_info;
		Intl parseAndWriteObject(const ScopeTableHash& scpth, fstream& fs);

		Intl constructObject(ScopeTableHash& scpth, fstream& fs);

	};



	class System {
	public:
		//id由systemTable自动分配，禁止手动赋值(filekey+id = fki)
		FKI fki;
		Strg systemName;
		FKA systemFKA;
		FKA parentScopeFKA;
		set<FKA> constructorFKAList;
		//    deque<Addr> parentSystemAddrList;
		set<FKI> decedentSystemFKIList;
		System() {
		}
		System(const Strg& name);

		System(const Strg& name, const FKA& parentScopeFKA);

		System(const Strg& name, const FKA& systemScopeFKA,
			const FKA& parentScopeFKA);

		Strg toStrg() const;

	};
	class SystemBinaryInfo {
	public:
		FKIBinaryInfo fkiInfo;
		StrgBinaryInfo systemNameInfo;
		FKABinaryInfo systemFKAInfo;
		FKABinaryInfo parentScopeFKAInfo;
		SetBinaryInfo<set<FKA>, FKABinaryInfo, FKA> constructorFKAListInfo;
		SetBinaryInfo<set<FKI>, FKIBinaryInfo, FKI> decedentSystemFKIListInfo;
		Intl parseAndWriteObject(const System& sys, fstream& fs);

		Intl constructObject(System& sys, fstream& fs);

	};
	class SystemTableHash;

	/**
	 * @attention 可以通过【】运算符来检索system的引用，但不能通过【】来创建新的引用。
	 * @attention system自身的fileKey为其第一次声明位置的filekey
	 * @attention 一个sys的filekey的fki的fileKey必须与其所在syst的fileKey一致,
	 * 但由于其声明位置可能与作用域位置可能不一，故fka的fileKey与syst的fileKey可能不一致
	 */
	class SystemTable {
	public:
		Intg id = 0;
		Strg fileKey;
		SystemTableHash* systh = nullptr;

		map<Intg, System> id_system_map;
		map<Strg, deque<Intg>> name_iddq_map;
		map<FKA, Intg> fka_id_map;

		void add(const System& sys);

		bool systemExisted(const System& sys);

		System& operator[](const FKA& fka);

		System& operator[](const Intg& id);

		System& operator[](const pair<Strg, FKA>& name_parentScopeFKA);

		Intg getSystemID(const pair<Strg, FKA>& name_parentScopeFKA);

		void erase(Intg id);

		Strg toStrg();

		void inherit(const Strg& childName, const FKA& childParentScopeFKA,
			const Strg& parentName);
		SystemTable() {
		}
		SystemTable(SystemTableHash* systh, Strg fileKey);


		Strg toStrg() const;

	};
	class SystemTableBinaryInfo {
	public:
		Intg id = 0;
		StrgBinaryInfo fileKeyInfo;
		mapBinaryInfo<Intg, Intg, SystemBinaryInfo, System> id_system_map_info;
		mapBinaryInfo<StrgBinaryInfo, Strg, ListBinaryInfo<deque<Intg>, Intg, Intg>,
			deque<Intg>> name_iddq_map_info;
		mapBinaryInfo<FKABinaryInfo, FKA, Intg, Intg> fka_id_map_info;
		Intl parseAndWriteObject(const SystemTable& syst, fstream& fs);

		Intl constructObject(SystemTable& syst, fstream& fs);

	};
	class SystemTableHashBinaryInfo;

	class SystemTableHash {
	public:
		CodeTableHash* cdth;
		ScopeTableHash* scpth;
		map<Strg, SystemTable> file_syst_map;
		SystemTableHash(CodeTableHash& cdth, ScopeTableHash& scpth);

		void add(SystemTable& syst);

		void add(System& sys);

		SystemTable& operator[](const Strg& fileKey);
		System& operator[](const FKA& fka);
		System& operator[](const FKI& fki);
		System& getDirectSystemByCodeFKA(const FKA& codeFKA);
		bool isChild(const FKA& maychild, const FKA& mayparent);
		bool systemExisted(const System& sys);
		FKI getSystemFKI(const pair<Strg, FKA>& name_parentScopeFKA);
		System& operator[](const pair<Strg, FKA>& name_parentScopeFKA);
		void inherit(const Strg& childName, const FKA& childParentScopeFKA,
			const Strg& parentName);
		Strg toStrg() const;

	};
	class SystemTableHashBinaryInfo {
	public:
		mapBinaryInfo<StrgBinaryInfo, Strg, SystemTableBinaryInfo, SystemTable> file_syst_map_info;
		Intl parseAndWriteObject(const SystemTableHash& systh, fstream& fs);

		Intl constructObject(SystemTableHash& systh, fstream& fs);

	};




	class TemplateFunctionTableHashBinaryInfo;


	class TemplateFunctionTableHashBinaryInfo {
	public:
		mapBinaryInfo<StrgBinaryInfo, Strg, TemplateFunctionTableBinaryInfo,
			TemplateFunctionTable> file_tft_map_info;
		Intl parseAndWriteObject(const TemplateFunctionTableHash& tfth,
			fstream& fs);

		Intl constructObject(TemplateFunctionTableHash& tfth, fstream& fs);

	};




	class HashCollectionBinaryInfo;
	class HashCollection {
	public:
		bool completed = false;    //是否完整（完整则无需再加载库）
		Setting setting;
		Strg fileKey;    //8-bit char string
		CodeTableHash cdth;
		ScopeTableHash scpth;
		TemplateFunctionTableHash tfth;
		SystemTableHash systh;

		HashCollection() :
			cdth(), scpth(cdth), tfth(&cdth), systh(cdth, scpth) {
		}
		void completeAll();
		void loadFromBinaryFile(const pathStr& path_);
		pathStr saveToMultiFiles(const pathStr& dir_path);
		//    Strg saveToMultiFiles();        //默认输出到COOLANG_PROJECT
		pathStr saveToSingleFile(const pathStr& dir_path);
		//    Strg saveToSingleFile();
		void merge(HashCollection& hc);
		HashCollection extractSimpleHash(const Strg& fileKey_);
		map<Strg, HashCollection> splitSelfToSimpleHashs();
		Strg toStrg() const;

	};
	/**
	 *  @attention info中各元素的顺序必须与对应class中各元素的顺序一致！
	 */
	class HashCollectionBinaryInfo {
	public:
		bool completed = false;
		SettingBinaryInfo settingInfo;
		StrgBinaryInfo fileKeyInfo;
		CodeTableHashBinaryInfo cdthInfo;
		ScopeTableHashBinaryInfo scpthInfo;
		TemplateFunctionTableHashBinaryInfo tfthInfo;
		SystemTableHashBinaryInfo systhInfo;
		Intl parseAndWriteObject(const HashCollection& hashCollection,
			fstream& fs);

		Intl constructObject(HashCollection& hashCollection, fstream& fs);

	};

	vector<filesystem::path> getFilePath(const filesystem::path& path_,
		const string& fileKey, const bool& withSuffix);



}

#endif /* COOLANG_VM_HPP_ */
