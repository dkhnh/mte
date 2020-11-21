#include "ctr.cpp"

// dth <MERGE FILE> <OUT DIRECTORY>

int main(int args, char **argv) {
	if (args != 3) {
		show(CWR);
		return 0;
	}
	
	if (!ctr::is_file(argv[1])) {
		deta(argv[1], NSFD);
		return 0;
	}
	
	if (!ctr::is_directory(argv[2])) {
		deta(argv[2], NSFD);
		return 0;
	}
	
	if (argv[2][strlen(argv[2]) - 1] == SEPERATE_C) {
		ctr::rst_items(argv[1], argv[2]);
	} else {
		char *outdir = new char[strlen(argv[2]) + 2];
		strcpy(outdir, argv[2]);
		strcat(outdir, SEPERATE);
		ctr::rst_items(argv[1], outdir);
		
		delete [] outdir;
	}
	
	return 0;
}