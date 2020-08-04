/*
 * e_testing.cxx
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
 * Locally generated main.cxx
 */

#include <iostream>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))h

bool canreach(int w, int h, int dw, int dh);
bool canreach(int w, int h, int dw, int dh) {
	return (((w - dw) >= 0) and ((h - dh) >= 0));
}

int main(int argc, char **argv)
{
	bool reachable = false;
	reachable = canreach(3,5,4,3);
	std::cout << reachable << std::endl;
	return 0;
}

