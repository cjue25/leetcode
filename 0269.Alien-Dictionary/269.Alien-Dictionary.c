typedef struct _node
{
    char c;
    struct _node * next;
} NODE;

typedef struct _queue
{
    NODE * head;
    NODE * tail;
    int    push;
    int    pop;
}QUEUE;

void queue_push( QUEUE * q, char c )
{
    NODE * node = malloc( sizeof( NODE ) );
    node->c = c;
    node->next = NULL;
    if( q->head != NULL )
    {
        q->tail->next = node;
        q->tail       = node;
    }
    else
    {
        q->head = node;
        q->tail = node;
    }
    q->push++;
    return;
}

NODE * queue_pop( QUEUE * q )
{
    NODE * node = q->head;
    q->head = q->head->next;
    q->pop++;
    
    return node;
}

int queue_size( QUEUE * q )
{
    return q->push - q->pop;
}

void create_adjlist( char c1, char c2, NODE ** list, unsigned int * indegree )
{
    c1 = c1 - 'a';
    c2 = c2 - 'a';
    
    NODE * node = list[c1];
    
    while( node != NULL )
    {
        if( node->c == c2 ) return;
        node = node->next;
    }
    
    node        = malloc( sizeof( NODE ) );
    node->c     = c2;
    node->next  = list[c1];
    list[c1]    = node;
    
    indegree[c2]++;
    
    return;
}
char * alienOrder(char ** words, int wordsSize){
    
    NODE ** list = malloc( sizeof( NODE * ) * 26 );
    memset( list, 0x00, sizeof( NODE * ) * 26 );
    
    QUEUE * q = malloc( sizeof( QUEUE ) );
    memset( q, 0x00, sizeof( QUEUE ) );
    
    char * ans = malloc( 27 );
    memset( ans, 0x00, 27 );
    
    unsigned int index = 0;
    unsigned int indegree[26];
    unsigned int first[26];
    memset( indegree, 0x00, sizeof( indegree ) );
    memset( first   , 0xFF, sizeof( first    ) );
    
    
    NODE * node;
    char * s1;
    char * s2;
    int     i;
    int     j;
    
    for( i = 0; i < wordsSize; i++ )
    {
        s1 = words[i];
        for( j = 0; s1[j] != 0; j++ ) first[s1[j]-'a'] = 0;
    }
    
    for( i = 0; i < wordsSize - 1; i++ )
    {
        s1 = words[i];
        s2 = words[i+1];
        j = 0;
        
        int found = false;
        
        while( s1[j] != 0 && s2[j] != 0 )
        {
            if( s1[j] != s2[j] )
            {
                found = true;
                create_adjlist( s1[j], s2[j], list, indegree );
                break;
            }
            
            j++;
        }
        
        if( found == false && s1[j] != 0 ) goto free_exit;
        
    }
    
    for( i = 0 ; i < sizeof( indegree ) / sizeof( indegree[0] ); i++ )
    {
        if( first[i] == 0 && indegree[i] == 0 )
        {
            queue_push( q, i );
        }
    }
    
    while( queue_size( q ) > 0 )
    {
        NODE * qnode = queue_pop( q );
        NODE * qlist = list[qnode->c];
        
        while( qlist != NULL )
        {
            if( qlist->c >= 0 && --indegree[qlist->c] == 0 ) queue_push( q, qlist->c );
            qlist = qlist->next;
        }
        ans[index++] = qnode->c + 'a';
        free( qnode );

    }
    
    
free_exit:
    
    for( i = 0; i < sizeof( indegree ) / sizeof( indegree[0] ); i++ )
    {
        node = list[i];
        while( node != NULL )
        {
            NODE * temp = node;
            node = node->next;
            free( temp );
        }
        
        if( indegree[i] != 0 ) ans[0] = 0;
    }
    
    
    if ( list != NULL ) free( list );
    if ( q    != NULL ) free( q );
    
    return ans;
}