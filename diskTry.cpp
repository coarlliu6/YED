/*
  This file is part of YED.

  Copyright (C) 2014 Yuhui Zhang. 

  YED is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include "./basis/basis.h"
#include "./interaction/interaction.h"
#include "./matrix/matrix.h"
#include "./ED/ED.h"
#include <cmath> // sqrt
#include <gsl/gsl_math.h> // pi
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <iomanip>  //std::setwi
#include <sstream>
std::string nameFileTool(std::string, int);
std::string nameFileTool(std::string, double);

void parameter_asg(char& geometry, int& n1, int& n2, int& nOrb, double& qn, int& wn);

int main()
{
  using namespace std;
  cout << "==============================================================" << endl;
  cout << "=========================== YED ==============================" << endl;
  cout << "==============================================================" << endl;
  const double timeDivider = 60.0 * 60;
  cout << "Feb. 4 version:" << endl;
  clock_t timeStart;
  timeStart = clock();
  cout << "start time: " << (double)(timeStart)/CLOCKS_PER_SEC/timeDivider << "hr" << endl;
  int nOrb =0;
  int s = 0; // this parameter is for torus, no use here
  // basis:
  int n1 =0, n2=0; // if it just has one component, make n1 = 0
  double  qn=0, qn_org=0;   // total angular momentum 
  int  wn;
  double Q = 0; // B field stregth for monople at origin in sphere case.
  int dimbasis;
  char geometry;
  int ip = 0;
   
   cout << "1) input parameters by hands; 2) automatically input parameters?" << endl;
   cin >> ip;

   switch(ip)
     {
       case 1:
              cout << "Geometry? [D(isk), S(phere)]" << endl;
              cin >> geometry;
              cout <<"Orbital number(for S: 2Q+1) = " << endl;
              cin >> nOrb;  
              cout << "Particle number(n1, default=0) = " << endl;
              cin >> n1;
              cout << "Particle number(n2) = " << endl;
              cin >> n2;
              cout << "Total angular momentum = " << endl;
              cin >> qn;
              cout << "The number of lowest energies do you want = " << endl;
              cin >> wn;
              break;

       case 2:
              parameter_asg(geometry, n1, n2, nOrb, qn, wn);
              break;
     }
 // cout << "geometry, nOrb, n1, n2, qn, wn =  " << geometry << " " << nOrb << " " << n1 << " " << n2 << " " << qn << " " << wn << endl;
  qn_org = qn;

  if (geometry == 'S')
    { 
      Q = (double)(nOrb - 1)/2;
      qn_org += (n1 + n2) * Q;
    }  
 
  basis bases(n1, n2, nOrb, qn_org, s, geometry);
  dimbasis = bases.bGen();
 // cout <<  "dimbasis = "  << dimbasis << endl;
  clock_t timeBasis;
  timeBasis = clock();
  cout << "basis finished time: " << (double)(timeBasis)/CLOCKS_PER_SEC/timeDivider << "hr" << endl;
//  bases.print();
  bases.record();
  if (wn < dimbasis)  // due to wn < dim
    {
       cout << "wn = " << wn << endl;
    }
  else 
    { wn = dimbasis - 1;
      cout << "wn >= dim, new wn = dim-1 =" << wn << endl;
    }

 // intra-layer interaction:  
  double a = 0, b = 0, // for torus, no use here
         c0 = 0, c1 = 1,  // for hard core interaction(fake Haldane interaction), 0th and 1st orders, respectively. The values are assign by us. 
         param[4] = {a, b, c0, c1};  
  int LLn = 0;
  char type[3];
       type[0]= geometry;
       type[1]='C';
   
  interaction itrA(nOrb, LLn, param, type, Q);
  itrA.itrGen();
  // inter-layer interaction:
  c0 = 0; c1 = 1;
  param[2] = c0; param[3] = c1;
  interaction itrR(nOrb, LLn, param, type, Q);
  itrR.itrGen();

  cout<<"finish interaction"<<endl;
 
 // generate matrix
  char sizeCountType = 'C';
  char matrixType = 'l'; // w: whole; l: low part
  matrix m(bases, itrA, itrR, matrixType, sizeCountType, qn_org, s);
  m.mGen();
 // m.print();

 cout<< "finish matrix generation"<<endl;

  // ED
  ED ed(m, wn);
  ed.arpackpp();
  ed.terminalOP();
  //ed.terminalVectorOP();
  cout << "==============================================================" << endl;
  cout << "==============================================================" << endl;
}

void parameter_asg(char& geometry, int& n1, int& n2, int& nOrb, double& qn, int& wn)
{
   using namespace std;
   
   char useless[20];
   char chr;

   string line;
   istringstream in;
   ifstream fin;
   fin.open ("parameter.txt");
   if(!fin)
     { cout << "Fail to open parameter.txt" << endl;}

   fin.getline(useless, 20, ' ');
   fin >> geometry;
   fin.get(chr);
   fin.getline(useless, 20, ' ');
   fin >> nOrb;
   fin.getline(useless, 20, ' ');
   fin >> n1;
   fin.getline(useless, 20, ' ');
   fin >> n2;
   fin.getline(useless, 20 , ' ');
   fin >> qn;
   fin.getline(useless, 20, ' ');
   fin >> wn;
 
   fin.close();
}

std::string nameFileTool(std::string p, int pn)
{
  using namespace std;
  string output = p;
  string temp;
  ostringstream convert;
  convert << pn;
  temp = convert.str();
  output += temp;
  convert.str("");
  return output;
}

std::string nameFileTool(std::string p, double number)
{
  using namespace std;
  string output = p;
  ostringstream strs;
  strs << number;
  string n = strs.str();
  output += n;
  return output;
}
