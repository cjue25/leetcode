/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */

typedef struct _list
{
    int   index;
    int   depth;
    int   visited;
    int * adjlist;
} LIST;

typedef struct _qnode
{
    int index;
    struct _qnode * next;
} QNODE;

typedef struct _queue
{
    int push;
    int pop;
    QNODE * head;
    QNODE * tail;
} QUEUE;


void queue_push( QUEUE * q, int index )
{
    QNODE * node = malloc( sizeof( QNODE ) );
    node->index = index;
    node->next  = NULL;
    
    if( q->head == NULL )
    {
        q->head = node;
        q->tail = node;
    }
    else
    {
        q->tail->next = node;
        q->tail       = node;
    }
    
    q->push++;
    return;
}

QNODE * queue_pop( QUEUE * q )
{
    QNODE * node = q->head;
    
    q->head = q->head->next;
    q->pop++;
    return node;
}

int queue_size( QUEUE * q )
{
    return q->push - q->pop;
}

int check( char * src, char * dst, int len )
{
    int i;
    int diff = 0;
    
    for( i = 0; i < len; i++ )
    {
        if( src[i] != dst[i] )
        {
            if( diff == 0 ) diff = 1;
            else            return false;
        }
    }
    
    return true;
}


void dfs( char ** wordList, int depth, int *** ans, int * col, int * size, LIST * list, int index, char ** onelist, int end_index , int curr_index )
{
    int i;
    
    LIST *ll = &list[curr_index];
    
    if( curr_index == end_index )
    {
  
        char ** new = malloc( sizeof( char * ) * depth );
        onelist[depth-1] = wordList[end_index];
        memcpy( new, onelist, sizeof( char * ) * depth );
        ans[(*size)]   = new;
        col[(*size)++] = depth;
        
        return;
    }
   
    // if( index >= depth ) return;
   
    for( i = 0; i < ll->index; i++ )
    {
        int next_index = ll->adjlist[i];
        
        /* only add when the depth is matched and smaller*/
        if( ( list[next_index].depth == index + 1 ) && index < depth )
        {
            onelist[index] = wordList[next_index];
            dfs( wordList, depth, ans, col, size, list, index + 1, onelist, end_index, next_index );    
        }
        
    }
    
    return;
    
}


char *** findLadders(char * beginWord, char * endWord, char ** wordList, int wordListSize, int* returnSize, int** returnColumnSizes){
    
    int i;
    int j;
    int end_idx   = -1;
    int start_idx = -1;
    int len     = strlen( beginWord );
    int size    = 0;
    int depth   = 1;
    int end     = false;
    int list_size = wordListSize;
    
    
    /* prepare ans */
    char *** ans = NULL;
    *returnSize = 0;
    *returnColumnSizes = NULL;
    
    /* find start and end index */
    for( i = 0; i < wordListSize; i++ ) 
    {
        if( !strcmp( wordList[i], endWord ) )    end_idx = i;
        if( !strcmp( wordList[i], beginWord ) )  start_idx = i;
    }
    
    /* if no endword, return NULL */
    if( end_idx < 0 ) return NULL;
    
    /* add beginword to wordList */
    if( start_idx < 0 )
    {
        wordList = realloc( wordList, sizeof( char * ) * ( wordListSize + 1  ) );
        wordList[wordListSize] = beginWord;
        start_idx = wordListSize;
        list_size++;
    }
    
    
    /* prepare adjlist buffer */
    LIST    * list    = malloc( sizeof( LIST ) * list_size );
    
    for( i = 0; i < list_size; i++ )
    {
        list[i].index   = 0;
        list[i].depth  = 0;
        list[i].visited = false;
        
        list[i].adjlist = malloc( sizeof( int ) * list_size );
        memset( list[i].adjlist, 0x00, sizeof( int ) * list_size );
    }
    
    /* generate adjlist */
    for( i = 0; i < list_size - 1; i++ )
    {
        for( j = i + 1; j < list_size; j++ )
        {
            if( check( wordList[i], wordList[j], len ) == true )   
            {
                LIST * ll = &list[i];
                ll->adjlist[ll->index++] = j;    
                
                if( i == start_idx ) continue;
                
                ll = &list[j];
                ll->adjlist[ll->index++] = i; 
            }
            
        }

    }
    
    /* allocate queue */
    QUEUE * queue = malloc( sizeof( QUEUE ) );
    memset( queue, 0x00, sizeof( QUEUE ) );
    
    queue_push( queue, start_idx );
    list[start_idx].visited = true;
    list[start_idx].depth  = 1;
    
    /* bfs */
    while( ( size = queue_size( queue ) ) > 0 )
    {
      
        while( --size >= 0 )
        {
            QNODE * q = queue_pop( queue );
            
            LIST * ql = &list[q->index];
            
            for( i = 0; i < ql->index; i++ )
            {
                int adjIndex = ql->adjlist[i];
                
                /* if first found this word, remember the depth and labeled visited */
                if( list[adjIndex].visited == false )
                {
                    list[adjIndex].visited = true;
                    list[adjIndex].depth  = list[q->index].depth + 1;
                    queue_push( queue, adjIndex );
                }
                
                if( adjIndex == end_idx ) end++;
            }
        }
        
        depth++;
        if( end != 0 ) break;
        
    }
    
    
    /*prepare ans buffer*/
    end = list_size;
    ans = malloc( sizeof( char ** ) * end );
    
    for( i = 0; i < end; i++ )
    {
        ans[i] = malloc( sizeof( char * ) * depth );
        memset( ans[i], 0x00, sizeof( char * ) * depth );
    }
    
    
    int * col = malloc( sizeof ( int ) *  end );
    memset( col, 0x00, sizeof( int ) * end );
    
    /* for the temp ans */
    char ** onelist = malloc( sizeof( char * ) * depth );
    memset( onelist, 0x00, sizeof( char * ) * depth );
    
    
    /* start */
    onelist[0] = beginWord;
    
    
    /* dfs */
    dfs( wordList, depth, ans, col, returnSize, list, 1, onelist, end_idx, start_idx );
    
    *returnColumnSizes = col;
    
    
    /* free */
    free( onelist );
    
    for( i = 0; i < list_size; i++ ) free( list[i].adjlist );
    free( list );
    
    while( queue->head != NULL )
    {
        QNODE * temp = queue->head;
        queue->head  = queue->head->next;
        free( temp );
    }
    
    free( queue );
    
    
    
    return ans;
}