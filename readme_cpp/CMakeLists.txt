cmake_minimum_required(VERSION 3.22)
project(PyPartMC_CXX LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development)
message(STATUS "Python_EXECUTABLE= ${Python_EXECUTABLE}")

execute_process(
    COMMAND ${Python_EXECUTABLE} -c "from pathlib import Path; import PyPartMC; print(f'{Path(PyPartMC.__file__).parent}')"
    OUTPUT_VARIABLE PYPARTMC_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE res
)
if(res)
    message(FATAL_ERROR "Failed to find PyPartMC directory. Exit code: ${res}")
endif()

message(STATUS "PYPARTMC_DIR= ${PYPARTMC_DIR}")

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

get_filename_component(VENV_PYPARTMC_DIR ${PYPARTMC_LIB} DIRECTORY)

add_executable(my_test test.cpp)

target_include_directories(my_test PUBLIC 
    ${PYPARTMC_DIR}/include
    ${VENV_PYPARTMC_DIR}/include
    ${NANOBIND_INCLUDE_DIR}
    ${Python_INCLUDE_DIRS}
)

add_library(PyPartMC_ext UNKNOWN IMPORTED)
set_target_properties(PyPartMC_ext PROPERTIES IMPORTED_LOCATION "${PYPARTMC_LIB}")

# Link against the imported target, NOT the raw variable
target_link_libraries(my_test PRIVATE 
    PyPartMC_ext
    ${VENV_PYPARTMC_DIR}/lib/libpartmclib.a
    ${Python_LIBRARIES}
)

# Copying .pyd file 
add_custom_command(TARGET my_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${PYPARTMC_LIB}
        $<TARGET_FILE_DIR:my_test>
)

add_test(NAME maketest
        COMMAND my_test)