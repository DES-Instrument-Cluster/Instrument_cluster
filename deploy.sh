#!/bin/bash

# Variables
DOCKER_IMAGE_NAME="qt-app-builder"
DOCKER_CONTAINER_NAME="qt-app-build-container"
HOST_COPY_DIR="."
RASPBERRY_PI_USER="koserjinda"
RASPBERRY_PI_HOST="192.168.86.38"
RASPBERRY_PI_TARGET_DIR="/home/${RASPBERRY_PI_USER}"
QT_APP_NAME="InstrumentCluster"

echo "Building the Docker container..."
docker build -t $DOCKER_IMAGE_NAME .

echo "Running the Docker container to build the Qt application..."
docker run --name $DOCKER_CONTAINER_NAME $DOCKER_IMAGE_NAME

echo "Copying the built application to the host machine..."
docker cp $DOCKER_CONTAINER_NAME:$BUILD_DIR $HOST_COPY_DIR

echo "Removing the Docker container..."
docker rm $DOCKER_CONTAINER_NAME

echo "Transferring the built application to the Raspberry Pi..."
scp -r $HOST_COPY_DIR/$QT_APP_NAME $RASPBERRY_PI_USER@$RASPBERRY_PI_HOST:$RASPBERRY_PI_TARGET_DIR

echo "Removing executable file from host..."
rm $HOST_COPY_DIR/$QT_APP_NAME

echo "Deployment completed successfully!"
