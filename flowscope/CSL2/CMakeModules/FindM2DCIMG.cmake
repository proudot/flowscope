# File : ---- FindM2DCIMG.cmake
# Author : Antoine Basset
# INRIA Serpico team
#
# Description : Try to find M2DCIMG library and protobuf:
#
#        M2DCIMG_LIBRARIES      The .so/.a/.dll/... to link against
#        M2DCIMG_INCLUDE_DIR    The headers directory of the M2DCIMG_LIBRARIES
#
#        M2DCIMG_FOUND          Is set to true if the library is found
#


# Find cimg_motion2d
find_library (M2DCIMG_LIB
    NAMES 
        cimg_motion2d
)
  
find_path (M2DCIMG_INC
    NAMES
        mvtEstimator.hpp
)


# motion2d
find_package (Motion2D)
if (MOTION2D_FOUND)
    set (OTHER_3RDPARTY_LIBRARIES ${OTHER_3RDPARTY_LIBRARIES} ${MOTION2D_LIBRARIES})
    set (OTHER_3RDPARTY_INCLUDE ${OTHER_3RDPARTY_INCLUDE} ${MOTION2D_INCLUDES})
    message (STATUS "CMakeLists.txt : MOTION2D found")
    message (STATUS "Motion2D : ${MOTION2D_LIBRARIES}")
    message (STATUS "CMotion2D.h : ${MOTION2D_INCLUDES}")
else (MOTION2D_FOUND)
    message (FATAL_ERROR "CMakeLists.txt : MOTION2D NOT found")
endif (MOTION2D_FOUND)


## --------------------------------
set (M2DCIMG_FOUND FALSE)
if (M2DCIMG_LIB AND M2DCIMG_INC)
    set (M2DCIMG_LIBRARIES ${M2DCIMG_LIB} ${PROTOBUF_LIBRARIES})
    set (M2DCIMG_INCLUDE_DIR ${M2DCIMG_INC} ${PROTOBUF_INCLUDE_DIRS} )
    set (M2DCIMG_FOUND TRUE)
endif (M2DCIMG_LIB AND M2DCIMG_INC)

set (M2DCIMG_LIBRARY ${M2DCIMG_LIBRARIES} CACHE STRING "lib for cimg_motion2d")
set (M2DCIMG_INCLUDE_DIR ${M2DCIMG_INCLUDE_DIR} CACHE STRING "include directory for cimg_motion2d")

mark_as_advanced (
  M2DCIMG_LIBRARY
  M2DCIMG_INCLUDE_DIR
)
