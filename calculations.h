#include <cmath>
#include <algorithm>
#include <map>
#include <iomanip>
using namespace std;

class calculations{
    public:
   float dot_product(vector<int> ix, vector<int> iy) {
    float sum = 0 ;
    for(int i=0; i<ix.size(); i++)
        sum += ix[i] * iy[i];
    return sum;
}

float sum(vector<int> v) {
    float sumv = 0;
    for (auto& n : v)
        sumv += n;
    return sumv;
}

float get_multiplier(string word) {
    return word.length() * word.length();
}

map<string, int> getFrequency(vector<string> tokens) {
 map<string, int> freqs;
    for (auto const & x : tokens)
        ++freqs[x];
    vector<string> unique_tokens;
    vector<int> freq_token;
    for (auto const & p : freqs){
        unique_tokens.push_back(p.first);
        freq_token.push_back(p.second);
    }

    return freqs;
}
};