find_path(
  enki_INCLUDE_DIR
  enki
)
set(
  enki_INCLUDE_DIR
  ${enki_INCLUDE_DIR}/enki
)
find_library(
  enki_LIBRARY
  enki
)
find_library(
  enkiviewer_LIBRARY
  enkiviewer
)
set(
  enki_LIBRARIES
  ${enki_LIBRARY}
  ${enkiviewer_LIBRARY}
)

set(test_vibe_SOURCES
  ./test_vibe.cpp
  ../../extensions/Component.cpp
  ../../interactions/VibrationSensor.cpp
  ../../interactions/VibrationSource.cpp
  ../../interactions/DecayVibration.cpp
  ../../interactions/QuadraticVibrationSource.cpp
  )

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/extensions
  ${CMAKE_SOURCE_DIR}/interactions
  ${enki_INCLUDE_DIR}
)

add_executable(
  test_vibe
  ${test_vibe_SOURCES}
)

target_link_libraries(
  test_vibe
  ${enki_LIBRARIES}
)





