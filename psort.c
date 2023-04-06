#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



//Struct for key(first 4 bytes) and value(next 96 bytes)
typedef struct{
	char key[5];
	char value[97];
} Record;


//function to merge both halves
void merge(Record *arr, int left, int mid, int right){


}

//recursive function that splits array into two halves and calls itself on each half
//https://www.geeksforgeeks.org/c-program-for-merge-sort/
void merge_sort_helper(Record *arr, int left, int right){
	if(right < left){
		int mid = left + (right-left) / 2;

		merge_sort_helper(arr, left, mid);
		merge_sort_helper(arr,mid+1, right);

		merge(arr, left, mid, right);
	}

}


void merge_sort(Record *arr, int num_records){
	merge_sort_helper(arr,0,num_records-1);
}


int main(int argc, char** argv) {


	// Read command-line arguments
	// prompt> ./psort input output 4
    const char* input_filename = argv[1];
    const char* output_filename = argv[2];
    int num_threads = strtol(argv[3], NULL, 10);


    // Open input file in binary mode
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


	merge_sort(records, num_records); //FIX MERGE FUNCTION



	free(records);

	return 0;


}