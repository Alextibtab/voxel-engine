cmake_minimum_required(VERSION 3.0.0)
project(voxel-engine VERSION 0.0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

link_directories(${CMAKE_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE HEADER_FILES
	${CMAKE_SOURCE_DIR}/src/*.hpp)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_library(GLFW3 "lib/libglfw3.a")

add_library(GLAD "lib/glad.c")

set(LIBS GLFW3 ${OPENGL_LIBRARY} GLAD)

include_directories(
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/include/glad"
	"${CMAKE_SOURCE_DIR}/include/glm"
)

target_link_libraries(${PROJECT_NAME} ${LIBS})
