# File : FindMSdetec.cmake
# Date : 11/01/2013
# Author : Antoine Basset
# INRIA Serpico team
# 
# Description :
#     Try to find sparselib library on the system and set the following variables :
#
# MSDETEC_FOUND
# MSDETEC_LIBRARIES
# MSDETEC_INCLUDES
#
# !!!!!! Not working for WINDOWS !!!!!!!
#

if (UNIX OR APPLE)
# Find libmixedStateDetec.dylib
    find_library (MSDETEC_LIBRARY
            NAMES mixedStateDetec
            PATHS
            /usr/local/lib
            /usr/local/lib/MSdetec
        )
  
    find_path (MSDETEC_INCLUDE
                MSImage.h
                /usr/local/include
                /usr/local/include/MSdetec
        )
endif (UNIX OR APPLE)

## --------------------------------
set (MSDETEC_FOUND FALSE)
if (MSDETEC_LIBRARY AND MSDETEC_INCLUDE)
    set (MSDETEC_LIBRARIES ${MSDETEC_LIBRARY})
    set (MSDETEC_INCLUDES ${MSDETEC_INCLUDE})
    set (MSDETEC_FOUND TRUE)
endif (MSDETEC_LIBRARY AND MSDETEC_INCLUDE)

set (MSDETEC_LIBRARIES ${MSDETEC_LIBRARIES} CACHE STRING "lib directories for msdetec")
set (MSDETEC_INCLUDES ${MSDETEC_INCLUDES} CACHE STRING "include directories for msdetec dependancies")

mark_as_advanced (
  MSDETEC_LIBRARY
  MSDETEC_INCLUDE
  MSDETEC_LIBRARIES
  MSDETEC_INCLUDES
)
