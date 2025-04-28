# tests/CMakeLists.txt
target_sources(TestPriorityQueue PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/TestPriorityQueue.cpp"
)

target_include_directories(TestPriorityQueue PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
)


target_sources(TestMove PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/TestMove.cpp"
    "${CMAKE_SOURCE_DIR}/src/move/Move.cpp" 
)
target_include_directories(TestMove PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
)