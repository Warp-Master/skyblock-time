from collections import namedtuple
import time

seasons = (
    "Winter",
    "Spring",
    "Summer",
    "Autumn",
)

season_parts = (
    "Early",
    "Mid",
    "Late",
)

SkyBlockDate = namedtuple("SkyBlockDate", ("minute", "hour", "day", "month", "year"))

def get_skyblock_date(seconds: float) -> SkyBlockDate:
    year = int(seconds / 446_400) + 1
    # season_num = int(seconds / 111_600) % 4
    # season = seasons[season_num]
    month = int(seconds / 37_200 + 2) % 12 + 1
    day = int(seconds / 1200) % 31 + 1
    hour = int(seconds / 50) % 24
    minute = int(seconds % 50 * 60 / 50)
    return SkyBlockDate(minute, hour, day, month, year)

def get_num_suffix(n: int) -> str:
    if n // 10 == 1:
        return "th"
    if n % 10 == 1:
        return "st"
    elif n % 10 == 2:
        return "nd"
    elif n % 10 == 3:
        return "rd"
    return "th"

def get_season_name(month: int) -> str:
    return f"{season_parts[month % 3]} {seasons[month // 3 % 4]}"

def build_date_str(date: SkyBlockDate) -> str:
    day_suffix = get_num_suffix(date.day)
    season = get_season_name(date.month)
    return "{hour:02d}:{minute:02d}, {day}{day_suffix} of {season}({month}), year {year}".format(day_suffix=day_suffix, season=season, **date._asdict())

epoch_start = 1_560_275_700
# d = get_skyblock_date(0)
# print(build_date_str(d))
prev_msg_len = 0
while True:
    now = time.time()
    skyblock_sec = now - epoch_start
    skyblock_date = get_skyblock_date(skyblock_sec)
    msg = build_date_str(skyblock_date)
    print(f"\r{msg:<{prev_msg_len}}", end='')
    prev_msg_len = len(msg)
    try:
        time.sleep(50 / 60 - now % (50 / 60))
    except KeyboardInterrupt:
        print()
        exit()

