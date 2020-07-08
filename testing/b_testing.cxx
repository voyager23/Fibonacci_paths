/*
 * a_test.cxx
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

/*
 * Consider a matrix(4,5) F(W,H) F(3,4)
 */

#include <iostream>
#include <array>
#include <vector>
#include <utility>

const int W = 1;
const int H = 1;

enum Status {avail, visited, from};

typedef struct Node Node;

typedef struct {
	//int to_x, to_y;	// bounds checking
	Node *next;		// pointer to next node
	Status status;
} Step;

enum NodeType { root, path_node, goal };

typedef struct Node {
	std::vector<Step> steps;
	NodeType type;
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
	for(auto x = 0; x <= W; ++x) {
		for(auto y = 0; y <= H; ++y) {
			Node &n = l[x][y];
			
			// initialise the vector of Step for this node
			n.steps.clear();
			for(int ds = 1; ds <= 3; ++ds) {				
				//s.to_x = x;	s.to_y = y+ds;
				if(y+ds <= H) {
					s.next = &(l[x][y+ds]);		
					s.status = avail;
					n.type = path_node;
					n.steps.push_back(s); // N 1
				}
					
				//s.to_x = x+ds;	s.to_y = y;
				if(x+ds <= W) {
					s.next = &(l[x+ds][y]);		
					s.status = avail; 
					n.type = path_node;
					n.steps.push_back(s); // E 1
				}
											
				//s.to_x = x;	s.to_y = y-ds;
				if(y-ds >= 0) {
					s.next = &(l[x][y-ds]);		
					s.status = avail; 
					n.type = path_node;
					n.steps.push_back(s); // S 1
				}
				
				//s.to_x = x-ds;	s.to_y = y;
				if(x-ds >= 0) {
					s.next = &(l[x-ds][y]);		
					s.status = avail; 
					n.type = path_node;
					n.steps.push_back(s); // W 1
				}
			} // for ds...
			
			// now consider possible diagonal steps
			int dx = 4; int dy = 3;
			
			//s.to_x = x+dx;	s.to_y = y+dy;
			if((x+dx <= W)and(y+dy <= H)) {
				s.next = &(l[x+dx][y+dy]);		
				s.status = avail;
				n.type = path_node;
				n.steps.push_back(s); // NE QUADRANT 5
			}
			//s.to_x = x+dy;	s.to_y = y+dx;	// Note: position of dx and dy exchanged
			if((x+dy <= W)and(y+dx <= H)) {
				s.next = &(l[x+dy][y+dx]);		
				s.status = avail;
				n.type = path_node;
				n.steps.push_back(s); // NE QUADRANT 5
			}
			
			//s.to_x = x+dx;	s.to_y = y-dy;
			if((x+dx <= W)and(y-dy >= 0)) {
				s.next = &(l[x+dx][y-dy]);		
				s.status = avail;
				n.type = path_node;
				n.steps.push_back(s); // SE QUADRANT 5
			}
			//s.to_x = x+dy;	s.to_y = y-dx;	// Note: position of dx and dy exchanged
			if((x+dy <= W)and(y-dx >= 0)) {
				s.next = &(l[x+dy][y-dx]);		
				s.status = avail;
				n.type = path_node;
				n.steps.push_back(s); // SE QUADRANT 5
			}
			
			//s.to_x = x-dx;	s.to_y = y-dy;
			if((0 <= x-dx)and(0 <= y-dy)) {
				s.next = &(l[x-dx][y-dy]);		
				s.status = avail;
				n.type = path_node;
				n.steps.push_back(s); // SW QUADRANT 5
			}
			//s.to_x = x-dy;	s.to_y = y-dx;	// Note: position of dx and dy exchanged
			if((0 <= x-dy)and(0 <= y-dx)) {
				s.next = &(l[x-dy][y-dx]);		
				s.status = avail;
				n.type = path_node;
				n.steps.push_back(s); // SW QUADRANT 5
			}
			
			//s.to_x = x-dx;	s.to_y = y+dy;
			if((0 <= x-dx)and(y+dy <= H)) {
				s.next = &(l[x-dx][y+dy]);		
				s.status = avail;
				n.type = path_node;
				n.steps.push_back(s); // NE QUADRANT 5
			}
			//s.to_x = x-dy;	s.to_y = y+dx;	// Note: position of dx and dy exchanged
			if((0 <= x-dy)and(y+dx <= H)) {
				s.next = &(l[x-dy][y+dx]);		
				s.status = avail;
				n.type = path_node;
				n.steps.push_back(s); // NE QUADRANT 5
			}						
		} // for y...
	} // for x...
	// adjust the node status for root & home
	l[0][0].type = root;
	l[W][H].type = goal;	
}
//-------------------------------------------------
bool explore(Node *n, int &count);
bool explore(Node *n, int &count) {
	if(n->type == goal) {
		count += 1;
		return true;
	}
	for(auto c = n->steps.begin(); c != n->steps.end(); ++c) {
		if ((*c).status == visited) continue;
		if(explore((*c).next, count)) return true;
	}
	for(auto c = n->steps.begin(); c != n->steps.end(); ++c) (*c).status = avail;
	return false;
}

//------------------------------------------------------
bool solve(Node *n, int &count, Node *from);

bool solve(Node *n, int &count, Node *from) {
	if(n->type == goal) {
		count += 1;
		return true;
	} // if...
	for(auto option = n->steps.begin(); option != n->steps.end(); ++option) {
		if(((*option).status == avail)and((*option).next != from)) {
			(*option).status = visited;
			if( solve((*option).next, count, n) == true) return true;
		} // if...
	} // for...
	// All option used, clear status to avail
	for(auto option = n->steps.begin(); option != n->steps.end(); ++option) (*option).status = avail;		
	// backtrack up stack
	return false;
}
//----------------------------------------------------------------------

int main(int argc, char **argv)
{
	Lattice l;
	Node *from = NULL;
	
	//Initialise all Lattice values
	initalise_lattice(l);
	int path_count = 0;
	bool result = solve(&(l[0][0]), path_count, from);
	std::cout << "Explore root returned. Path count = " << path_count << std::endl;
}

