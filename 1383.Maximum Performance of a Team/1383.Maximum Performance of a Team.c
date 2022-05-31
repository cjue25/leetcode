/*
choose k engineers
minimum efficiency
all sum speed 

*/
#define MODULO ( 1000 * 1000 * 1000 + 7 )
typedef struct _node
{
    int speed;
    int eff;
} NODE;

typedef struct _heap
{
    int size;
    int index;
    NODE ** buf;
} HEAP;

HEAP * heap_init( int size )
{
    HEAP * h = malloc( sizeof( HEAP ) );
    memset( h, 0x00, sizeof( HEAP ) );
    h->size = size;
    h->buf = malloc( sizeof( NODE * ) * size );
    memset( h->buf, 0x00, sizeof( NODE * ) * size );
    return h;
}

void heap_free( HEAP * h )
{
    free( h->buf );
    free( h );
    return;
}

void swap( NODE ** a, NODE ** b )
{
    NODE * temp = *a;
    *a = *b;
    *b = temp;
    return;
}
void heap_push( HEAP * h, NODE * node )
{
    if( h->index > h->size ) return;
    
    h->buf[h->index++] = node;
    
    int child = h->index-1;
    int parrent;
    
    while( child > 0 )
    {
        parrent = ( child - 1 ) >> 1;
        if( h->buf[child]->speed > h->buf[parrent]->speed ) break;
        swap( &h->buf[child], &h->buf[parrent] );
        child = parrent;
    }
    
    return;
}

int heap_pop( HEAP * h )
{
    int ret = h->buf[0]->speed;
    swap( &h->buf[0], &h->buf[--h->index] );
    
    int parrent = 0;
    int child   = ( parrent << 1 ) + 1;
    
    while( child < h->index )
    {
        if( child + 1 < h->index && h->buf[child+1]->speed < h->buf[child]->speed )
        {
            child++;
        }
        if( h->buf[child]->speed > h->buf[parrent]->speed ) break;
        swap( &h->buf[child], &h->buf[parrent] );
        parrent = child;
        child   = ( parrent << 1 ) + 1;
    }
    
    
    return ret;
}

int compare( void * a, void * b )
{
    NODE * aa = *( NODE ** )a;
    NODE * bb = *( NODE ** )b;
    return bb->eff - aa->eff;
}
int maxPerformance(int n, int* speed, int speedSize, int* efficiency, int efficiencySize, int k){
    
    NODE * list[speedSize];
    int i;
    for( i = 0; i < speedSize; i++ )
    {
        list[i] = malloc( sizeof( NODE ) );
        list[i]->speed = speed[i];
        list[i]->eff   = efficiency[i];
    }
    
    qsort( list, speedSize, sizeof( NODE * ), compare );
    
    HEAP * h = heap_init( speedSize );
    long long int sum = 0;
    long long int ret = INT_MIN;
    for( i = 0; i < speedSize; i++ )
    {
        if( i > k-1 )
        {
            sum -= heap_pop( h );;
        }
        
        heap_push( h, list[i] );
        sum += list[i]->speed;
        
        ret = ( sum * list[i]->eff > ret ) ? sum * list[i]->eff:ret;
    }
    
    
    for( i = 0; i < speedSize; i++ ) free( list[i] );
    
    heap_free( h );
    
    return ret % MODULO;
}