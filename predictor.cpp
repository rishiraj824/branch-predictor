#include <iostream>
#include <fstream>
#include <math.h>
#include <bitset>
#include <vector>
#include <stdlib.h>     /* strtoull */

//state:
#define SN "00" // Strong Not Taken
#define WN "01" // Weakly Not Taken
#define WT "10" // Weakly Taken
#define ST "11" // Strong Taken

using namespace std;


static inline long get_last_n_bits(long num, int nbbits) {
   return num & ((1L<<nbbits)-1);
}

int main (int argc, char** argv) {
	ifstream config;
	config.open(argv[1]);
	int m, k;
	config >> m >> k;

    int z = pow(2,m);
    string pht[z][k];
    string bhr;
    // initialize a bhr with k length
    for(int i=0;i<k;i++){
        bhr += '1';
    }
    // initialize everything in pht to 11
    for(int i=0;i<z;i++){
        for(int j=0;j<k;j++){
            pht[i][j] = ST;
        }
    }

	config.close();

	ofstream out;
	string out_file_name = string(argv[2]) + ".out";
	out.open(out_file_name.c_str());
	
	ifstream trace;
	trace.open(argv[2]);
    
	while (!trace.eof()) {
		unsigned long pc; bool taken;
		trace >> std::hex >> pc >> taken;
        int val = get_last_n_bits(pc, m); // last m bits in decimal
        int bhrval = strtoull(bhr.c_str(),NULL,2);
        
        bool prediction;
        
        bhrval = strtoull(bhr.c_str(),NULL,2);
        
        // updating pht(pattern history table)
        if(taken){
            if(pht[val][bhrval]==ST||pht[val][bhrval]==WT){
                pht[val][bhrval]=ST;
                prediction=true;
            }
            else if(pht[val][bhrval]==WN){
                pht[val][bhrval]=ST;
                prediction=false;
            }
            else if(pht[val][bhrval]==SN){
                pht[val][bhrval]=WN;
                prediction=false;
            }
        }
        else {
            if(pht[val][bhrval]==WT){
                pht[val][bhrval]=SN;
                prediction=true;
            }
            else if(pht[val][bhrval]==SN||pht[val][bhrval]==WN){
                pht[val][bhrval]=SN;
                prediction=false;
            }
            else if(pht[val][bhrval]==ST){
                pht[val][bhrval]=WT;
                prediction=true;
            }
        }
        
        // updating bhr(branch history register)        
        for(int i=k-1;i>0;i--){              
            bhr[i]=bhr[i-1];
        }
        if(k>1){
              if(taken){
                    bhr[0] = '1'; 
                }
                else {
                    bhr[0] = '0';
                } 
        }
		out << prediction << endl;
	}
	 
	trace.close();	
	out.close();
}
