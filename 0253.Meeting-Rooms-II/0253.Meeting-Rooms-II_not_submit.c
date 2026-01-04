int compare( void * a, void * b )
{
    int * aa = *( int ** ) a;
    int * bb = *( int ** ) b;

    return ( aa[0] == bb[0] ) ? b[1]-a[1]:a[0]-b[0];
}

typedef struct _heap
{
    int * buf;
    int   size;
    int   index;
} HEAP;

HEAP * heap_init( int size )
{
    HEAP * h = malloc( sizeof( HEAP ) );
    h->size = size;
    h->buf = malloc( sizeof( int ) * h->size );
    memset( h->buf, 0x00, sizeof( int ) * h->size );
    h->index = 0;
    return h;
}

void heap_free( HEAP * h )
{
    if( h == NULL ) return;
    free( h->buf );
    free( h );
    return;
}

void heap_add( HEAP * h, int val )
{
    h->buf[h->index++] = val;
    int parrent;
    int child = h->index - 1;
    while( child > 0 )
    {
        parrent = ( child - 1 ) >> 1;
        if( h->buf[parrent] <= h->buf[child] ) break;
        swap( &h->buf[parrent], &h->buf[child] );
    }
    return;
}

void heap_del( HEAP * h )
{
    swap( &h->buf[0], &h->buf[--h->index] );
    int parrent = 0;
    int child   = ( parrent << 1 ) + 1;

    while( child < h->index )
    {
        if( child + 1 < h->index && h->buf[child+1] < h->buf[child] ) child++;
        if( h->buf[parrent] <= h->buf[child] ) break;
        swap( &h->buf[parrent], &h->buf[child] );
        parrent = child;
        child   = ( parrent << 1 ) + 1;
    }
    return;
}


int min_meeting_rooms(int** intervals, int size, int* ColSize)
{
    qsort( intervals, size, sizeof( int * ), compare );

    HEAP * h = heap_init( size );

    int room = 0;
    heap_add( h, intervals[0][1] );
    room++;

    int i;
    for( i = 1; i < size; i++ )
    {
        int s = intervals[i][0];
        int e = intervals[i][1];

        int top = h->buf[0];
        if( s < top )
        {
            heap_add( h, e );
            room++;
        }
        else
        {
            heap_del( h );
            heap_add( h, e );
        }
    }

    heap_deinit( heap );
    return room;
}