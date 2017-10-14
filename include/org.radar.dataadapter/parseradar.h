#ifndef PARSER_H_
#define PARSER_H_

#include "vtbsource.h"
#include "parsevtb.h"
#include "parserdf.h"
#include "macro.h"
#ifdef WITH_MEM_CHECK
#include "vld.h"
#include "vld_def.h"
#endif

class DLL_EXPORT_CLASS_DECL ParseRadar
{
public:
    ParseRadar();

    ~ParseRadar();

    int parseVtb(const char *filename,RadarDataHead &filehead,NewLineDataBlock *&databuffer);

    bool parsePolar(const char* filename,PolarProduct &polarHead,PolarDataBuffer *&polarBuffer);

    bool parseSquar(const char* filename,SquareProduct &squarHead,short *&squarBuffer);

    bool parseVector(const char* filename,VectorProduct &vectorHead,VectorDataBuffer *&vectorBuffer);

    bool parseSquarVector(const char* filename,SquareProduct &squarHead,short *&squarBuffer,
                          VectorDataBuffer *&vectorBuffer);
    bool parsePolarVector( const char* filename,PolarProduct &palorHead,PolarDataBuffer *&polarBuffer,
                           VectorDataBuffer *&vectorBuffer );
    int parseSA(const char *fileName, RadarDataHead &fileHead, NewLineDataBlock *&dataBuffer);

private:
    CVTBSource *vtbSource;
    CParseRdf *parseRdf;
};

#endif /*PARSER_H_*/
