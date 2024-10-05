#include <stdio.h>
#define row 10
#define column 5

struct matrix{
    int al[row][column];
    int m[row][column];
    int av[1][column];
    int n[row][column];
};

int safeSequence[row];
int s=0;
int k=0;
int l=0;

void matRead(int r, int c, int m[row][column]){
    printf("\n");
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            scanf("%d", &m[i][j]);
        }
    }
}

void readInput(int *r, int *c, struct matrix* b){
    printf("Enter the no.of processes and no.of resources: ");
    scanf("%d %d", r, c);
    printf("Enter the Allocated matrix:");
    matRead(*r, *c, b->al);
    //printf("Enter the Max matrix:");
    matRead(*r, *c, b->m);
    //printf("Enter the available matrix:");
    matRead(1, *c, b->av);
}

void matPrint(int r, int c, struct matrix* b){
    printf("---------------------------------------\n");
    printf("| Px | Allocated |   Max   |   Need   |\n");
    printf("---------------------------------------\n");
    for(int i=0;i<r;i++){
        printf("| P%d |  ", i);
        for(int j=0;j<c;j++){
            printf("%d ", b->al[i][j]);
        }
        printf(" | ");
        for(int j=0;j<c;j++){
            printf("%d ", b->m[i][j]);
        }
        printf("| ");
        for(int j=0;j<c;j++){
            printf("%d ", b->n[i][j]);
        }
        printf("| \n");
    }
    printf("---------------------------------------\n");
    printf("Available: ");
    for(int j=0;j<c;j++){
        printf("%d ", b->av[0][j]);
    }
}

void calculateNeed(int r, int c, struct matrix* b){
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++) {
            b->n[i][j]=b->m[i][j]-b->al[i][j];
        }
    }
}

int safeCheck(int r, int c, int av[row][column], int n[row][column], int al[row][column]){
    int f=0;
    for(int j=0;j<c;j++){
        if(av[0][j]<n[r][j]){
            f++;
        }
    }

    if(f!=0){
        k=1;
        return 0;
    }
    else{
        safeSequence[s]=r;
        s++;
        l=1;
        for(int j=0;j<c;j++){
            av[0][j]+=al[r][j];
        }
        return 1;
    }
}

void checkSequence(int sr, int c, int sF[], struct matrix* b){
    for(int i=0;i<sr;i++){
        if(sF[i]==0) {
            int r = i;
            sF[i]=safeCheck(r, c, b->av, b->n, b->al);
        }
    }
    if(k!=0 && l!=0){
        k=0;
        l=0;
        checkSequence(sr, c, sF, b);
    }
}

void printSafe(int r){
    printf("\nSafe sequence: ");
    for(int i=0;i<s;i++){
        printf("%d ", safeSequence[i]);
    }
    if(s==r){

        printf("\nSystem is in safe state.");
    }
    else{
        printf("\nSystem is not in safe state.");
    }
}

int main(){
    struct matrix b0;
    int r=0,c=0;
    readInput(&r, &c, &b0);
    int safeFlags[r];
    for(int i=0;i<r;i++){
        safeFlags[i]=0;
    }
    calculateNeed(r, c, &b0);
    checkSequence(r, c, safeFlags, &b0);
    matPrint(r, c, &b0);
    printSafe(r);
}
