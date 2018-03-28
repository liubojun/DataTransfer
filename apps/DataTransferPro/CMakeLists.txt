PROJECT(DataTransferPro)

# cpp 文件
SET(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# .h文件
SET(HDRS
	${RadarProjectInclude}/ctkTransfer/CollectorBase.h
	${RadarProjectInclude}/ctkTransfer/ftpCollector.h
    ${RadarProjectInclude}/ctkTransfer/shareddirCollector.h
)

SET(MOC_HDRS
	${RadarProjectInclude}/ctkTransfer/CollectorBase.h
	${RadarProjectInclude}/ctkTransfer/ftpCollector.h
    ${RadarProjectInclude}/ctkTransfer/shareddirCollector.h
  
  #${RadarProjectInclude}/interface/ILogPrint.h
  
)

SET(UI

	)

SET (ICON_SRCS  )

QT4_WRAP_CPP(MOC_SRCS  ${MOC_HDRS})

QT4_WRAP_UI(UI_H ${UI})

QT4_ADD_RESOURCES(RESOUCES 
				 )

INCLUDE_DIRECTORIES(
					${CTK_INCLUDE_DIRS}
					${CMAKE_CURRENT_BINARY_DIR}		# 程序生成目录
					${RadarProjectInclude}
					${RadarProjectInclude}/interface
					${RadarProjectInclude}/QsLog
					${RadarProjectInclude}/ctkPublicFun
					${RadarProjectInclude}/ctkProtocol
					${RadarProjectInclude}/ctkTransfer
					${CURL_INCLUDE_DIR}
					${VLD_INCLUDE}
					${RadarProjectInclude}/3dpart
					${RadarProjectInclude}/3dpart/RCF-2.2.0.0/include
				)


#IF (WIN32)
#    SET(CMAKE_CXX_FLAGS  /DWIN32 /D_WINDOWS /W3 /GR)
#ENDIF (WIN32)

message(status "${CMAKE_CXX_FLAGS}")
#设置需要翻译的源文件				
#SET(PLUGINS_FILES_TO_TRANSLATE ${RADAR_DATATRANSFER_FILES_TO_TRANSLATE} ${SRCS} ${HDRS} PARENT_SCOPE)

#将指定的文件编译成可执行文件，命名为RadarServers
LINK_DIRECTORIES(${RadarProjectLib})

ADD_EXECUTABLE(${PROJECT_NAME} ${HDRS} ${SRCS} ${MOC_SRCS} ${UI_H} ${ICON_SRCS} ${RESOUCES})

#ADD_EXECUTABLE(PROJECT_NAME ${HDRS} ${SRCS} ${MOC_SRCS} ${UI_H} ${ICON_SRCS} ${RESOUCES})

message(STATUS "CURL_LIBRARY == ${CURL_LIBRARY}")
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
        ${QT_QTMAIN_LIBRARY}
        ${QT_QTCORE_LIBRARY}
        ${QT_QTGUI_LIBRARY}
        ${QT_QTUITOOLS_LIBRARY}
        ctkLogger
        ctkPublicFun
		ctkTransfer
        ${VLD_LIB}
		#RCF
		
        )



INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin)
