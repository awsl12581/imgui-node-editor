project(application)

set(_Application_Sources
    include/application.h
    source/application.cpp
    source/entry_point.cpp
    source/config.h.in
    source/setup.h
    source/platform.h
    source/platform_win32.cpp
    source/platform_glfw.cpp
    source/renderer.h
    source/renderer_dx11.cpp
    source/renderer_ogl3.cpp
)

add_library(application STATIC)

target_include_directories(application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)

# Setup OpenGL
cmake_policy(SET CMP0072 NEW) # Pefer GLVND over legacy GL libraries
find_package(OpenGL REQUIRED)

# Setup GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG "3.3.8"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

# Setup ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY "https://github.com/ocornut/imgui"
    GIT_TAG "v1.90.9"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(imgui)
set(IMGUI_SOURCE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SOURCE})
target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR};${imgui_SOURCE_DIR}/backends/")
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

set(imgui_components)
# if (WIN32)
#     list(APPEND imgui_components win32 dx11)
#     target_link_libraries(application PRIVATE imgui::win32 imgui::dx11)
# else()
find_package(glfw REQUIRED)
list(APPEND imgui_components glfw opengl3)
target_link_libraries(application PRIVATE imgui::glfw imgui::opengl3)
set(HAVE_GLFW3 YES)
# endif()


find_package(imgui REQUIRED COMPONENTS ${imgui_components})
find_package(stb REQUIRED COMPONENTS image)
target_link_libraries(application PUBLIC imgui)
target_link_libraries(application PRIVATE stb::image)

if (OpenGL_FOUND)
    set(HAVE_OPENGL YES)
endif()

configure_file(
    source/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/source/config.h
)

target_compile_definitions(application PRIVATE
    BACKEND_CONFIG=IMGUI_GLFW
    RENDERER_CONFIG=IMGUI_OGL3
)

target_include_directories(application PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/source)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${_Application_Sources})

target_sources(application PRIVATE ${_Application_Sources})

set_property(TARGET application PROPERTY FOLDER "lib")