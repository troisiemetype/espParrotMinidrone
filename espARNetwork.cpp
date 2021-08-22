/*
 * This program is an esp32-based Library to pilot Parrot Minidrones
 * Copyright (C) 2021  Pierre-Loup Martin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 #include "espARNetwork.h"

ARDeviceCallbacks::ARDeviceCallbacks(ARNetwork* network){
	_network = network;
}

void ARDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice){
	log_i("Advertised Device: %s \n", advertisedDevice.toString().c_str());

	if(!advertisedDevice.haveManufacturerData()){
		return;
	}
//	uint8_t length = advertisedDevice.getManufacturerData().length();
/*
	for(uint8_t i = 0; i < length; ++i){
		Serial.printf("%02x ", advertisedDevice.getManufacturerData().data()[i]);
	}
*/
	// Reversed manufacturer data. It's 0x4300cf19xxxxxxxx
	uint32_t manData = 0x19cf0043;
	if(memcmp(advertisedDevice.getManufacturerData().data(), &manData, 4)) return

//	if(strncmp(advertisedDevice.getName().c_str(), "RS_", 3)) return;
//	if(strncmp(advertisedDevice.getName().c_str(), "Mars_", 5)) return;
	
	BLEDevice::getScan()->stop();
	*(_network->_minidrone) = BLEAdvertisedDevice(advertisedDevice);
	 _network->_connected = true;

}

ARClientCallbacks::ARClientCallbacks(ARNetwork* network){
	_network = network;
}

void ARClientCallbacks::onConnect(BLEClient* client){
	Serial.println("connected !");
	_network->_connected = true;
}

void ARClientCallbacks::onDisconnect(BLEClient* client){
	Serial.println("disconnected !");
	_network->_connected = false;
}

// short 2-bytes ID need a base 16 ID to add to. Or something like this.
/*
BLEUUID ARNetwork::ARCOMMAND_SENDING_SERVICE_UUID = BLEUUID((uint16_t)0xfa00);

BLEUUID ARNetwork::SEND_DATA_UUID = BLEUUID((uint16_t)0xfa0a);
BLEUUID ARNetwork::SEND_DATA_WITH_ACK_UUID = BLEUUID((uint16_t)0xfa0b);
BLEUUID ARNetwork::SEND_HIGH_PRIORITY_UUID = BLEUUID((uint16_t)0xfa0c);
BLEUUID ARNetwork::SEND_ACK_UUID = BLEUUID((uint16_t)0xfa1e);

BLEUUID ARNetwork::ARCOMMAND_RECEIVING_SERVICE_UUID = BLEUUID((uint16_t)0xfb00);

BLEUUID ARNetwork::RECEIVE_DATA_UUID = BLEUUID((uint16_t)0xfb0f);
BLEUUID ARNetwork::RECEIVE_DATA_WITH_ACK_UUID = BLEUUID((uint16_t)0xfb0e);
BLEUUID ARNetwork::RECEIVE_ACK_DATA_UUID = BLEUUID((uint16_t)0xfb1b);
BLEUUID ARNetwork::RECEIVE_ACK_HIGH_PRIORITY_UUID = BLEUUID((uint16_t)0xfb1c);

BLEUUID ARNetwork::NORMAL_BLE_FTP_SERVICE_UUID = BLEUUID((uint16_t)0xfd21);

BLEUUID ARNetwork::NORMAL_FTP_TRANSFERRING_UUID = BLEUUID((uint16_t)0xfd22);
BLEUUID ARNetwork::NORMAL_FTP_GETTING_UUID = BLEUUID((uint16_t)0xfd23);
BLEUUID ARNetwork::NORMAL_FTP_HANDLING_UUID = BLEUUID((uint16_t)0xfd24);

BLEUUID ARNetwork::UPDATE_BLE_FTP_UUID = BLEUUID((uint16_t)0xfd51);

BLEUUID ARNetwork::UPDATE_FTP_TRANSFERRING_UUID = BLEUUID((uint16_t)0xfd52);
BLEUUID ARNetwork::UPDATE_FTP_GETTING_UUID = BLEUUID((uint16_t)0xfd53);
BLEUUID ARNetwork::UPDATE_FTP_HANDLING_UUID = BLEUUID((uint16_t)0xfd54);
*/
BLEUUID ARNetwork::ARCOMMAND_SENDING_SERVICE_UUID = BLEUUID("9a66fa00-0800-9191-11e4-012d1540cb8e");

