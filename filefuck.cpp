#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

static  int 	offset = 8;
char 	chunk[8] = {0};
using 	namespace std;

void usage (char *arg) {
	printf ("***** Warning *****\n");
	printf ("This program can corrupt files in your computer.\n");
	printf ("Corrupt: %s <filename> 0\n", arg);
	printf ("Restore: %s <filename> 1\n", arg);
}

int write2file (char *output, char *input, int size, bool mode) {

	int len = strlen(output);
	char buf[len+5];
	snprintf(buf, len+5, "%s.tmp", output);
	long pos;
	cout << buf << " - " << endl;

	if (!mode) {
	  	ofstream myfile (buf, ios::out | ios::app | ios::binary);

	  	strncpy(chunk, input, sizeof(chunk));
		printf("Chunk: %s Len:%d Bytes.\n", chunk, sizeof(chunk));

	  	*input += offset;
	  	myfile.write(input, size-offset);

	  	pos = myfile.tellp();
	  	myfile.seekp(pos);
	    myfile.write(chunk, offset);
	  	myfile.close();
  	} else {
  		cout << "####" << endl;
  		ofstream myfile (buf, ios::out | ios::app | ios::binary);
  		*input += (size-offset);
  		//seekp (0L, ios::beg);

  		printf("Chunk: %s Len:%d Bytes.\n", input, offset);

  		myfile.write (input, offset);
  		*input -= (size-offset);
		myfile.write (input, size-offset);
  		myfile.close();

  	}
  	if (remove(output) != 0)
    	cout << "Error deleting file: " << output;

    rename(buf, output);

  	return 0;
 
}

int operation(char *file, bool mode);
int main (int argc, char **argv) {

	if (argc < 3) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	operation(argv[1], atoi(argv[2]));
	
	return 0;
}


int operation(char *filename, bool mode) {

	streampos size;
	char *memblock;
	long pos;

	if (mode == 0) {	

		ifstream file(filename, ios::in | ios::binary | ios::ate);
		if (file.is_open())
		{
			/* Get the final position of file*/
			size = file.tellg(); 
			/* Allocate memory to read the entire file */
			memblock = new char[size];
			/*  Set the position to the beginning */
			file.seekg(0, ios::beg);
			/* read all file into bufer */
			file.read(memblock, size);
			
			write2file(filename, memblock, size, mode);

			file.close();
			cout << size  << " bytes has been read." << endl;
			delete[] memblock;
			  			
		} else {
			cout << "Unable to open file: " << filename;
		}
	} else {
		cout << "Recovering file..."<< endl;
		ifstream file(filename, ios::in | ios::binary | ios::ate);
		if (file.is_open())
		{	/*  Set the position to the end */
			/* Get the final position of file*/
			size = file.tellg(); 
			/* Allocate memory to read the entire file */
			memblock = new char[size];
			/*  Set the position to the beginning */
			file.seekg(0, ios::beg);
			/* read all file into bufer */
			file.read(memblock, size);
			/* Moving pointer to get the last 8 bytes */
			
			write2file(filename, memblock, size, mode);

			file.close();
			cout << size  << " bytes has been read." << endl;
			delete[] memblock;
			  			
		} else {
			cout << "Unable to open file: " << filename;	
		}
	}
}