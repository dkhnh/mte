#if defined WIN32 || defined _WIN32
#include <windows.h>
#define SEPERATE "\\\0"
#define SEPERATE_C '\\'
#define SEPERATE_H ".\\"
#define get_cwd _getcwd
#else
#include <sys/stat.h>
#include <unistd.h>
#define SEPERATE "/\0"
#define SEPERATE_C '/'
#define SEPERATE_H "./"
#define get_cwd getcwd
#endif

#include <dirent.h>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// const
#define IOS_LIMIT_SIZE 16777216
#define SPLIT_FILE_LIMIT_SIZE 1073741824

// error
#define CWR "This command is wrong."
#define NSFD "No such file or directory."
#define SNA "There is already a folder with the same name as the file."
#define SFLS "[SPLIT_FILE_SIZE] is limited to 2 to 1073741824."

// echo
#define show(x) std::cout << x << std::endl;
#define deta(x, y) std::cout << x << ": " << y << std::endl;

namespace ctr {
	struct path_dt;
	struct bname_dt;
	void dt_path(const char *path, struct path_dt &pdt);
	void dt_bname(const char *bname, struct bname_dt &bdt);
	void sfbp(char *bnp, int c);
	std::string cwdir();
	std::string exdir();
#if defined WIN32 || defined _WIN32
	__int64 file_size(const char* fp);
#else
	std::ifstream::pos_type file_size(const char *path);
#endif
	void list_dir(const char *path, std::queue<std::string> &listdir);
	bool is_file(const char *path);
	bool is_directory(const char *path);
	void create_file(const char *path, const char *buffer, std::streamsize buffersize);
	void create_file_cf(const char *path, std::ifstream &ifile, std::streamsize datasize);
	void write_file_cf(std::ofstream &ofile, std::ifstream &ifile, std::streamsize datasize);
	void append_file(const char *path, const char *buffer, std::streamsize buffersize);
	void create_folder(const char *path);
	void bkp_file(const char *filename, const char *path, std::ofstream &ofile);
	void bkp_directory(const char *foldername, const char *path, std::ofstream &ofile);
	void bkp_items(char **items, int itemcount, const char *outfile);
	void rst_item(std::ifstream &ifile, const char *outpath);
	void rst_item_h(std::ifstream &ifile, const char *outpath);
	void rst_items(const char *mgefile, const char *outdir);
	void spl_file(const char *ifp, long long int sfs, const char *odir);
	void cbn_file(const char *ifbp, const char *odir);
}

// struct path_dt
struct ctr::path_dt {
	char *dir;
	char *item;
};

// struct bname_dt
struct ctr::bname_dt {
	char *name;
	char *ext;
};

// detach path
void ctr::dt_path(const char *path, struct path_dt &pdt) {
	size_t pl = strlen(path);
	
	char *p = new char[pl + 1];
	strcpy(p, path);
	
	if (p[pl - 1] == SEPERATE_C) {
		p[pl - 1] = '\0';
		pl --;
	}
	
	for (int i = pl - 1; i >= 0; i --) {
		if (p[i] == SEPERATE_C) {
			// item
			char *itr = (p + i + 1);
			pdt.item = new char[pl - i];
			strcpy(pdt.item, itr);
			
			// dir
			*itr = '\0';
			pdt.dir = new char[i + 2];
			strcpy(pdt.dir, p);
			
			delete [] p;
			return;
		}
	}
	
	// item
	pdt.item = new char[pl + 1];
	strcpy(pdt.item, p);
	
	// dir
	pdt.dir = new char[3];
	strcpy(pdt.dir, SEPERATE_H);
	
	delete [] p;
}

// detach bname
void ctr::dt_bname(const char *bname, struct bname_dt &bdt) {
	size_t bl = strlen(bname);
	
	char *b = new char[bl + 1];
	strcpy(b, bname);
	
	for (int i = bl - 1; i >= 0; i --) {
		if (b[i] == '.') {
			// ext
			char *itr = (b + i);
			bdt.ext = new char[bl - i];
			strcpy(bdt.ext, itr + 1);
			
			// name
			*itr = '\0';
			bdt.name = new char[i + 1];
			strcpy(bdt.name, b);
			
			delete [] b;
			return;
		}
	}
	
	// ext
	bdt.ext = new char[1];
	strcpy(bdt.ext, "\0");
	
	// name
	bdt.name = new char[bl + 1];
	strcpy(bdt.name, b);
	
	delete [] b;
}