BLEUUID ARNetwork::SEND_DATA_UUID = BLEUUID("9a66fa0a-0800-9191-11e4-012d1540cb8e");
BLEUUID ARNetwork::SEND_DATA_WITH_ACK_UUID = BLEUUID("9a66fa0b-0800-9191-11e4-012d1540cb8e");
BLEUUID ARNetwork::SEND_HIGH_PRIORITY_UUID = BLEUUID("9a66fa0c-0800-9191-11e4-012d1540cb8e");
BLEUUID ARNetwork::SEND_ACK_UUID = BLEUUID("9a66fa1e-0800-9191-11e4-012d1540cb8e");

BLEUUID ARNetwork::ARCOMMAND_RECEIVING_SERVICE_UUID = BLEUUID("9a66fb00-0800-9191-11e4-012d1540cb8e");

BLEUUID ARNetwork::RECEIVE_DATA_UUID = BLEUUID("9a66fb0f-0800-9191-11e4-012d1540cb8e");
BLEUUID ARNetwork::RECEIVE_DATA_WITH_ACK_UUID = BLEUUID("9a66fb0e-0800-9191-11e4-012d1540cb8e");
BLEUUID ARNetwork::RECEIVE_ACK_DATA_UUID = BLEUUID("9a66fb1b-0800-9191-11e4-012d1540cb8e");
BLEUUID ARNetwork::RECEIVE_ACK_HIGH_PRIORITY_UUID = BLEUUID("9a66fb1c-0800-9191-11e4-012d1540cb8e");

BLEUUID ARNetwork::NORMAL_BLE_FTP_SERVICE_UUID = BLEUUID("9a66fd21-0800-9191-11e4-012d1540cb8e");

BLEUUID ARNetwork::NORMAL_FTP_TRANSFERRING_UUID = BLEUUID("9a66fd22-0800-9191-11e4-012d1540cb8e");
BLEUUID ARNetwork::NORMAL_FTP_GETTING_UUID = BLEUUID("9a66fd23-0800-9191-11e4-012d1540cb8e");
BLEUUID ARNetwork::NORMAL_FTP_HANDLING_UUID = BLEUUID("9a66fd24-0800-9191-11e4-012d1540cb8e");

BLEUUID ARNetwork::UPDATE_BLE_FTP_UUID = BLEUUID("9a66fd51-0800-9191-11e4-012d1540cb8e");

BLEUUID ARNetwork::UPDATE_FTP_TRANSFERRING_UUID = BLEUUID("9a66fd52-0800-9191-11e4-012d1540cb8e");
BLEUUID ARNetwork::UPDATE_FTP_GETTING_UUID = BLEUUID("9a66fd53-0800-9191-11e4-012d1540cb8e");
BLEUUID ARNetwork::UPDATE_FTP_HANDLING_UUID = BLEUUID("9a66fd54-0800-9191-11e4-012d1540cb8e");


ARNetwork* ARNetwork::_pInstance = nullptr;

ARNetwork::ARNetwork(ARCommands* arCommands){
	_arCommands = arCommands;
	_pInstance = this;
}

ARNetwork::~ARNetwork(){
	_pInstance = nullptr;
	delete _minidrone;
	// delete advertised devices callback
	// delete client callbacks
}

