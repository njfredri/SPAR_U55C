#include "spar.h"
#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include <experimental/xrt_xclbin.h>
#include <experimental/xrt_ip.h>
#include <math.h>

void SPAR::print_slvregs(xrt::ip &ip){
	int x=0;
	for(int i=0; i<10; i++){
		std::cout << "Register: " << i << "\t";
		x = ip.read_register(i*4);
		std::cout << std::hex << x << "\n";
	}
}

unsigned int Log2n(unsigned int n){
    return (n > 1) ? 1 + Log2n(n / 2) : 0;
}

inline int toggleBit(int n, int k){
    return (n ^ (1 << (k - 1)));
}

inline int getbit(int n, int k){
	return (n & ( 1 << k )) >> k;
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

void SPAR::WRITE(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int ADDRA, int ADDRB, int DIA, int DIB, xrt::ip &ip){

	int reg7_input = (Tile_i << 16) + Tile_j;
	int reg2_input = (BRAM_i << 16) + BRAM_j;
	int reg3_input = (ADDRA << 16) + ADDRB;
	int reg4_input = (DIA << 16) + DIB;
	//Tile_i = 0, Tile_j = 0
	ip.write_register(MYSPAR_S00_AXI_SLV_REG7_OFFSET, reg7_input);
	//BRAM_i = 0, BRAM_j = 0
	ip.write_register(MYSPAR_S00_AXI_SLV_REG2_OFFSET, reg2_input);
	//ADDRA = 32 , ADDRB = 64
	ip.write_register(MYSPAR_S00_AXI_SLV_REG3_OFFSET, reg3_input);
	//DIA = 0xFFFF, DIB = 0x1111
	ip.write_register(MYSPAR_S00_AXI_SLV_REG4_OFFSET, reg4_input);
	//external = 1, WEA = 1, WEB = 1
	ip.write_register(MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x0000001D+LEN);
	//external = 1, WEA = 0, WEB = 0
	ip.write_register(MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG7_OFFSET, 0x00000000);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG3_OFFSET, 0x00000000);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG4_OFFSET, 0x00000000);
}

int SPAR::READ(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int ADDRA, int ADDRB, xrt::ip &ip){

	int reg7_input = (Tile_i << 16) + Tile_j;
	int reg2_input = (BRAM_i << 16) + BRAM_j;
	int reg3_input = (ADDRA << 16) + ADDRB;

	int DO;

	//Tile_i = 0, Tile_j = 0
	ip.write_register(MYSPAR_S00_AXI_SLV_REG7_OFFSET, reg7_input);

	//BRAM_i = 0, BRAM_j = 0
	ip.write_register(MYSPAR_S00_AXI_SLV_REG2_OFFSET, reg2_input);

	//ADDRA = 32 , ADDRB = 64
	ip.write_register(MYSPAR_S00_AXI_SLV_REG3_OFFSET, reg3_input);

	//external = 1, WEA = 0, WEB = 0
	ip.write_register(MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000005+LEN);

	//read
	DO = ip.read_register(MYSPAR_S00_AXI_SLV_REG5_OFFSET);

	//external = 1, WEA = 0, WEB = 0
	ip.write_register(MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG7_OFFSET, 0x00000000);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG3_OFFSET, 0x00000000);

	return DO;
}

void SPAR::WRITE_REG(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg, unsigned int data, xrt::ip &ip){

	int base = reg*32;
	int i;
	for( i=base-2; i>=base-33; i-=2){
		unsigned int dia = ((data<<(31-i))>>31)<<PE;
		unsigned int dib = ((data<<(31-i-1))>>31)<<PE;
		unsigned int tmp, oldDia, oldDib, newDia, newDib;
		tmp = READ(Tile_i, Tile_j, BRAM_i, BRAM_j, 32+i, 32+i+1, ip);
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

		WRITE(Tile_i, Tile_j, BRAM_i, BRAM_j, 32+i, 32+i+1, newDia, newDib, ip);
	}
}

