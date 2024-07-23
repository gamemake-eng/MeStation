project(mestation)

add_subdirectory(unicorn)

file (GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.c src/*.h)

add_executable(meemu ${sources})

target_include_directories(meemu PUBLIC ${CMAKE_CURRENT_SOURCE_DIRECTORY} src unicorn/include)
target_link_libraries(meemu unicorn)
