#include "date.h"
#include "gps.h"

#if 0
#define FEBRUARY		2
#define	STARTOFTIME		1970
#define SECDAY			86400L           /*  һ���ж���s */
#define SECYR			(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define	days_in_month(a) 	(month_days[(a) - 1])
extern struct GNRMC_Info GNRMC;
static int month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*utcתʱ���*/
uint32_t mktimev(struct GNRMC_Info *tm)
{
	if (0 >= (int) (tm->month -= 2)) {	/* 1..12 -> 11,12,1..10 */
		tm->month += 12;		/* Puts Feb last since it has leap day */
		tm->year -= 1;
	}

	return (((
		(uint32_t) (tm->year/4 - tm->year/100 + tm->year/400 + 367*tm->month/12 + tm->day) +
			tm->year*365 - 719499
	    )*24 + tm->hour /* now have hours */
	  )*60 + tm->Minute /* now have minutes */
	)*60 + tm->second; /* finally seconds */	 
}



void to_tm(uint32_t tim, struct GNRMC_Info * tm)
{
	register uint32_t    i;
	register long   hms, day;

	day = tim / SECDAY;			/* �ж����� */
	hms = tim % SECDAY;			/* �����ʱ�䣬��λs */

	/* Hours, minutes, seconds are easy */
	tm->hour = hms / 3600;
	tm->Minute = (hms % 3600) / 60;
	tm->second = (hms % 3600) % 60;

	/* Number of years in days */ /*�����ǰ��ݣ���ʼ�ļ������Ϊ1970��*/
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	tm->year = i;

	/* Number of months in days left */ /*���㵱ǰ���·�*/
	if (leapyear(tm->year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	tm->month = i;

	/* Days are what is left over (+1) from all that. *//*���㵱ǰ����*/
	tm->day = day + 1;
}

#endif

