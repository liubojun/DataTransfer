#############################################################
# sources

SET(PROJECT_NAME ctkProtocol)

SET(SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/fileMsg.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lateFileTime.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/typeRegs.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transferSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/collectSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dataclearSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/pathAdapter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lastCollectTime.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/fileTmplate.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dbSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/pluginBaseInfo.cpp
)

SET(HDRS
	${RadarProjectInclude}/ctkProtocol/fileMsg.h
	${RadarProjectInclude}/ctkProtocol/lateFileTime.h
	${RadarProjectInclude}/ctkProtocol/typeRegs.h
	${RadarProjectInclude}/ctkProtocol/transferSet.h
	${RadarProjectInclude}/ctkProtocol/collectSet.h
	${RadarProjectInclude}/ctkProtocol/dataclearSet.h
	${RadarProjectInclude}/ctkProtocol/lastCollectTime.h
	${RadarProjectInclude}/ctkProtocol/fileTmplate.h
	${RadarProjectInclude}/ctkProtocol/pluginBaseInfo.h
	${RadarProjectInclude}/ctkProtocol/dbSet.h

)

#设置MOC相关变量
SET(MOC_HDRS

)

QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})

#添加依赖的头文件的路径
INCLUDE_DIRECTORIES(
					${RadarProjectInclude}
					${RadarProjectInclude}/3dpart
					${RadarProjectInclude}/ctkProtocol
					${RadarProjectInclude}/org.radar.dataadapter
					${Boost_INCLUDE_DIR}
					${VLD_INCLUDE}
					)

#设置需要翻译的源文件				
SET(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} ${SRCS} ${HDRS} PARENT_SCOPE)

#LINK_DIRECTORIES(${Boost_LIBRARY_DIR})
MESSAGE(STATUS "BOOST_lib = ${Boost_SERIALIZATION_LIBRARY_DEBUG}")

#设置为非console工程 WIN32为关键字

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS} ${HDRS} ${MOC_SRCS})

#查找依赖的库
IF (WIN32)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	  ${QT_QTMAIN_LIBRARY}
	  ${QT_QTCORE_LIBRARY}
	  ${VLD_LIB}	
	  ${Boost_SERIALIZATION_LIBRARY_DEBUG}  
	)
ELSE (WIN32)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	  ${QT_QTMAIN_LIBRARY}
	  ${QT_QTCORE_LIBRARY}
	  ${VLD_LIB}
      ${RadarProjectLib}/libboost_serialization.so
	)
ENDIF (WIN32)

#将生成文件输出到指定的目录中
INSTALL(TARGETS ${PROJECT_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION bin)
		
