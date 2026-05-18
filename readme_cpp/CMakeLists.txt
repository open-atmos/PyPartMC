cmake_minimum_required(VERSION 3.22)
project(PyPartMC_CXX LANGUAGES CXX)

find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development)
message(STATUS "Python_EXECUTABLE= ${Python_EXECUTABLE}")

execute_process(
    COMMAND ${Python_EXECUTABLE} -c "from pathlib import Path; import PyPartMC; print(f'{Path(PyPartMC.__file__).parent}/include')"
    OUTPUT_VARIABLE PYPARTMC_INCLUDE_DIRS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE res_inc
)
if(res_inc)
    message(FATAL_ERROR "Failed to find PyPartMC include directory. Exit code: ${res_inc}")
endif()

message(STATUS "PYPARTMC_INCLUDE_DIRS= ${PYPARTMC_INCLUDE_DIRS}")

execute_process(
    COMMAND ${Python_EXECUTABLE} -c "import nanobind; print(nanobind.include_dir())"
    OUTPUT_VARIABLE NANOBIND_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE res_nano
)
if(res_nano)
    message(FATAL_ERROR "Failed to find nanobind include directory. Exit code: ${res_nano}")
endif()
message(STATUS "NANOBIND_INCLUDE_DIR= ${NANOBIND_INCLUDE_DIR}")

# 2. Get the Library File
execute_process(
    COMMAND ${Python_EXECUTABLE} -c "from PyPartMC import _PyPartMC; print(f'{_PyPartMC.__file__}')"
    OUTPUT_VARIABLE PYPARTMC_LIB
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE res_lib
)
if(res_lib)
    message(FATAL_ERROR "Failed to find PyPartMC library. Exit code: ${res_lib}")
endif()

message(STATUS "PYPARTMC_LIB= ${PYPARTMC_LIB}")

add_executable(my_test test.cpp)

target_include_directories(my_test PUBLIC 
    ${PYPARTMC_INCLUDE_DIRS}
    ${NANOBIND_INCLUDE_DIR}
    ${Python_INCLUDE_DIRS}
)

target_link_libraries(my_test PRIVATE ${PYPARTMC_LIB})

add_custom_command(TARGET my_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PYPARTMC_LIB}
        $<TARGET_FILE_DIR:my_test>
)