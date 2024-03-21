#include "Main.h"
#include "Byte_Manager.h"
#include "Read_Packet.h"


void* Original_Move_Caller;
struct Global_Variables_Structure
{
	__int8 Additional_Bytes[12];

	float Current_Time;

	float Frame_Time;

	__int32 Maximum_Clients;

	__int32 Tick_Number;

	float Interval_Per_Tick;
};

void Redirected_Move(float Unknown_Parameter, int Final)
{
	if (*(void**)607867332 != nullptr)
	{
		Global_Variables_Structure Previous_Global_Variables;

		Global_Variables_Structure* Global_Variables = *(Global_Variables_Structure**)607726732;

		Byte_Manager::Copy_Bytes(0, &Previous_Global_Variables, sizeof(Previous_Global_Variables), Global_Variables);

		//Redirected_Read_Packets(Final);

		using Fire_Events_Type = void(__cdecl*)();

		Fire_Events_Type(537149456)();

		Byte_Manager::Copy_Bytes(0, Global_Variables, sizeof(*Global_Variables), &Previous_Global_Variables);
	}

	(decltype(&Redirected_Move)(Original_Move_Caller))(Unknown_Parameter, Final);
}