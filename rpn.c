/*
* File: rpn.c
* Author: Siyi Jiang
* Purpose: This program is a calculator that evaluates expressions in Reverse Polish notation.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

struct stackNode
{
	char *data;
	struct stackNode *next;
};

/*
* push(*head, *c) -- push node into stack.
*/

void push(struct stackNode *head, char *c)
{
	struct stackNode *newNode;
	newNode = calloc(1, sizeof(struct stackNode));
	char *value;
	value = calloc((strlen(c) + 2), sizeof(char));
	int i;
	
	for (i = 0; i < strlen(c); i++)
	{
		*(value + i) = *(c + i);
	}
	*(value + i) = '\0';
	newNode->data = value;

	if (head->next == NULL)
	{
		newNode->next = NULL;
		head->next = newNode;
		return;
	}

	newNode->next = head->next;
	head->next = newNode;
	return;
}

/*
* pop(*head) -- take the top node from the stack,
* return the top node.
*/

struct stackNode * pop(struct stackNode *head)
{
	if (head->next == NULL)
		return head->next;

	struct stackNode *node;
	node = head->next;
	head->next = node->next;
	
	return node;
}

/*
* freeNode(*temp) -- free all the node and data in the stack.
*/

void freeNode(struct stackNode *temp)
{
	if (temp == NULL)
		return;

	freeNode(temp->next);
	free(temp->data);
	free(temp);
	return;
}

/*
* commandU(*head) -- basic function of U, remove top node.
* return 0 if success, 1 otherwise.
*/

int commandU(struct stackNode *head)
{
	struct stackNode *temp;
	temp = pop(head);

	if (temp == NULL)
	{
		fprintf(stderr, "Nothing to undo\n");
		return 1;
	}

	free(temp->data);
	free(temp);

	return 0;
}

/*
* commandP(*temp) -- basic function of P, print top node.
*/

void commandP(struct stackNode *temp)
{
	if (temp == NULL)
	{
		printf("Stack Empty\n");
		return;
	}

	printf("%s\n", temp->data);
	return;
}

/*
* commandA(*temp) -- basic function of A, print top node.
*/

void commandA(struct stackNode *temp)
{
	if (temp->next != NULL)
		commandA(temp->next);

	printf("%s\n", temp->data);
	return;
}

/*
* bigger(*str1, *str2) -- compare two string of number
* return 1 if first string is bigger, 0 is equal, -1 if str2 is bigger.
*/

int bigger(char *str1, char *str2)
{
	int len1 = strlen(str1), len2 = strlen(str2);

	if (len1 > len2)
		return 1;
	else if (len1 < len2)
		return -1;
	else
	{
		int i;
		for (i = 0; i < len1; i++)
		{
			if (*(str1 + i) > *(str2 + i))
				return 1;
			
			if (*(str1 + i) < *(str2 + i))
				return -1;
		}

		return 0;
	}
}

/*
* adds(*str1, *str2) -- add two string of number
* return result of addition.
*/

char * adds(char *str1, char *str2)
{
	
	int len1 = strlen(str1), len2 = strlen(str2);
	int i, dig;
	int newlen = len1 > len2 ? len1 : len2;
	char *newStr = NULL;
	int inc = 0;
	newStr = calloc(newlen + 1, sizeof(char));

	if (bigger(str1, str2) >= 0)
	{
		for (i = 1; i <= len2; i++)
		{
			dig = (*(str1 + len1 - i) - '0') + (*(str2 + len2 - i) - '0') + inc;

			if (dig >= 10)
			{
				dig -= 10;
				inc = 1;
			}
			else
				inc = 0;

			*(newStr + newlen - i) = dig + '0';
		}

		for (; i <= len1; i++)
		{
			dig = *(str1 + len1 - i) - '0' + inc;

			if (dig >= 10)
			{
				dig -= 10;
				inc = 1;
			}
			else
				inc = 0;

			*(newStr + newlen - i) = dig + '0';
		}
	}
	else
	{
		for (i = 1; i <= len1; i++)
		{
			dig = (*(str1 + len1 - i) - '0') + (*(str2 + len2 - i) - '0') + inc;

			if (dig >= 10)
			{
				dig -= 10;
				inc = 1;
			}
			else
				inc = 0;

			*(newStr + newlen - i) = dig + '0';
		}

		for (; i <= len2; i++)
		{
			dig = *(str2 + len2 - i) - '0' + inc;

			if (dig >= 10)
			{
				dig -= 10;
				inc = 1;
			}
			else
				inc = 0;

			*(newStr + newlen - i) = dig + '0';
		}
	}

	if (inc == 1)
	{
		char *str;
		str = calloc(newlen + 2, sizeof(char));
		*str = '1';
		for (i = 0; i < newlen; i++)
		{
			*(str + 1 + i) = *(newStr + i);
		}

		free(newStr);

		return str;
	}
	
	return newStr;
}

