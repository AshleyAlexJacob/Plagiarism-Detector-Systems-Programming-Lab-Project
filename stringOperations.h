#include <iostream>
#include <fstream>

using namespace std;

class StringOperations{
    public:
    bool extCheck ( const string &fileName, string const &extension) {
    // check the file extension ends with .txt
    if (fileName.length() >= extension.length())
        return (0 == fileName.compare (fileName.length() - extension.length(), extension.length(), extension));
    else 
        return false;
    }

void stringCleansing(string &inpStr) {    
    int i = 0;
    int len = inpStr.length();
    while(i < len){
        if (!isalnum(inpStr[i]) && inpStr[i] != ' '){
            inpStr.erase(i,1);
            len--;
        }else
            i++;
    }
}
string getFileData(string filePath) {
    // ifstream object to access file data
    ifstream dFile(filePath);
    string retString,temp;
    // accessing file data
    while (dFile >> temp){
        retString += string(" ") + temp;
    }
   stringCleansing(retString);
    return retString;
}


};