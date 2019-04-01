#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct movie {
	char *id;
	char *titleType;
	char *primaryTitle;
	char *originalTitle;
	int isAdult;
	int startYear;
	int endYear;
	int runTime;
	char *genre;
} movie;

typedef struct node {
	char *key;
	movie *value;
	struct node *left;
	struct node *right;
	int height;
} node;

const bool AUTO_BAL = true;

char *stripArticle(char *input) {
	int length = strlen(input);
	for (int i = 0; i < length; i++) input[i] = tolower(input[i]);
	char *newInput = NULL;
//	char *tok;
//	char *vals[100];
	//int idx = 0;

	if (strncmp(input, "the ", 4) == 0) {
		newInput = strchr(input, ' ');
		//remove whitespace in front of string...dunno why its doing that
		if (newInput[0] == ' ') newInput++;
		return newInput;
	}
	if (strncmp(input, "a ", 2) == 0) {
		newInput = strchr(input, ' ');
		//remove whitespace in front of string...dunno why its doing that
		if (newInput[0] == ' ') newInput++;
		return newInput;
	}


	return input;
}

node *createNode(char *line) {
	char *tok;

	movie *movieStruct = (movie *)malloc(sizeof(movie));

	if (movieStruct == NULL) {
		printf ("Out of memory, could not create Movie!!! (createNode)\n");
		exit(1);
	}

	char *vals[9];
	int idx = 0;

	/* get the first token */
	tok = strtok(line, "\t");

	/* walk the token list until we hit a NULL */
	while(tok != NULL) {
		//  printf("%s\n", tok);

		/* do something with first token */
		vals[idx] = strcmp(tok, "\\N") ? tok : "";
		idx++;
		tok = strtok(NULL, "\t");
	}

	movieStruct->id = vals[0];
	movieStruct->titleType = vals[1];
	movieStruct->primaryTitle = vals[2];
	movieStruct->originalTitle = vals[3];
	movieStruct->isAdult = atoi(vals[4]);
	movieStruct->startYear = atoi(vals[5]);
	movieStruct->endYear = atoi(vals[6]);
	movieStruct->runTime = atoi(vals[7]);
	movieStruct->genre = vals[8];

	node *theNode = (node *)malloc(sizeof(node));

	if (theNode == NULL) {
		printf ("Out of memory, could not create Node!!! (createNode)\n");
		exit(1);
	}

	theNode->value = movieStruct;
	theNode->key = stripArticle(movieStruct->primaryTitle);
	theNode->left = NULL;
	theNode->right = NULL;
	theNode->height = 1;


	return (theNode);
}

int max(int a, int b) {
	return (a > b)? a : b;
}

//return the height of the AVL tree
int height(struct node *n) {
	if (n == NULL)
		return 0;

	return n->height;
}

