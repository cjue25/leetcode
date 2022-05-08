/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
typedef struct _node
{
    int idx;
    int val;
} NODE;

typedef struct _heap
{
    int     index;
    int     size;
    NODE ** max;
} HEAP;

HEAP * heap_init( int size )
{
    HEAP * h = malloc( sizeof( HEAP ) );
    h->index = 0;
    h->size  = size;
    h->max   = malloc( sizeof( NODE * ) * h->size );
    memset( h->max, 0x00, sizeof( NODE * ) * h->size );
    return h;
}

void heap_deinit( HEAP * h )
{
    int i;
    for( i = 0; i < h->size; i++ )
    {
        if( h->max[i] != NULL ) free( h->max[i] );
    }

    free( h->max );
    free( h );
    return;
}

void swap( NODE ** a, NODE ** b )
{
    NODE *temp = *a;
    *a = *b;
    *b = temp;
    return;
}

void heap_update_up( HEAP * h, int index )
{
    int child = index;
    int parrent;
    
    while( child > 0 )
    {
        parrent = ( child - 1 ) >> 1;
        if( h->max[parrent]->val >= h->max[child]->val ) break;

        swap( &h->max[parrent], &h->max[child] );

        child = parrent;
    }
    
    return;
}

void heap_update_down( HEAP * h, int index )
{
    int parrent = index;
    int child   = ( index << 1 ) + 1;
    
    while( child < h->index )
    {
        if( child + 1 < h->index && h->max[child]->val < h->max[child+1]->val )
        {
            child++;
        }
        
        if( h->max[parrent]->val >= h->max[child]->val ) break;
        swap( &h->max[parrent], &h->max[child] );

        parrent = child;
        child   = ( parrent << 1 ) + 1;
    }

    return;
}

int heap_max( HEAP * h )
{
    return h->max[0]->val;
}

void heap_add( HEAP * h, int index, int value )
{
    NODE * node   = malloc( sizeof( NODE ) );
    node->idx     = index;
    node->val     = value;
    
    
    h->max[h->index++] = node;
    
    heap_update_up( h, h->index - 1 );
    return;
}

void heap_pop( HEAP * h )
{
    NODE * node = h->max[0];
    swap( &h->max[0], &h->max[--h->index] );
    heap_update_down( h, 0 );
    h->max[h->index] = NULL;
    free( node );
    return;
}

int* maxSlidingWindow(int* nums, int numsSize, int k, int* returnSize){
    
    HEAP * h     = heap_init( numsSize );
    int i;

    for( i = 0; i < k; i++ )
    {
        heap_add( h, i, nums[i] );
    }
    

    int * ans = malloc( sizeof( int ) * numsSize - k + 1  );
    (*returnSize) = 0;
    ans[(*returnSize)++] = heap_max( h );
    
    for( i = k; i < numsSize; i++ )
    {    
        heap_add( h, i, nums[i] );

        while( h->max[0]->idx <= i - k )
        {
            heap_pop( h );
        }
        
        
        ans[(*returnSize)++] = heap_max( h );
    }
    

    
    heap_deinit( h );
    

    return ans;
    
    
    
    
}