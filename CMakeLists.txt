# Project definition
project(CinderTest)
cmake_minimum_required(VERSION 3.22)

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
add_executable(${PROJECT_NAME} ${SRC_DIR}/BasicApp.cpp)

# Dependencies
target_include_directories(${PROJECT_NAME}
        PUBLIC
        )

### External dependencies

# Cinder
get_filename_component(CINDER_PATH "${CMAKE_HOME_DIRECTORY}/../../Software/Cinder/" ABSOLUTE)
# Include Cinder's specific configure file which defines some basic variables
# based on the OS, build type etc you are running..
include("${CINDER_PATH}/proj/cmake/configure.cmake")
# Pull Cinder's package configuration file. This will pull in all Cinder's include and link dependencies for you.
# ${CINDER_LIB_DIRECTORY} will be pulled in from the above configure.cmake file.
find_package(cinder REQUIRED PATHS "${CINDER_PATH}/${CINDER_LIB_DIRECTORY}")
target_link_libraries(${PROJECT_NAME} PUBLIC cinder)
