/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */

#define MAX_SIZE 100

int max( int a, int b )
{
    return ( a > b ) ? a : b;
}

int height( struct TreeNode* root, int **array, int * column )
{
	
    int cur;
    if( root == NULL ) return -1;
    
    cur = 1 + max( height( root->left, array, column ), height( root->right, array, column ) );	
    
    column[cur]++;
    
    array[cur] = realloc( array[cur], sizeof( int ) * column[cur] );
    array[cur][column[cur] - 1] = root->val;
    
    free( root );
    
    return cur;
	
};
           

int** findLeaves(struct TreeNode* root, int* returnSize, int** returnColumnSizes){
    
    int ** array  = malloc( sizeof( int * ) * MAX_SIZE );
    int  * column = malloc( sizeof( int   ) * MAX_SIZE );
 
    memset( array , 0x00, sizeof( int * ) * MAX_SIZE );
    memset( column, 0x00, sizeof( int   ) * MAX_SIZE );
    
    if( root == NULL ) 
    {
        *returnSize = 0;
        *returnColumnSizes = NULL;
        return NULL;
    }
    

    *returnSize        = height( root, array, column ) + 1;    
    *returnColumnSizes = column;
    
    return array;
}
