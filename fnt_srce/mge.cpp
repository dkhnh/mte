#include "ctr.cpp"

// cbn <FILE NAME [000.bin]> [OUTPUT DIRECTORY]

int main(int args, char **argv) {
	if (args < 2 || args > 3) {
		show(CWR);
		return 0;
	}

	// check input file exists
	char *ifp = new char[strlen(argv[1]) + 8];
	strcpy(ifp, argv[1]);
	strcat(ifp, "000.bin\0");
	if (!ctr::is_file(ifp)) {
		deta(argv[1], NSFD);
		return 0;
	}
	delete [] ifp;

	// For command: cbn <FILE NAME [000.bin]>
	if (args == 2) {
		ctr::cbn_file(argv[1], "./");
		return 0;
	}

	// For command: cbn <FILE NAME [000.bin]> <OUTPUT DIRECTORY>
	if (args == 3) {
		ctr::cbn_file(argv[1], argv[2]);
		return 0;
	}

	return 0;
}
