PROJECT(org_radar_datacollection)

SET(PLUGIN_export_directive "org_radar_datacollection_EXPORT")

set(PLUGIN_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/datacollectionActivator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/datacollectionImp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/CollectManager.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/CltDispatch.cpp

)

set(PLUGIN_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/datacollectionActivator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/datacollectionImp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/CltDispatch.h
  ${RadarProjectInclude}/interface/IBaseInterface.h
  ${RadarProjectInclude}/org.radar.datacollection/IDataCollection.h
  ${RadarProjectInclude}/org.radar.datacollection/CollectManager.h
  ${RadarProjectInclude}/org.radar.datacollection/ICollectManager.h
  ${RadarProjectInclude}/org.common.quartz/DispatchInterface.h
  ${RadarProjectInclude}/interface/ILogPrint.h
  ${RadarProjectInclude}/ctkTransfer/CollectorBase.h
  ${RadarProjectInclude}/ctkTransfer/shareddirCollector.h
  ${RadarProjectInclude}/ctkTransfer/ftpCollector.h
  ${RadarProjectInclude}/ctkTransfer/streamCollector.h
)

# Files which should be processed by Qts moc
set(PLUGIN_MOC_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/datacollectionActivator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/datacollectionImp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/CltDispatch.h
  ${RadarProjectInclude}/interface/IBaseInterface.h
  ${RadarProjectInclude}/interface/ILogWidget.hpp
  ${RadarProjectInclude}/org.radar.datacollection/IDataCollection.h
  ${RadarProjectInclude}/org.common.quartz/DispatchInterface.h
  ${RadarProjectInclude}/org.radar.datacollection/CollectManager.h
  ${RadarProjectInclude}/org.radar.datacollection/ICollectManager.h
  ${RadarProjectInclude}/interface/ILogPrint.h
  ${RadarProjectInclude}/ctkTransfer/CollectorBase.h
  ${RadarProjectInclude}/ctkTransfer/shareddirCollector.h
  ${RadarProjectInclude}/ctkTransfer/ftpCollector.h
  ${RadarProjectInclude}/ctkTransfer/streamCollector.h
)

# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS
  #${CMAKE_CURRENT_SOURCE_DIR}/ui/datacollect.ui
)

# QRC Files which should be compiled into the plugin
SET(PLUGIN_resources
	${CMAKE_CURRENT_SOURCE_DIR}/image/res.qrc
)

# 翻译文件
SET(PLUGINS_FILES_TO_TRANSLATE ${PLUGINS_FILES_TO_TRANSLATE} ${PLUGIN_SRCS} ${PLUGIN_HDRS} ${PLUGIN_UI_FORMS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(
					${RadarProjectInclude}
					${RadarProjectInclude}/interface
					${RadarProjectInclude}/ctkPluginManager
					${RadarProjectInclude}/org.radar.datacollection
					${RadarProjectInclude}/org.radar.drawproduct
					${RadarProjectInclude}/org.common.quartz
					${RadarProjectInclude}/org.radar.clientmessage
					${RadarProjectInclude}/org.common.filesystemwatcher
					${RadarProjectInclude}/org.radar.dataadapter
					${RadarProjectInclude}/org.radar.pathadapter
					${RadarProjectInclude}/ctkPublicFun
					${RadarProjectInclude}/QsLog
					${RadarProjectInclude}/3dpart
					${RadarProjectInclude}/3dpart/Common
					${RadarProjectInclude}/ctkProtocol
					${RadarProjectInclude}/ctkTransfer
					${RadarProjectInclude}/3dpart
					${RadarProjectInclude}/3dpart/RCF-2.2.0.0/include
					${CURL_INCLUDE_DIR}
					${VLD_INCLUDE}
				)

#由于该插件依赖了加密静态库，故代码生成需要使用Mtd的方式				
#SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
#SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")

#Compute the plugin dependencies
ctkFunctionGetTargetLibraries(PLUGIN_target_libraries)
#MESSAGE(STATUS "${PLUGIN_target_libraries} ${PROJECT_NAME}")

#添加非标准的共享库搜索路径
LINK_DIRECTORIES(${RadarProjectLib})

# ctk插件的方式添加依赖库，不使用TARGET_LINK_LIBRARIES

ctkMacroBuildPlugin(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${PLUGIN_export_directive}
  SRCS ${PLUGIN_SRCS} ${PLUGIN_HDRS}
  MOC_SRCS ${PLUGIN_MOC_SRCS}
  UI_FORMS ${PLUGIN_UI_FORMS}
  RESOURCES ${PLUGIN_resources}
  TARGET_LIBRARIES ${PLUGIN_target_libraries} ${VLD_LIB}
  ctkLogger
  ctkPublicFun	
  #RCF
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  #Mpr.lib
  ${CURL_LIBRARY}
  ctkTransfer
)


INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/plugins
		LIBRARY DESTINATION bin/plugins)

#INSTALL(FILES
		#${RadarProjectRunTimeLib}/libcurl.dll
		#DESTINATION bin)	
