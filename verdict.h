#include <iostream>
#include <vector>   
#include "calculations.h"

class verdict
{
public:
    void getVerdict(vector<float> testScores, vector<string> matchingRes) {
    calculations c;
    vector<int> weights (testScores.size(), 0);
    
    weights[0] = 3;
    weights[1] = 4;
    weights[2] = 3;

    float final_score = (testScores[0]*weights[0] + testScores[1]*weights[1] + testScores[2]*weights[2])/c.sum(weights);
    string verdict;


    verdict=(final_score < 1)?"Not plagiarised":(final_score < 5)?"Slightly plagiarised":(final_score < 8)?"Fairly plagiarised":"Highly plagiarised";

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

