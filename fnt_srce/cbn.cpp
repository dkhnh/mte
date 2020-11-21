#include "ctr.cpp"

// For command: cbn <OUTPUT FILE> [FILE | DIRECOTRY] .. [FILE | DIRECOTRY]

int main(int args, char **argv) {
	// For command: cbn
	if (args == 1) {
		show(CWR);
		return 1;
	}
	
	// Check <OUTPUT FILE>
	if (ctr::is_directory(argv[1])) {
		deta(argv[1], SNA);
		return 1;
	}
	
	// For command: cbn <OUTPUT FILE>
	if (args == 2) {
		// Current working directory
		std::string cwdir = ctr::cwdir();

		// [FILE | DIRECOTRY] .. [FILE | DIRECOTRY]
		char **items = new char*[1];
		items[0] = new char[cwdir.size() + 1];
		strcpy(items[0], cwdir.c_str());

		// backup
		ctr::bkp_items(items, 1, argv[1]);

		// Release memory
		delete [] items[0];
		delete [] items;

		return 0;
	}

	// For command: cbn <OUTPUT FILE> [FILE | DIRECOTRY] .. [FILE | DIRECOTRY]
	
	// [FILE | DIRECOTRY] .. [FILE | DIRECOTRY]
	char **items;
	items = new char*[args - 2];
	
	// Check [FILE | DIRECOTRY] .. [FILE | DIRECOTRY]
	for (int i = 2; i < args; i ++) {
		if (!ctr::is_directory(argv[i]) && !ctr::is_file(argv[i])) {
			deta(argv[i], NSFD);
			return 1;
		}

		if (strcmp(argv[i], ".") == 0 || strcmp(argv[i], SEPERATE_H) == 0) {
			std::string cwdir = ctr::cwdir();
			
			items[i - 2] = new char[cwdir.size() + 1];
			strcpy(items[i - 2], cwdir.c_str());
			
			continue;
		}

		items[i - 2] = new char[strlen(argv[i]) + 1];
		strcpy(items[i - 2], argv[i]);
	}
	
	// backup items
	ctr::bkp_items(items, args - 2, argv[1]);

	// Release memory
	args -= 2;
	while(args --) {
		delete [] items[args];
	}
	
	delete [] items;

	return 0;
}
