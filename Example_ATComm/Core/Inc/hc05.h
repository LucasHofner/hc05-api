/*
 * 	API for HC-05 Module (hc05.c/hc05.h).
 * 	Authors: Camilla Gomes Fernandes
			 Lucas Guimarães Hofner
 *  This API was developed as an assignment for Embedded Systems
 *  Programming class at the Federal University of Minas Gerais
*/


#ifndef __HC05
#define __HC05

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------- INCLUDES --------------------------------------------------- //

#include "stm32f1xx_hal.h"
#include "string.h"


// ---------------------------------------------- DEFINES ---------------------------------------------------- //
// API Responses
#define OK "OK\r\n"
#define FAIL "FAIL\r\n"
// AT Commands
#define AT "AT\r\n"
#define AT_RESET "AT+RESET\r\n"
#define AT_VERSION "AT+VERSION?\r\n"
#define AT_ORGL "AT+ORGL\r\n"
#define AT_ADDR "AT+ADDR?\r\n"
#define AT_NAME "AT+NAME=" 
#define AT_NAME_ "AT+NAME?\r\n"
#define AT_RNAME "AT+RNAME?" 
#define AT_ROLE "AT+ROLE=" 
#define AT_ROLE_ "AT+ROLE?\r\n"
#define AT_CLASS "AT+CLASS=" 
#define AT_CLASS_ "AT+CLASS?\r\n"
#define AT_IAC "AT+IAC=" 
#define AT_IAC_ "AT+IAC\r\n"
#define AT_INQM "AT+INQM=" 
#define AT_INQM_ "AT+INQM?\r\n"
#define AT_PSWD "AT+PSWD=" 
#define AT_PSWD_ "AT+PSWD?\r\n"
#define AT_UART "AT+UART=" 
#define AT_UART_ "AT+UART?\r\n"
#define AT_CMODE "AT+CMODE=" 
#define AT_CMODE_ "AT+CMODE?\r\n"
#define AT_BIND "AT+BIND=" 
#define AT_BIND_ "AT+BIND?\r\n"
#define AT_POLAR "AT+POLAR="
#define AT_POLAR_ "AT+POLAR?\r\n"
#define AT_PIO "AT+PIO="
#define AT_IPSCAN "AT+IPSCAN="
#define AT_IPSCAN_ "AT+IPSCAN?\r\n"
#define AT_SNIFF "AT+SNIFF="
#define AT_SNIFF_ "AT+SNIFF?\r\n"
#define AT_SENM "AT+SENM="
#define AT_SENM_ "AT+SENM?\r\n"
#define AT_PMSAD "AT+PMSAD=" 
#define AT_RMAAD "AT+RMAAD\r\n"
#define AT_FSAD "AT+FSAD=" 
#define AT_ADCN "AT+ADCN?\r\n"
#define AT_MRAD "AT+MRAD?\r\n"
#define AT_STATE "AT+STATE?\r\n"
#define AT_INIT "AT+INIT\r\n"
#define AT_INQ "AT+INQ\r\n"
#define AT_INQC "AT+INQC\r\n"
#define AT_PAIR "AT+PAIR="
#define AT_LINK "AT+LINK=" 
#define AT_DISC "AT+DISC=" 
#define AT_ENSNIFF "AT+ENSNIFF=" 
#define AT_EXSNIFF "AT+EXSNIFF=" 
// AT Commands Suffix
#define AT_RESET_SUFFIX "+RESET"
#define AT_VERSION_SUFFIX "+VERSION"
#define AT_ORGL_SUFFIX "+ORGL"
#define AT_ADDR_SUFFIX "+ADDR"
#define AT_NAME_SUFFIX "+NAME"
#define AT_RNAME_SUFFIX "+RNAME"
#define AT_ROLE_SUFFIX "+ROLE"
#define AT_CLASS_SUFFIX "+CLASS"
#define AT_IAC_SUFFIX "+IAC"
#define AT_INQM_SUFFIX "+INQM"
#define AT_PSWD_SUFFIX "+PSWD"
#define AT_UART_SUFFIX "+UART"
#define AT_CMODE_SUFFIX "+CMODE"
#define AT_BIND_SUFFIX "+BIND"
#define AT_POLAR_SUFFIX "+POLAR"
#define AT_PIO_SUFFIX "+PIO"
#define AT_IPSCAN_SUFFIX "+IPSCAN"
#define AT_SNIFF_SUFFIX "+SNIFF"
#define AT_SENM_SUFFIX "+SENM"
#define AT_PMSAD_SUFFIX "+PMSAD"
#define AT_RMAAD_SUFFIX "+RMAAD"
#define AT_FSAD_SUFFIX "+FSAD"
#define AT_ADCN_SUFFIX "+ADCN"
#define AT_MRAD_SUFFIX "+MRAD"
#define AT_STATE_SUFFIX "+STATE"
#define AT_INIT_SUFFIX "+INIT"
#define AT_INQ_SUFFIX "+INQ"
#define AT_INQC_SUFFIX "+INQC"
#define AT_PAIR_SUFFIX "+PAIR"
#define AT_LINK_SUFFIX "+LINK"
#define AT_DISC_SUFFIX "+DISC"
#define AT_ENSNIFF_SUFFIX "+ENSNIFF"
#define AT_EXSNIFF_SUFFIX "+EXSNIFF"


