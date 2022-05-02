

char * minWindow(char * s, char * t){
    
    unsigned int slen = strlen( s );
    unsigned int tlen = strlen( t );
    
    if( slen < tlen ) return "";
    
    char * ans = malloc( slen + 1 );
    memset( ans, 0x00, slen + 1 );
    
    unsigned int min_start = 0;
    unsigned int min_len   = INT_MAX;
    unsigned int start     = 0;
    unsigned int end       = 0;
    
    
    int hash[128];
    memset( hash, 0x00, sizeof( hash ) );
    
    int i;
    for( i = 0 ; i < tlen; i++ )
    {
        hash[t[i]]++;
    }
    
    
    while( end < slen )
    {
        if( hash[s[end]]-- > 0 ) tlen--;
        
        
        while( tlen == 0 )
        {
            if( end - start + 1 < min_len )
            {
                min_len = end - start + 1;
                min_start = start;

            }
            
            if( hash[s[start]]++ == 0 ) tlen++;
            
            start++;
        }
        
        end++;
    }
    
    
    if( min_len == INT_MAX ) return "";
    
    strncpy( ans, s + min_start, min_len );
    
    return ans;
}