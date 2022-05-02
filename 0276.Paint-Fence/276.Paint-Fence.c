int numWays(int n, int k){
    
    if( n == 1 ) return k;
    if( n == 2 ) return k*k;
    
    int dp[n];
    // memset( dp, 0x00, sizeof( dp ) );
    
    
    dp[0] = k;
    dp[1] = k * k;
    
    int i;
    for( i = 2; i < n; i++ )
    {
        dp[i] =  ( k - 1 ) * ( dp[i-1] + dp[i-2] );
    }
    
    return dp[n-1];
}