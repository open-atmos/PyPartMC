cmake_minimum_required(VERSION 3.22)
project(AeroSimulation LANGUAGES CXX)

enable_testing()
set(CMAKE_CXX_STANDARD 17)

find_package(Python 3.8 REQUIRED COMPONENTS Interpreter)
execute_process(
    COMMAND ${Python_EXECUTABLE} -c "import PyPartMC, pathlib; print(pathlib.Path(PyPartMC.cmake_dir()).as_posix())"
    OUTPUT_VARIABLE PYPARTMC_CMAKE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(PyPartMC_DIR "${PYPARTMC_CMAKE_DIR}")

find_package(PyPartMC REQUIRED)

add_executable(my_test test.cpp)
target_link_libraries(my_test PRIVATE PyPartMC::PyPartMC)
pypartmc_setup_runtime(my_test)

add_test(NAME maketest COMMAND my_test)