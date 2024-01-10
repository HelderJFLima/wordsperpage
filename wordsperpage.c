/** by HelderJFLima **/

/*
*
* This program calculates an estimate of the average number of words per page
* of a book and its approximate total number of words. For this purpose, mean
* and standard deviation calculations are used and statistical inferences are
* made using the Student distribution. The error propagation calculation in the
* process is also used. The reliability of the results is 80%.
*
* For more information, look for the README file.
*
*/


#include <errno.h>		/* For error messages */
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE	  10	/* Base used in strtol function (stdlib.h) */
#define DOUBLE    1		/* Indentifies type double */
#define FILENAME1 "wpp_input.txt"
#define FILENAME2 "wpp_output.txt"
#define FINAL     1		/* For array with page numbers */
#define INITIAL   0		/* Ditto */
#define INT       0		/* Indentifies type int */
#define LINESIZE  70	/* Maximum line size */
#define LINNUM    10	/* For array with line numbers */
#define NUMELEM1  2		/* Number of elements on different lines of the file */
#define NUMELEM2  10
#define NUMELEM3  4
#define NUMLINES  10	/* Number of lines of word counts */
#define PGNUM     2		/* For array with page numbers */
#define STDNTLIN  0.46	/* Coefficient for population inference */
#define STDNTWRD  0.21	/* Ditto */
#define WRDSNUM   40	/* For array with word numbers */


int getdata(int *pages, int *nlines, double *nwords);

double mean(void *numbers, int quantity, int type);

double stddev(void *numbers, double mean, int quantity, int type);

int writefile(double wordsppg, double err1, double wordsnum, double err2);


/* Calculate an estimate of the average number of words per page of a book and
** its approximate total number of words.
*/

int main()
{
	int pgnumbers[PGNUM];		/* Starting and ending page numbers considered */
	int linesnum[LINNUM];		/* Line numbers */
	double wordsnum[WRDSNUM];	/* Word numbers */
	double linemean;			/* Mean of lines per page */
	double linestdv;			/* Standard deviation of linemean */
	double wordmean;			/* Mean of words per line */
	double wordstdv;			/* Standard deviation of wordmean */
	double wordsppg;			/* Mean of words per page */
	double wpperror;			/* Error associated with wordsppg */
	double totalwords;			/* Estimate of total words */
	double twerror;				/* Error associated with totalwords */

	if( !getdata(pgnumbers, linesnum, wordsnum) )		/* Get data from file */
	{
		printf("\nmain->getdata: error reading file\n");

		return 1;
	}

	linemean = mean( (void*)linesnum, LINNUM, INT );

	linestdv = stddev( (void*)linesnum, linemean, LINNUM, INT );

	wordmean = mean( (void*)wordsnum, WRDSNUM, DOUBLE );

	wordstdv = stddev( (void*)wordsnum, wordmean, WRDSNUM, DOUBLE );

	linestdv *= STDNTLIN;					/* Make population inference */

	wordstdv *= STDNTWRD;

	wordsppg = wordmean * linemean;			/* Words per page */

	wpperror = wordsppg * sqrt( pow(linestdv / linemean, 2.0) +
								pow(wordstdv / wordmean, 2.0)   );

																/* Total words */
	totalwords = wordsppg * ( pgnumbers[FINAL] - pgnumbers[INITIAL] + 1 );

	twerror = totalwords * sqrt( pow(wpperror / wordsppg, 2.0) );

	if( !writefile(wordsppg, wpperror, totalwords, twerror) )	/* Save results */
	{
		printf("\nmain->writefile: error when saving results\n");

		return 2;
	}
	else
	{
		printf("\nResults saved in the same folder as the input, ");

		printf("in file %s\n", FILENAME2);
	}

	return 0;
}


/* Read a file and copy the values into three arrays of numbers; return 0 if
** there is a problem in the process and 1 otherwise.
*/

int getdata(int *pages, int *nlines, double *nwords)
{
	char line[LINESIZE];
	int i;					/* Auxiliar counter */
	int lnum = 1;			/* Line number */
	FILE *file;

	int getdouble(double*, int, char*);
	int getint(int*, int, char*);

	file = fopen(FILENAME1, "r");

	if(!file)
	{
		perror("\ngetdata->fopen");

		return 0;
	}

	if( !fgets(line, LINESIZE, file) )			/* Read 1st line */
	{
		perror("\ngetdata->->fgets");

		printf("\ngetdata: error reading line %d\n", lnum);

		fclose(file);

		return 0;
	}

	if( !getint(pages, NUMELEM1, line) )		/* Get numbers */
	{
		printf("\ngetdata->getint: format error on line %d\n", lnum);

		fclose(file);

		return 0;
	}

	lnum++;

	if( !fgets(line, LINESIZE, file) )			/* Read 2nd line */
	{
		perror("\ngetdata->->fgets");

		printf("\ngetdata: error reading line %d\n", lnum);

		fclose(file);

		return 0;
	}

	if( !getint(nlines, NUMELEM2, line) )
	{
		printf("\ngetdata->getint: format error on line %d\n", lnum);

		fclose(file);

		return 0;
	}

	lnum++;

	for(i = 0; i < NUMLINES; i++)
	{
		if( !fgets(line, LINESIZE, file) )		/* Read n-th line */
		{
			perror("\ngetdata->->fgets");

			printf("\ngetdata: error reading line %d\n", lnum);

			fclose(file);

			return 0;
		}

		if( !getdouble(nwords, NUMELEM3, line) )
		{
			printf("\ngetdata->getdouble: format error on line %d\n", lnum);

			fclose(file);

			return 0;
		}

		nwords += NUMELEM3;						/* Update position in array */

		lnum++;
	}

	fclose(file);

	return 1;
}


