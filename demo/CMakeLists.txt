cmake_minimum_required(VERSION 3.1)

project(usdmat_demo)

set(CMAKE_CXX_STANDARD 14)

find_package(Eigen3 3.4 REQUIRED NO_MODULE)
find_package(pxr REQUIRED)

# example abc file
if(NOT EXISTS octopus_example.tgz)
set(ABC_URL https://storage.googleapis.com/google-code-archive-downloads/v2/code.google.com/alembic/Alembic_Octopus_Example.tgz)
file(
  DOWNLOAD ${ABC_URL} octopus_example.tgz
  TIMEOUT 60
)
endif()
file(ARCHIVE_EXTRACT INPUT octopus_example.tgz DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_definitions(-DOCTOPUS_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/Alembic_Octopus_Example/alembic_octopus.abc\")

# example demo program
add_executable(usdmat_demo main.cpp)
target_include_directories(usdmat_demo PRIVATE ${PXR_INCLUDE_DIRS})
target_link_libraries(usdmat_demo Eigen3::Eigen ${PXR_LIBRARIES})
#install(TARGETS usdmat_demo DESTINATION .)