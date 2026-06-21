cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

project("minc" C)

add_executable(minc  "min_runtime.c" "min_interpreter.h" "string.h" "string.c" "vector.h" "vector.c" "stack.c" "stack.h" "precision_delay.h" "file_reader.h" "file_reader.c" "min_interpreter.c" "precision_delay.c" "min_tokenizer.h" "min_token.h" "min_interpreter_v2.h" "min_tokenizer.c" "min_parser.h" "min_parser.c")

add_custom_target(tests
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/tests
	${CMAKE_CURRENT_BINARY_DIR}/tests
)

add_dependencies(minc tests)
