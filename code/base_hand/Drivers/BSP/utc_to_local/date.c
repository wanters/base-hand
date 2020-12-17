#include "date.h"
#include "gps.h"

#if 0
#define FEBRUARY		2
#define	STARTOFTIME		1970
#define SECDAY			86400L           /*  一天有多少s */
#define SECYR			(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)
#define	days_in_month(a) 	(month_days[(a) - 1])
extern struct GNRMC_Info GNRMC;
static int month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*utc转时间戳*/
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

	day = tim / SECDAY;			/* 有多少天 */
	hms = tim % SECDAY;			/* 今天的时间，单位s */

	/* Hours, minutes, seconds are easy */
	tm->hour = hms / 3600;
	tm->Minute = (hms % 3600) / 60;
	tm->second = (hms % 3600) % 60;

	/* Number of years in days */ /*算出当前年份，起始的计数年份为1970年*/
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	tm->year = i;

	/* Number of months in days left */ /*计算当前的月份*/
	if (leapyear(tm->year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	tm->month = i;

	/* Days are what is left over (+1) from all that. *//*计算当前日期*/
	tm->day = day + 1;
}

#endif

