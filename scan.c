#include "scan.h"

FILE *file;
int cbuf;
int linenum;
int num_attr;
char string_attr[MAXSTRSIZE];

int init_scan(char *filename){
    file = fopen(filename, "r");  // "read-only"
    if (file == NULL) return error("Cannot open input file");

    cbuf = fgetc(file);
    linenum = 1;
    return 0;
}

int scan(){
    char buf[MAXSTRSIZE];
    int i = 0;

    cbuf = fgetc(file);

    if (cbuf == EOF) return S_ERROR;

    switch (cbuf) {
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
            while ((cbuf = fgetc(file)) != EOF) {
                if (cbuf == '}') break;
                if (cbuf == '\n' || cbuf == '\r') check_lineBreak();
            }
            if (cbuf == EOF) return error("Comment is not closed");
            break;
        case '/':
            cbuf = fgetc(file);
            if (cbuf == '*') {
                while ((cbuf = fgetc(file)) != EOF) {
                    if (cbuf == '*') {
                        cbuf = fgetc(file);
                        if (cbuf == '/') break;
                    }
                    if (cbuf == '\n' || cbuf == '\r') check_lineBreak();
                }
                if (cbuf == EOF) return error("Comment is not closed");
            } else {
                ungetc(cbuf, file);
                return TDIV;
            }
            break;

        /* symbol */
        case '+': return TPLUS;
        case '-': return TMINUS;
        case '*': return TSTAR;
        case '(': return TLPAREN;
        case ')': return TRPAREN;
        case '[': return TLSQPAREN;
        case ']': return TRSQPAREN;
        case '.': return TDOT;
        case ',': return TCOMMA;
        case ':':
            cbuf = fgetc(file);
            if (cbuf == '=') return TASSIGN;
            ungetc(cbuf, file);
            return TCOLON;
        case ';': return TSEMI;
        case '=': return TEQUAL;
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
        case '\'':
            i = 0;
            while (1) {
                cbuf = fgetc(file);
                if (cbuf == EOF || cbuf == '\n' || cbuf == '\r') return error("String is not closed");
                if (cbuf == '\'') {
                    cbuf = fgetc(file);
                    if (cbuf != '\'') {
                        buf[i] = '\0';
                        strcpy(string_attr, buf);
                        ungetc(cbuf, file);
                        break;
                    }
                }
                buf[i++] = cbuf;
                if (i >= MAXSTRSIZE - 1) return error("String too long");
            }
            return TSTRING;

        /* unsigned integer */
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            i = 0;
            num_attr = 0;
            buf[i++] = cbuf;
            while (isdigit(cbuf = fgetc(file))) {
                buf[i++] = cbuf;
                num_attr = 10 * num_attr + (cbuf - '0');
                if (num_attr > MAXINT) return error("Number is too large");
            }
            buf[i] = '\0';
            strcpy(string_attr, buf);
            ungetc(cbuf, file);
            return TNUMBER;

        /* keyword or name */
        default:
            if (isalpha(cbuf)) {
                i = 0;
                buf[i++] = cbuf;
                while (isalnum(cbuf = fgetc(file)) || isalpha(cbuf)) {
                    buf[i++] = cbuf;
                }
                buf[i] = '\0';
                strcpy(string_attr, buf);
                ungetc(cbuf, file);

                for (i = 0; i < KEYWORDSIZE; i++) {
                    if (strcmp(buf, key[i].keyword) == 0) {
                        return key[i].keytoken;
                    }
                }
                return TNAME;
            } else {
                return error("Illegal character");
            }
    }

    return 0;
}

int get_linenum() {
    return linenum;
}

void end_scan() {
    fclose(file);
}

void check_lineBreak() {
    if (cbuf == '\n') {
        cbuf = fgetc(file);
        if (cbuf != '\r') ungetc(cbuf, file);
        linenum++;
    } else if (cbuf == '\r') {
        cbuf = fgetc(file);
        if (cbuf != '\n') ungetc(cbuf, file);
        linenum++;
    }
}
