set(enki_INCLUDE_DIR /home/pedro/local/include)
set(enki_LIBRARIES /home/pedro/local/lib/libenki.a)

set(test_light_SOURCES
  ./test_light.cpp
  ../interactions/LightSensor.cpp
  ../interactions/LightSource.cpp
  ../interactions/LightSourceFromAbove.cpp
  )

include_directories( ${enki_INCLUDE_DIR})

add_executable(test_light ${test_light_SOURCES})

target_link_libraries(test_light ${enki_LIBRARIES})

