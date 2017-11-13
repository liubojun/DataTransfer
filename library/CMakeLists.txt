ADD_SUBDIRECTORY(ctkLogger)
#ADD_SUBDIRECTORY(ctkProtocol)
ADD_SUBDIRECTORY(ctkPublicFun)
ADD_SUBDIRECTORY(ctkPluginManager)

# 如何构建meteogis插件，则将雷达组网拼图GIS插件加入
IF (${BUILD_plugins/org.meteogis.core})
	ADD_SUBDIRECTORY(meteoGisMosaicProvider)
ENDIF ()



#将需要翻译的文件，传递到外部
SET(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} PARENT_SCOPE)