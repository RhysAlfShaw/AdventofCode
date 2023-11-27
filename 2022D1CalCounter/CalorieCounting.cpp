#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>



int main(int argc, char const *argv[])
{

    const char* PATH_list = "list_cals.txt";

    // Create an input file stream object
    std::ifstream inputFile(PATH_list);

    // Check if the file is open
    
    if (!inputFile.is_open())
    {
        std::cout << "Could not open the file " << PATH_list << std::endl;
        return 1;
    }

    // Vector to store the lines from the file.
    std::vector<std::string> lines;

    // Read the file line by line
    std::string line;
    
    while (std::getline(inputFile, line)) // while there is a line to read
    {
        lines.push_back(line);
    }
    inputFile.close();

    // Vector to store sums
    std::vector<int> sums;

    // Temporary variable to store the current sum
    int currentSum = 0;

    
    // Iterate through the lines
    for (std::string line : lines)
    {
        // If the line is blank
        if (line.empty())
        {
            // Add the current sum to the sums vector
            sums.push_back(currentSum);

            // Reset the current sum
            currentSum = 0;
        }
        else
        {
            // Convert the line to an integer (with error handling)
            try
            {
                int value = std::stoi(line);
                currentSum += value;
            }
            catch (const std::invalid_argument& e)
            {
                std::cerr << "Invalid argument: " << e.what() << std::endl;
            }
            catch (const std::out_of_range& e)
            {
                std::cerr << "Out of range: " << e.what() << std::endl;
            }
        }
    }

    // Add the final sum after the loop completes
    sums.push_back(currentSum);
        // Print the length of the sums vector
        std::cout << "Number of sums: " << sums.size() << std::endl;

        // Print the sums
        for (int sum : sums)
        {
            std::cout << "Sum: " << sum << std::endl;
        }

    // print the index of the max element
    auto maxElement = std::max_element(sums.begin(), sums.end());
    int maxIndex = std::distance(sums.begin(), maxElement);
    std::cout << "Elf number: " << maxIndex+1 << ". Has the most Calories"<< std::endl;
    // Print the max element
    std::cout << "Elf " << maxIndex+1<<" Has "<< *maxElement << " Calories"<<std::endl;

    return 0;
}