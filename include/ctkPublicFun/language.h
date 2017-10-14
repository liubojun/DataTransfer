/*!
 * \file language.h
 * \date 2016/04/06 10:44
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 定义国际化语言相关选择
 *
 * TODO: long description
 *
 * \note
*/

#ifndef language_h__
#define language_h__

#pragma warning(disable:4996)	//禁止strcpy的警告

#include <string.h>

typedef enum LANGUAGE_T
{

	ZH,
	EN

}language_t;

typedef struct LANGUAGE2FILE 
{

	language_t oLan;
	char filesuffix[10];

	LANGUAGE2FILE(language_t l, const char *f)
	{
		oLan = l;
		//strncpy_s(filesuffix, f, sizeof(filesuffix)-1);
		strncpy(filesuffix, f, sizeof(filesuffix)-1);
	}

}language2file;

static language2file l2f[] = {LANGUAGE2FILE(ZH, "zh"), LANGUAGE2FILE(EN, "en")};

#endif // language_h__