#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "packing.h"

void Push(Stack** head, TreeNode * tree);
TreeNode * Pop(Stack ** head);
int max(int b, int c);
void HV(TreeNode * node);
void node_pos(TreeNode * node);
void FreeTree(TreeNode ** node);
void save(TreeNode * node, FILE * fp);
void find_node(TreeNode ** node, TreeNode * tree);

void Push(Stack ** head, TreeNode * tree)
{
	Stack * temp = malloc(sizeof(*temp));
	temp -> node = tree;
	temp -> next = *head;
	(*head) = temp;
}

TreeNode * Pop(Stack ** head)
{
	if((*head) == NULL)
	{
		return NULL;
	}
	else{
		Stack * temp = (*head);
		TreeNode * temp_tree = (*head) -> node;
		(*head) = (*head) -> next;
		free(temp);
		return temp_tree;
	}
	return NULL;
}

Stack * Load_From_File(char * Filename)
{
	FILE * fp = fopen(Filename, "r");
	if(fp == NULL)
	{
		return NULL;
	}

	fseek(fp, 0, SEEK_SET);
	Stack* head = NULL;
	char ch;
	while(1){
		ch = fgetc(fp);
		if(ch == EOF){
			break;
		}

		TreeNode * tree = malloc(sizeof(*tree));
		tree -> left = NULL;
		tree -> right = NULL;
		tree -> x = 0;
		tree -> y = 0;
		tree -> width = 0;
		tree -> height = 0;
		tree -> num_lable = 0;
		tree -> lable = '0';
		if(ch == 'H' || ch == 'V'){
			fseek(fp, -1, SEEK_CUR);
			fscanf(fp, "%c\n",&(tree -> lable));
			tree -> right = Pop(&head);
			tree -> left = Pop(&head);
			tree -> type = 'C'; //character
		}
		else{
			fseek(fp, -1, SEEK_CUR);
			fscanf(fp, "%d(%le,%le)\n", &(tree -> num_lable), &(tree -> width), &(tree -> height));
			tree -> x = tree -> width;
			tree -> y = tree -> height;
			tree -> type = 'N'; //num
		}
		Push(&head, tree);
	}

	fclose(fp);

	return head;
}

int max(int a, int b)
{
	int val = 0;
	if(a > b)
	{
		val = a;
	}
	if(b > a)
	{
		val = b;
	}
	if(a == b)
	{
		val = a;
	}
	return val;
}

void HV(TreeNode * tree)
{
	if(tree -> left == NULL)
	{
		return;
	}
	if((tree -> left -> type) == 'C')
	{
		HV(tree -> left);
	}
	if((tree -> right -> type) == 'C')
	{
		HV(tree -> right);
	}
	if(tree -> lable == 'H')
	{
		tree -> height = tree -> left -> height + tree -> right -> height;
		tree -> width = max(tree -> left -> width, tree -> right -> width);
	}
	if(tree -> lable == 'V')
	{
		tree -> width = tree -> left -> width + tree -> right -> width;
		tree -> height = max(tree -> left -> height, tree -> right -> height);
	}

}

void node_pos(TreeNode * tree)
{
	if(tree == NULL)
	{
		return;
	}
	if(tree -> lable == 'H')
	{
		tree -> left -> x = tree -> x;
		tree -> left -> y = tree -> y + tree -> right -> height;
		tree -> right -> x = tree -> x;
		tree -> right -> y = tree -> y;
	}
	if(tree -> lable == 'V')
	{
		tree -> left -> x = tree -> x;
		tree -> left -> y = tree -> y;
		tree -> right -> x = tree -> x + tree -> left -> width;
		tree -> right -> y = tree -> y;
	}
	node_pos(tree-> left);
	node_pos(tree->right);

	return;
}

void find_node(TreeNode ** temp, TreeNode * tree)
{
	if(tree == NULL)
	{
		return;
	}

	if(tree -> lable != 'H' || tree -> lable != 'V')
	{
		int ch = tree -> num_lable;
		if(ch > (*temp) -> num_lable)
		{
			(*temp) -> x = tree -> x;
			(*temp) -> y = tree -> y;
			(*temp) -> width = tree -> width;
			(*temp) -> height = tree -> height;
			(*temp) -> num_lable = tree -> num_lable;
		}
	}
	find_node(temp,tree -> left);
	find_node(temp,tree -> right);
}

Final * Perform_Packing(Stack * head)
{
	TreeNode * tree = head -> node;
	Final * f = malloc(sizeof(*f));
	HV(tree);
	node_pos(tree);
	TreeNode * large = malloc(sizeof(*large));;
	large -> left = NULL;
	large -> right = NULL;
	large -> x = 0;
	large -> y = 0;
	large -> width = 0;
	large -> height = 0;
	large -> num_lable = 0;
	large -> lable = '0';
	large -> num = 0;
	find_node(&large, tree);
	f -> width = tree -> width;//x_max;
	f -> height = tree -> height;//y_max;
	f -> x_co = large -> x;//x_n;
	f -> y_co = large -> y;//y_n;
	
	//printf("x %le, y %le\n", large -> x, large -> y);
	free(large);

	return f;
}

void FreeTree(TreeNode ** tree)
{
	if(*tree == NULL)
	{
		return;
	}
	FreeTree(&((*tree) -> left));
	FreeTree(&((*tree) -> right));
	free(*tree);
}

void Save_To_File(char * Filename, TreeNode * tree)
{
	FILE * fp = fopen(Filename, "w");
	if(fp == NULL)
	{
		printf("unable to open file to write in\n");
		return;
	}
	save(tree, fp);
	fclose(fp);
}

void save(TreeNode * tree, FILE * fp)
{
	if(tree == NULL)
	{
		return;
	}
	save(tree -> left, fp);
	save(tree -> right, fp);
	if(tree -> lable != 'H' && tree -> lable != 'V')
	{
		fprintf(fp,"%d %le %le %le %le\n", tree -> num_lable, tree -> width, tree -> height, tree -> x, tree -> y);
	}
	return;
}

