cmake_minimum_required(VERSION 3.1)
project(example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11")# 设置包含的目录; CMAKE_CXX_FLAGS：设置 C++ 编译选项，也可以通过指令 add_definitions() 添加

# SET(ENABLE_SSL OFF)
# SET(ENABLE_JAVA OFF)

# add_definitions(-DMSGPACK_NO_BOOST)
# if (ENABLE_SSL)
    # add_definitions(-DCINATRA_ENABLE_SSL)
    # message(STATUS "Use SSL")
    # find_package(Boost COMPONENTS system filesystem REQUIRED)
# endif()

# if (ENABLE_JAVA)
    # find_package(JNI REQUIRED)
    # message(STATUS "Use Java")
# endif()

# include_directories(# 设置包含的目录
        # "/usr/local/include"
        # "../include"
        # "../thirdparty/msgpack-c/include"
        # "../thirdparty/asio")

add_executable(pub subscribe.cpp)# 生成可执行文件
# add_executable(basic_client client/main.cpp)
target_link_libraries(pub -lhiredis -levent)
# if (ENABLE_SSL)
    # target_link_libraries(basic_server ${Boost_LIBRARIES} -lssl -lcrypto -lpthread)# 设置 target 需要链接的库
    # target_link_libraries(basic_client ${Boost_LIBRARIES} -lssl -lcrypto -lpthread)
# else()
    # target_link_libraries(basic_server ${Boost_LIBRARIES})
    # target_link_libraries(basic_client ${Boost_LIBRARIES})
# endif()