node *rightRotate(struct node *y) {
//	printf("\n\nright rotating : im working.\n");
	if (y->left != NULL) {
		struct node *x = y->left;
		struct node *t2 = x->right;

		// Perform rotation
		x->right = y;
		y->left = t2;

		// Update heights
		y->height = max(height(y->left), height(y->right))+1;
		x->height = max(height(x->left), height(x->right))+1;

		// Return new root
		return x;
	}
	else {
//	printf("\n\nLeft root was null, cannot rotate\n\n\n");
	return y;
	}
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
node *leftRotate(struct node *x) {
//	printf("\n\n left rotating : im working\n.");
	if (x->right != NULL) {
	struct node *y = x->right;
	struct node *t2 = y->left;

//	printf("\n\n left rotating : im working\n.");

	// Perform rotation
	y->left = x;
	x->right = t2;

	//  Update heights
	x->height = max(height(x->left), height(x->right))+1;
	y->height = max(height(y->left), height(y->right))+1;
	return y;
	}
	// Return new root
	else {
	//	printf("Node is null, cannot rotate \n\n\n");
		return x; }
}

int getBalance(struct node *n) {
	if (n == NULL)
		return 0;
	return height(n->left) - height(n->right);
}

node *_insert(node *startNode, node *nodeToAdd) {
	if (startNode == NULL) {
	//	printf("\nEmpty spot in tree found! Inserting..\n\n");
		return nodeToAdd;
	}

	if (strncmp(nodeToAdd->key, startNode->key, 100) <= 0) {
	//	printf("\n\nGoing left..\n\n");
		startNode->left = _insert(startNode->left, nodeToAdd);
	}
	else if (strncmp(nodeToAdd->key, startNode->key, 100) > 0) {
	//	printf("\n\nGoing right...\n\n");
	  startNode->right = _insert(startNode->right, nodeToAdd); }

//	printf("I'm still working..\n");

	/* 2. Update height of this ancestor node */
	startNode->height = 1 + max(height(startNode->left), height(startNode->right));


	int balance = getBalance(startNode);
//	printf("Node = %s :: Balance = %d\n", startNode->key, balance);

	// If this node becomes unbalanced, then there are 4 cases

	// Left Left Case
	if (balance > 1 && nodeToAdd->key < startNode->left->key) {
//		printf("LL case : rotating right \n");
		return rightRotate(startNode);
	}

	// Right Right Case
	if (balance < -1 && nodeToAdd->key > startNode->right->key) {
//		printf("RR case : rotating left\n");
		return leftRotate(startNode);
	}

	// Left Right Case
	if (balance > 1 && nodeToAdd->key > startNode->left->key) {
//		printf("LR case : rotating left... ");
		startNode->left = leftRotate(startNode->left);

	//	printf("rotating right\n");
		return rightRotate(startNode);
	}

	// Right Left Case
	if (balance < -1 && nodeToAdd->key < startNode->right->key) {
		//printf("RL case : rotating right... ");
//printf("\n\n\n\n\n\nleft root of node is: %s", startNode->left->key);
		startNode->right = rightRotate(startNode->right);

	//	printf("rotating left\n");
		return leftRotate(startNode);
	}

//	printf("\n\n");

	/* return the (unchanged) node pointer */
	return startNode;
}


/* A utility function to insert a new node with given key in BST */
node *insert(node *root, char *line) {
	node *newNode = createNode(line);
	if (strncmp(newNode->value->titleType, "tvEpisode", 20) == 0) {
	/* Do not insert TV Episodes into tree */
	return root;
	}
    //printf("\n\n\nNEWNODE KEY IS : %s\n\n", newNode->key);

	return _insert(root, newNode);
}


// Function only used for testing, full tree too large to display.
/* void displayTree(node *nd) {
    if (nd == NULL)
        return;


    printf("NODE IS %s , HEIGHT IS %d",nd->key, nd->height);

    if (nd->left != NULL)
        printf("(L:%s)",nd->left->key);
    if (nd->right != NULL)
        printf("(R:%s)",nd->right->key);

    printf("\n");

    displayTree(nd->left);
    displayTree(nd->right);
} */

struct node *search(struct node *root, char *key) {
    // Traverse untill root reaches to dead end
    while (root != NULL) {
        // pass right subtree as new tree
        if (strncmp(key, root->key, strlen(key)) > 0)
				            root = root->right;

        // pass left subtree as new tree
        else if (strncmp(key, root->key, strlen(key)) <= 0)
            root = root->left;
        else
            return root;
		}
    return root;
}

int fileExists(char *filename) {
    /* try to open file to read */
    FILE *checkFile;
    if ((checkFile = fopen(filename, "r"))) {
        fclose(checkFile);
        return 1;
    }
    return 0;
}

int main() {
	const int BUFF_SIZE = 1024;
	node *root = NULL;

	char line[BUFF_SIZE];
	FILE *fptr;

	if ((fptr = fopen("title.basics.tsv", "r")) == NULL) {
		printf("Error! No existing file.\n");
		// Program exits if file pointer returns NULL.
		exit(1);
	}

	// throw away first line because it is column headers
	fgets(line, BUFF_SIZE, fptr);

	/* reads file line by line until end */

	/* creates a node to be populated into AVL tree */
	while (fgets(line, BUFF_SIZE, fptr) != NULL) {
		root = insert(root, strdup(line));
	}

	fclose(fptr);

	char option[2048];

	printf("Root of tree is : %s\n", root->key);

	while (strncmp(option, "end", 2048) != 0) {

	printf("\n\n\nWOULD YOU LIKE TO CREATE A NEW FILE (type 'C'),\n"
	"DELETE A FILE ('type 'D'),\n"
	"UPDATE AN EXISTING FILE (type 'U'),\n"
	"RETRIEVE INFORMATION ON AN EXISTING FILE (type 'R')\n"
	"OR END THE PROGRAM (type 'END')?\n\n\n");

	scanf("%s", option);
	for (int i = 0; i < strlen(option); i++) option[i] = tolower(option[i]);

	if (strncmp(option, "c", 2048) == 0) {
		char fileName[2048];
		FILE *addFile;
		printf("What would you like to name your file? (End in '.log' preferably)\n");
		scanf("%s", fileName);
		if (fileExists(fileName) == 1) printf ("File already exists\n");\
		else {
			addFile = fopen(fileName, "w");
			if (addFile == NULL) printf("File did not create...try again\n");
			printf("File successfully created!\n");
		}
	}

	else if (strncmp(option, "d", 2048) == 0) {
		char delete[2048];
		int isDeleted;
		printf("Which file would you like to delete?\n");
		scanf("%s", delete);
		if (fileExists(delete) == 0) printf ("File does not exist.\n");
			else {
				isDeleted = remove(delete);
				if (isDeleted == 0) printf("%s file was successfully deleted.\n", delete);
				else { printf("Unable to delete file.\n"); }
			}
	}

	else if (strncmp(option, "u", 2048) == 0) {
		node *tempRoot = root;
		node *result = NULL;
		char fileUpdate[2048];
		char searchMovie[2048];
		printf("Which file would you like to update?\n");
		scanf("%s", fileUpdate);
		if (fileExists(fileUpdate) == 0) printf ("File does not exist.\n");
		FILE *updateFile;
		updateFile = fopen(fileUpdate, "r");
		printf("Enter a movie title in which you would like to search for"
		" (Type what movie begins with, articles will be discarded : ");
		fgets(searchMovie, BUFF_SIZE, stdin);
		if (strncmp(searchMovie, "\n", 10) == 0) fgets(searchMovie, BUFF_SIZE, stdin);
		char *tempName = stripArticle(searchMovie);
		printf("Your search is going to be %s", tempName);
		result = search(tempRoot, tempName);
		printf("result key is %s", result->value->primaryTitle);

	}

	else if (strncmp(option, "r", 2048) == 0) {
		printf("Which file would you like to display?\n");
		char fileName[2048];
		FILE *readFile;
		scanf("%s", fileName);
		fflush(stdin);
		if (fileExists(fileName) == 0) printf ("File does not exist.\n");
		else {
			readFile = fopen(fileName, "r");
			while (fgets(line, BUFF_SIZE, readFile) != NULL) {
			printf("%s", line);
			}
		}

	}


	else if (strncmp(option, "end", 2048) == 0) {
		printf("Exiting program...\n");
		exit(1);
		}

	else if (strncmp(option, " ", 32) == 0) printf("\n");

	else {
			printf("Invalid input, please try again (valid inputs : 'c' 'r' 'u' 'd' and 'end', not case sensitive).\n");
		}

	}


	//displayTree(root);
	//printf("\n\nDONE!\nROOT is %s\n\n", root->key);
	//displayTree(root);

	// Do stuff with populated BST


	return 0;
}
