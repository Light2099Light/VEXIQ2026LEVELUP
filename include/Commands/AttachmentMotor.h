#pragma once
struct MoveAttachMotorData
{
	CommandType commandType;
	bool isConfigured;
	unsigned short duration;
	int8_t power;
	int time;
};

void initMoveAttachMotorData(int time, int8_t power, unsigned short duration, bool isRecovery = false)
{
	VERIFY(sizeof(MoveAttachMotorData) <= sizeof(BaseCommand));

	MoveAttachMotorData data;
	data.commandType = isRecovery ? (CommandType)AttachMotorRec : (CommandType)AttachMotor;
	data.isConfigured = false;

	data.power = power;
	data.duration = duration;

	data.time = time;

	memcpy(&bot.commands[bot.index], &data, sizeof(MoveAttachMotorData));

	bot.index++;

}

void configure(MoveAttachMotorData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	ArmTimer.clear();

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(MoveAttachMotorData));

	bot.center.motorPort.setPosition(0);
}

bool hasRan(MoveAttachMotorData* data)
{
	
	if(data->power < 0 && armLimitSwitch.pressing())
	{
		return true;
	}
	else if((unsigned)abs(bot.center.motorPort.position()) >= data->duration)
	{
		return true;
	}
	else if(ArmTimer.time() >= data->time)
	{
		return true;
	}
	else
	{

		bot.center.motorPort.spin(((data->power)*((data->duration-bot.center.motorPort.position())/data->duration))+(10*sgn(data->power)));
	}




	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(MoveAttachMotorData));
	return false;
}

void cleanup(MoveAttachMotorData* data)
{
	bot.center.motorPort.stop();
}
