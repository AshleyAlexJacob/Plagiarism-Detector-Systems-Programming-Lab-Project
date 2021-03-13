#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <dirent.h>
#include <map>
#include <sstream>
#include <iomanip>
#include "calculations.h"
using namespace std;

// Removes Comma and Characters from the given string it helps to compare
// Input String: My,Name;is,Ashley.txt
// After Cleaning The String
// Output String: MyNameisAshleytxt
// tokenization
vector<string> stringToToken(string inpString) {
    istringstream dStream(inpString);
    return vector<string>(istream_iterator<string>{dStream}, istream_iterator<string>{});
}

// void stringCleansing(string &inpStr) {    
//     int i = 0;
//     int len = inpStr.length();
//     while(i < len){
//         if (!isalnum(inpStr[i]) && inpStr[i] != ' '){
//             inpStr.erase(i,1);
//             len--;
//         }else
//             i++;
//     }
// }

// string getFileData(string filePath) {
//     // ifstream object to access file data
//     ifstream dFile(filePath);
//     string retString,temp;
//     // accessing file data
//     while (dFile >> temp){
//         retString += string(" ") + temp;
//     }
// // uncomment to unleash the power
// // cout<<retString<<"\n";
//    stringCleansing(retString);
//     return retString;
// }


int main(){
    
    string test = "/home/ashley/work/spProject/SP Lab Project/target/ar.txt";
	 //printf("%s\n",test);
    //  cout<<test<<"\n";
    //  auto rest = stringToToken(test);
//     for(int i=rest.begin().;i!=rest.end();i++){

   // cout<<"After Cleaning The String\n";
    //cout<<test<<"\n";
// string res =     getFileData(test);
// cout<<res<<"\n";
   	return 0;
}
