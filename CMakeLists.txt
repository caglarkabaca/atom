cmake_minimum_required(VERSION 3.21)

project(atom)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include)
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (WIN32)
    set(SDL2_DIR "${CMAKE_SOURCE_DIR}/external/SDL2/cmake")
    set(SDL2_image_DIR "${CMAKE_SOURCE_DIR}/external/SDL2_image")
    set(SDL2_ttf_DIR "${CMAKE_SOURCE_DIR}/external/SDL2_ttf")

    set(GLEW_DIR "${CMAKE_SOURCE_DIR}/external/glew")
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

if (UNIX AND NOT APPLE)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(GLEW REQUIRED)
    include_directories(
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
    )
endif()

if (WIN32)

    set(SDL2_BIN "${CMAKE_SOURCE_DIR}/external/SDL2/x86_64-w64-mingw32/bin")
    set(SDL2_image_BIN "${CMAKE_SOURCE_DIR}/external/SDL2_image/x86_64-w64-mingw32/bin")
    set(SDL2_ttf_BIN "${CMAKE_SOURCE_DIR}/external/SDL2_ttf/x86_64-w64-mingw32/bin")
    set(GLEW_BIN "${GLEW_DIR}/bin/Release/x64")

    set(SDL2_IMAGE_INCLUDE_DIR ${SDL2_image_DIR}/x86_64-w64-mingw32/include)
    set(SDL2_TTF_INCLUDE_DIR ${SDL2_ttf_DIR}/x86_64-w64-mingw32/include)
    set(GLEW_INCLUDE_DIR "${GLEW_DIR}/include")

    set(SDL2_IMAGE_LIBRARIES ${SDL2_image_DIR}/x86_64-w64-mingw32/lib)
    set(SDL2_TTF_LIBRARIES ${SDL2_ttf_DIR}/x86_64-w64-mingw32/lib)
    set(GLEW_LIBRARIES "${GLEW_DIR}/lib/Release/x64")

    link_directories(
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${GLEW_LIBRARIES}
    )

    include_directories(
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
    )

endif()

find_package(OpenGL)

set(ATOM_SOURCE_DIR ${PROJECT_SOURCE_DIR}/atom)
set(IMGUI_SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/ImGUI)

include_directories(
    ${ATOM_SOURCE_DIR}/include
    ${IMGUI_SOURCE_DIR}/include
)
file(GLOB_RECURSE SRC "${ATOM_SOURCE_DIR}/src/*.cpp" "${ATOM_SOURCE_DIR}/include/*.h" "${IMGUI_SOURCE_DIR}/src/*.cpp")

message(STATUS "Add source files:")
foreach(SRC_FILE IN LISTS SRC)
    message(STATUS "${SRC_FILE}")
endforeach()
message(STATUS "")

add_executable(${PROJECT_NAME} ${SRC})

if (WIN32)
    target_link_libraries(
        ${PROJECT_NAME}
            mingw32
            ${SDL2_LIBRARIES} 
            SDL2_image
            SDL2_ttf
            glew32
            opengl32
            "-Wl,--subsystem,console"
    )
endif()

if (UNIX AND NOT APPLE)
    target_link_libraries(
        ${PROJECT_NAME}
            ${SDL2_LIBRARIES} 
            ${SDL2_IMAGE_LIBRARIES} 
            ${SDL2_TTF_LIBRARIES} 
            ${GLEW_LIBRARIES}
            OpenGL::OpenGL
    )
endif()


message(STATUS "Copying all assets to destination folder...")
file(
    COPY ${ATOM_SOURCE_DIR}/assets
    DESTINATION ${OUTPUT_DIR}
)

message(STATUS "Copying all dependencies to destination folder...")

if(WIN32)
    file(GLOB PROJECT_DEPENDENCIES ${SDL2_BIN}/*.dll ${SDL2_image_BIN}/*.dll ${SDL2_ttf_BIN}/*.dll ${GLEW_BIN}/*.dll)
endif()

foreach(Dependency IN LISTS PROJECT_DEPENDENCIES)
    file(
        COPY ${Dependency}
        DESTINATION ${OUTPUT_DIR}
    )
endforeach()