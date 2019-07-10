#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <dbDefs.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <aSubRecord.h>
#include <epicsExport.h>
#include "epicsTypes.h"

#include "myUtil.h"

/*
  subroutine: slitConvFnc

  Overall:

  Apply coordinate conversion for Slit edge position
  to linear stage position or from stage position
  to Slit edge position.

  THIS IS A DUMMY FUNCTION, since the Slit movement
  does NOT need conversion.

  Description:

  The routine can be used for convertion in
  both ways, from blade to stage position, or
  from stage to blade position.

  # Blade to stage / stage to blade
  Input A ===> Output VALA

  Input:

  A: Blade or stage set point.

  Output:

  VALA: Stage or blade position, respectively.

  OBS:

  VALA is not accessible by other records.
  Its value must be forwarded by OUTA.

*/
static long slitConvFnc(aSubRecord *precord)
{
    // since the slit blade is directly attached to the
    // linear stage, input motion is equal to output motion
    *((epicsFloat64*)precord->vala) = *((epicsFloat64*)(precord->a));

    // return success
    return 0;
}

/*
  subroutine: convFuncScrapTheoCmd

  Overall:

  Scraper coordinate conversion using a theoretical function.

  Apply conversion from Scraper blade tip position to linear
  stage position.

  # Scraper tip to linear stage
  Input A ===> Output VALA

  Input:

  A: Scraper blade tip position.
  B: scraper geometry parameter: 'small radius' in radians.
  C: scraper geometry parameter: 'large radius' in radians.
  D: scraper geometry parameter: 'stage angle' in radians.
  E: scraper geometry parameter: 'arm length' in desired output unit.

  Output:

  VALA: Linear stage position.

  OBS:

  VALA is not accessible by other records.
  Its value must be forwarded by OUTA.

*/
static long convFuncScrapTheoCmd(aSubRecord *precord)
{

    // Read parameters
    epicsFloat64 tip_pos = *((epicsFloat64*)precord->a);
    epicsFloat64 r_small = *((epicsFloat64*)precord->b);
    epicsFloat64 r_large = *((epicsFloat64*)precord->c);
    epicsFloat64 stage_angle = *((epicsFloat64*)precord->d);
    epicsFloat64 arm_length = *((epicsFloat64*)precord->e);

    // check input for problems
    if (r_large <= 0 || r_small <= 0) {
        // invalid radii
        printf("Coordinate conversion failure: "
               "In function convFuncScrapTheoCmd,"
               "inputs r_large and r_small cannot be zero or negative.\n");
        return -1;
    }
    if (cos(stage_angle) == 0) {
        // invalid stage angle will cause division by zero
        printf("Coordinate conversion failure: "
               "In function convFuncScrapTheoCmd,"
               "input stage angle cannot be multiple of 90 degrees.\n");
        return -2;
    }
    if (arm_length <= 0) {
        // invalid arm length
        printf("Coordinate conversion failure: "
               "In function convFuncScrapTheoCmd,"
               "input arm length cannot be zero.\n");
        return -3;
    }

    // precompute tip angle
    epicsFloat64 tip_angle = asin( r_small / r_large );

    // scraper tip position to linear stage displacement
    *((epicsFloat64*)precord->vala) = ( arm_length / cos(stage_angle) ) * 
                                      sin( asin((tip_pos + r_small) / r_large) - tip_angle );

    // return success
    return 0;
}

