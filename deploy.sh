#!/bin/bash

# Note that this will only work in ARM64 architecture host(e.g. Apple Silicon Mac)
# Please alternate RASPBERRY_PI_HOST value when the ip adress of Raspberry Pi is changed

# Variables
DOCKER_IMAGE_NAME="qt-app-builder"
DOCKER_CONTAINER_NAME="qt-app-build-container"
BUILD_DIR="/project/build"
HOST_COPY_DIR="."
RASPBERRY_PI_USER="pi"
RASPBERRY_PI_HOST="192.168.86.43"
RASPBERRY_PI_TARGET_DIR="/home/${RASPBERRY_PI_USER}"
QT_APP_NAME="InstrumentCluster"

echo "Building the Docker container..."
docker build -t $DOCKER_IMAGE_NAME .

echo "Running the Docker container..."
docker run --name $DOCKER_CONTAINER_NAME $DOCKER_IMAGE_NAME

echo "Copying the built application to the host machine..."
docker cp $DOCKER_CONTAINER_NAME:$BUILD_DIR/$QT_APP_NAME $HOST_COPY_DIR

echo "Removing the Docker container..."
docker rm $DOCKER_CONTAINER_NAME

echo "Transferring the built application to the Raspberry Pi..."
scp -r $HOST_COPY_DIR/$QT_APP_NAME $RASPBERRY_PI_USER@$RASPBERRY_PI_HOST:$RASPBERRY_PI_TARGET_DIR

echo "Removing executable file from host..."
rm $HOST_COPY_DIR/$QT_APP_NAME

echo "Done"
