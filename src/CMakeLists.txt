find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(color_picker main.cpp)
target_link_libraries( color_picker ${OpenCV_LIBS} )
