typedef struct TreeNode{
	char lable;
    int num_lable;
	int num;
	double x;
	double y;
	double width;
	double height;
	char type;
	struct TreeNode * left;
	struct TreeNode * right;
}TreeNode;

typedef struct Final{
	double width;
	double height;
	double x_co;
	double y_co;
}Final;

typedef struct _Stack
{
	TreeNode * node;
	struct _Stack * next;
    struct _Stack * prev;
}Stack;

Stack * Load_From_File(char * Filename);
Final * Perform_Packing(Stack * node);
void Save_To_File(char * Filename, TreeNode * tree);