void ARNetwork::init(){
	BLEDevice::init("minidrone");

	// default power level is P3
	// See if there is a particular way of setting it, it seems better but not that much.
	// There indeed is a sepcific way of setting it. See :
	// https://www.esp32.com/viewtopic.php?t=21011
	// External antena is probably needed.
	//BLEDevice::setPower(ESP_PWR_LVL_P3);

	_pBLEScan = BLEDevice::getScan();
	_pBLEScan->setAdvertisedDeviceCallbacks(new ARDeviceCallbacks(this));
	_pBLEScan->setActiveScan(true);
	_pBLEScan->setInterval(100);
	_pBLEScan->setWindow(99);

	_minidrone = new BLEAdvertisedDevice();

	_pClient = BLEDevice::createClient();
	_pClient->setClientCallbacks(new ARClientCallbacks(this));

	for(;;){
		if(_scan()) break;
//		delay(100);
	}
	_firstConnect();


}

bool ARNetwork::checkConnection(){
	if(!_connected){
		_connect();
	}
	return _connected;
}

void ARNetwork::shakeHands(){
	uint8_t toSend[] = {0, 1, 0, 0};

	_chrNormalFTPTransferring->writeValue(toSend, 4);
	_chrNormalFTPGetting->writeValue(toSend, 4);
	_chrNormalFTPHandling->writeValue(toSend, 4);
	
	_chrUpdateFTPTransferring->writeValue(toSend, 4);
	_chrUpdateFTPGetting->writeValue(toSend, 4);
	_chrUpdateFTPHandling->writeValue(toSend, 4);

}

void ARNetwork::update(){
	_sendFrame();
}

void ARNetwork::enumerateServices(){
	// List services
	std::map<std::string, BLERemoteService*>* services = _pClient->getServices();
	
	Serial.printf("found %i services :\n", services->size());

	std::map<std::string, BLERemoteService*>::key_compare mycomp = services->key_comp();

	std::string fin = services->rbegin()->first;
	std::map<std::string, BLERemoteService*>::iterator i = services->begin();

	do{
//		Serial.printf("\t%s => %s\n", i->first.c_str(), i->second->toString().c_str());
		Serial.printf("\t%s\n", i->second->toString().c_str());

		// List characteristics per service
		std::map<std::string, BLERemoteCharacteristic*>* chars = i->second->getCharacteristics();		
		std::map<std::string, BLERemoteCharacteristic*>::key_compare mycompchar = chars->key_comp();

		std::string finChar = chars->rbegin()->first;
		std::map<std::string, BLERemoteCharacteristic*>::iterator j = chars->begin();

		do{
//			Serial.printf("\t\t%s => %s\n", j->first.c_str(), j->second->toString().c_str());
			Serial.printf("\t\t%s\n", j->second->toString().c_str());
		} while(mycompchar((*j++).first, finChar));


	} while(mycomp((*i++).first, fin));
	Serial.println("end of services");
}

int ARNetwork::getRSSI(){
	return _pClient->getRssi();
}

void ARNetwork::onReceiveStatic(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* data, size_t length, bool isNotify){
	if(_pInstance == nullptr) return;
	_pInstance->onReceive(pBLERemoteCharacteristic, data, length, isNotify);
}

void ARNetwork::onReceive(BLERemoteCharacteristic* remote, uint8_t* data, size_t length, bool isNotify){
	BLEUUID remoteUUID = remote->getUUID();
	if(remoteUUID.equals(RECEIVE_DATA_UUID)){
		_onReceiveCommand(remote, data, length, isNotify);
	} else if(remoteUUID.equals(RECEIVE_DATA_WITH_ACK_UUID)){
		_onReceiveCommandAck(remote, data, length, isNotify);
	} else if(remoteUUID.equals(RECEIVE_ACK_DATA_UUID)){
		_onReceiveAckCommand(remote, data, length, isNotify);
	} else if(remoteUUID.equals(RECEIVE_ACK_HIGH_PRIORITY_UUID)){
		_onReceiveAckLowLatency(remote, data, length, isNotify);
	}
}


