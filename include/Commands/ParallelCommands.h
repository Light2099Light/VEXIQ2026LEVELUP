#include "MyCommandHeaders.h"
#pragma once


struct ParallelData
{
	CommandType commandType;
	bool isConfigured;
    uint8_t commCount;
    uint8_t count;
    uint8_t currIndex;

    bool isfinishedCommands[];
};


void initParallelCommand(int commandcount)
{
	VERIFY(sizeof(SetPneumaticData) <= sizeof(BaseCommand));

	ParallelData data;
	data.commandType = (CommandType)ParallelRun;
	data.isConfigured = false;
    data.commCount = commandcount;
    data.isfinishedCommands[data.commCount];

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
        data->isfinishedCommands[i] = false;
    }

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(ParallelData));

}

bool hasRan(ParallelData* realdata)
{
    
    printf("index: %d\n", realdata->currIndex);
    if(realdata->currIndex > realdata->commCount)
    {
        printf("resetIndex");
        realdata->currIndex = 1;
    }


        if(realdata->isfinishedCommands[realdata->currIndex-1])
        {
            printf("The %dth command is finished\n", realdata->currIndex);
        }
        else
        {
            BaseCommand& pcommand = bot.commands[bot.index+realdata->currIndex];
            switch(pcommand.commandType)
            {

                case AttachMotor:
                {
                    printf("arm");

                    MoveAttachMotorData data;

                    memcpy(&data, &pcommand, sizeof(MoveAttachMotorData));

                    configure(&data);

                     
                    

                    if(!hasRan(&data))
                    {
                        realdata->currIndex++;
                        return false;
                    }

                    cleanup(&data);

                    realdata->isfinishedCommands[realdata->currIndex-1] = true;
                    realdata->count++;
                }
                break;

                

                case MoveByDegrees:
                {

                    printf("drive parallel\n");

                    RobotDrivetrainData data;

                    memcpy(&data, &pcommand, sizeof(RobotDrivetrainData));

                    configure(&data);

                     
                  

                    if(!hasRan(&data))
                    {
                        printf("reached");
                        realdata->currIndex++;
                        return false;
                    }

                    cleanup(&data);

                    realdata->isfinishedCommands[realdata->currIndex-1] = true;
                    realdata->count++;

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
    printf("cleanedup");
    bot.index += (data->commCount+1);

    
}
