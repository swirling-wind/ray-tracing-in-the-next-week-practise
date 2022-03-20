// Copyright (c) 2012 Leonhard Gruenschloss (leonhard@gruenschloss.org)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "sobol.h"
#include <array>
#include <iostream>


std::array<double, 3> generate_3_dimension_sobol(const unsigned long long iterate_index)
{
	std::array<double, 3> sobol_point{};
	for (unsigned d = 0; d < 3; ++d)
	{
		sobol_point[d] = sobol::sample(iterate_index, d);
	}
	return sobol_point;
}


int test_sobol()
{
	// Iterate over points.
	for (unsigned long long i = 0; i < 64; ++i)
	{
		//std::cout << "sobol(" << i << ") = ";
		//// Print a few dimensions of each point.
		//for (unsigned d = 0; d < 3; ++d)
		//{
		//    const double s = sobol::sample(i, d);
		//    std::cout << "  " << s ;
		//}
		//std::cout << std::endl;
		auto point = generate_3_dimension_sobol(i);
		for (auto elem : point)
		{
			std::cout << elem << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}

