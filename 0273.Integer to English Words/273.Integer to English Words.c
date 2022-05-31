#define MAX_LENGTH ( 10000 )

char* single_digits[] = 
{ "One", "Two", "Three", "Four", 
  "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", 
  "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen" };
 
char* tens_multiple[] = 
{ "Twenty", "Thirty", "Forty",
  "Fifty", "Sixty", "Seventy", "Eighty", "Ninety" };
 
char* tens_power[] = { "Hundred", "Thousand", "Million", "Billion"};
int  tens_num[] = { 100, 1000, 1000000, 1000000000 };

void convert_to_words( int num, char * buf, int * len )
{
    int i;
    if( num == 0 ) return;
    
    if( num < 20 ) 
    {
        *len += snprintf( buf + *len, MAX_LENGTH - *len, " %s",  single_digits[num-1] );
        return;
    }
    
    if ( num < 100 ) 
    {
        *len += snprintf( buf + *len , MAX_LENGTH - *len, " %s", tens_multiple[num/10-2] );
        convert_to_words( num % 10, buf, len );
        
        return;
    }
    
    for (int i = 3; i >= 0; --i)
    {
      if (num >= tens_num[i]) 
      {
          convert_to_words( num / tens_num[i], buf, len );
          *len += snprintf( buf + *len , MAX_LENGTH - *len, " %s", tens_power[i] );
          convert_to_words( num % tens_num[i], buf, len );
          return;
      }
          
    }
    return;
}
 
char * numberToWords(int num){
    
    if( num ==  0 ) return "Zero";
    
    char * buf = malloc( sizeof( char  ) * MAX_LENGTH );
    memset( buf, 0x00, sizeof( char  ) * MAX_LENGTH );
    
    int len = 0;
    
    convert_to_words( num, buf, &len );
    
    buf[len] = NULL;
    
    return buf+1;
    
}