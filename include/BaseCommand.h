#include <cassert>

#ifndef VERIFY
#define VERIFY(condition) do { if (!(condition)) { assert(#condition && false); } } while (0)
#endif

enum CommandType
{
	NoOperation = 0x00,
	MoveByDegrees = 0x01,
	AttachMotor = 0x02,
	TurnByGyro = 0x03,
	AttachMotorRec = 0x81,
	MoveByDegreesRec = 0x82,
	StopRecovery = 0x84,
	Stop = 0x05,
	SetPneumatic = 0x06,
	WaitUntilPressed = 0x07,
	DriveUntilBeanBag = 0x08,
	DriveInCurve = 0x09,
	ParallelRun = 0x10,
	MoveUntilPressed = 0x011,
};

struct BaseCommand
{
	CommandType commandType;
	bool isConfigured;
	uint8_t commandBuffer[14];
};
