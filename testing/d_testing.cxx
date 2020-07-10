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

const int W = 1;
const int H = 2;

//typedef struct Node Node;

enum Status {avail, on_path, goal}; // Node status

typedef struct Node {
	std::vector<Node*> steps;
	Status status;
	
	int idx;	// debug node ident
} Node;

typedef std::array< std::array<Node,W+1>, H+1> Lattice;

void initalise_lattice(Lattice &l);
void node_stat(Node *n);
void voidsolve(Node *n, int &count, std::vector<int> &path );
void prt_path(Node *goal, std::vector<int> &path);

//-------------------------------------------------

void initalise_lattice(Lattice &l){
	/* Steps are defined as follows:
	 * N 1,2,3    E 1,2,3    S 1,2,3    E 1,2,3
	 * NE 5,8     SE 5,8     SW 5,8     NE 5,8
	 * 0 <= x+dx <= W    0 <= y+dy <= H
	 */
 	int idx = 0;
	for(auto y = 0; y <= H; ++y) {
		for(auto x = 0; x <= W; ++x) {
			std::cout<<std::endl;
			Node &n = l[x][y];			
			// initialise the vector of Step for this node
			n.steps.clear();
			n.idx = idx++;	// debug idx
			n.status = avail;	// {avail, on_path, goal}
			node_stat(&n);
			for(int ds = 1; ds <= 3; ++ds) {				
				if(y+ds <= H) {
					//s.next = &(l[x][y+ds]);		
					n.steps.push_back(&(l[x][y+ds])); // N 1
					std::cout << x << "," << y << " => " << x << "," << y+ds << std::endl;
				}
					
				if(x+ds <= W) {
					n.steps.push_back(&(l[x+ds][y])); // E 1
					std::cout << x << "," << y << " => " << x+ds << "," << y << std::endl;
				}
											
				if(y-ds >= 0) {
					n.steps.push_back(&(l[x][y-ds])); // S 1
					std::cout << x << "," << y << " => " << x << "," << y-ds << std::endl;
				}
				
				if(x-ds >= 0) {
					n.steps.push_back(&(l[x-ds][y])); // W 1
					std::cout << x << "," << y << " => " << x-ds << "," << y << std::endl;
				}
			} // for ds...
			
			// now consider 8 possible diagonal steps
			int dx = 4; int dy = 3;	// DEBUG SET ONLY
			
			if((x+dx <= W)and(y+dy <= H)) {
				n.steps.push_back(&(l[x+dx][y+dy])); // NE QUADRANT 5
					std::cout << x << "," << y << " => " << x+dx << "," << y+dy << std::endl;
			}
			if((x+dy <= W)and(y+dx <= H)) {
				n.steps.push_back(&(l[x+dy][y+dx])); // NE QUADRANT 5
					std::cout << x << "," << y << " => " << x+dy << "," << y+dx << std::endl;
			}
			
			if((x+dx <= W)and(y-dy >= 0)) {
				n.steps.push_back(&(l[x+dx][y-dy])); // SE QUADRANT 5
					std::cout << x << "," << y << " => " << x+dx << "," << y-dy << std::endl;
			}
			if((x+dy <= W)and(y-dx >= 0)) {
				n.steps.push_back(&(l[x+dy][y-dx])); // SE QUADRANT 5
					std::cout << x << "," << y << " => " << x+dy << "," << y-dx << std::endl;
			}
			
			if((0 <= x-dx)and(0 <= y-dy)) {
				n.steps.push_back(&(l[x-dx][y-dy])); // SW QUADRANT 5
					std::cout << x << "," << y << " => " << x-dx << "," << y-dy << std::endl;
			}
			if((0 <= x-dy)and(0 <= y-dx)) {
				n.steps.push_back(&(l[x-dy][y-dx])); // SW QUADRANT 5
					std::cout << x << "," << y << " => " << x-dy << "," << y-dx << std::endl;
			}
			
			if((0 <= x-dx)and(y+dy <= H)) {
				n.steps.push_back(&(l[x-dx][y+dy])); // NE QUADRANT 5
					std::cout << x << "," << y << " => " << x-dx << "," << y+dy << std::endl;
			}
			if((0 <= x-dy)and(y+dx <= H)) {
				n.steps.push_back(&(l[x-dy][y+dx])); // NE QUADRANT 5
					std::cout << x << "," << y << " => " << x-dy << "," << y+dx << std::endl;
			}
		} // for y...
	} // for x...
	
	// adjust the node status for root & home
	l[0][0].status = on_path;
	l[W][H].status = goal;	
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
	for(auto option = n->steps.begin(); option != n->steps.end(); ++option) {
		if((*option)->status == on_path) continue;
		if((*option)->status == goal) {
			count += 1;
			prt_path((*option), path);
		} // include a debug print
		if((*option)->status == avail)
			// step to next node
			voidsolve( *option, count, path);		
	}// All options considered so set this node open
	n->status = avail;
	path.pop_back();	// pop this node index
	// ...and retreat
}

void prt_path(Node *goal, std::vector<int> &path) {
	std::cout<<"Goal path: ";
	for(auto it = path.begin(); it != path.end(); ++it) std::cout << *it << ", ";
	std::cout << goal->idx << std::endl;	
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
	
	voidsolve(root, count, current_path);
		
	std::cout<<"Count = "<<count<<std::endl;
	return 0;
}

