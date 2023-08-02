#include<stdio.h>
int len1(char* s){
    int start = 0, end = 0, maxlen=0;
    char counter[256] = {0};
    counter[(int)*(s+start)] = 1;

    while(*(s + end) != 0)
    {
        
        ++end;
        maxlen = maxlen>(end-start)?maxlen:(end-start);
        while( 0 != counter[(int)*(s+end)])
        {
            counter[(int)*(s+start)] = 0;
            ++start;
        }
        counter[(int)*(s+end)] = 1;
    }
    return maxlen;
}
int main(){
    int maxlen = 0;
    char s[] = "1";
    maxlen = len1(s);
    printf("%d",maxlen);
    return 0;
    
}