/* Read a string and convert its tokens to floating point numbers, saving them
** to an array; return 0 if there is an error and 1 otherwise.
*/

int getdouble(double *numbers, int quantity, char *string)
{
	char *num;			/* Auxiliar pointer */
	int counter = 0;	/* Count read numbers */
	double number;

	num = strtok(string, ",\n");				/* Read 1st number in string */

	if(num != NULL)
	{
		number = strtod(num, NULL);				/* Convert to double */

		if(number <= 0.0 || number == HUGE_VAL)	/* If out of range */
		{
			printf("\ngetdouble->strtod: number format/value error\n");

			return 0;
		}

		*numbers++ = number;					/* Save number */

		counter++;
	}
	else
	{
		printf("\ngetdouble->strtok: file format error\n");

		return 0;
	}

	while(num != NULL)							/* Read the remaining numbers */
	{
		num = strtok(NULL, ",\n");

		if(num != NULL)
		{
			number = strtod(num, NULL);

			if(number <= 0.0 || number == HUGE_VAL)
			{
				printf("\ngetdouble->strtod: number format/value error\n");

				return 0;
			}

			*numbers++ = number;

			counter++;
		}
	}

	if(counter != quantity)						/* Check quantity */
	{
		printf("\ngetdouble->strtok: file format error\n");

		return 0;
	}

	return 1;
}


/* Read a string and convert its tokens to integers, saving them to an array;
** return 0 if there is an error and 1 otherwise.
*/

int getint(int *numbers, int quantity, char *string)
{
	char *num;			/* Auxiliar pointer */
	int counter = 0;	/* Count read numbers */
	long number;

	num = strtok(string, ",\n");				/* Read 1st number in string */

	if(num != NULL)
	{
		number = strtol(num, NULL, BASE);		/* Convert to long */

		if(number <= 0 || number > INT_MAX)		/* If out of range */
		{
			printf("\ngetint->strtol: number format/value error\n");

			return 0;
		}

		*numbers++ = (int)number;				/* Save number */

		counter++;
	}
	else
	{
		printf("\ngetint->strtok: file format error\n");

		return 0;
	}

	while(num != NULL)							/* Read the remaining numbers */
	{
		num = strtok(NULL, ",\n");

		if(num != NULL)
		{
			number = strtol(num, NULL, BASE);

			if(number <= 0 || number > INT_MAX)
			{
				printf("\ngetint->strtol: number format/value error\n");

				return 0;
			}

			*numbers++ = (int)number;

			counter++;
		}
	}

	if(counter != quantity)						/* Check quantity */
	{
		printf("\ngetint->strtok: file format error\n");

		return 0;
	}

	return 1;
}


/* Return the average of the numbers in an array; it is necessary to inform the
** total number of numbers and their type.
*/

double mean(void *numbers, int quantity, int type)
{
	int i = 0;			/* Array position */
	int *inumb;			/* int number */
	double mean = 0.0;
	double *dnumb;		/* double number */
	
	void* pointerto(void*, int, int);

	while(i < quantity)
	{
		if(type == INT)
		{									/* Get pointer to array element */
			inumb = (int*)pointerto( numbers, i++, sizeof(int) );

			mean += *inumb;
		}
		else if(type == DOUBLE)
		{
			dnumb = (double*)pointerto( numbers, i++, sizeof(double) );

			mean += *dnumb;
		}
	}

	mean /= quantity;

	return mean;
}


/* Return the pointer to an element of a generic element array based on the
** element's position and the size in bytes of the elements' type.
*/

void* pointerto(void *arr, int pos, int tsize)
{
	char *pointer = (char*)arr;			/* Because a char has 1 byte */

	pointer += pos * tsize;

	return (void*)pointer;
}


/* Return the sample standard deviation of the numbers in an array; it is
** necessary to inform the total number of numbers and their type.
*/

double stddev(void *numbers, double mean, int quantity, int type)
{
	int i = 0;			/* Array position */
	int *inumb;			/* int number */
	double diff;		/* Difference between two values */
	double var = 0.0;	/* Variance*/
	double *dnumb;		/* double number */

	void* pointerto(void*, int, int);

	while(i < quantity)
	{
		if(type == INT)
		{									/* Get pointer to array element */
			inumb = (int*)pointerto( numbers, i++, sizeof(int) );

			diff = (double)*inumb - mean;

			var += diff * diff;
		}
		else if(type == DOUBLE)
		{
			dnumb = (double*)pointerto( numbers, i++, sizeof(double) );

			diff = *dnumb - mean;

			var += diff * diff;
		}
	}

	var /= quantity - 1;		/* To calculate the sample standard deviation */

	return sqrt(var);
}


/* Create a file and save the program results; returns 0 if there is a problem
** and 1 otherwise.
*/

int writefile(double wordsppg, double err1, double wordsnum, double err2)
{
	FILE *file;

	file = fopen(FILENAME2, "w");

	if(!file)
	{
		perror("\nwritefile->fopen");

		return 0;
	}
															/* Save results */
	fprintf( file, "\ndensity: %ld +- %ld  words per page\n",
							   lround(wordsppg), lround(err1) );

	fprintf( file, "\ntotal: %ld +- %ld  kilowords",
					lround(wordsnum / 1000), lround(err2 / 1000) );

	if( ferror(file) )
	{
		printf("\nwritefile->fprintf: error when writing file\n");

		fclose(file);

		return 0;
	}

	fclose(file);

	return 1;
}