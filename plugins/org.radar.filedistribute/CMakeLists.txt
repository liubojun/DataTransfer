PROJECT(org_radar_filedistribute)

SET(PLUGIN_export_directive "org_radar_filedistribute_EXPORT")

set(PLUGIN_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/FileDistributeActivator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/FileDistribute.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/FileDistributeThread.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/FileCrypt.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/curlftp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/DistributeTaskThread.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/userManageDlg.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/cviewHeader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/param.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/userInfo.cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/transferSetDlg.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sendPrdcSetDlg.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/selUserDlg.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/addModel.cpp
  #${RadarProjectInclude}/ctkFtp/qurlinfo.cpp
  #${RadarProjectInclude}/ctkFtp/qftp.cpp
)

set(PLUGIN_HDRS
  ${RadarProjectInclude}/interface/IBaseInterface.h
  ${RadarProjectInclude}/org.radar.filedistribute/IFileDistribute.h
  ${RadarProjectInclude}/org.radar.filedistribute/IFileCrypt.h
  ${RadarProjectInclude}/org.radar.filedistribute/IDistributeManager.h
  ${RadarProjectInclude}/ctkProtocol/transFileInfo.h
  ${RadarProjectInclude}/ctkProtocol/fileMsg.h
  ${RadarProjectInclude}/ctkProtocol/transferSet.h
  ${CMAKE_CURRENT_SOURCE_DIR}/FileDistributeActivator.h  
  ${CMAKE_CURRENT_SOURCE_DIR}/FileDistribute.h
  ${CMAKE_CURRENT_SOURCE_DIR}/FileDistributeThread.h
  ${CMAKE_CURRENT_SOURCE_DIR}/FileCrypt.h
  ${CMAKE_CURRENT_SOURCE_DIR}/curlftp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/DistributeTaskThread.h
  ${CMAKE_CURRENT_SOURCE_DIR}/userManageDlg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cviewHeader.h
  ${CMAKE_CURRENT_SOURCE_DIR}/param.h
  ${CMAKE_CURRENT_SOURCE_DIR}/userInfo.h
  #${CMAKE_CURRENT_SOURCE_DIR}/transferSetDlg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/sendPrdcSetDlg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/selUserDlg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/addModel.h
)

# Files which should be processed by Qts moc
set(PLUGIN_MOC_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/FileDistributeActivator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/FileCrypt.h
  ${CMAKE_CURRENT_SOURCE_DIR}/curlftp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/userManageDlg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/cviewHeader.h
  ${CMAKE_CURRENT_SOURCE_DIR}/FileDistribute.h
  ${CMAKE_CURRENT_SOURCE_DIR}/FileDistributeThread.h
  ${CMAKE_CURRENT_SOURCE_DIR}/userInfo.h
  #${CMAKE_CURRENT_SOURCE_DIR}/transferSetDlg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/sendPrdcSetDlg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/selUserDlg.h
  ${CMAKE_CURRENT_SOURCE_DIR}/addModel.h
  ${CMAKE_CURRENT_SOURCE_DIR}/DistributeTaskThread.h
  ${RadarProjectInclude}/org.radar.filedistribute/IFileDistribute.h
  ${RadarProjectInclude}/org.radar.filedistribute/IFileCrypt.h
  ${RadarProjectInclude}/interface/IBaseInterface.h
  ${RadarProjectInclude}/org.radar.filedistribute/IDistributeManager.h
  #${RadarProjectInclude}/ctkFtp/qftp.h
)

# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS
  ${CMAKE_CURRENT_SOURCE_DIR}/ui/userManage.ui
  ${CMAKE_CURRENT_SOURCE_DIR}/ui/addUser.ui
  #${CMAKE_CURRENT_SOURCE_DIR}/ui/transferSet.ui
  ${CMAKE_CURRENT_SOURCE_DIR}/ui/sendProductSet.ui
  ${CMAKE_CURRENT_SOURCE_DIR}/ui/selUser.ui
  ${CMAKE_CURRENT_SOURCE_DIR}/ui/addModel.ui
)

# QRC Files which should be compiled into the plugin
SET(PLUGIN_resources
)

# Qt5中使用zlib（gzread,gzwrite）需要添加
IF (WITH_QT5)
	ADD_DEFINITIONS("-DZ_PREFIX")
ENDIF (WITH_QT5)

INCLUDE_DIRECTORIES(
					${RadarProjectInclude}
					${RadarProjectInclude}/ctkPluginManager
					${RadarProjectInclude}/ctkPublicFun
					${RadarProjectInclude}/org.radar.filedistribute
					${RadarProjectInclude}/org.radar.dataadapter
					${RadarProjectInclude}/org.common.crypt
					${RadarProjectInclude}/org.common.quartz
					${RadarProjectInclude}/org.radar.pathadapter
					${RadarProjectInclude}/interface
					${RadarProjectInclude}/3dpart
					${RadarProjectInclude}/QsLog
					${RadarProjectInclude}/ctkProtocol
					${RadarProjectInclude}/ctkFtp
					${RadarProjectInclude}/org.radar.datacollection
					${VLD_INCLUDE}
				)

#Compute the plugin dependencies
ctkFunctionGetTargetLibraries(PLUGIN_target_libraries)

# 翻译文件
SET(PLUGINS_FILES_TO_TRANSLATE ${PLUGINS_FILES_TO_TRANSLATE} ${PLUGIN_SRCS} ${PLUGIN_HDRS} PARENT_SCOPE)

#添加非标准的共享库搜索路径（在ctkMacroBuildPlugin之前）
LINK_DIRECTORIES(${RadarProjectLib})

ctkMacroBuildPlugin(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${PLUGIN_export_directive}
  SRCS ${PLUGIN_SRCS} ${PLUGIN_HDRS}
  MOC_SRCS ${PLUGIN_MOC_SRCS}
  UI_FORMS ${PLUGIN_UI_FORMS}
  RESOURCES ${PLUGIN_resources}  
  TARGET_LIBRARIES ${PLUGIN_target_libraries} ctkLogger
  ${VLD_LIB}
  ctkLogger
  ctkPublicFun
  Mpr.lib
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  #${Qt5Network_LIBRARIES}
  libcurl_imp
)

INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/plugins/org.radar.filedistribute_v1.0
		LIBRARY DESTINATION bin/plugins/org.radar.filedistribute_v1.0)
