#ifndef __SPAR_H_
#define __SPAR_H_
#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include <experimental/xrt_xclbin.h>
#include <experimental/xrt_ip.h>
#include <stdint.h>
#include <vector>

//define values
#define MAX_LEN 32
#define LEN  (MAX_LEN << 24)
#define FIXED_POINT_FRACTIONAL_BITS 16
#define Array_dim  1
#define Tile_dim  2


#define MYSPAR_S00_AXI_SLV_REG0_OFFSET 0
#define MYSPAR_S00_AXI_SLV_REG1_OFFSET 4
#define MYSPAR_S00_AXI_SLV_REG2_OFFSET 8
#define MYSPAR_S00_AXI_SLV_REG3_OFFSET 12
#define MYSPAR_S00_AXI_SLV_REG4_OFFSET 16
#define MYSPAR_S00_AXI_SLV_REG5_OFFSET 20
#define MYSPAR_S00_AXI_SLV_REG6_OFFSET 24
#define MYSPAR_S00_AXI_SLV_REG7_OFFSET 28
#define MYSPAR_S00_AXI_SLV_REG8_OFFSET 32
#define MYSPAR_S00_AXI_SLV_REG9_OFFSET 36

extern int sparAdd_cnt;
extern int sparSub_cnt;
extern int sparMul_cnt;
extern int sparShN_cnt;
extern int sparShS_cnt;
extern int sparShE_cnt;
extern int sparShW_cnt;
extern int sparWR_cnt;
extern int sparRD_cnt;

//define types
typedef int32_t fixed_point_t;

namespace SPAR{
		void print_slvregs(xrt::ip &ip);
		void WRITE(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int ADDRA, int ADDRB, int DIA, int DIB, xrt::ip &ip);
		int READ(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int ADDRA, int ADDRB, xrt::ip &ip);
		void WRITE_REG(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg, unsigned int data, xrt::ip &ip);
		unsigned int READ_REG(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg, xrt::ip &ip);
		int execute(int opcode, int rd, int rs1, int rs2, xrt::ip &ip);
		void SHIFT_NORTH(int rs, int rd, xrt::ip &ip);
		void SHIFT_SOUTH(int rs, int rd, xrt::ip &ip);
		void SHIFT_EAST(int rs, int rd, xrt::ip &ip);
		void SHIFT_WEST(int rs, int rd, xrt::ip &ip);
		void ELEMENTWISE_MULTIPLICATION(int matrixA_reg, int matrixB_reg, int Result_Reg, xrt::ip &ip);

		void WRITE_Matrix(int row, int col, std::vector<std::vector<int>> &W, int reg, int copy, xrt::ip &ip);

		void MATRIX_ADDITION(int matrixA_reg, int matrixB_reg, int Result_Reg, xrt::ip &ip);
		void MATRIX_SUBTRACTION(int matrixA_reg, int matrixB_reg, int Result_Reg, xrt::ip &ip);
		void MARTIX_MULTIPLICATION_Optimized(int matrixA_reg, int matrixB_reg, int Result_Reg, int matrixA_col, int block_dimension, xrt::ip &ip);
		int printReg(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int PE, int reg, xrt::ip &ip);
		void printRegFile(int Tile_i, int Tile_j, int BRAM_i, int BRAM_j, int number_of_regs, xrt::ip &ip);
		fixed_point_t float_to_fixed(int input);
		void FxP_Conversion(int row, int col, std::vector<std::vector<int>> &in, std::vector<std::vector<fixed_point_t>> & out);
		void RESET(xrt::ip ip);
};
#endif
