cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 23)

project("minc" C)

add_executable(minc  "min_runtime.c" "min_interpreter.h" "string.h" "string.c" "vector.h" "vector.c" "stack.c" "stack.h" "precision_delay.h")

add_custom_target(tests
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_CURRENT_LIST_DIR}/tests
	${CMAKE_CURRENT_BINARY_DIR}/tests
)

add_dependencies(minc tests)
