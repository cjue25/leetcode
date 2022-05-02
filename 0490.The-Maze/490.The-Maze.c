int dfs( int ** maze, int r, int c, int r_max, int c_max, int ** visit, int * target )
{
    int dir[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0 , -1 } };
    int i;
    
    
    
    
    visit[r][c] = true;
    
    for( i = 0; i < 4; i++ )
    {
        int rr = r;
        int cc = c;
        
        while( rr >= 0 && rr < r_max && cc >= 0 && cc < c_max && maze[rr][cc] != 1 )
        {
            rr += dir[i][0];
            cc += dir[i][1];
        }
        
        rr -= dir[i][0];
        cc -= dir[i][1];
        
        if( rr == target[0] && cc == target[1] ) return true;
        
        
        if( visit[rr][cc] != true )
        {
            if( dfs( maze, rr, cc, r_max, c_max, visit, target ) == true ) return true;
        }
        
    }
    
    return false;
    
}

bool hasPath(int** maze, int mazeSize, int* mazeColSize, int* start, int startSize, int* destination, int destinationSize){
    
    
    int ** visit = malloc( sizeof ( int * ) * mazeSize );
    int i;
    int ans;
    for( i = 0; i < mazeSize; i++ )
    {
        visit[i] = malloc( sizeof( int ) * mazeColSize[0] );
        memset( visit[i], 0x00, sizeof( int ) * mazeColSize[0] );
    }
    
    ans = dfs( maze, start[0], start[1], mazeSize, mazeColSize[0], visit, destination );
    
    for( i = 0; i < mazeSize; i++ )
    {
        free( visit[i] );
    }
    free( visit );
    
    return ans;
    

}