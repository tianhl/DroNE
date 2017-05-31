# - Find DroNECOre3
# Find the native DroNECoreWorld headers and libraries.
#
#  DroNECore_INCLUDE_DIRS - where to find DroNECore/DroNE.h, etc.
#  DroNECore_LIBRARIES    - List of libraries when using DroNECoreWorld.
#  DroNECore_FOUND        - True if DroNECore found.
# author: H.L. TIAN 2017

# Look for the header file.
SET(CMAKE_INCLUDE_PATH ${CMAKE_INSTALL_PREFIX}/include)
SET(CMAKE_LIBRARY_PATH ${CMAKE_INSTALL_PREFIX}/lib)

FIND_PATH(DroNECore_INCLUDE_DIR NAMES DroNECore/DroNE.h)
MARK_AS_ADVANCED(DroNECore_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(DroNECore_LIBRARY NAMES DroNECore)
MARK_AS_ADVANCED(DroNECore_LIBRARY)

# handle the QUIETLY and REQUIRED arguments and set DATASVC_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DroNECore DEFAULT_MSG DroNECore_LIBRARY DroNECore_INCLUDE_DIR)

IF(DRONECORE_FOUND)
  SET(DroNECore_LIBRARIES    ${DroNECore_LIBRARY})
  SET(DroNECore_INCLUDE_DIRS ${DroNECore_INCLUDE_DIR})
ELSE (DRONECORE_FOUND)
  SET(DroNECore_LIBRARIES)
  SET(DroNECore_INCLUDE_DIRS)
ENDIF(DRONECORE_FOUND)

