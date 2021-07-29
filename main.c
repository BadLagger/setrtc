#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>

struct rtc_time {
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};

#define RTC_RD_TIME   _IOR('p', 0x09, struct rtc_time) /* Read RTC time   */
#define RTC_SET_TIME  _IOW('p', 0x0a, struct rtc_time) /* Set RTC time    */

extern int errno;

static char *rtc_file = "/dev/rtc";

static void print_example(const char *err_msg)
{
  printf("Error: %s\n", err_msg);
  printf("Format of input data: DD/MM/YYYY/HH:MM:SS\n");
  printf("Example of usage setrtc:\n\n");
  printf("setrtc 03/05/2021/14:06:55\n\n");
}

int main(int argn, char *argv[])
{
  int errnum = 0;
  char *str = 0;
  int counter = 0;
  int rtc_f = 0;
  int ret = 0;
  struct rtc_time rtc_data;


  printf("Call Setrtc!!\n");
  if(argn != 2) {
    print_example("wrong number of arguments");
    return 0;
  }

  str = argv[1];
  for(counter = 0; counter < 19; ++counter){
    if(counter == 2 || counter == 5 || counter == 10) {
      if(str[counter] != '/') {
        print_example("wrong input format");
        return 0;
      }
    }
    else
      if(counter == 13 || counter == 16) {
        if(str[counter] != ':'){
          print_example("wrong input format");
          return 0;   
        }
      }
      else
        if(isdigit(str[counter]) == 0) {
          print_example("wrong input format");
          return 0;
        }
  }

  sscanf(str, "%d/%d/%d/%d:%d:%d", &rtc_data.tm_mday, &rtc_data.tm_mon, &rtc_data.tm_year, 
                                   &rtc_data.tm_hour, &rtc_data.tm_min, &rtc_data.tm_sec);

  if( (rtc_data.tm_year < 2000) || (rtc_data.tm_year > 2079) ){
    print_example("year should be between 2000 and 2079\n");
    return 0;
  }

  rtc_data.tm_year -= 1900;
  rtc_data.tm_mon -= 1;

  do
  {
    rtc_f = open(rtc_file, O_RDONLY);
    if(rtc_f == -1) {
      errnum = errno;
      printf("RTC %s open error: %d - %s\n", rtc_file, errnum, strerror( errnum ) );
    }
  }while(rtc_f == -1);

  ret = ioctl(rtc_f, RTC_SET_TIME, &rtc_data);
  if(ret == -1) {
    errnum = errno;
    printf("RTC %s set error: %d - %s\n", rtc_file, errnum, strerror( errnum ));
  } else {
    printf("RTC successfully set\n");
  }
  
  close(rtc_f);

  return 0;
}
