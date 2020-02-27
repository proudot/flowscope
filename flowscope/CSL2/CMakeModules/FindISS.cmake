# File : ---- FindISS.cmake
# Author : Antoine Basset
# INRIA Serpico team
#
# Description : Try to find image subsets headers:
#
#        ISS_INCLUDE_DIR    The headers directory of the ISS_LIBRARY
#
#        ISS_FOUND          Is set to true if the headers are found
#

  
find_path (ISS_INC
    NAMES
        imgNeighborhood.hpp
        imgSubset.hpp
        integralImg.hpp
        geometry.hpp
)

## --------------------------------
set (ISS_FOUND FALSE)
if (ISS_INC)
    set (ISS_INCLUDE_DIR ${ISS_INC})
    set (ISS_FOUND TRUE)
endif (ISS_INC)

set (ISS_INCLUDE_DIR $ISS_INCLUDE_DIR} CACHE STRING "include directory for image subsets")

mark_as_advanced (
  TBI_INCLUDE_DIR
)

