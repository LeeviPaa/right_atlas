set(PROJECT_FILES PngImage.h PngImage.cpp)

if(UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif()

if(LINUX)
ADD_EXECUTABLE( MakeAtlas 
	Main.cpp 
	${PROJECT_FILES}
)
find_package(PNG REQUIRED)
if(PNG_FOUND)
	include_directories(${PNG_INCLUDE_DIRS})
	target_link_libraries(MakeAtlas ${PNG_LIBRARIES})
endif(PNG_FOUND)
endif(LINUX)

if(WIN32)
set(HEADER_FILES include/png.h include/pngconf.h include/pnglibconf.h include/zconf.h include/zlib.h)
ADD_EXECUTABLE( MakeAtlas 
	Main.cpp 
	${PROJECT_FILES}
	${HEADER_FILES}
) 

target_include_directories(MakeAtlas PRIVATE include)

find_library(ZLIB_LOCATION zlibstat.lib
	HINTS "lib"
)
target_link_libraries(MakeAtlas ${ZLIB_LOCATION})

find_library(ZLIBD_LOCATION zlibstatd.lib
	HINTS "lib"
)
target_link_libraries(MakeAtlas ${ZLIBD_LOCATION})

find_library(PNGDLIB_LOCATION libpngd.lib
	HINTS "lib"
)
target_link_libraries(MakeAtlas ${PNGDLIB_LOCATION})

find_library(PNGLIB_LOCATION libpng.lib
	HINTS "lib"
)
target_link_libraries(MakeAtlas ${PNGLIB_LOCATION})
endif(WIN32)
