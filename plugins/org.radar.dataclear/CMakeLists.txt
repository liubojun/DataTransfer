PROJECT(org_radar_dataclear)

SET(PLUGIN_export_directive "org_radar_dataclear_EXPORT")

set(PLUGIN_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/DataClearActivator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/DataClear.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/CDispatch.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/DelFiles.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/DataCleartemp.cpp
)

set(PLUGIN_HDRS
 
  ${CMAKE_CURRENT_SOURCE_DIR}/DataClear.h
  ${CMAKE_CURRENT_SOURCE_DIR}/CDispatch.h
  ${CMAKE_CURRENT_SOURCE_DIR}/DelFiles.h
  ${CMAKE_CURRENT_SOURCE_DIR}/DataClearActivator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/DataCleartemp.h
  ${RadarProjectInclude}/interface/IBaseInterface.h
  ${RadarProjectInclude}/org.radar.dataclear/BaseDatas.h
  ${RadarProjectInclude}/org.radar.dataclear/IDataClear.h
  ${RadarProjectInclude}/org.common.quartz/DispatchInterface.h
)

# Files which should be processed by Qts moc
set(PLUGIN_MOC_SRCS
  ${RadarProjectInclude}/interface/IBaseInterface.h
  ${RadarProjectInclude}/org.radar.dataclear/IDataClear.h
  ${RadarProjectInclude}/org.common.quartz/DispatchInterface.h
  ${CMAKE_CURRENT_SOURCE_DIR}/DataClear.h
  ${CMAKE_CURRENT_SOURCE_DIR}/CDispatch.h
  ${CMAKE_CURRENT_SOURCE_DIR}/DataClearActivator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/DelFiles.h
  ${CMAKE_CURRENT_SOURCE_DIR}/DataCleartemp.h
)


# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS

)

# QRC Files which should be compiled into the plugin
SET(PLUGIN_resources
)

INCLUDE_DIRECTORIES(
					${RadarProjectInclude}
					${RadarProjectInclude}/org.radar.dataclear
					${RadarProjectInclude}/interface
					${RadarProjectInclude}/org.common.quartz
					${RadarProjectInclude}/ctkPluginManager
					${RadarProjectInclude}/org.radar.drawproduct
					${VLD_INCLUDE}	
				)

# 翻译文件
SET(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} ${PLUGIN_SRCS} PARENT_SCOPE)

#Compute the plugin dependencies
ctkFunctionGetTargetLibraries(PLUGIN_target_libraries)



ctkMacroBuildPlugin(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${PLUGIN_export_directive}
  SRCS ${PLUGIN_SRCS} ${PLUGIN_HDRS}
  MOC_SRCS ${PLUGIN_MOC_SRCS}
  UI_FORMS ${PLUGIN_UI_FORMS}
  RESOURCES ${PLUGIN_resources}
  TARGET_LIBRARIES ${PLUGIN_target_libraries} ctkLogger
  ${QT_QTGUI_LIBRARY} 
  ${QT_UITOOLS_LIBRARY}
  ${VLD_LIB} 
  ctkPublicFun 
  ctkLogger
)

INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/plugins
		LIBRARY DESTINATION bin/plugins)

#将依赖库输出到目标运行目录

#INSTALL(FILES ${RadarProjectRunTimeLib}/QParse.dll
#			  ${RadarProjectRunTimeLib}/bzip2.dll
#			  ${RadarProjectRunTimeLib}/QsLog.dll
#		DESTINATION bin)		
