# - Find TinyXML
# Find the native TinyXML headers and libraries.
#
#  TinyXML_INCLUDE_DIRS - where to find TinyXML/tinyxml.h, etc.
#  TinyXML_LIBRARIES    - List of libraries when using TinyXML.
#  TinyXML_FOUND        - True if TinyXML found.

# Look for the header file.
SET(CMAKE_INCLUDE_PATH ${CMAKE_INSTALL_PREFIX}/)
SET(CMAKE_LIBRARY_PATH ${CMAKE_INSTALL_PREFIX}/)

FIND_PATH(TinyXML_INCLUDE_DIR NAMES tinyxml.h)
MARK_AS_ADVANCED(TinyXML_INCLUDE_DIRS)

# Look for the library.
FIND_LIBRARY(TinyXML_LIBRARY NAMES tinyxml)
MARK_AS_ADVANCED(TinyXML_LIBRARIES)

# handle the QUIETLY and REQUIRED arguments and set TinyXML_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TinyXML DEFAULT_MSG TinyXML_LIBRARY TinyXML_INCLUDE_DIR)

MESSAGE(STATUS "TinyXML include path: " ${TinyXML_INCLUDE_DIR})

IF(TinyXML_INCLUDE_DIR AND TinyXML_LIBRARY)#DIM_FOUND)
  SET(TinyXML_FOUND TRUE)	
  SET(TinyXML_LIBRARIES    ${TinyXML_LIBRARY})
  SET(TinyXML_INCLUDE_DIRS ${TinyXML_INCLUDE_DIR})
ELSE()#IF(NOT Dim_FOUND)
  SET(TinyXML_FOUND FALSE)
  SET(TinyXML_LIBRARIES)
  SET(TinyXML_INCLUDE_DIRS)
ENDIF()

