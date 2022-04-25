/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */

#define MAX_SIZE    100001

struct TreeNode * ancestor( struct TreeNode * root, int src, int dst )
{
    
    if( root == NULL || root->val == src || root->val == dst ) return root;
    
    struct TreeNode * left  = ancestor( root->left, src, dst );
    struct TreeNode * right = ancestor( root->right, src, dst );
    
    if( left != NULL && right != NULL ) return root;
    
    return ( left == NULL ) ? right : left;
    
    
}


int dfs( struct TreeNode * root, int target, char * str, int up, int *index )
{
    if( root == NULL ) return false;
    
    if( root->val == target ) return true;
    
    if( up == true ) str[(*index)++] ='U';
    else             str[(*index)++] ='L'; 
    
    if( dfs( root->left, target, str, up, index ) == true ) return true;
    else  (*index)--;
    
    
    if( up == true ) str[(*index)++] ='U';
    else             str[(*index)++] ='R'; 

    if( dfs( root->right, target, str, up, index ) == true ) return true;
    else  (*index)--;
    
    return false;
}

char * getDirections(struct TreeNode* root, int startValue, int destValue){
    
    
    int   index = 0;
    char  * ans = malloc( MAX_SIZE );
    
    struct TreeNode* lca = ancestor( root, startValue, destValue );
    
    memset( ans, 0x00, MAX_SIZE );
    
    dfs( lca, startValue, ans, true , &index );
    dfs( lca, destValue , ans, false, &index );
    
    ans[index] = 0x00;
    return ans;
    
}
