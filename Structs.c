#include <string.h>
#include <stdlib.h>
#include "Structs.h"
#include <math.h>

#define FAIL 0
#define SUCCESS 1
#define POWER 2

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    Vector *vectorA = (Vector *) a;
    Vector *vectorB = (Vector *) b;
    int shortestVectorLen;
    if (vectorA->len < vectorB->len)
    {
        shortestVectorLen = vectorA->len;
    }
    else
    {
        shortestVectorLen = vectorB->len;
    }

    //check if the values in the vectors are similar for the length of the shortest vector
    for (
            int i = 0; i < shortestVectorLen; i++)
    {
        if (vectorA->vector[i] == vectorB->vector[i])
        {
            continue;
        }
        else
        {
            if (vectorA->vector[i] > vectorB->vector[i])
            {
                return 1;
            }
            else
            {
                return -1;
            }
        }
    }

    //if the value in the first length values in the vectors are the same the function determent
    // according to the vectors length
    if (vectorA->len == vectorB->len)
    {
        return 0;
    }
    else
    {
        if (shortestVectorLen == vectorA->len)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
}


/**
 * This function calculates a vectors norma
 * @param v- the vector to calculate its norma
 * @return the vectors norma
 */
double findNorm(Vector *v)
{
    double sum = 0;
    for (
            int i = 0; i < v->len; i++)
    {
        sum += pow(v->vector[i], POWER);
    }
    return sqrt(sum);
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if (pVector == NULL)
    {
        return FAIL;
    }
    Vector *pVectorS = (Vector *) pVector;
    if (pMaxVector == NULL)
    {
        return FAIL;
    }
    Vector *pMaxVectorS = (Vector *) pMaxVector;

    //if the vector is null
    if (pMaxVectorS->vector == NULL)
    {
        pMaxVectorS->vector = (double *) malloc(pVectorS->len * sizeof(double));
        if (pMaxVectorS->vector == NULL)
        {
            return FAIL;
        }
        pMaxVectorS->len = pVectorS->len;
        for (
                int i = 0; i < pVectorS->len; i++)
        {
            pMaxVectorS->vector[i] = pVectorS->vector[i];
        }
        return SUCCESS;
    }
    if (findNorm(pVectorS) > findNorm(pMaxVectorS))
    {
        pMaxVectorS->vector = (double *) realloc(pMaxVectorS->vector,
                                                 pVectorS->len * sizeof(double));
        if (pMaxVectorS->vector == NULL)
        {
            return FAIL;
        }
        pMaxVectorS->len = pVectorS->len;
        for (
                int i = 0; i < pVectorS->len; i++)
        {
            pMaxVectorS->vector[i] = pVectorS->vector[i];
        }

    }
    return SUCCESS;
}

/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    Vector *biggestNorma = (Vector *) malloc(sizeof(Vector));
    if (biggestNorma == NULL)
    {
        return NULL;
    }
    biggestNorma->len = 0;
    biggestNorma->vector = NULL;
    if (forEachRBTree(tree, copyIfNormIsLarger, biggestNorma) == FAIL)
    {
        return FAIL;
    }
    return biggestNorma;
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    if (pVector == NULL)
    {
        return;
    }
    Vector *pVectorS = (Vector *) pVector;
    if (pVectorS->vector != NULL)
    {
        free(pVectorS->vector);
        pVectorS->vector = NULL;
    }

    free(pVectorS);
    pVectorS = NULL;

}

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    char *a_s = (char *) a;
    char *b_s = (char *) b;
    return (strcmp(a_s, b_s));
}

/**
 * ForEach function that concatenates the given word to pConcatenated. pConcatenated is already allocated with
 * enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    char *wordS = (char *) word;
    char *pConcatenatedS = (char *) pConcatenated;
    if (strcat(pConcatenatedS, wordS) == NULL)
    {
        return FAIL;
    }
    if (strcat(pConcatenatedS, "\n") == NULL)
    {
        return FAIL;
    }
    return SUCCESS;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    char *s_as_string = (char *) s;
    if (s_as_string != NULL)
    {
        free(s_as_string);
        s_as_string = NULL;
    }
}
