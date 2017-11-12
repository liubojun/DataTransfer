#include "change_name.h"
#include "QsLog/ctkLog.h"
#pragma warning(disable:4996)
#pragma warning(disable:4267)
/*
 ** NAME
 **   change_name - changes a file name according a given rule
 **
 ** SYNOPSIS
 **   void change_name(char         *orig_file_name,
 **                    char         *filter,
 **                    char         *rename_to_rule,
 **                    char         *new_name,
 **                    int          *counter_fd,
 **                    unsigned int job_id)
 **
 ** DESCRIPTION
 **   'change_name' takes the 'filter' and analyses the 'orig_file_name'
 **   to chop them in to pieces. Then the pieces are glued together as
 **   described in 'rename_to_rule' and stored in to 'new_name'. To
 **   insert special terms in the new filename the '%' sign is
 **   interpreted as followed:
 **     %ax   - Alternateing character and x can be one of the following:
 **               b  - binary (0 or 1)
 **               dn - character alternates up to n decimal numbers
 **               hn - character alternates up to n hexadecimal numbers
 **     %*n   - address n-th asterisk
 **     %?n   - address n-th question mark
 **     %on   - n-th character from the original filename
 **     %On-m - range n to m of characters from original file
 **             name. n='^' - from the beginning
 **                   m='$' - to the end
 **     %n    - generate a 4 character unique number
 **     %tx   - insert the actual time (x = as strftime())
 **             a - short day "Tue",           A - long day "Tuesday",
 **             b - short month "Jan",         B - long month "January",
 **             d - day of month [01,31],      m - month [01,12],
 **             j - day of the year [001,366], y - year [01,99],
 **             Y - year 1997,                 H - hour [00,23],
 **             M - minute [00,59],            S - second [00,60],
 **             U - Unix time, number of seconds since 00:00:00 01/01/1970 UTC
 **     %T[+|-|*|/|%]xS|M|H|d - Time modifier
 **            |     |   |
 **            |     |   +----> Time unit: S - second
 **            |     |                     M - minute
 **            |     |                     H - hour
 **            |     |                     d - day
 **            |     +--------> Time modifier
 **            +--------------> How time modifier should be applied.
 **     %h    - insert the hostname
 **     %%    - the '%' sign
 **     \     - are ignored
 **
 ** RETURN VALUES
 **   None.
 **
 */

#include <stdio.h>           /*                                          */
#include <string.h>          /* strcpy(), strlen()                       */
#include <stdlib.h>          /* atoi(), getenv()                         */
#include <ctype.h>           /* isdigit()                                */
#include <sys/types.h>
#include <sys/stat.h>
# include <fcntl.h>
#include <errno.h>
#include <time.h>


//cts_ren_rule *CChangeName::rule;
//int CChangeName::no_of_rule_headers;
std::vector<cts_ren_rule> CChangeName::rule;

