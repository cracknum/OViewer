function(resource_copy)
    set(RESOURCE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
    set(RESOURCE_TARGET_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources")

    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${RESOURCE_SOURCE_DIR}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
        COMMENT "Copying assets to $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources"
    )
endfunction()