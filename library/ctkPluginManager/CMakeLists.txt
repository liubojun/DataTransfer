#############################################################
# sources

SET(PROJECT_NAME ctkPluginManager)

SET(SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/pluginmanagerImp.cpp	
)

SET(HDRS
	${RadarProjectInclude}/ctkPluginManager/ICtkPluginManager.h
	${CMAKE_CURRENT_SOURCE_DIR}/pluginmanagerImp.h	
)

#设置MOC相关变量
SET(MOC_HDRS
	${RadarProjectInclude}/ctkPluginManager/ICtkPluginManager.h
	${CMAKE_CURRENT_SOURCE_DIR}/pluginmanagerImp.h	
)

QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})

#添加依赖的头文件的路径
INCLUDE_DIRECTORIES(
					${CTK_INCLUDE_DIRS}
					${RadarProjectInclude}
					${RadarProjectInclude}/ctkPluginManager
					${RadarProjectInclude}/interface/dataservice					
					${RadarProjectInclude}/interface
					${RadarProjectInclude}/3dpart/QParse
					${RadarProjectInclude}/3dpart/Common
					${RadarProjectInclude}/QsLog
					${RadarProjectInclude}/ctkPublicFun
					${RadarProjectInclude}/3dpart/zeromq
					${RadarProjectInclude}/ctkProtocol
					${RadarProjectInclude}/org.radar.servermessage					
					${RadarProjectInclude}/org.radar.clientmessage
					${RadarProjectInclude}/org.radar.radarprocess
					${RadarProjectInclude}/org.radar.radarmosaic
					${RadarProjectInclude}/org.radar.dataadapter
					${VLD_INCLUDE}
					)

#SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#为源文件的编译添加由-D引入的define flag
add_definitions(-DQT_PLUGIN)
add_definitions(-DQT_SHARED)

#设置需要翻译的源文件				
SET(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} ${SRCS} ${HDRS} PARENT_SCOPE)

#生成共享库
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS} ${HDRS} ${MOC_SRCS})

#查找以来的库
IF (WIN32)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	  ${QT_QTMAIN_LIBRARY}
	  ${QT_QTCORE_LIBRARY}
	  ${QT_QTGUI_LIBRARY} 
	  ${QT_UITOOLS_LIBRARY}
	  ${QT_QTSQL_LIBRARY}
	  CTKPluginFramework
	  ${VLD_LIB}
	  ctkLogger
	  ctkPublicFun
	)
ELSE (WIN32)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	  ${QT_QTMAIN_LIBRARY}
	  ${QT_QTCORE_LIBRARY}
	  ${QT_QTGUI_LIBRARY} 
	  ${QT_UITOOLS_LIBRARY}
      ${QT_QTSQL_LIBRARY}
	  ${VLD_LIB}
      ctkLogger
      CTKPluginFramework
	)
ENDIF (WIN32)


#将生成文件输出到指定的目录中
IF (WIN32)
    INSTALL(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION bin/plugins/ctkPluginManager
            LIBRARY DESTINATION bin/plugins/ctkPluginManager)
ELSE (WIN32)
    INSTALL(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION bin)
ENDIF (WIN32)

		
# 输出头文件
# INSTALL(FILES ${RADAR_ROOT_PATH}/include/commontypedef.h 
			  # ${RADAR_ROOT_PATH}/include/macro.h
		# DESTINATION include)
		
# INSTALL(DIRECTORY ${RADAR_ROOT_PATH}/include/QParse DESTINATION include)		

#(FILES ${RadarProjectRunTimeLib}/CTKCore.dll
#			  ${RadarProjectRunTimeLib}/CTKPluginFramework.dll
#		DESTINATION bin)	

IF (WIN32)
	INSTALL(FILES ${RadarProjectRunTimeLib}/CTKCore.dll
			  ${RadarProjectRunTimeLib}/CTKPluginFramework.dll
		DESTINATION bin)
ELSE (WIN32)
	INSTALL(FILES ${RadarProjectRunTimeLib}/libCTKCore.so
			  ${RadarProjectRunTimeLib}/libCTKPluginFramework.so
		DESTINATION bin)
ENDIF (WIN32)


# 安装sqldriver驱动
INSTALL(DIRECTORY ${RadarProjectRunTimeLib}/sqldrivers	
		DESTINATION bin)

