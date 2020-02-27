# File : ---- FindXMLDetection.cmake
# Author : Tristan Lecorgne
# INRIA Serpico team
#
# Description : Try to find XMLDetection library :
#
#        XMLDetection_LIBRARY        The .so/.a/.dll/... to link against
#        XMLDetection_INCLUDE_DIR    The headers directory of the XMLDetection_LIBRARY
#
#        XMLDetection_FOUND          Is set to true if the library is found
#


# Find libXMLDetection
find_library (XMLDetection_LIB
    NAMES 
        xmldetection
    PATHS
        /usr/lib
)
  
find_path (XMLDetection_INC
    NAMES
        xmldetection.h
    PATHS
        /usr/include
    
)

## --------------------------------
set (XMLDetection_FOUND FALSE)
if (XMLDetection_LIB AND XMLDetection_INC)
    set (XMLDetection_LIBRARY ${XMLDetection_LIB})
    set (XMLDetection_INCLUDE_DIR ${XMLDetection_INC})
    set (XMLDetection_FOUND TRUE)
endif (XMLDetection_LIB AND XMLDetection_INC)

set (XMLDetection_LIBRARY ${XMLDetection_LIBRARY} CACHE STRING "lib for XMLDetection")
set (XMLDetection_INCLUDE_DIR ${XMLDetection_INCLUDE_DIR} CACHE STRING "include directory for XMLDetection")

mark_as_advanced (
  XMLDetection_LIBRARY
  XMLDetection_INCLUDE_DIR
)


