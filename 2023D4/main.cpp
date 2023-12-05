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
// calculate the score of that line by doubleing for each match so it should be 1X2X2X2 1*2^{nwins-1}


std::vector<int> splitIntoNumbers(const std::string& str) {
    std::istringstream iss(str);
    std::vector<int> numbers;
    int number;
    while (iss >> number) {
        numbers.push_back(number);
    }
    return numbers;
}

int countMatches(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    int matches = std::count_if(vec1.begin(), vec1.end(), [&](int i) {
        return std::find(vec2.begin(), vec2.end(), i) != vec2.end();
    });
    return matches;
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

    std::vector<std::string> lines = readLines(file_path);
    
    std::vector<int> scores(lines.size(), 0);
    
    // print the lines in the file
    
    for (std::string line : lines) {
        std::cout << "Line: " << line << std::endl;
    }

    int index = 0;

    // create a array of length number og lines

    std::vector<int> scores_total(lines.size(), 1);


    for (std::string line : lines) {
        index++;
        // seperate the line at the '|'
        std::cout <<"Index :"<< index;
        char delimiter = '|';

        size_t pos = line.find(delimiter);
        std::string winning_numbers = line.substr(0, pos);
        std::string my_numbers = line.substr(pos + 1);
        size_t colonPos = winning_numbers.find(':');
        winning_numbers = winning_numbers.substr(colonPos + 1);

        //std::cout << "First half: " << winning_numbers << '\n';
        //std::cout << "Second half: " << my_numbers << '\n';
        
        // put them into a vector

        std::vector<int> winning_numbers_vector = splitIntoNumbers(winning_numbers);
        std::vector<int> my_numbers_vector = splitIntoNumbers(my_numbers);
        
        //for (int number : winning_numbers_vector) {
        //std::cout << number << ' ';
       //}
        // std::cout << '\n';

        // std::cout << "My numbers: ";
        // for (int number : my_numbers_vector) {
        //     std::cout << number << ' ';
        // }


        int matches = countMatches(winning_numbers_vector,my_numbers_vector);
        std::cout << " \n Number of matches: " << matches <<'\n';
        

        // calculate the part 1 score.

        int score = 1*pow(2,matches-1);
        std::cout<< "Part 1 Score :" << score <<"\n"<<std::endl;
        scores_total[index-1] = score;

        //part 2 scores

        // if we have matches we want to add a one to the index of the next line up to the number of matches
        
        if (matches > 0){
            for (int i = 0; i < matches; i++){
                scores[index+i] = scores[index+i] + 1;
            }
        }
        
    }

    // combined score

    //std::cout << "Combined Score :"<<score_total<<std::endl;

    std::cout << "Part 2 Scores :"<<std::endl;

    // multiply scores_total by scores

    // sum the scores_total

    int sum_of_scores = 0;

    for (int i = 0; i < scores.size(); i++){
        scores[i] = scores[i]*scores_total[i];
        sum_of_scores = sum_of_scores + scores[i];
    }

    std::cout << "Sum of scores : " << sum_of_scores  << std::endl;


    
    // loop though the vector
    for (int i = 0; i < scores.size(); i++){
        //scores[i] = pow(2,scores[i]-1);
        std::cout << "Line " << i+1 << " : " << scores[i] << std::endl;
    }

    //std::cout << "Sum of scores : " << sum_of_scores  << std::endl;
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