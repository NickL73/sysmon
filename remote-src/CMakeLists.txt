set(AGENT_BIN_NAME agent-${CMAKE_SYSTEM_PROCESSOR})

add_subdirectory(networking)

add_executable(${AGENT_BIN_NAME} agent.c)
target_link_libraries(${AGENT_BIN_NAME} PRIVATE sysmon-networking)
set_target_properties(${AGENT_BIN_NAME} PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
install(TARGETS ${AGENT_BIN_NAME} DESTINATION ${PROJECT_SOURCE_DIR}/bin)