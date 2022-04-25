#define MAX_CALLS    50000

typedef struct _node{
    int val;
    int id;
    struct _node * next;
} NODE;

typedef struct {
    
    NODE * buff[MAX_CALLS];
    int  * change; 
    int  * temp;
    int    length;
    int    index;
    int    id;
    
} SnapshotArray;


SnapshotArray* snapshotArrayCreate(int length) {
    
    SnapshotArray * snap = malloc( sizeof( SnapshotArray ) );
 
    snap->id     = 0;
    snap->index  = 0;
    snap->temp   = malloc( sizeof( int ) * length );
    snap->change = malloc( sizeof( int ) * length );
    snap->length = length;
    
    memset( snap->buff  , 0x00, sizeof( snap->buff ) );
    memset( snap->temp  , 0x00, sizeof( int ) * length );
    memset( snap->change, 0xFF, sizeof( int ) * length );
    return snap;
    
}

void snapshotArraySet(SnapshotArray* obj, int index, int val) {
  
    int i;
    
    obj->temp[index] = val;

    for( i = 0; i < obj->index; i++ )
    {
        if( obj->change[i] == index ) return;
    }

    obj->change[obj->index++] = index;

}

int snapshotArraySnap(SnapshotArray* obj) {
    
    int loop;
    int i;
    

    for( loop = 0; loop < obj->index; loop++ )
    {
        i = obj->change[loop];
        
        NODE *node = malloc( sizeof( NODE ) );
        node->val = obj->temp[i];
        node->id  = obj->id;
        node->next = NULL;
   
        if( obj->buff[i] == NULL )
        {
            obj->buff[i] = node;
        }
        else
        {
            node->next = obj->buff[i];
            obj->buff[i] = node;
        }
        
        obj->change[loop] = -1;
    }
    
    obj->index = 0;
    
    return obj->id++;
}

int snapshotArrayGet(SnapshotArray* obj, int index, int snap_id) {
  
    NODE * node = obj->buff[index];

    while( node != NULL )
    {
        if( snap_id < node->id ) 
        {
            node = node->next;
        }
        else
        {
            return node->val;
        }
        
    }
    
    return 0;
}

void snapshotArrayFree(SnapshotArray* obj) {
 
    int i;

    NODE * temp = NULL;
    for( i = 0; i < MAX_CALLS; i++ )
    {
        NODE * node = obj->buff[i];
        
        while( node != NULL )
        {
            temp = node;
            node = node->next;
            free( temp );
        }
    }
    
    free( obj->temp );
    free( obj->change );
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
