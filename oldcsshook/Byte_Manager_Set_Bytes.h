#pragma once

#ifdef Bits_32
void Set_Bytes(__int8 Modify_Access_Rights, void* Starting_Location, unsigned __int32 Bytes_Amount, unsigned __int8 Byte)
#else
void Set_Bytes(__int8 Modify_Access_Rights, void* Starting_Location, unsigned __int64 Bytes_Amount, unsigned __int8 Byte)
#endif
{
	unsigned long Previous_Access_Rights;

	if (Modify_Access_Rights == 1)
	{
		(Starting_Location, Bytes_Amount, &Previous_Access_Rights);
	}

	(Starting_Location, Byte, Bytes_Amount);

	if (Modify_Access_Rights == 1)
	{
		(Starting_Location, Bytes_Amount, Previous_Access_Rights, &Previous_Access_Rights);
	}
}