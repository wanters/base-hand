#include <string.h>
#include <ctype.h>
#include "usart.h"
#include "debug.h"
#include "bhky_errno.h"
#include "gps.h"
#include "string.h"
#include "usart.h"
#include "lan8720.h"
#include "oled.h"

DEVICE_INFO_TO_NET base_info_to_net;
DEVICE_INFO_TO_NET hand_info_to_net;

#define INDENT_SPACES "	"
#define GPS_BUFFER_LEN	128

typedef struct
{
	char  gps_buffer[GPS_BUFFER_LEN];
	uint8_t  gps_flag;
	uint8_t  gps_state;
	uint16_t gps_len;
} GPS_DATA_T;

typedef struct
{
	uint8_t flag;
	uint8_t success;
	uint8_t pmtk_line[255];
} PMTK_RESPONSE;

GPS_DATA_T gps_data;
PMTK_RESPONSE pmtk_response;

struct minmea_sentence_rmc nmea_rmc;
struct minmea_sentence_gga nmea_gga;
struct minmea_sentence_gst nmea_gst;
struct minmea_sentence_gsv nmea_gsv;
struct minmea_sentence_vtg nmea_vtg;
struct minmea_sentence_zda nmea_zda;

//接收数据帧检查
void gps_recv_check_frame(void)
{    
	uint8_t status = BHKY_SUCCESS;
	uint8_t recv_char = 0;
	
	if(gps_data.gps_flag == 0)
	{
		if(gps_data.gps_state == 0)
		{
			status = uart_read_buf_one_char(&uart_gps_buff, &recv_char);

			if (status == BHKY_SUCCESS)
			{
				if (recv_char == '$')
				{
					gps_data.gps_state = 1;
					gps_data.gps_len = 0;
				}
			}
		}
		else
		{
			do
			{
				status = uart_read_buf_one_char(&uart_gps_buff, &recv_char);

				if (status == BHKY_SUCCESS)
				{
					gps_data.gps_state = 2;
					gps_data.gps_buffer[gps_data.gps_len] = recv_char;
					gps_data.gps_len++;
				}
				else
				{
					gps_data.gps_state = 1;
					break;
				}
				
				if(gps_data.gps_len >= GPS_BUFFER_LEN)
				{
					gps_data.gps_state = 0;
					gps_data.gps_len = 0;
					break;
				}
			}while(recv_char != '\r' && recv_char != '\n');

			if(gps_data.gps_state == 2)
			{
				gps_data.gps_flag = 1;
				gps_data.gps_buffer[gps_data.gps_len] = 0;
				gps_data.gps_len = 0;
				gps_data.gps_state = 0;
			}
		}
	}
}

