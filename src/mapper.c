#include "mapper.h"

// Instance variable to keep track of root node.
intermediateDS *DS = NULL;

// combined value list corresponding to a word <1,1,1,1....>
valueList *createNewValueListNode(char *value){
	valueList *newNode = (valueList *)malloc (sizeof(valueList));
	strcpy(newNode -> value, value);
	newNode -> next = NULL;
	return newNode;
}

// insert new count to value list
valueList *insertNewValueToList(valueList *root, char *count){
	valueList *tempNode = root;
	if(root == NULL)
		return createNewValueListNode(count);
	while(tempNode -> next != NULL)
		tempNode = tempNode -> next;
	tempNode -> next = createNewValueListNode(count);
	return root;
}

// free value list
void freeValueList(valueList *root) {
	if(root == NULL) return;

	valueList *tempNode = NULL;
	while (root != NULL){
		tempNode = root;
		root = root -> next;
		free(tempNode);
	}
}

// create <word, value list>
intermediateDS *createNewInterDSNode(char *word, char *count){
	intermediateDS *newNode = (intermediateDS *)malloc (sizeof(intermediateDS));
	strcpy(newNode -> key, word);
	newNode -> value = NULL;
	newNode -> value = insertNewValueToList(newNode -> value, count);
	newNode -> next = NULL;
	return newNode;
}

// insert or update a <word, value> to intermediate DS
intermediateDS *insertPairToInterDS(intermediateDS *root, char *word, char *count){
	intermediateDS *tempNode = root;
	if(root == NULL)
		return createNewInterDSNode(word, count);
	while(tempNode -> next != NULL) {
		if(strcmp(tempNode -> key, word) == 0){
			tempNode -> value = insertNewValueToList(tempNode -> value, count);
			return root;
		}
		tempNode = tempNode -> next;
		
	}
	if(strcmp(tempNode -> key, word) == 0){
		tempNode -> value = insertNewValueToList(tempNode -> value, count);
	} else {
		tempNode -> next = createNewInterDSNode(word, count);
	}
	return root;
}

// free the DS after usage. Call this once you are done with the writing of DS into file
void freeInterDS(intermediateDS *root) {
	if(root == NULL) return;

	intermediateDS *tempNode = NULL;
	while (root != NULL){
		tempNode = root;
		root = root -> next;
		freeValueList(tempNode -> value);
		free(tempNode);
	}
}

// emit the <key, value> into intermediate DS 
void emit(char *key, char *value) {
	DS = insertPairToInterDS(DS, key, value);
}

// map function
void map(char *chunkData){
	// you can use getWord to retrieve words from the 
	// chunkData one by one. Example usage in utils.h
	int i = 0;
	char *buffer;
	while ((buffer = getWord(chunkData, &i)) != NULL){
		emit(buffer, "1");
		free(buffer);
	}
}

// write intermediate data to separate word.txt files
// Each file will have only one line : word 1 1 1 1 1 ...
void writeIntermediateDS() {
	// Make sure DS has been utilized
	if(DS == NULL){
		printf("Mapper's intermediate DS was never used!");
	}
	
	// Setup file writing
	FILE* outputFd;
	char* filepath;
	int pathLength = MAXFILELEN; //assumptions: maximum size of a file path to be 50 bytes per writeup
	int createdPathLength = 0;
	

	intermediateDS* tempDS = DS;
	valueList*		tempVal;

	// Loop through all DS in linked list
	while(tempDS != NULL){
		filepath = (char*) malloc(pathLength * sizeof(char));
		//Create filepath for current word
		createdPathLength = snprintf(filepath, pathLength, "%s/%s.txt", mapOutDir, tempDS->key);

		//Make sure filepath wasn't written longer than intended
		if(createdPathLength >= pathLength){
			printf("Error, path length mismatch! Your filepath may be too long.");
			exit(0);
		}

		if((outputFd = fopen(filepath, "w+")) == NULL){
			printf("Could not open file: %s", filepath);
			exit(0);
		}

		fputs(tempDS->key, outputFd); // Write CurrentWord into file
		if(ferror(outputFd)){
			printf("Error writing value: %s to file: %s", tempDS->key, filepath);
		}
		tempVal = tempDS->value; // Set val List as tempDS value root
		
		//Write all values except last into file
		while(tempVal->next != NULL){
			if(fprintf(outputFd, " %s", tempVal->value) < 0){
				printf("Error writing value: %s to file: %s", tempVal->value, filepath);
				exit(0);
			}
			
			tempVal = tempVal->next;
		}
		//Write last value into file
		if(fprintf(outputFd, " %s\n",tempVal->value ) < 0){
			printf("Error writing value: %s\"n to file: %s", tempVal->value, filepath);
			exit(0);
		}
		// Make sure no errors have occured while writing
		if(ferror(outputFd)){
			printf("Error writing to file: %s", filepath);
			exit(0);
		}
		
		tempDS = tempDS->next;
	}

	// Free data
	free(filepath);
	freeInterDS(tempDS);
	freeInterDS(DS);
	fclose(outputFd);
}

int main(int argc, char *argv[]) {
	
	if (argc < 2) {
		printf("Less number of arguments.\n");
		printf("./mapper mapperID\n");
		exit(0);
	}
	// ###### DO NOT REMOVE ######
	mapperID = strtol(argv[1], NULL, 10);

    //Print statement for mapper, comment this for final submission
    // printf("Mapper id : %d \n",mapperID);

	// ###### DO NOT REMOVE ######
	// create folder specifically for this mapper in output/MapOut
	// mapOutDir has the path to the folder where the outputs of 
	// this mapper should be stored
	mapOutDir = createMapDir(mapperID);

	// ###### DO NOT REMOVE ######
	while(1) {
		// create an array of chunkSize=1024B and intialize all 
		// elements with '\0'
		char chunkData[chunkSize + 1]; // +1 for '\0'
		memset(chunkData, '\0', chunkSize + 1);

		char *retChunk = getChunkData(mapperID);
		if(retChunk == NULL) {
			break;
		}

		strcpy(chunkData, retChunk);
		free(retChunk);

		map(chunkData);
	}

	// ###### DO NOT REMOVE ######
	writeIntermediateDS();

    free(mapOutDir);
	return 0;
}
