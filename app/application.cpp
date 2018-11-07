
#include <user_config.h>
#include <SmingCore/SmingCore.h>

#include "FtpSendData.h"

// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
#define WIFI_SSID "IMUWIFI" // Put you SSID and Password here
#define WIFI_PWD "12345678"
#endif

#define NARODM_HOST "192.168.0.200"
#define NARODM_PORT 33381
bool state = true;
uint64_t  rtcDiff ;

Timer MsgTimer;
Timer procTimer;
String mac;
IPAddress localip ;

char *pdata = (char*)"12345678901234567890123456789=" ;

void sendData()
{

//	t1 += 1.39;
//	ftpclient_ctl.connect(NARODM_HOST, NARODM_PORT);

	ftpclient_ctl.write(pdata, 30,TCP_WRITE_FLAG_COPY );
}

void OutMsg()
{
//	digitalWrite(LED_PIN, state);
//	state = !state;

	Serial.printf("rtc diff = %d \n", rtcDiff) ;
}


void connectOk(String ssid, uint8_t ssid_len, uint8_t bssid[6], uint8_t channel)
{
	// debug msg
	debugf("I'm CONNECTED to WiFi");


	mac = WifiStation.getMAC();

	mac.toUpperCase();

	for(int i = 2; i < mac.length(); i += 2)
		mac = mac.substring(0, i) + "-" + mac.substring(i++);

	debugf("mac: %s", mac.c_str());
}

void connectFail(String ssid, uint8_t ssid_len, uint8_t bssid[6], uint8_t reason)
{
	debugf("I'm NOT CONNECTED!");
}

int OnFtpDataSend(TcpClient& client, int maxsize )
{
	client.write(pdata, 30,TCP_WRITE_FLAG_COPY );
	return sizeof(pdata) ;
}


void gotIP(IPAddress ip, IPAddress netmask, IPAddress gateway)
{
	Serial.println("GotIP");
	localip = ip ;
	Serial.println(ip.toString()) ;

//	ftpclient_ctl.setTimeOut(USHRT_MAX) ;
//	ftpclient_ctl.connect(NARODM_HOST, NARODM_PORT);
//	procTimer.initializeMs(1, sendData).start();
//	MsgTimer.initializeMs(2000, OutMsg).start();

	ftpsenddata.FtpSetup(ip, (char*)NARODM_HOST, (char*)"jaehyek", (char*)".", (char*)"ftptest.txt", OnFtpDataSend);
}

void init()
{

	Serial.begin(115200);
	Serial.systemDebugOutput(true);
	Serial.println("Hello friendly world! :)");


	WifiAccessPoint.enable(false);

	WifiStation.enableDHCP(false);
	WifiStation.config(WIFI_SSID, WIFI_PWD);
	WifiStation.enable(true);


	WifiEvents.onStationConnect(connectOk);
	WifiEvents.onStationDisconnect(connectFail);
	WifiEvents.onStationGotIP(gotIP);
}
