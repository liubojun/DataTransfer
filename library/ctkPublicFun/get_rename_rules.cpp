
#include "change_name.h"
#include "ctkLog.h"
#include <QFile>
#include <QTextStream>

/*
 ** NAME
 **   get_rename_rules - reads rename rules from a file and stores them
 **                      in a shared memory area
 **
 ** SYNOPSIS
 **   void get_rename_rules(char *rule_file, int verbose)
 **
 ** DESCRIPTION
 **   This function reads the rule file and stores the contents in the
 **   global structure rule. The contents of the rule file looks as
 **   follows:
 **
 **      [T4-charts]
 **      *PGAH??_EGRR*     *waf_egr_nat_000_000*
 **      *PGCX??_EGRR*     *waf_egr_gaf_000_900*
 **
 **   Where [T4-charts] is here defined as a rule header. *PGAH??_EGRR*
 **   and *PGCX??_EGRR* are the filter/file-mask, while the rest is the
 **   part to which we want to rename.
 **   The  number of rule headers and rules is not limited.
 **
 **   The caller is responsible for freeing the memory for rule.filter
 **   and rule.rename_to.
 **
 ** RETURN VALUES
 **   None. It will exit with INCORRECT if it fails to allocate memory
 **   or fails to open the rule_file.
 **
 */

#include <stdio.h>
#include <string.h>          /* strcpy(), strerror()                     */
#include <stdlib.h>          /* calloc(), free()                         */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>



