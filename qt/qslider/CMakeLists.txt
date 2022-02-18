cmake_minimum_required(VERSION 3.1.0)

project("qslider")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Qt5_DIR $ENV{QTDIR-G4}/lib/cmake/Qt5)
find_package(Qt5 COMPONENTS 
	Core 
	Gui 
	Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable("qslider" WIN32 "Main.cpp")

target_link_libraries("qslider" PRIVATE 
	Qt5::Core 
	Qt5::Widgets 
	Qt5::Gui)