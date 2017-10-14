
// QGSCONFIG.H

#ifndef QGSCONFIG_H
#define QGSCONFIG_H

// Version must be specified according to
// <int>.<int>.<int>-<any text>.
// or else upgrading old project file will not work
// reliably.
#define VERSION "2.10.1-Pisa"

//used in vim src/core/qgis.cpp
//The way below should work but it resolves to a number like 0110 which the compiler treats as octal I think
//because debuggin it out shows the decimal number 72 which results in incorrect version status.
//As a short term fix I (Tim) am defining the version in top level cmake. It would be good to 
//reinstate this more generic approach below at some point though
//#define VERSION_INT 2101
#define VERSION_INT 21001
#define ABISYM(x) x ## 21001
//used in main.cpp and anywhere else where the release name is needed
#define RELEASE_NAME "Pisa"

#define QGIS_PLUGIN_SUBDIR "plugins"
#define QGIS_DATA_SUBDIR "."
#define QGIS_LIBEXEC_SUBDIR "."
#define QGIS_LIB_SUBDIR "lib"
#define CMAKE_INSTALL_PREFIX "E:/radar/qgis-2.10.1"
#define CMAKE_SOURCE_DIR "E:/radar/qgis-2.10.1"

#define QSCINTILLA_VERSION_STR "2.8.4"

#if defined( __APPLE__ )
//used by Mac to find system or bundle resources relative to amount of bundling
#define QGIS_MACAPP_BUNDLE 
#endif

#define QT_PLUGINS_DIR "D:/Qt/Qt5.4.2/5.4/msvc2010_opengl/plugins"
#define OSG_PLUGINS_PATH ""

/* #undef USING_NMAKE */

/* #undef HAVE_POSTGRESQL */

#define HAVE_SPATIALITE

#define HAVE_MSSQL

/* #undef HAVE_ORACLE */

/* #undef HAVE_PYTHON */

/* #undef HAVE_TOUCH */

/* #undef HAVE_OSGEARTHQT */

/* #undef SERVER_SKIP_ECW */

/* #undef HAVE_SERVER_PYTHON_PLUGINS */

/* #undef ENABLE_MODELTEST */

#endif

