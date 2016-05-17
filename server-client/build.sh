#!/bin/bash

echo "Cleaning..."
source clean.sh

echo "Running cmake into out/ ..."

mkdir out
cd out
cmake ..