/*
  subroutine: convFuncScrapTheoRB

  Overall:

  Scraper coordinate conversion using a theoretical function.

  Apply conversion from linear stage position to Scraper 
  blade tip position.

  # Linear stage to Scraper tip position
  Input A ===> Output VALA

  Input:

  A: Linear stage position.
  B: scraper geometry parameter: 'small radius' in radians.
  C: scraper geometry parameter: 'large radius' in radians.
  D: scraper geometry parameter: 'stage angle' in radians.
  E: scraper geometry parameter: 'arm length' in desired output unit.

  Output:

  VALA: Scraper blade tip position.

  OBS:

  VALA is not accessible by other records.
  Its value must be forwarded by OUTA.

*/
static long convFuncScrapTheoRB(aSubRecord *precord)
{

    // Read parameters
    epicsFloat64 stage_pos = *((epicsFloat64*)precord->a);
    epicsFloat64 r_small = *((epicsFloat64*)precord->b);
    epicsFloat64 r_large = *((epicsFloat64*)precord->c);
    epicsFloat64 stage_angle = *((epicsFloat64*)precord->d);
    epicsFloat64 arm_length = *((epicsFloat64*)precord->e);

    // check input for problems
    if (r_large <= 0 || r_small <= 0) {
        // invalid radii
        printf("Coordinate conversion failure: "
               "In function convFuncScrapTheoCmd,"
               "inputs r_large and r_small cannot be zero or negative.\n");
        return -1;
    }
    if (cos(stage_angle) == 0) {
        // invalid stage angle will cause division by zero
        printf("Coordinate conversion failure: "
               "In function convFuncScrapTheoCmd,"
               "input stage angle cannot be multiple of 90 degrees.\n");
        return -2;
    }
    if (arm_length <= 0) {
        // invalid arm length
        printf("Coordinate conversion failure: "
               "In function convFuncScrapTheoCmd,"
               "input arm length cannot be zero.\n");
        return -3;
    }

    // precompute tip angle
    epicsFloat64 tip_angle = asin( r_small / r_large );

    // linear stage position to scraper tip position
    *((epicsFloat64*)precord->vala) = r_large * 
                                      sin( asin(stage_pos * cos(stage_angle)/arm_length)
                                           + tip_angle ) -
                                      r_small;

    // return success
    return 0;
}

/*
  subroutine: convFuncVScrapBottowCmd

  Overall:

  Apply coordinate conversion to Vertical Scraper Lower Blade.
  Convert desired tip position into linear stage position.

  Description:

  This function aims to provide an approximation to the Scraper
  tip to linear stage conversion function that achieves the required
  accuracy, considering all the nonlinearities. In order to do that,
  it uses interpolation of a lookup table, representing any
  function with accuracy only limited by the number of points used.

  # Scraper tip to stage position
  Input A ===> Output VALA

Input:

  A: Desired Scraper tip position.

  Output:

  VALA: Corresponding linear stage position.

  VAL: Conversion status.
    -1: conversion failed, input value out of bounds.
    -2: conversion failed, binary search failed.

  OBS:

  VALA is not accessible by other records.
  Its value must be forwarded by OUTA.

*/
static long convFuncVScrapBottowCmd(aSubRecord *precord)
{
    // variables
    epicsInt8 status = 0;

    // Read parameters
    epicsFloat64 tip_pos = *((epicsFloat64*)precord->a);

    // tip displacement to stage displacement
    status = interpolateFromTable(convTableVScrapBottow_Y,      /* input table */
                                  convTableVScrapBottow_X,      /* output table */
                                  convTableVScrapBottow_Size,   /* table size */
                                  (epicsFloat64*)precord->vala, /* result */
                                  tip_pos                       /* input value for conversion */
                                 );
    if (status < 0)
    {
        if (status == -1) printf("Coordinate conversion failure: In convFuncVScrapBottowCmd function,"
			          "input value is out of array bounds.\n");
        if (status == -2) printf("Coordinate conversion failure: In convFuncVScrapBottowCmd function,"
			         "search failed. Maybe an unsorted input array?\n");
        if (status == -3) printf("Coordinate conversion failure: In convFuncVScrapBottowCmd function,"
		                 "division by zero when calculating slope. Conversion array has repeated values.\n");
        return status;
    }

    // return success
    return 0;
}


/*
  subroutine: convFuncVScrapBottowRB

  Overall:

  Apply coordinate conversion to Vertical Scraper Lower Blade.
  Convert linear stage position into corresponding Scraper
  readback tip position.

  Description:

  This function aims to provide an approximation to the linear stage
  to Scraper tip conversion function that achieves the required
  accuracy, considering all the nonlinearities. In order to do that,
  it uses interpolation of a lookup table, representing any
  function with accuracy only limited by the number of points used.

  # Linear stage to Scraper tip position
  Input A ===> Output VALA

Input:

  A: Linear stage position.

  Output:

  VALA: Corresponding Scraper tip position.

  VAL: Conversion status.
    -1: conversion failed, input value out of bounds.
    -2: conversion failed, binary search failed.

  OBS:

  VALA is not accessible by other records.
  Its value must be forwarded by OUTA.

*/
static long convFuncVScrapBottowRB(aSubRecord *precord)
{
    // variables
    epicsInt8 status = 0;

    // Read parameters
    epicsFloat64 stage_pos = *((epicsFloat64*)precord->a);

    // stage displacement to tip position
    status = interpolateFromTable(convTableVScrapBottow_X,      /* input table */
                                  convTableVScrapBottow_Y,      /* output table */
                                  convTableVScrapBottow_Size,   /* table size */
                                  (epicsFloat64*)precord->vala, /* result */
                                  stage_pos                     /* input value for conversion */
                                 );
    if (status < 0){
        if (status == -1) printf("Coordinate conversion failure: In convFuncVScrapBottowRB function,"
			          "input value is out of array bounds.\n");
        if (status == -2) printf("Coordinate conversion failure: In convFuncVScrapBottowRB function,"
		                 "search failed. Maybe an unsorted input array?\n");
        if (status == -3) printf("Coordinate conversion failure: In convFuncVScrapBottowRB function,"
		                 "division by zero when calculating slope. Conversion array has repeated values.\n");
	return status;
    }

    // return success
    return 0;
}

