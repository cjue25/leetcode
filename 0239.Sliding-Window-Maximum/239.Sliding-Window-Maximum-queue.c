

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* maxSlidingWindow(int* nums, int numsSize, int k, int* returnSize){
    
    int queue[numsSize];
    int right = 0;
    int left  = 0;
    memset( queue, 0x00, sizeof( queue ) );
    
    int * ans = malloc( sizeof( int ) * numsSize - k + 1 );
    memset( ans, 0x00, sizeof( int ) * numsSize - k + 1 );
    
    int i;
    for( i = 0; i < k ; i++ )
    {
        while( right > 0 && nums[queue[right-1]] < nums[i] )
        {
            right--;
        }
        queue[right++] = i;
    }
    *returnSize = 0;
    
    ans[(*returnSize)++] = nums[queue[0]];
    
    for( i = k; i < numsSize; i++ )
    {
        while( right > left && nums[queue[right-1]] < nums[i] )
        {
            right--;
        }
        queue[right++] = i;
        
        while( queue[left] <= i - k )
        {
            left++;
        }
        
        ans[(*returnSize)++] = nums[queue[left]];
    }
    
    return ans;
    
}