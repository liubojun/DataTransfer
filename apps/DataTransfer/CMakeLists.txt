PROJECT(DataTransfer)

# cpp 文件
SET(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/collectSetDlg.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/userInfoDlg.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/selUserDlg.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/listItemWidget.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/userItem.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/DataBase.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/Indirect.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/DistributeUserInfo.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/MinimizedWin.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/DataBase.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/TransferMonitor.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/MyApplication.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/TaskThread.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/ConToMsg.cpp
  #${MYQML}
  ${CMAKE_CURRENT_SOURCE_DIR}/dataclearDlg.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/clearItemWidget.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/regexpression.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/renameDlg.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/status.cpp
)

# .h文件
SET(HDRS
  #${CMAKE_CURRENT_SOURCE_DIR}/CollectionInfo.h
  #${RadarProjectInclude}/interface/IBaseInterface.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
  ${CMAKE_CURRENT_SOURCE_DIR}/collectSetDlg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/userInfoDlg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/selUserDlg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/listItemWidget.h
  ${CMAKE_CURRENT_SOURCE_DIR}/userItem.h
  #${CMAKE_CURRENT_SOURCE_DIR}/DataBase.h
  ${CMAKE_CURRENT_SOURCE_DIR}/dataclearDlg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/clearItemWidget.h
  ${CMAKE_CURRENT_SOURCE_DIR}/regexpression.h
  ${CMAKE_CURRENT_SOURCE_DIR}/renameDlg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/status.h
)

