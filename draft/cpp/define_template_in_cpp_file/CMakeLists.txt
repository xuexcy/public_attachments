get_filename_component(CURRENT_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
get_filename_component(FOLDER_NAME ${CURRENT_DIR} NAME)
if(DEFINED ENV{build_output_bin_dir})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{build_output_bin_dir}/${FOLDER_NAME})
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/bin/${FOLDER_NAME})
endif()
message("CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(test_temp_1 test_temp_1.cpp)
add_library(test_temp_2 test_temp_2.cpp)

add_executable(dticf main.cc)
target_link_libraries(dticf test_temp_1 test_temp_2)
