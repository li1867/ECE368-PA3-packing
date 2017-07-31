//
//  main.c
//  pa03
//
//  Created by LIPeiyuan on 7/16/17.
//  Copyright © 2017 LIPeiyuan. All rights reserved.
//

#include <stdio.h>
#include "packing.h"
#include <stdlib.h>

void Push(Stack** head, TreeNode * tree);
TreeNode * Pop(Stack ** head);
int max(int b, int c);
void HV(TreeNode * node);
void node_pos(TreeNode * node);
void FreeTree(TreeNode ** node);
void save(TreeNode * node, FILE * fp);
void find_node(TreeNode ** node, TreeNode * tree);

int main(int argc, char * argv[]) {
	Stack * head = Load_From_File(argv[1]);
	Final * f = Perform_Packing(head);
	printf("Width: %le\n",f -> width);
	printf("Height: %le\n",f -> height);
	printf("X-coordinate: %le\n",f -> x_co);
	printf("Y-coordinate: %le\n",f -> y_co);
	Save_To_File(argv[2], head -> node);
	FreeTree(&(head -> node));
	free(f);
	free(head);
	return 0;
}

