// -------------------------------------------------------------------------------
// Name:        Digital Filter Implementation
//
// Author:      Julian Gherghel
// -------------------------------------------------------------------------------
//
// Purpose
// A program to demonstrate the application of a simple digital filter
// 
// Overview
// A sequence of data items and digital filter values need to be entered by the
// user. The application of the filter to the data involves a simple convolution 
// operation. The filtered data are stored separately. 
//
// Example
//  before filtering: 
//   data_in = [0 1 3 6 3 1 0]
//   filter = [-0.5 1 -0.5]
//  after filtering:
//   data_out = [-0.5 -0.5 3 -0.5 -0.5]
//  where
//   data_out[0]=data_in[0]*filter[0]+data_in[1]*filter[1]+data_in[2]*filter[2]
//   data_out[1]=data_in[1]*filter[0]+data_in[2]*filter[1]+data_in[3]*filter[2]
//   data_out[2]=data_in[2]*filter[0]+data_in[3]*filter[1]+data_in[4]*filter[2]
//   data_out[3]=data_in[3]*filter[0]+data_in[4]*filter[1]+data_in[5]*filter[2]
//   data_out[4]=data_in[4]*filter[0]+data_in[5]*filter[1]+data_in[6]*filter[2]
//
// The program checks the following
// 1. The data and filter values must have been entered before the filter is 
//    applied
// 2. The filter is not applied if the number of filter values is greater than
//    the number of input data values
// 3. The data and filter values must have been entered and the filter applied 
//    before the filtered data can be displayed
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// the data values and the filter
struct TheFilter {
    double* Values;   // the filter values
    unsigned long Length;  // number of filter values
    bool Valid;   // true if the filter values have been obtained
};

struct TheData {
    double* Values;  // holds the data to be filtered
    unsigned long Length;  // number of data values
    bool Valid;   // true if the data values have been obtained
};

// function return values
enum { OK, FILTER_TOO_LONG };

// function prototypes
/* <enter code here> */
void EnterData(struct TheData *OriginalData);
void EnterFilter(struct TheFilter *Filter);
int ApplyFilter(struct TheFilter Filter, struct TheData OriginalData, struct TheData *FilteredData);
void DisplayData(struct TheFilter Filter, struct TheData OriginalData, struct TheData FilteredData);

// Control the principal operations of the program
// Arguments: None
// Returns: 0 on completion
int main()
{
    // define the filter and its initial values
    struct TheFilter Filter = { 0,0,false };

    // define the original data and its initial values
    struct TheData OriginalData = { 0,0,false };

    // define the filtered data and its initial values
    struct TheData FilteredData = { 0,0,false };

    char UserInput;

    // loop until the user wishes to exit
    while (1) {

        // show the menu of options
        printf("\n");
        printf("Filter Menu\n");
        printf("-----------\n");
        printf("\n");
        printf("1. Enter data for filtering\n");
        printf("2. Enter filter values\n");
        printf("3. Apply filter\n");
        printf("4. Display filtered data\n");
        printf("5. Exit from the programi\n\n");

        // get the user's choice
        printf("Enter your option: ");
        scanf(" %c", &UserInput);
        printf("\n");

        // act on the user's input
        switch (UserInput) {
        case '1':
            EnterData(&OriginalData);
            FilteredData.Valid = false;
            break;

        case '2':
            EnterFilter(&Filter);
            FilteredData.Valid = false;
            break;

        case '3':
            if (Filter.Valid == true && OriginalData.Valid == true &&
                FilteredData.Valid == false) {
                if (ApplyFilter(Filter, OriginalData, &FilteredData) == FILTER_TOO_LONG) {
                    printf("The filter must not be longer than the data\n");
                }
                else {
                    FilteredData.Valid = true;
                    printf("Filter applied\n");
                }
            }
            break;

        case '4':
            if (Filter.Valid == true && OriginalData.Valid == true &&
                FilteredData.Valid == true) {
                // call the DisplayData function
                // <enter code here//
                DisplayData(Filter, OriginalData, FilteredData);
            }
            else {
                printf("Data have not yet been filtered\n");
            }
            break;

        case '5':
            free(Filter.Values);
            free(OriginalData.Values);
            free(FilteredData.Values);
            return 0;
            break;

        default:
            printf("Invalid entry\n\n");
            break;
        }
    }
}

