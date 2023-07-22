FROM ubuntu:22.10

RUN apt update
RUN apt install -y libcgal-dev
RUN apt install -y g++
RUN apt install -y cmake
RUN apt install -y libboost-all-dev
RUN apt install -y libgmp3-dev