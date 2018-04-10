#ifndef logprotocol_h__
#define logprotocol_h__

#include <RCF/RCF.hpp>
#include <string>
using namespace std;

RCF_BEGIN(I_LogPrint, "I_LogPrint")
RCF_METHOD_V4(void, print, const std::string &, const std::string &, const std::string &, int)
RCF_END(I_LogPrint)

#endif // logprotocol_h__
