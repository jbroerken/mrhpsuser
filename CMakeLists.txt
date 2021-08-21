#########################################################################
#
#  CMAKE
#
#########################################################################

###
#  Minimum Version
#  ---------------
#  The CMake version required.
###
cmake_minimum_required(VERSION 3.1)

###
#  CMake Configuration
#  -------------------
#  Configuration settings for CMake.
#
#  NOTE:
#  These settings have to be applied before the project() setting!
###
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 14)

###
#  Project Info
#  ------------
#  General simple information about our project.
###
project(mrhpsuser VERSION 1.0
                  DESCRIPTION "MRH user info platform service executable"
                  LANGUAGES CXX)


#########################################################################
#
#  PATHS
#
#########################################################################

###
#  Binary Paths
#  ------------
#  The paths for our created binary file(s).
###
set(BIN_DIR_PATH "${CMAKE_SOURCE_DIR}/bin/")
file(MAKE_DIRECTORY ${BIN_DIR_PATH})

###
#  Build Paths
#  -----------
#  The paths for the cmake build.
###
set(BUILD_DIR_PATH "${CMAKE_SOURCE_DIR}/build/")
file(MAKE_DIRECTORY ${BUILD_DIR_PATH})

###
#  Source Paths
#  ------------
#  The paths to the source files to use.
#  Add OS specific source files in their own list.
###
set(SRC_DIR_PATH "${CMAKE_SOURCE_DIR}/src/")

set(SRC_LIST_CALLBACK "${SRC_DIR_PATH}/Callback/Service/CBAvail.cpp"
                      "${SRC_DIR_PATH}/Callback/Service/CBAvail.h"
                      "${SRC_DIR_PATH}/Callback/Service/CBReset.cpp"
                      "${SRC_DIR_PATH}/Callback/Service/CBReset.h"
                      "${SRC_DIR_PATH}/Callback/Service/CBCustomCommand.cpp"
                      "${SRC_DIR_PATH}/Callback/Service/CBCustomCommand.h"
                      "${SRC_DIR_PATH}/Callback/Content/CBAccessContent.cpp"
                      "${SRC_DIR_PATH}/Callback/Content/CBAccessContent.h"
                      "${SRC_DIR_PATH}/Callback/Content/CBAccessClear.cpp"
                      "${SRC_DIR_PATH}/Callback/Content/CBAccessClear.h"
                      "${SRC_DIR_PATH}/Callback/Location/CBGetLocation.cpp"
                      "${SRC_DIR_PATH}/Callback/Location/CBGetLocation.h")
   
set(SRC_LIST_CONTENT "${SRC_DIR_PATH}/Content/Content.cpp"
					 "${SRC_DIR_PATH}/Content/Content.h"
					 "${SRC_DIR_PATH}/Content/ContentPaths.h")
                                        
set(SRC_LIST_SERVICE "${SRC_DIR_PATH}/Exception.h"
					 "${SRC_DIR_PATH}/Revision.h"
					 "${SRC_DIR_PATH}/Main.cpp")

###
#  Library Paths
#  -------------
#  The paths to the external libraries to use. 
#  Both the binary and include paths should be set here.
#  Add OS specific libraries in their own lists.
###
set(LIB_DIR_PATH "${CMAKE_SOURCE_DIR}/lib/")
set(INCLUDE_DIR_PATH "${CMAKE_SOURCE_DIR}/include/")

set(LIB_BIN_LIST "${LIB_DIR_PATH}/libmrhev.a"
				 "${LIB_DIR_PATH}/libmrhcevs.a"
				 "${LIB_DIR_PATH}/libmrhbf.a"
				 "${LIB_DIR_PATH}/libmrhpsb.a")
set(LIB_INCLUDE_LIST "${INCLUDE_DIR_PATH}/mrhshared"
					 "${INCLUDE_DIR_PATH}/libmrhev"
					 "${INCLUDE_DIR_PATH}/libmrhcevs"
					 "${INCLUDE_DIR_PATH}/libmrhbf"
					 "${INCLUDE_DIR_PATH}/libmrhpsb")


#########################################################################
#
#  TARGET
#
#########################################################################

###
#  Target
#  ------
#  The target(s) to build.
###
add_executable(mrhpsuser ${SRC_LIST_CALLBACK}
                         ${SRC_LIST_CONTENT}
                         ${SRC_LIST_SERVICE})
set_target_properties(mrhpsuser
    		          PROPERTIES
    		          ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
    		          LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
    		          RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR_PATH})

###
#  Required Libraries
#  ------------------
#  Libraries required by this platform service.
###
target_link_libraries(mrhpsuser PUBLIC ${LIB_BIN_LIST})
include_directories(${LIB_INCLUDE_LIST})

###
#  Source Definitions
#  ------------------
#  Preprocessor source definitions.
###
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_SERVICE_THREAD_COUNT=2)

target_compile_definitions(mrhpsuser PRIVATE MRH_PACKAGE_USER_DIRECTORY_SOURCE="/var/mrhpsuser/_User")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_DOCUMENTS_SOURCE_PATH="/home/owner/Documents")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_PICTURES_SOURCE_PATH="/home/owner/Pictures")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_VIDEOS_SOURCE_PATH="/home/owner/Videos")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_MUSIC_SOURCE_PATH="/home/owner/Music")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_DOWNLOADS_SOURCE_PATH="/home/owner/Downloads")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_CLIPBOARD_SOURCE_PATH="/home/owner/Clipboard/Clipboard.txt")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_INFO_PERSON_SOURCE_PATH="/etc/MRH_User/MRH_UserPerson.conf")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_INFO_RESIDENCE_SOURCE_PATH="/etc/MRH_User/MRH_UserResidence.conf")

target_compile_definitions(mrhpsuser PRIVATE MRH_USER_DOCUMENTS_LINK_PATH="/var/mrhpsuser/_User/Documents")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_PICTURES_LINK_PATH="/var/mrhpsuser/_User/Pictures")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_VIDEOS_LINK_PATH="/var/mrhpsuser/_User/Videos")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_MUSIC_LINK_PATH="/var/mrhpsuser/_User/Music")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_DOWNLOADS_LINK_PATH="/var/mrhpsuser/_User/Downloads")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_CLIPBOARD_LINK_PATH="/var/mrhpsuser/_User/Clipboard.txt")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_INFO_PERSON_LINK_PATH="/var/mrhpsuser/_User/MRH_UserPerson.conf")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_INFO_RESIDENCE_LINK_PATH="/var/mrhpsuser/_User/MRH_UserResidence.conf")