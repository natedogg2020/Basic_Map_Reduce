#include "reducer.h"

// Instance variable to keep track of root node.
finalKeyValueDS *DS;

// create a key value node
finalKeyValueDS *createFinalKeyValueNode(char *word, int count){
	finalKeyValueDS *newNode = (finalKeyValueDS *)malloc (sizeof(finalKeyValueDS));
	strcpy(newNode -> key, word);
	newNode -> value = count;
	newNode -> next = NULL;
	return newNode;
}

// insert or update an key value
finalKeyValueDS *insertNewKeyValue(finalKeyValueDS *root, char *word, int count){
	finalKeyValueDS *tempNode = root;
	if(root == NULL)
		return createFinalKeyValueNode(word, count);
	while(tempNode -> next != NULL){
		if(strcmp(tempNode -> key, word) == 0){
			tempNode -> value += count;
			return root;
		}
		tempNode = tempNode -> next;
	}
	if(strcmp(tempNode -> key, word) == 0){
		tempNode -> value += count;
	} else{
		tempNode -> next = createFinalKeyValueNode(word, count);
	}
	return root;
}

// free the DS after usage. Call this once you are done with the writing of DS into file
void freeFinalDS(finalKeyValueDS *root) {
	if(root == NULL) return;

	finalKeyValueDS *tempNode = NULL;
	while (root != NULL){
		tempNode = root;
		root = root -> next;
		free(tempNode);
	}
}

// reduce function
void reduce(char *key) {
	// setup to read words and counts
	int wordCount =0; // Counts 1's after word
	char word[MAXKEYSZ]; // Word is stored here
	int numb; // Temp variable for scannning 1s
	FILE *inputfd; // File input
	
	// Open file
	inputfd = fopen(key, "r");
	// Exit and print error if unable to open file
	if(inputfd == NULL){
		printf("Error: Unable to open map file %s", key);
		exit(0);
	}

	// Fetch word
	fscanf(inputfd, "%s", word);
	
	// Loop until end of file and count 1's
	while(fscanf(inputfd, "%d", &numb) != EOF){
		wordCount++;
	}

	// Close file
	fclose(inputfd);

    // //update the node with new word count
    DS = insertNewKeyValue(DS, word, wordCount);
}

// write the contents of the final intermediate structure
// to output/ReduceOut/Reduce_reducerID.txt
void writeFinalDS(int reducerID){
	// Setup file and filepath needed to create files
	FILE *outputFd;
	char filepath[MAXKEYSZ];

	// Create temp pointer to root node in tree
	finalKeyValueDS* tempDS = DS;

	// Update file name given the reducerID
	sprintf(filepath, "./output/ReduceOut/Reducer_%d.txt", reducerID);
	
	// Opening file, and printing error if unable to open/create file 
	outputFd = fopen(filepath, "w");
	if(outputFd == NULL){
		printf("Error: Unable to open reducer file %s", filepath);
		exit(0);
	}

	// loop through tree and write to reducer files
	while(tempDS != NULL)
    {
        // write the word and it's count value then move to next pointer.
		if(fprintf(outputFd, "%s %d\n", tempDS->key, tempDS->value) < 0){
			printf("Error writing key: %s value: %d to file: %s\n", tempDS->key, tempDS->value, filepath);
			exit(0);
		}
        tempDS = tempDS -> next;
    }

	// Make sure no errors have occured while writing
	if(ferror(outputFd)){
		printf("Error writing to file: %s", filepath);
		exit(0);
	}
	// Close file and free pointers
    fclose(outputFd);
    freeFinalDS(tempDS);
	freeFinalDS(DS);
}

int main(int argc, char *argv[]) {

	if(argc < 2){
		printf("Less number of arguments.\n");
		printf("./reducer reducerID");
	}

	// ###### DO NOT REMOVE ######
	// initialize 
	int reducerID = strtol(argv[1], NULL, 10);

    //Print statement for reducer, comment this for final submission
    // printf("Reducer id : %d \n",reducerID);

	// ###### DO NOT REMOVE ######
	// master will continuously send the word.txt files
	// alloted to the reducer
	char key[MAXKEYSZ];
	while(getInterData(key, reducerID))
		reduce(key);

	// You may write this logic. You can somehow store the
	// <key, value> count and write to Reduce_reducerID.txt file
	// So you may delete this function and add your logic
	writeFinalDS(reducerID);

	return 0;
}
