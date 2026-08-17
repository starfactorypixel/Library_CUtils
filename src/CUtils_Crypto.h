#pragma once
#include <inttypes.h>

#ifdef HAL_MODULE_ENABLED

void GetSerialNumber64(uint8_t *sn)
{
	const uint32_t *uid = (const uint32_t *)UID_BASE;
	
	uint32_t serial[2];
	serial[0] = uid[0] ^ uid[2];
	serial[1] = uid[1];
	memcpy(sn, serial, sizeof(serial));
}

#endif

#ifdef ESP32

void GetSerialNumber64(uint8_t *sn)
{
	uint8_t mac[6];
	esp_efuse_mac_get_default(mac);
	
	sn[0] = mac[0];
	sn[1] = mac[1];
	sn[2] = mac[2];
	sn[3] = mac[3];
	sn[4] = mac[4];
	sn[5] = mac[5];
	sn[6] = mac[0] ^ mac[2] ^ mac[4];
	sn[7] = mac[1] ^ mac[3] ^ mac[5];
}

#endif