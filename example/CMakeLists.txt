cmake_minimum_required (VERSION 2.4...3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("Extractor-Gui-Example")
set(EX_GUI_EXAMPLE_SOURCES )

IF (WIN32)
	add_compile_definitions(WIN32_LEAN_AND_MEAN)
	add_compile_definitions(NOMINMAX)
ENDIF(WIN32)

set(EXAMPLE_MAIN_SRC
	"src/main.cpp"
	"src/ExampleApp.cpp"
	"include/ExampleApp.h"
)
source_group("Example" FILES ${EXAMPLE_MAIN_SRC})
list(APPEND EX_GUI_EXAMPLE_SOURCES ${EXAMPLE_MAIN_SRC})

add_executable(Extractor-Gui-Example ${EX_GUI_EXAMPLE_SOURCES})
target_link_libraries(Extractor-Gui-Example Extractor-Core)

include_directories("include")
include_directories("${CMAKE_SOURCE_DIR}/ext/imgui")
include_directories("${CMAKE_SOURCE_DIR}/ext/glm/glm")
include_directories("${CMAKE_SOURCE_DIR}/ext/spdlog/include")
include_directories("${CMAKE_SOURCE_DIR}/ext/zlib/")
include_directories("${CMAKE_SOURCE_DIR}/ext/include")
include_directories("${CMAKE_SOURCE_DIR}/include")