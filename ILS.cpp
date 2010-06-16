#include "ILS.h"

void ILS(vector<vector<int>*> &cgroups, 
         vector<int> &rlenght, vector<int> &lot_s,
         vector<int> &lpiece, vector<int> &dpiece,
         vector<int> &leftover,vector<int> &used_rolls,
         vector<int> &rollType, vector<int> &variety) {

  int maxit = ILS_MAXIT;

  vector<vector<int>*> S_star;
  vector<vector<int>*> S_pert;
  vector<int> leftover_star;
  vector<int> leftover_pert;
  vector<int> used_rolls_star;
  vector<int> used_rolls_pert;
  vector<int> variety_star;
  vector<int> variety_pert;

  int nitems = lpiece.size();
  int sum;
  int max_pert = MAX_PERTURB;
  double T = INIT_TEMP;

  vector<vector<int>*> initSol = genInitSol(rlenght,lpiece,
                                            dpiece,leftover,
                                            used_rolls,rollType);

  localSearchBB(initSol,rlenght,lot_s,
		lpiece,dpiece,leftover,
		used_rolls,variety);
  
  S_star = initSol;//Para este punto, initSol a sido procesada

  leftover_star = leftover;
  used_rolls_star = used_rolls;
  variety_star = variety;

  srand(time(NULL));

  while (maxit > 0) {
    cout << maxit <<"\n";
    leftover_pert = leftover_star;
    used_rolls_pert = used_rolls_star;
    variety_pert = variety_star;
    
    S_pert = duplicate(S_star,nitems);
    
    Perturb(S_pert,
	    leftover_pert,
	    used_rolls_pert,
	    variety_pert,
	    rlenght,
	    lpiece,
	    lot_s,
	    MAX_PERTURB);
    
    localSearchBB(S_pert, 
		  rlenght, lot_s,
                  lpiece, dpiece,
		  leftover_pert,used_rolls_pert,
		  variety_pert);

    sum = 0;
    int tleft1 = 0;
    int leftoverS1_size = leftover.size();
    for(int i = 0; i < leftoverS1_size; i++) 
      sum += leftover_star[i];
    tleft1 = sum;

    sum = 0;
    int tleft2;
    int leftoverS2_size = leftover_pert.size();
    for(int i = 0; i < leftoverS2_size; i++) 
      sum += leftover_pert[i];
    tleft2 = sum;

    sum = 0;
    int tleft_best;
    int leftover_best_size = leftover.size();
    for(int i = 0; i < leftover_best_size; i++) 
      sum += leftover[i];
    tleft_best = sum;

    if (tleft_best > tleft2) {
      //cout << tleft2 << "\n";
      leftover = leftover_pert;
      used_rolls = used_rolls_pert;
      variety = variety_pert;
      overwrite(S_pert,cgroups,nitems);
    }

    if (tleft1 > tleft2) {
      //cout << tleft2 << "\n";
      leftover_star = leftover_pert;
      used_rolls_star = used_rolls_pert;
      variety_star = variety_pert;
      overwrite(S_pert,S_star,nitems);
    }
    else { // Se elige con probabilidad Simulated Annealing
      double p = exp(((double)(tleft1 - tleft2)) / T);
      double r = (double)random()/(double)RAND_MAX;
      // cout << "==\n";
      // cout << tleft1 << "<-tleft1\n";
      // cout << tleft2 << "<-tleft2\n";
      // cout << T << "<-T\n";
      // cout << (double)p <<"<-p\n";
      // cout << (double)r <<"<-r\n";
      // cout << "==\n";
      if (r < p) {
	leftover_star = leftover_pert;
	used_rolls_star = used_rolls_pert;
	variety_star = variety_pert;
	overwrite(S_pert,S_star,nitems);
      }
    }

    T *= EXP_DESC; 
    maxit--;
  }

  int i;
  int j;
  int totalLO = 0;
  for(i = 0; i < lpiece.size(); i++) {
    sum = 0;
    if (used_rolls[i]) {
      // cout << "Tipo " << i << endl;
      // cout << "rolls " << used_rolls[i] << endl;
      // cout << "leftover " << leftover[i] << endl;
      totalLO += leftover[i];
      // for(j=0;j<lpiece.size();++j)
      // 	sum += (*cgroups[i])[j];
      // cout << "n_used_pieces " << sum << endl; 
      // cout << "------------" << endl;
    }
  }
  cout << totalLO<<"\n";

}


// bool criteria(vector<vector<int>*> s1, 
//               vector<int> leftoverS1,
//               vector<int> leftoverS2,
//               vector<vector<int>*> s2) {
//   int promS1;
//   int sum = 0;
//   int leftoverS1_size = leftoverS1.size();
//   // Promedio leftoverS1
//   for(int i = 0; i < leftoverS1_size; i++) 
//     sum += leftoverS1[i];
//   promS1 = sum / leftoverS1_size;
//   // Promedio de leftoverS2
//   sum = 0;
//   int leftoverS2_size = leftoverS2.size();
//   int promS2;
//   for(int i = 0; i < leftoverS2_size; i++) 
//     sum += leftoverS2[i];
//   promS2 = sum / leftoverS2_size;
  
//   // Comparación de leftover
//   if (promS1 > promS2) return true;
//   else return false;
// }

  
  
