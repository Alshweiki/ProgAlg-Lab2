# Edit this file to introduce tasks to be run by cron.
#
# For example, you can run a backup of all your user accounts
# at 5 a.m every week with:
# 0 5 * * 1 tar -zcf /var/backups/home.tgz /home/
# 
# For more information see the manual pages of crontab(5) and cron(8)
# 
# m h  dom mon dow   command
SHELL=/bin/bash
10 0 29 4 * /etuhome/ggroup20/project/POPC/MATRIX/runme.sh  2>&1 > /etuhome/ggroup20/project/POPC/MATRIX/cronlog.log
10 17 2 5 * /etuhome/ggroup20/project/POPC/MATRIX/minirun.sh  2>&1 >> /etuhome/ggroup20/project/POPC/MATRIX/cronlog.log
10 12 3 5 * /etuhome/ggroup20/project/POPC/MATRIX/runme.sh  2>&1 >> /etuhome/ggroup20/project/POPC/MATRIX/cronlog.log
10 13 6 5 * /etuhome/ggroup20/project/POPC/MATRIX/runme.sh  2>&1 >> /etuhome/ggroup20/project/POPC/MATRIX/cronlog_125.log
10 15 6 5 * /etuhome/ggroup20/project/POPC/MATRIX/runme.sh  2>&1 >> /etuhome/ggroup20/project/POPC/MATRIX/cronlog_125_1_1.log
10 16 6 5 * /etuhome/ggroup20/project/POPC/MATRIX/runme.sh  2>&1 >> /etuhome/ggroup20/project/POPC/MATRIX/cronlog_125_1_1.log
