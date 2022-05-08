#define MAX_SIZE 100000
void dfs( int * nums, int target, int start, int index, int ** ans, int * column, int * size, int numsSize, int * temp, int k )
{
    int i;
    if( index >= k )
    {
        if( target != 0 ) return;
        int * new = malloc( sizeof( int ) * k );
        memcpy( new, temp, sizeof( int ) * k );
        ans[(*size)] = new;
        column[(*size)++] = k;
        return;
    }
    
    
    for( i = start; i < numsSize; i++ )
    {
        if( numsSize - i < k - index ) return;
        
        if( i != start && nums[i] == nums[i - 1 ] ) continue;
        
        if ( i < numsSize - 1 &&  nums[i] +  ( long ) ( k - 1 - index ) * nums[i + 1] > target ) return;
        if ( i < numsSize - 1 &&  nums[i] +  ( long ) ( k - 1 - index ) * nums[numsSize - 1] < target) continue;


        temp[index] = nums[i];
        dfs( nums, target - nums[i], i + 1, index + 1, ans, column, size, numsSize, temp, k );
        
    }
    
    return;
    
}

int compare( void * a, int * b )
{
    return *( int * )a - *(int * )b;
}

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** fourSum(int* nums, int numsSize, int target, int* returnSize, int** returnColumnSizes){
    
    int    k = 4;
    int ** ans = malloc( sizeof( int * ) * MAX_SIZE );
    int  * colum = malloc( sizeof( int ) * MAX_SIZE );
    int    temp[k];
    
    qsort( nums, numsSize, sizeof( int ), compare );
    *returnSize = 0;
    dfs( nums, target, 0, 0, ans, colum, returnSize, numsSize, temp, k );
    
    *returnColumnSizes = colum;
    return ans;
}