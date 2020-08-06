/*
 * fibvect_test.cxx
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
#include <iomanip>
#include <array>
#include <vector>
#include <cmath>

const int F = 46;

const int W = F+1;
const int H = F+1;
const int modulus = 1000000007;

// -----Global variables-----

typedef std::array<int, 3> Fibgroup;
typedef std::array<int, 2> Coord;
typedef std::vector<Fibgroup> Fibvect;

typedef struct {
	Coord ident; // debug ?
	int paths;
	std::vector<Coord> steps;	
} Node;

std::array<std::array<int,W>,H> lattice;
std::vector<Coord> rect_step;
std::vector<Coord> pyth_step;
Fibvect fibonacci;
bool path_found = false;

// -----Declarations-----

void prt_node(Node &n);
void prt_lattice(std::array<std::array<Node, W>, H> &l);
void construct_fibvect(Fibvect &fv, const int F);
void prt_fibvect(Fibvect &fv);

// -----Definitions------

void prt_node(Node &n) {
	// ident & count
	std::cout << "(" << n.ident[0] << "," << n.ident[1] << ")  Paths:" << n.paths << std::endl;
	// steps vector
	std::cout << "Step vector ";
	for(auto it = n.steps.begin(); it != n.steps.end(); ++it)
		std::cout << "(" << (*it)[0] << "," << (*it)[1] << ")  ";
	std::cout << "\nSteps: " << n.steps.size() << std::endl;
}

void prt_lattice(std::array<std::array<Node, W>, H> &l) {
	// Top-down print of lattice
	for(auto row = H-1; row >= 0; --row) {
		for(auto col = 0; col < W; ++col) {
			prt_node(l[col][row]);
			std::cout << std::endl;
		}
		std::cout << "-----" << std::endl;
	}
}

void construct_fibvect(Fibvect &fv, const int F) {
	fv.clear();
	fv.push_back( {0,0,0 } ); // F(0)
	fv.push_back( {1,0,0 } ); // F(1)
	fv.push_back( {1,0,0 } ); // F(2)
	fv.push_back( {2,0,0 } ); // F(3)
	fv.push_back( {3,0,0 } ); // F(4)
	fv.push_back( {5,3,4 } ); // F(5)
	// The value of the maximum Fibonnaci is derived from the dimensions of the lattice
	// Fmax >= sqrt(W*W + H*H)

	int current_fibonacci = (*(fv.end()-1))[0];
	
	while(current_fibonacci <= F) {
		// add non-pythag
		current_fibonacci = ((*(fv.end()-1))[0] + (*(fv.end()-2))[0]);
		fv.push_back({current_fibonacci,0,0});
		
		// add pythag
		current_fibonacci = ((*(fv.end()-1))[0] + (*(fv.end()-2))[0]);
		fv.push_back({current_fibonacci,0,0});
		(*(fv.end()-1))[1] = (*(fv.end()-2))[0] - (*(fv.end()-3))[1];
		(*(fv.end()-1))[2] = (*(fv.end()-3))[0] + (*(fv.end()-3))[1] + (*(fv.end()-3))[2];
		
	}
	// Remove unused entries
	fv.erase(fv.begin(), fv.begin()+2);
}

void prt_fibvect(Fibvect &fv) {
	
	for(auto it = fv.begin(); it != fv.end(); ++it) 
		std::cout << (*it)[0] << " / "<< (*it)[1] << " / "<< (*it)[2] << std::endl;
	std::cout << std::endl;
}

//----------------------------------------------------------------------

int main(int argc, char **argv) {
	
	
	construct_fibvect(fibonacci, F);	
	prt_fibvect(fibonacci);
	
	// from fibonnaci setup a vector of Coords which contains all legal steps for this lattice
	
	std::vector<Coord> legal_steps;
	for(auto it_fib = fibonacci.begin(); it_fib != fibonacci.end(); ++it_fib) {
		// first value has rect. step
		legal_steps.push_back( { (*it_fib)[0], 0 } ); // -dw
		
		legal_steps.push_back( { 0, (*it_fib)[0] } ); // -dh
		
		// if available add 2 diagonal steps
		if((*it_fib)[1] == 0) continue;
		legal_steps.push_back( { (*it_fib)[1], (*it_fib)[2] } ); // -dw -dh
		
		legal_steps.push_back( { (*it_fib)[2], (*it_fib)[1] } ); // -dh -dw
	}
	
	std::cout << "F(" << F << "," << F << ")  ";
		
	std::cout << "legal_steps size: " << legal_steps.size() << std::endl;
	

	// phase 2 scan starting at node(0,0) compute path counts
	bool root_node = true;
	for(int h = 0; h != H; ++h) {
		for(int w = 0; w != W; ++w) {
			int &node = lattice[w][h];
			if(root_node) {
				root_node = false;
				node = 1;
			} else {
				node = 0;
				for(auto s = legal_steps.begin(); s != legal_steps.end(); ++s) {
					int dw = w - (*s)[0];
					int dh = h - (*s)[1];
					if((dw >= 0)and(dh >= 0))
					node = (node + lattice[dw][dh]) % modulus;
				} // for s...
			} // else			
		} // for w...
	} // for h...
	
	//prt_lattice(lattice);
	std::cout << "F(" << F << "," << F << ")  ";
	std::cout << "Total path count: " << lattice[F][F] << std::endl;			
	
}
	