// build split file path
void ctr::sfbp(char *bnp, int c) {
	if (c < 1) {
		strcat(bnp, "000.bin\0");
	} else if (c < 10) {
		char *buffer = new char[2];
		sprintf(buffer, "%d", c);
		strcat(bnp, "00\0");
		strcat(bnp, buffer);
		strcat(bnp, ".bin\0");
		delete [] buffer;
	} else if (c < 100) {
		char *buffer = new char[3];
		sprintf(buffer, "%d", c);
		strcat(bnp, "0\0");
		strcat(bnp, buffer);
		strcat(bnp, ".bin\0");
		delete [] buffer;
	} else if (c < 1000) {
		char *buffer = new char[4];
		sprintf(buffer, "%d", c);
		strcat(bnp, buffer);
		strcat(bnp, ".bin\0");
		delete [] buffer;
	} else {
		strcat(bnp, "999.bin\0");
	}
}

// woking directory
std::string ctr::cwdir() {
	char buffer[FILENAME_MAX];
	get_cwd(buffer, FILENAME_MAX);
	std::string dir(buffer);
	return dir;
}

// execution directory
std::string ctr::exdir() {
	char buffer[FILENAME_MAX];
#if defined WIN32 || defined _WIN32
	return std::string(buffer, GetModuleFileName(NULL, buffer, FILENAME_MAX));
#else
	ssize_t count = readlink("/proc/self/exe", buffer, FILENAME_MAX);
	return std::string(buffer, (count > 0) ? count : 0);
#endif
}

// file size
#if defined WIN32 || defined _WIN32
__int64 ctr::file_size(const char* fp) {
	WIN32_FILE_ATTRIBUTE_DATA fad;
	if (!GetFileAttributesEx(fp, GetFileExInfoStandard, &fad)) {
		return -1;
	}
	LARGE_INTEGER fsize;
	fsize.HighPart = fad.nFileSizeHigh;
	fsize.LowPart = fad.nFileSizeLow;
	return fsize.QuadPart;
}
#else
std::ifstream::pos_type ctr::file_size(const char *path) {
	std::ifstream ifile (path, std::ifstream::ate | std::ifstream::binary);
	std::ifstream::pos_type result = ifile.tellg();
	ifile.close();
	return result;
}
#endif

// list dir
void ctr::list_dir(const char *path, std::queue<std::string> &listdir) {
	DIR *dp;
	struct dirent *ep;
	dp = opendir(path);

	if (dp != NULL) {
		while (ep = readdir(dp)) {
			if (strcmp(ep->d_name, ".") && strcmp(ep->d_name, "..")) {
				listdir.push(std::string(ep->d_name));
			}
		}
	
		(void) closedir (dp);
	}
}

// is file
bool ctr::is_file(const char *path) {
#if defined WIN32 || defined _WIN32
	DWORD attr = GetFileAttributes(path);
	if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY)) {
		return false;
	}

	return true;
#else
	struct stat statpath;

	if (stat(path, &statpath) != 0) {
		return false;
	}

	return S_ISREG(statpath.st_mode);
#endif
}

// is directory
bool ctr::is_directory(const char *path) {
#if defined WIN32 || defined _WIN32
	DWORD attr = GetFileAttributes(path);
	if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY)) {
		return true;
	}

	return false;
#else
	struct stat statpath;

	if (stat(path, &statpath) != 0) {
		return false;
	}
	
	return S_ISDIR(statpath.st_mode);
#endif
}

// create file
void ctr::create_file(const char *path, const char *buffer, std::streamsize buffersize) {
	std::ofstream file(path, std::ios::binary);
	file.write(buffer, buffersize);
	file.close();
}

// create file with content from file
void ctr::create_file_cf(const char *path, std::ifstream &ifile, std::streamsize datasize) {
	std::ofstream ofile(path, std::ios::binary);

	char *buffer = new char[IOS_LIMIT_SIZE];
	while (datasize > IOS_LIMIT_SIZE) {
		ifile.read(buffer, IOS_LIMIT_SIZE);
		ofile.write(buffer, IOS_LIMIT_SIZE);
		
		datasize -= IOS_LIMIT_SIZE;
	}
		
	ifile.read(buffer, datasize);
	ofile.write(buffer, datasize);

	delete [] buffer;
	ofile.close();
}

void ctr::write_file_cf(std::ofstream &ofile, std::ifstream &ifile, std::streamsize datasize) {
	char *buffer = new char[IOS_LIMIT_SIZE];
	while (datasize > IOS_LIMIT_SIZE) {
		ifile.read(buffer, IOS_LIMIT_SIZE);
		ofile.write(buffer, IOS_LIMIT_SIZE);
		
		datasize -= IOS_LIMIT_SIZE;
	}
		
	ifile.read(buffer, datasize);
	ofile.write(buffer, datasize);

	delete [] buffer;
}

