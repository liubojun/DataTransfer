#ifndef JulianDate_h__
#define JulianDate_h__




#define MIN_IN_HOR 60
#define SEC_IN_MIN 60
#define HOR_IN_DAY 24

class CDate
{
private:
    int year;	// 年
    int month;	// 月
    int date;	// 日

public:
    CDate(int year, int month, int date)
    {
        this->year = year;
        this->month = month;
        this->date = date;
    }

    CDate(long Jdate)
    {
        long i, j, l, n;

        long JdNum;
        JdNum = Jdate+2440587;

        l = JdNum + 68569;
        n = ( 4 * l ) / 146097;
        l = l - ( 146097 * n + 3 ) / 4;
        i = ( 4000 * ( l + 1 ) ) / 1461001;
        l = l - ( 1461 * i ) / 4 + 31;
        j = ( 80 * l ) / 2447;
        date = l - ( 2447 * j ) / 80;
        l = j / 11;
        month = j + 2 - ( 12 * l );
        year = 100 * ( n - 49 ) + i + l;
    }

    int GetYear() const
    {
        return year;
    }

    int GetMonth() const
    {
        return month;
    }

    int GetDate() const
    {
        return date;
    }

    int GetDay() const
    {
        return (GetJdNum()+1)%7;
    }

    long GetJdNum() const
    {
        if(year==0 || month==0 || date==0)
            return -1;

        long a = (month-14)/12;
        long Julian = (1461*(year+4800+a))/4+
                      (367*(month-2-12*(a)))/12-
                      (3*((year+4900+a)/100))/4+
                      date-32075;
        return Julian;
    }

    CDate& operator += (int days2add)
    {
        long JdNum = GetJdNum()+days2add;
        long i, j, l, n;

        l = JdNum + 68569;
        n = ( 4 * l ) / 146097;
        l = l - ( 146097 * n + 3 ) / 4;
        i = ( 4000 * ( l + 1 ) ) / 1461001;
        l = l - ( 1461 * i ) / 4 + 31;
        j = ( 80 * l ) / 2447;
        date = l - ( 2447 * j ) / 80;
        l = j / 11;
        month = j + 2 - ( 12 * l );
        year = 100 * ( n - 49 ) + i + l;
        return *this;
    }

    CDate& operator -= (int days2sub)
    {
        return operator += (-days2sub);
    }

    /*
    *	short int years; //完整的公元数值
    *	short int months;//月份
    *	short int days;	 //日期
    */
    long GetJulianDate()  //这个计算的Julian日是以1970/07/01为起点的
    {
        if(year==0 || month==0 || date==0)
            return -1;

        short int passedRuyears;				//70年1月1日开始经历的闰年
        short int passedDays;					//70年1月1日开始经历的天数
        short int daysPerMonth[13]= {0,31,28,31,30,
                                     31,30,31,31,30,
                                     31,30,31
                                    };	//非闰年每月的天数

        passedDays=0;

        if(year%400==0||(year%4==0 && year%100!=0))
        {
            passedRuyears=(year-1968)/4-1;
            daysPerMonth[2]=29;
            for(int i=0; i<month; i++)
            {
                passedDays+=daysPerMonth[i];
            }
            passedDays+=date;
        }
        else
        {
            passedRuyears=(year-1968)/4;
            for(int i=0; i<month; i++)
            {
                passedDays+=daysPerMonth[i];
            }
            passedDays+=date;
        }

        passedDays+=(passedRuyears*366+(year-1970-passedRuyears)*365);
        return passedDays;
    }
};

#endif // JulianDate_h__

