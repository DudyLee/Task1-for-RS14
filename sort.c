/*
  Written by Raymond Wong.
  It is used for experimental setup.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef WIN32
#include <sys/resource.h>
#include <sys/times.h>
#endif

struct list_s
{
	int *array;
	int size;
};

typedef struct list_s list;

struct config_s
{
	char *dataFilename;

	int size;
	int parameter2;
	int parameter3;
	float parameter4;
};

typedef struct config_s config;


struct statistics_s
{
	int noOfSwapping;
};

typedef struct statistics_s statistics;


// globl variable
statistics *stat;

#define MAXSTR 2048


#ifndef WIN32
float  userTime_read, sysTime_read;
float  userTime_sort, sysTime_sort;
float  userTime_write, sysTime_write;
float  userTime_overall, sysTime_overall;
struct rusage myTime_program_start, myTime_read_end, myTime_sort_end, myTime_write_end;
#endif


#ifndef WIN32
void calculateExecutionTime(struct rusage *myTimeStart, struct rusage *myTimeEnd, float *userTime, float *sysTime)
{
	(*userTime) =
		((float) (myTimeEnd->ru_utime.tv_sec  - myTimeStart->ru_utime.tv_sec)) +
		((float) (myTimeEnd->ru_utime.tv_usec - myTimeStart->ru_utime.tv_usec)) * 1e-6;
	(*sysTime) =
		((float) (myTimeEnd->ru_stime.tv_sec  - myTimeStart->ru_stime.tv_sec)) +
		((float) (myTimeEnd->ru_stime.tv_usec - myTimeStart->ru_stime.tv_usec)) * 1e-6;
	
}
#endif

list *list_new(int size)
{
	list *returnValue;

	returnValue = (list *) malloc(sizeof(list));
	memset(returnValue, 0, sizeof(list));

	returnValue->size = size;
	returnValue->array = (int *) malloc(sizeof(int)*size);

	return returnValue;
}


list *clone(list *aList)
{
	list *returnValue;

	int i;

	returnValue = list_new(aList->size);
	for (i = 0; i < aList->size; i++)
	{
		returnValue->array[i] = aList->array[i];
	}

	return returnValue;
}

statistics *statistics_new()
{
	statistics *returnValue;

	returnValue = (statistics *) malloc(sizeof(statistics));
	memset(returnValue, 0, sizeof(statistics));

	return returnValue;
}

config *config_new()
{
	config *returnValue;

	returnValue = (config *) malloc(sizeof(config));
	memset(returnValue, 0, sizeof(config));

	returnValue->dataFilename = (char *) malloc(sizeof(char)*MAXSTR);

	return returnValue;
}

config *readConfig(char *configFilename)
{
	config *returnValue;
	FILE *filePtr;

	float aFloat;

	returnValue = config_new();

	filePtr = (FILE *) fopen(configFilename, "r");
	if (filePtr == NULL)
	{
		printf("File %s cannot be opened!\n", configFilename);
		exit(0);
	}

	fscanf(filePtr, "%s\n", returnValue->dataFilename);
	fscanf(filePtr, "%d\n", &(returnValue->size));
	fscanf(filePtr, "%d\n", &(returnValue->parameter2));
	fscanf(filePtr, "%d\n", &(returnValue->parameter3));
	fscanf(filePtr, "%f\n", &aFloat);
	returnValue->parameter4 = aFloat;

	fclose(filePtr);

	return returnValue;
}

list *readDataFile(config *aConfig)
{
	list *returnValue;
	int i;
	int aNo;

	FILE *filePtr;

	filePtr = (FILE *) fopen(aConfig->dataFilename, "r");
	if (filePtr == NULL)
	{
		printf("File %s cannot be opened!\n", aConfig->dataFilename);
		exit(0);
	}

	returnValue = list_new(aConfig->size);

	for (i = 0; i < aConfig->size; i++)
	{
		fscanf(filePtr, "%d ", &aNo);

		returnValue->array[i] = aNo;
	}


	fclose(filePtr);

	return returnValue;
}

list *sort(list *aList)
{
	list *returnValue;
	int i, j;
	int size;

	int *array;
	int temp;

	size = aList->size;

	returnValue = clone(aList);

	array = returnValue->array;
	

	for (i = 0; i < size; i++)
	{
		for (j = i+1; j < size; j++)
		{
			if (array[i] > array[j])
			{
				// swap
				temp = array[i];
				array[i] = array[j];
				array[j] = temp;

				stat->noOfSwapping++;
			}
		}
	}

	return returnValue;
}

void printSortedFile(list *aList)
{
	int i;

	FILE *filePtr;

	char *filename = "output.txt";

	filePtr = (FILE *) fopen(filename, "w");
	if (filePtr == NULL)
	{
		printf("File %s cannot be opened!\n", filename);
		exit(0);
	}

	for (i = 0; i < aList->size; i++)
	{
		fprintf(filePtr, "%d ", aList->array[i]);
	}
	fprintf(filePtr, "\n");

	fclose(filePtr);
}

void printStatFile()
{
	char *filename = "stat.txt";

	FILE *filePtr;

	filePtr = (FILE *) fopen(filename, "w");
	if (filePtr == NULL)
	{
		printf("File %s cannot be opened!\n", filename);
		exit(0);
	}

	fprintf(filePtr, "%d\n", stat->noOfSwapping);

	fclose(filePtr);
}

void printTimeFile()
{
	FILE *filePtr;
	char *filename = "time.txt";

	filePtr = (FILE *) fopen(filename, "w");
	if (filePtr == NULL)
	{
		printf("File %s cannot be opened!\n", filename);
		exit(0);
	}

#ifndef WIN32
 	// output execution time 
	calculateExecutionTime(&myTime_program_start, &myTime_read_end, &userTime_read, &sysTime_read);
	calculateExecutionTime(&myTime_read_end, &myTime_sort_end, &userTime_sort, &sysTime_sort);
	calculateExecutionTime(&myTime_sort_end, &myTime_write_end, &userTime_write, &sysTime_write);
	calculateExecutionTime(&myTime_program_start, &myTime_write_end, &userTime_overall, &sysTime_overall);

	// reading time
	fprintf(filePtr, "%.10f %.10f\n", userTime_read, sysTime_read);

	// sorting time
	fprintf(filePtr, "%.10f %.10f\n", userTime_sort, sysTime_sort);

	// writing time
	fprintf(filePtr, "%.10f %.10f\n", userTime_write, sysTime_write);

	// overall time
	fprintf(filePtr, "%.10f %.10f\n", userTime_overall, sysTime_overall);

#endif
	fclose(filePtr);
}

int main()
{

	char *configFilename = "config.txt";

	list *aList;
	list *aSortedList;

	config *aConfig;

#ifndef WIN32
	// start time 
	getrusage(RUSAGE_SELF,&myTime_program_start);
#endif

	// initialize the stat
	stat = statistics_new();

	// reading config. file
	printf("Reading Config. file...\n");
	aConfig = readConfig(configFilename);

	// read data file
	printf("Reading data file...\n");
	aList = readDataFile(aConfig);


#ifndef WIN32
	// start time 
	getrusage(RUSAGE_SELF,&myTime_read_end);
#endif

	// sort
	printf("Sorting...\n");
	aSortedList = sort(aList);


#ifndef WIN32
	// start time 
	getrusage(RUSAGE_SELF,&myTime_sort_end);
#endif

	// output
	printf("Printing the sorted list...\n");
	printSortedFile(aSortedList);


#ifndef WIN32
	// start time 
	getrusage(RUSAGE_SELF,&myTime_write_end);
#endif

	// print statistics file
	printf("Printing the statistics file...\n");
	printStatFile();

	// print time file
	printf("Printing the time file...\n");
	printTimeFile();
	

	return 0;
}
