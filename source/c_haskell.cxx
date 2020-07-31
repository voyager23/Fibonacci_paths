/*
 * c_haskell.cxx
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

typedef std::array<int, 3> Fibgroup;
typedef std::array<int, 2> Coord;
typedef std::vector<Fibgroup> Fibvect;


const int S = 5 ;

const int W = S+1;
const int H = S+1;
const int MinFibonacci = 10000;
const int modulus = 1000000007;
//const int modulus = 17;

// macro to swap w and h values
#define SWAPWH(coord) coord[0] = coord[0] xor coord[1]; coord[1] = coord[1] xor coord[0]; coord[0] = coord[0] xor coord[1];

// -----Global variables-----
std::array<std::array<unsigned, W>, H> lattice;
std::vector<Coord> rect_step;
std::vector<Coord> pyth_step;
Fibvect fibonacci;

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
	
	int current_fibonacci = 0;
	do {
		current_fibonacci = (*(fv.end()-1))[0] + (*(fv.end()-2))[0];
		fv.push_back({current_fibonacci,0,0});
	} while (current_fibonacci < MinFibonacci);
	// starting at F(5) value 5, insert the pythag triples
	int idx = 5;
	fv.at(idx) = {5,3,4};
	idx +=2;
	do {
		fv.at(idx)[1] = fv.at(idx-1)[0] - fv.at(idx-2)[1];
		fv.at(idx)[2] = fv.at(idx-2)[0] + fv.at(idx-2)[1] + fv.at(idx-2)[2];	
		idx += 2;
	} while(idx < fv.size());
}

void construct_rect_step(std::vector<Coord> &rs, Fibvect &fv);
void construct_rect_step(std::vector<Coord> &rs, Fibvect &fv) {
	// First value will be F(2) = 1
	rs.clear();
	for(auto it_fv = fv.begin()+2; it_fv != fv.end(); ++it_fv) 
		rs.push_back({(*it_fv)[0],0});	
}

void construct_pyth_step(std::vector<Coord> &ps, Fibvect &fv);
void construct_pyth_step(std::vector<Coord> &ps, Fibvect &fv) {
	// First value will be F(5) = 5,3,4
	ps.clear();
	for(auto it_fv = fv.begin()+5; it_fv < fv.end(); it_fv += 2)
		ps.push_back( {(*it_fv)[1], (*it_fv)[2]} );	
}

// -----Main Code-----
int main(int argc, char **argv)
{
	construct_fibvect(fibonacci);
	construct_rect_step(rect_step, fibonacci);
	construct_pyth_step(pyth_step, fibonacci);
	
	// Setup and reflect values in column 0;
	// w = 0;
	lattice[0][0] = 1;
	for(auto h = 1; h < H; ++h) {
		lattice[0][h] = 0;
		for(auto it_rs = rect_step.begin(); it_rs != rect_step.end(); ++it_rs) {
			int step = (*it_rs)[0];
			Coord target = { 0, (h - step) };
			if((h - step) >= 0) {
				lattice[0][h] += lattice[0][target[1]];
				lattice[0][h] %= modulus;
				// reflect to horizontal axis
				lattice[h][0] = lattice[0][h];
			} else {
				break;
			}
		}			
	}

	// set the corner point
	int corner = 1; 
	//while( corner <= S)		
		// do horizontal line, rect and diagonal
		// do vertical line, rect and diagonal
		// do corner, rect and diagonal
		// increment corner
	// end while
	
	while(corner <= S) {
		for(auto n = 1; n < corner; ++n) {
			// process lattice[n][corner] rect count + diagonal count
			// process lattice[corner][n] rect count + diagonal count
			// ------------------------------------------------------
			// process lattice[corner][corner] rect_count + diagonal count
			// increment corner
			lattice[n][corner] = 0;
			lattice[corner][n] = 0;
			// possible rect steps
			for(auto it_rs = rect_step.begin(); it_rs != rect_step.end(); ++it_rs) {
				int step = n - (*it_rs)[0];
				if(step > 0) {
					lattice[n][corner] += lattice[step][corner];
					lattice[n][corner] %= modulus;
					
					lattice[corner][n] += lattice[corner][step];
					lattice[corner][n] %= modulus;
				}
			}
			// possible diagonal steps
			int wx = 0;	int hx = 0; int temp = 0;
			for(auto it_py = pyth_step.begin(); it_py != pyth_step.end(); ++it_py) {
				bool path_found = false;
				wx = n - (*it_py)[0];
				hx = corner - (*it_py)[1];	// path 1
				if((wx>=0)and(hx>=0)) {
					lattice[n][corner] += lattice[wx][hx];
					lattice[n][corner] %= modulus;
					path_found = true;
				}				
				// switch values of wx and hx
				temp = wx;	wx = hx;  hx = temp;
				if((wx>=0)and(hx>=0)) {
					lattice[n][corner] += lattice[wx][hx];
					lattice[n][corner] %= modulus;
					path_found = true;
				}
							
				wx = n - (*it_py)[1];
				hx = corner - (*it_py)[0];	// path 2
				if((wx>=0)and(hx>=0)) {
					lattice[n][corner] += lattice[wx][hx];
					lattice[n][corner] %= modulus;
					path_found = true;
				}
				
				// switch values of wx and hx
				temp = wx;	wx = hx;  hx = temp;
				if((wx>=0)and(hx>=0)) {
					lattice[n][corner] += lattice[wx][hx];
					lattice[n][corner] %= modulus;
					path_found = true;
				}			
				if(!path_found) break;
			}		
		}
		// now process corner
		lattice[corner][corner] = 0;
		for(auto it_rs = rect_step.begin(); it_rs != rect_step.end(); ++it_rs) {
			int step = corner - (*it_rs)[0];
			if(step > 0) {
				lattice[corner][corner] += lattice[step][corner];
				lattice[corner][corner] %= modulus;
				
				lattice[corner][corner] += lattice[corner][step];
				lattice[corner][corner] %= modulus;
			}
		}		
		// ------------------
		corner += 1;
	} // while (corner <= S)
				
	prt_lattice(lattice);
	std::cout << "F(" << S << "," << S << ") = " << lattice[S][S] << std::endl;
	return 0;
}

