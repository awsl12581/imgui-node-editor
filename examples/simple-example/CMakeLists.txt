# Setup ImPlot
FetchContent_Declare(
    implot
    GIT_REPOSITORY "https://github.com/epezent/implot"
    GIT_TAG "3da8bd34299965d3b0ab124df743fe3e076fa222"
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(implot)
set(IMPLOT_SOURCE
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_demo.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
)
add_library(implot STATIC ${IMPLOT_SOURCE})
target_include_directories(implot PUBLIC ${implot_SOURCE_DIR})
target_link_libraries(implot PUBLIC imgui)

# Setup ImPlot3D
set(IMPLOT3D_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(IMPLOT3D_SOURCE
    ${IMPLOT3D_SOURCE_DIR}/implot3d.cpp
    ${IMPLOT3D_SOURCE_DIR}/implot3d_demo.cpp
    ${IMPLOT3D_SOURCE_DIR}/implot3d_items.cpp
    ${IMPLOT3D_SOURCE_DIR}/implot3d_meshes.cpp
)
add_library(implot3d STATIC ${IMPLOT3D_SOURCE})
target_include_directories(implot3d PUBLIC ${IMPLOT3D_SOURCE_DIR})
target_link_libraries(implot3d PUBLIC imgui)



add_example_executable(simple-example
    simple-example.cpp
)

target_link_libraries(simple-example PRIVATE implot implot3d)