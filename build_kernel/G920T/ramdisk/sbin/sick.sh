#!/system/bin/sh

BB=/system/xbin/busybox;

# Mount root as RW to apply tweaks and settings
$BB mount -t rootfs -o remount,rw rootfs
$BB mount -o remount,rw /system


# Init.d
if [ "$($BB mount | grep rootfs | cut -c 26-27 | grep -c ro)" -eq "1" ]; then
	$BB mount -o remount,rw /;
fi;
if [ "$($BB mount | grep system | grep -c ro)" -eq "1" ]; then
	$BB mount -o remount,rw /system;
fi;
if [ ! -d /system/etc/init.d ]; then
	mkdir -p /system/etc/init.d/;
	chown -R root.root /system/etc/init.d;
	chmod 777 /system/etc/init.d/;
	chmod 777 /system/etc/init.d/*;
fi;
$BB run-parts /system/etc/init.d


# Parse Knox reomve from prop
if [ "`grep "kernel.knox=true" $PROP`" != "" ]; then
	cd /system
	rm -rf *app/BBCAgent*
	rm -rf *app/Bridge*
	rm -rf *app/ContainerAgent*
	rm -rf *app/ContainerEventsRelayManager*
	rm -rf *app/DiagMonAgent*
	rm -rf *app/ELMAgent*
	rm -rf *app/FotaClient*
	rm -rf *app/FWUpdate*
	rm -rf *app/FWUpgrade*
	rm -rf *app/HLC*
	rm -rf *app/KLMSAgent*
	rm -rf *app/*Knox*
	rm -rf *app/*KNOX*
	rm -rf *app/LocalFOTA*
	rm -rf *app/RCPComponents*
	rm -rf *app/SecKids*
	rm -rf *app/SecurityLogAgent*
	rm -rf *app/SPDClient*
	rm -rf *app/SyncmlDM*
	rm -rf *app/UniversalMDMClient*
	rm -rf container/*Knox*
	rm -rf container/*KNOX*
fi

rm -rf $PROP

# Google play services wakelock fix
sleep 40
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

$BB mount -t rootfs -o remount,ro rootfs
$BB mount -o remount,ro /system
$BB mount -o remount,rw /data