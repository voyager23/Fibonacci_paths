/*
 * d_testing.cxx
 * 
 * Copyright 2020 mike <mike@pop-os>
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
#include <array>
#include <vector>
#include <utility>
#include <iomanip>
#include <algorithm>

#define SQ 4;

const int W = 2;
const int H = 2;

const int Max_Fibonacci=10001;

#define MAX(x,y) ((x < y) ? y : x)

typedef struct Fgroup {
	int fib, a, b;
}Fgroup;

typedef std::vector<Fgroup> FGroups;

enum Status {avail, on_path, goal}; // Node status

typedef struct Node {
	int idx;	// debug node ident
	Status status;	
	std::vector<Node*> steps;
}Node;

typedef Node Lattice[H+1][W+1];

void initalise_lattice(Lattice &l, FGroups &fg);
void node_stat(Node *n);
void voidsolve(Node *n, int &count, std::vector<int> &path );
void prt_path(Node *goal, std::vector<int> &path);
void initialise_fgroups(FGroups &fg, const int limit);

void prt_node(Lattice &l, int h, int w);
void prt_lattice(Lattice &l);

//----------------------------------------------------------------------

void initialise_fgroups(FGroups &fg, const int limit) {
	// setup first two entries
	Fgroup temp{1,0,0};
	fg.push_back(temp);
	fg.push_back(temp);
	int idx = 2;
	// set remaining fibonacci numbers < Max_Fibonacci
	do {
		temp.fib = fg.at(idx-1).fib + fg.at(idx-2).fib;
		temp.a = 0;
		temp.b  = 0;
		fg.push_back(temp);
		idx += 1;
	} while(temp.fib < Max_Fibonacci);
	// Now setup the pythagorian triples starting at index 4 Fib=5
	fg.at(4).a = 3;
	fg.at(4).b = 4;
	idx = 6;
	while (idx < fg.size()) {
		fg.at(idx).a = fg.at(idx-1).fib - fg.at(idx-2).a;
		fg.at(idx).b = fg.at(idx-2).fib + fg.at(idx-2).a + fg.at(idx-2).b;
		idx += 2;
	}
	
	//std::cout << "FGroups has " << fg.size() << " groups." << std::endl;
}
//----------------------------------------------------------------------

void initalise_lattice(Lattice &l, FGroups &fg){
	/* Steps are defined as follows:
	 * N 1,2,3    E 1,2,3 
	 * NE 5,8 
	 * 0 <= w+ds <= W    0 <= h+ds <= H
	 * 
	 * ONLY N, E, NE steps are allowed
	 * 
	 * ONLY VALID FOR SQUARE LATTICE
	 * 
	 */
	 
	 std::cout << "Valid for square matrix only." << std::endl;
	 
 	int idx = 0;	// node ident
	for(auto h = 0; h <= H; ++h) {
		for(auto w = 0; w <= W; ++w) {
			
			//std::cout<<std::endl;
			//std::cout<<std::endl;
			Node *n = &l[h][w];						
			// initialise the vector of Step for this node
			n->steps.clear();
			n->idx = idx;		// node-idx
			n->status = avail;	// {avail, on_path, goal}
			//std::cout << "Node: " << n->idx << " h" << h << " w" << w << std::endl;
			
			int ds_idx = 1;
			int ds = fg.at(ds_idx).fib;
			
			while( (ds <= W) or (ds <= H)) {
								
				if(h+ds <= H) {
					n->steps.push_back(&(l[h+ds][w])); // move N
					//std::cout << w << "," << h << " => " << w << "," << h+ds << std::endl;
				}
					
				if(w+ds <= W) {
					n->steps.push_back(&(l[h][w+ds])); // move E
					//std::cout << w << "," << h << " => " << w+ds << "," << h << std::endl;
				}		
				ds_idx += 1;
				ds = fg.at(ds_idx).fib;
			} // end ds selector
			
			idx += 1; // inc ident
		} // for w...
	} // for h...	

	// now consider possible NE  diagonal steps
	// rescan all nodes
	// apply any NE steps at each node
	for(auto h = 0; h <= H; ++h) {
		for(auto w = 0; w <= W; ++w) {
			 
			Node *n = &l[h][w];						

			// scan values of dw & dh
			int ds_idx = 4;			
			int dw = fg.at(ds_idx).a;
			int dh = fg.at(ds_idx).b;
			
			while (MAX(dw,dh) <= MAX(W,H)) {
							
				if((w+dw <= W)and(h+dh <= H)) {
					n->steps.push_back(&(l[h+dh][w+dw])); // NE QUADRANT 5
						//std::cout << w << "," << h << " => " << w+dw << "," << h+dh << std::endl;
				}
				
				if((w+dh <= W)and(h+dw <= H)) {
					n->steps.push_back(&(l[h+dw][w+dh])); // NE QUADRANT 5
						//std::cout << w << "," << h << " => " << w+dh << "," << h+dw << std::endl;
				}
				ds_idx += 2;
				dw = fg.at(ds_idx).a;
				dh = fg.at(ds_idx).b;
			}
		}
	}
	
	// adjust the node status for root & home
	l[0][0].status = on_path;
	l[H][W].status = goal;
	
	//Patch code
	for(auto w = 0; w <= W; ++w) l[0][w].status = on_path;

}
//----------------------------------------------------------------------

