/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */

#define MAX_SIZE        ( 1 << 10 )

typedef struct _hashtable
{
    struct  TreeNode  * head;
    struct _hashtable * next;
    unsigned int        count; 
} HASH_TABLE;

bool check( struct TreeNode * q1, struct TreeNode * q2 )
{
    if( q1 != NULL && q2 != NULL )
    {
        if( q1->val != q2->val ) return false;
        if( check( q1->left , q2->left  ) == false ) return false;
        if( check( q1->right, q2->right ) == false ) return false;
        return true;
    }
    
    if( q1 == NULL && q2 == NULL) return true;
    
    return false;
    
}

void hash_add( HASH_TABLE ** h, int key, struct TreeNode * root )
{
    int h_key = ( key < 0 )? -key : key;
    h_key = h_key & ( MAX_SIZE - 1);
    
    HASH_TABLE * hash = h[h_key];
    
    while( hash != NULL )
    {

        if( check( hash->head, root ) )
        {
            hash->count++;
            return;
        }
        hash = hash->next;
    }
    
    hash = malloc( sizeof( HASH_TABLE ) );
    hash->head  = root;
    hash->count = 1;
    hash->next  = h[h_key];
    h[h_key] = hash;
    
    return;
}

int dfs( struct TreeNode * root, HASH_TABLE ** h )
{
    if( root == NULL ) return -1;
   
    int key = root->val;
        
    key += dfs( root->left, h );
    key += dfs( root->right, h );
    
    hash_add( h, key, root );
    
    return key;
}
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
struct TreeNode** findDuplicateSubtrees(struct TreeNode* root, int* returnSize){

    *returnSize = 0;
    
    if( root == NULL || ( root->left == NULL && root->right == NULL ) ) return NULL;
    
    struct TreeNode ** ans = malloc( sizeof( struct TreeNode * ) * MAX_SIZE );
    memset( ans, 0x00, sizeof( struct TreeNode * ) * MAX_SIZE );
    
    HASH_TABLE ** hash = malloc( sizeof( HASH_TABLE * ) * MAX_SIZE );
    memset( hash, 0x00, sizeof( HASH_TABLE * ) * MAX_SIZE );
    
    dfs( root, hash );
    
    HASH_TABLE * hh;
    unsigned int i;
    for( i = 0; i < MAX_SIZE; i++ )
    {
        hh = hash[i];
        while( hh != NULL )
        {
            HASH_TABLE * temp = hh;
            if( hh->count >= 2 ) 
            {
                ans[(*returnSize)++] = hh->head;
            }
            hh = hh->next;
            free( temp );
        }
        
    }
    
    free( hash );
    return ans;
    
}