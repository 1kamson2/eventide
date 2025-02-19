#!/bin/bash

echo "Building project..."
cmake -S . -B build/
cmake --build build/
./build/eventide-engine
