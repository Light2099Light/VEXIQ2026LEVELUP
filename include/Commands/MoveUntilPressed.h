
#include "Bot.h"
struct MoveUntilPressedData
{
	CommandType commandType;
	bool isConfigured;
    int8_t power;
};
void initMoveUntilPressedData(int8_t power)
{
	VERIFY(sizeof(MoveUntilPressedData) <= sizeof(BaseCommand));

	MoveUntilPressedData data;
	data.commandType = (CommandType)MoveUntilPressed;
	data.isConfigured = false;
    data.power = power;

	memcpy(&bot.commands[bot.index], &data, sizeof(MoveUntilPressedData));

	bot.index++;
}

void configure(MoveUntilPressedData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	
	DriveTimer.clear();

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(MoveUntilPressedData));
}

bool hasRan(MoveUntilPressedData* data)
{
	if(funnelDetector.pressing() && data->power < 0)
	{
		return true;
	}
	
    bot.left.motorPort.spin(data->power);
    bot.right.motorPort.spin(data->power);
	

	return false;
}

void cleanup(MoveUntilPressedData* data)
{
	bot.left.motorPort.stop();
	bot.right.motorPort.stop();
}
