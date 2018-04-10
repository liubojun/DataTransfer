PROJECT(DataTransferConsole)

# cpp 文件
SET(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${RadarProject}/apps/DataTransfer/mainwindow.cpp
  ${RadarProject}/apps/DataTransfer/collectSetDlg.cpp
  ${RadarProject}/apps/DataTransfer/userInfoDlg.cpp
  ${RadarProject}/apps/DataTransfer/selUserDlg.cpp
  ${RadarProject}/apps/DataTransfer/listItemWidget.cpp
  ${RadarProject}/apps/DataTransfer/userItem.cpp
  ${RadarProject}/apps/DataTransfer/dataclearDlg.cpp
  ${RadarProject}/apps/DataTransfer/clearItemWidget.cpp
  ${RadarProject}/apps/DataTransfer/regexpression.cpp
  ${RadarProject}/apps/DataTransfer/renameDlg.cpp
  ${RadarProject}/apps/DataTransfer/status.cpp
  ${RadarProject}/apps/DataTransfer/quartz.cpp
  ${RadarProject}/apps/DataTransfer/logprint.cpp
  ${RadarProject}/apps/DataTransfer/systemset.cpp
  ${RadarProject}/apps/DataTransfer/baseset.cpp
  ${RadarProject}/apps/DataTransfer/subdirtemplateedit.cpp
)

# .h文件
SET(HDRS
  ${RadarProject}/apps/DataTransfer/mainwindow.h
  ${RadarProject}/apps/DataTransfer/collectSetDlg.h
  ${RadarProject}/apps/DataTransfer/userInfoDlg.h
  ${RadarProject}/apps/DataTransfer/selUserDlg.h
  ${RadarProject}/apps/DataTransfer/listItemWidget.h
  ${RadarProject}/apps/DataTransfer/userItem.h
  ${RadarProject}/apps/DataTransfer/dataclearDlg.h
  ${RadarProject}/apps/DataTransfer/clearItemWidget.h
  ${RadarProject}/apps/DataTransfer/regexpression.h
  ${RadarProject}/apps/DataTransfer/renameDlg.h
  ${RadarProject}/apps/DataTransfer/status.h
  ${RadarProject}/apps/DataTransfer/quartz.h
  ${RadarProject}/apps/DataTransfer/logprint.h
  ${RadarProject}/apps/DataTransfer/systemset.h
  ${RadarProject}/apps/DataTransfer/baseset.h
  ${RadarProject}/apps/DataTransfer/subdirtemplateedit.h
)

SET(MOC_HDRS
	${RadarProjectInclude}/interface/IBaseInterface.h
	${RadarProject}/apps/DataTransfer/mainwindow.h
	${RadarProject}/apps/DataTransfer/collectSetDlg.h
	${RadarProject}/apps/DataTransfer/userInfoDlg.h
	${RadarProject}/apps/DataTransfer/selUserDlg.h
	${RadarProject}/apps/DataTransfer/listItemWidget.h
	${RadarProject}/apps/DataTransfer/userItem.h
	${RadarProject}/apps/DataTransfer/dataclearDlg.h
	${RadarProject}/apps/DataTransfer/clearItemWidget.h
	${RadarProject}/apps/DataTransfer/regexpression.h
	${RadarProject}/apps/DataTransfer/renameDlg.h
	${RadarProject}/apps/DataTransfer/status.h
	${RadarProject}/apps/DataTransfer/quartz.h
	${RadarProject}/apps/DataTransfer/logprint.h
	${RadarProject}/apps/DataTransfer/systemset.h
	${RadarProject}/apps/DataTransfer/baseset.h
	${RadarProject}/apps/DataTransfer/subdirtemplateedit.h
)

SET(UI
	${RadarProject}/apps/DataTransfer/ui/mainwindow.ui
	${RadarProject}/apps/DataTransfer/ui/collect.ui
	${RadarProject}/apps/DataTransfer/ui/selUser.ui
	${RadarProject}/apps/DataTransfer/ui/userManage.ui
	${RadarProject}/apps/DataTransfer/ui/itemWidget.ui
	${RadarProject}/apps/DataTransfer/ui/userItemWidget.ui
	${RadarProject}/apps/DataTransfer/ui/dataclear.ui
	${RadarProject}/apps/DataTransfer/ui/clearItemWidget.ui
	${RadarProject}/apps/DataTransfer/ui/RegularExpression.ui
	${RadarProject}/apps/DataTransfer/ui/renameDlg.ui
	${RadarProject}/apps/DataTransfer/ui/systemset.ui
	${RadarProject}/apps/DataTransfer/ui/baseset.ui
	${RadarProject}/apps/DataTransfer/ui/subdirtemplateedit.ui
	)

SET (ICON_SRCS  ${RadarProject}/apps/DataTransfer/Transfer.rc)

QT4_WRAP_CPP(MOC_SRCS  ${MOC_HDRS})

QT4_WRAP_UI(UI_H ${UI})

QT4_ADD_RESOURCES(RESOUCES #${RadarProject}/qml/FileTransfer/qml.qrc
						   ${RadarProject}/apps/DataTransfer/res/Icons.qrc
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
					${RadarProjectInclude}/org.common.filesystemwatcher
					${RadarProjectInclude}/org.radar.datacollection
					${RadarProjectInclude}/org.common.quartz
					${RadarProjectInclude}/org.radar.filedistribute
					${RadarProjectInclude}/interface
					${RadarProjectInclude}/QsLog
					${RadarProjectInclude}/ctkPublicFun
					${RadarProjectInclude}/ctkProtocol
					${RadarProject}/apps/DataTransfer
					${CURL_INCLUDE_DIR}
					${RadarProjectInclude}/3dpart
					${RadarProjectInclude}/3dpart/RCF-2.2.0.0/include
					${VLD_INCLUDE}
				)

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
        ${QT_QTQML_LIBRARY}
        ${QT_QTQUICK_LIBRARY}
		#${QT_QTNETWORK_LIBRARY}
        ctkLogger
        ctkPublicFun
        ctkPluginManager
        ${VLD_LIB}
        CTKPluginFramework
        ${CURL_LIBRARY}
		RCF
        )

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