// protected methods
void ARNetwork::_onReceiveCommand(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* data, size_t length, bool isNotify){
/*
	Serial.printf("Received %s", pBLERemoteCharacteristic->getUUID().toString().c_str());
	Serial.print(" of data length ");
	Serial.println(length);
*/	Serial.printf("%i\treceived : ", millis());
	for (uint8_t i = 0; i < length; ++i){
		Serial.printf("%i ", data[i]);
	}
	Serial.println();

	_arCommands->_populateReceiveQueue(ARCommands::FRAME_TYPE_DATA, data, length);
}

void ARNetwork::_onReceiveCommandAck(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* data, size_t length, bool isNotify){
/*
	Serial.printf("Received with ack %s", pBLERemoteCharacteristic->getUUID().toString().c_str());
	Serial.print(" of data length ");
	Serial.println(length);
*/	Serial.printf("%i\treceived : ", millis());
	for (uint8_t i = 0; i < length; ++i){
		Serial.printf("%i ", data[i]);
	}
	Serial.println();

	// Send ack
	// Note : calling BLECharacteristic.writeValue() from inside a callback function causes a freeze because of internal semaphores ; use flags !
	_arCommands->_populateReceiveQueue(ARCommands::FRAME_TYPE_DATA_WITH_ACK, data, length);
}

void ARNetwork::_onReceiveAckCommand(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* data, size_t length, bool isNotify){
	Serial.printf("%i\treceived Ack for command id : %i\n", millis(), data[2]);
	_arCommands->_populateReceiveQueue(ARCommands::FRAME_TYPE_ACK, data, length);

}

void ARNetwork::_onReceiveAckLowLatency(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* data, size_t length, bool isNotify){
	Serial.printf("%i\treceived Ack for Low Latency id : %i\n", millis(), data[2]);
	_arCommands->_populateReceiveQueue(ARCommands::FRAME_TYPE_ACK, data, length);

}

bool ARNetwork::_scan(){
	Serial.println("Start scanning");
	BLEScanResults devices = _pBLEScan->start(BLE_SCAN_TIME, false);
//	Serial.printf("%i devices found.\n", devices.getCount());
	_pBLEScan->clearResults();

//	Serial.println();

	return _connected;
}

void ARNetwork::_connect(){
	_pClient->connect(_minidrone);
}

void ARNetwork::_firstConnect(){
	_connect();
	BLEDevice::setPower(ESP_PWR_LVL_P9);

	_setSendCharacteristics();
	_setReceiveCharacteristics();
//	_setHandshakeCharacteristics();

//	shakeHands();

}

