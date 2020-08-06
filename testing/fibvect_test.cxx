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

const int F = 10000;

const int W = F+1;
const int H = F+1;
const int MinLeg = F; // patch code to get triangle legs > 10000
const long modulus = 1000000007;

// -----Global variables-----

typedef std::array<int, 3> Fibgroup;
typedef std::array<int, 2> Coord;
typedef std::vector<Fibgroup> Fibvect;

typedef struct {
	Coord ident; // debug ?
	long paths;
	std::vector<Coord> steps;	
} Node;

std::array<std::array<Node,W>,H> lattice;
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
	fv.erase(fv.begin(), fv.begin()+2);
}

void prt_fibvect(Fibvect &fv) {
	
	for(auto it = fv.begin(); it != fv.end(); ++it) 
		std::cout << (*it)[0] << " / "<< (*it)[1] << " / "<< (*it)[2] << std::endl;
	std::cout << std::endl;
}

//----------------------------------------------------------------------

int main(int argc, char **argv) {
	
	int dw,dh;	
	// the lattice is defined in terms of W and H
	
	construct_fibvect(fibonacci, F);	
	prt_fibvect(fibonacci);
	
	exit(0);//DEBUG EXIT
	
	// Initialise the lattice axes
	
	lattice[0][0].paths = 1;
	lattice[0][0].ident = {0,0};
	lattice[0][0].steps.clear();
	
	for (auto h = 1; h != H; ++h) {
		lattice[0][h].paths = 0;
		lattice[0][h].steps.clear();
		lattice[0][h].ident = {0,h};
		for(auto it_step = fibonacci.begin() + 2; it_step != fibonacci.end(); ++it_step) {
			bool step_found = false;
			// check for down step
			dh = lattice[0][h].ident[1] - (*it_step)[0];
			if(dh >= 0) {
				lattice[0][h].steps.push_back({0, dh});
				step_found = true;
			}
			if(!step_found) break;
		}
		// copy node values to opposite axis
		lattice[h][0].paths = 0;
		lattice[h][0].ident = {h,0};
		lattice[h][0].steps.clear();
		// copy the vector, swapping the step coords
		for(auto it = lattice[0][h].steps.begin(); it != lattice[0][h].steps.end(); ++it) 
			lattice[h][0].steps.push_back({(*it)[1],(*it)[0]});
	}
	
	// starting at node(1,1) compute the values for ident and steps for each node
	
	for(int h = 1; h != H; ++h) {
		for(int w = 1; w != W; ++w) {
			lattice[w][h].ident = {w,h};
			for(auto it_step = fibonacci.begin() + 2; it_step != fibonacci.end(); ++it_step) {
				bool step_found = false;
				// check for left step
				dw = w - (*it_step)[0];
				if(dw >= 0) {
					lattice[w][h].steps.push_back({dw, h});
					step_found = true;
				}
				
				// check for down step
				dh = h - (*it_step)[0];
				if(dh >= 0) {
					lattice[w][h].steps.push_back({w, dh});
					step_found = true;
				}
					
				// check for (2) diag. steps
				if((*it_step)[1] > 0) {
					dw = w - (*it_step)[1];
					dh = h - (*it_step)[2];
					if((dw >= 0)and(dh >= 0)) {
						lattice[w][h].steps.push_back({dw, dh});
						step_found = true;
					}		
					dw = w - (*it_step)[2];
					dh = h - (*it_step)[1];
					if((dw >= 0)and(dh >= 0)) {
						lattice[w][h].steps.push_back({dw, dh});
						step_found = true;
					}
				}
				if(!step_found) break;
			} // for it_step		
		} // for w...
	} // for h...
	
	// prt_lattice(lattice); exit(0);
	
	// phase 2 scan starting at node(0,0) compute path counts
	for(int h = 0; h != H; ++h) {
		for(int w = 0; w != W; ++w) {
			Node &node = lattice[w][h];
			if((w==0)and(h==0)) continue;
			for(auto s = node.steps.begin(); s != node.steps.end(); ++s) {
				node.paths += lattice[(*s)[0]][(*s)[1]].paths;
				node.paths %= modulus;
			}			
		}
	}
	
	//prt_lattice(lattice);
	std::cout << "F(" << F << "," << F << ")  ";
	std::cout << "Total path count: " << lattice[F][F].paths << std::endl;			
	
	prt_node(lattice[45][45]);
	prt_node(lattice[46][46]);
	prt_node(lattice[47][47]);
	
	
	return 0;	// only required gdb breakpoint		
}
	
