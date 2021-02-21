/*
 * 	API for HC-05 Module (hc05.c/hc05.h).
 * 	Authors: Camilla Gomes Fernandes
			 Lucas Guimarães Hofner
 *  This API was developed as an assignment for Embedded Systems
 *  Programming class at the Federal University of Minas Gerais
*/

// ------------------------------------------- INCLUDES ------------------------------------------------------------- //
#include "hc05.h"


// ------------------------------------------- GLOBAL VARIABLES ---------------------------------------------------- //
UART_HandleTypeDef huart;

// ------------------------------------------- PRIVATE FUNCTIONS ---------------------------------------------------- //

/*
	Description: Create AT Commands joining the command with the given parameters.
	@param[cmd]: AT command
	@param[numparams]: Number of parameters required by the given AT command
	@param[params]: Array of parameters
	@returnValue: Complete AT command with parameters
*/
static const char* createCmdWithParams(char* cmd,int numparams, char* params[]){
	char* buf = malloc(100);
	sprintf(buf,cmd);
	int i=0;
	for(i=0; i<numparams; i++){
		if(i!=0){
			sprintf(buf+strlen(buf),",");
		}
		sprintf(buf+strlen(buf),params[i]);
	}
	sprintf(buf+strlen(buf),"\r\n");
	return buf;
}

/*
	Description: Read UART buffer and check for "OK\r\n" or errors in the response.
	@returnValue: String AT response for the command or fail, if no "OK\r\n" is found.
*/
static const char *getStringResponse(){

	int maxCaract = 2000;
	int bufferPosition = 0;
	char buffer[maxCaract];
	char raw_buffer[maxCaract];
	HAL_UART_Receive(&huart,(uint8_t *)raw_buffer, maxCaract, 1000);


	while(bufferPosition<maxCaract) {
		buffer[bufferPosition]=  raw_buffer[bufferPosition];
		buffer[bufferPosition+1] = '\0';
		if(bufferPosition >= 4) {
			int len = strlen(buffer);
			char* lastFourChars = &buffer[len-4];
			if(strcmp(lastFourChars,OK)==0) {
				return strtok(buffer,"\0");
			}
		}

		if(bufferPosition >= 6) {
			int len = strlen(buffer);
			char* lastSixChars = &buffer[len-6];
			if(strcmp(lastSixChars,FAIL)==0) {
				return strtok(buffer,"\0");
			}
		}
		bufferPosition++;
	}

	return FAIL;
}

/*
	Description: Get the string AT response for the command and build the response.
	@returnValue: Return a OK or a FAIL, given the response.
*/
static HC05_RESPONSE getNoParamResponse() {
	char* strresponse = getStringResponse();
	HC05_RESPONSE response;
	if(strcmp(strresponse,OK)==0){
		response = RESPONSE_OK;
	} else {
		response = RESPONSE_FAIL;
	}
	return response;
}

/*
	Description: Get the string AT response for the command and build the response with one parameter in return.
	@param[cmd]: Suffix for that AT command.
	@returnValue: Return a OK or a FAIL, given the response, plus the one parameter received.
*/
static HC05_RESPONSE_ONEPARAM getOneParamResponse(char* cmd){
	char* strresponse = getStringResponse();
	HC05_RESPONSE response;

	int len = strlen(strresponse);
	char* lastFourChars = &strresponse[len-4];
	if(strcmp(lastFourChars,OK)==0) {
		response = RESPONSE_OK;
		int len_cmd = strlen(cmd) + 1;
		int param_len = len - len_cmd - 4;
		char param[param_len+1];
		memcpy(param,&strresponse[len_cmd],param_len);
		param[param_len+1] = "\0";
		char param1[100];
		strcpy(param1,param);
		HC05_RESPONSE_ONEPARAM oneParamResponse = {.response = response, .param1 = param1};
		return oneParamResponse;
	} else {
		response = RESPONSE_FAIL;
		HC05_RESPONSE_ONEPARAM oneParamResponse = {.response = response, .param1 = ""};
		return oneParamResponse;
	}
}

