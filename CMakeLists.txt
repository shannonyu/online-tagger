PROJECT (OnlineTagger)

SET (
    CMAKE_CXX_FLAGS 
    "$ENV{CXXFLAGS} -g -O3 -Wno-deprecated -ftemplate-depth-50 -fPIC"
)

SET (
    EXECUTABLE_OUTPUT_PATH 
    ${PROJECT_SOURCE_DIR}/bin
)

SET (
    LIBRARY_OUTPUT_PATH 
    ${PROJECT_SOURCE_DIR}/lib
)

ADD_SUBDIRECTORY(src)

CONFIGURE_FILE (
    ./src/demo/postag/otpos_dll.h
    ./include/otpos_dll.h
)

CONFIGURE_FILE (
    ./src/demo/segment/otws_dll.h
    ./include/otws_dll.h
)

ADD_SUBDIRECTORY(test)
