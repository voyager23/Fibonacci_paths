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
typedef std::array<unsigned, 2> Step;
typedef std::vector<Fibgroup> Fibvect;
typedef struct {
	unsigned count;
	std::vector<Step> steps;
} Node;

const unsigned W = 10;
const unsigned H = 10;
const unsigned MinFibonacci = 10000;

// macro to check if coords(h,w) are valid
#define VALIDWH(w,h) ((((w>=0)and(w<=W))and((h>=0)and(h<=H))) ? true : false)
// macro to swap w and h values
#define SWAPWH(w,h) w = w xor h; h = h xor w; w = w xor h;

// -----Global variables-----
std::array< std::array<unsigned, W>, H> lattice;
std::vector<Step> rect_step;
std::vector<Step> pyth_step;
Fibvect fibonacci;

// -----Definitions-----
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
void construct_rect_step(std::vector<Step> &rs, Fibvect &fv);
void construct_rect_step(std::vector<Step> &rs, Fibvect &fv) {
	
}
void construct_pyth_step(std::vector<Step> &ps, Fibvect &fv);
void construct_pyth_step(std::vector<Step> &ps, Fibvect &fv) {
	
}
// -----Main Code-----
int main(int argc, char **argv)
{
	construct_fibvect(fibonacci);
	construct_rect_step(rect_step, fibonacci);
	construct_pyth_step(pyth_step, fibonacci);
	
	return 0;
}

