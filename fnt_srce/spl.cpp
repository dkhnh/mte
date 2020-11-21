#include "ctr.cpp"

// spl <FILE> <SPLIT FILE SIZE> [OUTPUT DIRECTORY]

int main(int args, char **argv) {
	if (args < 3 || args > 4) {
		show(CWR);
		return 0;
	}

	// check input file exists
	if (!ctr::is_file(argv[1])) {
		deta(argv[1], NSFD);
		return 0;
	}

	// Check <SPLIT FILE SIZE> is a number
	char *nums = new char[11]; 
	strcpy(nums, "0123456789\0");
	
	for (char *c = argv[2]; *c != '\0'; c ++) {
		bool k = false;
		for (char *n = nums; *n != '\0'; n ++) {
			if (*c == *n) {
				k = true;
				break;
			}
		}
		
		if (!k) {
			deta(argv[2], "SPLIT FILE SIZE must be a number.");
			return 1;
		}
	}
	
	delete [] nums;

	// convert <SPLIT FILE COUNT> char * into int
	int sfs = atoi(argv[2]);

	// check <SPLIT FILE COUNT> limit
	if (sfs < 0 || sfs > SPLIT_FILE_LIMIT_SIZE) {
		deta(sfs, SFLS);
		return 1;
	}

	// For command: spl <FILE NAME> <SPLIT FILE COUNT>
	if (args == 3) {
		ctr::spl_file(argv[1], sfs, "./");
		return 0;
	}

	// For command: spl <FILE NAME> <SPLIT FILE COUNT> <OUT DIRECTORY>
	if (args == 4) {
		ctr::spl_file(argv[1], sfs, argv[3]);
		return 0;
	}

	return 0;
}
