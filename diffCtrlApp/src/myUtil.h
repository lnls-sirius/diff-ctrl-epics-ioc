#ifndef MY_UTIL
#define MY_UTIL

/* CONVERSION TABLES

 Y = f(X)

 where X is the stage position, and Y is the actuator tip position.
*/

#define convTableVScrapBottow_Size 60
#define convTableVScrapTop_Size    60
#define convTableHScrapOut_Size    10
#define convTableHScrapIn_Size     10

// Vertical Scraper - Bottow Blade - Movement Conversion Table
// X values (linear stage)
static const epicsFloat64 convTableVScrapBottow_X[convTableVScrapBottow_Size] =
{
  0.1,  1.1,  2.1,  3.1,  4.1,  5.1,  6.1,  7.1,  8.1,  9.1, 10.1, 11.1, 12.1, 13.1, 14.1, 15.1, 16.1, 17.1, 18.1, 19.1,
 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0,
 40.2, 41.2, 42.2, 43.2, 44.2, 45.2, 46.2, 47.2, 48.2, 49.2, 50.2, 51.2, 52.2, 53.2, 54.2, 55.2, 56.2, 57.2, 58.2, 59.2
};
// Y values (scraper tip)
static const epicsFloat64 convTableVScrapBottow_Y[convTableVScrapBottow_Size] =
{
   1,  11,  21,  31,  41,  51,  61,  71,  81,  91, 101, 111, 121, 131, 141, 151, 161, 171, 181, 191,
 200, 210, 220, 230, 240, 250, 260, 270, 280, 290, 300, 310, 320, 330, 340, 350, 360, 370, 380, 390,
 402, 412, 422, 432, 442, 452, 462, 472, 482, 492, 502, 512, 522, 532, 542, 552, 562, 572, 582, 592
};


// Vertical Scraper - Top Blade - Movement Conversion Table
// X values (linear stage)
static const epicsFloat64 convTableVScrapTop_X[convTableVScrapTop_Size] =
{
  0.1,  1.1,  2.1,  3.1,  4.1,  5.1,  6.1,  7.1,  8.1,  9.1, 10.1, 11.1, 12.1, 13.1, 14.1, 15.1, 16.1, 17.1, 18.1, 19.1,
 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0,
 40.2, 41.2, 42.2, 43.2, 44.2, 45.2, 46.2, 47.2, 48.2, 49.2, 50.2, 51.2, 52.2, 53.2, 54.2, 55.2, 56.2, 57.2, 58.2, 59.2
};
// Y values (scraper tip)
static const epicsFloat64 convTableVScrapTop_Y[convTableVScrapTop_Size] =
{
 0.01, 0.11, 0.21, 0.31, 0.41, 0.51, 0.61, 0.71, 0.81, 0.91, 1.01, 1.11, 1.21, 1.31, 1.41, 1.51, 1.61, 1.71, 1.81, 1.91,
 2.00, 2.10, 2.20, 2.30, 2.40, 2.50, 2.60, 2.70, 2.80, 2.90, 3.00, 3.10, 3.20, 3.30, 3.40, 3.50, 3.60, 3.70, 3.80, 3.90,
 4.02, 4.12, 4.22, 4.32, 4.42, 4.52, 4.62, 4.72, 4.82, 4.92, 5.02, 5.12, 5.22, 5.32, 5.42, 5.52, 5.62, 5.72, 5.82, 5.92
};

// Horizontal Scraper - Outer Blade - Movement Conversion Table
// X values (linear stage)
static const epicsFloat64 convTableHScrapOut_X[convTableHScrapOut_Size] =
{
 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};
// Y values (scraper tip)
static const epicsFloat64 convTableHScrapOut_Y[convTableHScrapOut_Size] =
{
 0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5
};

// Horizontal Scraper - Inner Blade - Movement Conversion Table
// X values (linear stage)
static const epicsFloat64 convTableHScrapIn_X[convTableHScrapIn_Size] =
{
 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
};
// Y values (scraper tip)
static const epicsFloat64 convTableHScrapIn_Y[convTableHScrapIn_Size] =
{
 0, 2, 4, 6, 8, 10, 12, 14, 16, 18
};

/* Functions */

epicsInt32 binarySearch(const epicsFloat64 *arr, epicsUInt16 lo, epicsUInt16 up, epicsFloat64 value);
epicsInt8 interpolateFromTable(const epicsFloat64 *arrX, const epicsFloat64 *arrY, epicsUInt16 arrSize, epicsFloat64* result, epicsFloat64 value);

#endif
