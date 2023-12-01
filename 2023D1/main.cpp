#include <fstream> // Include the necessary header file
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


// function to read the lines from a file and return them as a vector of strings

std::vector<std::string> readLines(std::string path){

    std::vector<std::string> lines;         // Vector to store the lines from the file.
    std::ifstream inputFile(path);          // Create an input file stream object
    std::string line;                       // Read the file line by line

    while (std::getline(inputFile, line)){  // while there is a line to read
        lines.push_back(line);              // Add the line to the vector
    }
    
    inputFile.close();                      // Close the file
    return lines;
}



int main(int argc, char const *argv[]){
    
    if(argc < 2) {
        std::cerr << "ERROR!\n";
        std::cerr << "No file path provided. Please provide a file path as an argument.\n";
        return 1;
    }
    
    std::string file_path = argv[1];
    // print the file path
    std::cout << "File Path: " << file_path << std::endl;

    std::vector<std::string> lines = readLines(file_path); // Read the lines from the file using the function
    

    // correct for speltout words.

    std::vector<std::string> numberStringss = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    

    // loop through each line and add the digit to the location of the numberString in the line + 1.
    // replaces onetwo3four with one1two2three3four4.

    for (std::string& line : lines) {

        // Convert any numberStrings to digits and do the change after the loop.
        
        for (const std::string& numberString : numberStringss) {
        
            // while loop to replace all instances of the numberString in the line.
        
            while (line.find(numberString) != std::string::npos) {
        
                // Find the position of the numberString in the line.
        
                int position = line.find(numberString);
        
                // add the digit to the location of the numberString in the line + 1.
        
                line.insert(position+1, std::to_string(std::distance(numberStringss.begin(), std::find(numberStringss.begin(), numberStringss.end(), numberString)) + 1));
    
            }

        }   
        
    }

    // go though each line and look for the first and last number string 

    // create two vectors to store the first and last number strings

    std::vector<std::string> firstNumberStringvector;
    std::vector<std::string> lastNumberStringvector;


    // loop through each line
    
    for (std::string line : lines){
    
        // loop through each character in the line  
        
        for (char character : line){
    
            // if the character is a number
            if (isdigit(character)){
    
                // save the character as a string and push it into the first number string vector
                std::string numberString(1, character);
    
                firstNumberStringvector.push_back(numberString);
    
                int position = line.find(numberString);
    
                break; // break out of the loop since we want the first number
            }
        }

        //loop though each character in the line backwards
        for (int i = line.length() - 1; i >= 0; i--){
        
            // if the character is a number
        
            if (isdigit(line[i])){
        
                // save the character as a string and push it into the last number string vector
        
                std::string numberString(1, line[i]);
        
                lastNumberStringvector.push_back(numberString);
        
                break;                          // break out of the loop since we want the last number
            }
        }
    }
    
    // append the strings of the same index in the vectors together into a new vector

    std::vector<std::string> numberStrings;

    for (int i = 0; i < firstNumberStringvector.size(); i++){
        numberStrings.push_back(firstNumberStringvector[i] + lastNumberStringvector[i]);
    }



    // print the number strings
    std::cout << std::endl;
    std::cout << "Number Strings: ";
    
    for (std::string numberString : numberStrings){
        std::cout << numberString << " ";
    }
    
    std::cout << std::endl;

    // print the sum of the numbers in the vector
    int sum = 0;
    for (std::string numberString : numberStrings){
        sum += std::stoi(numberString);             // convert the string to an int and add it to the sum
    }

    std::cout << "Sum: " << sum << std::endl;       // print the sum
    

    return 0;
}