macro(export_model config_file_path project_name)
    string(TOUPPER ${project_name} MODULE_NAME)
    set(export_file ${CMAKE_CURRENT_SOURCE_DIR}/include/${project_name}Export.h)
    if (NOT EXISTS ${export_file})
        configure_file(
                ${export_config_file_path}
                ${CMAKE_CURRENT_SOURCE_DIR}/include/${project_name}Export.h
        )
    endif ()
    if (NOT TARGET ${project_name})
        message(FATAL_ERROR "target ${project_name} is not exist")
    else ()
        target_compile_definitions(${project_name} PRIVATE ${MODULE_NAME}_EXPORTS)
    endif ()
endmacro()
