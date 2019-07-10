#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "epicsTypes.h"

#include "myUtil.h"

/*
  Short description: Interval Binary Search

  Full description:

  Performs a binary search to find the array interval where the input value is located.
  The interval that the algorithm tries to find is limited by two adjacent array indices.
  For instance, suppose the interval corresponding to value 14.5 should be found in the
  array represented below:

            Input Array                     Output Interval

     indices:  1  2  3  4  5        output interval:   3  4          |
               __ __ __ __ __                          __ __         |
              |__|__|__|__|__|                        |__|__|        | Because:
                                                                     |
     values:   3  7  11 16 19                values:   11 16         | 11 <= 14.5 <= 16

  The corresponding output interval would be given by indices 3 and 4.
  The function only returns the lower bound index, since the upper bound is always equal to lowerBoundIdx + 1.

  Arguments:

  epicsFloat64 *arr: sorted input array.
  epicsUInt16 lo: lower array bound for search.
  epicsUInt16 up: upper array bound for search.
  epicsFloat64: value to search.

  Return value:

  epicsInt32 idx: interval beginning index.
  
      Error status:

      idx = -1 : input value out of array bounds.
      idx = -2 : search error (maybe unsorted input array).

*/
epicsInt32 binarySearch(const epicsFloat64 *arr, epicsUInt16 lo, epicsUInt16 up, epicsFloat64 value)
{
    epicsUInt16 mid;
    // check if value is out of table range
    if (value < arr[lo] || value > arr[up]){
        return -1;
    }
    // find smallest table interval in which the value can be found
    while (lo < up)
    {
        mid = lo + (up - lo)/2;
        if (value > arr[mid]){
            lo = mid + 1;
            if (value <= arr[lo]){
                // return interval start idx
                return mid;
            }
        }
        else if (value < arr[mid]){
            up = mid - 1;
            if (value > arr[up]){
                // return interval start idx
                return up;
            }
        }
        else{
            // mid value exactly matches search
            return mid;
        }
    }
    return -2;
}

/*
  subroutine: interpolateFromTable

  Overall:

  Given two input tables, corresponding to the domain and
  codomain of a conversion function, convert a given input
  value by mapping this value to the second table, using an
  interval in the first table as reference, and interpolating
  for better resolution.
  
  Description:

  First, the function identifies the interval in the first
  table to which the input value belongs. The corresponding
  interval in the second table is taken and interpolated to
  yield the conversion output.

  OBS: The conversion accuracy is only as good as the accuracy
       and resolution of the conversion tables. Functions with
       smooth slope changes can be approximated by fewer points,
       while functions with high frequency components require
       more points.

  Arguments:

  arrX: conversion function domain table.
  arrY: conversion function codomain table.
  arrSize: size of the conversion tables.
  result: pointer to variable that will hold result.
          This variable MUST be allocated before calling this function.
          This function does NOT allocate space for the result.
  value: input value for conversion.

  OBS: TABLES MUST HAVE THE SAME SIZE.

  Return value:

  0: success
 -1: error: input value out of table bounds.
 -2: error: binary search failed.
 -3: error: division by zero when calculating slope.

*/
epicsInt8 interpolateFromTable(const epicsFloat64 *arrX, const epicsFloat64 *arrY, 
                               epicsUInt16 arrSize, epicsFloat64* result, epicsFloat64 value)
{

    // variables
    epicsInt32 index;
    epicsFloat64 slope; 

    //search index
    index = binarySearch(arrX, 0, arrSize-1, value);
    if (index < 0){
        return index;
    }
    // interpolation slope
    slope = arrX[index+1] - arrX[index]; // denominator
    if (slope == 0){
        // error, division by zero
        return -3;
    }
    slope = (arrY[index+1] - arrY[index]) / slope;
    // return interpolated value
    *result = slope*(value - arrX[index]) + arrY[index];

    // return status
    return 0;
}
