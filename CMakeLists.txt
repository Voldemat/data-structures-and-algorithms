cmake_minimum_required(VERSION 4.1.0)
project(data-structures-and-algorithms VERSION 0.0.1 LANGUAGES C)

function(add_lab directory_name)
    file(
        GLOB_RECURSE SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/${directory_name}/*.c
    )

    add_executable(${directory_name} ${SOURCES})

    set_target_properties(${directory_name} PROPERTIES
        C_STANDARD 23
        EXPORT_COMPILE_COMMANDS ON
    )

    target_include_directories(
        ${directory_name}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${directory_name}/
    )
endfunction()

add_lab(1-lab)
