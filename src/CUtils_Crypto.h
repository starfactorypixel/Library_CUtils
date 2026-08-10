#pragma once
#include <inttypes.h>

void GetSerialNumber64(uint8_t *sn)
{
	const uint32_t *uid = (const uint32_t *)UID_BASE;
	
	uint32_t serial[2];
	serial[0] = uid[0] ^ uid[2];
	serial[1] = uid[1];
	memcpy(sn, serial, sizeof(serial));
}
