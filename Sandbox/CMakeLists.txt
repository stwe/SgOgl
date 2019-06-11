project(Sandbox)

set(SANDBOX_SRC_FILES src/Sandbox.cpp)

add_executable(${PROJECT_NAME} ${SANDBOX_SRC_FILES})

target_link_libraries(${PROJECT_NAME} Sg::GameEngineLib)
