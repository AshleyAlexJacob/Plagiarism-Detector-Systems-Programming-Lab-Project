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
using namespace std;

class calculations{
    public:
   float dot_product(vector<int> ix, vector<int> iy) {
    float sum = 0 ;
    for(int i=0; i<ix.size(); i++)
        sum += ix[i] * iy[i];
    return sum;
}

};