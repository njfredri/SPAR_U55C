/*
 * Copyright (c) 2022, SPAR-Internal
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *  Author: MD Arafat Kabir
 *  Email: arafat.sun@gmail.com
 *  Compute Systems Design Lab
 *  University of Arkansas
 */
 
 
#ifndef SRC_SPARSPEC_H_
#define SRC_SPARSPEC_H_



#define MAX_LEN 32
#define FIXED_POINT_FRACTIONAL_BITS 16

#define Array_dim  1
#define Tile_dim  1
#define divide_size_W 50
#define divide_size_U 20
#define m1 4						//size of hidden layers
#define p 4							//size of inputs
#define q 1							//size of outputs
#define epoch 1

#define MAX_LEN 32		// Size of PE registers
#define LEN  (MAX_LEN << 24)


//Fixed-point Format: 11.5 (16-bit)
typedef int32_t fixed_point_t;




#endif /* SRC_SPARSPEC_H_ */
