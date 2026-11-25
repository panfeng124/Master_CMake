set(LAB_VERBOSE ON)
add_library(lab_core STATIC diagnostics.cpp)
target_include_directories(lab_core PUBLIC include)

add_library(lab_status STATIC status.cpp)
target_link_libraries(lab_status PUBLIC lab_core)

add_executable(lab_probe main.cpp)
target_link_libraries(lab_probe PRIVATE lab_status)

message(STATUS "inner:" ${LAB_VERBOSE})

if (LAB_VERBOSE)
    target_compile_definitions(lab_core PRIVATE LAB_VERBOSE)
endif ()