/*
* subs(*str1, *str2) -- subtract two string of number
* return result of subtraction.
*/

char * subs(char *str1, char *str2)
{
	int i, dig, dec = 0;
	int len1 = strlen(str1), len2 = strlen(str2);
	char str[len1];

	for (i = 1; i <= len2; i++)
	{
		dig = (*(str1 + len1 - i) - '0') - (*(str2 + len2 - i) - '0') - dec;

		if (dig < 0)
		{
			dec = 1;
			dig += 10;
		}
		else
			dec = 0;

		str[len1 - i] = dig + '0';
	}

	for (; i <= len1; i++)
	{
		dig = *(str1 + len1 - i) - '0' - dec;

		if (dig < 0)
		{
			dec = 1;
			dig += 10;
		}
		else
			dec = 0;

		str[len1 - i] = dig + '0';
	}

	char *newStr;
	newStr = calloc(len1 + 1, sizeof(char));
	int j = 0;
	for (i = 0; i < len1; i++)
	{
		if (j == 0 && str[i] == '0')
			continue;

		*(newStr + j) = str[i];
		j++;
	}

	return newStr;
}

/*
* check(*head) -- check if the list have two or more node.
* return 0 if list have more than 1 node, 1 other wise.
*/

int check(struct stackNode *head)
{
	if (head->next == NULL || head->next->next == NULL)
		return 1;

	return 0;
}

/*
* add(*head) -- add first two node
* return 0 if success, 1 otherwise.
*/

int add(struct stackNode *head)
{
	if (check(head))
	{
		fprintf(stderr, "Not enough operators\n");
		return 1;
	}
	
	struct stackNode *temp1;
	struct stackNode *temp2;
	
	temp1 = pop(head);
	temp2 = pop(head);

	char *str1, *str2 ,*newStr, *value = NULL;
	str2 = temp1->data;
	str1 = temp2->data;

	int neg = 0;
	
	if (*str1 == '-' && *str2 == '-')
	{
		value = adds(str1 + 1, str2 + 1);
		neg = 1;
	}
	else if (*str1 == '-' && *str2 != '-')
	{
		int k = bigger(str1 + 1, str2);

		if (k == 0)
		{
			newStr = calloc(3, sizeof(char));
			*newStr = '0';
			push(head, newStr);
			free(newStr);
			free(temp1->data);
			free(temp1);
			free(temp2->data);
			free(temp2);
			return 0;
		}

		if (k == 1)
		{
			neg = 1;
			value = subs(str1 + 1, str2);
		}

		if (k == -1)
			value = subs(str2, str1 + 1);
	}
	else if (*str1 != '-' && *str2 == '-')
	{
		int k = bigger(str1, str2 + 1);

		if (k == 0)
		{
			newStr = calloc(3, sizeof(char));
			*newStr = '0';
			push(head, newStr);
			free(newStr);
			free(temp1->data);
			free(temp1);
			free(temp2->data);
			free(temp2);
			return 0;
		}

		if (k == 1)
			value = subs(str1, str2 + 1);

		if (k == -1)
		{
			neg = 1;
			value = subs(str2 + 1, str1);
		}
	}
	else
		value = adds(str1, str2);

	newStr = calloc((strlen(value) + 2), sizeof(char));
	if (neg == 1)
		*newStr = '-';

	int i;
	for (i = 0; i < strlen(value); i++)
	{
		*(newStr + neg + i) = *(value + i);
	}

	push(head, newStr);
	free(value);
	free(newStr);
	free(temp1->data);
	free(temp1);
	free(temp2->data);
	free(temp2);

	return 0;
}

