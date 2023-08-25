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

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "mySPAR.h"
#include "sparlib.h"
#include "sparspec.h"

int main()
{
    init_platform();
	//cleanup_platform();
	printf("Program Started...\n\n\r");

	// Reset Array Processor system
	SPAR_RESET();

    // Some values to operate on
	static fixed_point_t W1[4][4] = {
	    {1,2,3,4},
	    {0x10,0x20,0x30,0x50},
	    {0x100,0x200,0x300,0x600},
	    {0x700,0x800,0x900,0x1000}
	};

	static fixed_point_t W2[4][4] = {
		{1,1,1,1},
		{2,2,2,2},
		{3,3,3,3},
		{4,4,4,4}
	};


    // Write values to SPAR
	WRITE_Matrix(4, 4, W1, 1, 1);
	WRITE_Matrix(4, 4, W2, 2, 1);
	printf("WRITE Matrix IS DONE! \n");

	// Perform vector add/sub operations
	MARTIX_ADDITION(1, 2, 3);		// R3 = R1 + R2
	MARTIX_SUBTRACTION(1, 2, 4);	// R4 = R1 - R2

	printf("\r\nADD/SUB results in R3 and R4 \n");
	printf("R3 = R1 + R2\n");
	printf("R4 = R1 - R2\n");
	printf("Printing the PEs Register Files (DEC)\r\n");
	printf("Tile(%d, %d): PE-Block(%d, %d): \n\r", 0, 0, 0, 0);
	printRegFile(0, 0, 0, 0, 6, 'd');		// print 6 registers of PEs in a PE-Block

	// Convert to fix point for multiplication
	static fixed_point_t W1_fx[4][4], W2_fx[4][4];
	FxP_Conversion(4,4, W1, W1_fx);
	FxP_Conversion(4,4, W2, W2_fx);

	// Write values to SPAR PEs
	WRITE_Matrix(4, 4, W1_fx, 1, 1);
	WRITE_Matrix(4, 4, W2_fx, 2, 1);
	printf("\r\nWRITE Fixed-Point Matrix IS DONE! \n");


	ELEMENTWISE_MULTIPLICATION(1, 2, 5);


	printf("\r\nMultiplication results in R5 \n");
	printf("R5 = R1 x R2 [Fixed Point: 16-bit integral, 16-bit fractional]\n");
	printf("Printing the PEs Register Files (HEX)\r\n");
	printf("Tile(%d, %d): PE-Block(%d, %d): \n\r", 0, 0, 0, 0);
	printRegFile(0, 0, 0, 0, 7, 'x');


	/******************************
	 * PE-Block organization
	 * 			   North
	 *
	 * 		   0   1   2   3
	 * 		   4   5   6   7
	 *  West   8   9  10  11    East
	 * 		  12  13  14  15
	 *
	 * 		       South
	 *
	 * 	So, when we accumulate towards east,
	 * 	The accumulation result is saved in PE-3,7,11,15.
	 * 	PE-3  = PE-0  + PE-1  + PE-2  + PE-3
	 * 	PE-7  = PE-4  + PE-5  + PE-6  + PE-7
	 * 	PE-11 = PE-8  + PE-9  + PE-10 + PE-11
	 * 	PE-15 = PE-12 + PE-13 + PE-14 + PE-15
	 ******************************/
	// Accumulate R5 row-wise
	execute(5,6,5,0);	// shift-east R5 -> R6
	MARTIX_ADDITION(6, 5, 6);	// R6 += R5

	execute(5,6,6,0);	// shift-east R6 -> R6
	MARTIX_ADDITION(6, 5, 6);	// R6 += R5

	execute(5,6,6,0);	// shift-east R6 -> R6
	MARTIX_ADDITION(6, 5, 6);	// R5 += R6

	printf("\r\nAccumulation results in R6 of PE-3,7,11,15 \n");
	printf("Printing the PEs Register Files (HEX)\r\n");
	printf("Tile(%d, %d): PE-Block(%d, %d): \n\r", 0, 0, 0, 0);
	printRegFile(0, 0, 0, 0, 8, 'x');


	// Accumulate column-wise
	execute(8,7,6,0);	// shift-north R6 -> R7
	MARTIX_ADDITION(6, 7, 7);	// R7 += R6

	execute(8,7,7,0);	// shift-north R7 -> R7
	MARTIX_ADDITION(6, 7, 7);	// R7 += R6

	execute(8,7,7,0);	// shift-north R7 -> R7
	MARTIX_ADDITION(6, 7, 7);	// R7 += R6

	printf("\r\nAccumulation of R6 of PE-3,7,11,15 in R7 of PE-3 \n");
	printf("Printing the PEs Register Files (HEX)\r\n");
	printf("Tile(%d, %d): PE-Block(%d, %d): \n\r", 0, 0, 0, 0);
	printRegFile(0, 0, 0, 0, 8, 'x');


    cleanup_platform();
    return 0;
}


