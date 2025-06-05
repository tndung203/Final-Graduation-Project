/*
 * mq135.h
 *
 *  Created on: Jun 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_MQ135_H_
#define INC_MQ135_H_

/// The load resistance on the board
#define RLOAD 10.0
/// Calibration resistance at atmospheric CO2 level
#define RZERO 76.63
/// Parameters for calculating ppm of CO2 from sensor resistance
#define PARA 116.6020682
#define PARB 2.769034857

/// Parameters to model temperature and humidity dependence
#define CORA 0.00035
#define CORB 0.02718
#define CORC 1.39538
#define CORD 0.0018

/// Atmospheric CO2 level for calibration purposes
#define ATMOCO2 397.13

float getCorrectionFactor(float t, float h);
float getResistance();
float getCorrectedResistance(float t, float h);
float getPPM();
float getCorrectedPPM(float t, float h);
float getRZero();
float getCorrectedRZero(float t, float h);


#endif /* INC_MQ135_H_ */
