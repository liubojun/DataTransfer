﻿#ifndef _MACRO_H
#define _MACRO_H

typedef long LPARAM;
typedef unsigned int WPARAM;

//#ifdef Q_WS_WIN
  #ifdef  WIN32
         #ifdef __cplusplus 
           #define DLL_EXPORT_C_DECL extern "C" __declspec(dllexport)
           #define DLL_IMPORT_C_DECL extern "C" __declspec(dllimport)
           #define DLL_EXPORT_DECL extern __declspec(dllexport)
           #define DLL_IMPORT_DECL extern __declspec(dllimport)
           #define DLL_EXPORT_CLASS_DECL __declspec(dllexport)
           #define DLL_IMPORT_CLASS_DECL __declspec(dllimport)
         #else
           #define DLL_EXPORT_DECL __declspec(dllexport)
           #define DLL_IMPORT_DECL __declspec(dllimport)
         #endif
    #else
         #ifdef __cplusplus
           #define DLL_EXPORT_C_DECL extern "C"
           #define DLL_IMPORT_C_DECL extern "C"
           #define DLL_EXPORT_DECL extern
           #define DLL_IMPORT_DECL extern
           #define DLL_EXPORT_CLASS_DECL
           #define DLL_IMPORT_CLASS_DECL
      #else
           #define DLL_EXPORT_DECL extern
           #define DLL_IMPORT_DECL extern
      #endif
    #endif

#endif
