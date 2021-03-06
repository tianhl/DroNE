# author: H.L. TIAN 2017
PROJECT(CtrlSvc)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Build Type
IF(CMAKE_BUILD_TYPE_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_BUILD_TYPE DEBUG) # RELEASE, RELWITHDEBINFO MINSIZEREL
ENDIF()

# Find Package
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../CMake/Modules)

FIND_PACKAGE(SniperKernel REQUIRED)
IF(SNIPERKERNEL_FOUND)
  INCLUDE_DIRECTORIES(${SniperKernel_INCLUDE_DIRS})
ELSEIF(NOT SniperKernel_FOUND) 
  MESSAGE(FATAL_ERROR "Unable to find SniperKernel. Did you set SniperKernel_ROOT?")
ENDIF()

FIND_PACKAGE(DroNECore REQUIRED)
IF(DRONECORE_FOUND)
  INCLUDE_DIRECTORIES(${DroNECore_INCLUDE_DIRS})
ELSEIF(NOT DroNECore_FOUND) 
  MESSAGE(FATAL_ERROR "Unable to find DroNECore. Did you set DroNECore_ROOT?")
ENDIF()

FIND_PACKAGE(DataSvc REQUIRED)
IF(DATASVC_FOUND)
  INCLUDE_DIRECTORIES(${DataSvc_INCLUDE_DIRS})
ELSEIF(NOT DataSvc_FOUND) 
  MESSAGE(FATAL_ERROR "Unable to find DataSvc. Did you set DataSvc_ROOT?")
ENDIF()

# Find Package
FIND_PACKAGE(Boost)
FIND_PACKAGE(PythonLibs)
IF(Boost_FOUND AND PYTHONLIBS_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_PATH})
  SET(Boost_USE_STATIC_LIBS OFF)
  SET(Boost_USE_MULTITHREADED ON)
  SET(Boost_USE_STATIC_RUNTIME OFF)
  FIND_PACKAGE(Boost  COMPONENTS python  thread REQUIRED) 
ELSEIF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "Unable to find Boost. Did you set BOOST_ROOT?")
ELSEIF(NOT PYTHONLIBS_FOUND)
  MESSAGE(FATAL_ERROR "Unable to find Python. Did you set PYTHONPATH?")
ENDIF()

# Compile Flag
SET(Boost_NO_BOOST_CMAKE TRUE)
IF(CMAKE_COMPILER_IS_GNUCXX)
  ADD_DEFINITIONS("-Wall")
ELSE()
  MESSAGE(FATAL_ERROR "CMakeLists.txt has not been tested/written for your compiler.")
ENDIF()

# Search Source Files
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})
FILE(GLOB_RECURSE CPPLIB_SRC_LIST "*.cc")

# Compile Library for CPP
ADD_LIBRARY(CtrlSvc SHARED ${CPPLIB_SRC_LIST})
TARGET_LINK_LIBRARIES(CtrlSvc ${Boost_LIBRARIES}) 
TARGET_LINK_LIBRARIES(CtrlSvc ${SniperKernel_LIBRARIES})
TARGET_LINK_LIBRARIES(CtrlSvc ${DroNECore_LIBRARIES})
TARGET_LINK_LIBRARIES(CtrlSvc ${DataSvc_LIBRARIES})


# Output Path and Install Area
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

INSTALL(TARGETS   CtrlSvc LIBRARY DESTINATION   ${PROJECT_SOURCE_DIR}/../DroNEInstall/lib)
INSTALL(DIRECTORY CtrlSvc DESTINATION         ${PROJECT_SOURCE_DIR}/../DroNEInstall/include)
INSTALL(DIRECTORY python/CtrlSvc DESTINATION  ${PROJECT_SOURCE_DIR}/../DroNEInstall/pylib)
INSTALL(DIRECTORY python/NEON    DESTINATION  ${PROJECT_SOURCE_DIR}/../DroNEInstall/pylib)
