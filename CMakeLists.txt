project(wxtest)
cmake_minimum_required(VERSION 3.22)

link_libraries(-L/usr/local/lib -pthread   -lwx_gtk3u_xrc-3.2 -lwx_gtk3u_html-3.2 -lwx_gtk3u_qa-3.2 -lwx_gtk3u_core-3.2 -lwx_baseu_xml-3.2 -lwx_baseu_net-3.2 -lwx_baseu-3.2 )

add_compile_options(-I/usr/local/lib/wx/include/gtk3-unicode-3.2 -I/usr/local/include/wx-3.2 -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXGTK__ -pthread)
set(CXX_STANDARD 23)
add_executable(wxtest src/main.cpp src/libcplot.cpp src/toojpeg.cpp)