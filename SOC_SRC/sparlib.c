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
#define _BSD_SOURCE

#include "sparlib.h"
#include "xparameters.h"
#include "xil_printf.h"
#include <stdio.h>
#include "xiltimer.h"
// ---- Arafat utility functions ----
void SPAR_RESET() {
    // Resetting the slave registers
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG0_OFFSET, 0x00000000);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000000);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG7_OFFSET, 0x00000000);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG3_OFFSET, 0x00000000);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000000+LEN);    // try removing +LEN


	// Resetting the system: reset = 1
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);

	//reset all the registers of all PEs in the array to zero
	int i, j, k, l, addra;
	for( i = 0; i < Array_dim; i++){
		for( j = 0; j < Array_dim; j++){
			for( k = 0; k < Tile_dim; k++){
				for( l = 0; l < Tile_dim; l++){
					for( addra = 0; addra < 32 * 32; addra++){
						WRITE(i, j, k, l, addra, 0, 0, 0);
					}
				}
			}
		}
	}
	printf("SPAR RESET DONE! \n");
}


// ---- Atiyeh utility functions ----
int READ(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int ADDRA, int ADDRB){

	int reg7_input = (Tile_i << 16) + Tile_j;
	int reg2_input = (BRAM_i << 16) + BRAM_j;
	int reg3_input = (ADDRA << 16) + ADDRB;

	int DO;

	//Tile_i = 0, Tile_j = 0
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG7_OFFSET, reg7_input);

	//BRAM_i = 0, BRAM_j = 0
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG2_OFFSET, reg2_input);

	//ADDRA = 32 , ADDRB = 64
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG3_OFFSET, reg3_input);

	//external = 1, WEA = 0, WEB = 0
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000005+LEN);

	//read
	DO = MYSPAR_mReadReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG5_OFFSET);

	//external = 1, WEA = 0, WEB = 0
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG7_OFFSET, 0x00000000);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG3_OFFSET, 0x00000000);

	return DO;
}


void WRITE(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int ADDRA, int ADDRB, int DIA, int DIB){

	int reg7_input = (Tile_i << 16) + Tile_j;
	int reg2_input = (BRAM_i << 16) + BRAM_j;
	int reg3_input = (ADDRA << 16) + ADDRB;
	int reg4_input = (DIA << 16) + DIB;

	//Tile_i = 0, Tile_j = 0
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG7_OFFSET, reg7_input);

	//BRAM_i = 0, BRAM_j = 0
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG2_OFFSET, reg2_input);

	//ADDRA = 32 , ADDRB = 64
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG3_OFFSET, reg3_input);

	//DIA = 0xFFFF, DIB = 0x1111
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG4_OFFSET, reg4_input);

	//external = 1, WEA = 1, WEB = 1
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x0000001D+LEN);

	//external = 1, WEA = 0, WEB = 0
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG7_OFFSET, 0x00000000);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG3_OFFSET, 0x00000000);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG4_OFFSET, 0x00000000);

}


void printRegFile(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int number_of_regs, char format){

	int PE, i;
	for( PE=0; PE<16; PE++){
		printf("PE-%-2d:    ", PE);
		for( i=number_of_regs-1; i>=0; i--)
		{
			if (format=='d')
				printf("R%-2d: %08d     ", i, printReg(Tile_i, Tile_j, BRAM_i, BRAM_j, PE, i));
			else
				printf("R%-2d: %08x     ", i, printReg(Tile_i, Tile_j, BRAM_i, BRAM_j, PE, i));
		}
		printf("\n\r");
	}

}


int printReg(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg){

	int REG[32];
	int reg_out;
	int DO;
	reg = reg << 5;
	int i;
	for( i=0; i<16; i++){
		DO = READ (Tile_i, Tile_j, BRAM_i, BRAM_j, reg+i, reg+i+16);
		REG[i+16] = getbit(DO, PE);
		REG[i] = getbit(DO, PE+16);
	}

	reg_out = getTrans(REG);

	return reg_out;
}


int getbit(int n, int k){
	return (n & ( 1 << k )) >> k;
}


int execute(int opcode, int rd, int rs1, int rs2)
{
	int instruction = (opcode<<26) + (rd<<21) + (rs1 << 16) + (rs2 << 11);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000000+LEN);//start = 0, reset = 0
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
	msleep(10);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
	msleep(10);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
	msleep(10);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
	msleep(10);
	return 0;
}


void WRITE_REG(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg, unsigned int data){

	int base = reg*32;
	int i;
	for( i=base-2; i>=base-33; i-=2){

		unsigned int dia = ((data<<(31-i))>>31)<<PE;
		unsigned int dib = ((data<<(31-i-1))>>31)<<PE;
		unsigned int tmp, oldDia, oldDib, newDia, newDib;
		tmp = READ(Tile_i, Tile_j, BRAM_i, BRAM_j, 32+i, 32+i+1);
		oldDia = tmp >> 16;
		oldDib = (tmp << 16) >> 16;
		char oldBitA = getbit(oldDia, PE);
		char oldBitB = getbit(oldDib, PE);
		char BitA = getbit(dia, PE);
		char BitB = getbit(dib, PE);
		newDia = dia|oldDia;
		if(BitA==0 && oldBitA==1) newDia = toggleBit(newDia, PE+1);

		newDib = dib|oldDib;
		if(BitB==0 && oldBitB==1) newDib = toggleBit(newDib, PE+1);

		WRITE(Tile_i, Tile_j, BRAM_i, BRAM_j, 32+i, 32+i+1, newDia, newDib);
	}
}


