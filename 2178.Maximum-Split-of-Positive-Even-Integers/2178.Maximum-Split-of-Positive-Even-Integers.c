/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
long long* maximumEvenSplit(long long finalSum, int* returnSize){
    
    *returnSize = 0;
    if( finalSum & 0x01 != 0 ) return NULL;
    
    long long i;
    long long sum = 0;
    long long diff = 0;
    
    
    for( i = 2; ; i += 2 )
    {
        sum += i;
        (*returnSize)++;
        
        if( sum == finalSum ) break;
        
        if( sum > finalSum ) 
        {
            sum -= ( i + i - 2 );
            diff = i - 2;
            (*returnSize)--;
            break;
        }
    }
    
    long long * ans = malloc( sizeof( long long ) * (*returnSize) );
    long long   j = 2;
    
    for( i = 0; i < *returnSize; i++ )
    {
        ans[i] = j;
        j+=2;
        if( ans[i] == diff )
        {
            ans[i] = finalSum - sum;
        }
    }
    

    
    return ans;
}