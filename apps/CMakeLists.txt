set (BUILD_APP_DataTransfer TRUE CACHE BOOL "whether add unit test projects")
#set (BUILD_APP_RadarMessageCentre TRUE CACHE BOOL "whether add unit test projects")
set (APPS
)

IF (BUILD_APP_DataTransfer)
	set (APPS
		${APPS} DataTransfer DataTransferPro DataTransferConsole
	)
ENDIF ()


#IF (BUILD_APP_RadarMessageCentre)
#	set (APPS
#		${APPS}
#			RadarMessageCentre
#		)
#ENDIF ()

foreach(app ${APPS})
  option(BUILD_APP_${app} "Build the ${app} application" ON)
  MESSAGE(STATUS "${app}====")
  add_subdirectory(${app})
endforeach()