/*
* sub(*head) -- subtract first two node
* return 0 if success, 1 otherwise.
*/

int sub(struct stackNode *head)
{
	if (check(head))
	{
		fprintf(stderr, "Not enough operators\n");
		return 1;
	}

	struct stackNode *temp1;
	struct stackNode *temp2;

	temp2 = pop(head);
	temp1 = pop(head);

	char *str1, *str2, *newStr, *value = NULL;
	str1 = temp1->data;
	str2 = temp2->data;

	int neg = 0;

	if (*str1 == '-' && *str2 == '-')
	{
		int k = bigger(str1 + 1, str2 + 1);
		
		if (k == 0)
		{
			newStr = calloc(3, sizeof(char));
			*newStr = '0';
			push(head, newStr);
			free(newStr);
			free(temp1->data);
			free(temp1);
			free(temp2->data);
			free(temp2);
			return 0;
		}

		if (k == 1)
		{
			neg = 1;
			value = subs(str1 + 1, str2 + 1);
		}

		if (k == -1)
			value = subs(str2 + 1, str1 + 1);
	}
	else if (*str1 == '-' && *str2 != '-')
	{
		neg = 1;
		value = adds(str1 + 1, str2);
	}
	else if (*str1 != '-' && *str2 == '-')
	{
		value = adds(str1, str2 + 1);
	}
	else
	{
		int k = bigger(str1, str2);

		if (k == 0)
		{
			newStr = calloc(3, sizeof(char));
			*newStr = '0';
			push(head, newStr);
			free(newStr);
			free(temp1->data);
			free(temp1);
			free(temp2->data);
			free(temp2);
			return 0;
		}

		if (k == 1)
			value = subs(str1, str2);

		if (k == -1)
		{
			neg = 1;
			value = subs(str2, str1);
		}		
	}

	newStr = calloc((strlen(value) + 2), sizeof(char));
	if (neg == 1)
		*newStr = '-';

	int i;
	for (i = 0; i < strlen(value); i++)
	{
		*(newStr + neg + i) = *(value + i);
	}

	push(head, newStr);
	free(value);
	free(newStr);
	free(temp1->data);
	free(temp1);
	free(temp2->data);
	free(temp2);

	return 0;
}

int main(void)
{
	struct stackNode *head;
	head = calloc(1, sizeof(struct stackNode));
	head->data = NULL;
	head->next = NULL;
	char *c = NULL;
	size_t size;
	int res, error = 0;

	while ((res = getline(&c, &size, stdin)) != EOF)
	{
		int len = (int)strlen(c);

		if (len <= 1)
		{
			fprintf(stderr, "Illegal input %s", c);
			error = 1;
			continue;
		}

		if (len == 2)
		{
			if (*c == 'c')
			{
				freeNode(head->next);
				head->next = NULL;
			}
			else if (*c == 'u')
				error += commandU(head);
			else if (*c == 'p')
				commandP(head->next);
			else if (*c == 'a')
			{
				if (head->next == NULL)
				{
					printf("Stack Empty\n");
					continue;
				}

				commandA(head->next);
			}
			else if (*c == '+')
				error += add(head);
			else if (*c == '-')
				error += sub(head);
			else if (isdigit(*c))
			{
				char *current;
				current = malloc(3 * sizeof(char));
				*current = *c;
				*(current + 1) = '\0';
				push(head, current);
				free(current);
			}
			else
			{
				fprintf(stderr, "Illegal command or operand: %s", c);
				error = 1;
			}

			continue;
		}

		int i, j = 0, leadzero = 0;
		char *current;
		current = calloc((len + 1), sizeof(char));

		for (i = 0; i < len - 1; i++)
		{
			if (!isdigit(*(c + i)))
			{
				fprintf(stderr, "Illegal input %s", c);
				error = 1;
				break;
			}

			if (*(c + i) == '0' && leadzero == 0)
			{
				if (i == len - 2)
					*current = '0';
				
				continue;
			}

			leadzero = 1;
			*(current + j) = *(c + i);
			j++;
		}

		if (i != len - 1)
		{
			free(current);
			continue;
		}

		*(current + i) = '\0';

		push(head, current);
		free(current);

	}

	if (head->next != NULL)
		freeNode(head->next);

	free(c);
	free(head);
	return error;
}