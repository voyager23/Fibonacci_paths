/*
 * radeye.cxx
 * 
 * Copyright 2020 mike <mike@Intel-i7>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */



#include <iostream>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <iomanip>
using namespace std ;
typedef long long ll ;
const ll MAX = 10001 ;
const int MOD = 1000000007 ;
char isfib[4*MAX] ;
ll *canreach[2*MAX] ;
int main(int argc, char *argv[]) {
   int w = atol(argv[1]) ;
   int h = atol(argv[2]) ;
   int a = 0 ;
   int b = 1 ;
   while (b <= w+h) {
      isfib[b] = 1 ;
      int c = a + b ;
      a = b ;
      b = c ;
   }
   vector<pair<int, int> > v ;
   for (int x=0; x<=w; x++) {
      for (int y=0; y<=h; y++) {
         int d = x * x + y * y ;
         int s = sqrt(d) ;
         if (s * s == d && isfib[s])
            v.push_back({x, y}) ;
      }
   }
   
   for (int i=0; i<=w+h; i++)
      canreach[i] = (ll *)calloc(w+1, sizeof(ll)) ;
   canreach[0][0] = 1 ;
   for (int s=0; s<=w+h; s++) {
      int xmin = max(0, s-h) ;
      int xmax = min(w, s) ;
      for (auto p:v) {
         int s0 = s - p.first - p.second ;
         if (s0 < 0)
            continue ;
         int xmin2 = max(xmin, p.first) ;
         for (int x=xmin2; x<=xmax; x++)
            canreach[s][x] += canreach[s0][x-p.first] ;
      }
      for (int x=xmin; x<=xmax; x++)
         canreach[s][x] %= MOD ;
   }
   cout << canreach[w+h][w] << endl ;
}


