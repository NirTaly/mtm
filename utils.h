/******************************************************************************
                              	   	 Utils
                                    Nir Taly
 *****************************************************************************/

#include <stdio.h>			/* FILE					*/
#include <string.h> 		/* strcmp				*/

/******************************************************************************/
#define BITS_IN_BYTE	(8)
#define SET_BYTE 		(0xFF)
#define BYTES_IN_WORD	(sizeof(size_t))
#define ARRAY_SIZE(A)	(sizeof(A)/sizeof((A)[0]))
#define UNUSED_VAR(A)	((void)(A))
#define MAX(A,B)		((A) > (B) ? (A) : (B))
#define MIN(A,B)		((A) < (B) ? (A) : (B))

#ifndef NDEBUG
#define DBG(X) X
#else
#define DBG(X)
#endif

/******************************************************************************/
						/* COLORS */
#define COLOR_RED     "\x1b[01;31m"
#define COLOR_GREEN   "\x1b[01;32m"
#define COLOR_YELLOW  "\x1b[01;33m"
#define COLOR_BLUE    "\x1b[01;34m"
#define COLOR_MAGENTA "\x1b[01;35m"
#define COLOR_CYAN    "\x1b[01;36m"
#define COLOR_RESET   "\x1b[0m"

#define LOG_RED(X) { printf("%s %s %s\n",COLOR_RED,X,COLOR_RESET); }
#define LOG_GREEN(X) { printf("%s %s %s\n",COLOR_GREEN,X,COLOR_RESET); }
#define LOG_CYAN(X) { printf("%s %s %s\n",COLOR_CYAN,X,COLOR_RESET); }

/******************************************************************************/
						/* Test Macro */
		/* Test Case For return val == VAL:*/
#define TEST(INDEX,EXPR,VAL,TEXT) { \
		if (VAL != EXPR) { \
			printf("%d) Test: " TEXT COLOR_RED "\t\t\tFAIL\n" COLOR_RESET, INDEX); \
			fprintf(stderr, "%s: %03d: %s: FAILED!\n", __FILE__, __LINE__, #EXPR); \
		} else { \
			printf("%d) Test " TEXT ":" COLOR_GREEN "\t\t\tTRUE!\n" COLOR_RESET, INDEX); \
		} }

		/* Test Case For return val != VAL:*/
#define NOT_EQUAL_TEST(INDEX,EXPR,VAL,TEXT) \
		if (VAL == EXPR) { \
			printf("%d) Test: " TEXT COLOR_RED "\t\t\tFAIL\n" COLOR_RESET, INDEX); \
			fprintf(stderr, "%s: %03d: %s: FAILED!\n", __FILE__, __LINE__, #EXPR); \
		} else { \
			printf("%d) Test " TEXT ":" COLOR_GREEN "\t\t\tTRUE!\n" COLOR_RESET, INDEX); \
		}

/* Format For Any Type */
#define TEST_BASE(INDEX, EXPR, VAL, TYPE, FORMAT, TEXT) { \
			TYPE val = VAL; TYPE expr = EXPR; \
		if (val != expr) { \
			printf("%d) Test: " TEXT COLOR_RED "\t\t\tFAIL\n" COLOR_RESET "Returned: " FORMAT \
				"\tExpected: " FORMAT "\n", INDEX, expr,val); \
			fprintf(stderr, "%s: %03d: %s: FAILED!\n", __FILE__, __LINE__, #EXPR); \
		} else { \
			printf("%d) Test " TEXT ":" COLOR_GREEN "\t\t\tTRUE!\n" COLOR_RESET, INDEX); \
		} }

/* comparisson between 2 strings */
#define TEST_STR(INDEX, EXPR, VAL, TEXT) TEST_BASE(INDEX, strcmp(EXPR,VAL), 0, \
			int, "%d", TEXT)

#define TEST_INT(INDEX, EXPR, VAL, TEXT) TEST_BASE(INDEX, EXPR, VAL, int, "%d", TEXT)
		
#define TEST_PTR(INDEX, EXPR, VAL, TEXT) TEST_BASE(INDEX, EXPR, VAL, \
			void*, "%p", TEXT)		

/* check is EXPR isn't 0 or NULL */
#define TEST_IS_TRUE(EXPR) { \
		if(!EXPR) { \
			fprintf(stderr, "%s: %03d: %s: FAILED!\n", __FILE__, __LINE__, #EXPR); \
				  } \
							}
/* check is EXPR is 0 or NULL */
#define TEST_FUNC_TRUE(EXPR) { \
		if(EXPR) { \
			fprintf(stderr, "%s: %03d: %s: FAILED!\n", __FILE__, __LINE__, #EXPR); \
				  } \
							}

#define TEST_TRUE(EXPR, VAL, I) { \
		if(EXPR != VAL) { \
			fprintf(stderr, "i =%d ,%s: %03d: %s: FAILED!\n",I, __FILE__, __LINE__, #EXPR); \
				  } \
							}

#define TEST_ARRAY(ARR1, ARR2, LENGTH) { \
		size_t i = 0; \
		while (ARR1[i] == ARR2[i] && i < LENGTH) {i++;} \
		if (i != LENGTH) \
				fprintf(stderr, "%s: %03d: Arrays Match: FAILED!\n" \
								"arr[%lu]\n", \
					__FILE__, __LINE__, i); \
										}

#define ERROR_HANDLER(VAR, VALUE, TEXT, EXIT_STATUS) { \
		if (VAR == VALUE) { \
		fprintf(stderr, TEXT "ERROR\n"); \
		return (EXIT_STATUS); \
						 } \
						 							 }
													  
