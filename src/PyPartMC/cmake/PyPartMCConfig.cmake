if(TARGET PyPartMC::PyPartMC)
    return()
endif()

find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development)

set(PyPartMC_RUNTIME_LIBRARY "${PYPARTMC_LIB_FILE}")

execute_process(
    COMMAND ${Python_EXECUTABLE} -c "import nanobind, PyPartMC, os; from PyPartMC import _PyPartMC; print(f'{os.path.dirname(PyPartMC.__file__)};{nanobind.include_dir()};{_PyPartMC.__file__}')"
    OUTPUT_VARIABLE PY_PATHS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE res
)

if(res)
    message(FATAL_ERROR "filed to find PyPartMC libraries")
endif()

list(GET PY_PATHS 0 PYPARTMC_DIR)
list(GET PY_PATHS 1 NANOBIND_INCLUDE_DIR)
list(GET PY_PATHS 2 PYPARTMC_LIB_FILE)

get_filename_component(VENV_PYPARTMC_DIR "${PYPARTMC_LIB_FILE}" DIRECTORY)

add_library(PyPartMC::PyPartMC INTERFACE IMPORTED)

add_library(PyPartMC::RuntimeCore UNKNOWN IMPORTED)
set_target_properties(PyPartMC::RuntimeCore PROPERTIES IMPORTED_LOCATION "${PYPARTMC_LIB_FILE}")

set(POTENTIAL_INCLUDE_DIRS 
    "${PYPARTMC_DIR}/include"
    "${VENV_PYPARTMC_DIR}/include"
    "${NANOBIND_INCLUDE_DIR}"
)

set(ACTUAL_INCLUDE_DIRS "")
foreach(DIR IN LISTS POTENTIAL_INCLUDE_DIRS)
    if(EXISTS "${DIR}")
        list(APPEND ACTUAL_INCLUDE_DIRS "${DIR}")
    endif()
endforeach()

target_include_directories(PyPartMC::PyPartMC INTERFACE 
    ${ACTUAL_INCLUDE_DIRS}
    ${Python_INCLUDE_DIRS}
)

target_link_libraries(PyPartMC::PyPartMC INTERFACE 
    "${VENV_PYPARTMC_DIR}/lib/libpartmclib.a"
    PyPartMC::RuntimeCore
    ${Python_LIBRARIES}
)

function(pypartmc_setup_runtime TARGET_NAME)
    if(WIN32)
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PYPARTMC_LIB_FILE}"
                $<TARGET_FILE_DIR:${TARGET_NAME}>
        )
    endif()
endfunction()

message(STATUS "Found PyPartMC: ${PYPARTMC_DIR}")
