IF(WIN32)
  # 在环境变量中添加VLD_DIR
  IF (MSVC)
    FIND_PATH(VLD_INCLUDE_DIR vld.h "$ENV{VLD_DIR}/include" $ENV{INCLUDE})
    FIND_LIBRARY(VLD_LIBRARY NAMES VLD VLD_i PATHS 
	    "$ENV{VLD_DIR}/lib/Win32" $ENV{LIB} /usr/lib c:/msys/local/lib)
    IF (VLD_LIBRARY)
      SET (
         VLD_LIBRARY;odbc32;odbccp32 
         CACHE STRING INTERNAL)
    ENDIF (VLD_LIBRARY)

  ENDIF (MSVC)
  
  
ELSE(WIN32)
  IF(UNIX) 

    # try to use framework on mac
    # want clean framework path, not unix compatibility path
    IF (APPLE)
      IF (CMAKE_FIND_FRAMEWORK MATCHES "FIRST"
          OR CMAKE_FRAMEWORK_PATH MATCHES "ONLY"
          OR NOT CMAKE_FIND_FRAMEWORK)
        SET (CMAKE_FIND_FRAMEWORK_save ${CMAKE_FIND_FRAMEWORK} CACHE STRING "" FORCE)
        SET (CMAKE_FIND_FRAMEWORK "ONLY" CACHE STRING "" FORCE)
        FIND_LIBRARY(VLD_LIBRARY VLD)
        IF (VLD_LIBRARY)
          # they're all the same in a framework
          SET (VLD_INCLUDE_DIR ${VLD_LIBRARY}/Headers CACHE PATH "Path to a file.")
          # set VLD_CONFIG to make later test happy, not used here, may not exist
          SET (VLD_CONFIG ${VLD_LIBRARY}/unix/bin/VLD-config CACHE FILEPATH "Path to a program.")
          # version in info.plist
          GET_VERSION_PLIST (${VLD_LIBRARY}/Resources/Info.plist VLD_VERSION)
          IF (NOT VLD_VERSION)
            MESSAGE (FATAL_ERROR "Could not determine VLD version from framework.")
          ENDIF (NOT VLD_VERSION)
          STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" VLD_VERSION_MAJOR "${VLD_VERSION}")
          STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" VLD_VERSION_MINOR "${VLD_VERSION}")
          IF (VLD_VERSION_MAJOR LESS 1 OR (VLD_VERSION EQUAL 1 AND VLD_VERSION_MINOR LESS 4))
            MESSAGE (FATAL_ERROR "VLD version is too old (${VLD_VERSION}). Use 1.4.0 or higher.")
          ENDIF (VLD_VERSION_MAJOR LESS 1 OR (VLD_VERSION EQUAL 1 AND VLD_VERSION_MINOR LESS 4))
        ENDIF (VLD_LIBRARY)
        SET (CMAKE_FIND_FRAMEWORK ${CMAKE_FIND_FRAMEWORK_save} CACHE STRING "" FORCE)
      ENDIF ()
    ENDIF (APPLE)

    IF(CYGWIN)
      FIND_LIBRARY(VLD_LIBRARY NAMES VLD PATHS /usr/lib /usr/local/lib)
    ENDIF(CYGWIN)

    IF (NOT VLD_INCLUDE_DIR OR NOT VLD_LIBRARY OR NOT VLD_CONFIG)
      # didn't find OS X framework, and was not set by user
      SET(VLD_CONFIG_PREFER_PATH "$ENV{VLD_HOME}/bin" CACHE STRING "preferred path to VLD (VLD-config)")
      SET(VLD_CONFIG_PREFER_FWTOOLS_PATH "$ENV{FWTOOLS_HOME}/bin_safe" CACHE STRING "preferred path to VLD (VLD-config) from FWTools")
      FIND_PROGRAM(VLD_CONFIG VLD-config
          ${VLD_CONFIG_PREFER_PATH}
          ${VLD_CONFIG_PREFER_FWTOOLS_PATH}
          /usr/local/bin/
          /usr/bin/
          )
      # MESSAGE("DBG VLD_CONFIG ${VLD_CONFIG}")
    
      IF (VLD_CONFIG) 

        ## extract VLD version 
        EXEC_PROGRAM(${VLD_CONFIG}
            ARGS --version
            OUTPUT_VARIABLE VLD_VERSION )
        STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" VLD_VERSION_MAJOR "${VLD_VERSION}")
        STRING(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" VLD_VERSION_MINOR "${VLD_VERSION}")
  
        # MESSAGE("DBG VLD_VERSION ${VLD_VERSION}")
        # MESSAGE("DBG VLD_VERSION_MAJOR ${VLD_VERSION_MAJOR}")
        # MESSAGE("DBG VLD_VERSION_MINOR ${VLD_VERSION_MINOR}")
  
        # check for VLD version
        # version 1.2.5 is known NOT to be supported (missing CPL_STDCALL macro)
        # According to INSTALL, 1.4.0+ is required
        IF (VLD_VERSION_MAJOR LESS 1 OR (VLD_VERSION_MAJOR EQUAL 1 AND VLD_VERSION_MINOR LESS 4))
          MESSAGE (FATAL_ERROR "VLD version is too old (${VLD_VERSION}). Use 1.4.0 or higher.")
        ENDIF (VLD_VERSION_MAJOR LESS 1 OR (VLD_VERSION_MAJOR EQUAL 1 AND VLD_VERSION_MINOR LESS 4))

        # set INCLUDE_DIR to prefix+include
        EXEC_PROGRAM(${VLD_CONFIG}
            ARGS --prefix
            OUTPUT_VARIABLE VLD_PREFIX)
        #SET(VLD_INCLUDE_DIR ${VLD_PREFIX}/include CACHE STRING INTERNAL)
        FIND_PATH(VLD_INCLUDE_DIR 
            VLD.h 
            ${VLD_PREFIX}/include/VLD
            ${VLD_PREFIX}/include
            /usr/local/include 
            /usr/include 
            )

        ## extract link dirs for rpath  
        EXEC_PROGRAM(${VLD_CONFIG}
            ARGS --libs
            OUTPUT_VARIABLE VLD_CONFIG_LIBS )

        ## split off the link dirs (for rpath)
        ## use regular expression to match wildcard equivalent "-L*<endchar>"
        ## with <endchar> is a space or a semicolon
        STRING(REGEX MATCHALL "[-][L]([^ ;])+" 
            VLD_LINK_DIRECTORIES_WITH_PREFIX 
            "${VLD_CONFIG_LIBS}" )
        #      MESSAGE("DBG  VLD_LINK_DIRECTORIES_WITH_PREFIX=${VLD_LINK_DIRECTORIES_WITH_PREFIX}")

        ## remove prefix -L because we need the pure directory for LINK_DIRECTORIES
      
        IF (VLD_LINK_DIRECTORIES_WITH_PREFIX)
          STRING(REGEX REPLACE "[-][L]" "" VLD_LINK_DIRECTORIES ${VLD_LINK_DIRECTORIES_WITH_PREFIX} )
        ENDIF (VLD_LINK_DIRECTORIES_WITH_PREFIX)

        ## split off the name
        ## use regular expression to match wildcard equivalent "-l*<endchar>"
        ## with <endchar> is a space or a semicolon
        STRING(REGEX MATCHALL "[-][l]([^ ;])+" 
            VLD_LIB_NAME_WITH_PREFIX 
            "${VLD_CONFIG_LIBS}" )
        #      MESSAGE("DBG  VLD_LIB_NAME_WITH_PREFIX=${VLD_LIB_NAME_WITH_PREFIX}")


        ## remove prefix -l because we need the pure name
      
        IF (VLD_LIB_NAME_WITH_PREFIX)
          STRING(REGEX REPLACE "[-][l]" "" VLD_LIB_NAME ${VLD_LIB_NAME_WITH_PREFIX} )
        ENDIF (VLD_LIB_NAME_WITH_PREFIX)

        IF (APPLE)
          IF (NOT VLD_LIBRARY)
            # work around empty VLD_LIBRARY left by framework check
            # while still preserving user setting if given
            # ***FIXME*** need to improve framework check so below not needed
            SET(VLD_LIBRARY ${VLD_LINK_DIRECTORIES}/lib${VLD_LIB_NAME}.dylib CACHE STRING INTERNAL FORCE)
          ENDIF (NOT VLD_LIBRARY)
        ELSE (APPLE)
          SET(VLD_LIBRARY ${VLD_LINK_DIRECTORIES}/lib${VLD_LIB_NAME}.so CACHE STRING INTERNAL)
        ENDIF (APPLE)
      
      ELSE(VLD_CONFIG)
        MESSAGE("FindVLD.cmake: VLD-config not found. Please set it manually. VLD_CONFIG=${VLD_CONFIG}")
      ENDIF(VLD_CONFIG)
    ENDIF (NOT VLD_INCLUDE_DIR OR NOT VLD_LIBRARY OR NOT VLD_CONFIG)
  ENDIF(UNIX)
ENDIF(WIN32)


IF (VLD_INCLUDE_DIR AND VLD_LIBRARY)
   SET(VLD_FOUND TRUE)
ENDIF (VLD_INCLUDE_DIR AND VLD_LIBRARY)

