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
#  CMake Options
#  -------------
#  Options to use when building with CMake.
###
option(USE_SERVER "Enable server based location recieving" ON)

###
#  Project Info
#  ------------
#  General simple information about our project.
###
project(mrhpsuser VERSION 1.0.0
                  DESCRIPTION "MRH user info platform service executable"
                  LANGUAGES CXX)

#########################################################################
#
#  PATHS
#
#########################################################################

###
#  Install Paths
#  -------------
#  The paths for our created binary file(s).
###
set(BIN_INSTALL_PATH "/usr/local/bin/")

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
                      
if(USE_SERVER MATCHES ON)
    set(SRC_LIST_CALLBACK ${SRC_LIST_CALLBACK}
                          "${SRC_DIR_PATH}/Callback/Location/Server.cpp"
                          "${SRC_DIR_PATH}/Callback/Location/Server.h")
endif()
   
set(SRC_LIST_CONTENT "${SRC_DIR_PATH}/Content/Content.cpp"
                     "${SRC_DIR_PATH}/Content/Content.h"
                     "${SRC_DIR_PATH}/Content/ContentPaths.h")
                                        
set(SRC_LIST_SERVICE "${SRC_DIR_PATH}/Configuration.cpp"
                     "${SRC_DIR_PATH}/Configuration.h"
                     "${SRC_DIR_PATH}/Exception.h"
                     "${SRC_DIR_PATH}/Revision.h"
                     "${SRC_DIR_PATH}/Main.cpp")

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

###
#  Required Libraries
#  ------------------
#  Libraries required by this platform service.
###
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

find_package(Threads REQUIRED)
find_library(libmrhbf NAMES mrhbf REQUIRED)
find_library(libmrhev NAMES mrhev REQUIRED)
find_library(libmrhcevs NAMES mrhcevs REQUIRED)
find_library(libmrhpsb NAMES mrhpsb REQUIRED)

if(USE_SERVER MATCHES ON)
    find_library(libmsquic NAMES msquic REQUIRED)
    find_library(libsodium NAMES sodium REQUIRED)
    find_library(libmrhsrv NAMES mrhsrv REQUIRED)
endif()

target_link_libraries(mrhpsuser PUBLIC Threads::Threads)
target_link_libraries(mrhpsuser PUBLIC mrhbf)
target_link_libraries(mrhpsuser PUBLIC mrhev)
target_link_libraries(mrhpsuser PUBLIC mrhcevs)
target_link_libraries(mrhpsuser PUBLIC mrhpsb)

if(USE_SERVER MATCHES ON)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        target_link_libraries(mrhpsuser PUBLIC
                              "-framework CoreFoundation"
                              "-framework Security")
    endif()
    target_link_libraries(mrhpsuser PUBLIC msquic)
    target_link_libraries(mrhpsuser PUBLIC sodium)
    target_link_libraries(mrhpsuser PUBLIC mrhsrv)
endif()

###
#  Source Definitions
#  ------------------
#  Preprocessor source definitions.
###
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_SERVICE_THREAD_COUNT=1)
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_CONFIGURATION_PATH="/usr/local/etc/mrh/mrhpservice/User.conf")

target_compile_definitions(mrhpsuser PRIVATE MRH_PACKAGE_USER_DIRECTORY_SOURCE="/var/mrh/mrhpsuser/_User")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_DOCUMENTS_SOURCE_PATH="/var/mrh/mrhpsuser/Documents")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_PICTURES_SOURCE_PATH="/var/mrh/mrhpsuser/Pictures")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_VIDEOS_SOURCE_PATH="/var/mrh/mrhpsuser/Videos")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_MUSIC_SOURCE_PATH="/var/mrh/mrhpsuser/Music")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_DOWNLOADS_SOURCE_PATH="/var/mrh/mrhpsuser/Downloads")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_CLIPBOARD_SOURCE_PATH="/var/mrh/mrhpsuser/Clipboard/Clipboard.txt")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_INFO_PERSON_SOURCE_PATH="/var/mrh/mrhpsuser/MRH_UserPerson.conf")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_INFO_RESIDENCE_SOURCE_PATH="/var/mrh/mrhpsuser/MRH_UserResidence.conf")

target_compile_definitions(mrhpsuser PRIVATE MRH_USER_DOCUMENTS_LINK_PATH="/var/mrh/mrhpsuser/_User/Documents")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_PICTURES_LINK_PATH="/var/mrh/mrhpsuser/_User/Pictures")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_VIDEOS_LINK_PATH="/var/mrh/mrhpsuser/_User/Videos")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_MUSIC_LINK_PATH="/var/mrh/mrhpsuser/_User/Music")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_DOWNLOADS_LINK_PATH="/var/mrh/mrhpsuser/_User/Downloads")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_CLIPBOARD_LINK_PATH="/var/mrh/mrhpsuser/_User/Clipboard.txt")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_INFO_PERSON_LINK_PATH="/var/mrh/mrhpsuser/_User/MRH_UserPerson.conf")
target_compile_definitions(mrhpsuser PRIVATE MRH_USER_INFO_RESIDENCE_LINK_PATH="/var/mrh/mrhpsuser/_User/MRH_UserResidence.conf")

if(USE_SERVER MATCHES ON)
    target_compile_definitions(mrhpsuser PRIVATE MRH_USER_LOCATION_USE_SERVER=1)
else()
    target_compile_definitions(mrhpsuser PRIVATE MRH_USER_LOCATION_USE_SERVER=0)
endif()

###
#  Install
#  -------
#  Application installation.
###
install(TARGETS mrhpsuser
        DESTINATION ${BIN_INSTALL_PATH})