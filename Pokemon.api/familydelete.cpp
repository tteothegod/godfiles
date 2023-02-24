#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    string file_name = "./league/notes.txt";
    string line;
    string output_line;
    
    // Read the entire file into memory
    ifstream file(file_name);
    if (file.is_open()) {
        while (getline(file, line)) {
            for (char c : line) {
                if (c != ' ') {
                    output_line += c;
                }
            }
            output_line += '\n';
        }
        file.close();
    } else {
        cout << "Unable to open file." << endl;
        return 1;
    }
    
    // Write the lines back to the file
    ofstream output_file(file_name);
    if (output_file.is_open()) {
        output_file << output_line;
        output_file.close();
    } else {
        cout << "Unable to open file for writing." << endl;
        return 1;
    }
    
    cout << "File updated successfully." << endl;
    return 0;
}
