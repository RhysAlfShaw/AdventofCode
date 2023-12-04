#include <iostream>
#include <sstream>  
#include <string>
#include <vector>
#include <fstream>


// function to read the lines from a file and return them as a vector of strings

std::vector<std::string> readLines(std::string path){
    
    // Create a vector of strings to store the lines from the file.

    std::vector<std::string> lines;         // Vector to store the lines from the file.
    std::ifstream inputFile(path);          // Create an input file stream object
    std::string line;                       // Read the file line by line

    while (std::getline(inputFile, line)){  // while there is a line to read
        lines.push_back(line);              // Add the line to the vector
    }
    
    inputFile.close();                      // Close the file
    return lines;
};

// task:
// Need to find how many matches per row there are the winning numbers ( the ones that are before the | with those after).
// caluclate the score of that line by doubleing for each match so it should be 1X2X2X2 1*2^{nwins-1}



int main(int argc, char const *argv[]){
    if(argc < 2) {
        std::cerr << "ERROR!\n";
        std::cerr << "No file path provided. Please provide a file path as an argument.\n";
        return 1;
    }
    
    std::string file_path = argv[1];
    // print the file path
    std::cout << "File Path: " << file_path << std::endl;
    std::vector<std::string> lines = readLines(file_path);
    
    // print the lines in the file
    for (std::string line : lines) {
        std::cout << "Line: " << line << std::endl;
    }

    // function that will interate though each line and caluclate the number of matches
    // take a line and create two vectors, one of winning and one of not winning. 
    // these can be seperated at the | and go left untill : or right too the end of the line.
    // we can then find all the numbers in the lines segements that makes numbers and are seperated by a space.
    // find the number of matches between the two vectors.
    // when we have the number of matches we can then use a funtction to find the point score

    // then add the points up for each line

    // print the result.

    return 0;
}