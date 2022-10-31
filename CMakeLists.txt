cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(DataStructures VERSION 1.0)

add_subdirectory(Structures)

add_executable(DataStructures main.cpp)

target_include_directories(DataStructures PUBLIC
	"${PROJECT_SOURCE_DIR}/Structures"
)
target_link_libraries(DataStructures Structures)