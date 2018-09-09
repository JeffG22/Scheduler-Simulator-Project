cmake_minimum_required(VERSION 3.10)
project(Progetto C)

set(CMAKE_C_STANDARD 11)

ADD_LIBRARY(LibsModule
        listmaker.c
        not_preemptive.c
        preemptive.c
        )

target_link_libraries(LibsModule -lpthread)

include_directories(.)

add_executable(Progetto main.c)

target_link_libraries(Progetto LibsModule)