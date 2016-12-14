# - Find DataSvc
# Find the native DataSvcWorld headers and libraries.
#
#  DataSvc_INCLUDE_DIRS - where to find DataSvc/DataSvc.h, etc.
#  DataSvc_LIBRARIES    - List of libraries when using DataSvcWorld.
#  DataSvc_FOUND        - True if DataSvc found.

# Look for the header file.
SET(CMAKE_INCLUDE_PATH ${CMAKE_INSTALL_PREFIX}/include)
SET(CMAKE_LIBRARY_PATH ${CMAKE_INSTALL_PREFIX}/lib)

FIND_PATH(DataSvc_INCLUDE_DIR NAMES DataSvc/DataSvc.h)
MARK_AS_ADVANCED(DataSvc_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(DataSvc_LIBRARY NAMES DataSvc)
MARK_AS_ADVANCED(DataSvc_LIBRARY)

# handle the QUIETLY and REQUIRED arguments and set DATASVC_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DataSvc DEFAULT_MSG DataSvc_LIBRARY DataSvc_INCLUDE_DIR)

IF(DATASVC_FOUND)
  SET(DataSvc_LIBRARIES    ${DataSvc_LIBRARY})
  SET(DataSvc_INCLUDE_DIRS ${DataSvc_INCLUDE_DIR})
ELSE(DATASVC_FOUND)
  SET(DataSvc_LIBRARIES)
  SET(DataSvc_INCLUDE_DIRS)
ENDIF(DATASVC_FOUND)

