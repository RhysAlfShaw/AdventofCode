#!/bin/bash

# C++ file name
cpp_file="CalorieCounting.cpp"

# Output executable name
output_executable="CalorieCounting.exe"

# Compilation command
g++ -o "$output_executable" "$cpp_file" -std=c++11

# Check if compilation was successful
if [ $? -eq 0 ]; then
  echo "Compilation successful. Executable: ./$output_executable"
else
  echo "Compilation failed."
fi

# Run executable
./"$output_executable"