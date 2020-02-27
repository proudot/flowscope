# File : FindMotion2D.cmake
# Date : 25/06/2013
# Author : Antoine Basset
# INRIA Serpico team
# 
# Description :
#     Try to find Motion2D library on the system and set the following variables :
#
# MOTION2D_FOUND
# MOTION2D_LIBRARIES
# MOTION2D_INCLUDES
#
# !!!!!! Not working for WINDOWS !!!!!!!
#

if (UNIX OR APPLE)
# Find libMotion2D.a
    find_library (MOTION2D_LIBRARY
            NAMES motion2d
            PATHS
            /usr/lib
            /usr/lib/Motion2D
        )
  
    find_path (MOTION2D_INCLUDE
                CMotion2D.h
                /usr/include
                /usr/include/Motion2D
        )
endif (UNIX OR APPLE)

## --------------------------------
set (MOTION2D_FOUND FALSE)
if (MOTION2D_LIBRARY AND MOTION2D_INCLUDE)
    set (MOTION2D_LIBRARIES ${MOTION2D_LIBRARY})
    set (MOTION2D_INCLUDES ${MOTION2D_INCLUDE})
    set (MOTION2D_FOUND TRUE)
endif (MOTION2D_LIBRARY AND MOTION2D_INCLUDE)

set (MOTION2D_LIBRARIES ${MOTION2D_LIBRARIES} CACHE STRING "lib directories for motion2d")
set (MOTION2D_INCLUDES ${MOTION2D_INCLUDES} CACHE STRING "include directories for motion2d dependancies")

mark_as_advanced (
  MOTION2D_LIBRARY
  MOTION2D_INCLUDE
  MOTION2D_LIBRARIES
  MOTION2D_INCLUDES
)
