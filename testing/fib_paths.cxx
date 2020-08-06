/*
 * fib_paths.cxx
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
#include <algorithm>

const int F = 10000;

const int W = F+1;
const int H = F+1;
const int modulus = 1000000007;

// -----Declarations-----
typedef std::array<int, 2> Coord;

bool coord_cmp(Coord l, Coord r);


// -----Definitions------

bool coord_cmp(Coord l, Coord r) {
	return((l[0] + l[1]) < (r[0] + r[1]));
}



// -----Global variables-----

std::vector<double> fib = { 1.0, 2.0, 3.0, 5.0, 8.0, 13.0, 21.0, 34.0, 55.0, 89.0, 144.0, 233.0, 
	377.0, 610.0, 987.0, 1597.0, 2584.0, 4181.0, 6765.0, 10946.0, 17711.0, 28657.0, 46368.0, 75025.0};


std::array<std::array<int,W>,H> lattice;
std::vector<Coord> steps;

int main(int argc, char **argv) {
	
	// calculate the steps vector
	std::cout << "Calculating step vector ...\n ";
	for(int h = 0; h < H; ++h) {
		double h2 = h*h;
		for(int w = 0; w < W; ++w) {
			double w2 = w*w;
			double d = sqrt(h2 + w2);
			// scan vector fib for match
			std::vector<double>::iterator it = fib.begin();
			while((*it < d)and(it != fib.end())) ++it;
			if(*it == d) {
				steps.push_back({w,h});
			}
		}
	}
	std::cout << "complete." << std::endl;
	
	std::sort(steps.begin(), steps.end(), coord_cmp);
	
	for(auto it = steps.begin(); it != steps.end(); ++it) 
		std::cout << (*it)[0] << "," << (*it)[1] << "   ";
	std::cout << std::endl;
#if(0)	
	bool root = true;
	for(auto h = 0; h < H; ++h) {
		for(auto w = 0; w < W; ++w) {
			if(root) {
				root = false;
				lattice[w][h] = 1;
			} else {
				lattice[w][h] = 0;
				
#endif	
	
}
