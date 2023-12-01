#include <fstream> // Include the necessary header file
#include <iostream>
#include <string>
#include <vector>
#include <fstream>



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
    
    
    std::vector<std::string> lines = readLines("calibration_data.txt"); // Read the lines from the file using the function
    
    // print the lines from the vector to the console
    for (std::string line : lines){
        std::cout << line << std::endl;
    }


    // go though each line and look for the first and last number sting 

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
                break; // break out of the loop since we want the last number
            }

        }
    }

    
    std::cout << "First Number String: ";
    for (std::string numberString : firstNumberStringvector){
        std::cout << numberString;
    }
    std::cout << std::endl;
    std::cout << "Last Number String: ";
    for (std::string numberstring : lastNumberStringvector){
        std::cout << numberstring;
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
        sum += std::stoi(numberString);
    }

    std::cout << "Sum: " << sum << std::endl;
    

    return 0;

}