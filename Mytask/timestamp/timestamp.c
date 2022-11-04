
#include "timestamp.h"

#define UTC_BASE_YEAR   1970
#define MONTH_PER_YEAR  12
#define DAY_PER_YEAR    365
#define SEC_PER_DAY     86400
#define SEC_PER_HOUR    3600
#define SEC_PER_MIN     60

/* ÿ���µ����� */
const unsigned char day_per_mon[MONTH_PER_YEAR] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
	�ж��Ƿ�Ϊ����
*/
static uint8_t is_leap_year(uint16_t year)
{
	if ((year % 400) == 0) 
	{
			return 1;
	} 
	else if ((year % 100) == 0) 
	{
			return 0;
	} 
	else if ((year % 4) == 0) 
	{
			return 1;
	}
	else 
	{
			return 0;
	}
}

/*
 * ���ܣ�
 *     �õ�ÿ�����ж�����
 * ������
 *     month����Ҫ�õ��������·���
 *     year����������Ӧ�������
 *
 * ����ֵ��
 *     �����ж�����
 *
 */
static unsigned char applib_dt_last_day_of_mon(unsigned char month, unsigned short year)
{
    if ((month == 0) || (month > 12)) {
 //       return day_per_mon[1] + is_leap_year(year);
			return 0;
    }

    if (month != 2) {
        return day_per_mon[month - 1];
    } else {
        return day_per_mon[1] + is_leap_year(year);
    }
}

/*utcʱ����ת��������ʱ�� ���
*/
void time_stamp_2_utc(long ts)
{
	uint8_t month = 0;
	uint8_t day   = 0;
	uint8_t hour  = 0;
	uint8_t min   = 0;
	uint8_t sec   = 0;
	uint32_t year = 0;
	
	//��ʱ���ֵת����������ͨ���������ԱȽϷ��������ꡢ�¡��ա�
	int days = ts / SEC_PER_DAY;
	//���ʱ���ֵ���ꡣ
	int yearTmp = 0;
	int dayTmp = 0;
	
	//ʹ�üбƷ����� days ���а�����������
	for (yearTmp = UTC_BASE_YEAR; days > 0; yearTmp++) {
			dayTmp = (DAY_PER_YEAR + is_leap_year(yearTmp)); //��һ���ж����죿
			if (days >= dayTmp) //������������ yearTmp �������ʱ���ֵ�������������
			{
				 days -= dayTmp;
			}
			else
			{
				 break;
			}
	}
	year = yearTmp;

	//���ʱ���ֵ����
	int monthTmp = 0;
	for (monthTmp = 1; monthTmp < MONTH_PER_YEAR; monthTmp++) {
		 dayTmp = applib_dt_last_day_of_mon(monthTmp, year);
		 if (days >= dayTmp) {
				 days -= dayTmp;
		 }
		 else
		 {
				 break;
		 }
	}
	month = monthTmp;

	day = days + 1;

	//ת�����롣
	int secs = ts % SEC_PER_DAY;
	//���ʱ���ֵ��Сʱ����
	hour = secs / SEC_PER_HOUR;
	//���ʱ���ֵ�ķ�������
	secs %= SEC_PER_HOUR;
	min = secs / SEC_PER_MIN;
	//���ʱ�������������
	sec = secs % SEC_PER_MIN;
//	printf("%d-%d-%d %d:%d:%d", year, month, day, hour, min, sec);
}

/***************************************************************************/
/*utcʱ����ת��������ʱ�� ������ұ���
*/
void stamp_2_time(long ts, rtc_t *time)
{
//	printf("ԭʼֵ%ld-%ld\r\n", ts, (long)time_2_time_stamp(time));
	//��ʱ���ֵת����������ͨ���������ԱȽϷ��������ꡢ�¡��ա�
	int days = ts / SEC_PER_DAY;
	//���ʱ���ֵ���ꡣ
	int yearTmp = 0;
	int dayTmp = 0;
	
	//ʹ�üбƷ����� days ���а�����������
	for (yearTmp = UTC_BASE_YEAR; days > 0; yearTmp++) {
			dayTmp = (DAY_PER_YEAR + is_leap_year(yearTmp)); //��һ���ж����죿
			if (days >= dayTmp) //������������ yearTmp �������ʱ���ֵ�������������
			{
				 days -= dayTmp;
			}
			else
			{
				 break;
			}
	}
	time->year = yearTmp - 2000;

	//���ʱ���ֵ����
	int monthTmp = 0;
	for (monthTmp = 1; monthTmp < MONTH_PER_YEAR; monthTmp++) {
		 dayTmp = applib_dt_last_day_of_mon(monthTmp, yearTmp);
		 if (days >= dayTmp) {
				 days -= dayTmp;
		 }
		 else
		 {
				 break;
		 }
	}
	time->month = monthTmp;

	time->day = days + 1;

	//ת�����롣
	int secs = ts % SEC_PER_DAY;
	//���ʱ���ֵ��Сʱ����
	time->hour = secs / SEC_PER_HOUR;
	//���ʱ���ֵ�ķ�������
	secs %= SEC_PER_HOUR;
	time->min = secs / SEC_PER_MIN;
	//���ʱ�������������
	time->sec = secs % SEC_PER_MIN;
//	printf("%d-%d-%d %d:%d:%d", time->year, time->month, time->day, time->hour, time->min, time->sec);

}

/**************************************************************************************/
//uint8_t DayOfMon[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
/* ʱ��תʱ��� */
/**************************************************************************************/  
//���ؾ���1970��1��1����
uint32_t time_2_time_stamp(rtc_t *date_time)
{
	uint16_t year, mon, day, hour, min, sec;
	
	year = date_time->year + 2000;
	mon  = date_time->month;
	day  = date_time->day;
	hour = date_time->hour;
	min  = date_time->min;
	sec  = date_time->sec;
	
	uint16_t i, cyear = 0;
	uint64_t count_day = 0;
	
	for(i = 1970; i < year; i++)
	{
		if(((i%4==0) && (i%100!=0)) || (i%400==0)) cyear++;   //����
	}
	count_day = cyear * 366 + (year - 1970 - cyear) * 365;   //����
	
  for(i = 1; i < mon; i++)
  {
    if((i==2) && (((year % 4==0)&&( year % 100!=0)) || ( year % 400 == 0)))
      count_day += 29;
    else
      count_day += day_per_mon[i-1];
  }
  count_day += (day - 1);
 
  count_day = count_day * SEC_PER_DAY + (unsigned long)hour * 3600 + (unsigned long)min * 60 + sec;
  return count_day;
}
/**********************************************************************************************************/
