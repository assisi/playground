# Find dependencies

# TODO: Make a FindEnki.cmake module
set(enki_INCLUDE_DIR /usr/local/include/enki)
set(enki_LIBRARIES /usr/local/lib/libenki.a
                   /usr/local/lib/libenkiviewer.a)

find_package(ZeroMQ REQUIRED)

# boost::for_each is used within WorldExt
find_package(Boost REQUIRED)

# Set up compilation of protobuffer files
find_package(Protobuf REQUIRED)
set(Proto_FILES ../msg/base_msgs.proto
                ../msg/dev_msgs.proto
                ../msg/sim_msgs.proto)

set(PROTOBUF_IMPORT_DIRS ${PROTOBUF_IMPORT_DIRS}
                         ${CMAKE_SOURCE_DIR}/msg)
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${Proto_FILES})

include_directories (${CMAKE_SOURCE_DIR}
                     ${CMAKE_BINARY_DIR}/playground
                     ${enki_INCLUDE_DIR}
                     ${ZeroMQ_INCLUDE_DIR})

#link_directories (${CMAKE_BINARY_DIR})

# Demo world "server" (no GUI)
set(test_ext_SOURCES WorldExt.cpp
                     ../handlers/EPuckHandler.cpp
                     TestWorldExt.cpp
                     ${ProtoSources})

add_executable(test_ext ${test_ext_SOURCES})
target_link_libraries(test_ext ${enki_LIBRARIES} ${ZeroMQ_LIBRARY} ${PROTOBUF_LIBRARY})

# Demo EPuck wander behavior ("client")
set(wander_demo_SOURCES wander_demo.cpp
                        ${ProtoSources})

add_executable(wander_demo ${wander_demo_SOURCES})
target_link_libraries(wander_demo ${ZeroMQ_LIBRARY} ${PROTOBUF_LIBRARY})

find_package(Qt4 REQUIRED)
find_package(OpenGL REQUIRED)
set(QT_USE_QTOPENGL TRUE)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# The ASSISI playground
set(playground_SOURCES AssisiPlaygroundMain.cpp
                       AssisiPlayground.cpp
                       WorldExt.cpp
                       ../robots/Casu.cpp
                       ../handlers/EPuckHandler.cpp
                       ${ProtoSources})

# For MOC-ing
set(playground_QT_HEADERS AssisiPlayground.h)

qt4_wrap_cpp(playground_QT_HEADERS_MOC ${playground_QT_HEADERS})
	
include_directories (${OPENGL_INCLUDE_DIR})    

#link_directories (${enki_BINARY_DIR})   

add_executable(assisi_playground ${playground_SOURCES} ${playground_QT_HEADERS_MOC})

target_link_libraries(assisi_playground ${enki_LIBRARIES}
                                        ${QT_LIBRARIES} 
                                        ${OPENGL_LIBRARIES} 
                                        ${ZeroMQ_LIBRARY}
                                        ${PROTOBUF_LIBRARY})

