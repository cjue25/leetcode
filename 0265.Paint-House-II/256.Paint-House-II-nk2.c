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
    
    int min  = INT_MAX;
    int min2 = INT_MAX;
    int mc   = -1;
    int mc2  = -1;
    
    int i;
    int j;
    
    for( j = 0; j < k; j++ )
    {
        dp[0][j] = costs[0][j];
        
        if( dp[0][j] < min )
        {
            min2 = min;
            mc2  = mc;
            
            min = dp[0][j];
            mc  = j;
            
        }
        else if( dp[0][j] < min2 )
        {
            min2 = dp[0][j];
            mc2  = j;
        }
        
    }
    
    int prev_mc  = mc;
    int prev_mc2 = mc2;
    
    for( i = 1; i < h; i++ )
    {
        min  = INT_MAX;
        min2 = INT_MAX;
        
        for( j = 0; j < k; j++ )
        {
            if( j == prev_mc )
            {
                dp[i][j] = dp[i-1][prev_mc2] + costs[i][j];
            }
            else
            {
                dp[i][j] = dp[i-1][prev_mc] + costs[i][j];
            }
            
            
            if( dp[i][j] < min )
            {
                min2 = min;
                mc2  = mc;

                min = dp[i][j];
                mc  = j;

            }
            else if( dp[i][j] < min2 )
            {
                min2 = dp[i][j];
                mc2  = j;
            }
            
        }
        
        prev_mc = mc;
        prev_mc2 = mc2;
        
    }
    
    return dp[h-1][prev_mc];
}