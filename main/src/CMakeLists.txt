include_directories(${wxtest_SOURCE_DIR}/main/include)
set(EXECUTABLE_OUTPUT_PATH ${wxtest_SOURCE_DIR}/bin)
add_executable(wxtest main.cpp)