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

typedef std::array<int, 3> Fibgroup;
typedef std::array<int, 2> Coord;
typedef std::vector<Fibgroup> Fibvect;

typedef struct {
	int count;
	std::vector<Coord> steps;
	
	Coord ident; // debug 
} Node;


const int S = 48 ;

const int W = S+1;
const int H = S+1;
const int MinLeg = S; // patch code to get triangle legs > 10000
const int modulus = 1000000007;

// -----Global variables-----
std::array<std::array<int,W>,H> lattice;
std::vector<Coord> rect_step;
std::vector<Coord> pyth_step;
Fibvect fibonacci;
bool path_found = false;

// -----Definitions-----
void prt_lattice(std::array<std::array<int, W>, H> &l);
void prt_lattice(std::array<std::array<int, W>, H> &l) {
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
	fv.push_back( {5,3,4 } ); // F(5)
	// The value of the maximum Fibonnaci is derived from the dimensions of the lattice
	// Fmax >= sqrt(W*W + H*H)
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

void prt_fibvect(Fibvect &fv);
void prt_fibvect(Fibvect &fv) {
	
	for(auto it = fv.begin(); it != fv.end(); ++it) 
		std::cout << (*it)[0] << " / "<< (*it)[1] << " / "<< (*it)[2] << std::endl;
	std::cout << std::endl;
}

void prt_node(Node &n);
void prt_node(Node &n) {
	// ident & count
	std::cout << "(" << n.ident[0] << "," << n.ident[1] << ")  count:" << n.count << std::endl;
	// steps vector
	std::cout << "Step vector\n";
	for(auto it = n.steps.begin(); it != n.steps.end(); ++it)
		std::cout << "(" << (*it)[0] << "," << (*it)[1] << ")" << std::endl;
}
		
		

int main(int argc, char **argv) {
	
	int dw,dh;
	
	construct_fibvect(fibonacci);
	
	prt_fibvect(fibonacci);
	
	Node test_node;
	
	test_node.ident = {47,47};
	test_node.count = 0;
	test_node.steps.clear();
	
	for(auto it_step = fibonacci.begin() + 2; it_step != fibonacci.end(); ++it_step) {
		bool step_found = false;
		// check for left step
		dw = test_node.ident[0] - (*it_step)[0];
		if(dw >= 0) {
			test_node.steps.push_back({dw, test_node.ident[1]});
			step_found = true;
		}
		
		// check for down step
		dh = test_node.ident[1] - (*it_step)[0];
		if(dh >= 0) {
			test_node.steps.push_back({test_node.ident[0], dh});
			step_found = true;
		}
			
		// check for (2) diag. steps
		if((*it_step)[1] > 0) {
			dw = test_node.ident[0] - (*it_step)[1];
			dh = test_node.ident[1] - (*it_step)[2];
			if((dw >= 0)and(dh >= 0)) {
				test_node.steps.push_back({-dw, -dh});
				step_found = true;
			}		
			dw = test_node.ident[0] - (*it_step)[2];
			dh = test_node.ident[1] - (*it_step)[1];
			if((dw >= 0)and(dh >= 0)) {
				test_node.steps.push_back({-dw, -dh});
				step_found = true;
			}
		}
						
		if(!step_found) break;
		
	}
	
	// print the node contents for analysis
	prt_node(test_node);
	
}
	
