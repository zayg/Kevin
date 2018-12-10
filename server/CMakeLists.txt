# kevin server
add_executable (kevin
    "${PROJECT_SOURCE_DIR}/server/kevin.cpp")
target_link_libraries(kevin ${THIRDPARTY_LIBS})
install(TARGETS kevin 
    RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
    LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/libs
    ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/bin
    )

