#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

//Struct for key(first 4 bytes) and value(next 96 bytes)
typedef struct{
	char key[5];     //1 extra bit for NULL terminating char
	char value[97];  //1 extra bit for NULL terminating char
} Record;

// Struct for list of Records
typedef struct{
	Record *records;
	int size;
} Records;

Records partitionRecords(Records rs, int first, int last){
	int size = last - first;
	//printf("in partitionRecords, size: %d\n", size);
	//printf("first: %d, last: %d\n", first, last);
	Record *new_rs = (Record*) malloc(size * sizeof(Record));
	for (int i = 0; i < size; i++){
		new_rs[i] = rs.records[first+i];
	}
	Records r = {new_rs, size};
	//printRecords(r);
	return r;
}

void printRecords(Records rs){
	printf("printing records of size %d\n", rs.size);
	for (int i = 0; i < rs.size; i++){
		printf("\trecord[%d] key: %s\tvalue: %s\n", i, rs.records[i].key, rs.records[i].value);
	}
}

//function to merge both halves
Records merge(Records a, Records b){
	printf("\n in merge \n");
	printf("printing a\n");
	printRecords(a);
	printf("printing b\n");
	printRecords(b);
	Record* m = (Record*) malloc((a.size + b.size) * sizeof(Record));
	int i_a = 0;
	int i_b = 0;
	int m_i = 0;
	//printf("i_a: %d\ta.size: %d\ti_b: %d\tb.size: %d\n", i_a, a.size, i_b, b.size);
	while (i_a < a.size && i_b < b.size){
		printf("a key: %s\n", a.records[i_a].key);
		printf("b key: %s\n", b.records[i_b].key);
		if (strcmp(a.records[i_a].key, b.records[i_b].key) <= 0){
			Record r = { a.records[i_a].key, a.records[i_a++].value};
			m[m_i++] = r;
		} else {
			Record r = { b.records[i_b].key, b.records[i_b++].value};
			m[m_i++] = r;
		}
	}
	Records merged_r = {m, m_i++};
	// printf("in merge\n");
	// printRecords(merged_r); 
	return merged_r;
}

//recursive function that splits array into two halves and calls itself on each half
//https://www.geeksforgeeks.org/c-program-for-merge-sort/
Records merge_sort(Records rs){
	int left = 0;
	int right = rs.size;
	printf("\nleft: %d\t right: %d\n", left, right);
	if(right > 1){
		int mid = (right + 1) / 2;
		printf("mid: %d\n", mid);

		Records a = merge_sort(partitionRecords(rs, left, mid));
		//printf("\ndone with a\n");
		Records b = merge_sort(partitionRecords(rs, mid, right));
		//printf("\ndone with b\n");

		return merge(a, b);
	}
	//printRecords(rs);
	return rs;

}


// void merge_sort(Records rs){
// 	merge_sort_helper(rs);
// }


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
	//int num_records = input_file_size / sizeof(Record);
	int num_records = (input_file_size + sizeof(Record) - 1) / sizeof(Record);

	//printf("Records: %d\n",num_records);

	//Create array of Records to hold input data
	Record* records = (Record*) malloc(num_records * sizeof(Record));

    // Read the input data file into an array of records
    for (int i = 0; i < num_records; i++) {

        fread(records[i].key, sizeof(char), 4, input_file);
        records[i].key[5] = '\0';
        fread(records[i].value, sizeof(char), 96, input_file);
        records[i].value[97] = '\0';
       // printf("Read record %d: key=%s, value=%s\n", i+1, records[i].key, records[i].value); 
        fseek(input_file, 1, SEEK_CUR);
    }

    //Close the input file
    fclose(input_file);

    // Print out each record in the array.
  //  for (int i = 0; i < num_records; i++) {
  //      printf("Record %d: key=%s, value=%s\n", i+1, records[i].key, records[i].value);
  //  }

  //UP TO HERE IT PRINTS
  //Record 1: key=aaaa, value=vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	//Record 2: key=cccc, value=yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
	//Record 3: key=bbbb, value=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

	Records rs = {records, num_records};

	Records res = merge_sort(rs); //FIX MERGE FUNCTION 

	//printf("num_records: %d\n", num_records);
	//printf("res.size: %d\n", res.size);

	printRecords(res);

	free(records);

	return 0;


}