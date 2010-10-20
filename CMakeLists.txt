cmake_minimum_required(VERSION 2.6)
project(chopsticks)

find_package(Qt4 REQUIRED)
set( QT_USE_QTGUI TRUE )
set( QT_USE_QTNETWORK TRUE )
set( QT_USE_QTDBUS TRUE )

include(${QT_USE_FILE} )

set (CMAKE_CXX_FLAGS -std=c++0x)

#
# Some include dirs we need(in order):
# 1. Qt include files
# 2. Generated headers i.e. ui to hdr
# 3. Our own include files
#
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#
# Qt can perform up to 3 code generation steps:
# 1. Generate source files from qrc resource files
# 2. generate moc sources from QObject class headers
#

set(MOC_SOURCES
	bartwidget.cpp
	clockwidget.cpp
	i3pagerwidget.cpp
	notifywidget.cpp
	commandwidget.cpp
	batterywidget.cpp
)

set(SOURCES
	main.cpp
)

set(UIS
	simplewidget.ui
)

qt4_wrap_ui(UIS_HEADERS ${UIS})
qt4_automoc(${MOC_SOURCES})

add_executable(bart ${SOURCES} ${MOC_SOURCES} ${UIS_HEADERS})
target_link_libraries(bart ${QT_LIBRARIES} qjson)


