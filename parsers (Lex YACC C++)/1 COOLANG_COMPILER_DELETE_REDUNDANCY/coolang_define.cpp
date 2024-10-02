/*
 * coolang_define.cpp
 *
 *  Created on: 2022Äê8ÔÂ16ÈÕ
 *      Author: dell
 */
#include "coolang_define.hpp"



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
        int *existUnknownCharacters) {
    int num = MultiByteToWideChar(codePage, 0, charp, -1,
    NULL, 0);
    wchar_t *wide = new wchar_t[num + 1];
    int writeInNum = MultiByteToWideChar(codePage, MB_ERR_INVALID_CHARS, charp,
            -1, wide, num);
    std::wstring w_str(wide);
    delete[] wide;
    if (writeInNum == 0 && num != 0) {
        *existUnknownCharacters = true;
    } else {
        *existUnknownCharacters = false;
    }
    return w_str;
}
std::wstring StringToWString(const string &str) {
    auto codepagev = { CP_UTF8, CP_ACP, CP_MACCP, CP_OEMCP, CP_SYMBOL,
    CP_THREAD_ACP, CP_UTF7 };

    int existUnknownCharacters1 = false;
    wstring ws;
    for (auto pg : codepagev) {

        ws = charpToWString(str.c_str(), pg, &existUnknownCharacters1);

        if (existUnknownCharacters1 == 0) {
            break;
        }
    }
    return ws;
}
std::string WStringToString(const wstring &wstr, const int codePage,
        int *existUnknownCharacters) {

    int iSize;

    char *strBytes;
    char rep = '?';

    iSize = WideCharToMultiByte(codePage, WC_NO_BEST_FIT_CHARS, wstr.c_str(),
            -1, NULL, 0, NULL,
            NULL);

    strBytes = new char[(iSize + 1)]/**sizeof(char)*/;

    int writeInNum = WideCharToMultiByte(codePage, WC_NO_BEST_FIT_CHARS,
            wstr.c_str(), -1, strBytes, iSize, &rep, existUnknownCharacters);

    std::string str(strBytes);
    delete[] strBytes;
    if (writeInNum == 0 && iSize != 0) {
        *existUnknownCharacters = true;
    } else {
        *existUnknownCharacters = false;
    }
    return str;
}
/**
 * @brief
 * @param wstr
 * @return
 */
std::string WStringToString(const wstring &wstr) {

    string s;

    int existUnknownCharacters1 = false;
    s = WStringToString(wstr.c_str(), CP_ACP, &existUnknownCharacters1);

    return s;
}

/**
 * @brief convert char* to utf8
 * @return
 */
string charpToUtf8(const char *chrp) {
    auto codepagev = { CP_UTF8, CP_ACP, CP_MACCP, CP_OEMCP, CP_SYMBOL,
    CP_THREAD_ACP, CP_UTF7 };

    int existUnknownCharacters1 = false;
    int existUnknownCharacters2 = false;
    wstring ws;
    string s;
    for (auto pg : codepagev) {

        ws = charpToWString(chrp, pg, &existUnknownCharacters1);

        if (existUnknownCharacters1 == 0) {
            break;
        }
    }
    s = WStringToString(ws, CP_UTF8, &existUnknownCharacters2);
    if (existUnknownCharacters1 == 0 && existUnknownCharacters2 == 0) {

        return s;
    }
    std::cout << "Invalid character in path:" << s << ", exit(-1)."
            << std::endl;
    exit(-1);
}
pathStr& assign(pathStr &pstr, const char *chrp) {
    pstr = StringToWString(string(chrp));
    return pstr;
}
pathStr& assign(pathStr &pstr, const string &str) {
    pstr = StringToWString(str);
    return pstr;
}
string pathStrToStr(const pathStr &pstr) {
    return WStringToString(pstr);
}
/**
 * @brief this function convert "filesystem::path" to ANSI code for win systems
 * @param pstr
 * @return
 */
string pathToStr(const std::filesystem::path &path_) {
    return pathStrToStr(path_.wstring());
}
pathStr pathToPathStr(const std::filesystem::path &path_) {
    return path_.wstring();
}
pathStr charpToPathStr(const char *charp) {
    return StringToWString(string(charp));
}
std::filesystem::path charpToPath(const char *charp) {
    return std::filesystem::path(charpToPathStr(charp));
}

#else
#define pathStr std::string  //other systems use utf8
pathStr& assign(pathStr & pstr,const char * chrp){
    pstr = chrp;
    return pstr;
}
pathStr& assign(pathStr & pstr,const string & str){
    pstr = str;
    return pstr;
}
/**
 * @brief this function convert "filesystem::path" to utf8 code for linux systems
 * @param pstr
 * @return
 */
string pathToStr(const std::filesystem::path &path_){
    return path_.string();
}
string pathToPathStr(const std::filesystem::path &path_){
    return path_.string();
}

pathStr charpToPathStr(const char *charp) {
    return string(charp);
}
std::filesystem::path charpToPath(const char *charp) {
    return std::filesystem::path(charpToPathStr(charp));
}
#endif



