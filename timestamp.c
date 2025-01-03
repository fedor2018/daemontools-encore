#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "fmt.h"
#include "taia.h"
#include "timestamp.h"

static char hex[16] = "0123456789abcdef";
struct tm *tm_info;
struct timespec tv;
char nsec_buf[11];

int fmt_tai64nstamp(char s[TIMESTAMP])
{
  struct taia now;
  char nowpack[TAIA_PACK];
  int i;

  taia_now(&now);
  taia_pack(nowpack,&now);

  s[0] = '@';
  for (i = 0;i < 12;++i) {
    s[i * 2 + 1] = hex[(nowpack[i] >> 4) & 15];
    s[i * 2 + 2] = hex[nowpack[i] & 15];
  }
  return 25;
}

int fmt_accustamp(char s[TIMESTAMP])
{
  struct taia now;
  int len;

  taia_now(&now);

  len = fmt_ulong(s,(unsigned long)tai_tounix(&now.sec));
  s[len++] = '.';
  len += fmt_uint0(s+len,now.nano / 1000,6);
  return len;
}

int readable_datetime(char s[DT])
{
  clock_gettime(CLOCK_REALTIME, &tv);
  tm_info = localtime(&tv.tv_sec);
  strftime(s, DT, "%F %T", tm_info);
  sprintf(nsec_buf, ".%09ld", tv.tv_nsec);
  strcat(s, nsec_buf);
  return DT;
}
