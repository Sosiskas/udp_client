cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(udp_client LANGUAGES C CXX)

set (CMAKE_PREFIX_PATH "D:\\Qt\\5.15.0\\msvc2015_64\\")

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()
if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
	if (UNIX)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	else()
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
	endif ()
endif()
if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif ()
mark_as_advanced(
	CMAKE_RUNTIME_OUTPUT_DIRECTORY
	CMAKE_LIBRARY_OUTPUT_DIRECTORY
	CMAKE_ARCHIVE_OUTPUT_DIRECTORY
)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to 'Debug' as none was specified")
	set(CMAKE_BUILD_TYPE Debug CACHE STRING  "Choose the type of build" FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if(NOT CMAKE_C_STANDARD)
	set(CMAKE_C_STANDARD 11)
endif()
if(NOT CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 14)
endif()

set(install_dir ${CMAKE_BINARY_DIR}/install)
set(download_dir ${CMAKE_BINARY_DIR}/download)
if (UNIX)
	set(CMAKE_PREFIX_PATH "${install_dir}" "/tmp" ${CMAKE_PREFIX_PATH})
else (WIN32)
	set(CMAKE_PREFIX_PATH "${install_dir}" "c:/temp" ${CMAKE_PREFIX_PATH})
endif ()

#include(findPathQtKit)
#if(QT_KIT_PATH)
#	set(CMAKE_PREFIX_PATH ${QT_KIT_PATH} ${CMAKE_PREFIX_PATH})
#endif()

#set(QT_MODULES Widgets)
find_package(Qt5 COMPONENTS Core Network REQUIRED)
find_package(Qt5Widgets)

qt5_wrap_ui(UI_FILES
	src/MainWindow.ui
)

string(TOUPPER "${LIBNAME}_EXPORTS" LIBNAME_EXPORTS)
string(TOUPPER "${LIBNAME}_STATIC" LIBNAME_STATIC)
string(TOUPPER "${LIBNAME}_DLL" LIBNAME_DLL)
string(TOUPPER "${LIBNAME}_NO_AUTOMATIC_LIBS" LIBNAME_NO_AUTOMATIC_LIBS)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/ClientView.h
	src/ClientView.cpp
	src/Messages.h
	src/Messages.cpp
	src/MessageOutputter.h
	src/MessageBoxOutputter.h
	src/MessageBoxOutputter.cpp
	src/SocketWorker.h
	src/SocketWorker.cpp

	${UI_FILES}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
	POSITION_INDEPENDENT_CODE ON
	VISIBILITY_INLINES_HIDDEN ON
	CXX_VISIBILITY_PRESET hidden
	
)
target_compile_definitions( ${PROJECT_NAME}
	PUBLIC $<$<CONFIG:Debug>:_DEBUG>
	#PUBLIC ${LIBNAME_STATIC}
)
target_link_libraries(${PROJECT_NAME}
	PUBLIC Qt5::Widgets
	PUBLIC Qt5::Network
)

if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME}
				POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different 
				$<TARGET_FILE:Qt5::Widgets>
				$<TARGET_FILE:Qt5::Gui>
				$<TARGET_FILE:Qt5::Core>
				$<TARGET_FILE:Qt5::Network>
				$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()

message(STATUS "CMake ${CMAKE_VERSION} successfully configured ${PROJECT_NAME} using ${CMAKE_GENERATOR} generator")
message(STATUS "CMAKE_CXX_LIBRARY_ARCHITECTURE - ${CMAKE_CXX_LIBRARY_ARCHITECTURE}")
message(STATUS "Installation target path: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "C_FLAGS:  =${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_C_FLAGS_DEBUG:=${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "CMAKE_C_FLAGS_RELEASE:=${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CMAKE_C_FLAGS_MINSIZEREL:=${CMAKE_C_FLAGS_MINSIZEREL}")
message(STATUS "CMAKE_C_FLAGS_RELWITHDEBINFO:=${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message(STATUS "")
message(STATUS "")
message(STATUS "CXX_FLAGS:=${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG:=${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE:=${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL:=${CMAKE_CXX_FLAGS_MINSIZEREL}")
message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO:=${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "")
message(STATUS "Installation package path: ${CPACK_PACKAGING_INSTALL_PREFIX}")
message(STATUS "CPACK_PACKAGE_VERSION:=${CPACK_PACKAGE_VERSION}")
message(STATUS "CPACK_PACKAGE_VERSION_MAJOR:=${CPACK_PACKAGE_VERSION_MAJOR}")
message(STATUS "CPACK_PACKAGE_VERSION_MINOR:=${CPACK_PACKAGE_VERSION_MINOR}")
message(STATUS "CPACK_PACKAGE_VERSION_PATCH:=${CPACK_PACKAGE_VERSION_PATCH}")
message(STATUS "")
message(STATUS "CMAKE_INSTALL_DEFAULT_COMPONENT_NAME:=${CMAKE_INSTALL_DEFAULT_COMPONENT_NAME}")