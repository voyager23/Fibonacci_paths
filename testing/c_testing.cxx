/*
 * c_testing.cxx
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

typedef struct Node Node;

enum Status {avail, visited, from};

typedef struct {
	//int to_x, to_y;	// bounds checking
	Node *next;		// pointer to next node
	Status status;
} Step;

enum NodeType { root, path_node, goal };

typedef struct Node {
	std::vector<Step> steps;
	NodeType type;
	int idx;	// debug node ident
	//Node *back;
} Node;

typedef std::array< std::array<Node,W+1>, H+1> Lattice;

//-------------------------------------------------
void initalise_lattice(Lattice &l);

/* Steps are defined as follows:
 * N 1,2,3    E 1,2,3    S 1,2,3    E 1,2,3
 * NE 5,8     SE 5,8     SW 5,8     NE 5,8
 * 0 <= to_x <= W    0 <= to_y <= H
 */
 
void initalise_lattice(Lattice &l){
	Step s;
	int idx = 0;
	for(auto y = 0; y <= H; ++y) {
		for(auto x = 0; x <= W; ++x) {
			std::cout<<std::endl;
			Node &n = l[x][y];			
			// initialise the vector of Step for this node
			n.steps.clear();
			n.idx = idx++;
			//n.back = NULL;
			n.type = path_node;
			for(int ds = 1; ds <= 3; ++ds) {				
				//s.to_x = x;	s.to_y = y+ds;
				if(y+ds <= H) {
					s.next = &(l[x][y+ds]);		
					s.status = avail;
					n.steps.push_back(s); // N 1
					std::cout << x << "," << y << " => " << x << "," << y+ds << std::endl;
				}
					
				//s.to_x = x+ds;	s.to_y = y;
				if(x+ds <= W) {
					s.next = &(l[x+ds][y]);		
					s.status = avail; 
					n.steps.push_back(s); // E 1
					std::cout << x << "," << y << " => " << x+ds << "," << y << std::endl;
				}
											
				//s.to_x = x;	s.to_y = y-ds;
				if(y-ds >= 0) {
					s.next = &(l[x][y-ds]);		
					s.status = avail; 
					n.steps.push_back(s); // S 1
					std::cout << x << "," << y << " => " << x << "," << y-ds << std::endl;
				}
				
				//s.to_x = x-ds;	s.to_y = y;
				if(x-ds >= 0) {
					s.next = &(l[x-ds][y]);		
					s.status = avail; 
					n.steps.push_back(s); // W 1
					std::cout << x << "," << y << " => " << x-ds << "," << y << std::endl;
				}
			} // for ds...
			
#if(1)			
			// now consider 8 possible diagonal steps
			int dx = 4; int dy = 3;
			
			//s.to_x = x+dx;	s.to_y = y+dy;
			if((x+dx <= W)and(y+dy <= H)) {
				s.next = &(l[x+dx][y+dy]);		
				s.status = avail;
				n.steps.push_back(s); // NE QUADRANT 5
					std::cout << x << "," << y << " => " << x+dx << "," << y+dy << std::endl;
			}
			//s.to_x = x+dy;	s.to_y = y+dx;	// Note: position of dx and dy exchanged
			if((x+dy <= W)and(y+dx <= H)) {
				s.next = &(l[x+dy][y+dx]);		
				s.status = avail;
				n.steps.push_back(s); // NE QUADRANT 5
					std::cout << x << "," << y << " => " << x+dy << "," << y+dx << std::endl;
			}
			
			//s.to_x = x+dx;	s.to_y = y-dy;
			if((x+dx <= W)and(y-dy >= 0)) {
				s.next = &(l[x+dx][y-dy]);		
				s.status = avail;
				n.steps.push_back(s); // SE QUADRANT 5
					std::cout << x << "," << y << " => " << x+dx << "," << y-dy << std::endl;
			}
			//s.to_x = x+dy;	s.to_y = y-dx;	// Note: position of dx and dy exchanged
			if((x+dy <= W)and(y-dx >= 0)) {
				s.next = &(l[x+dy][y-dx]);		
				s.status = avail;
				n.steps.push_back(s); // SE QUADRANT 5
					std::cout << x << "," << y << " => " << x+dy << "," << y-dx << std::endl;
			}
			
			//s.to_x = x-dx;	s.to_y = y-dy;
			if((0 <= x-dx)and(0 <= y-dy)) {
				s.next = &(l[x-dx][y-dy]);		
				s.status = avail;
				n.steps.push_back(s); // SW QUADRANT 5
					std::cout << x << "," << y << " => " << x-dx << "," << y-dy << std::endl;
			}
			//s.to_x = x-dy;	s.to_y = y-dx;	// Note: position of dx and dy exchanged
			if((0 <= x-dy)and(0 <= y-dx)) {
				s.next = &(l[x-dy][y-dx]);		
				s.status = avail;
				n.steps.push_back(s); // SW QUADRANT 5
					std::cout << x << "," << y << " => " << x-dy << "," << y-dx << std::endl;
			}
			
			//s.to_x = x-dx;	s.to_y = y+dy;
			if((0 <= x-dx)and(y+dy <= H)) {
				s.next = &(l[x-dx][y+dy]);		
				s.status = avail;
				n.steps.push_back(s); // NE QUADRANT 5
					std::cout << x << "," << y << " => " << x-dx << "," << y+dy << std::endl;
			}
			//s.to_x = x-dy;	s.to_y = y+dx;	// Note: position of dx and dy exchanged
			if((0 <= x-dy)and(y+dx <= H)) {
				s.next = &(l[x-dy][y+dx]);		
				s.status = avail;
				n.steps.push_back(s); // NE QUADRANT 5
					std::cout << x << "," << y << " => " << x-dy << "," << y+dx << std::endl;
			}
#endif
						
		} // for y...
	} // for x...
	
	// adjust the node status for root & home
	l[0][0].type = root;
	l[W][H].type = goal;	
}
//----------------------------------------------------------------------
bool solve(Node *n, int &count );

bool solve(Node *n,Node *from, int &count) {
	std::cout<<"At node "<< std::setw(2) << std::setfill('0') << n->idx << std::endl;
	// return pointer is set by calling function - root.back = NULL;
	// flag goal
	if(n->type == goal) {
		std::cout<<"GOAL\n";
		return true;
	}
	for(auto option = n->steps.begin(); option != n->steps.end(); ++option) {
		// test return path
		if ((*option).next == n) { 
			std::cout<<"Return path disregarded.\n";
			continue;
		}
		if ((*option).status != avail) {
			std::cout<<"Step not avail.\n";
			continue;
		}
		// assume this option is available so set it to used and descend the path
		(*option).status = visited; // sequential search -- not required??
		if (solve((*option).next, n, count) == true) {
			std::cout<<"found goal - backtracking via stack\n";
			return true;
		}
	}
	// all options tried - reset node to avail - backtrack vai stack
	std::cout<<"Options exhausted - node reset - backtracking via stack\n";
	
	for(auto option = n->steps.begin(); option != n->steps.end(); ++option)
		(*option).status = avail;
		
	return false;
	
}
//======================================================================
int main(int argc, char **argv)
{
	Lattice l;
	int count = 0;
	Node *root = &l[0][0];
	bool result;
	
	initalise_lattice(l); // examine all paths from root node
	do{
		result = solve(root, NULL, count);
	} while (result);
		
	std::cout<<"Count = "<<count<<std::endl;
	return 0;
}

