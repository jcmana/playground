cmake_minimum_required(VERSION 3.1.0)

project("mvc_observables")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Qt5_DIR $ENV{QTDIR-G4}/lib/cmake/Qt5)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable("mvc_observables" 
	"source/LineEdit.h"
	"source/Main.cpp")

target_include_directories("mvc_observables" PRIVATE
	"../../cpp/reactive/observable_pattern")

target_link_libraries("mvc_observables" PRIVATE 
	Qt5::Core 
	Qt5::Widgets 
	Qt5::Gui)