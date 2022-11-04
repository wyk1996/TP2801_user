
#include "timestamp.h"

#define UTC_BASE_YEAR   1970
#define MONTH_PER_YEAR  12
#define DAY_PER_YEAR    365
#define SEC_PER_DAY     86400
#define SEC_PER_HOUR    3600
#define SEC_PER_MIN     60

/* 每个月的天数 */
const unsigned char day_per_mon[MONTH_PER_YEAR] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/*
	判断是否为闰年
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
 * 功能：
 *     得到每个月有多少天
 * 参数：
 *     month：需要得到天数的月份数
 *     year：该月所对应的年份数
 *
 * 返回值：
 *     该月有多少天
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

/*utc时间秒转换成日历时间 输出
*/
void time_stamp_2_utc(long ts)
{
	uint8_t month = 0;
	uint8_t day   = 0;
	uint8_t hour  = 0;
	uint8_t min   = 0;
	uint8_t sec   = 0;
	uint32_t year = 0;
	
	//将时间戳值转化成天数。通过天数可以比较方便地算出年、月、日。
	int days = ts / SEC_PER_DAY;
	//这个时间戳值的年。
	int yearTmp = 0;
	int dayTmp = 0;
	
	//使用夹逼法计算 days 天中包含的年数。
	for (yearTmp = UTC_BASE_YEAR; days > 0; yearTmp++) {
			dayTmp = (DAY_PER_YEAR + is_leap_year(yearTmp)); //这一年有多少天？
			if (days >= dayTmp) //条件成立，则 yearTmp 即是这个时间戳值所代表的年数。
			{
				 days -= dayTmp;
			}
			else
			{
				 break;
			}
	}
	year = yearTmp;

	//这个时间戳值的月
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

	//转化成秒。
	int secs = ts % SEC_PER_DAY;
	//这个时间戳值的小时数。
	hour = secs / SEC_PER_HOUR;
	//这个时间戳值的分钟数。
	secs %= SEC_PER_HOUR;
	min = secs / SEC_PER_MIN;
	//这个时间戳的秒钟数。
	sec = secs % SEC_PER_MIN;
//	printf("%d-%d-%d %d:%d:%d", year, month, day, hour, min, sec);
}

/***************************************************************************/
/*utc时间秒转换成日历时间 输出并且保存
*/
void stamp_2_time(long ts, rtc_t *time)
{
//	printf("原始值%ld-%ld\r\n", ts, (long)time_2_time_stamp(time));
	//将时间戳值转化成天数。通过天数可以比较方便地算出年、月、日。
	int days = ts / SEC_PER_DAY;
	//这个时间戳值的年。
	int yearTmp = 0;
	int dayTmp = 0;
	
	//使用夹逼法计算 days 天中包含的年数。
	for (yearTmp = UTC_BASE_YEAR; days > 0; yearTmp++) {
			dayTmp = (DAY_PER_YEAR + is_leap_year(yearTmp)); //这一年有多少天？
			if (days >= dayTmp) //条件成立，则 yearTmp 即是这个时间戳值所代表的年数。
			{
				 days -= dayTmp;
			}
			else
			{
				 break;
			}
	}
	time->year = yearTmp - 2000;

	//这个时间戳值的月
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

	//转化成秒。
	int secs = ts % SEC_PER_DAY;
	//这个时间戳值的小时数。
	time->hour = secs / SEC_PER_HOUR;
	//这个时间戳值的分钟数。
	secs %= SEC_PER_HOUR;
	time->min = secs / SEC_PER_MIN;
	//这个时间戳的秒钟数。
	time->sec = secs % SEC_PER_MIN;
//	printf("%d-%d-%d %d:%d:%d", time->year, time->month, time->day, time->hour, time->min, time->sec);

}

/**************************************************************************************/
//uint8_t DayOfMon[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
/* 时间转时间戳 */
/**************************************************************************************/  
//返回距离1970年1月1日秒
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
		if(((i%4==0) && (i%100!=0)) || (i%400==0)) cyear++;   //闰年
	}
	count_day = cyear * 366 + (year - 1970 - cyear) * 365;   //天数
	
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
