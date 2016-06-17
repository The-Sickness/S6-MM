#!/system/bin/sh

#Setup Mhz Min/Max Cluster 0
echo 400000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq;
echo 1500000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq;
echo 400000 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq;
echo 1500000 > /sys/devices/system/cpu/cpu1/cpufreq/scaling_max_freq;
echo 400000 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq;
echo 1500000 > /sys/devices/system/cpu/cpu2/cpufreq/scaling_max_freq;
echo 400000 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq;
echo 1500000 > /sys/devices/system/cpu/cpu3/cpufreq/scaling_max_freq;

#Setup Mhz Min/Max Cluster 1
echo 800000 > /sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq;
echo 2100000 > /sys/devices/system/cpu/cpu4/cpufreq/scaling_max_freq;
echo 800000 > /sys/devices/system/cpu/cpu5/cpufreq/scaling_min_freq;
echo 2100000 > /sys/devices/system/cpu/cpu5/cpufreq/scaling_max_freq;
echo 800000 > /sys/devices/system/cpu/cpu6/cpufreq/scaling_min_freq;
echo 2100000 > /sys/devices/system/cpu/cpu6/cpufreq/scaling_max_freq;
echo 800000 > /sys/devices/system/cpu/cpu7/cpufreq/scaling_min_freq;
echo 2100000 > /sys/devices/system/cpu/cpu7/cpufreq/scaling_max_freq;

#e/frandom permissions
chmod 444 /dev/erandom
chmod 444 /dev/frandom

#Fix GPS Wake Issues. From LSpeed Mod 
mount -o remount,rw /
mount -o remount,rw rootfs
mount -o remount,rw /system
busybox mount -o remount,rw /
busybox mount -o remount,rw rootfs
busybox mount -o remount,rw /system

busybox sleep 40
su -c "pm enable com.google.android.gms/.update.SystemUpdateActivity"
su -c "pm enable com.google.android.gms/.update.SystemUpdateService"
su -c "pm enable com.google.android.gms/.update.SystemUpdateService$ActiveReceiver"
su -c "pm enable com.google.android.gms/.update.SystemUpdateService$Receiver"
su -c "pm enable com.google.android.gms/.update.SystemUpdateService$SecretCodeReceiver"
su -c "pm enable com.google.android.gsf/.update.SystemUpdateActivity"
su -c "pm enable com.google.android.gsf/.update.SystemUpdatePanoActivity"
su -c "pm enable com.google.android.gsf/.update.SystemUpdateService"
su -c "pm enable com.google.android.gsf/.update.SystemUpdateService$Receiver"
su -c "pm enable com.google.android.gsf/.update.SystemUpdateService$SecretCodeReceiver"

#Setup sickness file location if it doesn't exist already
[ ! -d "/data/data/sickness" ] && mkdir /data/data/sickness
chmod 755 /data/data/sickness

# init.d support
/system/xbin/busybox run-parts /system/etc/init.d

# Apollo Minfreq
CFILE="/data/data/sickness/APminfreq"
if [ -f $CFILE ]; then 
	FREQ=`cat $CFILE`
	echo $FREQ > /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq
	echo $FREQ > /sys/devices/system/cpu/cpu1/cpufreq/scaling_min_freq
	echo $FREQ > /sys/devices/system/cpu/cpu2/cpufreq/scaling_min_freq
	echo $FREQ > /sys/devices/system/cpu/cpu3/cpufreq/scaling_min_freq
fi

# Apollo Maxfreq
CFILE="/data/data/sickness/APmaxfreq"
if [ -f $CFILE ]; then 
	FREQ=`cat $CFILE`
	echo $FREQ > /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq
	echo $FREQ > /sys/devices/system/cpu/cpu1/cpufreq/scaling_max_freq
	echo $FREQ > /sys/devices/system/cpu/cpu2/cpufreq/scaling_max_freq
	echo $FREQ > /sys/devices/system/cpu/cpu3/cpufreq/scaling_max_freq
fi	
# Atlas Minfreq
CFILE="/data/data/sickness/ATminfreq"
if [ -f $CFILE ]; then 
	FREQ=`cat $CFILE`
	echo $FREQ > /sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq
	echo $FREQ > /sys/devices/system/cpu/cpu5/cpufreq/scaling_min_freq
	echo $FREQ > /sys/devices/system/cpu/cpu6/cpufreq/scaling_min_freq
	echo $FREQ > /sys/devices/system/cpu/cpu7/cpufreq/scaling_min_freq
fi

# Atlas Maxfreq
CFILE="/data/data/sickness/ATmaxfreq"
if [ -f $CFILE ]; then 
	FREQ=`cat $CFILE`
	echo $FREQ > /sys/devices/system/cpu/cpu4/cpufreq/scaling_max_freq
	echo $FREQ > /sys/devices/system/cpu/cpu5/cpufreq/scaling_max_freq
	echo $FREQ > /sys/devices/system/cpu/cpu6/cpufreq/scaling_max_freq
	echo $FREQ > /sys/devices/system/cpu/cpu7/cpufreq/scaling_max_freq
fi
	
#PEWQ's
CFILE="/data/data/sickness/PEWQ"
SFILE="/sys/module/workqueue/parameters/power_efficient"
[ -f $CFILE ] && echo `cat $CFILE` > $SFILE

#FSync
CFILE="/data/data/sickness/SHWL"
SFILE="/sys/module/wakeup/parameters/enable_sensorhub_wl"
[ -f $CFILE ] && echo `cat $CFILE` > $SFILE

#Task Packing
CFILE="/data/data/sickness/packing"
SFILE="/sys/kernel/hmp/packing_enable"
[ -f $CFILE ] && echo `cat $CFILE` > $SFILE




