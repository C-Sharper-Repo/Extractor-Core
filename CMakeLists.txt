cmake_minimum_required (VERSION 2.4...3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WARN_DEPRECATED OFF)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

project ("Extractor-Core")

option(EX_BASE_BUILD_EXAMPLES "Build the Extractor GUI Example" ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(EXTRACTOR_GUI_SOURCES )

add_subdirectory("ext/glm")
add_subdirectory("ext/imgui")
add_subdirectory("ext/spdlog")
add_subdirectory("ext/json")
add_subdirectory("ext/zlib")
add_subdirectory("ext/glfw-3.3.8")

add_compile_definitions(GLEW_STATIC)
IF (WIN32)
	add_compile_definitions(WIN32_LEAN_AND_MEAN)
	add_compile_definitions(NOMINMAX)
ENDIF(WIN32)

# OpenGL Glew files
set(GLEW_SRC
	"ext/include/GL/wglew.h"
	"ext/include/GL/glew.h"
	"ext/src/glew.c"
)
source_group("ExCore/OpenGL/Glew" FILES ${GLEW_SRC})
list(APPEND EXTRACTOR_GUI_SOURCES ${GLEW_SRC})

set(EX_GUI_MAIN_SRC
	"include/ExCore/EntryPoint.h"
	"include/ExCore/Application.h"
	"src/ExCore/Application.cpp"
	
	"include/ExCore/Log.h"
	"src/ExCore/Log.cpp"
)
source_group("ExCore" FILES ${EX_GUI_MAIN_SRC})
list(APPEND EXTRACTOR_GUI_SOURCES ${EX_GUI_MAIN_SRC})

set(EX_GUI_MAIN_SRC
	"include/ExCore/Rendering/Window.h"
	"src/ExCore/Rendering/Window.cpp"
	
	"include/ExCore/Rendering/Texture.h"
	"src/ExCore/Rendering/Texture.cpp"
	
	"include/ExCore/Rendering/DynamicBatch.h"
	
	"include/ExCore/Rendering/Sprite.h"
	"src/ExCore/Rendering/Sprite.cpp"
	
	"include/ExCore/Rendering/Animation.h"
	"src/ExCore/Rendering/Animation.cpp"
	
	"include/ExCore/Rendering/Renderer.h"
	"src/ExCore/Rendering/Renderer.cpp"
	
	"include/ExCore/Rendering/SortingLayer.h"
	"include/ExCore/Rendering/Renderable.h"
	"src/ExCore/Rendering/Renderable.cpp"
	"include/ExCore/Rendering/ICamera.h"	
	
	"include/ExCore/Rendering/Shader.h"
	"src/ExCore/Rendering/Shader.cpp"
)
source_group("ExCore/Rendering" FILES ${EX_GUI_MAIN_SRC})
list(APPEND EXTRACTOR_GUI_SOURCES ${EX_GUI_MAIN_SRC})

set(EX_GUI_MAIN_SRC
	"include/ExCore/Rendering/Buffers/BufferLayout.h"
	"include/ExCore/Rendering/Buffers/IndexBuffer.h"
	"src/ExCore/Rendering/Buffers/IndexBuffer.cpp"
	"include/ExCore/Rendering/Buffers/VertexArray.h"
	"src/ExCore/Rendering/Buffers/VertexArray.cpp"
	"include/ExCore/Rendering/Buffers/VertexBuffer.h"
	"src/ExCore/Rendering/Buffers/VertexBuffer.cpp"
	
	"include/ExCore/Rendering/Buffers/FrameBuffer.h"
	"src/ExCore/Rendering/Buffers/FrameBuffer.cpp"
)
source_group("ExCore/Rendering/Buffers" FILES ${EX_GUI_MAIN_SRC})
list(APPEND EXTRACTOR_GUI_SOURCES ${EX_GUI_MAIN_SRC})

set(EX_GUI_MAIN_SRC
	"include/ExCore/IO/Stream.h"
	
	"include/ExCore/IO/FileStream.h"
	"src/ExCore/IO/FileStream.cpp"
	
	"include/ExCore/IO/MemoryStream.h"
	"src/ExCore/IO/MemoryStream.cpp"
	
	"include/ExCore/IO/BitStream.h"
	"src/ExCore/IO/BitStream.cpp"
	
	"include/ExCore/IO/Image.h"
	"src/ExCore/IO/Image.cpp"
	
	"include/ExCore/IO/IOUtils.h"
	"src/ExCore/IO/IOUtils.cpp"
	
	"include/ExCore/IO/ZLib.h"
	"src/ExCore/IO/ZLib.cpp"
)
source_group("ExCore/IO" FILES ${EX_GUI_MAIN_SRC})
list(APPEND EXTRACTOR_GUI_SOURCES ${EX_GUI_MAIN_SRC})

set(EX_GUI_MAIN_SRC
 "include/ExCore/Util/EnumUtils.h"
 "include/ExCore/Util/DataUtilities.h"
 "include/ExCore/Util/HeapVector.h"
 "include/ExCore/Util/IComparable.h"
)
source_group("ExCore/Util" FILES ${EX_GUI_MAIN_SRC})
list(APPEND EXTRACTOR_GUI_SOURCES ${EX_GUI_MAIN_SRC})

set(EX_GUI_MAIN_SRC
 "include/ExCore/Gui/IGuiDrawable.h"
 "include/ExCore/Gui/IGuiExtras.h"
 "src/ExCore/Gui/IGuiExtras.cpp"
)
source_group("ExCore/Gui" FILES ${EX_GUI_MAIN_SRC})
list(APPEND EXTRACTOR_GUI_SOURCES ${EX_GUI_MAIN_SRC})

set(EX_GUI_MAIN_SRC
	"include/ExCore/Scene/Time.h"
	
	"include/ExCore/Scene/Scene.h"
	"src/ExCore/Scene/Scene.cpp"

	"include/ExCore/Scene/Component.h"
	"src/ExCore/Scene/Component.cpp"

	"include/ExCore/Scene/Object.h"
	"src/ExCore/Scene/Object.cpp"
	
	"include/ExCore/Scene/Transform.h"
	"src/ExCore/Scene/Transform.cpp"
)
source_group("ExCore/Scene" FILES ${EX_GUI_MAIN_SRC})
list(APPEND EXTRACTOR_GUI_SOURCES ${EX_GUI_MAIN_SRC})

set(EX_GUI_MAIN_SRC
	"include/ExCore/Scene/Components/Animator.h"
	"src/ExCore/Scene/Components/Animator.cpp"

	"include/ExCore/Scene/Components/SpriteRenderer.h"
	"src/ExCore/Scene/Components/SpriteRenderer.cpp"

	"include/ExCore/Scene/Components/Camera.h"
	"src/ExCore/Scene/Components/Camera.cpp"
)
source_group("ExCore/Scene/Components" FILES ${EX_GUI_MAIN_SRC})
list(APPEND EXTRACTOR_GUI_SOURCES ${EX_GUI_MAIN_SRC})


set(EX_GUI_MAIN_SRC
	"include/ExCore/Math/Color32.h"
	"src/ExCore/Math/Color32.cpp"
	
	"include/ExCore/Math/Color24.h"
	"src/ExCore/Math/Color24.cpp"
	
	"include/ExCore/Math/Rect.h"
	"include/ExCore/Math/Vertex.h"
	
	"include/ExCore/Math/Matrix4f.h"
	"src/ExCore/Math/Matrix4f.cpp"
	
	"include/ExCore/Math/Math.h"
	"src/ExCore/Math/Math.cpp"
)
source_group("ExCore/Math" FILES ${EX_GUI_MAIN_SRC})
list(APPEND EXTRACTOR_GUI_SOURCES ${EX_GUI_MAIN_SRC})

find_package(OpenGL REQUIRED)
add_library(Extractor-Core STATIC ${EXTRACTOR_GUI_SOURCES})
target_link_libraries(Extractor-Core OpenGL::GL glfw ImGUI glm zlibstatic spdlog nlohmann_json)

target_include_directories(Extractor-Core PUBLIC ${CMAKE_BINARY_DIR}/ext/zlib)

include_directories("include")
include_directories("ext/glm/glm")
include_directories("ext/imgui")
include_directories("ext/json/include")
include_directories("ext/include")
include_directories("ext/spdlog/include")
include_directories("ext/zlib/")

if(EX_BASE_BUILD_EXAMPLES)
	add_subdirectory("example")
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Extractor-Gui-Example)
endif()
