/*
Student Name: Dennis Nsi
Student Number: 041149827
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Constants for program configuration */
#define MAX_NUMBERS 2       /* Maximum allowed numerical arguments (AMOUNT + 2 EXCHANGE rates) */
#define CURRENCY_LEN 4      /* Length for currency symbol storage (3 chars + null terminator) */

/* Display help information about program usage */
void displayHelp() {
    printf("Usage: moneychange [OPTION]... AMOUNT EXCHANGE [EXCHANGE]\n");
    printf("Convert from one currency to another (by default convert the AMOUNT into USD).\n");
    printf("Options:\n");
    printf("    -l, --long             use long format output\n");
    printf("    -s, --symbol SYMBOL    add three letter currency code to output\n\n");
    printf("Exit status:\n");
    printf("    0       if OK,\n");
    printf("    1       if problem encountered.\n");
    printf("moneychange by Dennis Nsi.\n");
}

/* Validate currency symbol format (must be 3 uppercase letters) */
int validate_currency(const char *symbol) {
    int i;
    /* Currency code must be exactly 3 characters */
    if (strlen(symbol) != 3) return 1;

    /* Each character must be uppercase alphabetic */
    for (i = 0; i < 3; i++) {
        if (!isupper((unsigned)symbol[i])) return 1;
    }
    return 0;
}

/* Validate if string represents a valid positive number */
int is_valid_number(char *str) {
    int has_decimal = 0; /* Flag for decimal point */
    int has_digit = 0;   /* Flag for at least one digit */

    /* Check for empty string */
    if (str == NULL || *str == '\0') {
        return 0;
    }
    /* Reject negative numbers */
    if(*str == '-') return 0;

    /* Process each character in string */
    while(*str) {
        if(isdigit((unsigned)*str)) {
            has_digit = 1;
        } else if(*str == '.') {
            /* Reject multiple decimal points */
            if(has_decimal == 1) {
                return 0;
            }
            has_decimal = 1;
        } else {
            /* Reject any non-digit, non-decimal characters */
            return 0;
        }
        str++;
    }
    /* Must have at least one digit to be valid */
    return has_digit;
}

int main(int argc, char *argv[]) {
    /* Program configuration variables */
    int long_format = 0;                /* Flag for long output format */
    char currency_symbol[CURRENCY_LEN] = "USD";  /* Default currency symbol */
    double numbers[3] = {0};            /* Storage for numerical arguments */
    int num_count = 0;                  /* Count of valid numbers received */
    double result;                      /* Calculation result */
    int i;                              /* Declaration for first for-loop */
    int j;                              /* Declaration for second for-loop */

    /* Check for minimum arguments */
    if(argc < 2) {
        printf("Try 'moneychange --help' for more information.\n");
        return 1;
    }

    /* Process command line arguments */
    for (i = 1; i < argc; i++) {
        /* Help command */
        if (strcmp(argv[1], "--help") == 0) {
            displayHelp();
            return 0;
        }
        /* Long format option */
        else if (strcmp(argv[i], "--long") == 0 || strcmp(argv[i], "-l") == 0) {
            long_format = 1;
        }
        /* Currency symbol option */
        else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--symbol") == 0) {
            /* Validate currency symbol format */
            if (validate_currency(argv[i+1]) == 1) {
                printf("moneychange: invalid currency symbol\n"); 
                printf("Try 'moneychange --help' for more information.\n");
                return 1;
            }
            /* Store valid currency symbol */
            for(j = 0; j < CURRENCY_LEN - 1; j++) {
                currency_symbol[j] = argv[i+1][j];
            }
            currency_symbol[3] = '\0';
            i++;  /* Skip next argument (the symbol itself) */
        }
        /* Invalid option */
        else if(argv[i][0] == '-') {
            printf("Try 'moneychange --help' for more information.\n"); 
            return 1;
        }
        /* Numerical argument processing */
        else {
            /* Validate number format */
            if(is_valid_number(argv[i]) == 0) {
                printf("Invalid number format: %s\n", argv[i]);
                printf("Try 'moneychange --help' for more information.\n");
                return 1;
            }
            
            /* Check maximum number limit */
            if (num_count > MAX_NUMBERS) {
                printf("Error: Too many numerical arguments\n");
                printf("Try 'moneychange --help' for more information.\n");
                return 1;
            }
            
            /* Store valid number */
            numbers[num_count++] = atof(argv[i]);
        }    
    }

    /* Validate calculation inputs */
    if (numbers[1] == 0.00) {
        printf("Error: can't multiply by zero\n");
        return 1;
    }

    /* Perform currency conversion calculation */
    result = numbers[0] * numbers[1];

    /* Handle optional second exchange rate (division) */
    if((num_count == 3)) {
        if (numbers[2] == 0) {
            printf("Error: can't divide by zero\n");
            return 1;
        }
        result /= numbers[2];
    }

    /* Display results in requested format */
    if (long_format == 1) {
        printf("%.2f becomes %.2f%s", numbers[0], result, currency_symbol);
    } else {
        printf("%.2f%s", result, currency_symbol);
    }

    return 0;
}