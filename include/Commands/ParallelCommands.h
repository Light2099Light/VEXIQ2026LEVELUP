#include "MyCommandHeaders.h"
#pragma once


struct ParallelData
{
	CommandType commandType;
	bool isConfigured;
    uint8_t commCount;
    uint8_t count;
    uint8_t currIndex;

    bool finishedCommands[];
};


void initParallelCommand(int commandcount)
{
	VERIFY(sizeof(SetPneumaticData) <= sizeof(BaseCommand));

	ParallelData data;
	data.commandType = (CommandType)ParallelRun;
	data.isConfigured = false;
    data.commCount = commandcount;
    data.finishedCommands[data.commCount];

    BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, &data, sizeof(ParallelData));

	bot.index++;

}

void configure(ParallelData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

    data->count = 0;

    data->currIndex = 1;

    for(int i = 0; i < data->commCount; i++)
    {
        data->finishedCommands[i] = false;
    }

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(ParallelData));

}

bool hasRan(ParallelData* realdata)
{
    if(realdata->count >= realdata->commCount) return true;
    
    printf("index: %d\n", realdata->currIndex);
    if(realdata->currIndex > realdata->commCount)
    {
        printf("resetIndex");
        realdata->currIndex = 1;
    }


        if(realdata->finishedCommands[realdata->currIndex-1])
        {
            printf("The %dth command is finished\n", realdata->currIndex);
        }
        else
        {
            BaseCommand& command = bot.commands[bot.index+realdata->currIndex];
            switch(command.commandType)
            {

                case AttachMotor:
                {
                    printf("arm");

                    MoveAttachMotorData data;

                    memcpy(&data, &command, sizeof(MoveAttachMotorData));

                    configure(&data);

                     
                    

                    if(!hasRan(&data))
                    {
                        realdata->currIndex++;
                        return false;
                    }

                    cleanup(&data);

                    realdata->finishedCommands[realdata->currIndex-1] = true;
                    realdata->count++;
                }
                break;

                case SetPneumatic:
                {
                    SetPneumaticData data;

                    memcpy(&data, &command, sizeof(SetPneumaticData));

                    configure(&data);

                     
                 

                    if(!hasRan(&data))
                    {
                        realdata->currIndex++;
                        return false;
                    }

                    cleanup(&data);

                    realdata->finishedCommands[realdata->currIndex-1] = true;
                    realdata->count++;
                }
                break;

                case MoveByDegrees:
                {

                    printf("drive");

                    RobotDrivetrainData data;

                    memcpy(&data, &command, sizeof(RobotDrivetrainData));

                    configure(&data);

                     
                  

                    if(!hasRan(&data))
                    {
                        printf("reached");
                        realdata->currIndex++;
                        return false;
                    }
                    printf("hey you fucked up");
                    cleanup(&data);

                    realdata->finishedCommands[realdata->currIndex-1] = true;
                    realdata->count++;

                }
                break;

                case TurnByGyro:
                {
                    TurnByGyroData data;

                    memcpy(&data, &command, sizeof(TurnByGyroData));

                    configure(&data);

               

                    if(!hasRan(&data))
                    {
                        realdata->currIndex++;
                        return false;
                    }

                    cleanup(&data);

                    realdata->finishedCommands[realdata->currIndex-1] = true;
                    realdata->count++;

                }
                break;

                case WaitUntilPressed:
                {
                    WaitUntilLedPressedData data;

                    memcpy(&data, &command, sizeof(WaitUntilLedPressedData));

                    configure(&data);

                     
                 

                    if(!hasRan(&data))
                    {
                        realdata->currIndex++;
                        return false;
                    }

                    cleanup(&data);

                    realdata->finishedCommands[realdata->currIndex-1] = true;
                    realdata->count++;

                }
                break;

                case DriveUntilBeanBag:
                {
                    DriveUntilBeanBagData data;

                    memcpy(&data, &command, sizeof(DriveUntilBeanBagData));

                    configure(&data);

                     
                 

                    if(!hasRan(&data))
                    {
                        realdata->currIndex++;
                        return false;
                    }

                    cleanup(&data);

                    realdata->finishedCommands[realdata->currIndex-1] = true;
                    realdata->count++;

                }

                case DriveInCurve:
                {
                    printf("curve");
                    
                    CurveData data;

                    memcpy(&data, &command, sizeof(CurveData));

                    configure(&data);

                     
                   
                    if(!hasRan(&data))
                    {
                        realdata->currIndex++;
                        return false;
                    }

                    cleanup(&data);

                    realdata->finishedCommands[realdata->currIndex-1] = true;
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
        
    
	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, realdata, sizeof(ParallelData));
    return false;
}

void cleanup(ParallelData* data)
{
    bot.index += (data->commCount+1);

    
}
