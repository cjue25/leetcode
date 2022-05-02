#define MAX_SIZE   ( 1 << 20 )
typedef struct _node
{
    char * s;
    unsigned long int val;
    struct _node * next;
} NODE;

int hash_add( NODE ** h, char * s, int len )
{
    int i;
    unsigned long int key = 5381;
    NODE * node;
    int base = 27;
    
    for( i = 0; i < len; i++ )
    {
        
        if( s[i] != '*' )
        {
            key = key * base + ( s[i]-'a' );
        }
        else
        {
            key *= base;
        }
        
        
        
        // key = (key << 5) + key + s[i];
        // key += s[i] - 'a' + i;
        
    }
    key = key & ( MAX_SIZE - 1 );
    node = h[key];
    
    // while( node != NULL )
    {
        // if( !strcmp( node->s, s ) ) return true;
        // node = node->next;
    }
    
    node = malloc( sizeof( NODE ) );
    node->s = malloc( len + 1 );
    node->s[len] = 0;
    strcpy( node->s, s );
    node->next = h[key];
    h[key] = node;
    
    return false;
    
}

void hash_delete( NODE ** h )
{
    int i;
    NODE * node;
    for( i = 0; i < MAX_SIZE; i++ )
    {
        node = h[i];
        while( node != NULL )
        {
            NODE * temp = node;
            node = node->next;
            free( temp );
        }
        h[i] = NULL;
    }
    
    
    return;
}
bool differByOne(char ** dict, int dictSize){
    
    
    int i;
    int j;
    int len = strlen( dict[0] );
    char str[len+1];
    str[len] = 0x00;
    
    NODE ** hash_table = malloc( sizeof( NODE * ) * MAX_SIZE );
    memset( hash_table, 0x00, sizeof( NODE * ) * MAX_SIZE );
    
    
    for( i = 0; i < dictSize; i++ )
    {
        
        strcpy( str, dict[i] );
        for( j = 0; j < len; j++ )
        {
            
            str[j] = '*';
            printf("%d %d\r\n", i, j );
            if( hash_add( hash_table, str, len ) == true ) return true;
            printf("s %d %d\r\n", i, j );
            str[j] = dict[i][j];
        }
        
        // hash_delete( hash_table );
        
    
    }
    
    return false;
}