/*
  subroutine: convFuncVScrapTopCmd

  Overall:

  Apply coordinate conversion to Vertical Scraper Upper Blade.
  Convert desired tip position into linear stage position.

  Description:

  This function aims to provide an approximation to the Scraper
  tip to linear stage conversion function that achieves the required
  accuracy, considering all the nonlinearities. In order to do that,
  it uses interpolation of a lookup table, representing any
  function with accuracy only limited by the number of points used.

  # Scraper tip to stage position
  Input A ===> Output VALA

Input:

  A: Desired Scraper tip position.

  Output:

  VALA: Corresponding linear stage position.

  VAL: Conversion status.
    -1: conversion failed, input value out of bounds.
    -2: conversion failed, binary search failed.

  OBS:

  VALA is not accessible by other records.
  Its value must be forwarded by OUTA.

*/
static long convFuncVScrapTopCmd(aSubRecord *precord)
{
    // variables
    epicsInt8 status = 0;

    // Read parameters
    epicsFloat64 tip_pos = *((epicsFloat64*)precord->a);

    // tip position to stage displacement
    status = interpolateFromTable(convTableVScrapTop_Y,         /* input table */
                                  convTableVScrapTop_X,         /* output table */
                                  convTableVScrapTop_Size,      /* table size */
                                  (epicsFloat64*)precord->vala, /* result */
                                  tip_pos                       /* input value for conversion */
                                 );
    if (status < 0){
        if (status == -1) printf("Coordinate conversion failure: In convFuncVScrapTopCmd function,"
                                 "input value is out of array bounds.\n");
        if (status == -2) printf("Coordinate conversion failure: In convFuncVScrapTopCmd function,"
                                 "search failed. Maybe an unsorted input array?\n");
        if (status == -3) printf("Coordinate conversion failure: In convFuncVScrapTopCmd function,"
			         "division by zero when calculating slope. Conversion array has repeated values.\n");
	return status;
    }

    // return success
    return 0;
}

/*
  subroutine: convFuncVScrapTopRB

  Overall:

  Apply coordinate conversion to Vertical Scraper Upper Blade.
  Convert linear stage position into Scraper readback tip position.

  Description:

  This function aims to provide an approximation to the Scraper
  tip to linear stage conversion function that achieves the required
  accuracy, considering all the nonlinearities. In order to do that,
  it uses interpolation of a lookup table, representing any
  function with accuracy only limited by the number of points used.

  # Linear stage to Scraper tip position
  Input A ===> Output VALA

Input:

  A: Linear stage position.

  Output:

  VALA: Corresponding Scraper tip position.

  VAL: Conversion status.
    -1: conversion failed, input value out of bounds.
    -2: conversion failed, binary search failed.

  OBS:

  VALA is not accessible by other records.
  Its value must be forwarded by OUTA.

*/
static long convFuncVScrapTopRB(aSubRecord *precord)
{
    // variables
    epicsInt8 status = 0;

    // Read parameters
    epicsFloat64 stage_pos = *((epicsFloat64*)precord->a);

    // stage displacement to tip position
    status = interpolateFromTable(convTableVScrapTop_X,         /* input table */
                                  convTableVScrapTop_Y,         /* output table */
                                  convTableVScrapTop_Size,      /* table size */
                                  (epicsFloat64*)precord->vala, /* result */
                                  stage_pos                     /* input value for conversion */
                                 );
    if (status < 0){
        if (status == -1) printf("Coordinate conversion failure: In convFuncVScrapTopRB function,"
                                 "input value is out of array bounds.\n");
        if (status == -2) printf("Coordinate conversion failure: In convFuncVScrapTopRB function,"
                                 "search failed. Maybe an unsorted input array?\n");
        if (status == -3) printf("Coordinate conversion failure: In convFuncVScrapTopRB function,"
                                 "division by zero when calculating slope. Conversion array has repeated values.\n");
	return status;
    }

    // return success
    return 0;
}