unsigned int READ_REG(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg){

	int base = reg*32;
	unsigned int out;
	for(int i=base-2; i>=base-33; i-=2){

		unsigned int tmp, oldDia, oldDib;
		tmp = READ(Tile_i, Tile_j, BRAM_i, BRAM_j, 32+i, 32+i+1);
		oldDia = tmp >> 16;
		oldDib = (tmp << 16) >> 16;
		int oldBitA = getbit(oldDia, PE);
		int oldBitB = getbit(oldDib, PE);
		out = out << 1;
		out = out + oldBitB;
		out = out << 1;
		out = out + oldBitA;
	}
	return out;
}


void WRITE_Matrix(int row, int col, fixed_point_t W[][col], int reg, int copy){

	int s, t, i;
	if (copy == 1)		//write a 2D array, starts from the left-most PEs
	{
		for( s=0; s<row; s++)
		{
			for( t=0; t<col; t++)
			{
				//WRITE_REG(s/(4 * Tile_dim), t/(4 * Tile_dim), (s/4)%Tile_dim, (t/4)%Tile_dim, ((s*4+t)%4+s*4)%16, reg, W[s][t]);
				WRITE_REG(0, 0, 0, 0, s*4+t, reg, W[s][t]);
			}
		}
	}
	else if(copy == 0)	//write a 1D array into last column PEs
	{
		for( s=0; s<row; s++)
		{
			for( t=0; t<col; t++)
			{
				WRITE_REG(s/(4 * Tile_dim), Array_dim-1, (s/4)%Tile_dim, Tile_dim-1, (s*4+3)%16, reg, W[s][t]);
			}
		}
	}
	else if(copy == -1)	//write a 1D array into first column PEs
	{
		for( s=0; s<row; s++)
		{
			for( t=0; t<col; t++)
			{
				WRITE_REG(0, s/Array_dim, 0, (s/4)%Tile_dim, s%4, reg, W[s][t]);
			}
		}
	}
	else				//write a 1D array into first row, copies to the below PEs too
	{
		for( s=0; s<row; s++)
		{
			for( t=0; t<col; t++)
			{
				WRITE_REG(s/(4 * Tile_dim), Array_dim-1, (s/4)%Tile_dim, Tile_dim-1, (s*4+3)%16, reg, W[s][t]);
			}
		}
		ColumnToRow(reg, reg, 0);

		int instruction = (7<<26) + (reg<<21) + (reg << 16) + (0 << 11);
		MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
		for( i = 0; i < copy-1; i++)
		{
			MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
			MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
		}

	}

}


void ELEMENTWISE_MULTIPLICATION(int matrixA_reg, int matrixB_reg, int Result_Reg){
	execute(2,Result_Reg,matrixA_reg,matrixB_reg);
	//mult_cnt++;
}


void MARTIX_ADDITION(int matrixA_reg, int matrixB_reg, int Result_Reg){
	execute(0,Result_Reg,matrixA_reg,matrixB_reg);
	//add_cnt++;
}


void MARTIX_SUBTRACTION(int matrixA_reg, int matrixB_reg, int Result_Reg){
	execute(1,Result_Reg,matrixA_reg,matrixB_reg);
	//sub_cnt++;
}


int getTrans(int* ram){

	int reg = 0;
	int i;
	for(i=31; i>=0; i--){
		reg <<= 1;
		getbit(ram[i],0)==1? reg++ : reg;
	}
	return reg;

}


int toggleBit(int n, int k){
    return (n ^ (1 << (k - 1)));
}


void ColumnToRow(int rd, int rs, int function){

	//function: 0 == noOp, 1 == sigmoid, 2 == tanh
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG6_OFFSET, function);
	execute(5, rs, rs, 0);
	//ShR_cnt++;
	int instruction = (7<<26) + (rd<<21) + (rd << 16) + (0 << 11);
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
	MYSPAR_mWriteReg(XPAR_MYSPAR_V1_0_S00_AXI_BASEADDR, MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
	//ShS_cnt++;
}

void Write_Row(int *arr, int cols, int row_num, int reg){
	int s1 = row_num/(4 * Tile_dim);
	int s2 = (row_num/4)%Tile_dim;
	for(int t=0; t<cols; t++){
		WRITE_REG(s1, t/(4 * Tile_dim), s2, (t/4)%Tile_dim, ((row_num*4+t)%4+row_num*4)%16, reg, arr[t]);
	}
}

void Write_Col(int *arr, int rows, int col_num, int reg){
	int t1 = col_num/(4 * Tile_dim);
	int t2 = (col_num/4)%Tile_dim;
	for(int s=0; s<rows; s++){
		WRITE_REG(s/(4 * Tile_dim), t1, (s/4)%Tile_dim, t2, ((s*4+col_num)%4+s*4)%16, reg, arr[s]);
	}
}

fixed_point_t float_to_fixed(int input)
{
    return (fixed_point_t)(input * (1 << FIXED_POINT_FRACTIONAL_BITS));
}


void FxP_Conversion(int row, int col, fixed_point_t in[][col], fixed_point_t out[][col] ){
	int s, t;
	for( s=0; s<row; s++)
	{
		for( t=0; t<col; t++)
		{
			out[s][t] = float_to_fixed(in[s][t]);
		}
	}
}
