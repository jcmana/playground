cmake_minimum_required(VERSION 3.1.0)

project("qcombobox")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_executable("qcombobox" WIN32 
	"source/Main.cpp")

#target_include_directories("qcombobox" PRIVATE
#	"../../cpp/reactive/observable_pattern")

target_link_libraries("qcombobox" PRIVATE 
	Qt6::Core 
	Qt6::Widgets 
	Qt6::Gui)