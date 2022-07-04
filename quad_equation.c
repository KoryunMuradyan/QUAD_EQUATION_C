#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/* 
 * the below function "read_from_file" gets as argument filename as a string
 * reads the content and returns a b c as a string
 */
char* read_from_file(const char* filename)
{
	static char c[1000];
	FILE *fptr;
	if ((fptr = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "Error! File cannot be opened.!!!!!\n");
		exit(-1);
	}
	fscanf(fptr, "%[^\n]", c);
	fclose(fptr);
	return &(c[0]);
}

/* 
 * the below function gets a b c as string argument 
 * converts them to double type
 * and returns a b c as double type array
 */
double* get_abc(char* arg_expr, unsigned int sz)
{
	for(int i = 0; i < sz; ++i) {
		if(' ' == arg_expr[i] || '+' == arg_expr[i] || '-' == arg_expr[i] || '.' != arg_expr[i]) {
			continue;
		} else if(arg_expr[i] < '0' || arg_expr[i] > '9') {
			fprintf(stderr, "not numeric symbol got from file !\n");
			exit(-1);
		}
	}
	int index = 0;
	static double abc[] = {0, 0, 0};
	char *token = strtok(arg_expr, " ");
	while( token != NULL ) {
		abc[index++] = strtod(token, NULL);
		token = strtok(NULL, " ");
	}
	if(3 < index) {
		fprintf(stderr, "extra parameters got from file !\n");
		exit(-1);
	} else if(3 > index) {
		fprintf(stderr, "too few parameters got from file !\n");
		exit(-1);
	}
	return &(abc[0]);
}

/* 
 * the below function "solve_quad" gets as argument a b c
 * solves the equation and returns roots if any
 */
double* solve_quad(double* abc)
{
	if(0 == abc[0]) {
		if(0 == abc[1]) {
			if(0 == abc[2]) {
				printf("an identity was got from file \n no output file will be generated !\n");
				exit(1);
			} else {
				fprintf(stderr, "a wrong identity was got from file \n");
				exit(-1);
			}
		}
		printf(" not quadratic but linear equatation was got from file \n");
	}
	double *x1_x2 = malloc(2);
	double a = abc[0], b = abc[1], c = abc[2];
	double det = b * b - 4 * a * c;
	if(0 == det)
	{
		x1_x2[0] = -b/(2.0*a);
		x1_x2[1] = x1_x2[0];
	} else if(det > 0)
	{
	   x1_x2[0] = (-b+sqrt(det))/(2*a);
	   x1_x2[1] = (-b-sqrt(det))/(2*a);
	} else {
	    printf("No Solution. \n");
	    return x1_x2;
	}
	return x1_x2;
}

/* 
 * the below function "test" gets as argument roots if any
 * compares them with those in golden.txt and prints command line message
 * (the last is optional the purpose  is technical)
 */
int test(double* roots)
{
	FILE *fptr;
	fptr = fopen("golden.txt", "r");
	if(fptr == NULL){
		printf("file can not be opened\n");
		exit(0);
	}
	char x1_str[50], x2_str[50];
	fscanf(fptr, " %50s", x1_str);
	fscanf(fptr, " %50s", x2_str);
	double x1 = strtod(x1_str, NULL);
	double x2 = strtod(x2_str, NULL);
	fclose(fptr);
	if((x1 == roots[0] && x2 == roots[1]) || (x2 == roots[0] && x1 == roots[1])) {
		return 1;
	} else {
		return 0;
	}
	return 0;
}

/* 
 * the below function "create_output_file" gets as argument roots if any
 * and generates an "output.txt" file containing the results
 */
void create_output_file(double *roots, int test_result){
	char x0[50], x1[50];
	snprintf(x0, 50, "%f", roots[0]);
	snprintf(x1, 50, "%f", roots[1]);
	char whitespaces[] = "   ";
	strcat(x0, whitespaces);
	strcat(x0, x1);
	if(test_result) {
		strcat(x0, "  Right result\n");
	} else {
		strcat(x0, "  wrong result\n");
	}
	for(int i = strlen(x0) + 1; i != sizeof(x0); ++i) {
		x0[i] = '\0';
	}
	FILE * fptr;
	fptr = fopen("output.txt", "w");
	fwrite(x0, sizeof(x0[0]), sizeof(x0), fptr);
	fclose(fptr);
}

int main(int argc, char** argv)
{
	char* equation = read_from_file(argv[1]);
	double *abc = get_abc(equation, strlen(equation));
	double *roots = solve_quad(abc);
	create_output_file(roots, test(roots));
	return 0;
}