/*############################# change_name() ###########################*/
void CChangeName::cts_change_name(char         *orig_file_name,
                                  char         *filter,
                                  char         *rename_to_rule,
                                  char         *new_name,
                                  int          *counter_fd,
                                  unsigned int job_id)
{

    char   buffer[MAX_FILENAME_LENGTH],
           string[MAX_INT_LENGTH + 1],
           *ptr_asterix[10] =
    {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    },
    *ptr_questioner[50] =
    {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    },
    *ptr_oldname,
    *ptr_oldname_tmp,
    *ptr_filter,
    *ptr_filter_tmp,
    *ptr_rule = NULL,
     *ptr_newname = NULL,
      time_mod_sign = '+';
    int    alternate,
           count_asterix = 0,
           count_questioner = 0,
           tmp_count_questioner,
           act_asterix = 0,
           act_questioner = 0,
           i,
           number,
           start = 0,
           end,
           pattern_found = NO;
    time_t time_buf,
           time_modifier = 0;

    /* copy original filename to a temporary buffer */
    (void)strcpy(buffer, orig_file_name);

    ptr_oldname = ptr_oldname_tmp = buffer;
    ptr_filter  = ptr_filter_tmp  = filter;

    /* taking 'orig_file_name' in pieces like 'filter' */
    while (*ptr_filter != '\0')
    {
        switch (*ptr_filter)
        {
        case '*' : /* '*' in filter detected -> mark position in array */
            ptr_asterix[count_asterix++] = ptr_oldname;
            ptr_filter++;
            break;

        case '?' : /* '?' in filter -> skip one character in both words */
            /*                  and mark questioner              */
            ptr_filter++;
            ptr_questioner[count_questioner++] = ptr_oldname++;
            break;

        default  : /* search the char, found in filter, in oldname */
            pattern_found = NO;
            tmp_count_questioner = 0;

            /* mark the latest position */
            ptr_filter_tmp = ptr_filter;
            while (*ptr_filter_tmp != '\0')
            {
                /* position the orig_file_name pointer to the next      */
                /* place where filter pointer and orig_file_name pointer*/
                /* are equal                                            */
                while ((*ptr_oldname != *ptr_filter_tmp) &&
                        (*ptr_oldname != '\0'))
                {
                    ptr_oldname++;
                }
                /* mark the latest position */
                ptr_oldname_tmp = ptr_oldname;

                /* test the rest of the pattern */
                while ((*ptr_filter_tmp != '*') && (*ptr_filter_tmp != '\0'))
                {
                    if ((*ptr_filter_tmp == *ptr_oldname_tmp) ||
                            (*ptr_filter_tmp == '?'))
                    {
                        if (*ptr_filter_tmp == '?')
                        {
                            /* mark questioner */
                            ptr_questioner[count_questioner + tmp_count_questioner++] = ptr_oldname_tmp;
                        }
                        ptr_filter_tmp++;
                        ptr_oldname_tmp++;
                        pattern_found = YES;
                    }
                    else
                    {
                        pattern_found = NO;
                        break;
                    }
                }
                if ((pattern_found == YES) && (*ptr_filter_tmp == '\0') &&
                        (*ptr_oldname_tmp != '\0'))
                {
                    pattern_found = NO;
                }
                if (pattern_found == YES)
                {
                    /* mark end of string 'ptr_asterix[count_asterix]' */
                    /* and position the pointer to next char           */
                    *ptr_oldname = '\0';
                    ptr_oldname = ptr_oldname_tmp;
                    ptr_filter = ptr_filter_tmp;
                    count_questioner += tmp_count_questioner;
                    break;
                }
                else
                {
                    ptr_filter_tmp = ptr_filter;
                    ptr_oldname++;
                    if (tmp_count_questioner > 0)
                    {
                        tmp_count_questioner = 0;
                    }
                }
            }
            break;
        }
    }

#ifdef _DEBUG
    //system_log(INFO_SIGN, NULL, 0, "found %d *", count_asterix);
    //RunLog(LOG_INFO_LEVEL, "found %d *", count_asterix);

    for (i = 0; i < count_asterix; i++)
    {
        //system_log(INFO_SIGN, NULL, 0, "ptr_asterix[%d]=%s", i, ptr_asterix[i]);

        ////RunLog(LOG_INFO_LEVEL, "ptr_asterix[%d]=%s", i, ptr_asterix[i]);
    }
    //system_log(INFO_SIGN, NULL, 0, "found %d ?", count_questioner);
    ////RunLog(LOG_INFO_LEVEL, "found %d ?", count_questioner);

    for (i = 0; i < count_questioner; i++)
    {
        //system_log(INFO_SIGN, NULL, 0, "ptr_questioner[%d]=%c",
        //         i, *ptr_questioner[i]);
        //RunLog(LOG_INFO_LEVEL, "ptr_questioner[%d]=%c", i, *ptr_questioner[i]);
    }
#endif

    /* Create new_name as specified in rename_to_rule */
    ptr_rule = rename_to_rule;
    ptr_newname = new_name;
    while (*ptr_rule != '\0')
    {
        /* work trough the rule and paste the name */
        switch (*ptr_rule)
        {
        case '%' : /* locate the '%' -> handle the rule */
            ptr_rule++;
            switch (*ptr_rule)
            {
            case '*' : /* insert the addressed ptr_asterix */
                ptr_rule++;

                /* test for number */
                i = 0;
                while ((isdigit((int)(*ptr_rule))) && (i < MAX_INT_LENGTH))
                {
                    string[i++] = *ptr_rule++;
                }
                string[i] = '\0';
                number = atoi(string) - 1; /* human count from 1 and computer from 0 */
                if (number >= count_asterix)
                {
                    //system_log(ERROR_SIGN, __FILE__, __LINE__,
                    //"illegal '*' addressed: %d", number + 1);

                    QSLOG_ERROR(QString("illegal '*' addressed: %1").arg(number + 1));
                }
                else
                {
                    (void)strcpy(ptr_newname, ptr_asterix[number]);
                    ptr_newname += strlen(ptr_asterix[number]);
                }
                break;

            case '?' : /* insert the addressed ptr_questioner */
                ptr_rule++;

                /* test for a number */
                i = 0;
                while ((isdigit((int)(*ptr_rule))) && (i < MAX_INT_LENGTH))
                {
                    string[i++] = *ptr_rule++;
                }
                string[i] = '\0';
                number = atoi(string) - 1; /* human count from 1 and computer from 0 */
                if (number >= count_questioner)
                {
                    //system_log(ERROR_SIGN, __FILE__, __LINE__,
                    //         "illegal '?' addressed: %d (%s %s)",
                    //       number + 1, filter, rename_to_rule);

                    //RunLog(LOG_INFO_LEVEL, "illegal '?' addressed: %d (%s %s)",number + 1, filter, rename_to_rule);
                    QSLOG_ERROR(QString("illegal '?' addressed: %1 (%2 %3)").arg(number + 1).arg(filter).arg(rename_to_rule));

                }
                else
                {
                    *ptr_newname = *ptr_questioner[number];
                    ptr_newname++;
                }
                break;

            case 'o' : /* insert the addressed character from orig_file_name */
                ptr_rule++;

                /* test for a number */
                i = 0;
                while ((isdigit((int)(*ptr_rule))) && (i < MAX_INT_LENGTH))
                {
                    string[i++] = *ptr_rule++;
                }
                string[i] = '\0';
                number = atoi(string) - 1; /* human count from 1 and computer from 0 */
                *ptr_newname++ = *(orig_file_name + number);
                break;

            case 'O' : /* insert the addressed range of characters from orig_file_name */
                ptr_rule++;

                /* read the begin */
                if (*ptr_rule == '^') /* means start from the beginning */
                {
                    ptr_oldname = orig_file_name;
                    ptr_rule++;
                }
                else /* read the start number */
                {
                    i = 0;
                    while ((isdigit((int)(*ptr_rule))) && (i < MAX_INT_LENGTH))
                    {
                        string[i++] = *ptr_rule++;
                    }
                    string[i] = '\0';
                    start = atoi(string) - 1; /* human count from 1 and computer from 0 */
                    ptr_oldname = orig_file_name + start;
                }
                if (*ptr_rule == '-')
                {
                    /* skip the '-' */
                    ptr_rule++;
                    /* read the end */
                    if (*ptr_rule == '$') /* means up to the end */
                    {
                        (void)strcpy(ptr_newname, ptr_oldname);
                        ptr_newname += strlen(ptr_oldname);
                        ptr_rule++;
                    }
                    else
                    {
                        i = 0;
                        while ((isdigit((int)(*ptr_rule))) &&
                                (i < MAX_INT_LENGTH))
                        {
                            string[i++] = *ptr_rule++;
                        }
                        string[i] = '\0';
                        end = atoi(string) - 1; /* human count from 1 and computer from 0 */
                        number = end - start + 1;  /* including the first and last */
                        if (number < 0)
                        {
                            //system_log(WARN_SIGN, __FILE__, __LINE__,
                            //         "The start (%d) and end (%d) range do not make sense in rule %s. Start must be larger then end!",
                            //       rename_to_rule);

                            //RunLog(LOG_INFO_LEVEL,  "The start (%d) and end (%d) range do not make sense in rule %s. Start must be larger then end!",
                            //       rename_to_rule);
                            QSLOG_ERROR(QString("The start (%d) and end (%d) range do not make sense in rule %1. Start must be larger then end!").arg(rename_to_rule));
                        }
                        else
                        {
                            //my_strncpy(ptr_newname, ptr_oldname, number);
                            strncpy(ptr_newname, ptr_oldname, number);
                            ptr_newname += number;
                        }
                    }
                }
                else
                {
                    //system_log(WARN_SIGN, __FILE__, __LINE__,
                    //         "There is no end range specified for rule %s",
                    //       rename_to_rule);


                    QSLOG_ERROR(QString("There is no end range specified for rule %1").arg(rename_to_rule));
                }
                break;

            /* generate a unique number 4 characters */
            /*
                           case 'n' :
                              ptr_rule++;
                              if (*counter_fd == -1)
                              {
                                 if ((*counter_fd = open_counter_file(COUNTER_FILE)) < 0)
                                 {
                                    system_log(WARN_SIGN, __FILE__, __LINE__,
                                               "Failed to open counter file, ignoring %n.");
                                    break;
                                 }
                              }
                              next_counter(*counter_fd, &number);
                              (void)sprintf(ptr_newname, "%04d", number);
                              ptr_newname += 4;
                              break;
            */
            case 'h' : /* Insert local hostname. */
            {
                /*char hostname[40];

                ptr_rule++;
                if (gethostname(hostname, 40) == -1)
                {
                    char *p_hostname;

                    if ((p_hostname = getenv("HOSTNAME")) != NULL)
                    {
                        ptr_newname += sprintf(ptr_newname, "%s", p_hostname);
                    }
                }
                else
                {
                    ptr_newname += sprintf(ptr_newname, "%s", hostname);
                }*/
            }
            break;

            case 'T' : /* Time modifier */
            {
                int time_unit;

                ptr_rule++;
                switch (*ptr_rule)
                {
                case '+' :
                case '-' :
                case '*' :
                case '/' :
                case '%' :
                    time_mod_sign = *ptr_rule;
                    ptr_rule++;
                    break;
                default  :
                    time_mod_sign = '+';
                    break;
                }
                i = 0;
                while ((isdigit(*ptr_rule)) && (i < MAX_INT_LENGTH))
                {
                    string[i++] = *ptr_rule++;
                }
                if ((i > 0) && (i < MAX_INT_LENGTH))
                {
                    string[i] = '\0';
                    time_modifier = atoi(string);
                }
                else
                {
                    if (i == MAX_INT_LENGTH)
                    {
                        //system_log(WARN_SIGN, __FILE__, __LINE__,
                        //         "The time modifier specified for rule %s is to large.",
                        //       rename_to_rule);


                        QSLOG_ERROR(QString("The time modifier specified for rule %1 is to large.").arg(rename_to_rule));
                    }
                    else
                    {
                        //system_log(WARN_SIGN, __FILE__, __LINE__,
                        //         "There is no time modifier specified for rule %s",
                        //       rename_to_rule);

                        QSLOG_ERROR(QString("There is no time modifier specified for rule %1.").arg(rename_to_rule));

                    }
                    time_modifier = 0;
                }
                switch (*ptr_rule)
                {
                case 'S' : /* Second */
                    time_unit = 1;
                    ptr_rule++;
                    break;
                case 'M' : /* Minute */
                    time_unit = 60;
                    ptr_rule++;
                    break;
                case 'H' : /* Hour */
                    time_unit = 3600;
                    ptr_rule++;
                    break;
                case 'd' : /* Day */
                    time_unit = 86400;
                    ptr_rule++;
                    break;
                default :
                    time_unit = 1;
                    break;
                }
                if (time_modifier > 0)
                {
                    time_modifier = time_modifier * time_unit;
                }
            }
            break;

            case 't' : /* insert the actual time like strftime */
                time_buf = time(NULL);
                if (time_modifier > 0)
                {
                    switch (time_mod_sign)
                    {
                    case '-' :
                        time_buf = time_buf - time_modifier;
                        break;
                    case '*' :
                        time_buf = time_buf * time_modifier;
                        break;
                    case '/' :
                        time_buf = time_buf / time_modifier;
                        break;
                    case '%' :
                        time_buf = time_buf % time_modifier;
                        break;
                    case '+' :
                    default :
                        time_buf = time_buf + time_modifier;
                        break;
                    }
                }
                ptr_rule++;
                switch (*ptr_rule)
                {
                case 'a' : /* short day of the week 'Tue' */
                    number = strftime(ptr_newname, MAX_FILENAME_LENGTH,
                                      "%a", gmtime(&time_buf));
                    break;
                case 'A' : /* long day of the week 'Tuesday' */
                    number = strftime(ptr_newname, MAX_FILENAME_LENGTH,
                                      "%A", gmtime(&time_buf));
                    break;
                case 'b' : /* short month 'Jan' */
                    number = strftime(ptr_newname, MAX_FILENAME_LENGTH,
                                      "%b", gmtime(&time_buf));
                    break;
                case 'B' : /* month 'January' */
                    number = strftime(ptr_newname, MAX_FILENAME_LENGTH,
                                      "%B", gmtime(&time_buf));
                    break;
                case 'd' : /* day of month [01,31] */
                    number = strftime(ptr_newname, MAX_FILENAME_LENGTH,
                                      "%d", gmtime(&time_buf));
                    break;
                case 'j' : /* day of year [001,366] */
                    number = strftime(ptr_newname, MAX_FILENAME_LENGTH,
                                      "%j", gmtime(&time_buf));
                    break;
                case 'm' : /* month [01,12] */
                    number = strftime(ptr_newname, MAX_FILENAME_LENGTH,
                                      "%m", gmtime(&time_buf));
                    break;
                case 'y' : /* year 2 chars [01,99] */
                    number = strftime(ptr_newname, MAX_FILENAME_LENGTH,
                                      "%y", gmtime(&time_buf));
                    break;
                case 'Y' : /* year 4 chars 1997 */
                    number = strftime(ptr_newname, MAX_FILENAME_LENGTH,
                                      "%Y", gmtime(&time_buf));
                    break;
                case 'H' : /* hour [00,23] */
                    number = strftime(ptr_newname, MAX_FILENAME_LENGTH,
                                      "%H", gmtime(&time_buf));
                    break;
                case 'M' : /* minute [00,59] */
                    number = strftime(ptr_newname, MAX_FILENAME_LENGTH,
                                      "%M", gmtime(&time_buf));
                    break;
                case 'S' : /* second [00,59] */
                    number = strftime(ptr_newname, MAX_FILENAME_LENGTH,
                                      "%S", gmtime(&time_buf));
                    break;
                case 'U' : /* Unix time. */
#if SIZEOF_TIME_T == 4
                    number = sprintf(ptr_newname, "%ld",
#else
                    number = sprintf(ptr_newname, "%lld",
#endif
                                     (pri_time_t)time_buf);
                    break;
                default  : /* unknown character - ignore */
                    //system_log(WARN_SIGN, __FILE__, __LINE__,
                    //         "Illegal time option (%c) in rule %s",
                    //       *ptr_rule, rename_to_rule);


                    QSLOG_ERROR(QString("Illegal time option (%1) in rule %2").arg(*ptr_rule).arg(rename_to_rule));
                    number = 0;
                    break;
                }
                ptr_newname += number;
                ptr_rule++;
                break;

            case '%' : /* insert the '%' sign */
                *ptr_newname = '%';
                ptr_newname++;
                ptr_rule++;
                break;

            case 'a' : /* Insert an alternating character. */
                ptr_rule++;
                /*modify by tanxh
                       if ((alternate = get_alternate_number(job_id)) == INCORRECT)
                       {
                          alternate = 0;
                       }
                */
                alternate = 0;

                switch (*ptr_rule)
                {
                case 'b' : /* Binary */
                    if ((alternate % 2) == 0)
                    {
                        *ptr_newname = '0';
                    }
                    else
                    {
                        *ptr_newname = '1';
                    }
                    ptr_rule++;
                    ptr_newname++;
                    break;

                case 'd' : /* Decimal. */
                    ptr_rule++;
                    if (isdigit((int)*ptr_rule))
                    {
                        if ((alternate % (*ptr_rule - '0')) == 0)
                        {
                            *ptr_newname = '0';
                        }
                        else
                        {
                        }
                        ptr_newname++;
                    }
                    else
                    {
                        //system_log(WARN_SIGN, __FILE__, __LINE__,
                        //         "Illegal character (%c) found in rule %s",
                        //       *ptr_rule, rename_to_rule);

                        QSLOG_ERROR(QString("Illegal character (%1) found in rule %2").arg(*ptr_rule).arg(rename_to_rule));
                    }
                    break;

                default : /* Unknown character - ignore */
                    //system_log(WARN_SIGN, __FILE__, __LINE__,
                    //         "Illegal character (%c) found in rule %s",
                    //       *ptr_rule, rename_to_rule);

                    QSLOG_ERROR(QString("Illegal character (%1) found in rule %2").arg(*ptr_rule).arg(rename_to_rule));

                    ptr_rule++;
                    break;
                }
                break;

            default  : /* no action be specified, write an error message */
                //system_log(WARN_SIGN, __FILE__, __LINE__,
                //         "Illegal option (%d) in rule %s",
                //        *ptr_rule, rename_to_rule);

                QSLOG_ERROR(QString("Illegal character (%1) found in rule %2").arg(*ptr_rule).arg(rename_to_rule));
                ptr_rule++;
                break;
            }
            break;

        case '*' : /* locate the '*' -> insert the next 'ptr_asterix' */
            if (act_asterix == count_asterix)
            {
                //system_log(WARN_SIGN, NULL, 0,
                //         "can not paste more '*' as defined before -> ignored");

                ////RunLog(LOG_INFO_LEVEL, "can not paste more '*' as defined before -> ignored");
                QSLOG_ERROR(QString("can not paste more '*' as defined before -> ignored"));
                //system_log(DEBUG_SIGN, NULL, 0,
                //         "orig_file_name = %s | filter = %s | rename_to_rule = %s | new_name = %s",
                //       orig_file_name, filter, rename_to_rule, new_name);

                ////RunLog(LOG_INFO_LEVEL, "orig_file_name = %s | filter = %s | rename_to_rule = %s | new_name = %s",
                //       orig_file_name, filter, rename_to_rule, new_name);
            }
            else
            {
                (void)strcpy(ptr_newname, ptr_asterix[act_asterix]);
                ptr_newname += strlen(ptr_asterix[act_asterix]);
                act_asterix++;
            }
            ptr_rule++;
            break;

        case '?' : /* locate the '?' -> insert the next 'ptr_questioner' */
            if (act_questioner == count_questioner)
            {
                //system_log(WARN_SIGN, NULL, 0,
                //         "can not paste more '?' as defined before -> ignored");
                ////RunLog(LOG_INFO_LEVEL, "can not paste more '?' as defined before -> ignored");

                //system_log(DEBUG_SIGN, NULL, 0,
                //         "orig_file_name = %s | filter = %s | rename_to_rule = %s | new_name = %s",
                //       orig_file_name, filter, rename_to_rule, new_name);

                ////RunLog(LOG_INFO_LEVEL, "orig_file_name = %s | filter = %s | rename_to_rule = %s | new_name = %s",
                //       orig_file_name, filter, rename_to_rule, new_name);
            }
            else
            {
                *ptr_newname = *ptr_questioner[act_questioner];
                ptr_newname++;
                act_questioner++;
            }
            ptr_rule++;
            break;

        case '\\' : /* ignore */
            ptr_rule++;
            break;

        default  : /* found an ordinary character -> append it */
            *ptr_newname = *ptr_rule;
            ptr_newname++;
            ptr_rule++;
            break;
        }
    }
    *ptr_newname = '\0';

    return;
}



void CChangeName::init_rename_rules(const char *rule_file)
{
    rule.clear();
    QFile file(rule_file);
    if (!file.open(QIODevice::ReadOnly))
    {
        QSLOG_ERROR("open rule file failure.");
        return;
    }

    cts_ren_rule oRule;
    QTextStream stream(&file);
    while (!stream.atEnd())
    {
        QString strLine = stream.readLine().trimmed();
        if (strLine.startsWith("#") || strLine.isEmpty())
        {
            continue;
        }

        if (strLine.startsWith("[") && strLine.endsWith("]"))
        {
            memset(&oRule, 0, sizeof(oRule));
            strLine = strLine.mid(1, strLine.length() - 2);
            strcpy(oRule.header, strLine.toAscii().data());
        }
        else
        {
            QStringList bodys = strLine.split(" ");
            QStringList bodys2;
            foreach(QString each, bodys)
            {
                if (!each.trimmed().isEmpty())
                {
                    bodys2.append(each.trimmed());
                }
            }
            if (bodys2.size() == 2)
            {

                RT_ARRAY(oRule.filter, oRule.no_of_rules + 1,
                         bodys2.at(0).length() + 1, char);
                RT_ARRAY(oRule.rename_to, oRule.no_of_rules + 1,
                         bodys2.at(1).length() + 1, char);

                strcpy(oRule.filter[oRule.no_of_rules], bodys2.at(0).toAscii().data());
                strcpy(oRule.rename_to[oRule.no_of_rules], bodys2.at(1).toAscii().data());
                oRule.no_of_rules++;
            }

            rule.push_back(oRule);
            memset(&oRule, 0, sizeof(oRule));
        }


    }
}


std::string CChangeName::change_name(const char *orig_file_name, const char *prule)
{

    if (orig_file_name == NULL || prule == NULL)
    {
        QSLOG_ERROR("input orig_file_name or rule is not correct");
        return orig_file_name;
    }
    if (QString::fromLocal8Bit(prule) == QString::fromLocal8Bit("нч"))
    {
        return orig_file_name;
    }
    char wanted_rule[512] = { 0 };
    strcpy(wanted_rule, prule);

    int index = get_rule(wanted_rule, rule.size());
    if (index < 0)
    {
        QSLOG_ERROR("rename.rule is not correct config!");
        return orig_file_name;
    }
    char newfilename[512] = { 0 };
    char oriname[512] = { 0 };
    strcpy(oriname, orig_file_name);
    int iFd = 0;
    cts_change_name(oriname, rule[index].filter[0], rule[index].rename_to[0], newfilename, &iFd, 0);
    return newfilename;
}

std::vector<std::string> CChangeName::get_rules()
{
    std::vector<std::string> rules;
    rules.push_back("нч");
    for (size_t i = 0; i < rule.size(); ++i)
    {
        rules.push_back(rule[i].header);
    }

    return rules;
}

/*############################ get_rule() ###############################*/
int CChangeName::get_rule(char *wanted_rule, int no_of_rule_headers)
{
    size_t length;

    if ((length = strlen(wanted_rule)) < 1025)
    {
        char *rule_buffer;

        if ((rule_buffer = (char*)malloc(length + 1)) == NULL)
        {
            //system_log(ERROR_SIGN, __FILE__, __LINE__,
            //"malloc() error : %s", strerror(errno));

        }
        else
        {
            int  i = 0;
            char *ptr = wanted_rule;

            while ((*ptr != '\0') && (*ptr != ' ') &&
                    (*ptr != '\t') && (i != length))
            {
                rule_buffer[i] = *ptr;
                i++;
                ptr++;
            }
            rule_buffer[i] = '\0';

            for (i = 0; i < no_of_rule_headers; i++)
            {
                if (CHECK_STRCMP(rule[i].header, rule_buffer) == 0)
                {
                    free(rule_buffer);
                    return(i);
                }
            }
            free(rule_buffer);
        }
    }
    else
    {
        //system_log(ERROR_SIGN, __FILE__, __LINE__,
        //         "Refuse to allocate %d of memory for a rule identifier, limit is 1024",
        //       length);
    }

    return(INCORRECT);
}


