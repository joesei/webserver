# CMakeList.txt : CMake project for webserver, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

SET(CMAKE_CXX_STANDARD 11)

find_package (Threads REQUIRED)

# Add source to this project's executable.
add_executable (webserver "webserver.cpp" "webserver.h" "main.cpp" "clientsession.cpp" "clientsession.h" "socket.h" "socket.cpp")
if(WIN32)
  target_link_libraries(webserver wsock32 ws2_32)
endif()
#target_link_libraries (webserver PRIVATE Threads::Threads)
# TODO: Add tests and install targets if needed.
