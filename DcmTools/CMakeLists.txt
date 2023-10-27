cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)

# set Qt moc rcc uic
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

project(DcmTools)

set(CMAKE_BUILD_TYPE  "Release")

########### Manual settings required #############
set(QT_PATH "D:/DevTools/Qt/5.9/msvc2017_64/" CACHE PATH "qt5 cmake dir")
set(CMAKE_PREFIX_PATH ${QT_PATH})
find_package(Qt5 COMPONENTS 
	Widgets 
	Gui 
REQUIRED)

########### Manual settings required #############
set(DCMTK_DIR "D:/DevTools/DCMTK/DCMTK-3.6.5-VS2017-Install/")
set(DCMTK_LIB ${DCMTK_DIR}/x64-Release/lib/*.lib)
set(DCMTK_BIN_DIR ${DCMTK_DIR}/x64-Release/bin)
########### Manual settings required #############
set(VTK_DIR D:/DevTools/VTK/VTK-7.1.1-VS2017-Qt5.9-Install/)
set(VTK_LIB ${VTK_DIR}/lib-Release/*.lib)
set(VTK_BIN_DIR ${VTK_DIR}/bin-Release/)
########### Manual settings required #############
set(GDCM_DIR "D:/DevTools/GDCM/GDCM 3.0/")
set(GDCM_INCLUDE_DIR ${GDCM_DIR}/include)
set(GDCM_LIB ${GDCM_DIR}/lib/*.lib)
set(GDCM_BIN_DIR ${GDCM_DIR}/bin)

link_directories(
	${DCMTK_BIN_DIR}
	${VTK_BIN_DIR}
	${GDCM_BIN_DIR}
)

# include files
INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	# Qt include
	${QT_INCLUDE_DIR}
	# Dcmtk include
	${DCMTK_DIR}/x64-Release/include
	${DCMTK_DIR}/x64-Release/include/dcmtk
	# Vtk include
	${VTK_DIR}/include
	${VTK_DIR}/include/vtk-7.1
	# GDCM include
	${GDCM_DIR}/include
	${GDCM_DIR}/include/gdcm-3.0
)

# Main GUI files
set(MainGUI
	MainGUI/main.cpp
	MainGUI/MainWindow.h
	MainGUI/mainWindow.cpp
	MainGUI/DebugText.hpp
	MainGUI/DebugText.cpp
	MainGUI/DisplayWidget.h
	MainGUI/DisplayWidget.cpp
	MainGUI/IMAGraphicsView.h
	MainGUI/IMAGraphicsView.cpp
	MainGUI/RenderThread.h
	MainGUI/RenderThread.cpp
	MainGUI/InteractionDockWidget.h
	MainGUI/InteractionDockWidget.cpp
	MainGUI/DicomUtility.h
	MainGUI/DicomUtility.cpp
	MainGUI/QSliderDoubleRange.hpp
	MainGUI/QSliderDoubleRange.cpp
	)
# Make the MainGUI group
SOURCE_GROUP("MainGUI" FILES ${MainGUI})

# Core files
set(Core
	# 头文件与Utility
	Core/Fwd.h
	# 数据存储
	Core/FrameBuffer.h
	Core/FrameBuffer.cpp
)
# Make the Core group
SOURCE_GROUP("Core" FILES ${Core})

# Create executable
add_executable(DcmTools
	WIN32
	${MainGUI}
	${Core}
)

FILE(GLOB Icons ${CMAKE_CURRENT_SOURCE_DIR}/Icons/*.png)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${Icons} DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Icons/)

#Copy the DLL to avoid modifying system environment variables
set(QT_BIN 
${QT_PATH}/bin/Qt5Widgets.dll
${QT_PATH}/bin/Qt5Gui.dll)
file(COPY ${QT_BIN} DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/)

#Copy the DLL to avoid modifying system environment variables
FILE(GLOB VTK_BIN ${VTK_BIN_DIR}/*.dll)
FILE(GLOB DCMTK_BIN ${DCMTK_BIN_DIR}/*.dll)
FILE(GLOB GDCM_BIN ${GDCM_BIN_DIR}/*.dll)
file(COPY ${VTK_BIN} DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/)
file(COPY ${DCMTK_BIN} DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/)
file(COPY ${GDCM_BIN} DESTINATION ${EXECUTABLE_OUTPUT_PATH}/Release/)

target_link_libraries(DcmTools 
	Qt5::Widgets
	Qt5::Gui
	${DCMTK_LIB}
	${VTK_LIB}
	${GDCM_LIB}
)
target_link_directories(DcmTools PUBLIC
	${DCMTK_BIN_DIR}
	${VTK_BIN_DIR}
	${GDCM_BIN_DIR}
)

set_target_properties(DcmTools PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
set_target_properties(DcmTools PROPERTIES COMPILE_DEFINITIONS_RELEASE "_CONSOLE")

# 设置 DcmTools 为启动工程
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT DcmTools)




