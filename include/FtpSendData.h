/*
 * FtpSendData.h
 *
 *  Created on: 2018. 11. 5.
 *      Author: jaeh
 */

#ifndef INCLUDE_FTPSENDDATA_H_
#define INCLUDE_FTPSENDDATA_H_

#include <user_config.h>
#include <SmingCore/SmingCore.h>

#define FTP_CTL_PORT	21

#define FTP_DATA_PORT_HIGH	200
#define FTP_DATA_PORT_LOW	240
//#define FTP_DATA_PORT	(FTP_DATA_PORT_HIGH * 256 + FTP_DATA_PORT_LOW )
#define FTP_DATA_PORT	20

typedef Delegate<int( TcpClient& client, int maxsize)> FtpClientDataDelegate;

//참조할 ftp commands URL : https://en.wikipedia.org/wiki/List_of_FTP_commands
//참조할 FTP REPLIES : https://www.w3.org/Protocols/rfc959/4_FileTransfer.html

void ftpctl_OnCompleted(TcpClient& client, bool successful);
void ftpctl_OnReadyToSend(TcpClient& client, TcpConnectionEvent sourceEvent);
bool ftpctl_OnReceive(TcpClient& client, char* buf, int size);

void ftpdata_OnCompleted(TcpClient& client, bool successful);
void ftpdata_OnReadyToSend(TcpClient& client, TcpConnectionEvent sourceEvent);
bool ftpdata_OnReceive(TcpClient& client, char* buf, int size);

extern TcpClient  ftpclient_ctl;
extern TcpClient  ftpclient_data;


enum
{
	FTP_CONNECTING = 0 ,
	FTP_USER,
	FTP_USER_REQUESTING,
	FTP_PASSWORD,
	FTP_PASSWORD_REQUESTING,
	FTP_SYST,
	FTP_SYST_REQUESTING,
	FTP_TYPEI,
	FTP_TYPEI_REQUESTING,
	FTP_PASV,
	FTP_PASV_REQUESTING,
	FTP_DATACHANNEL,
	FTP_DATACHANNEL_CONNECTING,
	FTP_STOR_FILENAME,
	FTP_STOR_FILENAME_REQUESTING,
	FTP_DATA_SEND,
	FTP_DATA_SEND_GOING,
	FTP_QUIT,
	FTP_QUIT_REQUESTING,
	FTP_DONE

};

class FtpSendData
{

public :
	char * pServer = nullptr;
	char * pUser = nullptr;
	char * pPassword = nullptr;
	char * pfilename = nullptr;

	char outBuf[128];
	char outCount = 0 ;
	int status = 0 ;
	IPAddress localip ;

	uint8_t  passivemodeipport[6] ;

	FtpClientDataDelegate psend  = nullptr;

public:
	FtpSendData()
	{

	}

	void FtpSetup(IPAddress ip , char* pserver, char* puser, char* ppassword, char* filename, FtpClientDataDelegate OnSend )
	{
		localip = ip ;
		pServer = pserver ;
		pUser = puser ;
		pPassword = ppassword ;
		outCount = 0 ;
		pfilename = filename ;
		psend = OnSend ;

		ftpclient_ctl.setTimeOut(USHRT_MAX);
		ftpclient_ctl.connect(pserver, FTP_CTL_PORT);

		status = FTP_CONNECTING ;
		return ;

	}

};

FtpSendData ftpsenddata ;


int findsubstring(char *pstring, char *psub)
{
    int i, j ;
    int lengthstr = strlen((const char *)pstring) ;
    int lengthsub = strlen((const char *)psub) ;
    if( lengthstr> 0  && lengthsub > 0 && lengthstr == lengthsub )
    {
    	if(strcmp(pstring,psub ) == 0 )
    		return 0 ;
    	else
    		return -1 ;
    }

    int found = 0  ;
    for(i = 0 ; i < (lengthstr - lengthsub) ; i ++)
    {
        int allfound =  1 ;
        for(j = 0 ; j < lengthsub ; j ++)
            if (pstring[i+j] != psub[j] )  allfound = 0 ;
        if (allfound == 1 )
        {
            found = 1 ;
            break ;
        }
    }
    if(found == 1 )
        return i ;
    else
        return -1 ;
}

void GetInterArrayFrom(uint8_t* parray)
{
	int index = 0 ;
	char* pbuf = ftpsenddata.outBuf ;
	int pos = findsubstring(pbuf, (char*)"(");
	pbuf = pbuf + pos + 1  ;

	pos = findsubstring(pbuf, (char*)",");
	pbuf[pos] = 0 ;
	parray[index++] = atoi(pbuf);
	pbuf = pbuf + pos + 1 ;

	pos = findsubstring(pbuf, (char*)",");
	pbuf[pos] = 0 ;
	parray[index++] = atoi(pbuf);
	pbuf = pbuf + pos + 1 ;

	pos = findsubstring(pbuf, (char*)",");
	pbuf[pos] = 0 ;
	parray[index++] = atoi(pbuf);
	pbuf = pbuf + pos + 1 ;

	pos = findsubstring(pbuf, (char*)",");
	pbuf[pos] = 0 ;
	parray[index++] = atoi(pbuf);
	pbuf = pbuf + pos + 1 ;

	pos = findsubstring(pbuf, (char*)",");
	pbuf[pos] = 0 ;
	parray[index++] = atoi(pbuf);
	pbuf = pbuf + pos + 1 ;

	pos = findsubstring(pbuf, (char*)")");
	pbuf[pos] = 0 ;
	parray[index++] = atoi(pbuf);
	pbuf = pbuf + pos + 1 ;


}



