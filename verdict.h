#include <iostream>
#include <vector>
#include "calculations.h"

class verdict
{
public:
    void getVerdict(vector<float> testScores, vector<string> matchingRes) {
    calculations c;
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

    float final_score = (testScores[0]*weights[0] + testScores[1]*weights[1] + testScores[2]*weights[2])/c.sum(weights);
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

};

