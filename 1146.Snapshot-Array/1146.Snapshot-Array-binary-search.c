#define MAX_NODE_SIZE   4
typedef struct _node{
    int val;
    int id;
} NODE;

typedef struct _item
{
    NODE * node;
    int    size;
    int    idx;
    int    set;
} ITEM;

typedef struct {
    
    ITEM * items;
    int  * change;
    int    change_count;
    int    change_size;
    int    length;
    int    id;
    
} SnapshotArray;


SnapshotArray* snapshotArrayCreate(int length) {
    
    int loop;
    
    SnapshotArray * snap = malloc( sizeof( SnapshotArray ) );
 
    snap->id            = 0;
    snap->length        = length;
    snap->change_count  = 0;
    snap->change_size   = MAX_NODE_SIZE;
    
    snap->change  = malloc( sizeof( int  ) * snap->change_size );
    snap->items   = malloc( sizeof( ITEM ) * length );
    
    memset( snap->change, 0xFF, sizeof( int ) * snap->change_size );
    
    for( loop = 0; loop < snap->length; loop++ )
    {
        snap->items[loop].size = MAX_NODE_SIZE;
        snap->items[loop].idx  = 0;
        snap->items[loop].node = malloc( snap->items[loop].size * sizeof( NODE ) );
        memset( snap->items[loop].node, 0x00, snap->items[loop].size * sizeof( NODE ) );
    }
    
    
    return snap;
    
}


void snapshotArraySet(SnapshotArray* obj, int index, int val) {
  
    
    ITEM * item = &obj->items[index];
    int       i;
    
    /* new value replace */
    item->set = val;
    
    /* ever changed */
    for( i = 0; i < obj->change_count; i++ ) if( obj->change[i] == index ) return;
    
    /* add changed */
    obj->change[obj->change_count++] = index;
    
    /* expand */
    if( obj->change_count >= obj->change_size ) 
    {
        obj->change_size *= 2;
        obj->change = realloc( obj->change, sizeof( int ) * obj->change_size );
        for( i = obj->change_count; i < obj->change_size; i++ ) obj->change[i] = -1;
    }
    
    return;
}

int snapshotArraySnap(SnapshotArray* obj) {
    
    NODE * node;
    ITEM * item; 
    int index;
    int i;
    
    for( i = 0; i < obj->change_count; i++ )
    {
        index = obj->change[i];
        
        item  = &obj->items[index];
        
        node      = &item->node[item->idx++];
        node->val = item->set;
        node->id  = obj->id;
        
        /* expand node */
        if( item->idx >= item->size )
        {
            item->node = realloc( item->node, sizeof( NODE ) * item->size * 2 );
            item->size *=2;
        }
        
        obj->change[i] = -1;
    }
    
    obj->change_count = 0;
    
    return obj->id++;
}

int snapshotArrayGet(SnapshotArray* obj, int index, int snap_id) {
  
    ITEM * item = &obj->items[index];
    
    int start = 0;
    int end   = item->idx;
    int middle;
    
    while( start < end )
    {
        middle = start + ( end - start ) / 2;
    
        if( item->node[middle].id > snap_id )
        {
            end = middle;
        }
        else
        {
            start = middle + 1;
        }
            
    }
    
    if( start - 1 < 0 ) return 0;
    return item->node[start - 1].val;
}

void snapshotArrayFree(SnapshotArray* obj) {
 
    int i;
    for (i = 0; i < obj->length; i++) free( obj->items[i].node );
   
    free( obj->items );
    free( obj );
}

/**
 * Your SnapshotArray struct will be instantiated and called as such:
 * SnapshotArray* obj = snapshotArrayCreate(length);
 * snapshotArraySet(obj, index, val);
 
 * int param_2 = snapshotArraySnap(obj);
 
 * int param_3 = snapshotArrayGet(obj, index, snap_id);
 
 * snapshotArrayFree(obj);
*/
