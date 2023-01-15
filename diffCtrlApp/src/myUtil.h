#ifndef MY_UTIL
#define MY_UTIL

/* CONVERSION TABLES

 Y = f(X)

 where X is the stage position, and Y is the actuator tip position.

 The topmost and lowermost points are both extrapolations of the
 measured lookup table. Their purpose is to allow some extra range to
 avoid conversion failure when the device operating close to the
 operating range limit.
*/

#define convTableVScrapBottow_Size 7
#define convTableVScrapTop_Size    7
#define convTableHScrapOut_Size    7
#define convTableHScrapIn_Size     7

// Vertical Scraper - Bottow Blade - Movement Conversion Table
// X values (linear stage)
static const epicsFloat64 convTableVScrapBottow_X[convTableVScrapBottow_Size] =
{
 9, 12.4337, 13.5540, 14.6742, 15.7945, 16.9147, 24
};
// Y values (scraper tip)
static const epicsFloat64 convTableVScrapBottow_Y[convTableVScrapBottow_Size] =
{
 -9.75951013, -4.500, -2.784, -0.838, 1.087, 2.991, 15.03385949
};

// Vertical Scraper - Top Blade - Movement Conversion Table
// X values (linear stage)
static const epicsFloat64 convTableVScrapTop_X[convTableVScrapTop_Size] =
{
 -24, -16.9497, -15.76, -14.5703, -13.3806, -12.1962, -9
};
// Y values (scraper tip)
static const epicsFloat64 convTableVScrapTop_Y[convTableVScrapTop_Size] =
{
 -16.17751694, -4.669, -2.727, -0.747, 1.144, 3.175, 8.65581915
};

// Horizontal Scraper - Outer Blade - Movement Conversion Table
// X values (linear stage)
static const epicsFloat64 convTableHScrapOut_X[convTableHScrapOut_Size] =
{
 -24, -19.8036, -17.6451, -15.4866, -13.3280, -11.1695, -9
};
// Y values (scraper tip)
static const epicsFloat64 convTableHScrapOut_Y[convTableHScrapOut_Size] =
{
-16.96887728, -11.780, -9.111, -5.778, -2.539, 0.589, 3.7329407
};

// Horizontal Scraper - Inner Blade - Movement Conversion Table
// X values (linear stage)
static const epicsFloat64 convTableHScrapIn_X[convTableHScrapIn_Size] =
{
 9, 10.1396, 12.4137, 14.6877, 16.9618, 19.2358, 24
};
// Y values (scraper tip)
static const epicsFloat64 convTableHScrapIn_Y[convTableHScrapIn_Size] =
{
-13.6376545, -11.781, -8.076, -3.727, -0.648, 2.964, 10.53141003
};

/* Functions */

epicsInt32 binarySearchAsc(const epicsFloat64 *arr, epicsUInt16 lo, epicsUInt16 up, epicsFloat64 value);
epicsInt32 binarySearchDesc(const epicsFloat64 *arr, epicsUInt16 lo, epicsUInt16 up, epicsFloat64 value);
epicsInt8 interpolateFromTable(const epicsFloat64 *arrX, const epicsFloat64 *arrY, epicsUInt16 arrSize, epicsFloat64* result, epicsFloat64 value);

#endif
