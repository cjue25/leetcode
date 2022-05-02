#define MAX_SIZE 1024
#define MAX_MSG  32
typedef struct _node
{
    char   msg[MAX_MSG];
    int    ts;
    struct _node * next;
} NODE;

typedef struct {
    NODE * msg[MAX_SIZE];
    
} Logger;

int hash_function( char * str )
{
    int sum = 0;
    
    while( (*str) )
    {
        sum += (*str);
        str++;
    }
    return sum & ( MAX_SIZE - 1 ) ;
}

Logger* loggerCreate() {
    Logger * obj = malloc( sizeof( Logger ) );
    memset( obj->msg, 0x00, sizeof( obj->msg ) );
    return obj;
}

bool loggerShouldPrintMessage(Logger* obj, int timestamp, char * message) {
    
    int hash = hash_function( message );
    NODE * node = obj->msg[hash];
    
    while( node != NULL )
    {
        if( !strcmp( node->msg, message ) )
        {
            if( timestamp >= node->ts ) 
            {
                node->ts = timestamp + 10;
                return true;
            }
            else return false;
        }       
        else
        {
           node = node->next;
        }
    }
    
    node = malloc( sizeof( NODE ) );
    node->msg[sizeof( node->msg) - 1] = 0x00;
    strncpy( node->msg, message, sizeof( node->msg ) - 1 );
    node->ts  = timestamp + 10;
        
    node->next = obj->msg[hash];
    obj->msg[hash] = node;
    return true;
    
}

void loggerFree(Logger* obj) {
    
    int i;
    
    for( i = 0 ; i < sizeof( obj->msg ) / sizeof( obj->msg[0] ); i++ )
    {
        NODE * node = obj->msg[i];
        while( node != NULL )
        {
            NODE * temp = node;
            node = node->next;
            free( temp );
        }
        
    }
    
    free( obj );
    
}

/**
 * Your Logger struct will be instantiated and called as such:
 * Logger* obj = loggerCreate();
 * bool param_1 = loggerShouldPrintMessage(obj, timestamp, message);
 
 * loggerFree(obj);
*/