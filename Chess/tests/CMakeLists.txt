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


# Collect all .cpp files in src/
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Remove main.cpp explicitly (adjust path if needed)
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

# Now define the test target safely
target_sources(TestMoveRecommender PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/TestMoveRecommender.cpp"
    ${SOURCES}
)

target_include_directories(TestMoveRecommender PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
)