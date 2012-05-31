INCLUDE_DIRECTORIES (
    ./
    ${PROJECT_SOURCE_DIR}/include)

LINK_DIRECTORIES (
    ${PROJECT_SOURCE_DIR}/lib)

ADD_EXECUTABLE (test_otws
    ./wordseg.cpp)

ADD_EXECUTABLE (test_otpos
    ./postag.cpp)

TARGET_LINK_LIBRARIES (
    test_otws
    libotws.a )

TARGET_LINK_LIBRARIES (
    test_otpos
    libotpos.a )