void node_stat(Node *n) {
	std::cout << "Node " << n->idx << " Status:";
	if(n->status == avail) {std::cout << " avail." << std::endl; return; }
	if(n->status == on_path) {std::cout << " on_path." << std::endl; return; }
	if(n->status == goal) {std::cout << " goal." << std::endl; return; }
	std::cout << "Unknown" << std::endl;
}

void voidsolve(Node *n, int &count, std::vector<int> &path) {
	path.push_back(n->idx);	// update the current path
	n->status = on_path;
	
	// print node idx & options list
	// std::cout << "At node " << n->idx << " options: ";
	// for(auto option = n->steps.begin(); option != n->steps.end(); ++option) 
		// std::cout << (*option)->idx << " ";
	// std::cout << std::endl;
	
	for(auto option = n->steps.begin(); option != n->steps.end(); ++option) {
		if((*option)->status == on_path) continue;
		if((*option)->status == goal) {
			count += 1;
			prt_path((*option), path);
		}
		
		if((*option)->status == avail)
			// step to next node
			voidsolve( *option, count, path);
			
				// recursive call returns here
					
	} // for option...
	// All options considered so set this node open
	n->status = avail;
	path.pop_back();	// pop this node index
	// ...and retreat
}

// -----debug print routines-----
void prt_path(Node *goal, std::vector<int> &path) {
	std::cout<<"Goal path: ";
	for(auto it = path.begin(); it != path.end(); ++it) std::cout << *it << ", ";
	std::cout << goal->idx << std::endl;	
}

void prt_node(Lattice &l, int h, int w) {
	// Node:1 h = 0 w = 1 status: avail
	Node n = l[h][w];
	std::cout << "Node:" << n.idx << " h = " << h << " w = " << w << " Status: ";
	if(n.status == avail) {
		std::cout << " avail." << std::endl;
	} else if (n.status == on_path) {
		std::cout << " on_path." << std::endl;
		
	} else if (n.status == goal) {
		std::cout << " goal." << std::endl;
		
	} else {
		std::cout << " unknown" << std::endl;
	}	
}

void prt_lattice(Lattice &l){
	for(int h = 0; h <= H; ++h) {
		for(int w = 0; w <= W; ++w) {
			prt_node(l,h,w);
			std::cout << std::endl;
		}
	}	
}

//======================================================================

int main(int argc, char **argv)
{
	Lattice l;
	int count = 0;
	Node *root = &l[0][0];
	bool result;
	FGroups fg;
	
	initialise_fgroups(fg, Max_Fibonacci);
	
	initalise_lattice(l,fg); // examine all paths from root node
	
	std::vector<int> current_path;
	
	std::cout << "Solving F(" << W << "," << H << ")" << std::endl;
	
	voidsolve(root, count, current_path);
		
	std::cout<<"Count = "<<count<<std::endl;

}

