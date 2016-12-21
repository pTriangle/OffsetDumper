#include <stdio.h>
#include <stdlib.h>

// Extracts data from a file using a specified range [startoffset, endoffset] and prints it to the screen.
// It also writes extracted data to a text file, and produces a binary output in case you wanted to throw
// it into a hex editor for further analyzing for whatever purpose.
// Ex: main.exe in_file out_file start_offset end_offset

int main(int argc, char *argv[]){
	FILE *fd[3];
	unsigned char *buf;
    size_t result;
    int i, a[2];
    char name[50];

	/* Self explanatory, checks to make sure the valid amount of arguments has been passed */
	if(argc != 5){
		printf("Usage: %s in_file out_file start_offset end_offset", argv[0]);
		return -1;
	}

	/* Open up the binary file to extract data from*/
	fd[0] = fopen(argv[1], "rb");
	if(fd[0] == NULL){
		printf("Error opening file %s", argv[1]);
		fclose(fd[0]);
		return -1;
	}

	/* Grab the length */
	fseek(fd[0], 0, SEEK_END);
	size_t len = ftell(fd[0]);
	fseek(fd[0], 0, SEEK_SET);
	printf("Length: 0x%x\n", len);

	/* Allocate into memory */
	buf = malloc(len);
	if(buf == NULL){
		printf("Error allocating data");
		free(buf);
		fclose(fd[0]);
		return -1;
	}

	/* Read file contents into allocated memory buffer */
    result = fread(buf,1,len,fd[0]);
    if(result != len){
        printf("Error reading file");
        free(buf);
		fclose(fd[0]);
        return -1;
    }

	/* Convert strings into 'long' values since argv[i] is considered as being a string */
    for(i=0;i<=1;i++){
    a[i] = strtol(argv[3+i],NULL,16);
    }

	/* Create the text file to write the output to */
    fd[1] = fopen(argv[2],"w");
    if(fd[1] == NULL){
		printf("Error creating text output %s", argv[2]);
        for(i = 0; i <= 1 ; i++){
        fclose(fd[i]);
        }
		return -1;
	}

	/* Create the binary file to write the output to */
    sprintf(name,"%s.bin",argv[2]);
    fd[2] = fopen(name,"wb");
    if(fd[2] == NULL){
		printf("Error creating binary output %s", name);
        for(i = 0; i <= 2 ; i++){
        fclose(fd[i]);
        }
		return -1;
	}

	/* Main functionality of program is here */
	for(i = a[0]; i <= a[1]; i++){
        printf("Offset: 0x%02x\n",buf[i]);			// Print text to screen
        fprintf(fd[1],"Offset: 0x%02x\n",buf[i]);	// Write text to file
        fwrite(&buf[i],sizeof(char),1,fd[2]);		// Write binary data to file
	}

	/* free and close everything, we're done now :) */
	free(buf);
	for(i = 0; i <= 2 ; i++){
	fclose(fd[i]);
	}
	return 0;
}
