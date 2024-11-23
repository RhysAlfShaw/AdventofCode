#include <iostream> 
#include <sstream>
#include <string>
#include <vector>
#include <fstream>

auto isSymbol = [](std::string s) {
        for (char c : s) {
            if (!isdigit(c) && c != '.') {
                return true;
            }
        }
        return false;
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

std::vector<std::tuple<int, int, int, std::string>> findNumberIndices(const std::vector<std::vector<char>>& matrix) {
    std::vector<std::tuple<int, int, int, std::string>> indices;
    for (int i = 0; i < matrix.size(); i++) {
        const auto& row = matrix[i];
        int start = -1;
        std::string number;
        for (int j = 0; j < row.size(); j++) {
            if (isdigit(row[j])) {
                if (start == -1) {
                    start = j;
                }
                number += row[j];
            } else if (!number.empty()) {
                indices.push_back(std::make_tuple(i, start, j - 1, number));
                start = -1;
                number.clear();
            }
        }
        if (!number.empty()) {
            indices.push_back(std::make_tuple(i, start, row.size() - 1, number));
        }
    }
    return indices;
}

std::vector<std::tuple<int, int, int, std::string, bool, char, int, int>> checkSymbols(const std::vector<std::vector<char>>& matrix, const std::vector<std::tuple<int, int, int, std::string>>& indices) {
    std::vector<std::tuple<int, int, int, std::string, bool, char, int, int>> result;
    for (const auto& [row, start, end, number] : indices) {
        bool symbol = false;
        char symbolChar = ' ';
        int symbolRow = -1;
        int symbolCol = -1;
        // Check the cells to the left of the number
        if (start > 0 && !isdigit(matrix[row][start - 1]) && matrix[row][start - 1] != '.') {
            symbol = true;
            symbolChar = matrix[row][start - 1];
            symbolRow = row;
            symbolCol = start - 1;
        }
        // Check the cells to the right of the number
        if (!symbol && end < matrix[row].size() - 1 && !isdigit(matrix[row][end + 1]) && matrix[row][end + 1] != '.') {
            symbol = true;
            symbolChar = matrix[row][end + 1];
            symbolRow = row;
            symbolCol = end + 1;
        }
        // Check the cells above the number if there is a row above
        if (!symbol && row > 0) {
            for (int j = start; j <= end; j++) {
                if (!isdigit(matrix[row - 1][j]) && matrix[row - 1][j] != '.') {
                    symbol = true;
                    symbolChar = matrix[row - 1][j];
                    symbolRow = row - 1;
                    symbolCol = j;
                    break;
                }
            }
        }
        // Check the cells below the number if there is a row below
        if (!symbol && row < matrix.size() - 1) {
            for (int j = start; j <= end; j++) {
                if (!isdigit(matrix[row + 1][j]) && matrix[row + 1][j] != '.') {
                    symbol = true;
                    symbolChar = matrix[row + 1][j];
                    symbolRow = row + 1;
                    symbolCol = j;
                    break;
                }
            }
        }
        // Check the diagonal cells
        if (!symbol) {
            // Top-left diagonal
            if (row > 0 && start > 0 && !isdigit(matrix[row - 1][start - 1]) && matrix[row - 1][start - 1] != '.') {
                symbol = true;
                symbolChar = matrix[row - 1][start - 1];
                symbolRow = row - 1;
                symbolCol = start - 1;
            }
            // Top-right diagonal
            else if (row > 0 && end < matrix[row].size() - 1 && !isdigit(matrix[row - 1][end + 1]) && matrix[row - 1][end + 1] != '.') {
                symbol = true;
                symbolChar = matrix[row - 1][end + 1];
                symbolRow = row - 1;
                symbolCol = end + 1;
            }
            // Bottom-left diagonal
            else if (row < matrix.size() - 1 && start > 0 && !isdigit(matrix[row + 1][start - 1]) && matrix[row + 1][start - 1] != '.') {
                symbol = true;
                symbolChar = matrix[row + 1][start - 1];
                symbolRow = row + 1;
                symbolCol = start - 1;
            }
            // Bottom-right diagonal
            else if (row < matrix.size() - 1 && end < matrix[row].size() - 1 && !isdigit(matrix[row + 1][end + 1]) && matrix[row + 1][end + 1] != '.') {
                symbol = true;
                symbolChar = matrix[row + 1][end + 1];
                symbolRow = row + 1;
                symbolCol = end + 1;
            }
        }
        result.push_back(std::make_tuple(row, start, end, number, symbol, symbolChar, symbolRow, symbolCol));
    }
    return result;
}

std::vector<std::tuple<int, int>> findAsterisks(const std::vector<std::vector<char>>& matrix) {
    std::vector<std::tuple<int, int>> asterisks;
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            if (matrix[i][j] == '*') {
                asterisks.push_back(std::make_tuple(i, j));
            }
        }
    }
    return asterisks;
}

