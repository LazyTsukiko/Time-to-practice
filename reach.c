#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct node
{
	char *name;
	struct node *next;
};

struct nameList
{
	char *name;
	struct nameList *next;
	struct node *edge;
} *head = NULL;

/*
* checkName(*name) -- check if the input name is in the list
* return 1 if it exist, return 0 otherwise.
*/

int checkName(char *name)
{
	if (head == NULL)
		return 0;

	struct nameList *temp;
	temp = head;

	while (temp != NULL)
	{
		if (strcmp(temp->name, name) == 0)
			return 1;

		temp = temp->next;
	}

	return 0;
}

/*
* opN(*name) -- do the main fuction of operate @p, add node to the list,
* return 1 if there is an error occur, return 0 otherwise.
*/

int opN(char *name)
{
	if (checkName(name) != 0)
	{
		fprintf(stderr, "ERROR [addNode]: Node %s already exists", name);
		return 1;
	}

	struct nameList *newNode;
	newNode = malloc(sizeof(struct nameList));
	newNode->name = name;
	newNode->edge = NULL;
	newNode->next = NULL;

	if (head == NULL)
	{
		head = newNode;
		return 0;
	}

	struct nameList *temp;
	temp = head;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}

	temp->next = newNode;
	return 0;
}

/*
* opE(*name1, *name2) -- do the main fuction of operate @e, create the edge between two nodes,
* return 1 if there is an error occur, return 0 otherwise.
*/

int opE(char *name1, char *name2)
{
	struct nameList *temp;

	temp = head;

	while (temp != NULL)
	{
		if (strcmp(temp->name, name1) == 0)
			break;

		temp = temp->next;
	}

	if (temp == NULL)
	{
		fprintf(stderr, "ERROR [createEdge]: no such node %s", name1);
		return 1;
	}

	if (checkName(name2))
	{
		fprintf(stderr, "ERROR [createEdge]: no such node %s", name2);
		return 1;
	}

	struct node *newNode;
	newNode = malloc(sizeof(struct node));
	newNode->name = name2;
	newNode->next = NULL;

	if (temp->edge == NULL)
	{
		temp->edge = newNode;
		return 0;
	}

	struct node *temp2;
	temp2 = temp->edge;
	while (temp2->next != NULL)
	{
		temp2 = temp2->next;
	}

	temp2->next = newNode;
	return 0;
}

/*
* searchList(*temp, *name) -- match the given name in the list,
* return the node if matched, return NULL otherwise.
*/

struct nameList *searchList(struct nameList *temp, char *name)
{
	if (strcmp(temp->name, name) == 0)
		return temp;

	if (temp->next == NULL)
	{
		fprintf(stderr, "ERROR : no such node %s", name);
		return temp->next;
	}

	return searchList(temp->next, name);
}

/*
* searchNode(*temp, *name) -- match the given name in the list.
*/

void searchNode(struct node *temp, char *name)
{
	int pathExists = 0;

	if (temp->next == NULL)
	{
		printf("%d\n", pathExists);
		return;
	}

	if (strcmp(temp->name, name) == 0)
	{
		pathExists = 1;
		printf("%d\n", pathExists);
		return;
	}

	searchNode(temp->next, name);
	return;
}

/*
* opQ(*name1, *name2) -- do the main fuction of operate @e, find the path between two nodes
* return 1 if there is an error occur, return 0 otherwise.
*/

int opQ(char *name1, char *name2)
{
	struct nameList *match;
	match = searchList(head, name1);

	if (match == NULL)
		return 1;

	if (!checkName(name2))
	{
		fprintf(stderr, "ERROR : no such node %s", name2);
		return 1;
	}
	
	searchNode(match->edge, name2);
	return 0;
}

/*
* freeNode(*temp) -- free all the malloc nodes.
*/

void freeNode(struct node *temp)
{
	if (temp == NULL)
		return;

	freeNode(temp->next);
	free(temp->name);
	free(temp);
	return;
}

/*
* freeList(*temp) -- free all the malloc nameList.
*/

void freeList(struct nameList *temp)
{
	if (temp->next != NULL)
		freeList(temp->next);

	freeNode(temp->edge);
	free(temp->name);
	free(temp);
	return;
}

int main(void)
{
	char *line = NULL;
	size_t size;
	int error = 0;
	
	while (getline(&line, &size, stdin) >= 0)
	{
		char *p;
		p = line;
		int len = strlen(line);
		char op[5];

		if (sscanf(p, "%[^ ]", op) == -1)
		{
			fprintf(stderr, "ERROR: malformed input %s", line);
			error = 1;
			continue;
		}
		
		p += 3;

		if (strcmp(op, "@n") == 0)
		{
			char *name;
			name = malloc(65 * sizeof(char));
			if (sscanf(p, "%64s", name) == -1)
			{
				fprintf(stderr, "ERROR: malformed input %s", line);
				error = 1;
				continue;
			}

			char checkLine[65];
			if (sscanf(p, "%64s", checkLine) != -1)
			{
				fprintf(stderr, "ERROR: malformed input %s", line);
				error = 1;
				continue;
			}

			if (opN(name))
				error = 1;
			continue;
		}

		if (strcmp(op, "@e") == 0)
		{
			char name1[65], name2[65];
			if (sscanf(p, "%64s", name1) == EOF)
			{
				fprintf(stderr, "ERROR: malformed input %s", line);
				error = 1;
				continue;
			}
			
			if (sscanf(p, "%64s", name2) == EOF)
			{
				fprintf(stderr, "ERROR: malformed input %s", line);
				error = 1;
				continue;
			}

			char *checkLine = NULL;
			if (sscanf(p, "%s", checkLine) != EOF)
			{
				fprintf(stderr, "ERROR: malformed input %s", line);
				error = 1;
				continue;
			}

			if (opE(name1, name2))
				error = 1;
			continue;
		}

		if (strcmp(op, "@q") == 0)
		{
			char name1[65], name2[65];
			if (sscanf(p, "%64s", name1) == EOF)
			{
				fprintf(stderr, "ERROR: malformed input %s", line);
				error = 1;
				continue;
			}

			if (sscanf(p, "%64s", name2) == EOF)
			{
				fprintf(stderr, "ERROR: malformed input %s", line);
				error = 1;
				continue;
			}

			char *checkLine = NULL;
			if (sscanf(p, "%s", checkLine) != EOF)
			{
				fprintf(stderr, "ERROR: malformed input %s", line);
				error = 1;
				continue;
			}

			if (opQ(name1, name2))
				error = 1;
			continue;
		}

		fprintf(stderr, "ERROR: malformed input %s", line);
		error = 1;
	}

	freeList(head);

	return error;
}
