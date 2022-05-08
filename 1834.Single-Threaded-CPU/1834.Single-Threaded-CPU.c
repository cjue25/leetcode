
typedef struct _node
{
    unsigned int ts;
    unsigned int proc;
    unsigned int index;
} NODE;

typedef struct _heap
{
    unsigned int size;
    NODE ** max; 
    //NODE * max[MAX_SIZE]
} HEAP;


HEAP * heap_init( int size )
{
    HEAP * obj = malloc( sizeof( HEAP ) );
    obj->size = 0;
    obj->max = malloc( sizeof( NODE * ) * size );
    return obj;
}

void heap_deinit( HEAP * obj )
{
    free( obj->max );
    free( obj );
    return;
}
int compare( void * a, void * b )
{
    NODE * aa = *( NODE ** )a;
    NODE * bb = *( NODE ** )b;
    return ( aa->ts == bb->ts )? ( aa->proc - bb->proc ) : ( aa->ts - bb->ts );
}

int heap_compare( NODE * n1, NODE * n2 )
{
    if( n1->proc == n2->proc )
    {
        return n1->index < n2->index;
    }
    
    return n1->proc < n2->proc;
}

void swap( NODE ** a, NODE ** b )
{
    NODE * temp = *a;
    *a = *b;
    *b = temp;
}
    
void heap_add( HEAP * h, NODE * node )
{
    h->max[h->size++] = node;
    
    int parrent;
    int child  = h->size - 1;
    
    while( child > 0 )
    {
        parrent = ( child - 1 ) >> 1;
        if( heap_compare( h->max[parrent], h->max[child] ) ) break;
        
        swap( &h->max[parrent], &h->max[child] );
        child = parrent;
    }
    return;
    
}


NODE * heap_pop( HEAP * h )
{
    NODE * node = h->max[0];
    
    swap( &h->max[0], &h->max[--h->size] );
    
    int parrent = 0;
    int child   = ( parrent << 1 ) + 1;
    
    while( child < h->size )
    {
        if( child + 1 < h->size && heap_compare( h->max[child + 1], h->max[child] ) )
        {
            child++;
        }
        
        if( heap_compare( h->max[parrent], h->max[child] ) ) break;
        
        swap( &h->max[parrent], &h->max[child] );
        parrent = child;
        child = ( parrent << 1 ) + 1;
    }
    
    
    
    return node;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* getOrder(int** tasks, int tasksSize, int* tasksColSize, int* returnSize){
    
    
    NODE * node;
    NODE * list[tasksSize];
    int i;
    unsigned int curr = 0;
    int * ans = malloc( sizeof( int ) * tasksSize );
    memset( ans, 0x00, sizeof( int ) * tasksSize );
    
    *returnSize = 0;
    
    
    for( i = 0; i < tasksSize; i++ )
    {
        node = malloc( sizeof( NODE ) );
        node->ts = tasks[i][0];
        node->proc = tasks[i][1];
        node->index = i;
        list[i] = node;
    }
    
    qsort( list, tasksSize, sizeof( NODE * ), compare );
    
    HEAP * heap = heap_init( tasksSize );

    int j = 0;
    for( i = 0; i < tasksSize; i++ )
    {
        if( heap->size == 0 )
        {
            node = list[i];
            curr = ( curr > node->ts ) ? curr : node->ts;
        }
        
        while( j < tasksSize && list[j]->ts <= curr )
        {
            heap_add( heap, list[j] );
            j++;
        }
        
        node = heap_pop( heap );
        curr += node->proc;
        ans[(*returnSize)++] = node->index;
        free( node );
        
    }
        
    heap_deinit( heap );
    
    return ans;

}