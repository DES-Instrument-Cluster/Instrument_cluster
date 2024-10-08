FROM balenalib/raspberrypi4-64-debian:latest

RUN apt-get update && apt-get install -y \
    build-essential \
    qt5-qmake \
    qtbase5-dev \
    qtbase5-dev-tools \
    qtdeclarative5-dev \
    qttools5-dev-tools \
    qtchooser \
    libqt5opengl5-dev \
    libqt5svg5-dev \
    libqt5multimedia5-plugins \
    libqt5multimedia5 \
    libqt5multimediawidgets5 \
    libqt5x11extras5-dev \
	libqt5serialbus5-dev \
    libegl1-mesa-dev \
    libgles2-mesa-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
	qml-module-qtquick-controls2 \
    qtquickcontrols2-5-dev \
    qml-module-qtquick2 \
    qml-module-qtquick-layouts \
    qml-module-qtquick-window2 \
    qtmultimedia5-dev \
    qtbase5-private-dev \
    cmake \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /project

COPY . /project

RUN mkdir -p build && cd build && cmake .. && make

