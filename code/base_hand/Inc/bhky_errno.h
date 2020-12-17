/**
  ******************************************************************************
  * File Name          : errno.c
  * Description        : 
  *		∂®“Â¥ÌŒÛ¬Î
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 BHKY.
  * All rights reserved.</center></h2>
  *
  ******************************************************************************
  */
#ifndef __BHKY_ERRNO_H__
#define __BHKY_ERRNO_H__

#define COMMON_ERR(err)  (((0x00 << 6) | (err)) & 0xff)
#define HOSTIF_ERR(err)  (((0x01 << 6) | (err)) & 0xff)

/* General error code */
#define BHKY_SUCCESS                             COMMON_ERR(0x00)
#define BHKY_FAIL                                COMMON_ERR(0x01)
#define BHKY_BUSY                                COMMON_ERR(0x02)
#define BHKY_NULL_PTR                            COMMON_ERR(0x03)
#define BHKY_INVALID_PARAMETER                   COMMON_ERR(0x04)
#define BHKY_TIMEOUT                             COMMON_ERR(0x05)

/* Host interface error code */
#define BHKY_HOSTIF_OVER_BUFFER_SIZE             HOSTIF_ERR(0x00)
#define BHKY_HOSTIF_ADDRESS_MISMATCH             HOSTIF_ERR(0x01)
#define BHKY_HOSFIF_CHECKSUM_ERROR               HOSTIF_ERR(0x02)
#define BHKY_HOSTIF_CMD_NOT_SUPPORTED            HOSTIF_ERR(0x03)
#define BHKY_HOSTIF_CMD_NOT_PERMITED             HOSTIF_ERR(0x04)
#define BHKY_HOSTIF_CMD_PROTOCOL_INVALID         HOSTIF_ERR(0x05)
#define BHKY_HOSTIF_INVALID_PARAMETER            HOSTIF_ERR(0x06)


#endif 

