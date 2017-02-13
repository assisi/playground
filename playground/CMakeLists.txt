# Find dependencies

# TODO: Figure out how to use enkiConfig.cmake
#find_package(enki REQUIRED)
find_path(enki_INCLUDE_DIR enki)
set(enki_INCLUDE_DIR ${enki_INCLUDE_DIR}/enki)
find_library(enki_LIBRARY enki)
find_library(enkiviewer_LIBRARY enkiviewer)
set(enki_LIBRARIES ${enkiviewer_LIBRARY} ${enki_LIBRARY})

find_package(Threads)

find_package(ZeroMQ REQUIRED)

find_package(Boost COMPONENTS program_options filesystem system thread timer REQUIRED)

# Set up compilation of protobuffer files
find_package(Protobuf REQUIRED)
set(Proto_FILES ../msg/base_msgs.proto
                ../msg/dev_msgs.proto
                ../msg/sim_msgs.proto)

set(PROTOBUF_IMPORT_DIRS ${PROTOBUF_IMPORT_DIRS}
                         ${CMAKE_SOURCE_DIR}/msg)
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${Proto_FILES})

include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_SOURCE_DIR}/extensions
                     ${CMAKE_BINARY_DIR}/playground
                     ${enki_INCLUDE_DIR}
                     ${ZeroMQ_INCLUDE_DIR})

#link_directories (${CMAKE_BINARY_DIR})

# Demo world "server" (no GUI)
#set(test_ext_SOURCES WorldExt.cpp
#                     ../robots/Casu.cpp
#                     ../robots/Bee.cpp
#                     ../handlers/EPuckHandler.cpp
#                     ../handlers/CasuHandler.cpp
#                     ../handlers/PhysicalObjectHandler.cpp
#                     ../handlers/BeeHandler.cpp
#                     ../interactions/LightConstants.cpp
#                     ../interactions/LightSource.cpp
#                     ../interactions/LightSourceFromAbove.cpp
#                     ../interactions/LightSensor.cpp
#                     ../interactions/WorldHeat.cpp
#                     ../interactions/HeatSensor.cpp
#                     ../interactions/HeatActuatorMesh.cpp
#                     ../interactions/HeatActuatorPointSource.cpp
#					 ../interactions/WaveVibrationSource.cpp
#                     ../interactions/VibrationSource.cpp
#                     ../interactions/VibrationSensor.cpp
#                     ../interactions/ObjectSensor.cpp
#                     ../extensions/Component.cpp
#                     ../extensions/ExtendedRobot.cpp
#                     ../extensions/ExtendedWorld.cpp
#                     ../extensions/PointMesh.cpp
#                     TestWorldExt.cpp
#                     ${ProtoSources})

#add_executable(test_ext ${test_ext_SOURCES})
#target_link_libraries(test_ext ${enki_LIBRARIES} ${ZeroMQ_LIBRARY} ${PROTOBUF_LIBRARY} ${Boost_LIBRARIES})

find_package(Qt4 REQUIRED)
find_package(OpenGL REQUIRED)
set(QT_USE_QTOPENGL TRUE)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

#set(GCC_GRAPHITE_COMPILE_FLAGS, "-floop-parallelize-all -ftree-parallelize-loops=4")

set(GCC_GRAPHITE_COMPILE_FLAGS, "")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_GRAPHITE_COMPILE_FLAGS}" )


# The ASSISI playground
set(playground_SOURCES AssisiPlaygroundMain.cpp
                       AssisiPlayground.cpp
                       WorldExt.cpp
                       ../robots/Casu.cpp
                       ../robots/Bee.cpp
                       ../handlers/EPuckHandler.cpp
                       ../handlers/CasuHandler.cpp
                       ../handlers/PhysicalObjectHandler.cpp
                       ../handlers/BeeHandler.cpp
                       ../interactions/LightConstants.cpp
                       ../interactions/LightSource.cpp
                       ../interactions/LightSourceFromAbove.cpp
                       ../interactions/LightSensor.cpp
                       ../interactions/WorldHeat.cpp
                       ../interactions/HeatSensor.cpp
                       ../interactions/AbstractGrid.cpp
                       ../interactions/VibrationSource.cpp
                       ../interactions/HeatActuatorMesh.cpp
                       ../interactions/HeatActuatorPointSource.cpp
                       ../interactions/WaveVibrationSource.cpp
                       ../interactions/VibrationSensor.cpp
                       ../interactions/ObjectSensor.cpp
                       ../interactions/AirPump.cpp
                       ../interactions/AirFlowSensor.cpp
                       ../extensions/Component.cpp
                       ../extensions/ExtendedRobot.cpp
                       ../extensions/ExtendedWorld.cpp
                       ../extensions/PointMesh.cpp
                       ${ProtoSources})

# For MOC-ing
set(playground_QT_HEADERS AssisiPlayground.h)

qt4_wrap_cpp(playground_QT_HEADERS_MOC ${playground_QT_HEADERS})

qt4_add_resources(playground_RCC_SOURCES playground-textures.qrc)
	
include_directories (${OPENGL_INCLUDE_DIR})    

#link_directories (${enki_BINARY_DIR})   

add_executable(assisi_playground ${playground_SOURCES} ${playground_QT_HEADERS_MOC} ${playground_RCC_SOURCES})

target_link_libraries(assisi_playground ${enki_LIBRARIES}
                                        ${QT_LIBRARIES} 
                                        ${OPENGL_LIBRARIES} 
                                        ${ZeroMQ_LIBRARY}
                                        ${PROTOBUF_LIBRARY}
                                        ${Boost_LIBRARIES}
                                        ${CMAKE_THREAD_LIBS_INIT})

# Copy config files to binary dir
configure_file(Playground.cfg Playground.cfg COPYONLY)
