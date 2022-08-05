cmake_minimum_required(VERSION 3.23)
project(calculateLoRa)

set(CMAKE_CXX_STANDARD 14)

add_executable(calculateLoRa main.cpp LoRaCalculator/LoRaCalculator.cpp LoRaCalculator/LoRaCalculator.h)
