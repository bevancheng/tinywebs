# for each "src/x.cpp", generate target "x"
file(GLOB_RECURSE all_srcs *.cpp)

add_executable(server ${all_srcs})

link_directories(/usr/lib/x86_64-linux-gnu/)
target_link_libraries(server libmysqlclient.so)
target_link_libraries(server pthread)