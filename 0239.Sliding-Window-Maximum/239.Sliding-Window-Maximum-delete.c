/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
typedef struct _node
{
    int idx;
    int max_idx;
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
    
    return h;
}

void heap_deinit( HEAP * h )
{
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

        h->max[parrent]->max_idx = parrent;
        h->max[child]->max_idx   = child;

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

        h->max[parrent]->max_idx = parrent;
        h->max[child]->max_idx   = child;

        parrent = child;
        child   = ( parrent << 1 ) + 1;
    }

    return;
}

int heap_max( HEAP * h )
{
    return h->max[0]->val;
}

void heap_add( HEAP * h, int index, int value, NODE ** hash )
{
    NODE * node   = malloc( sizeof( NODE ) );
    node->idx     = index;
    node->val     = value;
    node->max_idx = h->index;
    
    hash[index]   = node;
    h->max[h->index++] = node;
    
    heap_update_up( h, h->index - 1 );
    return;
}

void heap_delete( HEAP * h, int index, NODE ** hash )
{
    NODE * node = hash[index];
    int    replace = node->max_idx;
    
    swap( &h->max[replace], &h->max[--h->index] );
    h->max[replace]->max_idx = replace;
    
    heap_update_up  ( h, node->max_idx );    
    heap_update_down( h, node->max_idx );
    
    return;
}

int* maxSlidingWindow(int* nums, int numsSize, int k, int* returnSize){
    
    HEAP * h     = heap_init( numsSize );
    NODE ** hash = malloc( sizeof( NODE * ) * numsSize );

    memset( hash, 0x00, sizeof( NODE * ) * numsSize );

    int i;

    for( i = 0; i < k; i++ )
    {
        heap_add( h, i, nums[i], hash );
    }
    

    int * ans = malloc( sizeof( int ) * numsSize - k + 1  );
    (*returnSize) = 0;
    ans[(*returnSize)++] = heap_max( h );
    
    for( i = k; i < numsSize; i++ )
    {    
        heap_add( h, i, nums[i], hash );
        heap_delete( h, i-k, hash );
        
        ans[(*returnSize)++] = heap_max( h );
    }
    

    for( i = 0; i < numsSize; i++ )
    {
        free( hash[i] );
    }

    free( hash );
    heap_deinit( h );
    

    return ans;
    
    
    
    
}