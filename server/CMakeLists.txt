# kevin server

file (GLOB KEVIN_SERVER_SRCS "${PROJECT_SOURCE_DIR}/server/*.cpp")
set_source_files_properties(${KEVIN_PROTO_SRCS} PROPERTIES GENERATED TRUE)
add_executable (kevin ${KEVIN_SERVER_SRCS} ${KEVIN_PROTO_SRCS})
target_link_libraries(kevin ${KEVIN_THIRDPARTY_LIBS})
add_dependencies (kevin kevin-proto)
install (TARGETS kevin 
    RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
    LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/libs
    ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/bin
    )

