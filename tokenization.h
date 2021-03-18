#include<iostream>
#include<iterator>
#include<vector>
#include<sstream>

using namespace std;

class Tokenization{
    public:
    // tokenization
    vector<string> stringToToken(string inpString) {
        istringstream dStream(inpString);
        return vector<string>(istream_iterator<string>{dStream}, istream_iterator<string>{});
    }


};