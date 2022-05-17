#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXN 1000000

typedef struct stack{
    double num[MAXN];
    char op[MAXN];
    int num_len;
    int op_len;
} Stack;

Stack eqn;
char input[MAXN];

double popNum(){
    // printf("===popNum===\n");
    double num = eqn.num[eqn.num_len - 1];
    eqn.num_len--;
    return num;
}

char popOp(){
    // printf("===popOp===\n");
    char op = eqn.op[eqn.op_len - 1];
    eqn.op_len--;
    return op;
}

void pushNum(double data){
    // printf("===pushNum===\n");
    eqn.num[eqn.num_len] = data;
    eqn.num_len++;
}

void pushOp(char data){
    // printf("===pushOp===\n");
    eqn.op[eqn.op_len] = data;
    eqn.op_len++;
}

double calculate(double a, double b, char op){
    switch(op){
        case '+':
            return a + b;
            break;
        case '-':
            return a - b;
            break;
        case '*':
            return a * b;
            break;
        case '/':
            return a / b;
            break;
    }
}

int main(int argc, char *argv[]){
    double ans = 0;
    while(scanf("%s", input) != EOF){
        int input_len = strlen(input);
        int input_indx = 0;
        eqn.num_len = 0;
        eqn.op_len = 0;
        while(input_indx < input_len){
            // printf("===input_indx: %d, input_len: %d===\n", input_indx, input_len);
            char curr = input[input_indx];
            double num = 0;
            // printf("===curr: %c===\n", curr);
            switch(curr){
                case '+':
                    // printf("===+===\n");
                    while(eqn.num_len > 1 && eqn.op[eqn.op_len - 1] != '(' && eqn.op_len > 0){
                        double b = popNum(), a = popNum();
                        char op = popOp();
                        pushNum(calculate(a, b, op));
                    }
                    pushOp(curr);
                    input_indx++;
                    break;
                case '-':
                    // printf("===-===\n");
                    while(eqn.num_len > 1 && eqn.op[eqn.op_len - 1] != '(' && eqn.op_len > 0){
                        double b = popNum(), a = popNum();
                        char op = popOp();
                        pushNum(calculate(a, b, op));
                    }
                    pushOp(curr);
                    input_indx++;
                    break;
                case '*':
                    // printf("===*===\n");
                    while(eqn.num_len > 1 && eqn.op[eqn.op_len - 1] != '(' && eqn.op[eqn.op_len - 1] != '+' && eqn.op[eqn.op_len - 1] != '-' && eqn.op_len > 0){
                        double b = popNum(), a = popNum();
                        char op = popOp();
                        pushNum(calculate(a, b, op));
                    }
                    pushOp(curr);
                    input_indx++;
                    break;
                case '/':
                    // printf("===/===\n");
                    while(eqn.num_len > 1 && eqn.op[eqn.op_len - 1] != '(' && eqn.op[eqn.op_len - 1] != '+' && eqn.op[eqn.op_len - 1] != '-' && eqn.op_len > 0){
                        double b = popNum(), a = popNum();
                        char op = popOp();
                        pushNum(calculate(a, b, op));
                    }
                    pushOp(curr);
                    input_indx++;
                    break;
                case '(':
                    // printf("===(===\n");
                    pushOp(curr);
                    input_indx++;
                    break;
                case ')':
                    // printf("===)===\n");
                    while(eqn.num_len > 1 && eqn.op[eqn.op_len - 1] != '(' && eqn.op_len > 0){
                        double b = popNum(), a = popNum();
                        char op = popOp();
                        pushNum(calculate(a, b, op));
                    }
                    if(eqn.op_len > 0 && eqn.op[eqn.op_len - 1] == '('){
                        char trash = popOp();
                    }
                    input_indx++;
                    break;
                default:
                    // printf("===def===\n");
                    while(isdigit(curr)){
                        num = num * 10 + (double)(curr) - (double)('0');
                        input_indx++;
                        curr = input[input_indx];
                    }
                    pushNum(num);
                    break;
            }
        }
        while(eqn.num_len > 1){
            double b = popNum(), a = popNum();
            char op = popOp();
            pushNum(calculate(a, b, op));
        }
        ans = eqn.num[0];
        printf("%.15lf\n", ans);
        // printf("%c\n", eqn.op[eqn.op_len - 1]);
    }
    
    return 0;
}
