#include "MyCommandHeaders.h"
#pragma once


struct ParallelData
{
	CommandType commandType;
	bool isConfigured;
    int range;
    uint8_t count;

    bool finishedCommands[];
};


void initParallelCommand(int range)
{
	VERIFY(sizeof(SetPneumaticData) <= sizeof(BaseCommand));

	ParallelData data;
	data.commandType = (CommandType)SetPneumatic;
	data.isConfigured = false;
    data.range = range;
    data.finishedCommands[range];

    BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, &data, sizeof(ParallelData));

	bot.index++;

}

void configure(ParallelData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

    data->count = 0;

    for(int i = 0; i < data->range; i++)
    {
        data->finishedCommands[i] = false;
    }

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(ParallelData));

}

bool hasRan(ParallelData* realdata)
{
    if(realdata->count >= realdata->range) return true;

    for(int i = 1; i < realdata->range; i++)
    {
        if(realdata->finishedCommands[i-1])
        {

        }
        else
        {
            BaseCommand& command = bot.commands[bot.index+i];
            switch(command.commandType & 0x7F)
            {

                case AttachMotor:
                {
                    MoveAttachMotorData data;

                    memcpy(&data, &command, sizeof(MoveAttachMotorData));

                    configure(&data);

                    if(!hasRan(&data)) break;

                    cleanup(&data);

                    realdata->finishedCommands[i-1] = true;
                    realdata->count++;
                }
                break;

                case SetPneumatic:
                {
                    SetPneumaticData data;

                    memcpy(&data, &command, sizeof(SetPneumaticData));

                    configure(&data);

                    if(!hasRan(&data)) break;

                    cleanup(&data);

                    realdata->finishedCommands[i-1] = true;
                    realdata->count++;
                }
                break;

                case MoveByDegrees:
                {

                    RobotDrivetrainData data;

                    memcpy(&data, &command, sizeof(RobotDrivetrainData));

                    configure(&data);

                    if(!hasRan(&data)) break;

                    cleanup(&data);

                    realdata->finishedCommands[i-1] = true;
                    realdata->count++;

                }
                break;

                case TurnByGyro:
                {
                    TurnByGyroData data;

                    memcpy(&data, &command, sizeof(TurnByGyroData));

                    configure(&data);

                    if(!hasRan(&data)) break;

                    cleanup(&data);

                    realdata->finishedCommands[i-1] = true;
                    realdata->count++;

                }
                break;

                case WaitUntilPressed:
                {
                    WaitUntilLedPressedData data;

                    memcpy(&data, &command, sizeof(WaitUntilLedPressedData));

                    configure(&data);

                    if(!hasRan(&data)) break;

                    cleanup(&data);

                    realdata->finishedCommands[i-1] = true;
                    realdata->count++;

                }
                break;

                case DriveUntilBeanBag:
                {
                    DriveUntilBeanBagData data;

                    memcpy(&data, &command, sizeof(DriveUntilBeanBagData));

                    configure(&data);

                    if(!hasRan(&data)) break;

                    cleanup(&data);

                    realdata->finishedCommands[i-1] = true;
                    realdata->count++;

                }

                case DriveInCurve:
                {
                    printf("curve");
                    
                    CurveData data;

                    memcpy(&data, &command, sizeof(CurveData));

                    configure(&data);

                    if(!hasRan(&data)) break;

                    cleanup(&data);

                    realdata->finishedCommands[i-1] = true;
                    realdata->count++;

                }

                case StopRecovery:
                {
                    bot.isRecovery = false;
                }
                break;

                case Stop:
                {
                    while(true)
                    {
                        wait(200, vex::timeUnits::msec);
                    }
                }
                break;
            }
        }
        
    }
	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, realdata, sizeof(ParallelData));
    return false;
}

void cleanup(ParallelData* data)
{
    bot.index += (data->range+1);

    
}
