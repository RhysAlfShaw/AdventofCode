#include <fstream> // Include the necessary header file
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>


std::vector<std::string> readLines(std::string path){

    std::vector<std::string> lines; // Vector to store the lines from the file.
    std::ifstream inputFile(path); // Create an input file stream object
    std::string line; // Read the file line by line

    while (std::getline(inputFile, line)){ // while there is a line to read
        lines.push_back(line); // Add the line to the vector
    }
    
    inputFile.close(); // Close the file
    return lines;
}


int main(int argc, char const *argv[]){
    
    
    std::vector<std::string> lines = readLines("speltout_data.txt"); // Read the lines from the file using the function
    
    // print the lines from the vector to the console
   
    // each line contains a number spelt out in words
    // go though each line and look for the first and last number sting

    std::vector<std::string> firstNumberStringvector;
    std::vector<std::string> lastNumberStringvector;

    // list of spelt out number
    std::vector<std::string> numberStrings = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    

    // loop through each line

    for (std::string& line : lines) {
        // Convert any numberStrings to digits and do the change after the loop.
        for (const std::string& numberString : numberStrings) {
            // Find the position of the numberString in the line.
            int position = line.find(numberString);
            // If the numberString is found in the line.
            if (position != std::string::npos) {
                // add the digit to the location of the numberString in the line.
                line.insert(position+1, std::to_string(std::distance(numberStrings.begin(), std::find(numberStrings.begin(), numberStrings.end(), numberString)) + 1));
            }
        }   
        
    }

    // Print the lines from the vector to the console
    for (const std::string& line : lines) {
        std::cout << line << std::endl;
    }
    
    return 0;

}