//
///*################################ posi() ###############################*/
//char *
//posi(char *search_text, char *search_string)
//{
//    int    hit = 0;
//    size_t string_length;
//
//    string_length = strlen(search_string);
//
//    while (*search_text != '\0')
//    {
//        if (*(search_text++) == *(search_string++))
//        {
//            if (++hit == string_length)
//            {
//                return(++search_text);
//            }
//        }
//        else
//        {
//            if ((hit == 1) &&
//                    (*(search_string - 2) == *(search_text - 1)))
//            {
//                search_string--;
//            }
//            else
//            {
//                search_string -= hit + 1;
//                hit = 0;
//            }
//        }
//    }
//
//    return(NULL); /* Found nothing */
//}
//
//
///*########################## get_rename_rules() #########################*/
//void CChangeName::get_rename_rules(const char *rule_file, int verbose)
//{
//    //RunLog(LOG_INFO_LEVEL, "enter the get_rename_rules");
//
//    static time_t last_read = 0;
//    static int    first_time = YES;
//    struct stat   stat_buf;
//
//    if (stat(rule_file, &stat_buf) == -1)
//    {
//        if (errno == ENOENT)
//        {
//            /*
//             * Only tell user once that the rules file is missing. Otherwise
//             * it is anoying to constantly receive this message.
//             */
//            if (first_time == YES)
//            {
//                if (verbose == YES)
//                {
//                    //system_log(INFO_SIGN, __FILE__, __LINE__,
//                    //         "There is no renaming rules file %s", rule_file);
//
//                    //RunLog(LOG_INFO_LEVEL, "There is no renaming rules file %s", rule_file);
//                }
//                first_time = NO;
//            }
//        }
//        else
//        {
//            //system_log(WARN_SIGN, __FILE__, __LINE__,
//            //         "Failed to stat() %s : %s", rule_file, strerror(errno));
//
//            QSLOG_ERROR(QString("Failed to stat() %1").arg(rule_file));
//        }
//    }
//    else
//    {
//        if (stat_buf.st_mtime != last_read)
//        {
//            register int i;
//            FILE          *fd;
//            char         *last_ptr,
//                         *ptr,
//                         *buffer;
//
//            if (first_time == YES)
//            {
//                first_time = NO;
//            }
//            else
//            {
//                if (verbose == YES)
//                {
//                    //system_log(INFO_SIGN, NULL, 0, "Rereading renaming rules file.");
//
//                    //RunLog(LOG_INFO_LEVEL, "Rereading renaming rules file.");
//                    QSLOG_DEBUG(QString("Rereading renaming rules file."));
//                }
//            }
//
//            if (last_read != 0)
//            {
//                /*
//                 * Since we are rereading the whole rules file again
//                 * lets release the memory we stored for the previous
//                 * structure of rule.
//                 */
//                for (i = 0; i < no_of_rule_headers; i++)
//                {
//                    if (rule[i].filter != NULL)
//                    {
//                        FREE_RT_ARRAY(rule[i].filter);
//                    }
//                    if (rule[i].rename_to != NULL)
//                    {
//                        FREE_RT_ARRAY(rule[i].rename_to);
//                    }
//                }
//                free(rule);
//                no_of_rule_headers = 0;
//            }
//            last_read = stat_buf.st_mtime;
//
//            /* Allocate memory to store file */
//            if ((buffer = (char*)malloc(1 + stat_buf.st_size + 1)) == NULL)
//            {
//                //system_log(FATAL_SIGN, __FILE__, __LINE__,
//                //        "malloc() error : %s", strerror(errno));
//                exit(INCORRECT);
//            }
//
//            /* Open file. */
//            if ((fd = fopen(rule_file, "rb")) == NULL)
//            {
//                //system_log(FATAL_SIGN, __FILE__, __LINE__,
//                //         "Failed to open() %s : %s", rule_file, strerror(errno));
//
//                QSLOG_ERROR(QString("Failed to open() %1").arg(rule_file));
//
//                free(buffer);
//                exit(INCORRECT);
//            }
//
//            /* Read file into buffer. */
//            buffer[0] = '\n';
//            if (fread(&buffer[1], 1, stat_buf.st_size, fd) != stat_buf.st_size)
//            {
//                //system_log(FATAL_SIGN, __FILE__, __LINE__,
//                //         "Failed to read() %s : %s", rule_file, strerror(errno));
//
//                QSLOG_ERROR(QString("Failed to read() %1").arg(rule_file));
//
//
//                free(buffer);
//                (void)fclose(fd);
//                exit(INCORRECT);
//            }
//            fclose(fd);
//
//            buffer[stat_buf.st_size] = '\0';
//
//            /*
//             * Now that we have the contents in the buffer lets first see
//             * how many rules there are in the buffer so we can allocate
//             * memory for the rules.
//             */
//            ptr = buffer;
//            last_ptr = buffer + stat_buf.st_size;
//            if (buffer[0] == '[')
//            {
//                no_of_rule_headers++;
//            }
//            while ((ptr = posi(ptr, "\n[")) != NULL)
//            {
//                no_of_rule_headers++;
//            }
//
//            if (no_of_rule_headers > 0)
//            {
//                register int j, k;
//                int          no_of_rules,
//                             max_rule_length,
//                             count,
//                             total_no_of_rules = 0;
//                char         *end_ptr,
//                             *search_ptr;
//
//                if ((rule = (cts_ren_rule*)calloc(no_of_rule_headers,
//                                                  sizeof(rule))) == NULL)
//                {
//                    QSLOG_ERROR(QString("calloc() error : %1").arg(strerror(errno)));
//                    exit(INCORRECT);
//                }
//                ptr = buffer;
//
//                for (i = 0; i < no_of_rule_headers; i++)
//                {
//                    if (((ptr == buffer) && (buffer[0] == '[')) ||
//                            ((ptr = posi(ptr, "\n[")) != NULL))
//                    {
//                        /*
//                         * Lets first determine how many rules there are.
//                         * This is simply done by counting the \n characters.
//                         * While doing this, lets also find the longest rule or
//                         * rename_to string.
//                         */
//                        if ((ptr == buffer) && (buffer[0] == '['))
//                        {
//                            ptr++;
//                        }
//                        no_of_rules = max_rule_length = 0;
//                        search_ptr = ptr;
//                        while (*search_ptr != '\n')
//                        {
//                            search_ptr++;
//                        }
//                        do
//                        {
//                            search_ptr++;
//
//                            /* Ignore any comments. */
//                            if (*search_ptr == '#')
//                            {
//                                while ((*search_ptr != '\n') && (*search_ptr != '\0'))
//                                {
//                                    search_ptr++;
//                                }
//                            }
//                            else
//                            {
//                                count = 0;
//                                while ((*search_ptr != ' ') && (*search_ptr != '\t') &&
//                                        (search_ptr < last_ptr))
//                                {
//                                    if ((*search_ptr == '\\') &&
//                                            (*(search_ptr + 1) == ' '))
//                                    {
//                                        count++;
//                                        search_ptr++;
//                                    }
//                                    count++;
//                                    search_ptr++;
//                                }
//                                if (search_ptr == last_ptr)
//                                {
//                                    break;
//                                }
//                                if (count > max_rule_length)
//                                {
//                                    max_rule_length = count;
//                                }
//                                while ((*search_ptr == ' ') || (*search_ptr == '\t'))
//                                {
//                                    search_ptr++;
//                                }
//                                count = 0;
//                                while ((*search_ptr != '\n') && (*search_ptr != '\0'))
//                                {
//                                    count++;
//                                    search_ptr++;
//                                }
//                                if (count > max_rule_length)
//                                {
//                                    max_rule_length = count;
//                                }
//                                no_of_rules++;
//                            }
//                        }
//                        while ((*(search_ptr + 1) != '\n') &&
//                                (*(search_ptr + 1) != '[') &&
//                                (*search_ptr != '\0') &&
//                                (*(search_ptr + 1) != '\0'));
//
//                        /* Allocate memory for filter and rename_to */
//                        /* part of struct rule.                     */
//                        rule[i].filter = NULL;
//                        RT_ARRAY(rule[i].filter, no_of_rules,
//                                 max_rule_length + 1, char);
//                        rule[i].rename_to = NULL;
//                        RT_ARRAY(rule[i].rename_to, no_of_rules,
//                                 max_rule_length + 1, char);
//
//                        ptr--;
//                        end_ptr = ptr;
//                        while ((*end_ptr != ']') && (*end_ptr != '\n') &&
//                                (*end_ptr != '\0'))
//                        {
//                            end_ptr++;
//                        }
//                        if ((end_ptr - ptr) <= MAX_RULE_HEADER_LENGTH)
//                        {
//                            if (*end_ptr == ']')
//                            {
//                                *end_ptr = '\0';
//                                (void)strcpy(rule[i].header, ptr);
//                                ptr = end_ptr + 1;
//
//                                while ((*ptr != '\n') && (*ptr != '\0'))
//                                {
//                                    ptr++;
//                                }
//                                if (*ptr == '\n')
//                                {
//                                    j = 0;
//
//                                    do
//                                    {
//                                        ptr++;
//                                        if (*ptr == '#') /* Ignore any comments. */
//                                        {
//                                            while ((*ptr != '\n') && (*ptr != '\0'))
//                                            {
//                                                ptr++;
//                                            }
//                                        }
//                                        else
//                                        {
//                                            k = 0;
//                                            end_ptr = ptr;
//                                            while ((*end_ptr != ' ') && (*end_ptr != '\t') &&
//                                                    (*end_ptr != '\n') && (*end_ptr != '\0'))
//                                            {
//                                                if ((*end_ptr == '\\') &&
//                                                        (*(end_ptr + 1) == ' '))
//                                                {
//                                                    end_ptr++;
//                                                }
//                                                rule[i].filter[j][k] = *end_ptr;
//                                                end_ptr++;
//                                                k++;
//                                            }
//                                            if ((*end_ptr == ' ') || (*end_ptr == '\t'))
//                                            {
//                                                /*
//                                                 * Store the filter part.
//                                                 */
//                                                rule[i].filter[j][k] = '\0';
//                                                end_ptr++;
//                                                while ((*end_ptr == ' ') || (*end_ptr == '\t'))
//                                                {
//                                                    end_ptr++;
//                                                }
//
//                                                /*
//                                                 * Store the renaming part.
//                                                 */
//                                                ptr = end_ptr;
//                                                k = 0;
//                                                while ((*end_ptr != ' ') && (*end_ptr != '\t') &&
//                                                        (*end_ptr != '\n') && (*end_ptr != '\0'))
//                                                {
//                                                    if ((*end_ptr == '\\') &&
//                                                            (*(end_ptr + 1) == ' '))
//                                                    {
//                                                        end_ptr++;
//                                                    }
//                                                    rule[i].rename_to[j][k] = *end_ptr;
//                                                    end_ptr++;
//                                                    k++;
//                                                }
//                                                rule[i].rename_to[j][k] = '\0';
//                                                if ((*end_ptr == ' ') || (*end_ptr == '\t'))
//                                                {
//                                                    int more_data = NO;
//
//                                                    end_ptr++;
//                                                    while ((*end_ptr != '\n') &&
//                                                            (*end_ptr != '\0'))
//                                                    {
//                                                        if ((more_data == NO) &&
//                                                                ((*end_ptr != ' ') || (*end_ptr != '\t')))
//                                                        {
//                                                            more_data = YES;
//                                                        }
//                                                        end_ptr++;
//                                                    }
//                                                    if (more_data == YES)
//                                                    {
//                                                        //system_log(WARN_SIGN, __FILE__, __LINE__,
//                                                        //         "In rule [%s] the rule %s %s has data after the rename-to-part. Ignoring it!",
//                                                        //       rule[i].header,
//                                                        //     rule[i].filter[j],
//                                                        //   rule[i].rename_to[j]);
//
//                                                        //RunLog(LOG_INFO_LEVEL, "In rule [%s] the rule %s %s has data after the rename-to-part. Ignoring it!",
//                                                        //rule[i].header, rule[i].filter[j],rule[i].rename_to[j]);
//
//                                                    }
//                                                }
//                                                ptr = end_ptr;
//                                                j++;
//                                            }
//                                            else
//                                            {
//                                                if (end_ptr != ptr)
//                                                {
//                                                    //system_log(WARN_SIGN, __FILE__, __LINE__,
//                                                    //         "A filter is specified for the rule header %s but not a rule.",
//                                                    //       rule[i].header);
//
//                                                    //RunLog(LOG_INFO_LEVEL, "A filter is specified for the rule header %s but not a rule.",
//                                                    //rule[i].header);
//
//                                                    ptr = end_ptr;
//                                                }
//                                                else if (*ptr == '\n')
//                                                {
//                                                    ptr++;
//                                                }
//                                            }
//                                        }
//                                    }
//                                    while ((*ptr == '\n') && (j < no_of_rules));
//
//                                    rule[i].no_of_rules = j;
//                                    total_no_of_rules += j;
//                                }
//                                else
//                                {
//                                    //system_log(WARN_SIGN, __FILE__, __LINE__,
//                                    //         "Rule header %s specified, but could not find any rules.",
//                                    //       rule[i].header);
//
//                                    //RunLog(LOG_INFO_LEVEL, "Rule header %s specified, but could not find any rules.",
//                                    //rule[i].header);
//                                }
//                            }
//                            else
//                            {
//                                //system_log(WARN_SIGN, __FILE__, __LINE__,
//                                //         "Failed to determine the end of the rule header.");
//
//                                ////RunLog(LOG_INFO_LEVEL, "Failed to determine the end of the rule header.");
//                            }
//                        }
//                        else
//                        {
//                            //system_log(WARN_SIGN, __FILE__, __LINE__,
//                            //         "Rule header to long. May not be longer then %d Bytes [MAX_RULE_HEADER_LENGTH].",
//                            //       MAX_RULE_HEADER_LENGTH);
//
//                            //RunLog(LOG_INFO_LEVEL, "Rule header to long. May not be longer then %d Bytes [MAX_RULE_HEADER_LENGTH].",MAX_RULE_HEADER_LENGTH);
//                        }
//                    }
//                    else
//                    {
//                        /* Impossible! We just did find it and now it's gone?!? */
//                        //system_log(DEBUG_SIGN, __FILE__, __LINE__,
//                        //         "Could not get start of rule header %d [%d].",
//                        //       i, no_of_rule_headers);
//
//                        //RunLog(LOG_INFO_LEVEL, "Could not get start of rule header %d [%d].", i, no_of_rule_headers);
//
//                        rule[i].filter = NULL;
//                        rule[i].rename_to = NULL;
//                        break;
//                    }
//                } /* for (i = 0; i < no_of_rule_headers; i++) */
//
//                if (verbose == YES)
//                {
//                    //system_log(INFO_SIGN, NULL, 0,
//                    //         "Found %d rename rule headers with %d rules.",
//                    //       no_of_rule_headers, total_no_of_rules);
//
//                    //RunLog(LOG_INFO_LEVEL, "Could not get start of rule header %d [%d].", i, no_of_rule_headers);
//                }
//            } /* if (no_of_rule_headers > 0) */
//            else
//            {
//                if (verbose == YES)
//                {
//                    //system_log(INFO_SIGN, NULL, 0,
//                    //         "No rename rules found in %s", rule_file);
//
//                    //RunLog(LOG_INFO_LEVEL, "No rename rules found in %s", rule_file);
//                }
//            }
//
//            /* The buffer holding the contents of the rule file */
//            /* is no longer needed.                             */
//            free(buffer);
//
////#ifdef _DEBUG_RULES
//            //{
//            register int j;
//
//            for (i = 0; i < no_of_rule_headers; i++)
//            {
//                //system_log(DEBUG_SIGN, NULL, 0, "[%s]", rule[i].header);
//                //RunLog(LOG_INFO_LEVEL, "[%s]", rule[i].header);
//
//                for (j = 0; j < rule[i].no_of_rules; j++)
//                {
//                    //system_log(DEBUG_SIGN, NULL, 0, "%s  %s",
//                    //         rule[i].filter[j], rule[i].rename_to[j]);
//                    //RunLog(LOG_INFO_LEVEL, "%s  %s", rule[i].filter[j], rule[i].rename_to[j]);
//
//                }
//            }
//        }
////#endif /* _DEBUG_RULES */
//        //} /* if (stat_buf.st_mtime != last_read) */
//    }
//
//    if (no_of_rule_headers <= 0)
//    {
//        QSLOG_ERROR("rename.rule is not correct config");
//    }
//    return;
//}