/*
	Description: Get the string AT response for the command and build the response with two parameters in return.
	@param[cmd]: Suffix for that AT command.
	@returnValue: Return a OK plus the two parameters received,or a FAIL, given the response.
*/
static HC05_RESPONSE_TWOPARAM getTwoParamResponse(char* cmd) {
	char* strresponse = getStringResponse();
	HC05_RESPONSE response;

	int len = strlen(strresponse);
	char* lastFourChars = &strresponse[len-4];
	if(strcmp(lastFourChars,OK)==0) {
		response = RESPONSE_OK;
		int len_cmd = strlen(cmd) + 1;
		int param_len = len - len_cmd - 4;
		char param[100] = "";
		memcpy(param,&strresponse[len_cmd],param_len);
		param[param_len] = '\0';
		char* token = strtok(param,",");
		char parameters[2][100];
		int i = 0;
		while(token!=NULL){
			token[strlen(token)]='\0';
			strcpy(parameters[i], token);
			i++;
			token = strtok(NULL,",");
		}


		HC05_RESPONSE_TWOPARAM twoParamResponse = {.response = response, .param1 = parameters[0], .param2 = parameters[1]};
		return twoParamResponse;
	} else {
		response = RESPONSE_FAIL;
		HC05_RESPONSE_TWOPARAM twoParamResponse = {.response = response, .param1 = "", .param2 = ""};
		return twoParamResponse;
	}
}

/*
	Description: Get the string AT response for the command and build the response with three parameters in return.
	@param[cmd]: Suffix for that AT command.
	@returnValue: Return a OK plus the three parameters received,or a FAIL, given the response.
*/
static HC05_RESPONSE_THREEPARAM getThreeParamResponse(char* cmd){
	char* strresponse = getStringResponse();
	HC05_RESPONSE response;

	int len = strlen(strresponse);
	char* lastFourChars = &strresponse[len-4];
	lastFourChars[strlen(lastFourChars)]= '\0';
	if(strcmp(lastFourChars,OK)==0) {
		response = RESPONSE_OK;
		int len_cmd = strlen(cmd) + 1;
		int param_len = len - len_cmd - 6;
		char param[100] = "";
		memcpy(param,&strresponse[len_cmd],param_len);
		param[param_len] = '\0';
		char* token = strtok(param,",");
		char parameters[3][100];
		int i = 0;
		while(token!=NULL){
			token[strlen(token)]='\0';
			strcpy(parameters[i], token);
			i++;
			token = strtok(NULL,",");
		}

	    HC05_RESPONSE_THREEPARAM threeParamResponse = {.response = response, .param1 = parameters[0], .param2 = parameters[1], .param3 = parameters[2]};
		return threeParamResponse;
	} else {
		response = RESPONSE_FAIL;
		HC05_RESPONSE_THREEPARAM threeParamResponse = {.response = response, .param1 = "", .param2 = "", .param3 = ""};
		return threeParamResponse;
	}
}

/*
	Description: Get the string AT response for the command and build the response with four parameters in return.
	@param[cmd]: Suffix for that AT command.
	@returnValue: Return a OK plus the four parameters received,or a FAIL, given the response.
*/
static HC05_RESPONSE_FOURPARAM getFourParamResponse(char* cmd){
	char* strresponse = getStringResponse(100);
	HC05_RESPONSE response;

	int len = strlen(strresponse);
	char* lastFourChars = &strresponse[len-4];
	if(strcmp(lastFourChars,OK)==0) {
		response = RESPONSE_OK;
		int len_cmd = strlen(cmd) + 1;
		int param_len = len - len_cmd - 4;
		char param[100] = "";
		memcpy(param,&strresponse[len_cmd],param_len);
		param[param_len] = '\0';
		char* token = strtok(param,",");
		char parameters[4][100];
		int i = 0;
		while(token!=NULL){
			token[strlen(token)]='\0';
			strcpy(parameters[i], token);
			i++;
			token = strtok(NULL,",");
		}

	    HC05_RESPONSE_FOURPARAM fourParamResponse = {.response = response, .param1 = parameters[0], .param2 = parameters[1], .param3 = parameters[2], .param4 = parameters[3]};
		return fourParamResponse;
	} else {
		response = RESPONSE_FAIL;
		HC05_RESPONSE_FOURPARAM fourParamResponse = {.response = response, .param1 = "", .param2 = "", .param3 = "", .param4 = ""};
		return fourParamResponse;
	}
}

