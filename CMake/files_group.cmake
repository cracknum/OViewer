# group_qt_sources.cmake

function(group_sources_by_directory TARGET_NAME)
    get_target_property(ALL_SOURCES ${TARGET_NAME} SOURCES)

    foreach(FILE_PATH IN LISTS ALL_SOURCES)
        get_source_file_property(IS_GENERATED "${FILE_PATH}" GENERATED)
        get_filename_component(FILE_NAME "${FILE_PATH}" NAME)

        if(IS_GENERATED STREQUAL "TRUE")
            # ---- 分类 Qt 自动生成文件 ----
            if(FILE_NAME MATCHES "^mocs_compilation_.*\\.cpp$" OR FILE_NAME MATCHES "^moc_.*\\.cpp$")
                set(GROUP "Generated Files/MOC")
            elseif(FILE_NAME MATCHES "^ui_.*\\.h$")
                set(GROUP "Generated Files/UI")
            elseif(FILE_NAME MATCHES "^qrc_.*\\.cpp$")
                set(GROUP "Generated Files/Resources")
            else()
                # 其他未识别的生成文件（如 lex/yacc 等）
                file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_BINARY_DIR}" "${FILE_PATH}")
                string(REPLACE "\\" "/" REL_PATH "${REL_PATH}")
                get_filename_component(DIR_PART "${REL_PATH}" DIRECTORY)
                if(DIR_PART STREQUAL "")
                    set(GROUP "Generated Files/Other")
                else()
                    set(GROUP "Generated Files/Other/${DIR_PART}")
                endif()
            endif()
        else()
            # ---- 用户源文件：按源码目录结构分组 ----
            file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${FILE_PATH}")
            string(REPLACE "\\" "/" REL_PATH "${REL_PATH}")
            get_filename_component(DIR_PART "${REL_PATH}" DIRECTORY)
            if(DIR_PART STREQUAL "")
                set(GROUP "Source Files")
            else()
                set(GROUP "${DIR_PART}")
            endif()
        endif()

        source_group("${GROUP}" FILES "${FILE_PATH}")
    endforeach()
endfunction()