void ARNetwork::_setSendCharacteristics(){
	// Set send service
	BLERemoteService *sendService = _pClient->getService(ARCOMMAND_SENDING_SERVICE_UUID);
	if(sendService == NULL){
		log_i("failed to create service %s\n", ARCOMMAND_SENDING_SERVICE_UUID.toString().c_str());
		return;		
	} else {
		log_i("created service %s\n", ARCOMMAND_SENDING_SERVICE_UUID.toString().c_str());		
	}

//	Serial.println();

	// Set non-ack send command
	_chrSendCommand = sendService->getCharacteristic(SEND_DATA_UUID);
	if(_chrSendCommand == NULL){
		log_i("failed to find characteristic %s\n", SEND_DATA_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", SEND_DATA_UUID.toString().c_str());		
	}

	// Set ack send command
	_chrSendCommandAck = sendService->getCharacteristic(SEND_DATA_WITH_ACK_UUID);
	if(_chrSendCommandAck == NULL){
		log_i("failed to find characteristic %s\n", SEND_DATA_WITH_ACK_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", SEND_DATA_WITH_ACK_UUID.toString().c_str());		
	}

	// Set low-latency send command
	_chrSendLowLatency = sendService->getCharacteristic(SEND_HIGH_PRIORITY_UUID);
	if(_chrSendLowLatency == NULL){
		log_i("failed to find characteristic %s\n", SEND_HIGH_PRIORITY_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", SEND_HIGH_PRIORITY_UUID.toString().c_str());		
	}

	// Set ack command
	_chrSendAck = sendService->getCharacteristic(SEND_ACK_UUID);
	if(_chrSendAck == NULL){
		log_i("failed to find characteristic %s\n", SEND_ACK_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", SEND_ACK_UUID.toString().c_str());		
	}

	Serial.println();
}

void ARNetwork::_setReceiveCharacteristics(){
	// Set receive device
	BLERemoteService *receiveService = _pClient->getService(ARCOMMAND_RECEIVING_SERVICE_UUID);

	if(receiveService == NULL){
		log_i("failed to create service %s\n", ARCOMMAND_RECEIVING_SERVICE_UUID.toString().c_str());
		return;
	} else {
		log_i("created service %s\n", ARCOMMAND_RECEIVING_SERVICE_UUID.toString().c_str());
	}

	Serial.println();

	// Set non-ack receive commands (like battery, etc.)
	_chrReceiveCommand = receiveService->getCharacteristic(RECEIVE_DATA_UUID);
	if(_chrReceiveCommand == NULL){
		log_i("failed to find characteristic %s\n", RECEIVE_DATA_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", RECEIVE_DATA_UUID.toString().c_str());		
	}

	if(_chrReceiveCommand->canNotify()){
//		_chrReceiveCommand->registerForNotify(ARNetworkISR);
		_chrReceiveCommand->registerForNotify(onReceiveStatic);
		log_i("callback registered for receive commands\n");
	}

	// Set ack receive commands
	_chrReceiveCommandAck = receiveService->getCharacteristic(RECEIVE_DATA_WITH_ACK_UUID);
	if(_chrReceiveCommandAck == NULL){
		log_i("failed to find characteristic %s\n", RECEIVE_DATA_WITH_ACK_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", RECEIVE_DATA_WITH_ACK_UUID.toString().c_str());		
	}

	if(_chrReceiveCommandAck->canNotify()){
//		_chrReceiveCommandAck->registerForNotify(ARNetworkISR);
		_chrReceiveCommandAck->registerForNotify(onReceiveStatic);
		log_i("callback registered for receive commands with ack\n");
	}

	// Set receive ack from commands
	_chrReceiveAckCommand = receiveService->getCharacteristic(RECEIVE_ACK_DATA_UUID);
	if(_chrReceiveAckCommand == NULL){
		log_i("failed to find characteristic %s\n", RECEIVE_ACK_DATA_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", RECEIVE_ACK_DATA_UUID.toString().c_str());		
	}

	if(_chrReceiveAckCommand->canNotify()){
//		_chrReceiveAckCommand->registerForNotify(ARNetworkISR);
		_chrReceiveAckCommand->registerForNotify(onReceiveStatic);
		log_i("callback registered for acks from commands\n");
	}

	// Set receive ack from low latency
	_chrReceiveAckLowLatency = receiveService->getCharacteristic(RECEIVE_ACK_HIGH_PRIORITY_UUID);
	if(_chrReceiveAckLowLatency == NULL){
		log_i("failed to find characteristic %s\n", RECEIVE_ACK_HIGH_PRIORITY_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", RECEIVE_ACK_HIGH_PRIORITY_UUID.toString().c_str());		
	}

	if(_chrReceiveAckLowLatency->canNotify()){
//		_chrReceiveAckLowLatency->registerForNotify(ARNetworkISR);
		_chrReceiveAckLowLatency->registerForNotify(onReceiveStatic);
		log_i("callback registered for acks from low latency\n");
	}

	Serial.println();

}

void ARNetwork::_setHandshakeCharacteristics(){
	// Set handshake services
	BLERemoteService *sendService = _pClient->getService(NORMAL_BLE_FTP_SERVICE_UUID);
	if(sendService == NULL){
		log_i("failed to create service %s\n", NORMAL_BLE_FTP_SERVICE_UUID.toString().c_str());
		return;		
	} else {
		log_i("created service %s\n", NORMAL_BLE_FTP_SERVICE_UUID.toString().c_str());		
	}

//	Serial.println();

	_chrNormalFTPTransferring = sendService->getCharacteristic(NORMAL_FTP_TRANSFERRING_UUID);
	if(_chrNormalFTPTransferring == NULL){
		log_i("failed to find characteristic %s\n", NORMAL_FTP_TRANSFERRING_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", NORMAL_FTP_TRANSFERRING_UUID.toString().c_str());		
	}

	_chrNormalFTPGetting = sendService->getCharacteristic(NORMAL_FTP_GETTING_UUID);
	if(_chrNormalFTPGetting == NULL){
		log_i("failed to find characteristic %s\n", NORMAL_FTP_GETTING_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", NORMAL_FTP_GETTING_UUID.toString().c_str());		
	}

	_chrNormalFTPHandling = sendService->getCharacteristic(NORMAL_FTP_HANDLING_UUID);
	if(_chrNormalFTPHandling == NULL){
		log_i("failed to find characteristic %s\n", NORMAL_FTP_HANDLING_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", NORMAL_FTP_HANDLING_UUID.toString().c_str());		
	}

	sendService = _pClient->getService(UPDATE_BLE_FTP_UUID);
	if(sendService == NULL){
		log_i("failed to create service %s\n", UPDATE_BLE_FTP_UUID.toString().c_str());
		return;		
	} else {
		log_i("created service %s\n", UPDATE_BLE_FTP_UUID.toString().c_str());		
	}

//	Serial.println();

	_chrUpdateFTPTransferring = sendService->getCharacteristic(UPDATE_FTP_TRANSFERRING_UUID);
	if(_chrUpdateFTPTransferring == NULL){
		log_i("failed to find characteristic %s\n", UPDATE_FTP_TRANSFERRING_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", UPDATE_FTP_TRANSFERRING_UUID.toString().c_str());		
	}

	_chrUpdateFTPGetting = sendService->getCharacteristic(UPDATE_FTP_GETTING_UUID);
	if(_chrUpdateFTPGetting == NULL){
		log_i("failed to find characteristic %s\n", UPDATE_FTP_GETTING_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", UPDATE_FTP_GETTING_UUID.toString().c_str());		
	}

	_chrUpdateFTPHandling = sendService->getCharacteristic(UPDATE_FTP_HANDLING_UUID);
	if(_chrUpdateFTPHandling == NULL){
		log_i("failed to find characteristic %s\n", UPDATE_FTP_HANDLING_UUID.toString().c_str());
		return;
	} else {
		log_i("found characteristic %s\n", UPDATE_FTP_HANDLING_UUID.toString().c_str());		
	}
}

void ARNetwork::_sendFrame(){
	ARCommands::buffer_t bf;
	if(!_arCommands->_updateSend(&bf)){
		return;
	}
//	Serial.println("sending data");

	BLERemoteCharacteristic *remote = NULL;

	switch(bf.frameType){
		case ARCommands::FRAME_TYPE_ACK:
			remote = _chrSendAck;
			break;
		case ARCommands::FRAME_TYPE_DATA:
			remote = _chrSendCommand;
			break;
		case ARCommands::FRAME_TYPE_LOW_LATENCY:
			remote = _chrSendLowLatency;
			break;
		case ARCommands::FRAME_TYPE_DATA_WITH_ACK:
			remote = _chrSendCommandAck;
			break;
	}

	uint8_t length = bf.length;

	uint8_t toSend[length + 2];

	toSend[0] = bf.frameType;
	toSend[1] = bf.sequenceNumber;

	for(uint8_t i = 0; i < length; ++i){
		toSend[i + 2] = bf.data[i];
	}
/*
	Serial.printf("%i\tsending ", millis());
	for(uint8_t i = 0; i < length + 2; ++i){
		Serial.printf("%i ", toSend[i]);
	}
	Serial.println();
//	Serial.printf("to remote %s\n", remote->toString().c_str());
*/
	remote->writeValue(toSend, length + 2);

}
