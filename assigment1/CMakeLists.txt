cmake_minimum_required(VERSION 3.19)
project(assigment1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(assigment1
        Action.h
        assigment1.cpp
        Customer.cpp
        Customer.h
        main.cpp
        Studio.cpp
        Studio.h
        Trainer.cpp
        Trainer.h
        Workout.h
        Action.cpp)
