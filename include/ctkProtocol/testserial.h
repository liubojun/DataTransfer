#ifndef testserial_h__
#define testserial_h__


#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <map>


#include "macro.h"

class CInterData
{

public:
	int _tag;
	std::string _text;
	std::vector<int> _arrary;
	std::list<int> _list;
	std::map<int, int> _map;
};

class CMyData
{
public:
	CMyData():_tag(0), _text(""){}

	CMyData(int tag, std::string text):_tag(tag), _text(text){}

	int GetTag() const {return _tag;}
	std::string GetText() const {return _text;}

public:
	int _tag;
	std::string _text;
	std::vector<int> _arrary;
	std::list<int> _list;
	std::map<int, int> _map;

	CInterData _d;
};

DLL_EXPORT_C_DECL void TestArchive1();


#endif // testserial_h__