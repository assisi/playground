<<<<<<< HEAD
set(enki_INCLUDE_DIR /usr/local/include)
set(enki_LIBRARIES /usr/local/lib/libenki.a)

#set(enki_INCLUDE_DIR /home/pedro/local/include)
#set(enki_LIBRARIES /home/pedro/local/lib/libenki.a)
=======

find_path(enki_INCLUDE_DIR enki)
find_library(enki_LIBRARIES enki)
>>>>>>> 855e5837aa0090a4feedbf24bf2de3bcd1982655

set(test_light_SOURCES
  ./test_light.cpp
  ../interactions/LightSensor.cpp
  ../interactions/LightSource.cpp
  ../interactions/LightSourceFromAbove.cpp
<<<<<<< HEAD
  ../Component.cpp
  )

set(test_vibe_SOURCES
  ./test_vibe.cpp
  ../interactions/LightSensor.cpp
  ../interactions/LightSource.cpp
  ../interactions/LightSourceFromAbove.cpp
  ../interactions/VibrationSensor.cpp
  ../interactions//VibrationSource.cpp
  ../interactions/QuadraticVibrationSource.cpp
  ../Component.cpp
  ../robots/Bee.cpp
  )

include_directories( ${enki_INCLUDE_DIR})

add_executable(test_light ${test_light_SOURCES})

add_executable(test_vibe ${test_vibe_SOURCES})

target_link_libraries(test_light ${enki_LIBRARIES})

target_link_libraries(test_vibe ${enki_LIBRARIES})
=======
  )

include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/extensions
                     ${CMAKE_SOURCE_DIR}/interactions
                     ${enki_INCLUDE_DIR})

add_executable(test_light ${test_light_SOURCES})

target_link_libraries(test_light ${enki_LIBRARIES})

set(test_heat_SOURCES
./test-heat.cpp
../interactions/WorldHeat.cpp
../interactions/HeatSensor.cpp
../interactions/HeatActuator.cpp
../extensions/Component.cpp
../extensions/ExtendedRobot.cpp
../extensions/ExtendedWorld.cpp
)

#find_library(GLUT_Xmu_LIBRARY libglut)

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

add_executable(test-heat ${test_heat_SOURCES})

target_link_libraries(test-heat
  ${enki_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${QT_LIBRARIES} 
  ${GLUT_LIBRARY}
  )

# Copy config files to binary dir
configure_file(config-heat.txt config-heat.txt COPYONLY)

>>>>>>> 855e5837aa0090a4feedbf24bf2de3bcd1982655


