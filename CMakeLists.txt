cmake_minimum_required(VERSION 3.8)
project(KDZ)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp huffman.h EncoderDecoder.h fano.h lz77.h Utility.h experiments.h)
add_executable(KDZ ${SOURCE_FILES})