/*
dp[i][d] = start from i on the current d day
dp[i][d] = min( dp[i][d], dp[j+1][d+1] + hard )
hard = max( difficulty[i:j] );
i <= j < n - ( total_d - d - 1 )

*/

int my_min( int a, int b )
{ 
    return ( a < b ) ? a : b;
}

int my_max( int a, int b )
{
    return ( a > b ) ? a : b;    
}

int minDifficulty(int* jobDifficulty, int jobDifficultySize, int d){
    
    if( jobDifficultySize < d ) return -1;
    int dp[jobDifficultySize][d];
    memset( dp, 0x00, sizeof( dp ) );
    
    int i;
    int j;
    int hard;
    int n = jobDifficultySize;
    dp[n-1][d-1] = jobDifficulty[n-1];
    
    for( i = n-2; i >= 0; i-- )
    {
        dp[i][d-1] = my_max( dp[i+1][d-1], jobDifficulty[i] );
    }
    
    int dd;
    for( dd = d - 2; dd >= 0; dd-- )
    {
        for( i = dd; i < n - ( d - dd - 1 ); i++ )
        {
            hard = jobDifficulty[i];
            
            dp[i][dd] = hard + dp[i+1][dd+1];
            
            for( j = i + 1; j < n - ( d - dd - 1); j++ )
            {
                hard = my_max( hard, jobDifficulty[j] );
                dp[i][dd] = my_min( hard + dp[j+1][dd+1], dp[i][dd] );
            }
        }
            
    }
    
    
    return dp[0][0];
}