#include "bot.h"
#pragma once
#define MIN_POWER 10


struct CurveData
{
	CommandType commandType;
	bool isConfigured;
    short duration;
    int8_t leftMax;
    int8_t rightMax;
};


void initCurveData(short duration, int8_t leftMax, int8_t rightMax, bool isRecovery = false)
{
	VERIFY(sizeof(CurveData) <= sizeof(BaseCommand));

	CurveData data;
	data.commandType = (CommandType)TurnByGyro;
	data.isConfigured = false;

	data.angle = angle;

	memcpy(&bot.commands[bot.index], &data, sizeof(CurveData));

	bot.index++;

}

void configure(TurnByGyroData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(TurnByGyroData));

	bot.gyro.setRotation(0, rotationUnits::deg);
}

bool hasRan(TurnByGyroData* data)
{
    int gyroRotation = bot.gyro.rotation();
    printf("Gyro rotation: %d, angle: %d\n", gyroRotation, data->angle);
    
    int direction = sgn(data->angle);

    if(abs(gyroRotation) < abs(data->angle))
    {
        int power = direction * abs(MIN_POWER);
        bot.left.motorPort.spin(power);
        bot.right.motorPort.spin(-power);
    }
    else
    {
        bot.left.motorPort.stop();
        bot.right.motorPort.stop();
        return true;
    }


    
        

    

    
	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(TurnByGyroData));
	return false;
}

void cleanup(TurnByGyroData* data)
{
	bot.left.motorPort.stop();
    bot.right.motorPort.stop();
    bot.center.motorPort.stop();
}
