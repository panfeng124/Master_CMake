message(STATUS "app entry: ${LAB_VERBOSE}")
# 在这里尝试为当前目录设置普通变量 LAB_VERBOSE。
set(LAB_VERBOSE "ON")
message(STATUS "app target: ${LAB_VERBOSE}")

add_executable(lab_probe main.cpp)
if(LAB_VERBOSE)
    target_compile_definitions(lab_probe PRIVATE LAB_VERBOSE)
endif()
