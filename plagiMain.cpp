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

char const * database = "database";
char const * target_folder = "target";
char const * stopwords_file = "stopwords.txt";

int score_accuracy = 1;
int number_of_tests = 3;

float sum(vector<int> v) {
    float sumv = 0;
    for (auto& n : v)
        sumv += n;
    return sumv;
}

float get_multiplier(string word) {
    return word.length() * word.length();
}

float cosine_score(vector<int> bvector, vector<int> tvector) {
    calculations c;
    return c.dot_product(bvector, tvector) / 
            (   sqrt(c.dot_product(bvector, bvector)) * 
                sqrt(c.dot_product(tvector, tvector)) );
}

bool extCheck (string const &fileName, string const &extension) {
    // check the file extension ends with .txt
    if (fileName.length() >= extension.length())
        return (0 == fileName.compare (fileName.length() - extension.length(), extension.length(), extension));
    else 
        return false;
}
// Removes Comma and Characters from the given string it helps to compare
// Input String: My,Name;is,Ashley.txt
// After Cleaning The String
// Output String: MyNameisAshleytxt
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
// uncomment to unleash the power
// cout<<retString<<"\n";
   stringCleansing(retString);
    return retString;
}

map<string, int> get_frequency(vector<string> tokens) {
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
// we can minus to check
// tokenization
vector<string> stringToToken(string inpString) {
    istringstream dStream(inpString);
    return vector<string>(istream_iterator<string>{dStream}, istream_iterator<string>{});
}

float ngram_score(vector<string> dataBase, vector<string> target, int n) {
    vector<vector<string>> bngrams;
    vector<vector<string>> tngrams;
    vector<string> temp;

    for(int i=0; i<=dataBase.size()-n; i++) {
        temp.clear();
        for(int j=i; j<i+n; j++) 
            temp.push_back(dataBase[j]);
        bngrams.push_back(temp);
    }

    for(int i=0; i<=target.size()-n; i++) {
        temp.clear();
        for(int j=i; j<i+n; j++) 
            temp.push_back(target[j]);
        tngrams.push_back(temp);
    }

    int shared = 0;
    int total = tngrams.size();

    for(auto const & tngram: tngrams)
        for(auto const & bngram: bngrams)
            if(tngram == bngram) {
                shared += 1;
                break;
            }

    return 1.0 * shared / total;
}

float tokenizeTest(vector<string> dbTokens, vector<string> tarTokens) {
    ifstream inFile(stopwords_file);
    string stopword;
    while (inFile >> stopword){
        tarTokens.erase(remove(tarTokens.begin(), tarTokens.end(), stopword), tarTokens.end());
    }
    
    auto tarFreqs = get_frequency(tarTokens);
    auto dbFreqs = get_frequency(dbTokens);
    
    int shared = 0;
    int total = 0;
    
    for(auto const & word : tarFreqs) {
        auto search = dbFreqs .find(word.first);
        if(search != dbFreqs .end()){
            shared += std::min(word.second, search->second) * get_multiplier(word.first);
            total += word.second * get_multiplier(word.first);
        } else {
            total += word.second * get_multiplier(word.first);
        }
    }
    float score = 10.0 * shared / total;

    return score;
}

float ngramTest(vector<string> dbTokens,vector<string> tarTokens) {
   vector<int> tests {3, 5, 7};
   vector<int> weights {3, 5, 7};
   vector<float> ngresults;

    ngresults.push_back(ngram_score(dbTokens, tarTokens, 3));
    ngresults.push_back(ngram_score(dbTokens, tarTokens, 5));
    ngresults.push_back(ngram_score(dbTokens, tarTokens, 7));

    float score = 10 * pow((ngresults[0]*weights[0] + ngresults[1]*weights[1] + ngresults[2]*weights[2])/sum(weights), 0.4);
    return score;
}

float cosineTest(vector<string> dbTokens,vector<string> tarTokens) {
    ifstream infile(stopwords_file);
    string stopword;
    while (infile >> stopword) {
        tarTokens.erase(remove(tarTokens.begin(), tarTokens.end(), stopword), tarTokens.end());
        dbTokens.erase(remove(dbTokens.begin(), dbTokens.end(), stopword), dbTokens.end());
    }

    vector<string> all_tokens;
    all_tokens.reserve( tarTokens.size() + dbTokens.size() );
    all_tokens.insert( all_tokens.end(), tarTokens.begin(), tarTokens.end() );
    all_tokens.insert( all_tokens.end(), dbTokens.begin(), dbTokens.end() );
    sort( all_tokens.begin(), all_tokens.end() );
    all_tokens.erase( unique( all_tokens.begin(), all_tokens.end() ), all_tokens.end() );

    auto tarFreqs = get_frequency(tarTokens);
    auto dbFreqs  = get_frequency(dbTokens);

    vector<int> dbVector;
    vector<int> tarVector;

    for(auto & token: all_tokens) {
        auto search = dbFreqs .find(token);
        if(search != dbFreqs .end()) {
            dbVector.push_back(search->second);
        } else {
            dbVector.push_back(0);
        }

        search = tarFreqs.find(token);
        if(search != tarFreqs.end()) {
            tarVector.push_back(search->second);
        } else {
            tarVector.push_back(0);
        }
    }

    float score = 10.0 * cosine_score(dbVector, tarVector);

    return score;
}

void get_verdict(vector<float> testScores, vector<string> matchingRes) {
    vector<int> weights (testScores.size(), 0);
    
    /**************************
        test1 - tokenize test
        test2 - ngram test
        test3 - cosine test
    ***************************/
// deal with it
    weights[0] = 3;
    weights[1] = 4;
    weights[2] = 3;

    float final_score = (testScores[0]*weights[0] + testScores[1]*weights[1] + testScores[2]*weights[2])/sum(weights);
    string verdict;

    if(final_score < 1)
        verdict = "Not plagiarised";
    else if(final_score < 5)
        verdict = "Slightly plagiarised";
    else if(final_score < 8)
        verdict = "Fairly plagiarised";
    else
        verdict = "Highly plagiarised";

    matchingRes.erase( remove( matchingRes.begin(), matchingRes.end(), "" ), matchingRes.end() );
    sort( matchingRes.begin(), matchingRes.end() );
    matchingRes.erase( unique( matchingRes.begin(), matchingRes.end() ), matchingRes.end() );

    cout<<"********************************************"<<std::endl;
    cout<<"\tFinal score: "<<final_score<<endl;
    cout<<"\tVerdict: "<<verdict<<endl;
    if(verdict != "Not plagiarised") {
        cout<<"\tMatch found in:"<<endl;
        if(matchingRes.size() == 0)
            cout<<"\t-nil-"<<endl;
        for (auto const & file : matchingRes)
           cout<<"\t\t"<<file<<endl;
    }
    
        cout<<"********************************************"<<endl;

}

int main(int argc, char *argv[]) {
    DIR *dir; // for target folder
    DIR *dirB;
    struct dirent *dir_object;
    string target_file;
    string base_file;
    string target;
    string base;

    float temp;

    if ((dir = opendir (target_folder)) != NULL) {
        while ((dir_object = readdir (dir)) != NULL)
            if(extCheck((string)dir_object->d_name, ".txt")){
                // target file name
                printf ("\nPlagiarism scores for %s\n", dir_object->d_name);
                // generate Path dynamically
                target_file = target_folder + string("/") + dir_object->d_name;
                target = getFileData(target_file);
            // vector files declared variables
                vector<float> test(number_of_tests, 0.0);
               vector<string> match(number_of_tests, "");
// database directory
                if ((dirB = opendir (database)) != NULL) {
                    while ((dir_object = readdir (dirB)) != NULL)
                        if(extCheck((string)dir_object->d_name, ".txt")){
                           // path generate
                            base_file = database + string("/") + dir_object->d_name;
                        // accessing file data but by bit
                            base = getFileData(base_file);
                        // generating tokens from string
                            auto databaseTokens = stringToToken(base);
                            auto targetTokens = stringToToken(target);
                //  Tokenization Done
                           // FiRST tEST RESULT
                            temp = tokenizeTest(databaseTokens,targetTokens);
                            if(test[0] < temp) {
                                test[0] = temp;
                                match[0] = dir_object->d_name;
                            }
                            temp = ngramTest(databaseTokens,targetTokens);
                            if(test[1] < temp) {
                                test[1] = temp;
                                match[1] = dir_object->d_name;
                            }
                            temp = cosineTest(databaseTokens,targetTokens);
                            if(test[2] < temp) {
                                test[2] = temp;
                                match[2] = dir_object->d_name;
                            }

                        }
                    closedir (dirB);
                }

                cout<<"Test 1 score: "<<fixed<<setprecision(score_accuracy)<<test[0]<<"/10"<<endl;
                cout<<"Test 2 score: "<<fixed<<setprecision(score_accuracy)<<test[1]<<"/10"<<endl;
                cout<<"Test 3 score: "<<fixed<<setprecision(score_accuracy)<<test[2]<<"/10"<<endl;

                get_verdict(test, match);

              cout<<"\n";
            }
                
        closedir (dir);
    }
}