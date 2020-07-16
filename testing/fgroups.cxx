/*
 * fgroups.cxx
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
 * generate a vector of fibonacci numbers and associated pythag triples
 */

#include <iostream>
#include <vector>

//----------------------------------------------------------------------
const int Max_Fibonacci=10001;

typedef struct Fgroup {
	int fib, a, b;
}Fgroup;

typedef std::vector<Fgroup> FGroups;

void initialise_fgroups(FGroups &fg, const int limit);

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
	
	std::cout << "FGroups has " << fg.size() << " groups." << std::endl;
}
//----------------------------------------------------------------------
int main(int argc, char **argv)
{
	FGroups fg;
	
	initialise_fgroups(fg, Max_Fibonacci);
	
	for(auto it = fg.begin(); it != fg.end(); ++it) {
		std::cout << (*it).fib << " ";
		std::cout << (*it).a << " ";
		std::cout << (*it).b << std::endl;;
	}
	
	return 0;
}

