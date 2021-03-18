#include <dirent.h>
#include "verdict.h"
#include "tests.h"
#include "globals.h"
#include "stringOperations.h"
#include "tokenization.h"
using namespace std;

/****Objects****/
Tokenization tok;
tests tst;
verdict v;
globals mGlobals;
StringOperations sP;
    
int main(int argc,char * argv[]) {

    DIR *dir; // for target directory
    DIR *dirB; //for datbase directory
    struct dirent *dir_object;
    string target_file,base_file,target,base;
    float temp;

if(argc==2){

mGlobals.score_accuracy=atoi(argv[1]);

    if ((dir = opendir (mGlobals.target_folder)) != NULL) {
        while ((dir_object = readdir (dir)) != NULL)
            if(sP.extCheck((string)dir_object->d_name, ".txt")){
                // target file name
                printf ("\nPlagiarism scores for %s\n", dir_object->d_name);
                // generate Path dynamically
                target_file = mGlobals.target_folder + string("/") + dir_object->d_name;
                target = sP.getFileData(target_file);
            // vector files declared variables
                vector<float> test(mGlobals.number_of_tests, 0.0);
               vector<string> match(mGlobals.number_of_tests, "");
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
                            temp = tst.tokenizeTest(databaseTokens,targetTokens,mGlobals.stopwords_file);
                            if(test[0] < temp) {
                                test[0] = temp;
                                match[0] = dir_object->d_name;
                            }
                            temp = tst.ngramTest(databaseTokens,targetTokens);
                            if(test[1] < temp) {
                                test[1] = temp;
                                match[1] = dir_object->d_name;
                            }
                            temp = tst.cosineTest(databaseTokens,targetTokens,mGlobals.stopwords_file);
                            if(test[2] < temp) {
                                test[2] = temp;
                                match[2] = dir_object->d_name;
                            }

                        }
                    closedir (dirB);
                }
                cout<<"Test 1 score: "<<fixed<<setprecision(mGlobals.score_accuracy)<<test[0]<<"/10"<<endl; 
                cout<<"Test 2 score: "<<fixed<<setprecision(mGlobals.score_accuracy)<<test[1]<<"/10"<<endl;
                cout<<"Test 3 score: "<<fixed<<setprecision(mGlobals.score_accuracy)<<test[2]<<"/10"<<endl;

                v.getVerdict(test, match);

              cout<<"\n";
            }
                
        closedir (dir);
    
    return 0;
    }
    else{

    perror("No Target Folder");

return -1;
}

}
else{

perror("provide the score accuracy");
    return -1;
}

}