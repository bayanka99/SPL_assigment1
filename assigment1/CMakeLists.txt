cmake_minimum_required(VERSION 3.19)
project(assigment1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(assigment1
        include/Action.h
        src/Customer.cpp
        include/Customer.h
        src/main.cpp
        src/Studio.cpp
        include/Studio.h
        src/Trainer.cpp
        include/Trainer.h
        include/Workout.h
        src/Workout.cpp
        src/Action.cpp)
