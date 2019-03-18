#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//#include <"bst.h">

struct movie {
  char *id;
  char *titleType;
  char *primaryTitle;
  char *originalTitle;
  int isAdult;
  int startYear;
  int endYear;
  int runTime;
  char *genre;
};

struct node {
  struct movie *value;
  struct node *left, *right;
  struct node *parent;
};

struct node createNode(char* line, int num) {
	char* tok;
	struct movie aMovie;
  	int idx = 0;

	/* get the first token */
	tok = strtok(line, "\t");

   	/* walk the token list until we hit a NULL */
  	while(tok != NULL) {
      	printf("%s\n", tok);

		/* do something with first token */
		aMovie[idx] = tok; /* == "\N" ? "" : tok; */
      	idx++;
      	tok = strtok(NULL, "\t");
    }

  	printf("Movie: %s\n%s\n%s\n%s\n%d\n%d\n%d\n%d\n%s", aMovie.id, aMovie.titleType, aMovie.originalTitle, aMovie.primaryTitle, aMovie.isAdult, aMovie.startYear, aMovie.endYear, aMovie.runTime, aMovie.genre);

  	struct node theNode;
  	theNode.value = aMovie;
    return (theNode);
}

int main() {
	const int BUFF_LEN = 1024;
  char* token;
  int num = 10;

    char line[BUFF_LEN];
    FILE *fptr;

    if ((fptr = fopen("title.basics.tsv", "r")) == NULL) {
        printf("Error! No existing file.\n");
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    // reads text until newline
    fgets(line, BUFF_LEN, fptr);
    // throw away first line because it is column headers

    for (int i = 0; i <= 5; i++) {
       (fgets(line, BUFF_LEN, fptr));
       token = createNode(line, num);
       printf("%s", token);



    //  printf("data from the file:\n %s", line);
    }

  //  while (fgets(line, BUFF_LEN, fptr)) {
  //  		printf("Data from the file:\n%s", line);
//    }
        // just got a line
        // #1 - Break the line up into tokens by \t
        // #2 - Put each token into a movie struct at the appropriate place
      /*  movie *aMovie;
        aMovie.id = tokens[0];
        aMovie.titleType = tokens[1]
        .
        .
        .

        node *aNode;
        aNode.value = aMovie;

        // add aNode to the BST
    }

    // BST is now populated */

    fclose(fptr);

    return 0;

 }
