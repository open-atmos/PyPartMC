cmake_minimum_required(VERSION 3.22)
project(PyPartMC_CXX LANGUAGES CXX)

find_package(Python 3.8 REQUIRED COMPONENTS Interpreter)
message(STATUS "Python_EXECUTABLE= ${Python_EXECUTABLE}")

execute_process(
    COMMAND ${Python_EXECUTABLE} -c "from pathlib import Path; import PyPartMC; print(f'{Path(PyPartMC.__file__).parent}/include')"
    OUTPUT_VARIABLE PYPARTMC_INCLUDE_DIRS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "PYPARTMC_INCLUDE_DIRS= ${PYPARTMC_INCLUDE_DIRS}")

execute_process(
    COMMAND ${Python_EXECUTABLE} -c "from PyPartMC import _PyPartMC; print(f'{_PyPartMC.__file__}')"
    OUTPUT_VARIABLE PYPARTMC_LIB
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE res
)
if(res)
    message(FATAL_ERROR "Command failed with exit code ${res}")
endif()

message(STATUS "PYPARTMC_LIB= ${PYPARTMC_LIB}")


add_executable(my_test test.cpp)
target_include_directories(my_test PUBLIC 
    ${PYPARTMC_INCLUDE_DIRS}
    # ${Python_INCLUDE_DIRS}
    RESULT_VARIABLE res
)
if(res)
    message(FATAL_ERROR "Command failed with exit code ${res}")
endif()

target_link_libraries(my_test PRIVATE PYPARTMC_LIB)