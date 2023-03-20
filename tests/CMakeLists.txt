cmake_minimum_required (VERSION 3.13.0)

project(test-corgi-plot)

add_executable(${PROJECT_NAME} "") 

# I'm not sure I understand all the advantages on using target_sources but at least
# it's one way to avoid adding variables, and to define our target early
target_sources(${PROJECT_NAME} PRIVATE src/main.cpp)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD # Adds a post build event to the given target
    COMMAND ${CMAKE_COMMAND} -E copy_directory # Executes the copy directory command
    "${CMAKE_SOURCE_DIR}/resources"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

target_link_libraries(${PROJECT_NAME} corgi-plot ${CONAN_LIBS})

set_property(TARGET ${PROJECT_NAME}  PROPERTY CXX_STANDARD 20)

add_test( NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
