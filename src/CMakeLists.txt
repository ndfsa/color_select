add_executable(color_picker main.cpp)
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=1 )

find_package( ImageMagick COMPONENTS Magick++ )
include_directories( ${ImageMagick_INCLUDE_DIRS} )
target_link_libraries( color_picker ${ImageMagick_LIBRARIES} )
