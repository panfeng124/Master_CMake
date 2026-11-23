#set(LAB_VERBOSE ON)

add_executable(lab_probe main.cpp diagnostics.cpp)
if (LAB_VERBOSE)
    target_compile_definitions(lab_probe PRIVATE LAB_VERBOSE)
endif ()
