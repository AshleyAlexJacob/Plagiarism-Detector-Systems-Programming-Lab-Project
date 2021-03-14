#include <iostream>
// #include <vector>
// #include <cmath>
// #include <fstream>
// #include <algorithm>
// #include <iterator>
#include <dirent.h>
// #include <map>
// #include <sstream>
// #include <iomanip>
#include "verdict.h"
#include "globals.h"
#include "stringOperations.h"
#include "tokenization.h"

using namespace std;
int score_accuracy = 1;
int number_of_tests = 3;


float cosine_score(vector<int> bvector, vector<int> tvector) {
    calculations c;
    return c.dot_product(bvector, tvector) / 
            (   sqrt(c.dot_product(bvector, bvector)) * 
                sqrt(c.dot_product(tvector, tvector)) );
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

float tokenizeTest(vector<string> dbTokens, vector<string> tarTokens,const char *stopWords) {
    ifstream inFile(stopWords);
    string stopword;
    while (inFile >> stopword){
        tarTokens.erase(remove(tarTokens.begin(), tarTokens.end(), stopword), tarTokens.end());
    }
    calculations c1;
    auto tarFreqs = c1.getFrequency(tarTokens);
    auto dbFreqs = c1.getFrequency(dbTokens);
    
    int shared = 0;
    int total = 0;
    calculations c;
    for(auto const & word : tarFreqs) {
        auto search = dbFreqs .find(word.first);
        if(search != dbFreqs .end()){
            shared += std::min(word.second, search->second) * c.get_multiplier(word.first);
            total += word.second * c.get_multiplier(word.first);
        } else {
            total += word.second * c.get_multiplier(word.first);
        }
    }
    float score = 10.0 * shared / total;

    return score;
}

float ngramTest(vector<string> dbTokens,vector<string> tarTokens) {
   vector<int> tests {3, 5, 7};
   vector<int> weights {3, 5, 7};
   vector<float> ngresults;
   calculations c;

    ngresults.push_back(ngram_score(dbTokens, tarTokens, 3));
    ngresults.push_back(ngram_score(dbTokens, tarTokens, 5));
    ngresults.push_back(ngram_score(dbTokens, tarTokens, 7));

    float score = 10 * pow((ngresults[0]*weights[0] + ngresults[1]*weights[1] + ngresults[2]*weights[2])/c.sum(weights), 0.4);
    return score;
}

float cosineTest(vector<string> dbTokens,vector<string> tarTokens,const char* stopWords) {
    ifstream infile(stopWords);
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
    calculations c2;
    auto tarFreqs = c2.getFrequency(tarTokens);
    auto dbFreqs  = c2.getFrequency(dbTokens);

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


int main(int argc, char *argv[]) {
    Tokenization tok;
    verdict v;
    DIR *dir; // for target folder
    DIR *dirB;
    struct dirent *dir_object;
    string target_file;
    string base_file;
    string target;
    string base;
    globals mGlobals;
    StringOperations sP;
    float temp;

    if ((dir = opendir (mGlobals.target_folder)) != NULL) {
        while ((dir_object = readdir (dir)) != NULL)
            if(sP.extCheck((string)dir_object->d_name, ".txt")){
                // target file name
                printf ("\nPlagiarism scores for %s\n", dir_object->d_name);
                // generate Path dynamically
                target_file = mGlobals.target_folder + string("/") + dir_object->d_name;
                target = sP.getFileData(target_file);
            // vector files declared variables
                vector<float> test(number_of_tests, 0.0);
               vector<string> match(number_of_tests, "");
// database directory
                if ((dirB = opendir (mGlobals.database)) != NULL) {
                    while ((dir_object = readdir (dirB)) != NULL)
                        if(sP.extCheck((string)dir_object->d_name, ".txt")){
                           // path generate
                            base_file = mGlobals.database + string("/") + dir_object->d_name;
                        // accessing file data but by bit
                            base = sP.getFileData(base_file);
                        // generating tokens from string
                            auto databaseTokens = tok.stringToToken(base);
                            auto targetTokens = tok.stringToToken(target);
                //  Tokenization Done
                           // FiRST tEST RESULT
                            temp = tokenizeTest(databaseTokens,targetTokens,mGlobals.stopwords_file);
                            if(test[0] < temp) {
                                test[0] = temp;
                                match[0] = dir_object->d_name;
                            }
                            temp = ngramTest(databaseTokens,targetTokens);
                            if(test[1] < temp) {
                                test[1] = temp;
                                match[1] = dir_object->d_name;
                            }
                            temp = cosineTest(databaseTokens,targetTokens,mGlobals.stopwords_file);
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

                v.getVerdict(test, match);

              cout<<"\n";
            }
                
        closedir (dir);
    }
}