unsigned int SPAR::READ_REG(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg, xrt::ip &ip){
//	static int sparRD_cnt = 0;

	int base = reg*32;
	unsigned int out=0;
	for(int i=base-2; i>=base-33; i-=2){

		unsigned int tmp, oldDia, oldDib;
		tmp = READ(Tile_i, Tile_j, BRAM_i, BRAM_j, 32+i, 32+i+1, ip);
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

int SPAR::execute(int opcode, int rd, int rs1, int rs2, xrt::ip &ip)
{

	int instruction = (opcode<<26) + (rd<<21) + (rs1 << 16) + (rs2 << 11);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000000+LEN);//start = 0, reset = 0
	ip.write_register(MYSPAR_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
//	usleep(1200);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
//	usleep(1200);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//	usleep(1200);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
//	usleep(2400);
	return 0;
}

void SPAR::SHIFT_NORTH(int rs, int rd, xrt::ip &ip){
	execute(8, rd, rs, 0, ip);
}

void SPAR::SHIFT_SOUTH(int rs, int rd, xrt::ip &ip){
	execute(7, rd, rs, 0, ip);
}

void SPAR::SHIFT_EAST(int rs, int rd, xrt::ip &ip){
	execute(5, rd, rs, 0, ip);
}

void SPAR::SHIFT_WEST(int rs, int rd, xrt::ip &ip){
	execute(6, rd, rs, 0, ip);
}
void SPAR::ELEMENTWISE_MULTIPLICATION(int matrixA_reg, int matrixB_reg, int Result_Reg, xrt::ip &ip){
	execute(2,Result_Reg,matrixA_reg,matrixB_reg, ip);
}
void SPAR::MATRIX_ADDITION(int matrixA_reg, int matrixB_reg, int Result_Reg, xrt::ip &ip){
	execute(0,Result_Reg,matrixA_reg,matrixB_reg, ip);
}
void SPAR::MATRIX_SUBTRACTION(int matrixA_reg, int matrixB_reg, int Result_Reg, xrt::ip &ip){
	execute(1,Result_Reg,matrixA_reg,matrixB_reg, ip);
}

void SPAR::WRITE_Matrix(int row, int col, std::vector<std::vector<int>> &W, int reg, int copy, xrt::ip &ip){
	int s, t;
	if (copy == 1)		//write a 2D array, starts from the left-most PEs
	{
		for( s=0; s<row; s++)
		{
			for( t=0; t<col; t++)
			{
				//WRITE_REG(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg, unsigned int data)
				WRITE_REG(s/(4 * Tile_dim), t/(4 * Tile_dim), (s/4)%Tile_dim, (t/4)%Tile_dim, ((s*4+t)%4+s*4)%16, reg, W[s][t], ip);
			}
		}
	}
}

//void WRITE_Matrix_Large(int row, int col, int block_row, int block_col, int row_blk_size, int col_blk_size, int W[][col], int reg, int copy, int block_dimension, xrt::ip &ip)){
//	int ss, tt, s, t, i;
//	if (copy == 1)		//write a 2D array, starts from the left-most PEs
//	{
//		for( s = row_blk_size * block_row; s < ((block_row + 1) * row_blk_size); s++)
//		{
//			if(s >= row){break;}
//			for( t = col_blk_size * block_col; t < ((block_col + 1) * col_blk_size); t++)
//			{
//				if(t >= col){break;}
//				ss = s - (row_blk_size * block_row);
//				tt = t - (col_blk_size * block_col);
//				WRITE_REG(ss/(4 * Tile_dim), tt/(4 * Tile_dim), (ss/4)%Tile_dim, (tt/4)%Tile_dim, ((ss*4+tt)%4+ss*4)%16, reg, W[s][t], ip);
//			}
//		}
//	}
//	else if (copy == 2)		//write a 2D array, starts from the left-most PEs
//	{
//		for( t = row_blk_size * block_row; t < ((block_row + 1) * row_blk_size); t++)
//		{
//			if(t >= row){break;}
//			for( s = col_blk_size * block_col; s < ((block_col + 1) * col_blk_size); s++)
//			{
//				if(s >= col){break;}
//				tt = t - (row_blk_size * block_row);
//				ss = s - (col_blk_size * block_col);
//				WRITE_REG(ss/(4 * Tile_dim), tt/(4 * Tile_dim), (ss/4)%Tile_dim, (tt/4)%Tile_dim, ((ss*4+tt)%4+ss*4)%16, reg, W[t][s], ip);
//			}
//		}
//	}
//	else if(copy == 0)	//write a 1D array into last column PEs
//	{
//		for( s = row_blk_size * block_row; s < ((block_row + 1) * row_blk_size); s++)
//		{
//			if(s >= row){break;}
//			for( t = col_blk_size * block_col; t < col; t++)
//			{
//				if(t >= col){break;}
//				ss = s - (row_blk_size * block_row);
//				tt = t - (col_blk_size * block_col);
//				WRITE_REG(ss/(4 * Tile_dim), Array_dim-1, (ss/4)%Tile_dim, Tile_dim-1, (ss*4+3)%16, reg, W[s][t], ip);
//
//			}
//		}
//	}
//	else if(copy == -1)	//write a 1D array into first column PEs
//	{
//		for( s = row_blk_size * block_row; s < ((block_row + 1) * row_blk_size); s++)
//		{
//			if(s >= row){break;}
//			for( t = col_blk_size * block_col; t < col; t++)
//			{
//				if(t >= col){break;}
//				ss = s - (row_blk_size * block_row);
//				tt = t - (col_blk_size * block_col);
//				WRITE_REG(0, ss/Array_dim, 0, (ss/4)%Tile_dim, ss%4, reg, W[s][t], ip);
//			}
//		}
//	}
//	else				//write a 1D array into first row, copies to the below PEs too
//	{
//		for( s = row_blk_size * block_row; s < ((block_row + 1) * row_blk_size); s++)
//		{
//			if(s >= row){break;}
//			for( t = col_blk_size * block_col; t < col; t++)
//			{
//				if(t >= col){break;}
//				ss = s - (row_blk_size * block_row);
//				tt = t - (col_blk_size * block_col);
//				WRITE_REG(ss/(4 * Tile_dim), Array_dim-1, (ss/4)%Tile_dim, Tile_dim-1, (ss*4+3)%16, reg, W[s][t]);
//			}
//		}
//		ColumnToRow(reg, reg, 0);
//
//		int instruction = (7<<26) + (reg<<21) + (reg << 16) + (0 << 11);
//		BIT_SERIAL_mWriteReg(XPAR_BIT_SERIAL_0_S00_AXI_BASEADDR, BIT_SERIAL_S00_AXI_SLV_REG0_OFFSET, instruction);//instruction
//		for( i = 0; i < copy-1; i++)
//		{
//			BIT_SERIAL_mWriteReg(XPAR_BIT_SERIAL_0_S00_AXI_BASEADDR, BIT_SERIAL_S00_AXI_SLV_REG1_OFFSET, 0x00000003+LEN);//start = 1, reset = 1
//			BIT_SERIAL_mWriteReg(XPAR_BIT_SERIAL_0_S00_AXI_BASEADDR, BIT_SERIAL_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);//start = 0, reset = 1
//		}
//	}
//}

void SPAR::MARTIX_MULTIPLICATION_Optimized(int matrixA_reg, int matrixB_reg, int Result_Reg, int matrixA_col, int block_dimension, xrt::ip &ip){
	//Optimized with BinTree accumulation
	unsigned int log_colA = Log2n(matrixA_col);
	int col_cnt = 0;
	execute(2,21,matrixB_reg,matrixA_reg, ip);	//Mult
	execute(0,23,21,0, ip);						//Add
	int i, j;
	for( i=0; i<int(log_colA); i++){
		execute(5,22,21,0,ip);						//ShiftEast
		col_cnt++;
		for( j=0; j<pow((double)2,(double)i)-(double)1; j++){
			execute(5,22,22,0, ip);						//ShiftEast
			col_cnt++;
		}
		execute(0,21,22,21, ip);					//Add
	}
	if(col_cnt != matrixA_col -1)
	{
		//shift and add
		for( i = col_cnt; i < matrixA_col; i++)
		{
			execute(5,22,21,0, ip);					//ShiftEast
			execute(0,21,23,22, ip);				//Add
		}
	}
	//shift till edge
	for( i = 0; i < (block_dimension*4) - matrixA_col; i++)
	{
		execute(5,21,21,0, ip);					//ShiftEast
	}
	execute(0,Result_Reg,21,0, ip);
	return;

}

int SPAR::printReg(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg, xrt::ip &ip){
	int REG[32];
	int reg_out;
	int DO;
	reg = reg << 5;
	int i;
	for( i=0; i<16; i++){
		DO = READ (Tile_i, Tile_j, BRAM_i, BRAM_j, reg+i, reg+i+16, ip);
		REG[i+16] = getbit(DO, PE);
		REG[i] = getbit(DO, PE+16);
	}

	reg_out = getTrans(REG);

	return reg_out;
}

void SPAR::printRegFile(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int number_of_regs, xrt::ip &ip){
	int PE, i;
	for( PE=0; PE<16; PE++){
		for( i=number_of_regs-1; i>=0; i--)
		{
			printf("%08x \t", printReg(Tile_i, Tile_j, BRAM_i, BRAM_j, PE, i, ip));
		}
		printf("\n\r");
	}

}

fixed_point_t SPAR::float_to_fixed(int input)
{
    return (fixed_point_t)(input * (1 << FIXED_POINT_FRACTIONAL_BITS));
}


void SPAR::FxP_Conversion(int row, int col, std::vector<std::vector<int>> &in, std::vector<std::vector<fixed_point_t>> & out){
	int s, t;
	for( s=0; s<row; s++)
	{
		for( t=0; t<col; t++)
		{
			out[s][t] = float_to_fixed(in[s][t]);
		}
	}
}

void SPAR::RESET(xrt::ip ip) {
    // Resetting the slave registers
	ip.write_register(MYSPAR_S00_AXI_SLV_REG0_OFFSET, 0x00000000);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000000);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG2_OFFSET, 0x00000000);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG7_OFFSET, 0x00000000);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG3_OFFSET, 0x00000000);
	ip.write_register(MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000000+LEN);    // try removing +LEN


	// Resetting the system: reset = 1
	ip.write_register(MYSPAR_S00_AXI_SLV_REG1_OFFSET, 0x00000001+LEN);

	//reset all the registers of all PEs in the array to zero
	int i, j, k, l, addra;
	for( i = 0; i < Array_dim; i++){
		for( j = 0; j < Array_dim; j++){
			for( k = 0; k < Tile_dim; k++){
				for( l = 0; l < Tile_dim; l++){
					for( addra = 0; addra < 32 * 32; addra++){
						WRITE(i, j, k, l, addra, 0, 0, 0, ip);
					}
				}
			}
		}
	}
	printf("SPAR RESET DONE! \n");
}


