#!/bin/bash
cpp_file="main.cpp"
output_executable="main.exe"
file_name="part2_data.txt"
g++ -o "$output_executable" "$cpp_file" -std=c++11
if [ $? -eq 0 ]; then
  echo "Compilation successful. Executable: ./$output_executable"
else
  echo "Compilation failed."
fi

./"$output_executable" "$file_name"