// append file
void ctr::append_file(const char *path, const char *buffer, std::streamsize buffersize) {
	std::ofstream file(path, std::ofstream::ate | std::ofstream::binary);
	file.write(buffer, buffersize);
	file.close();
}

// create folder
void ctr::create_folder(const char *path) {
#if defined WIN32 || defined _WIN32
	CreateDirectory(path, NULL);
#else
	mkdir(path, 0777);
#endif
}

//--------------------------------------------------------------------------------
// For command: mge
// Backup

// backup file
void ctr::bkp_file(const char *filename, const char *path, std::ofstream &ofile) {
	// size of filename
	int filenamesize = strlen(filename);
	
	// build filepath
	char *filepath = new char [filenamesize + strlen(path) + 1];
	strcpy(filepath, "\0");
	strcat(filepath, path);
	strcat(filepath, filename);
	
	show(filepath);
	
	// file size
	long long int filesize = file_size(filepath);
	
	// write header info
	ofile.write(reinterpret_cast<char *>(&filenamesize), sizeof(int));
	ofile.write(filename, filenamesize);
	ofile.write(reinterpret_cast<char *>(&filesize), sizeof(long long int));
	
	// write data
	std::ifstream ifile(filepath, std::ios::binary);
	char *buffer = new char[IOS_LIMIT_SIZE];
	while (filesize > IOS_LIMIT_SIZE) {
		ifile.read(buffer, IOS_LIMIT_SIZE);
		ofile.write(buffer, IOS_LIMIT_SIZE);
		
		filesize -= IOS_LIMIT_SIZE;
	}
	
	ifile.read(buffer, filesize);
	ofile.write(buffer, filesize);
	
	// close file and release memory
	ifile.close();
	delete [] buffer;
	delete [] filepath;
}

// backup directory	
void ctr::bkp_directory(const char *foldername, const char *path, std::ofstream &ofile) {
	// size of foldername
	int foldernamesize = strlen(foldername);
	
	// folderpath size
	int folderpathsize = foldernamesize + strlen(path) + 1;

	// build folderpath
	char *folderpath = new char[folderpathsize + 1];
	strcpy(folderpath, "\0");
	strcat(folderpath, path);
	strcat(folderpath, foldername);
	strcat(folderpath, SEPERATE);
	
	show(folderpath);
	
	// folder size
	long long int foldersize = -1;
		
	// write header info
	ofile.write(reinterpret_cast<char *>(&foldernamesize), sizeof(int));
	ofile.write(foldername, foldernamesize);
	ofile.write(reinterpret_cast<char *>(&foldersize), sizeof(long long int));

	// list dir
	std::queue<std::string> listdir;
	list_dir(folderpath, listdir);
		
	// write item count
	int itemcount = listdir.size();
	ofile.write(reinterpret_cast<char *>(&itemcount), sizeof(int));
		
	while (!listdir.empty()) {
		char *itempath = new char[folderpathsize + listdir.front().size() + 1];
		strcpy(itempath, "\0");
		strcat(itempath, folderpath);
		strcat(itempath, listdir.front().c_str());
			
		if (is_directory(itempath)) {
			bkp_directory(listdir.front().c_str(), folderpath, ofile);
		} else if (is_file(itempath)) {
			bkp_file(listdir.front().c_str(), folderpath, ofile);
		}
		
		delete [] itempath;
		listdir.pop();
	}
	
	delete [] folderpath;
}

// backup item
void ctr::bkp_items(char **items, int itemcount, const char *outfile) {
	std::ofstream ofile(outfile, std::ios::binary);
	ofile.write(reinterpret_cast<char *>(&itemcount), sizeof(int));
	while (itemcount --) {
		struct path_dt pdt;
		dt_path(items[itemcount], pdt);
		
		if (is_directory(items[itemcount])) {
			bkp_directory(pdt.item, pdt.dir, ofile);
		} else if (is_file(items[itemcount])) {
			bkp_file(pdt.item, pdt.dir, ofile);
		}
	}

	ofile.close();
}

//--------------------------------------------------------------------------------
// For command: spr
// Restore

