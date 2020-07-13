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

const int W = 3;
const int H = 4;

//typedef struct Node Node;

enum Status {avail, on_path, goal}; // Node status

typedef struct Node {
	int idx;	// debug node ident
	Status status;	
	std::vector<Node*> steps;
}Node;

typedef Node Lattice[H+1][W+1];

void initalise_lattice(Lattice &l);
void node_stat(Node *n);
void voidsolve(Node *n, int &count, std::vector<int> &path );
void prt_path(Node *goal, std::vector<int> &path);

void prt_node(Lattice &l, int h, int w);
void prt_lattice(Lattice &l);

//-------------------------------------------------

void initalise_lattice(Lattice &l){
	/* Steps are defined as follows:
	 * N 1,2,3    E 1,2,3    S 1,2,3    E 1,2,3
	 * NE 5,8     SE 5,8     SW 5,8     NE 5,8
	 * 0 <= w+ds <= W    0 <= h+ds <= H
	 * 
	 * 13/07/20 ASSUME ONLY N, E, NE steps are allowed
	 */
 	int idx = 0;
	for(auto h = 0; h <= H; ++h) {
		for(auto w = 0; w <= W; ++w) {
			//std::cout<<std::endl;
			//std::cout<<std::endl;
			Node *n = &l[h][w];	
					
			// initialise the vector of Step for this node
			n->steps.clear();
			n->idx = idx;	// node-idx
			n->status = avail;	// {avail, on_path, goal}
			//std::cout << "Node: " << n->idx << " h" << h << " w" << w << std::endl;
			
			for(int ds = 1; ds <= 3; ++ds) {
								
				if(h+ds <= H) {
					n->steps.push_back(&(l[h+ds][w])); // N 1 2 3
					//std::cout << w << "," << h << " => " << w << "," << h+ds << std::endl;
				}
					
				if(w+ds <= W) {
					n->steps.push_back(&(l[h][w+ds])); // E 1 2 3
					//std::cout << w << "," << h << " => " << w+ds << "," << h << std::endl;
				}
// only allow steps N & E
#if(0)										
				if(h-ds >= 0) {
					n->steps.push_back(&(l[h-ds][w])); // S 1 2 3
					//std::cout << w << "," << h << " => " << w << "," << h-ds << std::endl;
				}
				
				if(w-ds >= 0) {
					n->steps.push_back(&(l[h][w-ds])); // W 1 2 3
					//std::cout << w << "," << h << " => " << w-ds << "," << h << std::endl;
				}
#endif
			} // for ds = 1->3
			
//......................................................................			
			
			// now consider 8 possible diagonal steps
			int dw = 4; int dh = 3;	// DEBUG SET ONLY
			
			if((w+dw <= W)and(h+dh <= H)) {
				n->steps.push_back(&(l[h+dh][w+dw])); // NE QUADRANT 5
					//std::cout << w << "," << h << " => " << w+dw << "," << h+dh << std::endl;
			}
			if((w+dh <= W)and(h+dw <= H)) {
				n->steps.push_back(&(l[h+dw][w+dh])); // NE QUADRANT 5
					//std::cout << w << "," << h << " => " << w+dh << "," << h+dw << std::endl;
			}
// only allow  steps NE
#if(0)
		
			if((w+dw <= W)and(h-dh >= 0)) {
				n->steps.push_back(&(l[h-dh][w+dw])); // SE QUADRANT 5
					//std::cout << w << "," << h << " => " << w+dw << "," << h-dh << std::endl;
			}
			if((w+dh <= W)and(h-dw >= 0)) {
				n->steps.push_back(&(l[h-dw][w+dh])); // SE QUADRANT 5
					//std::cout << w << "," << h << " => " << w+dh << "," << h-dw << std::endl;
			}
			
			if((0 <= w-dw)and(0 <= h-dh)) {
				n->steps.push_back(&(l[h-dh][w-dw])); // SW QUADRANT 5
					//std::cout << w << "," << h << " => " << w-dw << "," << h-dh << std::endl;
			}
			if((0 <= w-dh)and(0 <= h-dw)) {
				n->steps.push_back(&(l[h-dw][w-dh])); // SW QUADRANT 5
					//std::cout << w << "," << h << " => " << w-dh << "," << h-dw << std::endl;
			}
			
			if((0 <= w-dw)and(h+dh <= H)) {
				n->steps.push_back(&(l[h+dh][w-dw])); // NE QUADRANT 5
					//std::cout << w << "," << h << " => " << w-dw << "," << h+dh << std::endl;
			}
			if((0 <= w-dh)and(h+dw <= H)) {
				n->steps.push_back(&(l[h+dw][w-dh])); // NE QUADRANT 5
					//std::cout << w << "," << h << " => " << w-dh << "," << h+dw << std::endl;
			}
			
#endif
//......................................................................

			idx += 1;
		} // for w...
	} // for h...
	
	// adjust the node status for root & home
	l[0][0].status = on_path;
	l[H][W].status = goal;	
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
// debug print routines
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
	
	initalise_lattice(l); // examine all paths from root node
	
	std::vector<int> current_path;
	
	// prt_lattice(l);
	
	voidsolve(root, count, current_path);
		
	std::cout<<"Count = "<<count<<std::endl;

}

