#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <ctype.h>
#include "opcodes.h"
#include "common.h"
#include "string.h"


#define INSTR(val) val

char const instructions[] = {
    VM_INSTRUCTIONS
};

#undef INSTR


#define EMIT(instr)                                                     \
    else if (strcmp (opcode_str, #instr) == 0)                          \
        fwrite (&instructions[instr], sizeof (char), 1, foutfile);


char const * DEFAULT_OUTPUT_FILENAME = "a.out";
char const * SHORT_OPTIONS = "o:i:h";
struct option LONG_OPTIONS[] =
{
    {"out-file", required_argument, NULL, 0},
    {"in-file", required_argument, NULL, 0},
    {"help", no_argument, NULL, 0},
    {0, 0, 0, 0}
};
char const * HELP_INFO = "\
Usage: as -i INFILE [OPTION...] \n\
Assemble a tiny-vm file. \n\
\n\
Argumets: \n\
 -o, --out-file        file name for the output file, it's \"a.out\" by default \n\
 -i, --in-file         file name for the input file \n\
 -h, --help            display this information \n\
\n";
int const MAX_OP_LENGTH = 5;


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
        c = getc (fp);

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
    int long_opt_index;
    int opt;
    char const *infile = "";
    char const *outfile = DEFAULT_OUTPUT_FILENAME;
    char opcode_str[MAX_OP_LENGTH];
    char opcode_arg;
    char *end;
    FILE *finfile;
    FILE *foutfile;

    while ((opt = getopt_long (argc, argv, SHORT_OPTIONS, LONG_OPTIONS, &long_opt_index)) != -1)
    {
        switch (opt)
        {
        case 0: // Handel long options
            if (long_opt_index == 0)
                outfile = optarg;
            else if (long_opt_index == 1)
                infile = optarg;
            else if (long_opt_index == 2)
            {
                printf ("\n%s", HELP_INFO);
                exit (0);
            }
            break;

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

    foutfile = fopen (outfile, "w");
    finfile = fopen (infile, "r");

    if (!foutfile)
    {
        fprintf (stderr, "Cannot output to: %s\n", outfile);
        exit (-1);
    }

    if (!finfile)
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
        else if (strcmp (opcode_str, "PSH") == 0)
        {
            fwrite (&instructions[PSH], sizeof (char), 1, foutfile);
            get_next (finfile, opcode_str);
            opcode_arg = strtol (opcode_str, &end, 10);
            fwrite (&opcode_arg, sizeof (char), 1, foutfile);
        }
        else if (strcmp (opcode_str, "JMP") == 0)
        {
            fwrite (&instructions[JMP], sizeof (char), 1, foutfile);
            get_next (finfile, opcode_str);
            opcode_arg = strtoul (opcode_str, &end, 10);
            fwrite (&opcode_arg, sizeof (char), 1, foutfile);
        }
        else if (strcmp (opcode_str, "JIF") == 0)
        {
            fwrite (&instructions[JIF], sizeof (char), 1, foutfile);
            get_next (finfile, opcode_str);
            opcode_arg = strtoul (opcode_str, &end, 10);
            fwrite (&opcode_arg, sizeof (char), 1, foutfile);
        }
        else
        {
            exit (-1);
        }
    }

    fclose (finfile);
    fclose (foutfile);
    printf ("Wrote program: %s to %s\n", infile, outfile);
}
