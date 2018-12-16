# kevin server

file (GLOB KEVIN_SERVER_SRCS "${PROJECT_SOURCE_DIR}/server/*.cpp")
add_executable (kevin ${KEVIN_SERVER_SRCS})
target_link_libraries(kevin ${kevin-thirdparties}
                            $<TARGET_OBJECTS:kevin-proto>
                            $<TARGET_OBJECTS:kevin-common>
                            $<TARGET_OBJECTS:kevin-logging>
                            # TODO(yihao) use kevin-test object only for test
                            # related targets later
                            $<TARGET_OBJECTS:kevin-test>
                            )
install (TARGETS kevin 
    RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin
    LIBRARY DESTINATION ${PROJECT_BINARY_DIR}/libs
    ARCHIVE DESTINATION ${PROJECT_BINARY_DIR}/bin
    )

