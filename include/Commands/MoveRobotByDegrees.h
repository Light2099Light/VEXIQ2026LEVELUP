
#include "TrapezoidalProfile.h"
struct RobotDrivetrainData
{
	CommandType commandType;
	bool isConfigured;
	TrapezoidalStates leftState;
	TrapezoidalStates rightState;
	unsigned short time;
	short leftDuration;
	short rightDuration;
};
const int commandSize = sizeof(RobotDrivetrainData);
void initMoveByDegreeData(unsigned short time, short leftDuration, short rightDuration, TrapezoidalStates leftState, TrapezoidalStates rightState, bool isRecovery = false)
{
	VERIFY(commandSize <= sizeof(BaseCommand));

	RobotDrivetrainData data;
	data.commandType = isRecovery ? (CommandType)MoveByDegreesRec : (CommandType)MoveByDegrees;
	data.isConfigured = false;
	data.leftDuration = leftDuration;
	data.rightDuration = rightDuration;
	data.leftState = leftState;
	data.rightState = rightState;
	data.time = time;
	printf("InitLeftD: %d\n", data.leftDuration);

	memcpy(&bot.commands[bot.index], &data, commandSize);

	bot.index++;
}

void configure(RobotDrivetrainData* data)
{
	if(data->isConfigured) return;

	data->isConfigured = true;

	initTrapezoidalProfileData((MotorData&)bot.left, data->leftDuration, data->leftState, bot.left.motorPort.position(), 3);
	initTrapezoidalProfileData((MotorData&)bot.right, data->rightDuration, data->rightState, bot.right.motorPort.position(), 5);
	motorDataChecksAndProcedures((MotorData&)bot.left);
	motorDataChecksAndProcedures((MotorData&)bot.right);
	resetCurrentPosition((MotorData&)bot.left, bot.left.motorPort.position());
	resetCurrentPosition((MotorData&)bot.right, bot.right.motorPort.position());
	CommTimer.clear();

	BaseCommand* command = (BaseCommand*)&bot.commands[bot.index];
	memcpy(command, data, sizeof(RobotDrivetrainData));
}

bool hasRan(RobotDrivetrainData* data)
{
	if(CommTimer.time() >= data->time)
	{
		return true;
	}
	bool isLeftDone = TrapezoidalProfileMoveByDegrees((MotorData&)bot.left, bot.left.motorPort.position());
	bool isRightDone = TrapezoidalProfileMoveByDegrees((MotorData&)bot.right, bot.right.motorPort.position());

	if(isLeftDone && isRightDone)
	{
		return true;
	}

	return false;
}

void cleanup(RobotDrivetrainData* data)
{
	bot.left.motorPort.stop();
	bot.right.motorPort.stop();
}
