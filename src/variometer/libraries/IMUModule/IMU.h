// IMU.h
//

#ifndef __IMU_H__
#define __IMU_H__

#include <I2CDevice.h>
#include <IVerticalVelocity.h>

////////////////////////////////////////////////////////////////////////////////////
// class IMU

class IMU
{
protected:
	IMU();
	
public:
	int					begin(IVerticalVelocity * vv, boolean calibrateGyro = true);
	int					begin();
	void				end();
	
	boolean				readRawAccelData(float * accel, float * uv, float * va);
	
	float				getAcceleration();
	float				getPressure();
	float				getTemperature();
	float				getAltitude();
	
	float *				getRawAccelData();
	float *				getRawGyroData();	
	
	static IMU &		getInstance();
	static void			timerProc();
	
protected:
	//
	void				initMS6511();
	void				initMPU6050(boolean calibrateGyro);
	
	void				updatePressure();
	boolean				updateAcceleration();

	//
	uint16_t			readPROMValue(int address);
	uint32_t			readDigitalValue();
	
	void				convertD1();
	void				convertD2();
	
	void				readTempStep();
	void				readPressureStep();
	void				readStep();
	
	//
	void				startTimer();
	void				stopTimer();
	
	void				restartTimer();

	
protected:
	//
	volatile uint8_t				imuState;
	
	//
	IVerticalVelocity *	iVelocity;
	
	// MS5611
	//
	
	// PROM factors
	uint16_t 			c1, c2, c3, c4, c5, c6; //PROM factors
	// compensated values
	float 				compensatedTemperature;
	float 				compensatedPressure;
	
	// to store measures generated by interrupts
	volatile uint32_t 	d1i;
	volatile uint32_t 	d2i;

	// measure status
	volatile int 		measureStep;
	volatile boolean 	newData;
	
	// MPU6050
	//
	
	// raw data
	float				accelData[3];
	float				gyroData[3];	
	
	//
	volatile float		vertAccel;
};

#endif // __IMU_H__

