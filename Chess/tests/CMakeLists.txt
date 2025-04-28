# tests/CMakeLists.txt
target_sources(TestPriorityQueue PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/TestPriorityQueue.cpp"
)

target_include_directories(TestPriorityQueue PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
)