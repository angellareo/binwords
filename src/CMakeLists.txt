set (INCLUDE_DIR ../include)
include_directories(${INCLUDE_DIR})

add_library (binWordsBuffer SHARED wordsBuffer.c)
set_target_properties(binWordsBuffer PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(binWordsBuffer m)

install(TARGETS binWordsBuffer DESTINATION lib)
install(FILES ${INCLUDE_DIR}/wordsBuffer.h DESTINATION include)
