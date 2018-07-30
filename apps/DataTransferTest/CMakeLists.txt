PROJECT(DataTransferTest)

# cpp 文件
SET(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# .h文件
SET(HDRS
	${RadarProjectInclude}/ctkTransfer/CollectorBase.h
	${RadarProjectInclude}/ctkTransfer/ftpCollector.h
	${RadarProjectInclude}/ctkTransfer/sftpCollector.h
    ${RadarProjectInclude}/ctkTransfer/shareddirCollector.h
)

SET(MOC_HDRS
	${RadarProjectInclude}/ctkTransfer/CollectorBase.h
	${RadarProjectInclude}/ctkTransfer/ftpCollector.h
	${RadarProjectInclude}/ctkTransfer/sftpCollector.h
    ${RadarProjectInclude}/ctkTransfer/shareddirCollector.h
  
  
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


message(status "${CMAKE_CXX_FLAGS}")


#将指定的文件编译成可执行文件，命名为RadarServers
LINK_DIRECTORIES(${RadarProjectLib})

ADD_EXECUTABLE(${PROJECT_NAME} ${HDRS} ${SRCS} ${MOC_SRCS} ${UI_H} ${ICON_SRCS} ${RESOUCES})

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
