#ifndef _H_H_TYPEDEFS_H_H_
#define _H_H_TYPEDEFS_H_H_


/*定义日志窗体类型 分别对应数据收集、消息处理、单站、组网*/
typedef enum LOGTYPE{ COLLECTION, MESSAGE, SINGLE, MOSAIC } LOGWIDGETTYPE;

/*定义传输系统向插件管理器中保存的对象类型为其相关业务插件提供使用目前分为O, T, TH, F*/
typedef enum TRANSSYS{ TS_COLLECTION_O=0, TS_COLLECTION_T, TS_COLLECTION_TH, TS_COLLECTION_F } TRANSFERSYSTEMTYPE;

#endif	//_H_H_TYPEDEFS_H_H_