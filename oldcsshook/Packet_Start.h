#pragma once
#include "Main.h"

void* Original_Packet_Start_Caller;
struct Sequence_Structure
{
	__int32 Network_Sequence_Number;

	__int32 Prediction_Sequence_Number;
};
Sequence_Structure Sequences[90];

void Redirected_Packet_Start(void* Unknown_Parameter_1, void* Unknown_Parameter_2, __int32 Outgoing_Sequence_Number)
{
	Sequence_Structure* Sequence = &Sequence[Outgoing_Sequence_Number % 90];

	if (Outgoing_Sequence_Number == Sequence->Network_Sequence_Number)
	{
		(decltype(&Redirected_Packet_Start)(Original_Packet_Start_Caller))(Unknown_Parameter_1, Unknown_Parameter_2, Sequence->Prediction_Sequence_Number);
	}
	else
	{
		(decltype(&Redirected_Packet_Start)(Original_Packet_Start_Caller))(Unknown_Parameter_1, Unknown_Parameter_2, Outgoing_Sequence_Number);
	}
}