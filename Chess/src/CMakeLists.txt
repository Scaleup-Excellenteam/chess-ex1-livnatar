target_sources(Chess PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Chess.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ChessBoard.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/GameManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ChessPiece.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Rook.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/King.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Bishop.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Queen.cpp"
)
