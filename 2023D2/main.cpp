#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

class Game {
public:
    int gameNumber;
    int numSets;
    std::vector<std::unordered_map<std::string, int>> setColorCounts;
    std::vector<std::vector<int>> matrix;

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
        numSets++; // add one to account for the last set

        colormatrix = new int[numSets][3];

        // for each set string find the position of the matched color and save the number before it in the array

        for (int i = 0; i < numSets; i++) {
            std::string setString;
            std::getline(iss, setString, ';'); // extract the set string

            // print the set string
            std::cout << "Set " << i + 1 << ": " << setString << std::endl;



            // find the position of the first color in the set string
            // try and do this
            try{
                int position = setString.find("blue");
                // color nymber is the number before the color
                int bluecolorNumber = std::stoi(setString.substr(position - 2, 1));
            }
            // if it fails do this
            catch (std::exception e){
                int bluecolorNumber = 0;
            }

            try{
                int position = setString.find("red");
                // color nymber is the number before the color
                int redcolorNumber = std::stoi(setString.substr(position - 2, 1));
            }
            catch (std::exception e){
                int redcolorNumber = 0;
            }


            try{
                int position = setString.find("green");
                // color nymber is the number before the color
                int greencolorNumber = std::stoi(setString.substr(position - 2, 1));
            }
            catch (std::exception e){
                int greencolorNumber = 0;
            }

            // add the colors to the matrix
            colormatrix[i][0] = bluecolorNumber;
            colormatrix[i][1] = redcolorNumber;
            colormatrix[i][2] = greencolorNumber;


            
        }
        //print the matrix
        for (int i = 0; i < numSets; i++){
            for (int j = 0; j < 3; j++){
                std::cout << colormatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    std::string gameString = "Game 2: 3 blue, 4 red; 1 red, 2 green, 6 blue; 1 red, 4 green, 2 blue; 2 red, 1 green, 3 blue";

    Game game(gameString);

    std::cout << "Game Number: " << game.gameNumber << std::endl;
    std::cout << "Number of Sets: " << game.numSets << std::endl;

    

    return 0;
}