// Allow the user to enter the data to be filtered
// Arguments:
//   (1) the structure containing the input data
// Returns: nothing
// 
void EnterData(struct TheData *OriginalData)
{
    // initialize the data structure that holds the data to be filtered, including 
    // getting the number of data values from the user
    free(OriginalData->Values);
    printf("Type the number of data values you would like to enter: ");
    scanf("%lu", &OriginalData->Length);
    OriginalData->Valid = true;

        // allocate memory to the data
    OriginalData->Values = malloc(sizeof(double) * OriginalData->Length);
    if (OriginalData->Values == 0) {
        printf("Insufficient memory allocated!\n");
        exit(1);
    }

        // obtain all of the data values
    printf("Type the data values you would like to enter: \n");
    for (unsigned long CountData = 0; CountData < OriginalData->Length; CountData++) {
        printf("Enter data value: %lu \n", CountData + 1);
        scanf("lf", &OriginalData->Values[CountData]);
    }
}

// Allow the user to enter the filter values
// Arguments:
//   (1) the structure of the filter to be defined
// Returns: nothing
// 
void EnterFilter(struct TheFilter *Filter)
{
    // initialize the data structure that holds the filter, including getting 
    // the number of filter values from the user
    free(Filter->Values);
    printf("Type the number of data values you wish to enter: ");
    scanf("%lu", &Filter->Length);
    Filter->Valid = true;

        // allocate memory to the filter values
        Filter->Values = malloc(sizeof(double) * Filter->Length);
        if (Filter->Values == 0) {
            printf("Insufficient memory allocated!\n");
            exit(1);
    }

        // obtain all of the filter values
        printf("Type the filter values you would like to enter: \n");
        for (unsigned long CountData = 0; CountData < Filter->Length; CountData++) {
            printf("Enter filter value: %lu \n", CountData+1);
            scanf("%lf", &Filter->Values[CountData]);
        }
}

// Apply the filter to the input data and store in the filtered data structure
// Arguments:
//   (1) the structure of the filter to be applied
//   (2) the structure containing the data to be filtered
//   (3) the structure to hold the filtered data
// Returns: OK - if the filter is applied
//          FILTER_TOO_LONG - the filter is longer than the data 
//  
int ApplyFilter(struct TheFilter Filter, struct TheData OriginalData, struct TheData *FilteredData)
{
    // return an error if the filter is longer than the data
    if (Filter.Length > OriginalData.Length) return FILTER_TOO_LONG;

    // initialize the data structure that holds the filtered data
    free(FilteredData->Values);
    FilteredData->Length = OriginalData.Length - Filter.Length + 1;

        // get memory for the filtered data
    FilteredData->Values = malloc(sizeof(double) * FilteredData->Length); 
    if (FilteredData->Values == 0) {
        printf("Insufficient memory allocated!\n");
        exit(1);
    }

        // apply the filter to the data
    for (unsigned long CountData = 0; CountData < FilteredData->Length; CountData++) {
        FilteredData->Values[CountData] = 0.0;
        for (unsigned long CountFilter = 0; CountFilter < Filter.Length; CountFilter++) {
            FilteredData->Values[CountData] += OriginalData.Values[CountData + CountFilter] * 
                Filter.Values[CountFilter];
        }
        }

        return OK;
}

// Display input data, filter values and output data
// Arguments:
//   (1) the structure of the filter to be applied
//   (2) the structure containing the data to be filtered
//   (3) the structure that holds the filtered data
// Returns: nothing
// 
void DisplayData(struct TheFilter Filter, struct TheData OriginalData, struct TheData FilteredData)
{
    // display all of the input data values
    printf("The inputted data values:");
    for (unsigned long CountData = 0; CountData < OriginalData.Length; CountData++) {
        printf("%lf", OriginalData.Values[CountData]);
    }

    // display all of the filter values
    printf("The inputted filter values:");
    for (unsigned long CountData = 0; CountData < Filter.Length; CountData++) {
        printf("%lf", Filter.Values[CountData]);
    }

    // display all of the data output values
    printf("The outputted data values:");
    for (unsigned long CountData = 0; CountData < FilteredData.Length; CountData++) {
        printf("%lf", FilteredData.Values[CountData]);
    }
}
