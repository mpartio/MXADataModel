
#include <string>
#include <iostream>
#include <hdf5.h>

int main(int argc, char **argv) {
	std::cout << "size_t: " << sizeof (size_t) << std::endl;
	std::cout << "hsize_t: " << sizeof (hsize_t) << std::endl;
	return 0;
}