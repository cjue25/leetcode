#define MAX_SIZE 100000

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
    
    int      k = 4;
    int ** ans = malloc( sizeof( int * ) * MAX_SIZE );
    int  * column = malloc( sizeof( int ) * MAX_SIZE );
    
    
    qsort( nums, numsSize, sizeof( int ), compare );
    
    *returnSize = 0;
    
    int l1   = 0;
    int l2   = 0;
    
    int start = 0;
    int end   = 0;
    
    long int temp;
    int i;
    for( l1 = 0; l1 < numsSize - ( k - 1 ); l1++ )
    {
        
        if( l1 != 0 && nums[l1] == nums[l1-1] ) continue;
        
        temp = ( long ) nums[l1] + nums[l1+1] + nums[l1+2] + nums[l1+3];
        
        if( temp > target ) break;

        temp = ( long ) nums[l1] + nums[numsSize-1] + nums[numsSize-2] + nums[numsSize-3];
        
        
        if( temp < target ) continue;
        
        
        for( l2 = l1 + 1; l2 < numsSize - ( k - 2 ); l2++ )
        {
            if( l2 != l1 + 1 && nums[l2] == nums[l2-1] ) continue;
            
            temp = ( long ) nums[l1] + nums[l2] + nums[l2+1] + nums[l2+2];
            
            if( temp > target ) break;

            temp = ( long ) nums[l1] + nums[l2] + nums[numsSize-1] + nums[numsSize-2];
            
            if( temp < target ) continue;
            
            start = l2 + 1;
            end   = numsSize - 1;
            
            while( start < end )
            {
                
                int sum = nums[l1] + nums[l2] + nums[start] + nums[end];
                
                if( sum == target )
                {
                    int * new = malloc( sizeof( int ) * k );
                    
                    new[0] = nums[l1];
                    new[1] = nums[l2];
                    new[2] = nums[start];
                    new[3] = nums[end];
                    
                    ans[(*returnSize)] = new;
                    column[(*returnSize)++] = k;
                    
                    while( start < end && nums[start] == nums[start+1] ) start++;
                    while( start < end && nums[end] == nums[end-1] ) end--;
                    
                    start++;
                    end--;
                    
                }
                else if( sum < target )
                {
                    start++;
                }
                else
                {
                    end--;
                }
            
            }
            
        }
        
    }
    
    
    
    *returnColumnSizes = column;
    return ans;
}