std::vector<std::string> findNumbersWithAsterisks(const std::vector<std::tuple<int, int, int, std::string, bool, char, int, int>>& symbols, const std::vector<std::tuple<int, int>>& asterisks) {
    std::vector<std::string> numbers;
    for (const auto& [row, start, end, number, symbol, symbolChar, symbolRow, symbolCol] : symbols) {
        if (symbol && symbolChar == '*') {
            for (const auto& [asteriskRow, asteriskCol] : asterisks) {
                if (symbolRow == asteriskRow && symbolCol == asteriskCol) {
                    numbers.push_back(number);
                    break;
                }
            }
        }
    }
    return numbers;
}

std::vector<int> productOfNumbersWithSameAsterisks(const std::vector<std::tuple<int, int, int, std::string, bool, char, int, int>>& symbols, const std::vector<std::tuple<int, int>>& asterisks) {
    std::vector<int> products(asterisks.size(), 1);
    for (int i = 0; i < asterisks.size(); i++) {
        const auto& [asteriskRow, asteriskCol] = asterisks[i];
        std::vector<std::string> numbers;
        for (const auto& [row, start, end, number, symbol, symbolChar, symbolRow, symbolCol] : symbols) {
            if (symbol && symbolChar == '*' && symbolRow == asteriskRow && symbolCol == asteriskCol) {
                numbers.push_back(number);
            }
        }
        if (numbers.size() > 1) {
            for (const auto& number : numbers) {
                products[i] *= std::stoi(number);
            }
        }
        else if (numbers.size() == 1) {
            products[i] = 0;
        }
    }
    return products;
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
    
    // print the lines in the file
    for (std::string line : lines) {
        std::cout << "Line: " << line << std::endl;
    }

    // create a matrix to represent all characters in all lines in the file
    std::vector<std::vector<char>> matrix;
    for (std::string line : lines) {
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        matrix.push_back(row);
    }

    // print the matrix
    for (std::vector<char> row : matrix) {
        for (char c : row) {
            std::cout << c;
        }
        std::cout << std::endl;
    }

     
    
    std::vector<std::tuple<int, int, int, std::string>> indices = findNumberIndices(matrix);

    // Print the indices and the number
    for (const auto& [row, start, end, number] : indices) {
        std::cout << "Row: " << row << ", Start: " << start << ", End: " << end << ", Number: " << number << '\n';
    }
    
    // create a function that will go though each row in the matrix and find numbers and the indexes where the begin and end

    
    std::vector<std::tuple<int, int, int, std::string, bool, char, int, int>> result = checkSymbols(matrix, indices);

    // Print the indices, the number, whether a symbol is present, the symbol, and the symbol's position
    for (const auto& [row, start, end, number, symbol, symbolChar, symbolRow, symbolCol] : result) {
        std::cout << "Row: " << row << ", Start: " << start << ", End: " << end << ", Number: " << number << ", Symbol: " << (symbol ? "true" : "false") << ", SymbolChar: " << symbolChar << ", SymbolRow: " << symbolRow << ", SymbolCol: " << symbolCol << '\n';
    }

    // sum the numbers that have symbol == true
    int sum = 0;
    for (const auto& [row, start, end, number, symbol, symbolChar, symbolRow, symbolCol] : result) {
        if (symbol) {
            sum += std::stoi(number);
        }
    }
    std::cout << "Sum: " << sum << '\n';
    std::vector<std::tuple<int, int>> asterisks = findAsterisks(matrix);

    // Print the positions of the asterisks
    for (const auto& [row, col] : asterisks) {
        std::cout << "Asterisk at Row: " << row << ", Col: " << col << '\n';
    }
    std::vector<std::string> numbers = findNumbersWithAsterisks(result, asterisks);

    // Print the numbers that have a '*' symbol at the same position
    for (const std::string& number : numbers) {
        std::cout << "Number with '*': " << number << '\n';
    }
    std::vector<int> products = productOfNumbersWithSameAsterisks(result, asterisks);
    for (int i = 0; i < products.size(); i++) {
        std::cout << "Product of numbers with asterisk " << i + 1 << ": " << products[i] << '\n';
    }
    // sum all the products
    int productSum = 0;
    for (int product : products) {
        productSum += product;
    }
    std::cout << "Product Sum: " << productSum << '\n';
    return 0;
}