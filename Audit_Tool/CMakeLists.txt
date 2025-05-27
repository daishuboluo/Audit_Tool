cmake_minimum_required(VERSION 3.16)

project(untitled VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(USE_CUDA OFF)  # 禁用CUDA

# -------------- OpenCV ------------------#
include_directories("C:/Users/Ld/Desktop/Cjiajia/souko/opencv/build/include")
set(OPENCV_LIB_DIR "C:/Users/Ld/Desktop/Cjiajia/souko/opencv/build/x64/vc16/lib")
file(GLOB OPENCV_LIBS "${OPENCV_LIB_DIR}/*.lib")


# -------------- ONNX Runtime ------------------#
include_directories("C:/Users/Ld/Desktop/Cjiajia/souko/onnxruntime_CPU/include")
set(ONNXRUNTIME_CPU_LIB_DIR "C:/Users/Ld/Desktop/Cjiajia/souko/onnxruntime_CPU/lib")  # 设置ONNX Runtime根目录
file(GLOB ONNXRUNTIME_CPU_LIBS "${ONNXRUNTIME_CPU_LIB_DIR}/*.lib")


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)








set(PROJECT_SOURCES
        main.cpp
        widget.cpp
        widget.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(untitled
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        left_column.h left_column.cpp
        right_column.h right_column.cpp
        central_bundle_bridge.h central_bundle_bridge.cpp
        main_page.h main_page.cpp
        settings_page.h settings_page.cpp
        execute_inference.cpp execute_inference.h inference.cpp inference.h
        screenshot.h screenshot.cpp
        mouse_capture.h mouse_capture.cpp
        reminder.h reminder.cpp
        image_display.h image_display.cpp
        global_reminder_box.h global_reminder_box.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET untitled APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(untitled SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(untitled
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(untitled PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(untitled PRIVATE  ${OPENCV_LIBS})
target_link_libraries(untitled PRIVATE  ${ONNXRUNTIME_CPU_LIBS}) #ONNX Runtime库



# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.untitled)
endif()
set_target_properties(untitled PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS untitled
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(untitled)
endif()
