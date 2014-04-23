<<<<<<< HEAD
set(enki_INCLUDE_DIR /usr/local/include)
set(enki_LIBRARIES /usr/local/lib/libenki.a)

set(test_light_SOURCES
  ./test_light.cpp
  ../interactions/LightSensor.cpp
  ../interactions/LightSource.cpp
  ../interactions/LightSourceFromAbove.cpp
  )

include_directories( ${enki_INCLUDE_DIR})

add_executable(test_light ${test_light_SOURCES})

target_link_libraries(test_light ${enki_LIBRARIES})
=======
set(enki_INCLUDE_DIR /usr/local/include /usr/local/include/enki . )
set(enki_LIBRARIES /usr/local/lib/libenki.a)

set(test_heat_SOURCES
./test-heat.cpp
../interactions/WorldHeat.cpp
../interactions/HeatSensor.cpp
../interactions/HeatActuator.cpp
../Component.cpp
../ExtendedRobot.cpp
../ExtendedWorld.cpp
)


set(GLUT_Xmu_LIBRARY /usr/lib/i386-linux-gnu/libglut.so  )

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
#add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
#add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)


find_package(Qt4 REQUIRED)

include_directories(
  ${enki_INCLUDE_DIR}
  )

include_directories(
  ${CMAKE_SOURCE_DIR}/..
  )

#include_directories (${OPENGL_INCLUDE_DIR})    


add_executable(test-heat ${test_heat_SOURCES})

target_link_libraries(test-heat
  ${enki_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${QT_LIBRARIES} 
  ${GLUT_LIBRARY}
  )

>>>>>>> 1e78c01e4fb4f4a35e83f5257962093c2c1a038d

