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

#include "basis.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <complex>

basis::basis(int n1_, int n2_, int nOrb_, double qn_, int s_, char type_)
{
  n1 = n1_;
  n2 = n2_;
  nOrb = nOrb_;
  qn = qn_;
  s = s_;
  type = type_; 
}

int basis::bGen()
{
  switch(type)
    {
    case 'D': dGen();
      break;
    case 'T': tGen();
      break;
    case 'S': dGen();
      break;
    }
  std::cout << "dim = " << dim << std::endl;
  return dim;
}

void basis::print()
{
  using namespace std;
  int i;
  cout << "The generated bases are:" << endl;
  cout << "n1 = " << n1 << ", n2 = " << n2 << ", nOrb = " << nOrb << ", qn = "; 

  cout << "------ from the large orbital to the small ------" << endl;
  for (i = 0; i < dim; i++)
    {
      cout << bases[i]
	   << " "
	   << i << ": ";
      if (type == 'T')
	cout << tStSetNo[i] << " -> ";
      deciToPBShow(bases[i]);
      cout <<endl;
    }
  cout << "dim = " << dim << endl;
}

void basis::record()
{
  using namespace std;
  int i; 
  int deciShift = (int) (pow(2, nOrb) + 0.5);
  int dec1, dec2;
  int dec;
  int orb = nOrb - 1;  

  std::ofstream fout("data.txt");
 
  fout << "n1 =" << n1 << " " << "n2 =" << n2 << " " << "nOrb =" << nOrb << " " << "qn =" << qn << endl;
  fout << "bases dim = " << dim << endl;   
  fout << "bases are = " << endl;
  fout << "num" << "  " << "deci num" << "  " << "dec1" << "    " << "dec2" << "  " << endl;
  
  for(i = 0; i < dim; i++ )
     {
        dec =bases[i];
        dec1 = dec /deciShift;
        dec2 = dec % deciShift;
        
        fout << i+1 <<  "      " << bases[i] <<  "      " << dec1 << "      " << dec2 << "      ";
       
        do 
          { 
            switch (dec1 % 2)
               {
                 case 1: 
                    if (type == 'T')
                      { fout << orb + 1 << " "; }
                    else
                      { fout << orb << " "; }
                   
                    orb--;
                    break;
                 case 0:
                    orb--;
                    break;
               }
           dec1 = dec1/2;
          } while (orb >=0);
       fout << "||";
       orb = nOrb -1;
       do
         {
           switch (dec2 % 2)
             {
                case 1:
                       if (type == 'T')
                          { fout << orb + 1 << " ";}
                       else 
                          { fout << orb << " ";}
                       orb--;
                       break;
                case 0:
                       orb--;
                       break; 
             }
           dec2 = dec2/2;
         } while (orb >= 0);       
        
        fout << "\n" ;
      }
 
  
  fout.close();
}
