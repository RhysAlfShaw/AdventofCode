#!/bin/bash
cpp_file="CalorieCounting.cpp"
output_executable="CalorieCounting.exe"
g++ -o "$output_executable" "$cpp_file" -std=c++11
if [ $? -eq 0 ]; then
  echo "Compilation successful. Executable: ./$output_executable"
else
  echo "Compilation failed."
fi
./"$output_executable"