/*
  subroutine: convFuncHScrapOutCmd

  Overall:

  Apply coordinate conversion to Horizontal Scraper Outer Blade.
  Convert desired tip position into linear stage position.

  Description:

  This function aims to provide an approximation to the Scraper
  tip to linear stage conversion function that achieves the required
  accuracy, considering all the nonlinearities. In order to do that,
  it uses interpolation of a lookup table, representing any
  function with accuracy only limited by the number of points used.

  # Scraper tip to stage position
  Input A ===> Output VALA

Input:

  A: Desired Scraper tip position.

  Output:

  VALA: Corresponding linear stage position.

  VAL: Conversion status.
    -1: conversion failed, input value out of bounds.
    -2: conversion failed, binary search failed.

  OBS:

  VALA is not accessible by other records.
  Its value must be forwarded by OUTA.

*/
static long convFuncHScrapOutCmd(aSubRecord *precord)
{
    // variables
    epicsInt8 status = 0;

    // Read parameters
    epicsFloat64 tip_pos = *((epicsFloat64*)precord->a);

    // tip displacement to stage displacement
    status = interpolateFromTable(convTableHScrapOut_Y,          /* input table */
                                  convTableHScrapOut_X,          /* output table */
                                  convTableHScrapOut_Size,       /* table size */
                                  (epicsFloat64*)precord->vala,  /* result */
                                  tip_pos                        /* input value for conversion */
                                 );
    if (status < 0)
    {
        if (status == -1) printf("Coordinate conversion failure: In convFuncHScrapOutCmd function,"
			          "input value is out of array bounds.\n");
        if (status == -2) printf("Coordinate conversion failure: In convFuncHScrapOutCmd function,"
                                  "search failed. Maybe an unsorted input array?\n");
        if (status == -3) printf("Coordinate conversion failure: In convFuncHScrapOutCmd function,"
      	                         "division by zero when calculating slope. Conversion array has repeated values.\n");
        return status;
    }

    // return success
    return 0;
}

/*
  subroutine: convFuncHScrapOutRB

  Overall:

  Apply coordinate conversion to Horizontal Scraper Outer Blade.
  Convert linear stage position into Scraper readback tip position.

  Description:

  This function aims to provide an approximation to the Scraper
  tip to linear stage conversion function that achieves the required
  accuracy, considering all the nonlinearities. In order to do that,
  it uses interpolation of a lookup table, representing any
  function with accuracy only limited by the number of points used.

  # Linear stage to Scraper tip position
  Input A ===> Output VALA

Input:

  A: Linear stage position.

  Output:

  VALA: Corresponding Scraper tip position.

  VAL: Conversion status.
    -1: conversion failed, input value out of bounds.
    -2: conversion failed, binary search failed.

  OBS:

  VALA is not accessible by other records.
  Its value must be forwarded by OUTA.

*/
static long convFuncHScrapOutRB(aSubRecord *precord)
{
    // variables
    epicsInt8 status = 0;

    // Read parameters
    epicsFloat64 stage_pos = *((epicsFloat64*)precord->a);

    // stage displacement to tip position
    status = interpolateFromTable(convTableHScrapOut_X,          /* input table */
                                  convTableHScrapOut_Y,          /* output table */
                                  convTableHScrapOut_Size,       /* table size */
                                  (epicsFloat64*)precord->vala,  /* result */
                                  stage_pos                      /* input value for conversion */
                                 );
    if (status < 0){
        if (status == -1) printf("Coordinate conversion failure: In convFuncHScrapOutRB function,"
                                 "input value is out of array bounds.\n");
        if (status == -2) printf("Coordinate conversion failure: In convFuncHScrapOutRB function,"
                                 "search failed. Maybe an unsorted input array?\n");
        if (status == -3) printf("Coordinate conversion failure: In convFuncHScrapOutRB function,"
      	                         "division by zero when calculating slope. Conversion array has repeated values.\n");
	return status;
    }

    // return success
    return 0;
}

