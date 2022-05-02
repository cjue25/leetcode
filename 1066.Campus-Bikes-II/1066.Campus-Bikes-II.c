
unsigned int distance( int * w, int * b )
{
    return abs( w[0] - b[0] ) + abs( w[1] - b[1] );
}

void dfs( int ** workers, int ** bikes, int w_index, int * min, int sum, int * visit, int b_max, int w_max )
{
    
    if( w_index >= w_max )
    {
        if( *min > sum ) *min = sum;
        
        return;
    }
    if( sum >= *min ) return;
    
    int i;
    int dis;
    for( i = 0 ; i < b_max; i++ )
    {
        if( visit[i] == false ) 
        {
            visit[i] = true;
            dis = distance( workers[w_index], bikes[i] );
            dfs( workers, bikes, w_index + 1, min, sum + dis, visit, b_max, w_max );
            visit[i] = false;
        }
        
        
    }
}

int assignBikes(int** workers, int workersSize, int* workersColSize, int** bikes, int bikesSize, int* bikesColSize){
    
    int sum = INT_MAX/2;
    int visit[bikesSize];
    memset( visit, 0x00, sizeof( visit ) );
    
    dfs( workers, bikes, 0, &sum, 0, visit, bikesSize, workersSize );
    return sum;

}