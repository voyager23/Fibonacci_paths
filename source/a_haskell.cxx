/*
 * a_haskell.cxx
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
 * 
 */


#include <iostream>
#include <array>
#include <vector>

typedef std::array<unsigned, 3> Fibgroup;
typedef std::array<unsigned, 2> Coord;
typedef std::vector<Fibgroup> Fibvect;
typedef struct {
	unsigned count;
	std::vector<Coord> steps;
} Node;

const unsigned W = 10;
const unsigned H = 10;
const unsigned MinFibonacci = 10000;

// macro to swap w and h values
#define SWAPWH(coord) coord[0] = coord[0] xor coord[1]; coord[1] = coord[1] xor coord[0]; coord[0] = coord[0] xor coord[1];

// -----Global variables-----
std::array<std::array<Node, W>, H> lattice;
std::vector<Coord> rect_step;
std::vector<Coord> pyth_step;
Fibvect fibonacci;

// -----Definitions-----
bool valid_coord(Coord &coord);
bool valid_coord(Coord &coord) {
	return  ((((coord[0]>=0)and(coord[0]<=W))and((coord[1]>=0)and(coord[1]<=H))) ? true : false);	
}

void construct_fibvect(Fibvect &fv);
void construct_fibvect(Fibvect &fv) {
	fv.clear();
	fv.push_back( {0,0,0 } ); // F(0)
	fv.push_back( {1,0,0 } ); // F(1)
	
	unsigned current_fibonacci = 0;
	do {
		current_fibonacci = (*(fv.end()-1))[0] + (*(fv.end()-2))[0];
		fv.push_back({current_fibonacci,0,0});
	} while (current_fibonacci < MinFibonacci);
	// starting at F(5) value 5, insert the pythag triples
	unsigned idx = 5;
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
	lattice[0][0].count = 1;
	lattice[0][0].steps.clear();
	for(auto h = 1; h < H; ++h) {
		lattice[0][h].count = 0;
		// set iterator to rect_step
		// target = current - rect_step
		// if(VALIDWH(target)) {
		//		add target count to current count;
		//		swap step
		//		target = current - rect_step
		//		if(VALIDWH(target)) add target count to current
		// } else { // first step failed
		//		swap step
		//		target = current -rect_step;
		//		if(VALIDWH(target) {
		//			add target count to current;
		//		} else { 
		//			break; // both steps failed break from step_loop
		//		}
		//	}
		for(auto it_rs = rect_step.begin(); it_rs != rect_step.end(); ++it_rs) {
			Coord step = *it_rs;
			Coord target = { (0 - step[0]), (h - step[1]) };
			if(valid_coord(target)) {
				lattice[0][h].count += lattice[target[0]][target[1]].count;
				target = { (0 - step[1]), (h - step[0]) }; // swapped the step coords
				if(valid_coord(target))
					lattice[0][h].count += lattice[target[0]][target[1]].count;
				// reflect result
				lattice[h][0].count = lattice[0][h].count;
			} else { // first step failed
				target = { (0 - step[1]), (h - step[0]) }; // swapped the step coords
				if(valid_coord(target)) {
					lattice[0][h].count += lattice[target[0]][target[1]].count;
					// reflect result
					lattice[h][0].count = lattice[0][h].count;					
				} else {
					break; // both steps failed
				} // break
			} // first step fail
		} // it_rs
		
	} // column 0;
	
	// starting at w1,h1 ascend column, setting counts and reflecting results
	for(auto h = 1; h < H; ++h) {
		int w = h;
		
		lattice[w][h].count = 0; // 
		
		for(auto it_rs = rect_step.begin(); it_rs != rect_step.end(); ++it_rs) {
			Coord step = *it_rs;
			Coord target = { (w - step[0]), (h - step[1]) };
			if(valid_coord(target)) {
				lattice[w][h].count += lattice[target[0]][target[1]].count;
				target = { (w - step[1]), (h - step[0]) }; // swapped the step coords
				if(valid_coord(target))
					lattice[w][h].count += lattice[target[0]][target[1]].count;
				// reflect result
				lattice[h][w].count = lattice[0][h].count;
			} else { // first step failed
				target = { (w - step[1]), (h - step[0]) }; // swapped the step coords
				if(valid_coord(target)) {
					lattice[w][h].count += lattice[target[0]][target[1]].count;
					// reflect result
					lattice[h][w].count = lattice[0][h].count;					
				} else {
					break; // both steps failed
				} // break
			} // first step fail
		} // it_rs
				
	}
	return 0;
}

