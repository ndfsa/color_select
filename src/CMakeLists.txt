find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(color_select color_select.cpp)
target_link_libraries( color_select ${OpenCV_LIBS} )
