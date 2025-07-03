get_filename_component(CURRENT_DIR ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
get_filename_component(FOLDER_NAME ${CURRENT_DIR} NAME)
if(DEFINED ENV{build_output_bin_dir})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{build_output_bin_dir}/${FOLDER_NAME})
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/bin/${FOLDER_NAME})
endif()
message("CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# set(main_name main.cc)
# add_executable({main_name} ${main_name}.cc)

add_executable(is_all_true is_all_true/main.cc)
add_executable(three_way_comparation three_way_comparation/main.cc)
add_executable(concept concept/main.cc)
add_executable(generator generator/main.cc)
