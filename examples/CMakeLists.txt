
add_executable(SRE-Spinning-Cube spinning-cube.cpp)
add_executable(SRE-Spinning-Sphere spinning-sphere.cpp)
add_executable(SRE-Spinning-Cube-Unlit spinning-cube-unlit.cpp)
add_executable(SRE-Spinning-Sphere-Unlit spinning-sphere-unlit.cpp)
add_executable(SRE-Spinning-Cube-Tex spinning-cube-tex.cpp)
add_executable(SRE-Hello-Engine hello-engine.cpp)
add_executable(SRE-Particle-Test particle-test.cpp)
add_executable(SRE-Particle-Sprite particle-sprite.cpp)
add_executable(SRE-GUI gui.cpp)
add_executable(SRE-Spheres spheres.cpp)

target_link_libraries(SRE-Spinning-Cube ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} SRE)
target_link_libraries(SRE-Spinning-Sphere ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} SRE)
target_link_libraries(SRE-Spinning-Cube-Unlit ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} SRE)
target_link_libraries(SRE-Spinning-Sphere-Unlit ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} SRE)
target_link_libraries(SRE-Spinning-Cube-Tex ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} SRE)
target_link_libraries(SRE-Hello-Engine ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} SRE)
target_link_libraries(SRE-Particle-Test ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} SRE)
target_link_libraries(SRE-Particle-Sprite ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} SRE)
target_link_libraries(SRE-GUI ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} SRE)
target_link_libraries(SRE-Spheres ${EXTRA_LIBS} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} SRE)

file(COPY ../examples-data DESTINATION Debug)
file(COPY ../examples-data DESTINATION Release)

IF (WIN32)
    SET(SDL2_dll "" CACHE STRING "Where the SDL2.dll can be found")
    SET(SDL2_image_lib_path "" CACHE PATH "Where the SDL2_image.dll can be found")
    SET(DLLFileList "${SDL2_image_lib_path}/SDL2_image.dll" "${SDL2_image_lib_path}/libjpeg-9.dll" "${SDL2_image_lib_path}/libpng16-16.dll" "${SDL2_image_lib_path}/libtiff-5.dll" "${SDL2_image_lib_path}/libwebp-4.dll" "${SDL2_image_lib_path}/zlib1.dll"  ${SDL2_dll})
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)
ENDIF(WIN32)