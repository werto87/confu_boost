cmake_minimum_required(VERSION 3.17)
project(convenienceFunctionForSoci VERSION 1.0.0
  DESCRIPTION convenienceFunctionForSoci
  LANGUAGES CXX
  ) 
cmake_policy(SET CMP0015 NEW)




########## enable cache system
include(cmake/Cache.cmake)

#set(CMAKE_EXECUTABLE_SUFFIX ".html")
########## link against the libs to apply the configurations to the target
# compiler warnings
set(PROJECT_WARNINGS project_warning)
add_library(${PROJECT_WARNINGS} INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_WARNINGS})
# compiler options
set(PROJECT_OPTIONS project_options)


add_library(${PROJECT_OPTIONS} INTERFACE)
target_compile_features(${PROJECT_OPTIONS} INTERFACE cxx_std_20)



# include(cmake/Sanitizers.cmake)
# enable_sanitizers(${PROJECT_OPTIONS})

# allow for static analysis options
#include(cmake/StaticAnalyzers.cmake) 

OPTION(BUILD_WASM "build wasm" OFF)

######## conan setup
IF(BUILD_WASM)
SET(CONAN_DISABLE_CHECK_COMPILER ON)
ENDIF(BUILD_WASM)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)


####### create executable


set(TARGET_NAME confuBoost)
set(SOURCES_${TARGET_NAME})
set(HEADER_${TARGET_NAME})

add_library(${TARGET_NAME})

set_target_properties(${TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)

######## add sub dir
add_subdirectory(confu_boost) 

####### create executable
OPTION(BUILD_TESTS "enable tests" OFF)
IF(BUILD_TESTS)

  enable_testing()
  set(TEST_TARGET_NAME ${TARGET_NAME}_test)
  set(TEST_SOURCES_${TARGET_NAME})
  set(TEST_HEADER_${TARGET_NAME})
  add_subdirectory(test) 
  add_executable(${TEST_TARGET_NAME}

    ${HEADER_${TARGET_NAME}}
    ${SOURCES_${TARGET_NAME}}
    ${TEST_HEADER_${TARGET_NAME}}
    ${TEST_SOURCES_${TARGET_NAME}}
  )
  target_link_libraries(${TEST_TARGET_NAME}
    ${PROJECT_OPTIONS}
    CONAN_PKG::catch2
    CONAN_PKG::boost
  )
  if (CMAKE_BINARY_DIR)
    target_compile_definitions(${TEST_TARGET_NAME} PRIVATE PATH_TO_BINARY="${CMAKE_BINARY_DIR}")
  endif()
  target_include_directories(${TEST_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR})

  add_test(NAME ${TEST_TARGET_NAME}
          COMMAND ${TEST_TARGET_NAME} -r junit) 
ENDIF(BUILD_TESTS)



target_sources(${TARGET_NAME} PRIVATE 
${SOURCES_${TARGET_NAME}}
${HEADER_${TARGET_NAME}}
)
####### link



if(BUILD_WASM)
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${TARGET_NAME} PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  /home/walde/some_boost_test/modular-boost
)
else()
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${TARGET_NAME} PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
endif()


  

if(BUILD_WASM)
target_link_directories(${TARGET_NAME} PRIVATE
  /home/walde/some_boost_test/modular-boost/stage/lib
 )


else()
target_link_directories(${TARGET_NAME} PRIVATE
 )
endif()



IF(BUILD_WASM)
  target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_WARNINGS} ${PROJECT_OPTIONS}
  libboost_serialization.a
  )
ELSE()
  target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_WARNINGS} ${PROJECT_OPTIONS}
  CONAN_PKG::boost
  )
ENDIF()

