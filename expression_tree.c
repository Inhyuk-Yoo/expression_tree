#include <stdlib.h> // malloc, atoi
#include <stdio.h>
#include <ctype.h> // isdigit
#include <assert.h> // assert

#define MAX_STACK_SIZE	50

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	char		data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *createTree( void);

/* Deletes all data in tree and recycles memory
*/
void destroyTree( TREE *pTree);

static void _destroy( NODE *root);

/*  Allocates dynamic memory for a node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
static NODE *_makeNode( char ch);

/* converts postfix expression to binary tree
	return	1 success
			0 invalid postfix expression
*/
int postfix2tree( char *expr, TREE *pTree);

/* Print node in tree using inorder traversal
*/
void traverseTree( TREE *pTree);

/* internal traversal function
	an implementation of ALGORITHM 6-6
*/
static void _traverse( NODE *root);

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);

/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

/* evaluate postfix expression
	return	value of expression
*/
float evalPostfix( char *expr);

////////////////////////////////////////////////////////////////////////////////
void destroyTree( TREE *pTree)
{
	if (pTree)
	{
		_destroy( pTree->root);
	}
		
	free( pTree);
}

////////////////////////////////////////////////////////////////////////////////
void printTree( TREE *pTree)
{
	_infix_print(pTree->root, 0);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////
void traverseTree( TREE *pTree)
{
	_traverse(pTree->root);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	TREE *tree;
	char expr[1024];
	
	fprintf( stdout, "\nInput an expression (postfix): ");
	
	while (fscanf( stdin, "%s", expr) == 1)
	{
		// creates a null tree
		tree = createTree();
		
		if (!tree)
		{
			printf( "Cannot create tree\n");
			return 100;
		}
		
		// postfix expression -> expression tree
		int ret = postfix2tree( expr, tree);
		if (!ret) // if invalid postfix expression
		{
			fprintf( stdout, "invalid expression!\n");
			destroyTree( tree);
			fprintf( stdout, "\nInput an expression (postfix): ");
			continue;
		}
		
		// expression tree -> infix expression
		fprintf( stdout, "\nInfix expression : ");
		traverseTree( tree);
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "\n\nTree representation:\n");
		printTree( tree);
		
		// evaluate postfix expression
		float val = evalPostfix( expr);
		fprintf( stdout, "\nValue = %f\n", val);
		
		// destroy tree
		destroyTree( tree);
		
		fprintf( stdout, "\nInput an expression (postfix): ");
	}
	return 0;
}

/* Allocates dynamic memory for a tree head node and returns its address to caller
    return    head node pointer
            NULL if overflow
*/
TREE *createTree( void){
    TREE * newTree = (TREE*)malloc(sizeof(TREE));
    if(newTree == NULL) return NULL;
    return newTree;
}

/*  Allocates dynamic memory for a node and returns its address to caller
    return    node pointer
            NULL if overflow
*/
static NODE *_makeNode( char ch){
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    if(newNode == NULL) return NULL;
    else{
        newNode->data = ch;
        newNode->left = NULL;
        newNode->right = NULL;
    }
    return newNode;
}

static void _destroy( NODE *root){
    if(root == NULL) return;
    
    _destroy(root->left);
    _destroy(root->right);
    free(root);
}

/* converts postfix expression to binary tree
    return    1 success
            0 invalid postfix expression
*/
int postfix2tree( char *expr, TREE *pTree){
    NODE* nodeStack[MAX_STACK_SIZE];
    int topIdx = -1;
    NODE* temp1;
    NODE* temp2;
    
    while(*expr != 0){
        if(isdigit(*expr)) // expr이 숫자(피연산자)인 경우
        {
            topIdx++;
            nodeStack[topIdx] = _makeNode(*expr);
        }
        else  // expr이 연산자인 경우
        {
            temp2 = nodeStack[topIdx];
            topIdx--;
            temp1 = nodeStack[topIdx];
            topIdx--;
            if(temp1 == NULL | temp2 ==NULL) return 0;
            
            topIdx++;
            nodeStack[topIdx] = _makeNode(*expr);
            nodeStack[topIdx]->left = temp1;
            nodeStack[topIdx]->right = temp2;
        }
        expr++;
    }
    pTree->root = nodeStack[topIdx];
    
    if(topIdx !=0){
        _destroy(pTree->root);
        return 0;
    }

    return 1;
}

/* internal traversal function
    an implementation of ALGORITHM 6-6
*/
static void _traverse( NODE *root){
    if(root->data != 0){
        if(isdigit(root->data) == 1)
            printf("%d", root->data - 48);
        else{
            printf("(");
            _traverse(root->left);
            printf("%c", root->data);
            _traverse(root->right);
            printf(")");
        }
    }
    return;
}

/* internal traversal function
*/
static void _infix_print( NODE *root, int level){
    int i;
    if(root == NULL) return;
    _infix_print(root->right, level+1);
    for(i=0; i<level; i++){
        printf("\t");
    }
    printf("%c\n", root->data);
    _infix_print(root->left, level+1);
    return;
}

/* evaluate postfix expression
    return    value of expression
*/
float evalPostfix( char *expr){
    int temp1, temp2;
    float result;
    int stackArr[MAX_STACK_SIZE];
    int topIdx;
    topIdx = -1;
    
    while(*expr != 0){
        if(isdigit(*expr)) // expr이 숫자(피연산자)인 경우
        {
            topIdx++;
            stackArr[topIdx] = *expr-48;
        }
        else  // expr이 연산자인 경우
        {
            temp2 = stackArr[topIdx];
            topIdx--;
            temp1 = stackArr[topIdx];
            topIdx--;
            if(*expr == 43)
                result = temp1 + temp2;
            else if(*expr == 45)
                result = temp1 - temp2;
            else if(*expr == 42)
                result = temp1 * temp2;
            else if(*expr == 47)
                result = temp1 / temp2;
            //else
            //    assert();
            topIdx++;
            stackArr[topIdx] = result;
        }
        expr++;
    }
    //if(topIdx != -1) //오류형성?
    return result;
}