// restore item
void ctr::rst_item(std::ifstream &ifile, const char *outpath) {
	// item name size
	int itemnamesize;
	ifile.read(reinterpret_cast<char *>(&itemnamesize), sizeof(int));
		
	// item name
	char *itemname = new char[itemnamesize + 1];
	ifile.read(itemname, itemnamesize);
	*(itemname + itemnamesize) = '\0';
		
	// item size
	long long int itemsize;
	ifile.read(reinterpret_cast<char *>(&itemsize), sizeof(long long int));
		
	// build itempath
	char *itempath = new char[itemnamesize + strlen(outpath) + 2];
	strcpy(itempath, "\0");
	strcat(itempath, outpath);
	strcat(itempath, itemname);
			
	if (itemsize != -1) {
		show(itempath);
		create_file_cf(itempath, ifile, itemsize);
	} else {
		strcat(itempath, SEPERATE);
		show(itempath);
		create_folder(itempath);
		rst_item_h(ifile, itempath);
	}
	
	delete [] itempath;
	delete [] itemname;
}

// restore item header	
void ctr::rst_item_h(std::ifstream &ifile, const char *outpath) {
	int itemcount;
	ifile.read(reinterpret_cast<char *>(&itemcount), sizeof(int));
	while (itemcount --) {
		rst_item(ifile, outpath);
	}
}

// restore items
void ctr::rst_items(const char *mgefile, const char *outdir) {
	std::ifstream ifile(mgefile, std::ios::binary);
	rst_item_h(ifile, outdir);
	ifile.close();
}

//--------------------------------------------------------------------------------
// For command: spl
// Split

// split file
void ctr::spl_file(const char *ifp, long long int sfs, const char *odir) {
	long long int fs = file_size(ifp);			// file size
	std::ifstream ifile(ifp, std::ios::binary);	// open input file

	long long int e = fs%sfs;	// balance size
	short int n = fs/sfs; 		// split file count
	
	// ofn
	struct path_dt *pdt = new struct path_dt;
	dt_path(ifp, *pdt);
	struct bname_dt *bdt = new struct bname_dt;
	dt_bname(pdt->item, *bdt);
	
	// build base split file path
	int ofps = strlen(odir) + strlen(bdt->name) + 8;
	char *ofp = new char[ofps];
	strcpy(ofp, odir);
	strcat(ofp, bdt->name);
	
	// create header file
	sfbp(ofp, 0);
	std::ofstream ofile(ofp, std::ios::binary);
	
	// write header file
	ofile.write(reinterpret_cast<char *>(&n), sizeof(short int));
	ofile.write(reinterpret_cast<char *>(&sfs), sizeof(long long int));
	int ifns = strlen(pdt->item);
	ofile.write(reinterpret_cast<char *>(&ifns), sizeof(int));
	ofile.write(pdt->item, ifns);
	ofile.write(reinterpret_cast<char *>(&e), sizeof(long long int));
	write_file_cf(ofile, ifile, e);
	delete bdt;
	delete pdt;
	ofile.close();
	// file
	
	for (short int i = 1; i <= n; i ++) {
		*(ofp + ofps - 8) = '\0';
		sfbp(ofp, i);
		create_file_cf(ofp, ifile, sfs);
	}
	
	delete [] ofp;
}

void ctr::cbn_file(const char *ifbp, const char *odir) {
	int ifbps = strlen(ifbp);
	char *ifp = new char[ifbps + 8];
	strcpy(ifp, ifbp);
	strcat(ifp, "000.bin\0");
	
	std::ifstream ifile(ifp, std::ios::binary);
	short int n = 0;
	ifile.read(reinterpret_cast<char *>(&n), sizeof(short int));
	long long int sfs = 0;
	ifile.read(reinterpret_cast<char *>(&sfs), sizeof(long long int));
	int ifns = 0;
	ifile.read(reinterpret_cast<char *>(&ifns), sizeof(int));
	char *ifn = new char[ifns + 1];
	ifile.read(ifn, ifns);
	long long int e = 0;
	ifile.read(reinterpret_cast<char *>(&e), sizeof(long long int));
	
	char *ofp = new char[strlen(odir) + ifns + 1];
	strcpy(ofp, odir);
	strcat(ofp, ifn);
	delete [] ifn;
	
	std::ofstream ofile(ofp, std::ios::binary);
	write_file_cf(ofile, ifile, e);
	ifile.close();
	
	for (short int i = 1; i <= n; i ++) {
		*(ifp + ifbps) = '\0';
		sfbp(ifp, i);
		ifile.open(ifp, std::ios::binary);
		write_file_cf(ofile, ifile, sfs);
		ifile.close();
	}
	
	ofile.close();
	delete [] ifp;
}