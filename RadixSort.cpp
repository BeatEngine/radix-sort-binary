#include <vector>
#include <stdio.h>
#include "radixSort.h"
// rsort

template <class T>
void printBin(T var)
{
    size_t bits = sizeof(T)*8;
    char tmp[128] = "";
    int ti = 0;
    for (int i = 0; i < bits; i++)
    {
        if (radixDep::getBitOfBlock(&var, i))
        {
            tmp[ti] = '1';
            ti++;
        }
        else
        {
            tmp[ti] = '0';
            ti++;
        }
    }
    ti--;
    if (ti >= 0)
    {
        while (tmp[ti] == '0')
        {
            if (ti == 0)
            {
                break;
            }
            ti--;
        }
        printf("%.*s", ti+1, tmp);
    }
    else
    {
        printf("0");
    }
}

int main()
{
    std::vector<unsigned short> testlist = { 123, 12, 685, 2892, 777, 89, 4, 389 };


    for (int i = 0; i < testlist.size(); i++)
    {
        if (i > 0)
        {
            printf(", ");
        }
        printf("%d", testlist[i]);
    }
    printf("\nMemory reversed bin order:\n");
    for (int i = 0; i < testlist.size(); i++)
    {
        if (i > 0)
        {
            printf(", ");
        }
        printBin(testlist[i]);
    }
    printf("\n");
    rsort(testlist.data(), testlist.size(), sizeof(*testlist.data()));
    for (int i = 0; i < testlist.size(); i++)
    {
        if (i > 0)
        {
            printf(", ");
        }
        printf("%d", testlist[i]);
    }
    printf("\nMemory reversed bin order:\n");
    for (int i = 0; i < testlist.size(); i++)
    {
        if (i > 0)
        {
            printf(", ");
        }
        printBin(testlist[i]);
    }
    printf("\n");

    printf("\n");

    //Alternative
    testlist = { 128, 64, 32, 16, 8, 4, 2, 1 };
    for (int i = 0; i < testlist.size(); i++)
    {
        if (i > 0)
        {
            printf(", ");
        }
        printf("%d", testlist[i]);
    }
    printf("\nMemory reversed bin order:\n");
    for (int i = 0; i < testlist.size(); i++)
    {
        if (i > 0)
        {
            printf(", ");
        }
        printBin(testlist[i]);
    }
    printf("\n");
    rsort(testlist.data(), testlist.size(), sizeof(*testlist.data()));
    for (int i = 0; i < testlist.size(); i++)
    {
        if (i > 0)
        {
            printf(", ");
        }
        printf("%d", testlist[i]);
    }
    printf("\nMemory reversed bin order:\n");
    for (int i = 0; i < testlist.size(); i++)
    {
        if (i > 0)
        {
            printf(", ");
        }
        printBin(testlist[i]);
    }
    printf("\n");


    return 0;
}

