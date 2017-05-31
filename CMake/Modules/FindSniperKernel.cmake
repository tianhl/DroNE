# - Find SniperKernelWorld
# Find the native SniperKernelWorld headers and libraries.
#
#  SniperKernel_INCLUDE_DIRS - where to find SniperKernel/Sniper.h, etc.
#  SniperKernel_LIBRARIES    - List of libraries when using SniperKernelWorld.
#  SniperKernel_FOUND        - True if SniperKernel found.
# author: H.L. TIAN 2017

# Look for the header file.
SET(CMAKE_INCLUDE_PATH ${CMAKE_INSTALL_PREFIX}/include)
SET(CMAKE_LIBRARY_PATH ${CMAKE_INSTALL_PREFIX}/lib)

FIND_PATH(SniperKernel_INCLUDE_DIR NAMES SniperKernel/Sniper.h)
MARK_AS_ADVANCED(SniperKernel_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(SniperKernel_LIBRARY NAMES SniperKernel)
MARK_AS_ADVANCED(SniperKernel_LIBRARY)

# handle the QUIETLY and REQUIRED arguments and set SniperKernel_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SniperKernel DEFAULT_MSG SniperKernel_LIBRARY SniperKernel_INCLUDE_DIR)

MESSAGE(STATUS "SniperKernel include path: " ${SniperKernel_INCLUDE_DIR})
IF(SNIPERKERNEL_FOUND)
  SET(SniperKernel_LIBRARIES    ${SniperKernel_LIBRARY})
  SET(SniperKernel_INCLUDE_DIRS ${SniperKernel_INCLUDE_DIR})
ELSE(SNIPERKERNEL_FOUND)
  SET(SniperKernel_LIBRARIES)
  SET(SniperKernel_INCLUDE_DIRS)
ENDIF(SNIPERKERNEL_FOUND)

