#include "bot.h"
#pragma once
#define MIN_POWER 10


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

	bot.gyro.setRotation(0, rotationUnits::deg);
}

bool hasRan(CurveData* data)
{
    int error = angle - bot.gyro.rotation();

    bot.left.motorPort.setVelocity(50 - error, pct);
    bot.right.motorPort.setVelocity(50 + error, pct);

    if(abs(bot.gyro.rotation()) >= abs(data->angle) && (bot.left.motorPort.position()+bot.right.motorPort.position())>>1 >= data->duration)
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
