/*
 * coolang_define.h
 *
 *  Created on: 2022年8月16日
 *      Author: dell
 */

#ifndef COOLANG_DEFINE_H_
#define COOLANG_DEFINE_H_


#define debug false
#define enableNonAsciiConvert false

#define Intg int
#define Intl int64_t
#define Numb double

#define safe_delete(p)  { if(p) { delete (p);     (p)=NULL; } }


/*
 * Support for non-ASCII character paths
 */
#ifndef pathStr

#include <string.h>
#include <iostream>
#include <vector>
#include <filesystem>
typedef std::string string;
typedef std::wstring wstring;

#if __WIN32__
#include <windows.h>
#define pathStr std::wstring  //win systems use ANSI(GBK)
/**
 * @brief convert char * to wstring
 * @param charp
 * @param codePage
 * @param existUnknownCharacters if there are unknown character in charp, set to 1.
 * @return
 */
std::wstring charpToWString(const char *charp, const int codePage,
        int *existUnknownCharacters) ;
std::wstring StringToWString(const string &str);
std::string WStringToString(const wstring &wstr, const int codePage,
        int *existUnknownCharacters) ;
/**
 * @brief
 * @param wstr
 * @return
 */
std::string WStringToString(const wstring &wstr) ;
/**
 * @brief convert char* to utf8
 * @return
 */
string charpToUtf8(const char *chrp);
pathStr& assign(pathStr &pstr, const char *chrp);
pathStr& assign(pathStr &pstr, const string &str) ;
string pathStrToStr(const pathStr &pstr);
/**
 * @brief this function convert "filesystem::path" to ANSI code for win systems
 * @param pstr
 * @return
 */
string pathToStr(const std::filesystem::path &path_);
pathStr pathToPathStr(const std::filesystem::path &path_);
pathStr charpToPathStr(const char *charp) ;
std::filesystem::path charpToPath(const char *charp);

#else
#define pathStr std::string  //other systems use utf8
pathStr& assign(pathStr & pstr,const char * chrp);
pathStr& assign(pathStr & pstr,const string & str);
/**
 * @brief this function convert "filesystem::path" to utf8 code for linux systems
 * @param pstr
 * @return
 */
string pathToStr(const std::filesystem::path &path_);
string pathToPathStr(const std::filesystem::path &path_);
pathStr charpToPathStr(const char *charp) ;
std::filesystem::path charpToPath(const char *charp);
#endif

#endif

#endif /* COOLANG_DEFINE_H_ */
