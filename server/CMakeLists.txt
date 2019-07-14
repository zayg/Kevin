# kevin server

file (GLOB KEVIN_SERVER_SRCS "${PROJECT_SOURCE_DIR}/server/*.cpp")
add_executable (kevin
    ${KEVIN_SERVER_SRCS}
    $<TARGET_OBJECTS:kevin-proto>
    $<TARGET_OBJECTS:kevin-common>
    $<TARGET_OBJECTS:kevin-raft>
    )
target_link_libraries(kevin ${kevin-thirdparties})
install (TARGETS kevin RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin)

