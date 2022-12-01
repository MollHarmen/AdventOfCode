FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get upgrade -y &&  apt-get install -y \
    build-essential \
    git \
    cmake 
