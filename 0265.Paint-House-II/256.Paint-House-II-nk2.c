/*
no adjacent color
minimum cost

houses = costsSize
colors = costsColSize[0];

i-th house paint color j with minimum costs
dp[i][j] = min( dp[i-1][0~k] without j )
*/

int my_min( int a, int b )
{
    return ( a < b )? a : b;
}

int minCostII(int** costs, int costsSize, int* costsColSize){

    int h = costsSize;
    int k = costsColSize[0];
    int dp[h][k];
    memset( dp, 0x00, sizeof( dp ) );
    
    int min = INT_MAX;
    int i;
    int j;
    int m;
    for( j = 0; j < k; j++ )
    {
        dp[0][j] = costs[0][j];
        
    }
    
    for( i = 1; i < h; i++ )
    {
        
        for( j = 0; j < k; j++ )
        {
            min = INT_MAX;
            for( m = 0; m < k; m++ )
            {
                if( m == j ) continue;
                min = my_min( dp[i-1][m], min );
            }
            
            dp[i][j] = min + costs[i][j];
        }
        
    }
    
    min = INT_MAX;
    for( j = 0; j < k; j++ )
    {
        min = my_min( dp[h-1][j], min );
    }
    
    
    
    
    return min;
}