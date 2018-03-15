cmake_minimum_required(VERSION 3.8)
project(KDZ)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp huffman.h HuffmanEncoderDecoder.h)
add_executable(KDZ ${SOURCE_FILES})