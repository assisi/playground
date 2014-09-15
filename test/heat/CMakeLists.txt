
find_path(enki_INCLUDE_DIR enki)
find_library(enki_LIBRARIES enki)


include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/../../extensions
                     ${CMAKE_SOURCE_DIR}/../../interactions
                     ${enki_INCLUDE_DIR})



set(test_heat_SOURCES
./test-heat.cpp
../../interactions/WorldHeat.cpp
../../interactions/HeatSensor.cpp
../../interactions/HeatActuatorMesh.cpp
../../interactions/HeatActuatorPointSource.cpp
../../interactions/VibrationSource.cpp
../../extensions/Component.cpp
../../extensions/ExtendedRobot.cpp
../../extensions/ExtendedWorld.cpp
)

# find_library(GLUT_Xmu_LIBRARY libglut)

find_package(GLUT REQUIRED)

# include_directories(${GLUT_INCLUDE_DIRS})
# link_directories(${GLUT_LIBRARY_DIRS})
# #add_definitions(${GLUT_DEFINITIONS})
# if(NOT GLUT_FOUND)
#     message(ERROR " GLUT not found!")
# endif(NOT GLUT_FOUND)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)


add_executable(test-heat ${test_heat_SOURCES})

target_link_libraries(test-heat
  ${enki_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${QT_LIBRARIES} 
  ${GLUT_LIBRARY}
  )

# Copy config files to binary dir
configure_file(config-heat.txt config-heat.txt COPYONLY)



