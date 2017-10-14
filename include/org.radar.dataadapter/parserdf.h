#ifndef parserdf_h__
#define parserdf_h__

#include "ProductDef.h"
#include <stdio.h>
#include <math.h>
#include <string>
using namespace std;

class CParseRdf
{
public:
    CParseRdf();
    virtual~CParseRdf();
public:
    //解析数据入口函数
    bool parsePolar(const char* filename,PolarProduct &polarHead,  PolarDataBuffer *&polarBuffer);
    bool parseSquar(const char* filename,SquareProduct &squarHead, short *&squarBuffer);
    bool parseVector(const char* filename,VectorProduct &vectorHead, VectorDataBuffer *&vectorBuffer);

    bool parseSquarVector(const char* filename,SquareProduct &squarHead,
                          short *&squarBuffer,VectorDataBuffer *&vectorBuffer);
    bool parsePolarVector( const char* filename,PolarProduct &polarHead,
                           PolarDataBuffer *&polarBuffer,VectorDataBuffer *&vectorBuffer );
};

#endif // parserdf_h__
