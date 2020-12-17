
#ifndef __GPS_H__
#define __GPS_H__

#include "stm32f4xx.h"
#include "minmea.h"
#include "communication.h"

extern struct minmea_sentence_rmc nmea_rmc;
extern struct minmea_sentence_gga nmea_gga;
extern struct minmea_sentence_gst nmea_gst;
extern struct minmea_sentence_gsv nmea_gsv;
extern struct minmea_sentence_vtg nmea_vtg;
extern struct minmea_sentence_zda nmea_zda;

void gps_recv_check_frame(void);
void gps_process_line(const char *line);
void gps_init(void);
void gps_process(void);
void pack_device_info(DEVICE_INFO *device_info);

#endif
