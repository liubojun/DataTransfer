#############################################################
# sources
SET(PROJECT_NAME ctkLogger)

SET(SRCS
	ctkLog.cpp
)

SET(HDRS
	${RadarProjectInclude}/QsLog/ctkLog.h
	
	
)

#设置MOC相关变量
SET(MOC_HDRS
	${RadarProjectInclude}/QsLog/ctkLog.h
)

QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})

#添加依赖的头文件的路径
INCLUDE_DIRECTORIES(
					${RadarProjectInclude}/QsLog
					${VLD_INCLUDE}
					)

#设置为非console工程 WIN32为关键字
ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS} ${HDRS} ${MOC_SRCS})

#查找以来的库
TARGET_LINK_LIBRARIES(${PROJECT_NAME}
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  #${QT_QTGUI_LIBRARY} 
  ${QT_UITOOLS_LIBRARY}
  ${VLD_LIB}
)

#将生成文件输出到指定的目录中
INSTALL(TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION bin/
  LIBRARY DESTINATION bin/)
		