// ---------------------------------------------- STRUCTURES ---------------------------------------------------- //

typedef enum {
	RESPONSE_OK = 0x1,
	RESPONSE_FAIL = 0x2
}HC05_RESPONSE;

typedef struct {
	HC05_RESPONSE response;
	char* param1;
}HC05_RESPONSE_ONEPARAM;

typedef struct {
	HC05_RESPONSE response;
	char* param1;
	char* param2;
}HC05_RESPONSE_TWOPARAM;


typedef struct {
	HC05_RESPONSE response;
	char* param1;
	char* param2;
	char* param3;
}HC05_RESPONSE_THREEPARAM;


typedef struct {
	HC05_RESPONSE response;
	char* param1;
	char* param2;
	char* param3;
	char* param4;
}HC05_RESPONSE_FOURPARAM;

// ---------------------------------------------- PRIVATE FUNCTIONS ---------------------------------------------------- //
static const char *createCmdWithParams(char* cmd,int numparams, char* params[]);
static const char *getStringResponse();
static HC05_RESPONSE getNoParamResponse();
static HC05_RESPONSE_ONEPARAM getOneParamResponse(char* cmd);
static HC05_RESPONSE_TWOPARAM getTwoParamResponse(char* cmd);
static HC05_RESPONSE_THREEPARAM getThreeParamResponse(char* cmd);
static HC05_RESPONSE_FOURPARAM getFourParamResponse(char* cmd);

// ---------------------------------------------- PUBLIC FUNCTIONS ---------------------------------------------------- //
void initializeHC05Module(UART_HandleTypeDef* _huart);
void sendData(char* data);
const char* receiveData(int size);
HC05_RESPONSE testModule();
HC05_RESPONSE resetModule();
HC05_RESPONSE_ONEPARAM getModuleVersion();
HC05_RESPONSE restoreModule();
HC05_RESPONSE_ONEPARAM getModuleAddress();
HC05_RESPONSE setModuleName(char* name);
HC05_RESPONSE_ONEPARAM getModuleName();
HC05_RESPONSE_ONEPARAM getDeviceName(char* addr);
HC05_RESPONSE setModuleRole(char* role);
HC05_RESPONSE_ONEPARAM getModuleRole();
HC05_RESPONSE setDeviceClass(char* class);
HC05_RESPONSE_ONEPARAM getDeviceClass();
HC05_RESPONSE setGIAC(char* giac);
HC05_RESPONSE_ONEPARAM getGIAC();
HC05_RESPONSE setQueryAccessPatterns(char* mode,char* maxDevices, char* timeout);
HC05_RESPONSE_THREEPARAM getQueryAccessPatterns();
HC05_RESPONSE setPIN(char* pin);
HC05_RESPONSE_ONEPARAM getPin();
HC05_RESPONSE setSerialParameter(char* baud, char* stopBit, char* parity);
HC05_RESPONSE_THREEPARAM getSerialParameter();
HC05_RESPONSE setConnectMode(char* mode);
HC05_RESPONSE_ONEPARAM getConnectMode();
HC05_RESPONSE setFixedAddress(char* addr);
HC05_RESPONSE_ONEPARAM getFixedAddress();
HC05_RESPONSE setLEDIO(char* pio8, char* pio9);
HC05_RESPONSE_TWOPARAM getLEDIO();
HC05_RESPONSE setPIOOutput(char* pionumber, char* piolevel);
HC05_RESPONSE setScanParameter(char* queryInterval, char* queryDuration, char* pagingInterval, char* callDuration);
HC05_RESPONSE_FOURPARAM getScanParameter();
HC05_RESPONSE setSNIFFParameter(char* maxTime, char* minTime, char* retryTime, char* timeout);
HC05_RESPONSE_FOURPARAM getSNIFFParameter();
HC05_RESPONSE setSecurityMode(char* secmode, char* hci);
HC05_RESPONSE_TWOPARAM getSecurityMode();
HC05_RESPONSE deleteAuthenticatedDevice(char* addr);
HC05_RESPONSE deleteAllAuthenticatedDevices();
HC05_RESPONSE searchAuthenticatedDevice(char* addr);
HC05_RESPONSE_ONEPARAM getAuthenticatedDeviceCount();
HC05_RESPONSE_ONEPARAM mostRecentlyUsedAuthenticatedDevice();
HC05_RESPONSE_ONEPARAM getModuleState();
HC05_RESPONSE initializeSPPProfile();
HC05_RESPONSE_FOURPARAM inquiryBluetoothDevice();
HC05_RESPONSE cancelInquiringBluetoothDevice();
HC05_RESPONSE pairWithDevice(char* addr, char* timeout);
HC05_RESPONSE connectWithDevice(char* addr);
HC05_RESPONSE_ONEPARAM disconnectWithDevice(char* addr);
HC05_RESPONSE energySavingMode(char* addr);
HC05_RESPONSE exertsEnergySavingMode(char* addr);


#ifdef __cplusplus
}
#endif

#endif /* __HC05 */





