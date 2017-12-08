#############################################################
# sources
SET(PROJECT_NAME ctkFtpTransfer)

# 工程实现文件
SET(SRCS
    curlftp.cpp
)

# 工程头文件
SET(HDRS
   curlftp.h
)

#设置MOC相关变量
SET(MOC_HDRS
	curlftp.h
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
					${CURL_INCLUDE_DIR}
					)
							

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
		${CURL_LIBRARY}
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
		${CURL_LIBRARY}
        #	${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libCTKPluginFramework.so
	)
ENDIF (WIN32)


#将生成文件输出到指定的目录中
INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION bin)
