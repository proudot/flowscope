# File : ---- FindImgColor.cmake
# Author : Antoine Basset
# INRIA Serpico team
#
# Description : Try to find ImgColor library :
#
#        ICOL_LIBRARY        The .so/.a/.dll/... to link against
#        ICOL_INCLUDE_DIR    The headers directory of the ImgColor_LIBRARY
#
#        ICOL_FOUND          Is set to true if the library is found
#
  
find_path (ICOL_INC
    NAMES
        imgColor.hpp
)

## --------------------------------
set (ICOL_FOUND FALSE)
if (ICOL_INC)
    set (ICOL_INCLUDE_DIR ${ICOL_INC})
    set (ICOL_FOUND TRUE)
endif (ICOL_INC)

set (ICOL_INCLUDE_DIR ${ICOL_INCLUDE_DIR} CACHE STRING "include directory for ImgColor")

mark_as_advanced (
  ICOL_INCLUDE_DIR
)


