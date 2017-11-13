PROJECT(org_common_quartz)

SET(PLUGIN_export_directive "org_common_quartz_EXPORT")

set(PLUGIN_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/DispatchTimerActivator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/DispatchTimerImp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ccroncalculator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ccrontimer.cpp
)

set(PLUGIN_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/DispatchTimerActivator.h
  ${RadarProjectInclude}/org.common.quartz/IDispatchTimer.h
  ${RadarProjectInclude}/org.common.quartz/DispatchInterface.h
  ${RadarProjectInclude}/org.common.quartz/TimerCallBackParam.h
  ${RadarProjectInclude}/interface/IBaseInterface.h
  ${CMAKE_CURRENT_SOURCE_DIR}/DispatchTimerImp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ccroncalculator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ccrontimer.h
)

# Files which should be processed by Qts moc
set(PLUGIN_MOC_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/DispatchTimerActivator.h
  ${RadarProjectInclude}/org.common.quartz/IDispatchTimer.h
  ${RadarProjectInclude}/org.common.quartz/DispatchInterface.h
  ${RadarProjectInclude}/interface/IBaseInterface.h
  ${CMAKE_CURRENT_SOURCE_DIR}/DispatchTimerImp.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ccroncalculator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ccrontimer.h
)

# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS
)

# QRC Files which should be compiled into the plugin
SET(PLUGIN_resources
)

INCLUDE_DIRECTORIES(
					${RadarProjectInclude}/interface
					${RadarProjectInclude}/ctkPluginManager
					${RadarProjectInclude}/org.common.quartz
					#${RadarProjectInclude}/3dpart
					${RadarProjectInclude}/QsLog
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
  TARGET_LIBRARIES ${PLUGIN_target_libraries}  ctkLogger
  ${QT_QTGUI_LIBRARY}
  ${QT_QTUITOOLS_LIBRARY}
  ${VLD_LIB}
  ctkPublicFun
)

INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/plugins
		LIBRARY DESTINATION bin/plugins)


