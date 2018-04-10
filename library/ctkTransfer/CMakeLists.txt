#############################################################
# sources
SET(PROJECT_NAME ctkTransfer)

# 工程实现文件
SET(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/curlftp.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CollectorBase.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ftpCollector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shareddirCollector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DistributeFile.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/record.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LibCurlFtp.cpp
)

# 工程头文件
SET(HDRS
   ${RadarProjectInclude}/ctkTransfer/curlftp.h
   ${RadarProjectInclude}/ctkTransfer/CollectorBase.h
   ${RadarProjectInclude}/ctkTransfer/ftpCollector.h
   ${RadarProjectInclude}/ctkTransfer/shareddirCollector.h
   ${RadarProjectInclude}/ctkTransfer/LibCurlFtp.h
   ${CMAKE_CURRENT_SOURCE_DIR}/DistributeFile.h
   ${CMAKE_CURRENT_SOURCE_DIR}/record.h
   
)

#设置MOC相关变量
SET(MOC_HDRS
	${RadarProjectInclude}/ctkTransfer/curlftp.h
	${RadarProjectInclude}/ctkTransfer/CollectorBase.h
	${RadarProjectInclude}/ctkTransfer/ftpCollector.h
    ${RadarProjectInclude}/ctkTransfer/shareddirCollector.h
	${CMAKE_CURRENT_SOURCE_DIR}/DistributeFile.h
	${CMAKE_CURRENT_SOURCE_DIR}/record.h
)

IF (WITH_QT5)
	ADD_DEFINITIONS("-DZ_PREFIX")
ENDIF (WITH_QT5)

QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})

#添加依赖的头文件的路径
INCLUDE_DIRECTORIES(
					${RadarProjectInclude}
					${RadarProjectInclude}/ctkPublicFun
					${RadarProjectInclude}/ctkTransfer
					${RadarProjectInclude}/org.radar.drawproduct
					${RadarProjectInclude}/org.common.quartz
					#${RadarProjectInclude}/org.radar.dataadapter
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
		#${QT_QTNETWORK_LIBRARY}
		CTKPluginFramework	
		ctkLogger
		ctkPublicFun
		RCF
		Mpr
		${CURL_LIBRARY}
	)
ELSE (WIN32)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}
		${QT_QTMAIN_LIBRARY}
		${QT_QTCORE_LIBRARY}
		${QT_UITOOLS_LIBRARY}
        ${QT_QTSQL_LIBRARY}
		#${QT_QTNETWORK_LIBRARY}
		${VLD_LIB}
		${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libctkLogger.so	#需要明确路径
        CTKPluginFramework
		ctkPublicFun
		RCF
		${CURL_LIBRARY}
	)
ENDIF (WIN32)


#将生成文件输出到指定的目录中
INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION bin)
