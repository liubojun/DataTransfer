#ifndef pupstruct_h__
#define pupstruct_h__

#include "productDef.h"
#include "commontypedef.h"
#include <vector>

//////////////////////////////////////////////////////////////////////////
typedef struct
{
    EProductType productType;
    //polar文件特有的画图结构体
    PolarProduct polarFileHead;			//径向数据头
    std::vector<PolarDataBuffer> polarBuffer;		//极坐标
    //Square文件特有的画图结构体
    SquareProduct squareFileHead;		//格点数据头
    std::vector<char> squareBuffer;					//直角坐标
    //Vector文件特有的画图结构体
    VectorProduct vectorFileHead;		//矢量数据头
    std::vector<VectorDataBuffer> vectorDataBuf;	//矢量数据

} PupData;

typedef struct
{
    SquareProduct sqHead[2];
    std::vector<char> sqBuffer[2];
} MaxPupData;

//////////////////////////////////////////////////////////////////////////

#endif // pupstruct_h__
