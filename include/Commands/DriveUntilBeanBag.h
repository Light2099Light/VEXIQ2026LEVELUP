#include "bot.h"
#pragma once
#define MIN_POWER 30


struct DriveUntilBeanBagData
{
	CommandType commandType;
	bool isConfigured;
	bool isBeanBagDetected;
    int8_t power;
	int8_t max;
	int8_t min;
};


void initDriveUntilBeanBagData(int8_t power, int8_t max, int8_t min, bool isBeanBagDetected, bool isRecovery = false)
{
	VERIFY(sizeof(DriveUntilBeanBagData) <= sizeof(BaseCommand));

	DriveUntilBeanBagData data;
	data.commandType = (CommandType)DriveUntilBeanBag;
	data.isConfigured = false;
	data.power = power;
	data.max = max;
	data.min = min;
	data.isBeanBagDetected = isBeanBagDetected;
	memcpy(&bot.commands[bot.index], &data, sizeof(DriveUntilBeanBagData));

	bot.index++;

}

void configure(DriveUntilBeanBagData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(DriveUntilBeanBagData));
}

bool hasRan(DriveUntilBeanBagData* data)
{
    bot.left.motorPort.spin(data->power);
    bot.right.motorPort.spin(data->power);

    if(bot.optical.hue() <= data->max && bot.optical.hue() >= data->min && !data->isBeanBagDetected)
    {
        bot.left.motorPort.stop();
        bot.right.motorPort.stop();
		
        return true;
    }
	



	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(DriveUntilBeanBagData));
	return false;
}

void cleanup(DriveUntilBeanBagData* data)
{
	bot.left.motorPort.stop();
    bot.right.motorPort.stop();
    bot.center.motorPort.stop();
}
