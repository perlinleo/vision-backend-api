cmake_minimum_required(VERSION 3.7.2)
project(vision)

set(Boost_USE_MULTITHREAD ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -pedantic -fprofile-arcs -ftest-coverage -L/usr/local/Cellar/libpqxx/7.5.2 -L/usr/local/Cellar/libpq/13.3 -pthread")

# Building lib
set(VISION_LIB_PATH ${PROJECT_SOURCE_DIR}/lib/vision-go)


# <Test>
configure_file(CMakeLists.txt.in
        googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )

add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
      ${CMAKE_BINARY_DIR}/googletest-build)

enable_testing()
add_subdirectory(${VISION_LIB_PATH}/test)



# </Test>
set(OpenCV_DIR scripts/installation/OpenCV-master/lib/cmake/opencv4)
find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_search_module(ZBAR REQUIRED zbar)

find_package(Boost REQUIRED)
include_directories(${Boost_LIBRARIES})
include_directories(/usr/local/Cellar/boost/1.75.0_3/include)
find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)
include_directories(/usr/local/Cellar/libpqxx/7.5.2/include)


file(GLOB sources "${VISION_LIB_PATH}/*/source/*.cpp")
file(GLOB headers "${VISION_LIB_PATH}/*/include/*.hpp")


include_directories("lib/vision-go/database/include")
include_directories("lib/vision-go/clientHandler/include")
include_directories("lib/vision-go/httpClient/include")
include_directories("lib/vision-go/httpServer/include")
include_directories("lib/vision-go/logger/include")
include_directories("lib/vision-go/terminalClient/include")
include_directories("lib/vision-go/terminalHandler/include")
include_directories("lib/vision-go/terminalServer/include")
include_directories("lib/vision-go/timecode/include")

add_library(lib/vision-go SHARED ${sources} ${headers})

link_libraries(lib/vision-go ${Boost_LIBRARIES} -lboost_thread ${PQXX_LIB} ${PQ_LIB} -lboost_system)
# Building lib




### DEBUG BUILD
set(CMAKE_BUILD_TYPE DEBUG)
###


### <-->

#TerminalServer
add_executable(terminalServer ${PROJECT_SOURCE_DIR}/terminalServer/main.cpp)
target_link_libraries(terminalServer lib/vision-go ${Boost_LIBRARIES} -lboost_thread ${PQXX_LIB} ${PQ_LIB})
target_include_directories(terminalServer PUBLIC "${VISION_LIB_PATH}/include")

#UserServer
add_executable(userServer ${PROJECT_SOURCE_DIR}/userServer/main.cpp)
target_link_libraries(userServer lib/vision-go ${Boost_LIBRARIES} -lboost_thread ${PQXX_LIB} ${PQ_LIB})
target_include_directories(userServer PUBLIC "${VISION_LIB_PATH}/include")

### <-->