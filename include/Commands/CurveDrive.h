#include "bot.h"
#pragma once
#define MIN_POWER 10


vex::touchled versionLed = vex::touchled(PORT7);

struct CurveData
{
	CommandType commandType;
	bool isConfigured;
    short duration;
    short angle;
};


void initCurveData(short duration, short angle,  bool isRecovery = false)
{
	VERIFY(sizeof(CurveData) <= sizeof(BaseCommand));

	CurveData data;
	data.commandType = (CommandType)DriveInCurve;
	data.isConfigured = false;

    data.duration = duration;

	data.angle = angle;

	memcpy(&bot.commands[bot.index], &data, sizeof(CurveData));

	bot.index++;

}

void configure(CurveData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(CurveData));

	bot.gyro.setRotation(0, vex::rotationUnits::deg);
}

bool hasRan(CurveData* data)
{
	int current = bot.gyro.rotation(degrees);
    int error = data->angle - current;

    bot.left.motorPort.spin((20*sgn(data->duration) + error*(0.1)));
    bot.right.motorPort.spin((20*sgn(data->duration) - error*(0.1)));

	printf("Angle: %d, Error: %d, Current: %d\n", data->angle, error, bot.gyro.rotation());

    if(abs(bot.gyro.rotation()) >= abs(data->angle) && (abs(bot.left.motorPort.position()+bot.right.motorPort.position())/2 >= abs(data->duration)))
    {
        return true;
    }

    
	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(CurveData));
	return false;
}

void cleanup(CurveData* data)
{
	bot.left.motorPort.stop();
    bot.right.motorPort.stop();
    bot.center.motorPort.stop();
}
