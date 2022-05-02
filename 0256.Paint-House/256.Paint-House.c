#define MIN( x, y )     ({(x<y)?x:y;})
    
int minCost(int** costs, int costsSize, int* costsColSize){
    
    int dp[costsSize][costsColSize[0]];
    memset( dp, 0x00, sizeof( dp ) );
    
    int i;
    int min = INT_MAX;
    
    dp[0][0] = costs[0][0];
    dp[0][1] = costs[0][1];
    dp[0][2] = costs[0][2];
    
    for( i = 1; i < costsSize; i++ )
    {
        dp[i][0] = MIN( dp[i-1][1], dp[i-1][2] );
        dp[i][0] += costs[i][0];
        
        dp[i][1] = MIN( dp[i-1][0], dp[i-1][2] );
        dp[i][1] += costs[i][1];
        
        dp[i][2] = MIN( dp[i-1][1], dp[i-1][0] );
        dp[i][2] += costs[i][2];
        
        
                          
    }
    
    for( i = 0; i < 3 ; i++ )
    {
        min = MIN( min, dp[costsSize-1][i] );
    }
    
    return min;
}