# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.10)

# 项目信息
project (testthrustallocation)
set(CMAKE_CXX_STANDARD 17)


# UNIX, WIN32, WINRT, CYGWIN, APPLE are environment 
# variables as flags set by default system
if(UNIX)
    message("This is a ${CMAKE_SYSTEM_NAME} system")
elseif(WIN32)
    message("This is a Windows System")
endif()

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -Wextra -g -ggdb -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


set(CMAKE_INCLUDE_CURRENT_DIR ON)


# thread库
find_package(Threads MODULE REQUIRED)


# 添加 include 子目录


set(HEADER_DIRECTORY ${HEADER_DIRECTORY} 
	"${PROJECT_SOURCE_DIR}/../../../"
	"/opt/mosek/9.0/tools/platform/linux64x86/h")

set(LIBRARY_DIRECTORY ${LIBRARY_DIRECTORY} 
	"/usr/lib"
    "/opt/mosek/9.0/tools/platform/linux64x86/bin")

set(SOURCE_FILES ${SOURCE_FILES} 
	"${PROJECT_SOURCE_DIR}/../../../common/logging/src/easylogging++.cc" )


# 指定生成目标

add_executable (testmosek qo1.cc)
target_include_directories(testmosek PRIVATE ${HEADER_DIRECTORY})
find_library(MOSEK_LIBRARY mosek64 HINTS ${LIBRARY_DIRECTORY})
target_link_libraries(testmosek PUBLIC ${MOSEK_LIBRARY})



set(RARE_LIBRARIES ${RARE_LIBRARIES} 
	"boost_system"
	"boost_filesystem"
	"boost_iostreams"
	"util"
	)
add_executable (testthrust testthrust.cc ${SOURCE_FILES})
target_include_directories(testthrust PRIVATE ${HEADER_DIRECTORY})
find_library(MOSEK_LIBRARY mosek64 HINTS ${LIBRARY_DIRECTORY})
target_link_libraries(testthrust PUBLIC ${MOSEK_LIBRARY})
target_link_libraries(testthrust PUBLIC ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(testthrust PUBLIC ${RARE_LIBRARIES})


add_executable (testcontroller testcontroller.cc  ${SOURCE_FILES})
target_include_directories(testcontroller PRIVATE ${HEADER_DIRECTORY})
target_link_libraries(testcontroller PUBLIC ${MOSEK_LIBRARY})
target_link_libraries(testcontroller PUBLIC ${CMAKE_THREAD_LIBS_INIT})


# add_executable (testmotor testcontrolandmotor.cc ${SOURCE_FILES})
# target_include_directories(testmotor PRIVATE ${HEADER_DIRECTORY})
# target_link_libraries(testmotor PUBLIC ${MOSEK_LIBRARY})
# target_link_libraries(testmotor PUBLIC ${CMAKE_THREAD_LIBS_INIT})


add_executable (testlos testlos.cc ${SOURCE_FILES} )
target_include_directories(testlos PRIVATE ${HEADER_DIRECTORY})


#add_executable (testthrust_osqp testthrust_osqp.cc ${SOURCE_FILES})
#target_include_directories(testthrust_osqp PRIVATE ${HEADER_DIRECTORY})
#find_package(osqp REQUIRED)
#target_link_libraries(testthrust_osqp PUBLIC osqp::osqp)
#target_link_libraries(testthrust_osqp PUBLIC ${RARE_LIBRARIES})
