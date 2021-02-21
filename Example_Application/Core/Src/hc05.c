#include "hc05.h"


void initializeHC05Module(UART_HandleTypeDef* _huart) {
	memcpy(&huart, _huart, sizeof(*_huart));
}

void sendData(char* data){
	HAL_UART_Transmit(&huart,(uint8_t *)data, strlen(data), 1000);
}

const char* receiveData(int size){
	char* data = malloc(size);
	HAL_UART_Receive(&huart,(uint8_t *)data, size, 1000);
	return data;
}

const char* createCmdWithParams(char* cmd,int numparams, char* params[]){
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

const char *getStringResponse(){

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

HC05_RESPONSE getNoParamResponse() {
	char* strresponse = getStringResponse();
	HC05_RESPONSE response;
	if(strcmp(strresponse,OK)==0){
		response = RESPONSE_OK;
	} else {
		response = RESPONSE_FAIL;
	}
	return response;
}

HC05_RESPONSE_ONEPARAM getOneParamResponse(char* cmd){
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

HC05_RESPONSE_TWOPARAM getTwoParamResponse(char* cmd) {
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
HC05_RESPONSE_THREEPARAM getThreeParamResponse(char* cmd){
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
HC05_RESPONSE_FOURPARAM getFourParamResponse(char* cmd){
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

HC05_RESPONSE testModule(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT, strlen(AT),100);
	return getNoParamResponse();
}

HC05_RESPONSE resetModule(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_RESET, strlen(AT_RESET),100);
	return getNoParamResponse();
}

HC05_RESPONSE_ONEPARAM getModuleVersion(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_VERSION, strlen(AT_VERSION),100);
	return getOneParamResponse(AT_VERSION_CMD);
}

HC05_RESPONSE restoreModule(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_ORGL, strlen(AT_ORGL),100);
	return getNoParamResponse();
}

HC05_RESPONSE_ONEPARAM getModuleAddress(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_ADDR, strlen(AT_ADDR),100);
	return getOneParamResponse(AT_ADDR_CMD);
}

HC05_RESPONSE setModuleName(char* name){
	const char * params[] = {
    name,
	};
	char* cmd = createCmdWithParams(AT_NAME,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE_ONEPARAM getModuleName(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_NAME_, strlen(AT_NAME_),100);
	return getOneParamResponse(AT_NAME_CMD);
}

HC05_RESPONSE_ONEPARAM getDeviceName(char* name){
	const char * params[] = {
    name,
	};
	char* cmd = createCmdWithParams(AT_RNAME,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getOneParamResponse(AT_RNAME_CMD);
}

HC05_RESPONSE setModuleRole(char* role){
	const char * params[] = {
    role,
	};
	char* cmd = createCmdWithParams(AT_ROLE,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE_ONEPARAM getModuleRole(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_ROLE_, strlen(AT_ROLE_),100);
	return getOneParamResponse(AT_ROLE_CMD);
}

HC05_RESPONSE setDeviceClass(char* class){
	const char * params[] = {
    class,
	};
	char* cmd = createCmdWithParams(AT_CLASS,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE_ONEPARAM getDeviceClass(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_CLASS_, strlen(AT_CLASS_),100);
	return getOneParamResponse(AT_CLASS_CMD);
}

HC05_RESPONSE setGIAC(char* giac){
	const char * params[] = {
    giac,
	};
	char* cmd = createCmdWithParams(AT_IAC,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE_ONEPARAM getGIAC(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_IAC_, strlen(AT_IAC_),100);
	return getOneParamResponse(AT_IAC_CMD);
}

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

HC05_RESPONSE_THREEPARAM getQueryAccessPatterns(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_INQM_, strlen(AT_INQM_),100);
	return getThreeParamResponse(AT_INQM_CMD);
}

HC05_RESPONSE setPIN(char* pin){
	const char * params[] = {
    pin,
	};
	char* cmd = createCmdWithParams(AT_PSWD,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE_ONEPARAM getPin(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_PSWD_, strlen(AT_PSWD_),100);
	return getOneParamResponse(AT_PSWD_CMD);
}

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

HC05_RESPONSE_THREEPARAM getSerialParameter(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_UART_, strlen(AT_UART_),100);
	return getThreeParamResponse(AT_UART_CMD);
}

HC05_RESPONSE setConnectMode(char* mode){
	const char * params[] = {
    mode,
	};
	char* cmd = createCmdWithParams(AT_CMODE,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE_ONEPARAM getConnectMode(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_CMODE_, strlen(AT_CMODE_),100);
	return getOneParamResponse(AT_CMODE_CMD);
}

HC05_RESPONSE setFixedAddress(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_BIND,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE_ONEPARAM getFixedAddress(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_BIND_, strlen(AT_BIND_),100);
	return getOneParamResponse(AT_BIND_CMD);
}

HC05_RESPONSE setLEDIO(char* pio8, char* pio9){
	const char * params[] = {
    pio8,
	pio9,
	};
	char* cmd = createCmdWithParams(AT_POLAR,2,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE_TWOPARAM getLEDIO(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_POLAR_, strlen(AT_POLAR_),100);
	return getTwoParamResponse(AT_POLAR_CMD);
}

HC05_RESPONSE setPIOOutput(char* pionumber, char* piolevel){
	const char * params[] = {
    pionumber,
	piolevel,
	};
	char* cmd = createCmdWithParams(AT_PIO,2,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

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

HC05_RESPONSE_FOURPARAM getScanParameter(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_IPSCAN_, strlen(AT_IPSCAN_),100);
	return getFourParamResponse(AT_IPSCAN_CMD);
}

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

HC05_RESPONSE_FOURPARAM getSNIFFParameter(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_SNIFF_, strlen(AT_SNIFF_),100);
	return getFourParamResponse(AT_SNIFF_CMD);
}

HC05_RESPONSE setSecurityMode(char* secmode, char* hci){
	const char * params[] = {
    secmode,
	hci,
	};
	char* cmd = createCmdWithParams(AT_SENM,2,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE_TWOPARAM getSecurityMode(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_SENM_, strlen(AT_SENM_),100);
	return getTwoParamResponse(AT_SENM_CMD);
}

HC05_RESPONSE deleteAuthenticatedDevice(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_PMSAD,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE deleteAllAuthenticatedDevices(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_RMAAD, strlen(AT_RMAAD),100);
	return getNoParamResponse();
}

HC05_RESPONSE searchAuthenticatedDevice(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_FSAD,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE_ONEPARAM getAuthenticatedDeviceCount(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_ADCN, strlen(AT_ADCN),100);
	return getOneParamResponse(AT_ADCN_CMD);
}

HC05_RESPONSE_ONEPARAM mostRecentlyUsedAuthenticatedDevice(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_MRAD, strlen(AT_MRAD),100);
	return getOneParamResponse(AT_MRAD_CMD);
}

HC05_RESPONSE_ONEPARAM getModuleState(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_STATE, strlen(AT_STATE),100);
	return getOneParamResponse(AT_STATE_CMD);
}

HC05_RESPONSE initializeSPPProfile(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_INIT, strlen(AT_INIT),100);
	return getNoParamResponse();
}

HC05_RESPONSE_FOURPARAM inquiryBluetoothDevice(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_INQ, strlen(AT_INQ),100);
	return getFourParamResponse(AT_INQ_CMD);
}

HC05_RESPONSE cancelInquiringBluetoothDevice(){
	HAL_UART_Transmit(&huart, (uint8_t *)AT_INQC, strlen(AT_INQC),100);
	return getNoParamResponse();
}

HC05_RESPONSE pairWithDevice(char* addr, char* timeout){
	const char * params[] = {
    addr,
	timeout,
	};
	char* cmd = createCmdWithParams(AT_PAIR,2,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE connectWithDevice(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_LINK,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE_ONEPARAM disconnectWithDevice(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_DISC,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getOneParamResponse(AT_DISC_CMD);
}

HC05_RESPONSE energySavingMode(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_ENSNIFF,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}

HC05_RESPONSE exertsEnergySavingMode(char* addr){
	const char * params[] = {
    addr,
	};
	char* cmd = createCmdWithParams(AT_EXSNIFF,1,params);
	HAL_UART_Transmit(&huart, (uint8_t *)cmd, strlen(cmd),100);
	return getNoParamResponse();
}
