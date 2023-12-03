#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>

class Game {
public:
    int gameNumber;
    int numSets = 0;
    std::vector<std::unordered_map<std::string, int>> setColorCounts;
    std::vector<std::vector<int>> matrix; 
    int sumblue = 0;
    int sumred = 0;
    int sumgreen = 0;
    int maxblue;
    int maxred;
    int maxgreen;
    int minblue;
    int minred;
    int mingreen;
    int power;

    Game(const std::string& gameString) {
        std::istringstream iss(gameString);
        std::string gameHeader;
        std::getline(iss, gameHeader, ':'); // extract the game header (e.g., "Game 1")

        gameNumber = std::stoi(gameHeader.substr(5)); // extract the game number from the header

        // calulate the number of sets in the game as the number of semicolons in the game string +1
        // loop though each character in the game string
        for (char c : gameString) {
            if (c == ';') {
                numSets++;
            }
        }
        numSets++;
        int colormatrix[numSets][3];

        // for each set string find the position of the matched color and save the number before it in the array

        for (int i = 0; i < numSets; i++) {
            std::string setString;
            std::getline(iss, setString, ';'); // extract the set string
            // define the blue, red, and green color numbers
            int bluecolorNumber = 0;
            int redcolorNumber = 0;
            int greencolorNumber = 0;
            // print the set string
            std::cout << "Set " << i + 1 << ": " << setString << std::endl;



            // find the position of the first color in the set string
            // try and do this
          
            try {
                int position = setString.find("blue");

                if (position > 0){
                    std::cout << "Position: " << position << std::endl;
                    // color nymber is the number before the color
                    int lastSpace = setString.rfind(' ', position - 1);
                    // Find the space before the last space
                    int spaceBeforeLast = setString.rfind(' ', lastSpace - 1);
                    // Extract the number before the color
                    bluecolorNumber = std::stoi(setString.substr(spaceBeforeLast + 1, lastSpace - spaceBeforeLast - 1));
                }
                else {
                    bluecolorNumber = 0;
                }
            }
            catch (std::exception& e) {
                bluecolorNumber = 0;
            }

           
            try{
                int position = setString.find("red");
                // print the position
                if (position > 0) {
                    std::cout << "Position: " << position << std::endl;
                    // color nymber is the number before the color
                    int lastSpace = setString.rfind(' ', position - 1);
                    // Find the space before the last space
                    int spaceBeforeLast = setString.rfind(' ', lastSpace - 1);
                    // Extract the number before the color
                    redcolorNumber = std::stoi(setString.substr(spaceBeforeLast + 1, lastSpace - spaceBeforeLast - 1));
                }
                else {
                    redcolorNumber = 0;
                }
                }
            catch (std::exception e){
                // print the error

                redcolorNumber = 0;
            }


            try{
                int position = setString.find("green");

                // if the position is positive, else set the number to 0
                if (position > 0) {
                    std::cout << "Position: " << position << std::endl;
                    // color nymber is the number before the color
                    int lastSpace = setString.rfind(' ', position - 1);
                    // Find the space before the last space
                    int spaceBeforeLast = setString.rfind(' ', lastSpace - 1);
                    // Extract the number before the color
                    greencolorNumber = std::stoi(setString.substr(spaceBeforeLast + 1, lastSpace - spaceBeforeLast - 1));
                }
                else{
                    greencolorNumber = 0;
                }
                
            }
            catch (std::exception e){
                greencolorNumber = 0;
            }

            // add the colors to the matrix for the set
            colormatrix[i][0] = bluecolorNumber;
            colormatrix[i][1] = redcolorNumber;
            colormatrix[i][2] = greencolorNumber;
            
            //print the colors
            std::cout << "Blue: " << bluecolorNumber << std::endl;
            std::cout << "Red: " << redcolorNumber << std::endl;
            std::cout << "Green: " << greencolorNumber << std::endl;

            
        }
        
        // calculate sum of green, red, and blue
        std::vector<int> columnblue(numSets);
        for(int i = 0; i < numSets; i++) {
            columnblue[i] = colormatrix[i][0];
        }

        maxblue = *std::max_element(columnblue.begin(), columnblue.end());
        minblue = *std::min_element(columnblue.begin(), columnblue.end());
        std::cout << std::endl;
        std::cout << "Max Blue: " << maxblue << std::endl; 

        std::vector<int> columnred(numSets);
        for(int i = 0; i < numSets; i++) {
            columnred[i] = colormatrix[i][1];
        }

        maxred = *std::max_element(columnred.begin(), columnred.end());
        minred = *std::min_element(columnred.begin(), columnred.end());

        std::cout << "Max Red: " << maxred << std::endl;

        std::vector<int> columngreen(numSets);
        for(int i = 0; i < numSets; i++) {
            columngreen[i] = colormatrix[i][2];
        }

        maxgreen = *std::max_element(columngreen.begin(), columngreen.end());
        mingreen = *std::min_element(columngreen.begin(), columngreen.end());

        std::cout << "Max Green: " << maxgreen << std::endl;

        // calculate the power of the game

        power = maxblue*maxred*maxgreen;

        for (int i = 0; i < numSets; i++) {
            sumblue += colormatrix[i][0];
            sumred += colormatrix[i][1];
            sumgreen += colormatrix[i][2];
        }
    }
};

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



int main(int argc, char const *argv[]) {

    if(argc < 2) {
        std::cerr << "ERROR!\n";
        std::cerr << "No file path provided. Please provide a file path as an argument.\n";
        return 1;
    }
    
    std::string file_path = argv[1];
    // print the file path
    std::cout << "File Path: " << file_path << std::endl;
    std::vector<std::string> lines = readLines(file_path);
    // print the number of lines in the file

    int maxred = 12;
    int maxblue = 14;
    int maxgreen = 13;

    int sum_game_number_if_possible = 0;
    int sumpower = 0;

    // loop though each line in the file
    for (std::string line : lines) {
        // print the line
        std::cout << "Line: " << line << std::endl;
        Game game(line);

        //std::cout << "Game Number: " << game.gameNumber << std::endl;
        std::cout << "Number of Sets: " << game.numSets << std::endl;
        std::cout << "Color Matrix: " << std::endl;
        
        std::cout << "Min Blue: " << game.minblue << std::endl;
        std::cout << "Min Red: " << game.minred << std::endl;
        std::cout << "Min Green: " << game.mingreen << std::endl;

        sumpower += game.power;
       // check if game is possble so long as the max number of each color is not exceeded
        if (game.maxblue <= maxblue && game.maxred <= maxred && game.maxgreen <= maxgreen) {
            std::cout << "Game " << game.gameNumber << " is possible." << std::endl;
            sum_game_number_if_possible += game.gameNumber;
        }
        else {
            std::cout << "Game " << game.gameNumber << " is not possible." << std::endl;
        }
        
    }

    std::cout << "Sum of Game Numbers for Possible Games: " << sum_game_number_if_possible << std::endl;
    std::cout << "Sum of Powers: " << sumpower << std::endl;

    

    return 0;
};