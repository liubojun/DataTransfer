#ifndef __CTS_RENAME_H__
#define __CTS_RENAME_H__

#include "../macro.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <vector>

using namespace std;

#define INFO_SIGN                  "<I>"
#define CONFIG_SIGN                "<C>"   /* If changed see:            */
/*           config_log.c     */
/*           mconfig_log.c    */
#define WARN_SIGN                  "<W>"
#define ERROR_SIGN                 "<E>"
#define FATAL_SIGN                 "<F>"   /* donated by Paul Merken.    */
#define DEBUG_SIGN                 "<D>"
#define TRACE_SIGN                 "<T>"
#define DUMMY_SIGN                 "<#>"   /* To always show some general*/
/* information, eg month.     */
#define SEPARATOR                  "-->"

/* Separator to separate elements in log files. */
#define SEPARATOR_CHAR             '|'

#define INCORRECT                  -1
#define SUCCESS                    0

/* Definitions for renaming */
#define MAX_PATH_LENGTH        256    /* Maximum length of a pathname.*/
#define MAX_FILENAME_LENGTH        256    /* Maximum length of a filename.*/
#define READ_RULES_INTERVAL        30          /* in seconds             */
#define MAX_RULE_HEADER_LENGTH     50
#define MAX_INT_LENGTH             11     /* When storing integer values  */
/* as string this is the no.    */
/* characters needed to store   */
/* the largest integer value.   */

typedef long                pri_time_t;

struct cts_ren_rule
{
    int  no_of_rules;
    char header[MAX_RULE_HEADER_LENGTH + 1];
    char **filter;
    char **rename_to;
};

#define YES 1
#define NO  0

/* Runtime array */
#define RT_ARRAY(name, rows, columns, type)                              \
        {                                                                \
           int macro_row_counter;                                        \
                                                                         \
           if ((name = (type **)calloc((rows), sizeof(type *))) == NULL) \
		   {                                                             \
              return;                                 \
		   }                                                             \
                                                                         \
           if (((name)[0] = (type *)calloc(((rows) * (columns)),         \
                                           sizeof(type))) == NULL)       \
		   {                                                             \
              return;                                           \
		   }                                                             \
                                                                         \
           for (macro_row_counter = 1; macro_row_counter < (rows); macro_row_counter++) \
              (name)[macro_row_counter] = ((name)[0] + (macro_row_counter * (columns)));\
        }
#define FREE_RT_ARRAY(name) \
        {                   \
           free((name)[0]); \
           free((name));    \
        }
#define REALLOC_RT_ARRAY(name, rows, columns, type)                         \
        {                                                                   \
           int  macro_row_counter;                                          \
           char *macro_ptr = (name)[0];                                     \
                                                                            \
           if (((name) = (type **)realloc((name), (rows) * sizeof(type *))) == NULL) \
           {                                                                \
              return;                                              \
           }                                                                \
                                                                            \
           if (((name)[0] = (type *)realloc(macro_ptr,                      \
                             (((rows) * (columns)) * sizeof(type)))) == NULL) \
           {                                                                \
              return;                                              \
           }                                                                \
                                                                            \
           for (macro_row_counter = 1; macro_row_counter < (rows); macro_row_counter++) \
              (name)[macro_row_counter] = ((name)[0] + (macro_row_counter * (columns)));\
        }
/* Runtime pointer array */
#define RT_P_ARRAY(name, rows, columns, type)                               \
        {                                                                   \
           register int macro_i;                                            \
                                                                            \
           if (((name) = (type ***)malloc(rows * sizeof(type **))) == NULL) \
           {                                                                \
              return;                                              \
           }                                                                \
           if (((name)[0] = (type **)malloc(rows * columns * sizeof(type *))) == NULL) \
           {                                                                \
              return;                                              \
           }                                                                \
           for (macro_i = 0; macro_i < rows; macro_i++)                     \
           {                                                                \
              (name)[macro_i] = (name)[0] + (macro_i * columns);            \
           }                                                                \
        }
#define FREE_RT_P_ARRAY(name)   \
        {                       \
           free((name));        \
           free((name)[0]);     \
        }

/* Macro that does do a strncpy without filling up rest with binary zeros. */
#define STRNCPY(dest, src, n)                          \
        {                                              \
           register int macro_i;                       \
                                                       \
           for (macro_i = 0; macro_i < (n); macro_i++) \
           {                                           \
              (dest)[macro_i] = (src)[macro_i];        \
              if ((src)[macro_i] == '\0')              \
              {                                        \
                 break;                                \
              }                                        \
           }                                           \
        }

/* Macro that positions pointer just after binary zero. */
#define NEXT(ptr)                 \
        {                         \
           while (*(ptr) != '\0') \
           {                      \
              (ptr)++;            \
           }                      \
           (ptr)++;               \
        }

#define SET_DIR_STATUS(flag, status)            \
        {                                       \
           if ((flag) & DIR_DISABLED)           \
           {                                    \
              (status) = DISABLED;              \
           }                                    \
           else if ((flag) & DIR_ERROR_SET)     \
                {                               \
                   (status) = NOT_WORKING2;     \
                }                               \
           else if ((flag) & WARN_TIME_REACHED) \
                {                               \
                   (status) = WARNING_ID;       \
                }                               \
                else                            \
                {                               \
                   (status) = NORMAL_STATUS;    \
                }                               \
        }

/* Macro to check if we can avoid a strcmp. */
#define CHECK_STRCMP(a, b)  (*(a) != *(b) ? (int)((unsigned char) *(a) - (unsigned char) *(b)) : strcmp((a), (b)))

class DLL_EXPORT_CLASS_DECL CChangeName
{
public:
    // 必须首先调用该方法完成初始化
    static void init_rename_rules(const char *rule_file);

    static std::string change_name_by_name(const char *orig_file_name, const char *rule);

    static std::string change_name_by_id(const char *orig_file_name, const char *ruleID);

    // 从文件中获取所有换名规则
    static std::vector<std::string> get_rules();

private:

    static int get_rule(char *wanted_rule, int no_of_rule_headers);

    static void cts_change_name(char         *orig_file_name,
                                char         *filter,
                                char         *rename_to_rule,
                                char         *new_name,
                                int          *counter_fd,
                                unsigned int job_id);

    //static cts_ren_rule *rule;
    static std::vector<cts_ren_rule> rule;
    //static int         no_of_rule_headers;
};

#endif
