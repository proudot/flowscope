# File : ---- FindTBI.cmake
# Author : Tristan Lecorgne
# INRIA Serpico team
#
# Description : Try to find TBI library and protobuf:
#
#        TBI_LIBRARIES        The .so/.a/.dll/... to link against
#        TBI_INCLUDE_DIR    The headers directory of the TBI_LIBRARIES
#
#        TBI_FOUND          Is set to true if the library is found
#


# Find libtbi
find_library (TBI_LIB
    NAMES 
        tbi
    PATHS
        /soft/serpico/lib
        /usr/lib
)
  
find_path (TBI_INC
    NAMES
        tbi.pb.h
    PATHS
        /soft/serpico/include
        ../../libraries/trackingBinaryInterface/cpp
    
)

find_package(Protobuf)


## --------------------------------
set (TBI_FOUND FALSE)
if (PROTOBUF_FOUND AND TBI_LIB AND TBI_INC)
    set (TBI_LIBRARIES ${TBI_LIB} ${PROTOBUF_LIBRARIES})
    set (TBI_INCLUDE_DIR ${TBI_INC} ${PROTOBUF_INCLUDE_DIRS} )
    set (TBI_FOUND TRUE)
endif (PROTOBUF_FOUND AND TBI_LIB AND TBI_INC)

set (TBI_LIBRARY ${TBI_LIBRARIES} CACHE STRING "lib for TBI")
set (TBI_INCLUDE_DIR ${TBI_INCLUDE_DIR} CACHE STRING "include directory for TBI")

mark_as_advanced (
  TBI_LIBRARY
  TBI_INCLUDE_DIR
)


