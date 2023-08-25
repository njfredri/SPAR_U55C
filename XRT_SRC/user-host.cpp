#include <iostream>
#include <cstring>
#include <vector>

#include "xrt/xrt_bo.h"
#include "xrt/xrt_device.h"
#include <experimental/xrt_xclbin.h>
#include <experimental/xrt_ip.h>
#include "spar.h"

using namespace SPAR;

int main(int argc, char** argv) {
	std::cout << "argc = " << argc << std::endl;
	for(int i=0; i < argc; i++){
		std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
	}

	std::string binaryFile = argv[1]; //"./vadd.xclbin";//
	auto xclbin = xrt::xclbin(binaryFile);
	int device_index = 0;

	std::cout<<"Open the device" << device_index << std::endl;
	auto device = xrt::device(device_index);

	std::cout << "Load the xclbin " << binaryFile <<std::endl;
	auto uuid = device.load_xclbin(binaryFile);

	auto ip1 = xrt::ip(device, uuid, "mySPAR_v2_0");

	RESET(ip1);
	std::cout<<"Writing\n";
	WRITE_REG(0, 0, 0, 0, 0, 0, 2<<16, ip1);
	WRITE_REG(0, 0, 0, 0, 0, 1, 2<<16, ip1);
	execute(2, 3, 1, 0, ip1);
	std::cout<<"Reading\n";
	int x = READ_REG(0, 0, 0, 0, 0, 3, ip1);
	std::cout<<"\n";
	std::cout<<x;
	std::cout<<"\n";
	printRegFile(0, 0, 0, 0, 4, ip1);
	std::cout<<"Done\n";

	// Some values to operate on
	static std::vector<std::vector<int>> W1 = {
		{1,2,3,4},
		{0x10,0x20,0x30,0x50},
		{0x100,0x200,0x300,0x600},
		{0x700,0x800,0x900,0x1000}
	};

	static std::vector<std::vector<int>> W2 = {
		{1,1,1,1},
		{2,2,2,2},
		{3,3,3,3},
		{4,4,4,4}
	};

	// Write values to SPAR
	WRITE_Matrix(4, 4, W1, 1, 1, ip1);
	WRITE_Matrix(4, 4, W2, 2, 1, ip1);
	printf("WRITE Matrix IS DONE! \n");
	// Perform vector add/sub operations
	MATRIX_ADDITION(1, 2, 3, ip1);		// R3 = R1 + R2
	MATRIX_SUBTRACTION(1, 2, 4, ip1);	// R4 = R1 - R2

	printf("\r\nADD/SUB results in R3 and R4 \n");
	printf("R3 = R1 + R2\n");
	printf("R4 = R1 - R2\n");
	printf("Printing the PEs Register Files (DEC)\r\n");
	printf("Tile(%d, %d): PE-Block(%d, %d): \n\r", 0, 0, 0, 0);
	printRegFile(0, 0, 0, 0, 6, ip1);		// print 6 registers of PEs in a PE-Block

	// Convert to fix point for multiplication
	static std::vector<std::vector<fixed_point_t>> W1_fx(4, std::vector<fixed_point_t>(4));
	static std::vector<std::vector<fixed_point_t>> W2_fx(4, std::vector<fixed_point_t>(4));
	FxP_Conversion(4,4, W1, W1_fx);
	FxP_Conversion(4,4, W2, W2_fx);

	// Write values to SPAR PEs
	WRITE_Matrix(4, 4, W1_fx, 1, 1, ip1);
	WRITE_Matrix(4, 4, W2_fx, 2, 1, ip1);
	printf("\r\nWRITE Fixed-Point Matrix IS DONE! \n");

	ELEMENTWISE_MULTIPLICATION(1, 2, 5, ip1);

	printf("\r\nMultiplication results in R5 \n");
	printf("R5 = R1 x R2 [Fixed Point: 16-bit integral, 16-bit fractional]\n");
	printf("Printing the PEs Register Files (HEX)\r\n");
	printf("Tile(%d, %d): PE-Block(%d, %d): \n\r", 0, 0, 0, 0);
	printRegFile(0, 0, 0, 0, 7, ip1);

	// Accumulate R5 row-wise
	execute(5,6,5,0, ip1);	// shift-east R5 -> R6
	MATRIX_ADDITION(6, 5, 6, ip1);	// R6 += R5

	execute(5,6,6,0, ip1);	// shift-east R6 -> R6
	MATRIX_ADDITION(6, 5, 6, ip1);	// R6 += R5

	execute(5,6,6,0, ip1);	// shift-east R6 -> R6
	MATRIX_ADDITION(6, 5, 6, ip1);	// R5 += R6

	printf("\r\nAccumulation results in R6 of PE-3,7,11,15 \n");
	printf("Printing the PEs Register Files (HEX)\r\n");
	printf("Tile(%d, %d): PE-Block(%d, %d): \n\r", 0, 0, 0, 0);
	printRegFile(0, 0, 0, 0, 8, ip1);


	// Accumulate column-wise
	execute(8,7,6,0, ip1);	// shift-north R6 -> R7
	MATRIX_ADDITION(6, 7, 7, ip1);	// R7 += R6

	execute(8,7,7,0, ip1);	// shift-north R7 -> R7
	MATRIX_ADDITION(6, 7, 7, ip1);	// R7 += R6

	execute(8,7,7,0, ip1);	// shift-north R7 -> R7
	MATRIX_ADDITION(6, 7, 7, ip1);	// R7 += R6

	printf("\r\nAccumulation of R6 of PE-3,7,11,15 in R7 of PE-3 \n");
	printf("Printing the PEs Register Files (HEX)\r\n");
	printf("Tile(%d, %d): PE-Block(%d, %d): \n\r", 0, 0, 0, 0);
	printRegFile(0, 0, 0, 0, 8, ip1);

	return 0;
}
