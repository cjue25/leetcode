
#define MAX_SIZE  ( 1 << 17 )


typedef struct _node
{
    int val;
    int ts;
    int min_idx;
    int max_idx;
    struct _node * next;
} NODE;

typedef struct {
    int    curr_ts;
    int    curr_val;
    NODE * hash[MAX_SIZE];
    NODE * max[MAX_SIZE];
    NODE * min[MAX_SIZE];
    int    max_idx;
    int    min_idx;
} StockPrice;

void swap( NODE ** a, NODE ** b )
{
    NODE *temp = *a;
    *a = *b;
    *b = temp;
    return;
}

void heap_up( StockPrice * obj, int index, int max )
{
    int parrent;
    int child = index;
    if( max == true )
    {

        while( child > 0 )
        {
            parrent = ( child - 1 ) >> 1;
            if( obj->max[parrent]->val >= obj->max[child]->val ) break;
            swap( &obj->max[parrent], &obj->max[child] );    
            obj->max[parrent]->max_idx = parrent;
            obj->max[child]->max_idx   = child;
            child = parrent;
        }
        
    }
    else
    {
   
        
        while( child > 0 )
        {
            parrent = ( child - 1 ) >> 1;
            if( obj->min[parrent]->val <= obj->min[child]->val ) break;
            swap( &obj->min[parrent], &obj->min[child] );    
            obj->min[parrent]->min_idx = parrent;
            obj->min[child]->min_idx   = child;
            child = parrent;
        }
    }
    
    return;
}

void heap_down( StockPrice * obj, int index, int max  )
{
    int parrent = index;
    int child  = ( parrent << 1 ) + 1;
    if( max == true )
    {
        while( child < obj->max_idx )
        {
            
            if( child + 1 < obj->max_idx && obj->max[child]->val < obj->max[child+1]->val )
            {
                child += 1;
            }
            
            if( obj->max[parrent]->val >= obj->max[child]->val ) break;
            swap( &obj->max[parrent], &obj->max[child] );    
            obj->max[parrent]->max_idx = parrent;
            obj->max[child]->max_idx   = child;
            parrent = child;
            child = ( parrent << 1 ) + 1;
            
        }
        
        
    }
    else
    {
        while( child < obj->min_idx )
        {
            
            if( child + 1 < obj->min_idx && obj->min[child]->val > obj->min[child+1]->val )
            {
                child += 1;
            }
            
            if( obj->min[parrent]->val <= obj->min[child]->val ) break;
            swap( &obj->min[parrent], &obj->min[child] );    
            obj->min[parrent]->min_idx = parrent;
            obj->min[child]->min_idx   = child;
            parrent = child;
            child = ( parrent << 1 ) + 1;
            
        }
    }
    
    
    return;
}
StockPrice* stockPriceCreate() {
    
    StockPrice * obj = malloc( sizeof ( StockPrice ) );
    memset( obj, 0x00, sizeof( StockPrice ) );
    printf("%d\r\n", sizeof( obj->hash ) );
    return obj;
}

void stockPriceUpdate(StockPrice* obj, int timestamp, int price) {
    
    if ( timestamp >= obj->curr_ts ) 
    {
        obj->curr_val = price;
        obj->curr_ts = timestamp;
    }
    
    int h = timestamp & ( MAX_SIZE - 1 );
    NODE  * node = obj->hash[h];
    while( node != NULL )
    {
        if( node->ts == timestamp )
        {
            node->val = price;
            heap_up( obj, node->max_idx, true );
            heap_down( obj, node->max_idx, true );
            heap_up( obj, node->min_idx, false );
            heap_down( obj, node->min_idx, false );
            return;
        }
        node = node->next;
    }
    
    /* add */
    node = malloc( sizeof( NODE ) );
    node->val       = price;
    node->ts        = timestamp;
    node->min_idx   = obj->min_idx;
    node->max_idx   = obj->max_idx;
    node->next      = obj->hash[h];
    
    obj->hash[h] = node;
    obj->max[obj->max_idx++] = node;
    obj->min[obj->min_idx++] = node;
    
    heap_up( obj, obj->max_idx - 1, true );
    heap_up( obj, obj->min_idx - 1, false );
    
    return;
    
}

int stockPriceCurrent(StockPrice* obj) {
    return obj->curr_val;
}

int stockPriceMaximum(StockPrice* obj) {
    return obj->max[0]->val;
  
}

int stockPriceMinimum(StockPrice* obj) {
    return obj->min[0]->val;
  
}

void stockPriceFree(StockPrice* obj) {
    
    int i;
    NODE * node;
    for( i = 0; i < sizeof( obj->hash ) / sizeof( obj->hash[0] ) ; i++ )
    {
        node = obj->hash[i];
        while( node != 0 )
        {
            NODE * temp = node;
            node = node->next;
            free( temp );
        }
    }
    
    free( obj );
    
}

/**
 * Your StockPrice struct will be instantiated and called as such:
 * StockPrice* obj = stockPriceCreate();
 * stockPriceUpdate(obj, timestamp, price);
 
 * int param_2 = stockPriceCurrent(obj);
 
 * int param_3 = stockPriceMaximum(obj);
 
 * int param_4 = stockPriceMinimum(obj);
 
 * stockPriceFree(obj);
*/