void nema_sentence_handle(const char *line)
{
	switch (minmea_sentence_id_parse(line, false)) 
	{
        case MINMEA_SENTENCE_RMC: {
			#if 0
            if (minmea_parse_rmc(&nmea_rmc, line)) {
				#if 0
                GPS_INFO(INDENT_SPACES "$xxRMC: raw coordinates and speed: (%d/%d,%d/%d) %d/%d\n",
                        nmea_rmc.latitude.value, nmea_rmc.latitude.scale,
                        nmea_rmc.longitude.value, nmea_rmc.longitude.scale,
                        nmea_rmc.speed.value, nmea_rmc.speed.scale);
                GPS_INFO(INDENT_SPACES "$xxRMC fixed-point coordinates and speed scaled to three decimal places: (%d,%d) %d\n",
                        minmea_rescale(&nmea_rmc.latitude, 1000),
                        minmea_rescale(&nmea_rmc.longitude, 1000),
                        minmea_rescale(&nmea_rmc.speed, 1000));
                GPS_INFO(INDENT_SPACES "$xxRMC floating point degree coordinates and speed: (%f,%f) %f\n",
                        minmea_tocoord(&nmea_rmc.latitude),
                        minmea_tocoord(&nmea_rmc.longitude),
                        minmea_tofloat(&nmea_rmc.speed));
                        
                GPS_INFO(INDENT_SPACES "UTC:%d : %d : %d : %d\n", nmea_rmc.time.hours, nmea_rmc.time.minutes, nmea_rmc.time.seconds, nmea_rmc.time.microseconds);
                GPS_INFO(INDENT_SPACES "date:%d : %d : %d\n", nmea_rmc.date.year, nmea_rmc.date.month, nmea_rmc.date.day);
				//minmea_gettime(NULL, &nmea_rmc.date, &nmea_rmc.time);
                GPS_INFO(INDENT_SPACES "position:%f : %f\n", minmea_tocoord(&nmea_rmc.longitude), minmea_tocoord(&nmea_rmc.latitude));
				GPS_INFO(INDENT_SPACES "speed:%f\n", minmea_tofloat(&nmea_rmc.speed));
				GPS_INFO(INDENT_SPACES "valid:%d\n", nmea_rmc.valid);
				#endif
            }
            else {
                GPS_INFO(INDENT_SPACES "$xxRMC sentence is not parsed\n");
            }
			#endif
        } break;

        case MINMEA_SENTENCE_GGA: {
		#if 1
            if (minmea_parse_gga(&nmea_gga, line)) {
				#if 0
                printf(INDENT_SPACES "$xxGGA: fix quality: %d\n", nmea_gga.fix_quality);
                printf(INDENT_SPACES "<utc time>: %d : %d : %d : %d\n", nmea_gga.time.hours, nmea_gga.time.minutes, nmea_gga.time.seconds, nmea_gga.time.microseconds);
                printf(INDENT_SPACES "<long - lat>: %f : %f\n", minmea_tocoord(&nmea_gga.longitude), minmea_tocoord(&nmea_gga.latitude));
				printf(INDENT_SPACES "<alt>: %f\n", minmea_tocoord(&nmea_gga.altitude));
				#endif
			}
            else {
                GPS_INFO(INDENT_SPACES "$xxGGA sentence is not parsed\n");
            }
		#endif
        } break;

        case MINMEA_SENTENCE_GST: {
		#if 0
            if (minmea_parse_gst(&nmea_gst, line)) {
                GPS_INFO(INDENT_SPACES "$xxGST: raw latitude,longitude and altitude error deviation: (%d/%d,%d/%d,%d/%d)\n",
                        nmea_gst.latitude_error_deviation.value, nmea_gst.latitude_error_deviation.scale,
                        nmea_gst.longitude_error_deviation.value, nmea_gst.longitude_error_deviation.scale,
                        nmea_gst.altitude_error_deviation.value, nmea_gst.altitude_error_deviation.scale);
                GPS_INFO(INDENT_SPACES "$xxGST fixed point latitude,longitude and altitude error deviation"
                       " sca9led to one decimal place: (%d,%d,%d)\n",
                        minmea_rescale(&nmea_gst.latitude_error_deviation, 10),
                        minmea_rescale(&nmea_gst.longitude_error_deviation, 10),
                        minmea_rescale(&nmea_gst.altitude_error_deviation, 10));
                GPS_INFO(INDENT_SPACES "$xxGST floating point degree latitude, longitude and altitude error deviation: (%f,%f,%f)",
                        minmea_tofloat(&nmea_gst.latitude_error_deviation),
                        minmea_tofloat(&nmea_gst.longitude_error_deviation),
                        minmea_tofloat(&nmea_gst.altitude_error_deviation));
            }
            else {
                GPS_INFO(INDENT_SPACES "$xxGST sentence is not parsed\n");
            }
		#endif
        } break;

        case MINMEA_SENTENCE_GSV: {
		#if 0
            if (minmea_parse_gsv(&nmea_gsv, line)) {
                GPS_INFO(INDENT_SPACES "$xxGSV: message %d of %d\n", nmea_gsv.msg_nr, nmea_gsv.total_msgs);
                GPS_INFO(INDENT_SPACES "$xxGSV: sattelites in view: %d\n", nmea_gsv.total_sats);
				int i;
                for (i = 0; i < 4; i++)
                    GPS_INFO(INDENT_SPACES "$xxGSV: sat nr %d, elevation: %d, azimuth: %d, snr: %d dbm\n",
                        nmea_gsv.sats[i].nr,
                        nmea_gsv.sats[i].elevation,
                        nmea_gsv.sats[i].azimuth,
                        nmea_gsv.sats[i].snr);
            }
            else {
                GPS_INFO(INDENT_SPACES "$xxGSV sentence is not parsed\n");
            }
		#endif
        } break;

        case MINMEA_SENTENCE_VTG: {
		#if 0
			if (minmea_parse_vtg(&nmea_vtg, line)) {
				 GPS_INFO(INDENT_SPACES "$xxVTG: true track degrees = %f\n",
						minmea_tofloat(&nmea_vtg.true_track_degrees));
				 GPS_INFO(INDENT_SPACES " 	   magnetic track degrees = %f\n",
						minmea_tofloat(&nmea_vtg.magnetic_track_degrees));
				 GPS_INFO(INDENT_SPACES " 	   speed knots = %f\n",
						 minmea_tofloat(&nmea_vtg.speed_knots));
				 GPS_INFO(INDENT_SPACES " 	   speed kph = %f\n",
						 minmea_tofloat(&nmea_vtg.speed_kph));
			}
			else {
				 GPS_INFO(INDENT_SPACES "$xxVTG sentence is not parsed\n");
			}
		#endif
        } break;

        case MINMEA_SENTENCE_ZDA: {
		#if 0
            if (minmea_parse_zda(&nmea_zda, line)) {
                GPS_INFO(INDENT_SPACES "$xxZDA: %d:%d:%d %02d.%02d.%d UTC%+03d:%02d\n",
                       nmea_zda.time.hours,
                       nmea_zda.time.minutes,
                       nmea_zda.time.seconds,
                       nmea_zda.date.day,
                       nmea_zda.date.month,
                       nmea_zda.date.year,
                       nmea_zda.hour_offset,
                       nmea_zda.minute_offset);
            }
            else {
                GPS_INFO(INDENT_SPACES "$xxZDA sentence is not parsed\n");
            }
		#endif
        } break;

        case MINMEA_INVALID: {
            GPS_INFO(INDENT_SPACES "$xxxxx sentence is not valid\n");
        } break;

        default: {
            GPS_INFO(INDENT_SPACES "$xxxxx sentence is not parsed\n");
        } break;
    }	
}

