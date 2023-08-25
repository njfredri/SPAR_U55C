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

#ifndef SRC_SPARLIB_H_
#define SRC_SPARLIB_H_



#include "mySPAR.h"
#include "sparspec.h"


// Prototype
void WRITE(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int ADDRA, int ADDRB, int DIA, int DIB);
int READ(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int ADDRA, int ADDRB);
int execute(int opcode, int rd, int rs1, int rs2);
int getbit(int n, int k);
void getTranspose(int* ram, int* reg);
int printReg(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg);
int getTrans(int* ram);
void printRegFile(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int number_of_regs, char format);
void WRITE_REG(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg, unsigned int data);
unsigned int READ_REG(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg);
int toggleBit(int n, int k);
void ColumnToRow(int rd, int rs, int function);
void WRITE_Matrix(int row, int col, fixed_point_t W[][col], int reg, int copy);
void FxP_Conversion(int row, int col, fixed_point_t in[][col], fixed_point_t out[][col] );
void MARTIX_ADDITION(int matrixA_reg, int matrixB_reg, int Result_Reg);
void MARTIX_SUBTRACTION(int matrixA_reg, int matrixB_reg, int Result_Reg);
void ELEMENTWISE_MULTIPLICATION(int matrixA_reg, int matrixB_reg, int Result_Reg);
void SPAR_RESET();





#endif /* SRC_SPARLIB_H_ */
