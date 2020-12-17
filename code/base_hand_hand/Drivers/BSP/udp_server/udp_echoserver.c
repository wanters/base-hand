/**
  ******************************************************************************
  * @file    udp_echoserver.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   UDP echo server
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "ip_addr.h"
#include <string.h>
#include <stdio.h>
#include "udp_echoserver.h"
#include "usart.h"
#include "lan8720.h"


#undef  UDP_SERVER_PORT
#define UDP_SERVER_PORT 8089

uint16_t client_port;
uint8_t receivebuf[1000];
struct ip4_addr remoteaddr;
int receivelen = 0;

struct udp_pcb *upcb;

void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const struct ip4_addr *addr, u16_t port);


void udp_echoserver_init(void)
{
  err_t err;
   
   /* Create a new UDP control block  */
   upcb = udp_new();
   
   if (upcb)
   {
     /* Bind the upcb to the UDP_PORT port */
     /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
      err = udp_bind(upcb, IP_ADDR_ANY, UDP_SERVER_PORT);
      
      if(err == ERR_OK)
      {
        /* Set a receive callback for the upcb */
        udp_recv(upcb, udp_echoserver_receive_callback, NULL);
      }
      else
      {
        udp_remove(upcb);
        printf("can not bind pcb");
      }
   }
   else
   {
     printf("can not create pcb");
   } 
}

/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p,const struct ip4_addr *addr, u16_t port)
{
	int data_len = 0;
	remoteaddr = *addr;
	client_port = port;
	struct pbuf *q;
	
	if(p != NULL)
	{
		memset(receivebuf, 0, 1000); //数据接收缓存区清0
		for(q=p; q!=NULL; q=q->next)
		{
			memcpy(receivebuf+data_len,q->payload,q->len);
			data_len += q->len;
		}
		pbuf_free(p);
		
		#if 0
			uint16_t i;
			printf("Recv data:\n");
			for(i=0; i<data_len; i++)
			{
				printf("%02X ", receivebuf[i]);
			}
			printf("\n");
		#endif
		net_message_analysis(receivebuf);
	}
	else
	{
		printf("close\n");
	}
}

//向udp服务器发送数据
void udp_senddata(struct udp_pcb *upcb, char *str, int nSize)
{
	struct pbuf *ptr;
	
	udp_connect(upcb, &remoteaddr, client_port);
	ptr = pbuf_alloc(PBUF_TRANSPORT,nSize,PBUF_POOL);
	if(ptr)
	{
		ptr->payload = (void*)str;
	}
	udp_send(upcb, ptr);
	udp_disconnect(upcb);		//断开？？？
	pbuf_free(ptr);
	
	#if 0
	uint16_t i;
	printf("Send data:\n");
	for(i=0; i<nSize; i++)
	{
		printf("%02X ", str[i]);
	}
	printf("\n");
	#endif
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
