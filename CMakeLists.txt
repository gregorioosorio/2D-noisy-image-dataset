cmake_minimum_required(VERSION 3.16.3)

project(Slice3DImage)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

include_directories(
)

add_executable(
  ${PROJECT_NAME}
  Slice3DImage.cxx
)

target_link_libraries(${PROJECT_NAME} ${ITK_LIBRARIES})