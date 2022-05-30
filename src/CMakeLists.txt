# Add source files
SET(SOURCE_FILES 
	Main.cpp MainWindow.cpp ShaderProgram.cpp Sphere.cpp Material.cpp BasicMaterial.cpp LitMaterial.cpp Camera.cpp
)
set(HEADER_FILES 
	MainWindow.h ShaderProgram.h Sphere.h Material.h BasicMaterial.h LitMaterial.h Camera.h
)
set(SHADER_FILES 
	basicShader.vert basicShader.frag litShader.vert litShader.frag
)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES})

# Definition (SHADER_FILES)
target_compile_definitions(${PROJECT_NAME} PUBLIC SHADERS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})