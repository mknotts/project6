#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//Struct for key(first 4 bytes) and value(next 96 bytes)
typedef struct{
	char record[101];	// pointer to record
	int key;			// key for record
} Record;

// Struct for list of Records
typedef struct{
	Record *records;
	int size;
} Records;

Records partitionRecords(Records rs, int first, int last){
	int size = last - first;
	Record *new_rs = (Record*) malloc(size * sizeof(Record));
	for (int i = 0; i < size; i++){
		new_rs[i] = rs.records[first+i];
	}
	Records r = {new_rs, size};
	return r;
}

void printRecords(Records rs){
	printf("printing records of size %d\n", rs.size);
	for (int i = 0; i < rs.size; i++){
		printf("\trecord[%d]: %s\n", i, rs.records[i].record);
	}
}

//function to merge both halves
Records merge(Records a, Records b){
	Record* m = (Record*) malloc((a.size + b.size) * sizeof(Record));
	int i_a = 0;
	int i_b = 0;
	int m_i = 0;
	while (i_a < a.size && i_b < b.size){
		if (a.records[i_a].key < b.records[i_b].key){
			m[m_i++] = a.records[i_a++];
		} else {
			m[m_i++] = b.records[i_b++];
		}
	}

	if (i_a == a.size && i_b != b.size){
		for (int i = i_b; i < b.size; i++){
			m[m_i++] = b.records[i];
		}
	} else if (i_a != a.size && i_b == b.size) {
		for (int i = i_a; i < a.size; i++){
			m[m_i++] = a.records[i];
		}
	}
	Records merged_r = {m, m_i++}; 
	return merged_r;
}

//recursive function that splits array into two halves and calls itself on each half
//https://www.geeksforgeeks.org/c-program-for-merge-sort/
Records merge_sort(Records rs){
	int left = 0;
	int right = rs.size;
	if(right > 1){
		int mid = (right + 1) / 2;

		Records a = merge_sort(partitionRecords(rs, left, mid));
		Records b = merge_sort(partitionRecords(rs, mid, right));

		return merge(a, b);
	}
	return rs;

}

int main(int argc, char** argv) {
	
	// Read command-line arguments
	// prompt> ./psort input output 4
    const char* input_filename = argv[1];
    const char* output_filename = argv[2];
    int num_threads = strtol(argv[3], NULL, 10);


  // Open input file 
	FILE* input_file = fopen(input_filename, "rb");

	//NULL CHECK
	if(input_file == NULL){
		fprintf(stderr, "Could not open input file '%s'\n", input_filename);
		exit(EXIT_FAILURE);
	}

	// Get the size of the input file
	fseek(input_file, 0, SEEK_END);
	int input_file_size = ftell(input_file);


	// Set the file pointer back to the beginning
	fseek(input_file, 0, SEEK_SET);

	// Calculate the number of records in the file
	int num_records = input_file_size / 100;

	//Create array of Records to hold input data
	Record* records = (Record*) malloc(num_records * 100 * sizeof(Record));

    // Read the input data file into an array of records
    for (int i = 0; i < num_records; i++) {
        fread(records[i].record, sizeof(char), 100, input_file);
		char * r = (char*) records[i].record;
		int key = *(int*)r;
       	records[i].key = key;
        fseek(input_file, 1, SEEK_CUR);
    }

    //Close the input file
    fclose(input_file);

	Records rs = {records, num_records};
	printf("before merge\n");
	printRecords(rs);

	Records res = merge_sort(rs); //FIX MERGE FUNCTION 

	printf("after merge\n");
	printRecords(res);


	//Printing to output file
FILE* output_file = fopen(output_filename, "w");
if(output_file == NULL){
    fprintf(stderr, "Could not open output file '%s'\n", output_filename);
    exit(EXIT_FAILURE);
}

for(int i = 0; i < res.size; i++) {
    fprintf(output_file, "%s\n", res.records[i].record);
}

fclose(output_file);

	free(records);
	free(res.records);

	return 0;


} 


