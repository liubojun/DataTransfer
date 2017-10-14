PROJECT(org_common_crypt)

SET(PLUGIN_export_directive "org_common_crypt_EXPORT")

set(PLUGIN_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/CryptActivator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/CryptInfo.cpp
)

set(PLUGIN_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/CryptActivator.h
  ${RadarProjectInclude}/org.common.crypt/ICrypt.h
  ${RadarProjectInclude}/interface/IBaseInterface.h
  ${CMAKE_CURRENT_SOURCE_DIR}/CryptInfo.h
)

# Files which should be processed by Qts moc
set(PLUGIN_MOC_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/CryptActivator.h
  ${RadarProjectInclude}/org.common.crypt/ICrypt.h
  ${RadarProjectInclude}/interface/IBaseInterface.h
  ${CMAKE_CURRENT_SOURCE_DIR}/CryptInfo.h
)

# Qt Designer files which should be processed by Qts uic
SET(PLUGIN_UI_FORMS
)

# QRC Files which should be compiled into the plugin
SET(PLUGIN_resources
)

# 翻译文件
# SET(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} ${PLUGIN_SRCS} PARENT_SCOPE)

INCLUDE_DIRECTORIES(
					${RadarProjectInclude}/interface
					${RadarProjectInclude}/ctkPluginManager
					${RadarProjectInclude}/org.common.crypt
					${RadarProjectInclude}/org.common.crypt/include_cryptopp
					${RadarProjectInclude}/3dpart/QsLog
					${VLD_INCLUDE}
				)
				
#由于该插件依赖了加密静态库，故代码生成需要使用Mtd的方式				
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")

#Compute the plugin dependencies
ctkFunctionGetTargetLibraries(PLUGIN_target_libraries)

ctkMacroBuildPlugin(
  NAME ${PROJECT_NAME}
  EXPORT_DIRECTIVE ${PLUGIN_export_directive}
  SRCS ${PLUGIN_SRCS} ${PLUGIN_HDRS}
  MOC_SRCS ${PLUGIN_MOC_SRCS}
  UI_FORMS ${PLUGIN_UI_FORMS}
  RESOURCES ${PLUGIN_resources}
  TARGET_LIBRARIES ${PLUGIN_target_libraries} ${RadarProjectLib}/cryptlib.lib
  ${QT_QTGUI_LIBRARY}
  ${QT_QTUITOOLS_LIBRARY}
  ${VLD_LIB}
  ctkPublicFun
)

INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin/plugins/org.common.crypt_v1.0
		LIBRARY DESTINATION bin/plugins/org.common.crypt_v1.0)
