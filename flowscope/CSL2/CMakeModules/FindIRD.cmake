# File : ---- FindImgReader.cmake
# Author : Antoine Basset
# INRIA Serpico team
#
# Description : Try to find ImgReader library :
#
#        IRD_INCLUDE_DIR    The headers directory of the ImgReader_LIBRARY
#
#        IRD_FOUND          Is set to true if the library is found
#

  
find_path (IRD_INC
    NAMES
        imgReader.hpp optionChecker.hpp
)

## --------------------------------
set (IRD_FOUND FALSE)
if (IRD_INC)
    set (IRD_INCLUDE_DIR ${IRD_INC})
    set (IRD_FOUND TRUE)
endif (IRD_INC)

set (IRD_INCLUDE_DIR ${IRD_INCLUDE_DIR} CACHE STRING "include directory for ImgReader")

mark_as_advanced (
  IRD_INCLUDE_DIR
)


