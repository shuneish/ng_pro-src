#include "scan.h"

FILE *file;
int cbuf;
int linenum;

int init_scan(char *filename){
    file = fopen(filename, "r");  // "read-only"
    if (file == NULL) return S_ERROR;

    cbuf = fgetc(file);
    linenum = 1;
    return 0;
}

int scan(){

    while (cbuf != EOF){
        cbuf = fgetc(file);

        switch (cbuf){
            /* separator */
            case ' ':
            case '\t':
                break;
            case '\n':
            case '\r':
                check_lineBreak();
                break;
            /* comment */
            case '{':
                while (cbuf != EOF){\
                    cbuf = fgetc(file);
                    if (cbuf == '}') break;
                    if (cbuf == '\n' || cbuf == '\r') check_lineBreak();
                }
                break;
            case '/':
                cbuf = fgetc(file);
                if (cbuf == '*'){
                    while (cbuf != EOF){
                        cbuf = fgetc(file);
                        if (cbuf == '*'){
                            cbuf = fgetc(file);
                            if (cbuf == '/') break;
                        }
                        if (cbuf == '\n' || cbuf == '\r') check_lineBreak();
                    }
                } else {
                    ungetc(cbuf, file);
                    return TDIV;
                }
                break;

            /* symbol */
            case '+':
                return TPLUS;
            case '-':
                return TMINUS;
            case '*':
                return TSTAR;
            case '(':
                return TLPAREN;
            case ')':
                return TRPAREN;
            case '[':
                return TLSQPAREN;
            case ']':
                return TRSQPAREN;
            case '.':
                return TDOT;
            case ',':
                return TCOMMA;
            case ':':
                cbuf = fgetc(file);
                if (cbuf == '=') return TASSIGN;
                ungetc(cbuf, file);
                return TCOLON;
            case ';':
                return TSEMI;
            case '=':  
                return TEQUAL;
            case '<':
                cbuf = fgetc(file);
                if (cbuf == '>') return TNOTEQ;
                if (cbuf == '=') return TLEEQ;
                ungetc(cbuf, file);

                return TLE;
            case '>':
                cbuf = fgetc(file);
                if (cbuf == '=') return TGREQ;
                ungetc(cbuf, file);

                return TGR;
            /* string */
            case '\'' :
                cbuf = fgetc(file);
                if (cbuf == EOF) return S_ERROR;
                if (cbuf == '\n' || cbuf == '\r') return S_ERROR;
                if (cbuf == '\'') {
                    cbuf = fgetc(file);
                    if (cbuf == EOF) return S_ERROR;
                    if (cbuf == '\n' || cbuf == '\r') return S_ERROR;
                    if (cbuf != '\'') return S_ERROR;
                }
                cbuf = fgetc(file);
                if (cbuf == EOF) return S_ERROR;
                if (cbuf == '\n'|| cbuf == '\r') return S_ERROR;
                if (cbuf != '\'') return S_ERROR;

                return TSTRING;
            /* unsigned integer */
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                // if (cbuf == '\''){
                //     cbuf = fgetc(file);
                //     if (isdigit(cbuf)) return S_ERROR;
                //     ungetc(cbuf, file);
                // }
                while (isdigit(cbuf = fgetc(file))){
                    ;
                }
                ungetc(cbuf, file);
                return TNUMBER;
            /* keyword or name */
            default:
                if (isalpha(cbuf) || cbuf == '_'){
                    char buf[256];
                    int i = 0;
                    buf[i++] = cbuf;
                    while (isalnum(cbuf = fgetc(file)) || cbuf == '_'){
                        buf[i++] = cbuf;
                    }
                    buf[i] = '\0';
                    ungetc(cbuf, file);

                    for (i = 0; i < KEYWORDSIZE; i++){
                        if (strcmp(buf, key[i].keyword) == 0){
                            return key[i].keytoken;
                        }
                    }

                    return TNAME;

                } else {
                    return S_ERROR;
                }
        }
    }

    return S_ERROR;
}

int get_linenum(){
    return linenum;
}

void end_scan(){
    fclose(file);
}

void check_lineBreak(){
    if (cbuf == '\n'){
       cbuf = fgetc(file);
                if (cbuf == '\r'){
                    /* \n\r */
                } else {
                    ungetc(cbuf, file);
                }
                linenum++;
    } else if (cbuf == '\r'){
        cbuf = fgetc(file);
                if (cbuf == '\n'){
                    /* \r\n */
                } else {
                    ungetc(cbuf, file);
                }
                linenum++;
    }
}

