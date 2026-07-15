#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_COUNT_ENTRIES 1024

typedef struct Word_Count_Pair
{
    char str[ 64 ];
    int  count; // Always 1!

} Word_Count_Pair;

static Word_Count_Pair* g_WordCountStorage;
static int              g_NumWordCountPairs;

typedef struct Bucket
{
    char             str[ 64 ];
    Word_Count_Pair* wordCountPairs;
    int              count;

} Bucket;

static Bucket* g_Buckets;
static int     g_NumBuckets;

typedef struct WordCount
{
    char str[ 64 ];
    int  count;

} WordCount;

static WordCount* g_WordCountArray;
static int        g_LenWordCountArray;

void map(char* text, const char* delim)
{
    printf("### Map Stage ###\n");

    char* word = strtok(text, delim);

    while ( word != NULL )
    {
        printf("(%s,1)\n", word);
        Word_Count_Pair wcp;
        strcpy(wcp.str, word);
        wcp.count                                   = 1;
        g_WordCountStorage[ g_NumWordCountPairs++ ] = wcp;

        word = strtok(NULL, delim);
    }

    printf("### Done. ###\n");
}

void shuffle()
{
    for ( int i = 0; i < g_NumWordCountPairs; i++ )
    {
        Word_Count_Pair wcp = g_WordCountStorage[ i ];
        int             j   = 0;
        for ( ; j < g_NumBuckets; j++ )
        {
            Bucket* bucket = &g_Buckets[ j ];
            if ( !strcmp(wcp.str, bucket->str) )
            {
                bucket->wordCountPairs[ bucket->count ] = wcp;
                bucket->count += 1;
                break;
            }
        }

        // WCP does not have a bucket yet?
        if ( j == g_NumBuckets )
        {
            printf("shuffle debug: Creating new bucket...\n");
            g_Buckets[ g_NumBuckets ].wordCountPairs = malloc(MAX_WORD_COUNT_ENTRIES * sizeof(Word_Count_Pair));
            strcpy(g_Buckets[ g_NumBuckets ].str, wcp.str);
            g_Buckets[ g_NumBuckets ].count               = 1;
            g_Buckets[ g_NumBuckets ].wordCountPairs[ 0 ] = wcp;

            g_NumBuckets++;
        }
    }
}

void reduce()
{
    g_LenWordCountArray = g_NumBuckets;
    for ( int i = 0; i < g_NumBuckets; i++ )
    {
        Bucket*    bucket = &g_Buckets[ i ];
        WordCount* wca    = &g_WordCountArray[ i ];
        strcpy(wca->str, bucket->str);
        wca->count = bucket->count;
    }
}

void InitStorage()
{
    g_WordCountStorage = malloc(MAX_WORD_COUNT_ENTRIES * sizeof(Word_Count_Pair));
    g_Buckets          = malloc(MAX_WORD_COUNT_ENTRIES * sizeof(Bucket));
    g_WordCountArray   = malloc(MAX_WORD_COUNT_ENTRIES * sizeof(WordCount));
}

int main(int argc, char** argv)
{
    InitStorage();

    char str[] = "Hallo Welt Hallo Hadoop Hallo Welt";
    map(str, " ");

    printf("After map:\n\n");
    printf("##########\n");
    for ( int i = 0; i < g_NumWordCountPairs; i++ )
    {
        printf("%s, %d\n", g_WordCountStorage[ i ].str, g_WordCountStorage[ i ].count);
    }
    printf("##########\n\n");

    shuffle();

    printf("After shuffle:\n");
    printf("##############\n");
    for ( int i = 0; i < g_NumBuckets; i++ )
    {
        Bucket* bucket = &g_Buckets[ i ];
        printf("Bucket: %s (%d entries):\n", bucket->str, bucket->count);
        for ( int j = 0; j < bucket->count; j++ )
        {
            Word_Count_Pair* wcp = &bucket->wordCountPairs[ j ];
            printf("  %s, %d\n", wcp->str, wcp->count);
        }
    }
    printf("##############\n\n");

    reduce();

    printf("After reduce:\n");
    for ( int i = 0; i < g_LenWordCountArray; i++ )
    {
        WordCount* wc = &g_WordCountArray[ i ];
        printf("%s -> %d\n", wc->str, wc->count);
    }
    printf("##############\n\n");

    return 0;
}
