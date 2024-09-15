cmake_minimum_required(VERSION 3.0.0)
project(voxel-engine VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/include/glad"
	"${CMAKE_SOURCE_DIR}/include/glm"
)

link_directories(${CMAKE_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE HEADER_FILES
	${CMAKE_SOURCE_DIR}/src/*.hpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_library(GLFW3 "lib/libglfw3.a")

add_library(GLAD "${CMAKE_SOURCE_DIR}/lib/glad.c")

set(LIBS GLFW3 ${OPENGL_LIBRARY} GLAD)

target_link_libraries(${PROJECT_NAME} ${LIBS})

message(STATUS "Source files: ${SOURCE_FILES}")
message(STATUS "Header files: ${HEADER_FILES}")
