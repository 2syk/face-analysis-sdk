cmake_policy(SET CMP0057 NEW)
find_package(PythonLibs 3.6 EXACT REQUIRED)
find_package(PythonInterp 3.6 EXACT REQUIRED)

find_package(Python3 3.6 EXACT REQUIRED COMPONENTS Interpreter Development NumPy)
find_package(pybind11 REQUIRED)

pybind11_add_module(face_analysis_sdk
  MODULE
  face_analysis_binding.cpp
  ndarray_converter.cpp)

target_include_directories(face_analysis_sdk PRIVATE ${Python3_NumPy_INCLUDE_DIRS})
target_link_libraries(face_analysis_sdk PRIVATE clmTracker utilities ${LIBS})
