#include "scan.h"

FILE *file;
int cbuf;

int init_scan(char *filename){
    file = fopen(filename, "r");  // "read-only"
    cbuf = fgetc(file);
    return (file == NULL) ? -1 : 0;
}

int scan(){
    if (cbuf == EOF) return -1;
    while (cbuf == ' ' || cbuf == '\t' || cbuf == '\n' || cbuf == '\r'){
        cbuf = fgetc(file);
    }

    return 0;
}

int get_linenum(){

    return 0;
}

void end_scan(){
    fclose(file);
}