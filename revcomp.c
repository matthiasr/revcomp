#include <stdlib.h>
#include <stdio.h>
#include <strings.h>


char complement(char c) {
    if ('a' <= c && c <= 'z') c -= 'a'-'A';
    switch(c) {
#define complementCase(a, b) case (a): return (b); case(b): return (a)
        complementCase('G','C');
        complementCase('A','T');
        complementCase('M', 'K');
        complementCase('R', 'Y');
        complementCase('V', 'B');
        complementCase('H', 'D');
        default: return c;
    }
}

void complementAll(char* array, size_t len) {
    int i;
    for(i=0; i<len; i++)
        array[i] = complement(array[i]);
}

void reverse(char* array, size_t len) {
    char temp;
    size_t a = 0, b = len;
    while(a<b) {
        while(array[a] == '\n') a++;
        while(array[b] == '\n') b--;
        temp = array[a];
        array[a] = array[b];
        array[b] = temp;
        a++; b--;
    }
}

int main(int argc, char** argv) {
    size_t bufsize = 1024;
    size_t nread = 0;
    size_t blockstart=0, blockend=0;
    char* buf;
    buf = malloc(sizeof(char)*bufsize);

    while(!feof(stdin)) {
        nread += fread(buf+nread, sizeof(char), bufsize-nread, stdin);
        if(nread == bufsize) {
            bufsize *= 2;
            buf = realloc(buf, bufsize);
        }
    }
    
    while(blockstart < nread) {
        /* skip header */
        while(buf[blockstart] != '\n')
            blockstart++;
        blockend = blockstart;
        while(blockend < nread && buf[blockend] != '>')
            blockend++;
        blockend--;

        complementAll(buf+blockstart, blockend-blockstart);
        reverse(buf+blockstart, blockend-blockstart);

        blockstart = blockend+1;
    }

    fwrite(buf, sizeof(char), nread, stdout);
    exit(0);
}