SET(MOC_HDRS
	${RadarProjectInclude}/interface/IBaseInterface.h
	#${RadarProjectInclude}/interface/IFileTransfer.h
	${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
	${CMAKE_CURRENT_SOURCE_DIR}/collectSetDlg.h
	${CMAKE_CURRENT_SOURCE_DIR}/userInfoDlg.h
	${CMAKE_CURRENT_SOURCE_DIR}/selUserDlg.h
	${CMAKE_CURRENT_SOURCE_DIR}/listItemWidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/userItem.h
	#${CMAKE_CURRENT_SOURCE_DIR}/DataBase.h
	#${CMAKE_CURRENT_SOURCE_DIR}/CollectionInfo.h
	#${CMAKE_CURRENT_SOURCE_DIR}/Indirect.h
	#${CMAKE_CURRENT_SOURCE_DIR}/DistributeUserInfo.h
	#${CMAKE_CURRENT_SOURCE_DIR}/MinimizedWin.h
	#${CMAKE_CURRENT_SOURCE_DIR}/DataBase.h
	#${CMAKE_CURRENT_SOURCE_DIR}/TransferMonitor.h
	#${CMAKE_CURRENT_SOURCE_DIR}/MyApplication.h
	#${CMAKE_CURRENT_SOURCE_DIR}/TaskThread.h
	#${CMAKE_CURRENT_SOURCE_DIR}/ConToMsg.h
	${CMAKE_CURRENT_SOURCE_DIR}/dataclearDlg.h
	${CMAKE_CURRENT_SOURCE_DIR}/clearItemWidget.h
	${CMAKE_CURRENT_SOURCE_DIR}/regexpression.h
	${CMAKE_CURRENT_SOURCE_DIR}/renameDlg.h
	${CMAKE_CURRENT_SOURCE_DIR}/status.h
)

SET(UI
	${CMAKE_CURRENT_SOURCE_DIR}/ui/mainwindow.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ui/collect.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ui/selUser.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ui/userManage.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ui/itemWidget.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ui/userItemWidget.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ui/dataclear.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ui/clearItemWidget.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ui/RegularExpression.ui
	${CMAKE_CURRENT_SOURCE_DIR}/ui/renameDlg.ui
	)

SET (ICON_SRCS  ${CMAKE_CURRENT_SOURCE_DIR}/Transfer.rc)

QT4_WRAP_CPP(MOC_SRCS  ${MOC_HDRS})

QT4_WRAP_UI(UI_H ${UI})

QT4_ADD_RESOURCES(RESOUCES #${RadarProject}/qml/FileTransfer/qml.qrc
						   ${CMAKE_CURRENT_SOURCE_DIR}/res/Icons.qrc
				 )

INCLUDE_DIRECTORIES(
					${CTK_INCLUDE_DIRS}
					${CMAKE_CURRENT_BINARY_DIR}		# 程序生成目录
					${RadarProjectInclude}
					${RadarProjectInclude}/org.radar.communication
					${RadarProjectInclude}/org.radar.dataadapter
					${RadarProjectInclude}/org.radar.radarmosaic
					${RadarProjectInclude}/ctkPluginManager
					${RadarProjectInclude}/interface/dataservice			
					${RadarProjectInclude}/org.radar.dataclear
					#${RadarProjectInclude}/org.radar.radarprocess
					#${RadarProjectInclude}/org.radar.tlogP
					${RadarProjectInclude}/org.common.filesystemwatcher
					${RadarProjectInclude}/org.radar.datacollection
					${RadarProjectInclude}/org.common.quartz
					${RadarProjectInclude}/org.radar.filedistribute
					${RadarProjectInclude}/interface
					${RadarProjectInclude}/QsLog
					${RadarProjectInclude}/ctkPublicFun
					${RadarProjectInclude}/ctkProtocol
					${CURL_INCLUDE_DIR}
					#${RadarProjectInclude}/3dpart/zeromq
					${VLD_INCLUDE}
				)


IF (WIN32)
    SET(CMAKE_CXX_FLAGS  /DWIN32 /D_WINDOWS /W3 /GR)
ENDIF (WIN32)

message(status "${CMAKE_CXX_FLAGS}")
#设置需要翻译的源文件				
#SET(PLUGINS_FILES_TO_TRANSLATE ${RADAR_DATATRANSFER_FILES_TO_TRANSLATE} ${SRCS} ${HDRS} PARENT_SCOPE)

#将指定的文件编译成可执行文件，命名为RadarServers
LINK_DIRECTORIES(${RadarProjectLib})

ADD_EXECUTABLE(${PROJECT_NAME} WIN32 ${HDRS} ${SRCS} ${MOC_SRCS} ${UI_H} ${ICON_SRCS} ${RESOUCES})

#ADD_EXECUTABLE(PROJECT_NAME ${HDRS} ${SRCS} ${MOC_SRCS} ${UI_H} ${ICON_SRCS} ${RESOUCES})

message(STATUS "CURL_LIBRARY == ${CURL_LIBRARY}")

    TARGET_LINK_LIBRARIES(${PROJECT_NAME}
            ${QT_QTMAIN_LIBRARY}
            ${QT_QTCORE_LIBRARY}
            ${QT_QTGUI_LIBRARY}
            ${QT_QTUITOOLS_LIBRARY}
            ${QT_QTQML_LIBRARY}
            ${QT_QTQUICK_LIBRARY}
            ctkLogger
            ctkPublicFun
            ctkPluginManager
            ${VLD_LIB}
            CTKPluginFramework
            ${CURL_LIBRARY}
            )

#TARGET_LINK_LIBRARIES(DataTransfer_DebugInfo
	#${QT_QTMAIN_LIBRARY}
	#${QT_QTCORE_LIBRARY}
	#${QT_QTGUI_LIBRARY}
	#${QT_QTUITOOLS_LIBRARY}
	#${QT_QTQML_LIBRARY}
	#${QT_QTQUICK_LIBRARY}
	#ctkLogger
	#ctkPublicFun
	#ctkProtocol
	#${VLD_LIB}
	#CTKPluginFramework
	#libcurl_imp
	#)

INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin)

#INSTALL(TARGETS DataTransfer_DebugInfo
		#RUNTIME DESTINATION bin)

#INSTALL(TARGETS ${PROJECT_NAME}_debugInfo
#		RUNTIME DESTINATION bin)
		
#安装翻译文件
#INSTALL(FILES 
		#${CMAKE_BINARY_DIR}/bin/i18n/FileTransfer_zh.qm
		#${CMAKE_BINARY_DIR}/bin/i18n/FileTransfer_en.qm
		#DESTINATION bin/i18n
#		)		
