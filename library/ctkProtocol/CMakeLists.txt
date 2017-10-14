#############################################################
# sources

SET(PROJECT_NAME ctkProtocol)

SET(SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/testserial.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/msg.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ConnectConfig.cpp	
	${CMAKE_CURRENT_SOURCE_DIR}/fileMsg.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lateFileTime.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/typeRegs.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transferSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/collectSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dataclearSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ParaConfig.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/InvokeMethod.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/pathAdapter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/msgqueue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/radarProduct.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mosaicArea.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/IXClientCommSet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RadarFileConfig.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/radarAnimationSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/prdcTransferSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/radarQueue.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/lastCollectTime.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DataCollectionPath2Type.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SourceDataClear.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/hwStation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/fileTmplate.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trecMsg.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dbSet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/pluginBaseInfo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/pupmosaicProduct.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/messageQueue.cpp
)

SET(HDRS
	${RadarProjectInclude}/ctkProtocol/testserial.h
	${RadarProjectInclude}/ctkProtocol/msg.h
	${RadarProjectInclude}/ctkProtocol/ConnectConfig.h	
	${RadarProjectInclude}/ctkProtocol/fileMsg.h
	${RadarProjectInclude}/ctkProtocol/lateFileTime.h
	${RadarProjectInclude}/ctkProtocol/typeRegs.h
	${RadarProjectInclude}/ctkProtocol/transferSet.h
	${RadarProjectInclude}/ctkProtocol/collectSet.h
	${RadarProjectInclude}/ctkProtocol/dataclearSet.h
	${RadarProjectInclude}/ctkProtocol/ParaConfig.h
	${RadarProjectInclude}/ctkProtocol/InvokeMethod.h
	${RadarProjectInclude}/ctkProtocol/pathAdapter.h
	${RadarProjectInclude}/ctkProtocol/msgqueue.h
	${RadarProjectInclude}/ctkProtocol/radarProduct.h
	${RadarProjectInclude}/ctkProtocol/mosaicArea.h
	${RadarProjectInclude}/ctkProtocol/IXClientCommSet.h
    ${RadarProjectInclude}/ctkProtocol/RadarFileConfig.h
	${RadarProjectInclude}/ctkProtocol/radarAnimationSet.h
	${RadarProjectInclude}/ctkProtocol/prdcTransferSet.h
	${RadarProjectInclude}/ctkProtocol/radarQueue.h
	${RadarProjectInclude}/ctkProtocol/lastCollectTime.h
	${RadarProjectInclude}/ctkProtocol/DataCollectionPath2Type.h
	${RadarProjectInclude}/ctkProtocol/SourceDataClear.h
	${RadarProjectInclude}/ctkProtocol/pupmosaicProduct.h
	${RadarProjectInclude}/ctkProtocol/hwStation.h
	${RadarProjectInclude}/ctkProtocol/fileTmplate.h
	${RadarProjectInclude}/ctkProtocol/trecMsg.h
	${RadarProjectInclude}/ctkProtocol/pluginBaseInfo.h
	${RadarProjectInclude}/ctkProtocol/dbSet.h
	${RadarProjectInclude}/ctkProtocol/messageQueue.h
)

#设置MOC相关变量
SET(MOC_HDRS
	${RadarProjectInclude}/ctkProtocol/InvokeMethod.h
)

QT4_WRAP_CPP(MOC_SRCS ${MOC_HDRS})

#添加依赖的头文件的路径
INCLUDE_DIRECTORIES(
					${RadarProjectInclude}
					${RadarProjectInclude}/3dpart
					${RadarProjectInclude}/3dpart/zeromq
					${RadarProjectInclude}/ctkProtocol
					${RadarProjectInclude}/org.radar.dataadapter
					${VLD_INCLUDE}
					)

#设置需要翻译的源文件				
SET(FILES_TO_TRANSLATE ${FILES_TO_TRANSLATE} ${SRCS} ${HDRS} PARENT_SCOPE)

LINK_DIRECTORIES(${RadarProjectLib})

#设置为非console工程 WIN32为关键字

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS} ${HDRS} ${MOC_SRCS})

#查找依赖的库
IF (WIN32)
	TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	  ${QT_QTMAIN_LIBRARY}
	  ${QT_QTCORE_LIBRARY}
	  ${VLD_LIB}	  
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
		
