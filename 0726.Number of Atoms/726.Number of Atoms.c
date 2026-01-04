#define MAX_SIZE  ( 128 )

typedef struct _node
{
    char * name;
    int    num;
    struct _node * next;
} NODE;

int hash_key( char * str )
{
    int i;
    int num = 0;
    for( i = 0; str[i] != 0; i++ )
    {
        num += str[i];
    }
    
    return num & ( MAX_SIZE - 1 );
}

void hash_add( NODE ** h, char * str, int num )
{
    int key = hash_key( str );
    NODE * node = h[key];
    while( node != NULL )
    {
        if( !strcmp( node->name, str ) )
        {
            node->num += num;
            return;
        }
        node = node->next;
    }
    
    node = malloc( sizeof( NODE ) );
    int len = strlen( str );
    
    node->name = malloc( len + 1 );
    node->name[len] = 0;
    strncpy( node->name, str, len );
    node->num = num;
    node->next = h[key];
    h[key]     = node;
    return;
        
}

int cmp( void * a, void * b )
{
    NODE * aa = *( NODE ** ) a;
    NODE * bb = *( NODE ** ) b;
    return ( strcmp( aa->name, bb->name ) );
    
}
char * countOfAtoms(char * formula){
    
    NODE ** table = malloc( sizeof( NODE * ) * MAX_SIZE );
    memset( table, 0x00, sizeof( NODE * ) * MAX_SIZE );
    
    NODE ** stack[MAX_SIZE];
    memset( stack, 0x00, sizeof( stack ) );
    
    int push = 0;
    
    int i;
    int j;
    int k;
    int size;
    int num;
    for( i = 0; formula[i] != 0 ; i++ )
    {
        if( formula[i] == '(' )
        {
            stack[push++] = table;
            table = malloc( sizeof( NODE * ) * MAX_SIZE );
            memset( table, 0x00, sizeof( NODE * ) * MAX_SIZE );
        }
        else if( formula[i] == ')')
        {
            NODE ** temp = table;
            
            j = i + 1;
            while( formula[j] != 0 && isdigit( formula[j] ) )
            {
                j++;
            }
            
            size = j - i;
            
            if( size <= 1 ) num = 1;
            else
            {
                char val[size];
                val[size-1] = 0x00;
                strncpy( val, formula + i + 1, size - 1);
                num = atoi( val );
            }
            
            table = stack[--push];
            for( k = 0; k < MAX_SIZE; k++ )
            {
                NODE * n = temp[k];
                while( n != NULL )
                {
                    NODE * fnode = n;
                    hash_add( table, n->name, n->num * num );
                    n = n->next;
                    free( fnode->name );
                    free( fnode );
                }
            }
            
            free( temp );
            
            i = j - 1;
        }
        else if( formula[i] >= 'A' && formula[i] <= 'Z' )
        {
            
            j = i + 1;
            while( formula[j] != 0 && formula[j] >= 'a' && formula[j] <= 'z')
            {
                j++;
            }
            
            size = j - i + 1;        
            char name[size];
            name[size-1] = 0x00;
            strncpy( name, formula + i, size - 1 );
            
            i = j;
            
            while( formula[j] != 0 && isdigit( formula[j] ) )
            {
                j++;
            }
            
            size = j - i + 1;
            
            if( size <= 1 ) num = 1;
            else
            {
                char val[size];
                val[size-1] = 0x00;
                strncpy( val, formula + i, size - 1);
                num = atoi( val );
            }
            
            hash_add( table, name, num );
            
            i = j - 1;
            
        }
            
        
    }
    
    NODE * element[1000];
    memset( element, 0x00, sizeof( element ) );
    push = 0;
    
    for( i = 0; i < MAX_SIZE; i++ )
    {
        NODE * node = table[i];
        while( node != NULL )
        {
            element[push++] = node;
            node = node->next;
        }
    }
    
    qsort( element, push, sizeof( NODE * ), cmp );
    
    char * ans = malloc( 10000 );
    memset( ans, 0x00, 10000 );
    
    char * p = ans;
    
    for( i = 0; i < push; i++ )
    {
        p += sprintf( p, "%s", element[i]->name );
        if( element[i]->num > 1 )
        {
            p += sprintf( p, "%d", element[i]->num );
        }
        
        free( element[i]->name );
        free( element[i] );
    }
    
    
    free( table );
    
    
    return ans;
}