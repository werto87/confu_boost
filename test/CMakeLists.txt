set(TEST_SOURCES_${TARGET_NAME} ${TEST_SOURCES_${TARGET_NAME}}
${CMAKE_CURRENT_SOURCE_DIR}/main.cxx 
${CMAKE_CURRENT_SOURCE_DIR}/test.cxx
PARENT_SCOPE
)


set(TEST_HEADER_${TARGET_NAME} ${TEST_HEADER_${TARGET_NAME}}
${CMAKE_CURRENT_SOURCE_DIR}/constant.hxx
PARENT_SCOPE
)
