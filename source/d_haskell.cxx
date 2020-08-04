/*
 * d_haskell.cxx
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
 * Solution for Fibonacci paths in lattice. Drawn from notes to
 * projecteuler 662
 * Here are some other results:

		Limit      Result      Time
		-----------------------------
		 1000    790802713    0.3 sec
		 5000    126968109    8.8 sec
		10000    860873428   42.3 sec
		20000    893422979  205.8 sec
		30000    451632021  509.2 sec
		40000    264938001  910.5 sec
		50000    395017546 1477.0 sec
		60000    156903036 2164.8 sec
		63000    543864149 2440.1 sec

		Execution time is just under 43 seconds.
 * 		F(12,12) 1912692520 exhaustive search
 */


#include <iostream>
#include <iomanip>
#include <array>
#include <vector>
#include <cmath>

typedef std::array<int, 3> Fibgroup;
typedef std::array<int, 2> Coord;
typedef std::vector<Fibgroup> Fibvect;


const int S = 48 ;

const int W = S+1;
const int H = S+1;
const int MinLeg = S; // patch code to get triangle legs > 10000
const int modulus = 1000000007;

// -----Global variables-----
std::array<std::array<unsigned,W>,H> lattice;
std::vector<Coord> rect_step;
std::vector<Coord> pyth_step;
Fibvect fibonacci;
bool path_found = false;

// -----Definitions-----
void prt_lattice(std::array<std::array<unsigned, W>, H> &l);
void prt_lattice(std::array<std::array<unsigned, W>, H> &l) {
	// Top-down print of lattice
	for(auto row = H-1; row >= 0; --row) {
		for(auto col = 0; col != W; ++col) std::cout << std::setw(10) << lattice[col][row] << " ";
		std::cout << std::endl;
	}
}

void construct_fibvect(Fibvect &fv);
void construct_fibvect(Fibvect &fv) {
	fv.clear();
	fv.push_back( {0,0,0 } ); // F(0)
	fv.push_back( {1,0,0 } ); // F(1)
	fv.push_back( {1,0,0 } ); // F(2)
	fv.push_back( {2,0,0 } ); // F(3)
	fv.push_back( {3,0,0 } ); // F(4)
	fv.push_back( {5,3,4 } ); // F(4)
	// The value of the maximum Fibonnaci is derived from the dimensions of the lattice
	// Fmax >= sqrt(W*W + H*H)
	int fmax = (int)sqrt((W*W) + (H*H));
	int limit = 0;
	int current_fibonacci = 0;
	do {
		// add non-pythag
		current_fibonacci = ((*(fv.end()-1))[0] + (*(fv.end()-2))[0]);
		fv.push_back({current_fibonacci,0,0});
		
		// add pythag
		current_fibonacci = ((*(fv.end()-1))[0] + (*(fv.end()-2))[0]);
		fv.push_back({current_fibonacci,0,0});
		(*(fv.end()-1))[1] = (*(fv.end()-2))[0] - (*(fv.end()-3))[1];
		(*(fv.end()-1))[2] = (*(fv.end()-3))[0] + (*(fv.end()-3))[1] + (*(fv.end()-3))[2];
		
		limit =	(*(fv.end()-1))[1];				
	} while (limit < MinLeg);
}

// -----Main Code-----
int main(int argc, char **argv)
{
	int wx, hx, dw, dh, delta;
	int w = 0;
	int h = 0;
	
	construct_fibvect(fibonacci);
	
	// initialise the vertical axis
	lattice [0][0] = 1;
	for(auto h = 1; h != H; ++h) {
			for(auto it_path = fibonacci.begin() + 2; it_path != fibonacci.end(); ++it_path) {
				path_found = false;
				hx = h - (*it_path)[0];
				if(hx >= 0) {
					lattice[0][h] += lattice[0][hx];
					lattice[0][h] %= modulus;
					lattice[h][0] = lattice[0][h];
					path_found = true;
				}
				if(!path_found) break;
			}
		}
	// main lattice			
	for(auto w = 1; w != W; ++w) {
		for(auto h = 1; h != H; ++h) {
			lattice[w][h] = 0;
			for(auto it_path = fibonacci.begin() + 2; it_path != fibonacci.end(); ++it_path) {
				path_found = false;
				// do any W steps - update path_found 
				delta= (*it_path)[0];
				if((w - delta) >= 0) { // h must be valid
					lattice[w][h] += lattice[w - delta][h];
					lattice[w][h] %= modulus;
					path_found = true;
				}
				
				// do any S steps - update path_found
				if((h - delta) >= 0) { // w must be valid
					lattice[w][h] += lattice[w][h - delta];
					lattice[w][h] %= modulus;
					path_found = true;
				}
				if(!path_found) break;
			}
			
			for(auto it_path = fibonacci.begin() + 5; it_path != fibonacci.end(); it_path += 2) {					
				// do any D steps - update path_found
				path_found = false;
				dw = (*it_path)[1];
				dh = (*it_path)[2];
				wx = w - dw;
				hx = h - dh;
				if((wx >= 0)and(hx >= 0)) {
					lattice[w][h] += lattice[wx][hx];
					lattice[w][h] %= modulus;
					path_found = true;
				}
				wx = w - dh;
				hx = h - dw;
				if((wx >= 0)and(hx >= 0)) {
					lattice[w][h] += lattice[wx][hx];
					lattice[w][h] %= modulus;
					path_found = true;
				}
							
				if(!path_found) break;
			}
								
		} // for h...
	} // for w...
	
	

	//prt_lattice(lattice);
	std::cout << "\tF(" << S << "," << S << ") = " << lattice[S][S] << std::endl;
	return 0;
}

