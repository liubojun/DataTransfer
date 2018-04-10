
SET(PROJECT_NAME RCF)

SET(SRCS
	${RadarProjectInclude}/3dpart/RCF-2.2.0.0/src/RCF/RCF.cpp
)

SET(HDRS
	)

IF (WIN32)
	ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)
	ADD_DEFINITIONS(-D_WIN32_WINNT=0x0500)
	ADD_DEFINITIONS(-DRCF_BUILD_DLL)
ELSE (WIN32)
ENDIF (WIN32)


#添加依赖的头文件的路径
INCLUDE_DIRECTORIES(
					${RadarProjectInclude}
					${RadarProjectInclude}/3dpart
					${RadarProjectInclude}/3dpart/RCF-2.2.0.0/include
					)
						
#设置为非console工程 WIN32为关键字
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS} ${HDRS} ${MOC_SRCS})

#查找依赖的库
IF (WIN32)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        CTKPluginFramework
        ws2_32
)
ENDIF(WIN32)

#将生成文件输出到指定的目录中
INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION bin)