//处理收到的gps数据帧
void gps_process_line(const char *line)
{
    if (pmtk_response.flag == 1)//发送了配置命令
    {
		if (strstr(line, "PMTK"))
		{

		}
		else
		{
			nema_sentence_handle(line);
		}
    }
    else
    {
		nema_sentence_handle(line);
    }
}	

void gps_init(void)
{
	memset(&gps_data, 0, sizeof(GPS_DATA_T));
}

void pack_device_info(DEVICE_INFO *device_info)
{
	NET_FRAME net_frame;
	DEVICE_INFO device_info_base;
	
	net_frame.head = 0xAABB1122;
	net_frame.type = NET_DEVICE_INFO;
	net_frame.length = sizeof(device_info_base);
	if(device_info == NULL)//基站信息
	{
		device_info_base.device_id = BASE_DEVICE_ID;
		device_info_base.device_type = BASE_TYPE;
		device_info_base.longitude = minmea_tocoord(&nmea_gga.longitude);
		device_info_base.latitude = minmea_tocoord(&nmea_gga.latitude);
		device_info_base.altitude = minmea_tocoord(&nmea_gga.altitude);
		
		if(device_info_base.longitude>180.0f && device_info_base.longitude<-180.0f)
		{
			nmea_gga.fix_quality = 0;
		}
			
		if(device_info_base.latitude>90.0f && device_info_base.latitude<-90.0f)
		{
			nmea_gga.fix_quality = 0;
		}
		
		if(nmea_gga.fix_quality > 0)
		{
			device_info_base.device_status = 1;
		}
		else
		{
			device_info_base.device_status = 0;
		}
		
		memcpy(base_info_to_net.buffer, (uint8_t *)&net_frame, 12);
		base_info_to_net.length = 12;
		memcpy(base_info_to_net.buffer+base_info_to_net.length, (uint8_t *)&device_info_base, sizeof(DEVICE_INFO));
		base_info_to_net.length += sizeof(DEVICE_INFO);
	}
	else//手持设备信息
	{
		device_info_base.device_id = device_info->device_id;
		device_info_base.device_type = device_info->device_type;
		device_info_base.longitude = device_info->longitude;
		device_info_base.latitude = device_info->latitude;
		device_info_base.altitude = device_info->altitude;
		
		if(device_info_base.longitude>180.0f && device_info_base.longitude<-180.0f)
		{
			device_info->device_status = 0;
		}
			
		if(device_info_base.latitude>90.0f && device_info_base.latitude<-90.0f)
		{
			device_info->device_status = 0;
		}
		
		device_info_base.device_status = device_info->device_status;
		
		memcpy(hand_info_to_net.buffer, (uint8_t *)&net_frame, 12);
		hand_info_to_net.length = 12;
		memcpy(hand_info_to_net.buffer+hand_info_to_net.length, (uint8_t *)&device_info_base, sizeof(DEVICE_INFO));
		hand_info_to_net.length += sizeof(DEVICE_INFO);
		
		#if 0
		printf("repot to pc: \n");
		for(uint8_t i=0; i<hand_info_to_net.length; i++)
		{
			printf("%02X ", hand_info_to_net.buffer[i]);
		}
		printf("\n");
		printf("device_info_base.longitude:		%f\n", device_info_base.longitude);		
		#endif 
	}

	#if 0
	printf("device_info_base.device_id:		%08X\n", device_info_base.device_id);
	printf("device_info_base.device_type:	%08X\n", device_info_base.device_type);
	printf("device_info_base.longitude:		%f\n", device_info_base.longitude);
	printf("device_info_base.latitude:		%f\n", device_info_base.latitude);	
	printf("device_info_base.altitude:		%f\n", device_info_base.altitude);
	printf("device_info_base.device_status:	%d\n", device_info_base.device_status);	
	#endif
}

void gps_process(void)
{
	GPS_INFO_T gps_info;
	gps_recv_check_frame();
	
	if(gps_data.gps_flag == 1)
	{
		#if 0
		uint16_t i;
		printf("\n### gps data ###\n");
		for(i=0; i<strlen(gps_data.gps_buffer); i++)
		{
			printf("%c", gps_data.gps_buffer[i]);
		}
		printf("\n");
		#endif
		gps_process_line(gps_data.gps_buffer);
		gps_data.gps_flag = 0;
		
		pack_device_info(NULL);
		
		gps_info.lon = minmea_tocoord(&nmea_gga.longitude);
		gps_info.lat = minmea_tocoord(&nmea_gga.latitude);	
		gps_info.status = nmea_gga.fix_quality;
		oled_display_control(SECOND_SCREEN, TYPE_GPS, &gps_info);
	}
}


