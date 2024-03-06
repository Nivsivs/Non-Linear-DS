#include <iostream> //operations for input and output
#include <fstream>  //file input/output operations
#include <vector>  //Vector container

using namespace std;

int main() { 
    ifstream inFS; //input file stream
    char file; //declaring file specified character to search for
    char userName; //declaring user specified character to search for
    int count = 0;
    vector<char> collectChar;
    vector<int> position;

    //check if the file is open
    inFS.open("input.txt"); 
    if (!inFS.is_open()) {
        cout <<"The input file input.txt could not be opened." << endl;
        exit(EXIT_FAILURE); //
    }


    //Ask the user to enter a character to search for
    cout << "Enter the character to search for: "<< endl;
    cin >> userName;

    //read the line of text from the file
    while (inFS.get(file)){
        collectChar.push_back(file);
    }

    
    //close the file
    inFS.close();

    for (int i = 0; i < collectChar.size(); i++){
        if (collectChar.at(i) == userName){ 
            count++;
            position.push_back(i);
        }
    }

    cout << "The number of occurences of the character " << userName << " is " << count << endl;

    if (count > 0) {
        cout << "The number of indices";
        for (int i = 0; i < position.size(); i++){
            cout << " " << position.at(i);
        }
    }



    


}