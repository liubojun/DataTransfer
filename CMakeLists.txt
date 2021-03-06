#指定CMake需要的最小版本
cmake_minimum_required(VERSION 2.8.11)

#指定项目名
project(DataTransfer)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(EXTERNAL_PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmakeExternals)
set(EXTERNAL_PROJECT_FILE_PREFIX "")
include(ExternalProject)
include(ctkMacroCheckExternalProjectDependency)

#是否需要使用Qt5
SET (WITH_QT5 TRUE CACHE BOOL "use qt5")
SET (QT_VERSIONS)
IF (WITH_QT5)
	find_package(Qt5Core REQUIRED)		#查找并加载外部package；REQUIRED表示若没找到，cmake将终止
	find_package(Qt5Gui REQUIRED)
	find_package(Qt5Widgets REQUIRED)
	find_package(Qt5Sql REQUIRED)
        #find_package(Qt5Xml REQUIRED)
        find_package(Qt5Concurrent REQUIRED)
        #find_package(Qt5UiTools REQUIRED)
	find_package(Qt5LinguistTools REQUIRED)
        #find_package(Qt5Qml REQUIRED)
        #find_package(Qt5Quick REQUIRED)
        #find_package(Qt5QuickWidgets REQUIRED)
	#find_package(Qt5OpenGl REQUIRED)
        find_package(Qt5Network)
        #find_package(Qt5Concurrent REQUIRED)
	include(${CMAKE_MODULE_PATH}/ECMQt4To5Porting.cmake)
	ADD_DEFINITIONS("-DWITH_QT5")
	SET (QT_VERSIONS Qt5)
ELSE (WITH_QT5)
	find_package(Qt4 REQUIRED)
	include(${QT_USE_FILE})
	SET (QT_VERSIONS Qt4)
ENDIF (WITH_QT5)

MESSAGE(STATUS "${QT5_WRAP_UI}")
find_package(CTK REQUIRED)

#关闭boost自动链接
#ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")
#output窗口中输出链接boost库版本以及链接顺序。
ADD_DEFINITIONS("-DBOOST_LIB_DIAGNOSTIC")

#if(WIN32)
#    set(Boost_USE_STATIC_LIBS ON)
#endif()

#set(Boost_USE_MULTITHREAD ON)
#find_package(Boost REQUIRED COMPONENTS exception)

#SET(BOOST_EXCEPTION_LIBRARY ${Boost_EXCEPTION_LIBRARY_DEBUG})
#MESSAGE(STATUS ">>>>>>>>${BOOST_EXCEPTION_LIBRARY}<<<<<<<<<<<<")

#Boost相关的三个变量 
#MESSAGE(STATUS "${Boost_INCLUDE_DIR}\n" 
#"Boost_LIBRARY_DIR = ${Boost_LIBRARY_DIR}"
#"${Boost_LIBRARY_DIR_DEBUG}\n"
#"${Boost_LIBRARY_DIR_RELEASE}\n"
#"${Boost_SERIALIZATION_LIBRARY_DEBUG}\n"
#"${Boost_SERIALIZATION_LIBRARY_RELEASE}")

#SET(Boost_LIBRARY_DIR ${Boost_SERIALIZATION_LIBRARY_DEBUG})
#MESSAGE(STATUS ">>>>>>>>${Boost_LIBRARY_DIR}<<<<<<<<<<<<")

find_package(CURL REQUIRED)

#find_package(Log4cpp REQUIRED)

#ADD_DEFINITIONS("-DCURL_STATICLIB")

# Qt翻译家
FIND_PROGRAM(QT_LRELEASE_EXECUTABLE
    NAMES lrelease
    PATHS $ENV{PATH}
    NO_DEFAULT_PATH
    )

# 编译版本设置
# IF (NOT CMAKE_BUILD_TYPE)
  # message(STATUS "Setting build type to 'Debug' as none was specified.")
  # set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # # mark_as_advanced(CMAKE_BUILD_TYPE)
  # # Set the possible values of build type for cmake-gui
  # set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
# endif()

#SET (BUILD_TYPE_DEBUG TRUE CACHE BOOL "Determines build version")
# IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
	# SET(DEST_BUILD_VERSION Debug)
	# SET(QT_SUFFIX d)
# ELSE (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# SET(DEST_BUILD_VERSION Release)
	# SET(QT_SUFFIX )
# ENDIF()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

# 设置导出宏
IF (WIN32)
  SET(DLLIMPORT "__declspec(dllimport)")
  SET(DLLEXPORT "__declspec(dllexport)")
  SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
  SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO")
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
  SET(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
  
  ADD_DEFINITIONS("-DWIN32_LEAN_AND_MEAN")
ELSE (WIN32)
  IF(PEDANTIC AND NOT APPLE)
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
    SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--no-undefined")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined")
  ENDIF(PEDANTIC AND NOT APPLE)

  SET(DLLIMPORT "")
  SET(DLLEXPORT "")
