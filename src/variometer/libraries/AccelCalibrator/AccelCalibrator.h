// AccelCalibrator.h
//

#ifndef __ACCELCALIBRATOR_H__
#define __ACCELCALIBRATOR_H__

#include <DefaultSettings.h>
#include <Arduino.h>
#include <MPU6050_.h>
#include <LEDFlasher.h>

#define ACCEL_CALIBRATOR_ORIENTATION_COUNT 			6
#define ACCEL_CALIBRATOR_ORIENTATION_THRESHOLD 		0.7

/*********************/
/*    ORIENTATION    */
/*********************/

/* here the accel orientation code : */
/* (0, 0, 1)  ->  0                  */
/* (0, 0, -1) ->  1                  */
/* (0, 1, 0)  ->  2                  */
/* (0, -1, 0) ->  3                  */
/* (1, 0, 0)  ->  4                  */
/* (-1, 0, 0) ->  5                  */

/* !!! adjust the following settings to your setup !!! */ 

/* -> the exception  */
/* this orientation is not used in the calibration procedure */
/* because the accelerometer is almost never in this position */
/* usually pointing to the the ground */
#define ACCEL_CALIBRATOR_ORIENTATION_EXCEPTION 1

/* -> the three prority orientations */
/* the three orientations used for base calibration */
/* this make priority to them. You may use : */
/* toward sky, toward left or right, toward user */
#define ACCEL_CALIBRATOR_ORIENTATION_P1 0
#define ACCEL_CALIBRATOR_ORIENTATION_P2 3
#define ACCEL_CALIBRATOR_ORIENTATION_P3 4


/**************************/
/* CALIBRATION PARAMETERS */
/**************************/
#define ACCEL_CALIBRATOR_WAIT_DURATION 3000
#define ACCEL_CALIBRATOR_FILTER_SIZE 1000
#define ACCEL_CALIBRATOR_BASE_RADIUS 1.0
#define ACCEL_CALIBRATOR_BASE_RADIUS_DRIFT 0.01
#define ACCEL_CALIBRATOR_BASE_RADIUS_STEP 0.001
#define ACCEL_CALIBRATOR_OPTIMIZATION_PRECISION 0.000001


//
//
//
//

//#define USE_ALL_ORIENTATION


////////////////////////////////////////////////////////////////////////////////////
// class AccelCalibrator

class AccelCalibrator
{
public:
	AccelCalibrator();

	void init(void);

	/* reset the calibration, you need to restart measures */
	void reset(void);

	/* launch accel measure and compute standard deviation */
	void measure(LEDFlasher * flasher = 0);
	
	void prepareMeasure(); // call frequently : it's empty the FIFO and stabilize the accelerometer
	void startMeasure();
	int continueMeasure();
	void finishMeasure();

	void getCalibratedMeasure(float * accel);

	/* return measure orientation */
	int getMeasureOrientation(void);

	/* save the measured value */
	/* return if the measure can be used for calibration */
	/* possible problems : ambiguous orientation, bad standard deviation */
	boolean pushMeasure(void);

	/* once all measures are done, calibrate */
	boolean canCalibrate(void);
	void calibrate(void);

	/* you can get calibrated measures */
	void calibratedMeasure(void);


private:
	void computeCenter(float* v1, float* v2, float* v3, float radius, float* center);
	float computeDistanceVariance(float *v, float* center);
	
	int readRawAccelAsync(float * accel);
	void readRawAccel(float * accel);

public:
	/* for one measure */
	float measuredAccel[3];  //computed by measure()
	float measuredAccelSD;   //accel standard deviation

	/* store measure for calibration */
	float accelList[3*ACCEL_CALIBRATOR_ORIENTATION_COUNT]; //all possible orientations
	boolean accelListDone[ACCEL_CALIBRATOR_ORIENTATION_COUNT];
	float accelListSD[ACCEL_CALIBRATOR_ORIENTATION_COUNT]; //record strandard deviation

	/* final calibration data */
	float calibration[3]; 
	boolean calibrated;
	
	//
	MPU6050_ & imu;
	
private:
	// for asynchronous measurement
	unsigned long measureTime;
	int measureCount;

	float measureSquareMean[3];
};

#endif // __ACCELCALIBRATOR_H__
