/*
*              File Binder v1.0
*              By Joshua Taylor
*
*    Basically this take a bunch of files and puts
*    them in one file, then it spits out a header
*    file with defines to an index number.
*
*
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <libgen.h>
#include <errno.h>

#define byte unsigned char
#define MAX_FILE_NAME 16


void print_title()
{
    printf("                                   File Binder\n");
    printf("                                 By Joshua Taylor\n");
    printf("===============================================================================\n\n");
}

int get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

// Go back and add error checking 
int pack_to_file(int argc, char *argv[])
{
    char fileout_name[MAX_FILE_NAME] = "DEFAULT.DAT";
    char header_name[MAX_FILE_NAME] = "DEFAULT.H";
    FILE *fp;
    FILE *fpout;
    FILE *header;
    char i_mark[] = "idx";

    byte ch; //for byte transfer
    
    //Loop through all the files and put them in a file
    printf("\n\nEnter resulting filename (ex. mydata.dat): ");
    if ((gets(fileout_name)) == NULL)
    {
        printf("fgets failure on fileout_name!");
        return 1;
    }
    printf("Enter header filename (ex. mydata.h): ");
    if ((gets(header_name) == NULL))
    {
        printf("fgets failure on header_name!");
        return 1;
    }

    printf("\nDatafile: %s\nHeader file: %s\n", fileout_name, header_name);

    if ((fpout = fopen(fileout_name, "wb")) == NULL)
    {
        printf("Errno: %d, %s\n", errno, strerror(errno));
        printf("Error with creating file out!\n");
        return 1;
    } 
    
    if ((header = fopen(header_name, "w")) == NULL)
    {
        printf("Errno: %d, %s\n", errno, strerror(errno));
        printf("Error with creating header file!\n");
        return 1;
    }

    //Go through the files one by one
    for (int i = 1; i < argc; i++)
    {
        int file_size = 0;
        byte *buff;

        fp = fopen(argv[i], "rb");
        rewind(fp);
        fwrite(i_mark, 1, sizeof(i_mark), fpout); //Write index marker
        fprintf(fpout, "%d", i-1);                //Write index number (Might change this to fwrite)
        fprintf(header, "#define %s        %d\n", basename((argv[i])), i-1); //add the basename to the header and assigne the index

        //Find out the filesize in bytes
        fseek(fp, 0, SEEK_END);
        file_size = ftell(fp);
        rewind(fp);

        buff = malloc(file_size * sizeof(byte)); //Create a buffer to dump file contents into
        fread(buff, 1, file_size, fp);

        for (int b = 0; b < file_size; b++)
        {
            fwrite(buff+b, sizeof(byte), 1, fpout); //Write it to the dat one byte at a time
        }
   
        printf("\n%s packed to %s", argv[i], fileout_name);
        free(buff);
        fclose(fp);
    }

    fclose(fpout);
    fclose(header);

    return 0;
}

int main(int argc, char *argv[])
{
    int file_count = argc-1;
    
    print_title();

    if (argc > 2)
    {
        printf("%d files to bind together\n\n", file_count);
        printf("%-5s   %-47s %5s\n", "INDEX", "FILE", "BYTES");
        printf("--------------------------------------------------------------------------------\n");

        //Loop through all the files then ask to continue
        for (int i = 1; i < argc; i++)
        {
            FILE *fp;
            fp = fopen(argv[i], "r");
            printf("%05d   %-48.45s %-5d\n", i-1, argv[i], get_file_size(fp));
            fclose(fp);
        }

        printf("\nWould you like to proceed? [Y/N]");
        char c = _getch();
        c = toupper(c);
        if (c == 'Y') //If yes do this shit!
        {
            if (pack_to_file(argc, argv) == 0)
            {
                printf("\nSuccess!");
            }
        }
    }
    else
    {
        printf("Please provide 2 or more files to bind!");
    }
    
    printf("\n\nPress any key to close...");
    _getch();
    return 0;
}