/*
	Description: Initialize the UART handler for the Bluetooth Module, allowing the communication.
	@param[_huart]: Pointer to the UART handler.
*/
void initializeHC05Module(UART_HandleTypeDef* _huart) {
	memcpy(&huart, _huart, sizeof(*_huart));
}

/*
	Description: Send a string to the Bluetooth Module through the UART handler.
	@param[data]: Data to be sent to HC-05.
*/
void sendData(char* data){
	HAL_UART_Transmit(&huart,(uint8_t *)data, strlen(data), 1000);
}

/*
	Description: Receive a string from the Bluetooth Module through the UART handler.
	@param[size]: Size of the word expected to be received.
	@returnValue: String containing the data receveid.
*/
const char* receiveData(int size){
	char* data = malloc(size);
	HAL_UART_Receive(&huart,(uint8_t *)data, size, 1000);
	return data;
}

/*
	Description: AT Command to test the Module.
	@returnValue: 	{response:OK}
					or
					{response:FAIL}.
*/
HC05_RESPONSE testModule(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT, strlen(AT),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to reset the Module.
	@returnValue: 	{response:OK}
					or
					{response:FAIL}.
*/
HC05_RESPONSE resetModule(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_RESET, strlen(AT_RESET),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get the Module's version.
	@returnValue: 	{response:OK
					param1:Module's version}
					or 
					{response:FAIL}.
*/
HC05_RESPONSE_ONEPARAM getModuleVersion(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_VERSION, strlen(AT_VERSION),100);
	return getOneParamResponse(AT_VERSION_SUFFIX);
}

/*
	Description: AT Command to resetore Module's factory configurations.
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE restoreModule(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_ORGL, strlen(AT_ORGL),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get the Module's address.
	@returnValue: 	{response:OK,
					param1:Module's address} 
					or
					{response:FAIL}.
*/
HC05_RESPONSE_ONEPARAM getModuleAddress(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_ADDR, strlen(AT_ADDR),100);
	return getOneParamResponse(AT_ADDR_SUFFIX);
}

/*
	Description: AT Command to set the Module's name.
	@param[name]: Desired Module's name. 
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE setModuleName(char* name){
	const char * params[] = {
    name,
	};
	char* cmd = createCmdWithParams(AT_NAME,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get the Module's name.
	@returnValue: 	{response:OK,
					param1: Module's name} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE_ONEPARAM getModuleName(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_NAME_, strlen(AT_NAME_),100);
	return getOneParamResponse(AT_NAME_SUFFIX);
}

/*
	Description: AT Command to get a device's name based on its address.
	@param[addr]: Device's address whose name is wanted.
	@returnValue: 	{response:OK,
					param1: device's name} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE_ONEPARAM getDeviceName(char* addr){
	const char * params[] = {
    name,
	};
	char* cmd = createCmdWithParams(AT_RNAME,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getOneParamResponse(AT_RNAME_SUFFIX);
}

/*
	Description: AT Command to set Module role.
	@param[role]: Desired role (0 - Slave, 1- Master, 2- Slave-Loop).
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE setModuleRole(char* role){
	const char * params[] = {
    role,
	};
	char* cmd = createCmdWithParams(AT_ROLE,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get Module's role.
	@returnValue: 	{response:OK,
					param1: role} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE_ONEPARAM getModuleRole(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_ROLE_, strlen(AT_ROLE_),100);
	return getOneParamResponse(AT_ROLE_SUFFIX);
}

/*
	Description: AT Command to set Module's class.
	@param[class]: Desired Module's class.
	@returnValue: 	{response:OK}
				   	or 
				   	{response:FAIL.
*/
HC05_RESPONSE setDeviceClass(char* class){
	const char * params[] = {
    class,
	};
	char* cmd = createCmdWithParams(AT_CLASS,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get Module's class.
	@returnValue: 	{response:OK
					param1: class}
					or
					{response:FAIL}.
*/
HC05_RESPONSE_ONEPARAM getDeviceClass(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_CLASS_, strlen(AT_CLASS_),100);
	return getOneParamResponse(AT_CLASS_SUFFIX);
}

/*
	Description: AT Command to set Module's GIAC.
	@param[giac]: Desired Module's GIAC.
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE setGIAC(char* giac){
	const char * params[] = {
    giac,
	};
	char* cmd = createCmdWithParams(AT_IAC,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get Module's GIAC.
	@returnValue: 	{response:OK,
					param1: GIAC} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE_ONEPARAM getGIAC(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_IAC_, strlen(AT_IAC_),100);
	return getOneParamResponse(AT_IAC_SUFFIX);
}

/*
	Description: AT Command to set Module's Query Access Patterns.
	@param[mode]: Inquiry mode (0 - Inquiry Mode Standard, 1 - Inquiry Mode RSSI).
	@param[maxDevices]: Maximum number of devices to respond.
	@param[timeout]: Timeout value (1-48 -> 1.28s to 61.44s)
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE setQueryAccessPatterns(char* mode,char* maxDevices, char* timeout){
	const char * params[] = {
    mode,
	maxDevices,
	timeout,
	};
	char* cmd = createCmdWithParams(AT_INQM,3,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get Module's Query Access Patterns.
	@returnValue: 	{response:OK, 
					param1:inquiry mode,
					param2:maximum number of devices
					param3:timeout value} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE_THREEPARAM getQueryAccessPatterns(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_INQM_, strlen(AT_INQM_),100);
	return getThreeParamResponse(AT_INQM_SUFFIX);
}

/*
	Description: AT Command to set Module's PIN code.
	@param[pin]: Desired Module's PIN code.
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE setPIN(char* pin){
	const char * params[] = {
    pin,
	};
	char* cmd = createCmdWithParams(AT_PSWD,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get Module's PIN code.
	@returnValue: OK and Module's PIN code or FAIL.
*/
HC05_RESPONSE_ONEPARAM getPin(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_PSWD_, strlen(AT_PSWD_),100);
	return getOneParamResponse(AT_PSWD_SUFFIX);
}

/*
	Description: AT Command to set Module's UART Parameters.
	@param[baud]: UART Baud Rate (9600,19200,38400,57600,115200,230400,460800).
	@param[stopBit]: Stop Bit.
	@param[parity]: Parity.
	@returnValue: 	{response:OK} 
					or 	
					{response:FAIL}.
*/
HC05_RESPONSE setSerialParameter(char* baud, char* stopBit, char* parity){
	const char * params[] = {
    baud,
	stopBit,
	parity,
	};
	const char* cmd = createCmdWithParams(AT_UART,3,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get Module's UART Parameters.
	@returnValue: 	{response:OK,
					param1:baud rate,
					param2:stop bit,
					param3:parity} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE_THREEPARAM getSerialParameter(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_UART_, strlen(AT_UART_),100);
	return getThreeParamResponse(AT_UART_SUFFIX);
}

/*
	Description: AT Command to set Module's connection mode.
	@param[mode]: Desired Module's connection mode (0 - fixed address, 1 - any address, 2 - slave-loop).
	@returnValue: 	{response:OK} 
				   	or 
					{response:FAIL}.
*/
HC05_RESPONSE setConnectMode(char* mode){
	const char * params[] = {
    mode,
	};
	char* cmd = createCmdWithParams(AT_CMODE,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get Module's connection mode.
	@returnValue: OK and Module's connection mode or FAIL.
*/
HC05_RESPONSE_ONEPARAM getConnectMode(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_CMODE_, strlen(AT_CMODE_),100);
	return getOneParamResponse(AT_CMODE_SUFFIX);
}

/*
	Description: AT Command to set Module's fixed address.
	@param[addr]: Desired Module's fixed address.
	@returnValue:	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE setFixedAddress(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_BIND,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to et Module's fixed address.
	@returnValue: 	{response:OK,
				  	param1: fixed address}
					or 
					{response:FAIL}.
*/
HC05_RESPONSE_ONEPARAM getFixedAddress(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_BIND_, strlen(AT_BIND_),100);
	return getOneParamResponse(AT_BIND_SUFFIX);
}

/*
	Description: AT Command to set Module's LED/IO.
	@param[pio8]: PIO8 Drive LED (0 - LOW Drive LED, 1 - HIGH Drive LED)
	@param[pio9]: PIO9 Drive LED (0 - LOW Drive LED, 1 - HIGH Drive LED)
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE setLEDIO(char* pio8, char* pio9){
	const char * params[] = {
    pio8,
	pio9,
	};
	char* cmd = createCmdWithParams(AT_POLAR,2,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get Module's LED/IO.
	@returnValue: 	{response:OK,
					param1: PIO8 Drive LED,
					param2: PIO9 Drive LED} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE_TWOPARAM getLEDIO(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_POLAR_, strlen(AT_POLAR_),100);
	return getTwoParamResponse(AT_POLAR_SUFFIX);
}

/*
	Description: AT Command to set Module's PIO Output.
	@param[pionumber]: PIO number to be set.
	@param[piolevel]: PIO level to be set.
	@returnValue: {response: OK} 
				or 
				{response:FAIL}.
*/
HC05_RESPONSE setPIOOutput(char* pionumber, char* piolevel){
	const char * params[] = {
    pionumber,
	piolevel,
	};
	char* cmd = createCmdWithParams(AT_PIO,2,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to set Module's Scan Parameter.
	@param[queryInterval]: Query Time Interval.
	@param[queryDuration]: Query Duration.
	@param[pagingInterval]: Paging Interval.
	@param[callDuration]: call Duration.
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE setScanParameter(char* queryInterval, char* queryDuration, char* pagingInterval, char* callDuration){
	const char * params[] = {
    queryInterval,
	queryDuration,
	pagingInterval,
	callDuration,
	};
	char* cmd = createCmdWithParams(AT_IPSCAN,4,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get Module's Scan Parameter.
	@returnValue: {	response:OK, 
					param1: queryInterval, 
					param2: queryDuration, 
					param3: pagingInterval, 
				   	param4: callDuration} 
				or 
					{response:FAIL}.
*/
HC05_RESPONSE_FOURPARAM getScanParameter(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_IPSCAN_, strlen(AT_IPSCAN_),100);
	return getFourParamResponse(AT_IPSCAN_SUFFIX);
}

/*
	Description: AT Command to set Module's SNIFF Parameter.
	@param[maxTime]: Maximum Time.
	@param[minTime]: Minimum Time.
	@param[retryTime]: Retry Time.
	@param[timeout]: Timeout.
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE setSNIFFParameter(char* maxTime, char* minTime, char* retryTime, char* timeout){
	const char * params[] = {
    maxTime,
	minTime,
	retryTime,
	timeout,
	};
	char* cmd = createCmdWithParams(AT_SNIFF,4,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get Module's SNIFF Parameter.
	@returnValue: {	response:OK, 
					param1: maxTime, 
					param2: minTime, 
					param3: retryTime, 
				   	param4: timeout} 
				or 
					{response:FAIL}.
*/
HC05_RESPONSE_FOURPARAM getSNIFFParameter(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_SNIFF_, strlen(AT_SNIFF_),100);
	return getFourParamResponse(AT_SNIFF_SUFFIX);
}


/*
	Description: AT Command to set Module's security mode.
	@param[secmode]: Security Mode (0 - Sec Mode off, 1 - Non secure, 2 - Service, 3 - Link, 4 - Unknown)
	@param[HCI]: HCI(0 - hci mode off, 1 - PT to PT, 2 - PT to PT and BCAST)
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE setSecurityMode(char* secmode, char* hci){
	const char * params[] = {
    secmode,
	hci,
	};
	char* cmd = createCmdWithParams(AT_SENM,2,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to get Module's security mode.
	@returnValue: {	response:OK, 
					param1: secmode, 
				   	param4: hci} 
				or 
					{response:FAIL}.
*/
HC05_RESPONSE_TWOPARAM getSecurityMode(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_SENM_, strlen(AT_SENM_),100);
	return getTwoParamResponse(AT_SENM_SUFFIX);
}

/*
	Description: AT Command to delete a authenticated device, given its address.
	@param[addr]: Address of device to be deleted. 
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE deleteAuthenticatedDevice(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_PMSAD,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to delete all authenticated devices.
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE deleteAllAuthenticatedDevices(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_RMAAD, strlen(AT_RMAAD),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to search a authenticated device, given its address.
	@param[addr]: Address of device to be searched. 
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE searchAuthenticatedDevice(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_FSAD,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to count the number of authenticated devices. 
	@returnValue: 	{response:OK,
					param1: number of devices} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE_ONEPARAM getAuthenticatedDeviceCount(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_ADCN, strlen(AT_ADCN),100);
	return getOneParamResponse(AT_ADCN_SUFFIX);
}

/*
	Description: AT Command to get the most recently used device.
	@returnValue: 	{response:OK,
					param1: address of most used device} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE_ONEPARAM mostRecentlyUsedAuthenticatedDevice(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_MRAD, strlen(AT_MRAD),100);
	return getOneParamResponse(AT_MRAD_SUFFIX);
}

/*
	Description: AT Command to get Module's state.
	@returnValue: 	{response:OK,
					param1: state (“INITIALIZED”,“READY”,“PAIRABLE”,“PAIRED”,“INQUIRING”,
									“CONNECTING”,“CONNECTED”,“DISCONNECTED”,“NUKNOW”)} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE_ONEPARAM getModuleState(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_STATE, strlen(AT_STATE),100);
	return getOneParamResponse(AT_STATE_SUFFIX);
}

/*
	Description: AT Command to initializa SPP profile lib.
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE initializeSPPProfile(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_INIT, strlen(AT_INIT),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to inquiry bluetooth device.
	@returnValue: {	response:OK, 
					param1: address, 
					param2: device class, 
					param3: rssi signal strength} 
				or 
					{response:FAIL}.
*/
HC05_RESPONSE_FOURPARAM inquiryBluetoothDevice(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_INQ, strlen(AT_INQ),100);
	return getFourParamResponse(AT_INQ_SUFFIX);
}

/*
	Description: AT Command to cancel inquiring bluetooth device.
	@returnValue: {	response:OK} 
				or 
					{response:FAIL}.
*/
HC05_RESPONSE cancelInquiringBluetoothDevice(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_INQC, strlen(AT_INQC),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to pair with device.
	@param[addr]: Address of device to be paired
	@param[timeout]: Timeout value
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE pairWithDevice(char* addr, char* timeout){
	const char * params[] = {
    addr,
	timeout,
	};
	char* cmd = createCmdWithParams(AT_PAIR,2,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to connect with device.
	@param[addr]: Address of device to be connected.
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE connectWithDevice(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_LINK,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to disconnect from device.
	@param[addr]: Address of device to be paired
	@returnValue: 	{response:OK,
					param1: [SUCCESS,LINK_LOSS,NO_SLC,TIMEOUT,ERROR]} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE_ONEPARAM disconnectWithDevice(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_DISC,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getOneParamResponse(AT_DISC_SUFFIX);
}

/*
	Description: AT Command to enter in energy saving mode.
	@param[addr]: Address of device to enter in energy saving mode.
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE energySavingMode(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_ENSNIFF,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

/*
	Description: AT Command to exerts energy saving mode.
	@param[addr]: Address of device to exerts energy saving mode.
	@returnValue: 	{response:OK} 
					or 
					{response:FAIL}.
*/
HC05_RESPONSE exertsEnergySavingMode(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_EXSNIFF,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}
