cmake_minimum_required(VERSION 3.1.0)

project("observable_widgets")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(
	Qt5 
	COMPONENTS 
		Core 
		Gui 
		Widgets 
	REQUIRED)

link_directories(
	"../../cpp/concurrency/x64/Debug")

add_executable("observable_widgets" 
	"source/Main.cpp")

target_include_directories("observable_widgets" PRIVATE
	"../../cpp/link/link"
	"../../cpp/callback_store/atomic_callback_store"
	"../../cpp/concurrency/concurrency"
	"../../cpp/reactive/observable_pattern")

target_link_libraries("observable_widgets" PRIVATE 
	concurrency
	Qt5::Core 
	Qt5::Widgets 
	Qt5::Gui)