ENDIF(WIN32)

#是否开启内存泄露检查
SET (WITH_CHECK_MEM_LEAK FALSE CACHE BOOL "Check memory leak")
IF (WITH_CHECK_MEM_LEAK)
  FIND_PACKAGE(VLD)  
  SET(VLD_LIB ${VLD_LIBRARY})
  SET(VLD_INCLUDE ${VLD_INCLUDE_DIR})
  ADD_DEFINITIONS("-DWITH_MEM_CHECK")
ELSE (WITH_CHECK_MEM_LEAK)
  SET(VLD_LIB "")
  SET(VLD_INCLUDE "")
ENDIF (WITH_CHECK_MEM_LEAK)

#设置工程变量
SET(RadarProject ${CMAKE_SOURCE_DIR})
SET(RadarProjectInclude ${RadarProject}/include)
#SET(RadarProjectQml ${RadarProject}/qml)
SET(RadarProjectRunTimeLib ${RadarProject}/3dpart/${QT_VERSIONS}/bin/${DEST_BUILD_VERSION})
SET(RadarProjectLib ${RadarProject}/3dpart/${QT_VERSIONS}/lib/${DEST_BUILD_VERSION})
#SET(GisPluginDir ${RadarProject}/gisplugins/${DEST_BUILD_VERSION})

#这段用来查找插件存放的目录，解决编译调试版本和最终发布版本插件路径不一致的问题
#此处不存储插件路径，而存储系统的构建路径
SET(ctkPluginHomeDir ${CMAKE_BINARY_DIR}/bin/plugins/${DEST_BUILD_VERSION})
#文件操作命令
FILE(WRITE ${CMAKE_BINARY_DIR}/bin/radarprojectbuildpath.txt "${CMAKE_BINARY_DIR}/bin/")

#待翻译的文件，定义全局变量
SET(FILES_TO_TRANSLATE)

#综合显示客户端翻译文件变量
SET(RADAR_MOSAIC_CLIENT_FILES_TO_TRANSLATE)

#数据传输客户端翻译文件变量
SET(RADAR_DATATRANSFER_FILES_TO_TRANSLATE)

#雷达组网拼图应用处理系统翻译文件全局变量
SET(RADAR_SERVER_V2_FILES_TO_TRANSLATE)

#消息服务翻译文件全局变量
SET(RADAR_MESSAGECENTRE_FILES_TO_TRANSLATE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/library)	#基础依赖库

# ctk相关
#> Extract all library names starting with org_mydomain_
macro(GetMyTargetLibraries all_target_libraries  varname)
  set(re_ctkplugin "^org_[a-zA-Z0-9_]+$")
  set(_tmp_list)
  list(APPEND _tmp_list ${all_target_libraries})
  ctkMacroListFilter(_tmp_list re_ctkplugin OUTPUT_VARIABLE ${varname})
endmacro()
#>

#ctk插件配置

#> Create a list of available plug-ins and make them known to the build system
 set(plugins
   plugins/org.common.quartz:ON
   #plugins/org.common.crypt:ON
   plugins/org.radar.datacollection:ON
   plugins/org.radar.dataclear:ON
 )
 ctkMacroSetupPlugins(${plugins})
#>

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/apps)		#应用
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/i18n) #国际化


#拷贝相应的配置文件
#判断在程序运行目录下是否存在需要的配置文件
SET(SUFFIC "-")
#匹配生成一个文件list，存储到变量MYCONFIGFILE中
FILE(GLOB MYCONFIGFILE ${PROJECT_BINARY_DIR}/bin/${DEST_BUILD_VERSION}/config)
SET (MYCONFIGFILE ${MYCONFIGFILE}${SUFFIC})
MESSAGE(STATUS "MYCONFIGFILE = ${MYCONFIGFILE}")
 IF (${MYCONFIGFILE} STREQUAL ${SUFFIC})
	 #判断构建下面的目录是否存在
	 #将config文件夹复制到目标位置
	 FILE(INSTALL config 
		 DESTINATION ${PROJECT_BINARY_DIR}/bin/${DEST_BUILD_VERSION})
	 #FILE(INSTALL ${RadarProjectRunTimeLib}/
	#	 DESTINATION ${PROJECT_BINARY_DIR}/bin/${DEST_BUILD_VERSION})
 ENDIF()


INSTALL(DIRECTORY config
		DESTINATION bin
)

INSTALL(FILES ${RadarProject}/protect.sh
		DESTINATION bin
)
INSTALL(FILES ${RadarProject}/start.sh
		DESTINATION bin
)

INSTALL(FILES ${RadarProject}/daemon.vbs
		DESTINATION bin
)
INSTALL(FILES ${RadarProject}/guard.bat
		DESTINATION bin
)
