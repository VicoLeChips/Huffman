//
//  main.c
//  Huffman Coding Project
//
//  Created by pierre dewatre on 25/10/2020.
//  Copyright © 2020 Pierre Dewatre. All rights reserved.
//

//
//  Main
//  Huffman Project
//
//  Created by pierre dewatre on 25/10/2020.
//  Copyright © 2020 Pierre Dewatre. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"
#include "time.h"



int main()
{
    clock_t start, end;
    double time;

    int choice = 0;
    int toContinue = 1;
    while (toContinue)
    {
        printf("\nWelcome to this compression / decompression app.\n\nWhat do you want to do ?\n1) Part I of the project\n2) Compression\n3) decompression\n");
        do
        {
            printf("Please enter a numher (between 1 and 3)\n");
            scanf("%d", &choice);
        }while (choice < 1 || choice > 3);

        if (choice == 1)
        {
            FILE* input = fopen("input.txt", "r"); // read only
            FILE* output = fopen("output.txt", "w"); // write only
            if (input == NULL || output == NULL)
            {
                printf("Files could not be open.");
            }
            else
            {
                txt_to_bin(input, output);
                printf("\nthe number of characters in the input file is %d\n\n", number_of_char(input));
                fclose(output);
                fclose(input);
            }
        }

        else if (choice == 2)
        {
            FILE* input = fopen("input.txt", "r"); // read only
            if (input == NULL)
            {
                printf("File could not be open.");
            }
            else
            {
                start = clock();
                compress_text_file(input);
                end = clock();
                time = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("\nIt took %lf sec to compress the %d characters of the file.\n\n", time, number_of_char(input));
                fclose(input);
            }
        }

        else if (choice == 3)
        {
            FILE* compressedText = fopen("compressed.bin", "r");
            if (compressedText == NULL)
            {
                printf("File could not be open.");
            }
            else
            {
                start = clock();
                decompress_text(compressedText);
                end = clock();
                time = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("\nIt took %lf sec to decompress the file.\n\n", time);
                fclose(compressedText);
            }
        }
        printf("Do you want to leave ?\n1) No\n2) Yes\n");
        do
        {
            printf("Please enter a number (between 1 and 2)\n");
            scanf("%d", &choice);
        }while (choice < 1 || choice > 2);
        if (choice == 2)
            toContinue = 0;
    }
    printf("Thanks for using our compressor / decompressor !\n");
    return 0;
}