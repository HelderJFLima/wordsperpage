This program calculates an estimate of the average number of words per page of a book and its approximate total number of words. For this purpose, mean and standard deviation calculations are used and statistical inferences are made using the Student distribution. The error propagation calculation in the process is also used. The reliability of the results is 80%.

The program receives as input a text file which must contain the following information:

- 1st line: the numbers of the starting and ending pages that will be considered in the analysis;

- 2nd line: the number of lines of 10 pages selected at random;

- 3rd – 12th lines: the number of words from 4 lines selected at random from each of the 10 pages previously chosen; incomplete words are counted as 0.5.

The result is saved in a file in the same folder as the input.

The input format is schematized below:

********************************************************************************
N1,N2
L1,L2,L3,...,L10
W1-1,W1-2,W1-3,W1-4
W2-1,W2-2,W2-3,W2-4
W3-1,W3-2,W3-3,W3-4
...
W10-1,W10-2,W10-3,W10-4
********************************************************************************

Below is a real example of input:

********************************************************************************
9,140
28,28,27,25,16,26,28,28,28,28
7,11,6,8
10,2,11,9.5
9,7.5,13,13
9.5,10,4,7.5
9.5,7,10,11.5
10,10,10,9
12,10,9,9
12,12,12,13
12,9,9,12
13,9.5,9,7
********************************************************************************

This is the corresponding output:

********************************************************************************

density: 252 +- 21  words per page

total: 33 +- 3  kilowords

********************************************************************************


by HelderJFLima
