#include <string.h>
#include "gps.h"
#include "usart.h"

int pmtk_test(void)
{
	//pmtk_send_command
	return 0;
}

int pmtk_hot_start(void)
{
	pmtk_send_command("$PMTK101*32");
	return 0;
}

int pmtk_warm_start(void)
{
	pmtk_send_command("$PMTK102*31");
	return 0;
}

int pmtk_cold_start(void)
{
	pmtk_send_command("$PMTK103*30");
	return 0;
}

int pmtk_full_cold_start(void)
{
	return 0;
}

int pmtk_clear_flash_aid(void)
{
	return 0;
}

int pmtk_clear_epo(void)
{
	return 0;
}

int pmtk_standby_mode(void)
{
	return 0;
}

int pmtk_set_periodic(void)
{
	return 0;
}

int pmtk_set_nmea_baudrate(void)
{
	return 0;
}

int pmtk_set_nmea_output(void)
{
	return 0;
}

int pmtk_set_stop_qzss(void)
{
	return 0;
}

int pmtk_set_gnss_search_mode(void)
{
	return 0;
}

int pmtk_set_hdop_threshold(void)
{
	return 0;
}

int pmtk_get_hdop_threshold(void)
{
	return 0;
}

int pmtk_set_static_nav_thd(void)
{
	return 0;
}

int pmtk_sbas_enabled(void)
{
	return 0;
}

int pmtk_release(void)
{
	return 0;
}

