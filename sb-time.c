#include <math.h>
#include <stdio.h>
#include <time.h>

#define EPOCH_START 1560275700

char *seasons[4] = {
    "Winter",
    "Spring",
    "Summer",
    "Autumn",
};

char *season_parts[3] = {
    "Early",
    "Mid",
    "Late",
};

struct SkyBlockDate {
    unsigned int year:12;
    unsigned int month:4;
    unsigned int day:5;
    unsigned int hour:5;
    unsigned int minute:6;
};

void set_skyblock_date(double seconds, struct SkyBlockDate *data);
void set_sleep_time(double seconds, struct timespec *tv);
double tv2double(struct timespec *tv);
const char *get_num_suffix(unsigned int n);
void set_season_name(unsigned int month, char *season_name);
int set_msg(struct SkyBlockDate *data, char *msg);

int main()
{
    struct timespec tv;
    char msg[50];
    int prev_len = 0;
    do
    {
        struct SkyBlockDate data;

        clock_gettime(CLOCK_REALTIME, &tv);
        tv.tv_sec -= EPOCH_START;
        double now = tv2double(&tv);
        set_skyblock_date(now, &data);
        int msg_len = set_msg(&data, msg);

        printf("\r%-*s", prev_len, msg);
        for (int i = prev_len; i > msg_len; i--) printf("\b");
        fflush(stdout);

        prev_len = msg_len;

        set_sleep_time(now, &tv);
    }
    while(nanosleep(&tv, NULL) != -1);
    return 0;
}

void set_skyblock_date(double seconds, struct SkyBlockDate *data)
{
    data->year = (unsigned int) seconds / 446400 + 1;
    data->month = ((unsigned int) seconds / 37200 + 2) % 12 + 1;
    data->day = (unsigned int) seconds / 1200 % 31 + 1;
    data->hour = (unsigned int) seconds / 50 % 24;
    data->minute = (unsigned int) (fmod(seconds, 50) * 60) / 50;
}

void set_sleep_time(double seconds, struct timespec *tv)
{
    tv->tv_sec = 0;
    tv->tv_nsec = (long) ((50. / 60 - fmod(seconds, 50. / 60)) * 1e9);
}

double tv2double(struct timespec *tv)
{
    return (double) tv->tv_sec + (double) tv->tv_nsec * 1e-9;
}

const char *get_num_suffix(unsigned int n)
{
    if (n / 10 == 1)
        return "th";
    switch (n % 10)
    {
        case 1:
            return "st";
        case 2:
            return "nd";
        case 3:
            return "rd";
        default:
            return "th";
    }
}

void set_season_name(unsigned int month, char *season_name)
{
    char *part = season_parts[month % 3];
    char *season = seasons[month / 3 % 4];
    sprintf(season_name, "%s %s", part, season);
}

int set_msg(struct SkyBlockDate *data, char *msg)
{
    const char *day_suffix = get_num_suffix(data->day);
    char season_name[12];
    set_season_name(data->month, season_name);
    int len = snprintf(msg, 50, "%02u:%02u, %u%.2s of %s(%u), Year %u", data->hour, data->minute, data->day, day_suffix, season_name, data->month, data->year);
    return len;
}
