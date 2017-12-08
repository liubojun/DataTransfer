#############################################################
# sources
SET(PROJECT_NAME ctkPublicFun)

# 工程实现文件
SET(SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/ctkPublicFun.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/pathbuilder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ginformation.cpp	
	${CMAKE_CURRENT_SOURCE_DIR}/dllloader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DataBase.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/change_name.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/get_rename_rules.cpp
)

# 工程头文件
SET(HDRS
	${RadarProjectInclude}/ctkPublicFun/ctkPublicFun.h
	${RadarProjectInclude}/ctkPublicFun/pathbuilder.h
	${RadarProjectInclude}/ctkPublicFun/language.h
	${RadarProjectInclude}/ctkPublicFun/ginformation.h
	${RadarProjectInclude}/ctkPublicFun/publicthread.h
	${RadarProjectInclude}/ctkPublicFun/dllloader.h
	${RadarProjectInclude}/ctkPublicFun/event_macro.h
	${RadarProjectInclude}/ctkPublicFun/pupdata.h  
	${RadarProjectInclude}/ctkPublicFun/DataBase.h
	${RadarProjectInclude}/ctkPublicFun/change_name.h
	${RadarProjectInclude}/ctkPublicFun/logprotocol.h
)

#设置MOC相关变量
SET(MOC_HDRS
	${RadarProjectInclude}/ctkPublicFun/DataBase.h
)

IF (WITH_QT5)
	ADD_DEFINITIONS("-DZ_PREFIX")
ENDIF (WITH_QT5)

QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})

#添加依赖的头文件的路径
INCLUDE_DIRECTORIES(
					${RadarProjectInclude}
					${RadarProjectInclude}/ctkPublicFun
					${RadarProjectInclude}/org.radar.drawproduct
					${RadarProjectInclude}/org.radar.dataadapter
					${RadarProjectInclude}/QsLog
					${RadarProjectInclude}/ctkProtocol
					${RadarProjectInclude}/3dpart
					${RadarProjectInclude}/3dpart/RCF-2.2.0.0/include
					${VLD_INCLUDE}
					)
					
#设置需要翻译的源文件				
SET(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} ${SRCS} ${HDRS} PARENT_SCOPE)				

LINK_DIRECTORIES(${RadarProjectLib})

#设置为非console工程 WIN32为关键字
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS} ${HDRS} ${MOC_SRCS})

#查找依赖的库
IF (WIN32)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}
		${QT_QTMAIN_LIBRARY}
		${QT_QTCORE_LIBRARY}
		${QT_UITOOLS_LIBRARY}
		${VLD_LIB}	#VLD内存泄露检查
		CTKPluginFramework	
		ctkLogger
		Mpr
	)
ELSE (WIN32)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}
		${QT_QTMAIN_LIBRARY}
		${QT_QTCORE_LIBRARY}
		${QT_UITOOLS_LIBRARY}
         ${QT_QTSQL_LIBRARY}
		${VLD_LIB}
		${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libctkLogger.so	#需要明确路径
      CTKPluginFramework
        #	${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libCTKPluginFramework.so
	)
ENDIF (WIN32)


#将生成文件输出到指定的目录中
INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION bin)