void ftpctl_OnCompleted(TcpClient& client, bool successful)
{
	debugf("ftpctl_OnCompleted\n");
}
void ftpctl_OnReadyToSend(TcpClient& client, TcpConnectionEvent sourceEvent)
{
	String strtemp ;
	int port;

	switch(ftpsenddata.status)
	{
	case FTP_USER:
		strtemp = String("USER ") + ftpsenddata.pUser + "\n";
		Serial.println(strtemp);
		ftpclient_ctl.writeString(strtemp);
		ftpsenddata.status = FTP_USER_REQUESTING ;
		break;
	case FTP_PASSWORD:
		strtemp = String("PASS ") + ftpsenddata.pPassword + "\n";
		Serial.println(strtemp);
		ftpclient_ctl.writeString(strtemp);
		ftpsenddata.status = FTP_PASSWORD_REQUESTING ;
		break;
	case FTP_SYST:
		strtemp = String("SYST\n") ;
		Serial.println(strtemp);
		ftpclient_ctl.writeString(strtemp);
		ftpsenddata.status = FTP_SYST_REQUESTING ;
		break;
	case FTP_TYPEI:
		strtemp = String("Type I\n") ;
		Serial.println(strtemp);
		ftpclient_ctl.writeString(strtemp);
		ftpsenddata.status = FTP_TYPEI_REQUESTING ;
		break;
//	case FTP_PORT:
//	{
//		strtemp = String("PORT (") + ftpsenddata.localip[0] + "," + ftpsenddata.localip[1] + "," + \
//				ftpsenddata.localip[2] + "," + ftpsenddata.localip[3] + "," + FTP_DATA_PORT_HIGH + "," + FTP_DATA_PORT_LOW + ")\n" ;
//		Serial.println(strtemp);
//		ftpclient_ctl.writeString(strtemp);
//		ftpsenddata.status = FTP_PORT_REQUESTING ;
//		break;
//	}
	case FTP_PASV:
		strtemp = String("PASV\n") ;
		Serial.println(strtemp);
		ftpclient_ctl.writeString(strtemp);
		ftpsenddata.status = FTP_PASV_REQUESTING ;
		break;
	case FTP_DATACHANNEL:
		Serial.println("FTP_DATACHANNEL");
		ftpclient_data.setTimeOut(USHRT_MAX);
		port = ftpsenddata.passivemodeipport[4]*256 + ftpsenddata.passivemodeipport[5] ;
		ftpclient_data.connect(ftpsenddata.pServer, port);
		ftpsenddata.status = FTP_DATACHANNEL_CONNECTING ;
		break;
	case FTP_STOR_FILENAME:
		strtemp = String("STOR ")  + ftpsenddata.pfilename + "\n";
		Serial.println(strtemp);
		ftpclient_ctl.writeString(strtemp);
		ftpsenddata.status = FTP_STOR_FILENAME_REQUESTING ;
		break;
	case FTP_DATA_SEND:
		ftpsenddata.status = FTP_DATA_SEND_GOING ;
		break;
	case FTP_QUIT:
		strtemp = String("QUIT\n");
		Serial.println(strtemp);
		ftpclient_ctl.writeString(strtemp);
		ftpsenddata.status = FTP_QUIT_REQUESTING ;
		break;
	default:
		break;
	}

}
bool ftpctl_OnReceive(TcpClient& client, char* buf, int size)
{
	if(size > 128 )
		Serial.println("input size is over 128");
	os_memcpy(ftpsenddata.outBuf, buf, size );
	ftpsenddata.outCount = size ;
	debugf("OnReceive: %s",buf);

	if(ftpsenddata.status == FTP_PASV_REQUESTING)
		GetInterArrayFrom(ftpsenddata.passivemodeipport) ;

	ftpsenddata.status ++ ;
	return true;
}


void ftpdata_OnCompleted(TcpClient& client, bool successful)
{
	Serial.println("ftpdata_OnCompleted");
}
void ftpdata_OnReadyToSend(TcpClient& client, TcpConnectionEvent sourceEvent)
{
	Serial.println("ftpdata_OnReadyToSend");

	if(ftpsenddata.status == FTP_DATACHANNEL_CONNECTING  )
		ftpsenddata.status ++ ;
	else if(ftpsenddata.status == FTP_DATA_SEND_GOING && ftpsenddata.psend  )
	{
		Serial.println("Data Sending");
		int count = ftpsenddata.psend(client, 64 ) ;
		if(count < 64 )
			ftpsenddata.status = FTP_QUIT ;
	}

}
bool ftpdata_OnReceive(TcpClient& client, char* buf, int size)
{
	Serial.println("ftpdata_OnReceive");

	if(size > 128 )
		Serial.println("input size is over 128");
	os_memcpy(ftpsenddata.outBuf, buf, size );
	ftpsenddata.outCount = size ;
	debugf("OnReceive: %s",buf);

	ftpsenddata.status ++ ;

	return true;
}



TcpClient  ftpclient_ctl(ftpctl_OnCompleted, ftpctl_OnReadyToSend, ftpctl_OnReceive);
TcpClient  ftpclient_data(ftpdata_OnCompleted, ftpdata_OnReadyToSend, ftpdata_OnReceive);



#endif /* INCLUDE_FTPSENDDATA_H_ */
