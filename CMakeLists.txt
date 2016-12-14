CMAKE_MINIMUM_REQUIRED (VERSION 3.5)
Project (Loader)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
##############get all files in the folder##############3
file (GLOB SOURCE_FILES "*.cpp" "*.h" "*.frag" "*.vs")
add_executable(Loader ${SOURCE_FILES} Camera.cpp GraphicsObject.cpp Mesh.cpp Model.cpp Sameh.cpp Shader.cpp)
set_target_properties(Loader PROPERTIES LINKER_LANGUAGE CXX)
######link ALl LIbs###################
target_link_libraries(Loader -lGL -lglfw -lGLEW -lSOIL -lassimp )
########link Bullet for physics###################
find_package( Bullet REQUIRED )
target_include_directories(Loader PUBLIC ${BULLET_INCLUDE_DIR} )
target_LINK_LIBRARIES(Loader  BulletDynamics BulletCollision LinearMath)
########Not beautiful but works####################
##########works only on a linux where all libs are installed########
#########speeds up the build##########################
