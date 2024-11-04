#include <stdio.h>
#include <stdlib.h>

struct ResMatrix {
    int **alloc;      // Allocated resources
    int **max;        // Maximum resources
    int **need;       // Needed resources
    int *avail;       // Available resources
};

int **allocMatrix(int rows, int cols) {
    int **mat = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        mat[i] = (int *)malloc(cols * sizeof(int));
    }
    return mat;
}

void freeMatrix(int **mat, int rows) {
    for (int i = 0; i < rows; i++) {
        free(mat[i]);
    }
    free(mat);
}

void readMatrix(int rows, int cols, int **mat) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &mat[i][j]);
        }
    }
}

void inputRes(int *pCount, int *rCount, struct ResMatrix* rData) {
    printf("Enter the number of processes and resources: ");
    scanf("%d %d", pCount, rCount);
    
    rData->alloc = allocMatrix(*pCount, *rCount);
    rData->max = allocMatrix(*pCount, *rCount);
    rData->need = allocMatrix(*pCount, *rCount);
    rData->avail = (int *)malloc(*rCount * sizeof(int));
    
    printf("Enter the Allocation matrix:\n");
    readMatrix(*pCount, *rCount, rData->alloc);
    
    printf("Enter the Max matrix:\n");
    readMatrix(*pCount, *rCount, rData->max);
    
    printf("Enter the Available matrix:\n");
    for (int j = 0; j < *rCount; j++) {
        scanf("%d", &rData->avail[j]);
    }
}

void displayMatrix(int pCount, int rCount, struct ResMatrix* rData) {
    printf("---------------------------------------\n");
    printf("| Px | Allocated |   Max   |   Need   |\n");
    printf("---------------------------------------\n");
    for (int i = 0; i < pCount; i++) {
        printf("| P%d |  ", i);
        for (int j = 0; j < rCount; j++) {
            printf("%d ", rData->alloc[i][j]);
        }
        printf(" | ");
        for (int j = 0; j < rCount; j++) {
            printf("%d ", rData->max[i][j]);
        }
        printf("| ");
        for (int j = 0; j < rCount; j++) {
            printf("%d ", rData->need[i][j]);
        }
        printf("| \n");
    }
    printf("---------------------------------------\n");
    printf("Available: ");
    for (int j = 0; j < rCount; j++) {
        printf("%d ", rData->avail[j]);
    }
    printf("\n");
}

void calcNeed(int pCount, int rCount, struct ResMatrix* rData) {
    for (int i = 0; i < pCount; i++) {
        for (int j = 0; j < rCount; j++) {
            rData->need[i][j] = rData->max[i][j] - rData->alloc[i][j];
        }
    }
}

int isSafe(int pIdx, int rCount, int *avail, int **need, int **alloc) {
    for (int j = 0; j < rCount; j++) {
        if (avail[j] < need[pIdx][j]) {
            return 0;
        }
    }
    for (int j = 0; j < rCount; j++) {
        avail[j] += alloc[pIdx][j];
    }
    return 1;
}

void findSafeSeq(int pCount, int rCount, struct ResMatrix* rData, int* safeSeq, int* seqLen) {
    int *flags = (int *)calloc(pCount, sizeof(int));
    int completed = 0;

    while (completed < pCount) {
        int found = 0;
        for (int i = 0; i < pCount; i++) {
            if (flags[i] == 0 && isSafe(i, rCount, rData->avail, rData->need, rData->alloc)) {
                flags[i] = 1;
                safeSeq[(*seqLen)++] = i;
                completed++;
                found = 1;
            }
        }
        if (!found) {
            break;  // No safe sequence exists
        }
    }
    free(flags);
}

void displaySafeSeq(int pCount, int* safeSeq, int seqLen) {
    if (seqLen == pCount) {
        printf("\nSafe sequence: ");
        for (int i = 0; i < seqLen; i++) {
            printf("P%d ", safeSeq[i]);
        }
        printf("\nSystem is in a safe state.\n");
    } else {
        printf("\nSystem is not in a safe state.\n");
    }
}

void freeRes(int pCount, struct ResMatrix* rData) {
    freeMatrix(rData->alloc, pCount);
    freeMatrix(rData->max, pCount);
    freeMatrix(rData->need, pCount);
    free(rData->avail);
}

int main() {
    struct ResMatrix rData;
    int pCount = 0, rCount = 0;
    
    inputRes(&pCount, &rCount, &rData);
    calcNeed(pCount, rCount, &rData);
    
    int *safeSeq = (int *)malloc(pCount * sizeof(int));
    int seqLen = 0;
    
    findSafeSeq(pCount, rCount, &rData, safeSeq, &seqLen);
    displayMatrix(pCount, rCount, &rData);
    displaySafeSeq(pCount, safeSeq, seqLen);
    
    free(safeSeq);
    freeRes(pCount, &rData);

    return 0;
}
