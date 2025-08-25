#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "opcodes.h"
#include "common.h"

#ifdef _WIN32
#include "wingetopt.h"
#else
#include <getopt.h>
#endif

#define INSTR(val) val

char const instructions[] = {
    VM_INSTRUCTIONS
};

#undef INSTR


#define EMIT(instr)                                                     \
    else if (strcmp (opcode_str, #instr) == 0)                          \
        fwrite (&instructions[instr], sizeof (char), 1, foutfile);


char const * DEFAULT_OUTPUT_FILENAME = "a.out";
char * SHORT_OPTIONS = "o:i:h";
char const * HELP_INFO = "\
Usage: as -i INFILE [OPTION...] \n\
Assemble a tiny-vm file. \n\
\n\
Argumets: \n\
 -o        file name for the output file, it's \"a.out\" by default \n\
 -i        file name for the input file \n\
 -h        display this information \n\
\n";
#define MAX_OP_LENGTH 5


int
get_next (FILE *fp, char *opcode_buff)
{
    char c;
    int i;
    static int at_eof = 0;

    if (at_eof)
        return 0;

    for (i = 0; i < MAX_OP_LENGTH - 1; i++)
    {
        c = (char) getc (fp);

        if (c == EOF)
        {
            at_eof = 1;
            break;
        }

        if (isprint (c) && !isspace (c))
            opcode_buff[i] = c;
        else
            break;
    }

    opcode_buff[i] = '\0';
    return 1;
}


int
main (int argc, char *argv[])
{
    int opt;
    char const *infile = "";
    char const *outfile = DEFAULT_OUTPUT_FILENAME;
    char opcode_str[MAX_OP_LENGTH];
    char opcode_arg;
    char *end;
    FILE *finfile;
    FILE *foutfile;

    while ((opt = getopt (argc, argv, SHORT_OPTIONS)) != -1)
    {
        switch (opt)
        {
        case 'o':
            outfile = optarg;
            break;

        case 'i':
            infile = optarg;
            break;

        case 'h':
            printf ("%s", HELP_INFO);
            exit (0);
            break;

        default:
            exit (-1);
            break;
        }
    }

    if (strcmp (infile, "") == 0)
    {
        fprintf (stderr, "Input file required\n\n%s", HELP_INFO);
        exit (-1);
    }

    
    if (!fopen_s (&foutfile, outfile, "w"))
    {
        fprintf (stderr, "Cannot output to: %s\n", outfile);
        exit (-1);
    }

    if (!fopen_s (&finfile, infile, "r"))
    {
        fprintf (stderr, "Cannot read: %s\n", infile);
        fclose (foutfile);
        exit (-1);
    }

    // Emit the bytecode
    while (get_next (finfile, opcode_str))
    {
        if (strcmp (opcode_str, "HLT") == 0)
            fwrite (&instructions[HLT], sizeof (char), 1, foutfile);
        EMIT (POP)
        EMIT (DUP)
        EMIT (ADD)
        EMIT (SUB)
        EMIT (MUL)
        EMIT (DIV)
        EMIT (XOR)
        EMIT (AND)
        EMIT (NOT)
        EMIT (OR)
        EMIT (ISEQ)
        EMIT (ISGT)
        EMIT (NOP)
        EMIT (PSH)
        EMIT (JMP)
        EMIT (JIF)
        else
        {
            exit (-1);
        }

        if (strcmp (opcode_str, "PSH") == 0
            || strcmp (opcode_str, "JMP") == 0
            || strcmp (opcode_str, "JIF") == 0)
        {
            get_next (finfile, opcode_str);
            opcode_arg = (char) strtol (opcode_str, &end, 10);
            fwrite (&opcode_arg, sizeof (char), 1, foutfile);
        }
    }

    fclose (finfile);
    fclose (foutfile);
    printf ("Wrote program: %s to %s\n", infile, outfile);
}