/*
  subroutine: convFuncHScrapInCmd

  Overall:

  Apply coordinate conversion to Horizontal Scraper Inner Blade.
  Convert desired tip position into linear stage position.

  Description:

  This function aims to provide an approximation to the Scraper
  tip to linear stage conversion function that achieves the required
  accuracy, considering all the nonlinearities. In order to do that,
  it uses interpolation of a lookup table, representing any
  function with accuracy only limited by the number of points used.

  # Scraper tip to stage position
  Input A ===> Output VALA

Input:

  A: Desired Scraper tip position.

  Output:

  VALA: Corresponding linear stage position.

  VAL: Conversion status.
    -1: conversion failed, input value out of bounds.
    -2: conversion failed, binary search failed.

  OBS:

  VALA is not accessible by other records.
  Its value must be forwarded by OUTA.

*/
static long convFuncHScrapInCmd(aSubRecord *precord)
{
    // variables
    epicsInt8 status = 0;

    // Read parameters
    epicsFloat64 tip_pos = *((epicsFloat64*)precord->a);

    // tip displacement to stage displacement
    status = interpolateFromTable(convTableHScrapIn_Y,          /* input table */
                                  convTableHScrapIn_X,          /* output table */
                                  convTableHScrapIn_Size,       /* table size */
                                  (epicsFloat64*)precord->vala, /* result */
                                  tip_pos                       /* input value for conversion */
                                 );
    if (status < 0)
    {
        if (status == -1) printf("Coordinate conversion failure: In convFuncHScrapInCmd function,"
			          "input value is out of array bounds.\n");
        if (status == -2) printf("Coordinate conversion failure: In convFuncHScrapInCmd function,"
			         "search failed. Maybe an unsorted input array?\n");
        if (status == -3) printf("Coordinate conversion failure: In convFuncHScrapInCmd function,"
      	                         "division by zero when calculating slope. Conversion array has repeated values.\n");
        return status;
    }

    // return success
    return 0;
}

/*
  subroutine: convFuncHScrapInRB

  Overall:

  Apply coordinate conversion to Horizontal Scraper Inner Blade.
  Convert linear stage position into Scraper readback tip position.

  Description:

  This function aims to provide an approximation to the Scraper
  tip to linear stage conversion function that achieves the required
  accuracy, considering all the nonlinearities. In order to do that,
  it uses interpolation of a lookup table, representing any
  function with accuracy only limited by the number of points used.

  # Linear stage to Scraper tip position
  Input A ===> Output VALA

Input:

  A: Linear stage position.

  Output:

  VALA: Corresponding Scraper tip position.

  VAL: Conversion status.
    -1: conversion failed, input value out of bounds.
    -2: conversion failed, binary search failed.

  OBS:

  VALA is not accessible by other records.
  Its value must be forwarded by OUTA.

*/
static long convFuncHScrapInRB(aSubRecord *precord)
{
    // variables
    epicsInt8 status = 0;

    // Read parameters
    epicsFloat64 stage_pos = *((epicsFloat64*)precord->a);

    // stage displacement to tip position
    status = interpolateFromTable(convTableHScrapIn_X,          /* input table */
                                  convTableHScrapIn_Y,          /* output table */
                                  convTableHScrapIn_Size,       /* table size */
                                  (epicsFloat64*)precord->vala, /* result */
                                  stage_pos                     /* input value for conversion */
                                 );
    if (status < 0){
        if (status == -1) printf("Coordinate conversion failure: In convFuncHScrapInRB function,"
			          "input value is out of array bounds.\n");
        if (status == -2) printf("Coordinate conversion failure: In convFuncHScrapInRB function,"
                                 "search failed. Maybe an unsorted input array?\n");
        if (status == -3) printf("Coordinate conversion failure: In convFuncHScrapInRB function,"
      	                         "division by zero when calculating slope. Conversion array has repeated values.\n");
	return status;
    }

    // return success
    return 0;
}

/* Register these symbols for use by IOC code: */

epicsRegisterFunction(slitConvFnc);
epicsRegisterFunction(convFuncVScrapBottowCmd);
epicsRegisterFunction(convFuncVScrapBottowRB);
epicsRegisterFunction(convFuncVScrapTopCmd);
epicsRegisterFunction(convFuncVScrapTopRB);
epicsRegisterFunction(convFuncHScrapOutCmd);
epicsRegisterFunction(convFuncHScrapOutRB);
epicsRegisterFunction(convFuncHScrapInCmd);
epicsRegisterFunction(convFuncHScrapInRB);
epicsRegisterFunction(convFuncScrapTheoCmd);
epicsRegisterFunction(convFuncScrapTheoRB);
