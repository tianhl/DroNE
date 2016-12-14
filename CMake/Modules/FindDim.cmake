# - Find SniperKernelWorld
# Find the native SniperKernelWorld headers and libraries.
#
#  SniperKernel_INCLUDE_DIRS - where to find SniperKernel/Sniper.h, etc.
#  SniperKernel_LIBRARIES    - List of libraries when using SniperKernelWorld.
#  SniperKernel_FOUND        - True if SniperKernel found.

# Look for the header file.
SET(CMAKE_INCLUDE_PATH ${CMAKE_INSTALL_PREFIX})
SET(CMAKE_LIBRARY_PATH ${CMAKE_INSTALL_PREFIX})

FIND_PATH(Dim_INCLUDE_DIR NAMES dim.h)
MARK_AS_ADVANCED(Dim_INCLUDE_DIR)

# Look for the library.
FIND_LIBRARY(Dim_LIBRARY NAMES dim)
MARK_AS_ADVANCED(Dim_LIBRARY)

# handle the QUIETLY and REQUIRED arguments and set SniperKernel_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Dim DEFAULT_MSG Dim_LIBRARY Dim_INCLUDE_DIR)

MESSAGE(STATUS "Dim include path: " ${Dim_INCLUDE_DIR})

IF(DIM_FOUND)
	SET(Dim_LIBRARIES    ${Dim_LIBRARY})
	SET(Dim_INCLUDE_DIRS ${Dim_INCLUDE_DIR})
ELSE(Dim_FOUND)
	SET(Dim_LIBRARIES)
	SET(Dim_INCLUDE_DIRS)
ENDIF(DIM_FOUND)

