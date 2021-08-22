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

#include "espARCommands.h"

ARCommands::ARCommands(){

}

ARCommands::~ARCommands(){

}

void ARCommands::update(){
	// Check if there is any received informations pending.
	if(!_receiveQueue.empty()){
		buffer_t bf = _receiveQueue.front();

		switch(bf.frameType){
			case FRAME_TYPE_DATA:
				_unpackFrame(bf.data, bf.length);
				break;
			case FRAME_TYPE_DATA_WITH_ACK:
				_populateSendQueue(FRAME_TYPE_ACK, &bf.sequenceNumber, 1);
				_unpackFrame(bf.data, bf.length);
				break;
			case FRAME_TYPE_ACK:
				_processAck(bf.data[0]);
				break;
			default:
				break;
		}
		_receiveQueue.pop();
	}
}

// 0/2/0
// common/settings/allSettings
void ARCommands::sendAllSettings(){
	const uint8_t length = 4;
	uint8_t toSend[length];
	toSend[0] = PROJECT_COMMON;
	toSend[1] = CMN_SETTINGS;
	_uint16tToBuffer(0, &toSend[2]);

	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);
}

// 2/0/0
// minidrone/piloting/flattrim
void ARCommands::sendFlatTrim(){
	if(_mdState.flyingState != FLYING_STATE_LANDED){
		return;
	}

	const uint8_t length = 4;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_PILOTING;
	_uint16tToBuffer(0, &toSend[2]);


	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);		
}

// 2/0/1
// minidrone/piloting/takeOff
void ARCommands::sendTakeOff(){
	if(_mdState.flyingState != FLYING_STATE_LANDED){
		return;
	}

	const uint8_t length = 4;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_PILOTING;
	_uint16tToBuffer(1, &toSend[2]);


	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);
}

// 2/0/2	(non ack)
// minidrone/piloting/PCMD
void ARCommands::sendPCMD(int8_t roll, int8_t pitch, int8_t yaw, int8_t gaz, bool rollPitchFlag){
	if(_mdState.flyingState != FLYING_STATE_FLYING &&
		_mdState.flyingState != FLYING_STATE_HOVERING){
		return;
	}

	const uint8_t length = 13;
	uint8_t toSend[length];
	memset(toSend, 0, length);
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_PILOTING;
	_uint16tToBuffer(2, &toSend[2]);
	toSend[4] = rollPitchFlag;
	toSend[5] = roll;
	toSend[6] = pitch;
	toSend[7] = yaw;
	toSend[8] = gaz;
	// last four bytes are a timestamp. We will see if really needed.
	_populateSendQueue(FRAME_TYPE_DATA, toSend, length);
}

// 2/0/3
// minidrone/piloting/landing
void ARCommands::sendLanding(){
	if(_mdState.flyingState == FLYING_STATE_LANDED){
		return;
	}

	const uint8_t length = 4;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_PILOTING;
	_uint16tToBuffer(3, &toSend[2]);


	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);
}

// 2/0/4	(low latency)
// minidrone/piloting/emergency
void ARCommands::sendEmergency(){
	const uint8_t length = 4;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_PILOTING;
	_uint16tToBuffer(4, &toSend[2]);


	_populateSendQueue(FRAME_TYPE_LOW_LATENCY, toSend, length, -1);
}

// 2/0/5
// minidrone/piloting/autoTakeOffMode
void ARCommands::sendAutoTakeOffMode(bool autoTakeOffMode){
	const uint8_t length = 5;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_PILOTING;
	_uint16tToBuffer(5, &toSend[2]);
	toSend[4] = (uint8_t)autoTakeOffMode;


	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);
}

// 2/0/8
// minidrone/piloting/togglePilotingMode
void ARCommands::sendTogglePilotingMode(){
	const uint8_t length = 4;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_PILOTING;
	_uint16tToBuffer(8, &toSend[2]);


	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);
}

// 2/8/0
// minidrone/pilotingSettings/MaxAltitude
void ARCommands::sendMaxAltitude(float maxAltitude){
	const uint8_t length = 8;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_PILOTING_SETTINGS;
	_uint16tToBuffer(0, &toSend[2]);
	_floatToBuffer(maxAltitude, &toSend[4]);

	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);
}

// 2/8/1
// minidrone/pilotingSettings/maxTilt
void ARCommands::sendMaxTilt(float maxTilt){
//	Serial.printf("max tilt : %f\n", maxTilt);
	const uint8_t length = 8;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_PILOTING_SETTINGS;
	_uint16tToBuffer(1, &toSend[2]);
	_floatToBuffer(maxTilt, &toSend[4]);

	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);
}

// 2/8/2
// minidrone/pilotingMode/bankedTurn
void ARCommands::sendBankedTurn(bool bankedTurn){
	const uint8_t length = 5;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_PILOTING_SETTINGS;
	_uint16tToBuffer(2, &toSend[2]);
	toSend[4] = (uint8_t)bankedTurn;

	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);
}

// 2/8/3
// minidrone/pilotingMode/maxThrottle
void ARCommands::sendMaxThrottle(float maxThrottle){
	const uint8_t length = 8;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_PILOTING_SETTINGS;
	_uint16tToBuffer(3, &toSend[2]);
	_floatToBuffer(maxThrottle, &toSend[4]);

	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);
}

// 2/8/4
// minidrone/pilotingSettings/preferredPilotingMode
void ARCommands::sendPreferredPilotingMode(uint8_t preferredPilotingMode){
	const uint8_t length = 8;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_PILOTING_SETTINGS;
	_uint16tToBuffer(4, &toSend[2]);
	_int32tToBuffer((int32_t)preferredPilotingMode, &toSend[4]);

	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);

}

// 2/1/0
// minidrone/speedSettings/maxVerticalSpeed
void ARCommands::sendMaxVerticalSpeed(float maxVerticalSpeed){
	const uint8_t length = 8;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_SPEED_SETTINGS;
	_uint16tToBuffer(0, &toSend[2]);
	_floatToBuffer(maxVerticalSpeed, &toSend[4]);

	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);
}

// 2/1/1
// minidrone/speedSettings/maxRotationalSpeed
void ARCommands::sendMaxRotationSpeed(float maxRotationSpeed){
	const uint8_t length = 8;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_SPEED_SETTINGS;
	_uint16tToBuffer(1, &toSend[2]);
	_floatToBuffer(maxRotationSpeed, &toSend[4]);

	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);
}

// 2/1/2
// minidrone/speedSettings/wheels
void ARCommands::sendWheels(bool wheels){
	const uint8_t length = 5;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_SPEED_SETTINGS;
	_uint16tToBuffer(2, &toSend[2]);
	toSend[4] = wheels;

	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);
}

// 2/1/3
// minidrone/speedSettings/maxHorizontalSpeed
void ARCommands::sendMaxHorizontalSpeed(float maxHorizontalSpeed){
	const uint8_t length = 8;
	uint8_t toSend[length];
	toSend[0] = PROJECT_MINIDRONE;
	toSend[1] = MD_SPEED_SETTINGS;
	_uint16tToBuffer(3, &toSend[2]);
	_floatToBuffer(maxHorizontalSpeed, &toSend[4]);

	_populateSendQueue(FRAME_TYPE_DATA_WITH_ACK, toSend, length);
}

uint8_t ARCommands::getBattery(){
	return _mdState.battery;
}

int16_t ARCommands::getRSSI(){
	return _mdState.rssi;
}

// Protected methods

void ARCommands::_populateReceiveQueue(frameType_t frameType, uint8_t* data, uint8_t length){
	buffer_t bf;

	bf.frameType = frameType;
	bf.sequenceNumber = data[1];

	data += 2;
	length -= 2;

	bf.length = length;

	memcpy(&bf.data, data, length);

	_receiveQueue.push(bf);
}

void ARCommands::_populateSendQueue(frameType_t frameType, uint8_t* data, uint8_t length, uint8_t retry){
	bool sendWithAck = false;
	std::queue<buffer_t> *queue;

	// Some kind of data require an ack from the drone, other don't.
	if(frameType == FRAME_TYPE_LOW_LATENCY){
		sendWithAck = true;
		queue = &_lowLatencyQueue;
	} else if(frameType == FRAME_TYPE_DATA_WITH_ACK){
		sendWithAck = true;
		queue = &_dataWithAckQueue;
	} else {
		sendWithAck = false;
		queue = &_dataQueue;
	}

	buffer_t bf;

	bf.frameType = frameType;
	bf.length = length;
	bf.retry = retry;
	bf.frameStatus = FRAME_STATUS_WAIT_SEND;
	memcpy(bf.data, data, length);

	// Each type of frame have its own sequence number.
	switch(frameType){
		case FRAME_TYPE_ACK:
			bf.sequenceNumber = _sequenceNumberAck++;
			break;
		case FRAME_TYPE_DATA:
			bf.sequenceNumber = _sequenceNumberData++;
			break;
		case FRAME_TYPE_LOW_LATENCY:
			bf.sequenceNumber = _sequenceNumberLowLatency++;
			break;
		case FRAME_TYPE_DATA_WITH_ACK:
			bf.sequenceNumber = _sequenceNumberDataWithAck++;
			break;
	}

	queue->push(bf);
}

bool ARCommands::_updateSend(buffer_t* bf){
	if(_updateSendLowLatency(bf)){
		return true;
	} else if(_updateSendData(bf)){
		return true;
	} else if(_updateSendDataWithAck(bf)){
		return true;
	} else {
		return false;
	}
}

bool ARCommands::_updateSendData(buffer_t* bf){
	if(_dataQueue.empty()) return false;
//	Serial.println("data to send");

	memcpy(bf, &_dataQueue.front(), sizeof(buffer_t));
	_dataQueue.pop();

	return true;
}

bool ARCommands::_updateSendLowLatency(buffer_t* bf){
	if(_lowLatencyQueue.empty()) return NULL;
	bool doReturn = false;

	uint32_t now = millis();

	buffer_t *front = &_lowLatencyQueue.front();

	if(front->frameStatus == FRAME_STATUS_WAIT_SEND){
		front->timestamp = now;
		front->frameStatus = FRAME_STATUS_WAIT_ACK;
		doReturn = true;
	} else if(front->frameStatus == FRAME_STATUS_WAIT_ACK){
		if((now - front->timestamp) > FRAME_TIMEOUT){
			front->timestamp = now;
			front->frameStatus = FRAME_STATUS_WAIT_ACK;
			doReturn = true;
		} else {
			doReturn = false;
		}
	}
	if(doReturn) memcpy(bf, front, sizeof(buffer_t));
	return doReturn;
}

bool ARCommands::_updateSendDataWithAck(buffer_t* bf){
	if(_dataWithAckQueue.empty()) return NULL;
	bool doReturn = false;

	uint32_t now = millis();

	buffer_t *front = &_dataWithAckQueue.front();

	if(front->frameStatus == FRAME_STATUS_WAIT_SEND){
		front->timestamp = now;
		front->frameStatus = FRAME_STATUS_WAIT_ACK;
		doReturn = true;
	} else if(front->frameStatus == FRAME_STATUS_WAIT_ACK){
		if((now - front->timestamp) > FRAME_TIMEOUT){
			if(--front->retry > 0){
				front->timestamp = now;
				front->frameStatus = FRAME_STATUS_WAIT_ACK;
				doReturn = true;
			} else {
				doReturn = false;
			}
		} else {
			doReturn = false;
		}
	}
	if(doReturn) memcpy(bf, front, sizeof(buffer_t));
	return doReturn;
}

void ARCommands::_processAck(uint8_t sequenceNumber){
//	buffer_t bf = _dataWithAckQueue.front();
	if(_lowLatencyQueue.front().sequenceNumber == sequenceNumber){
		_lowLatencyQueue.pop();
	} else if(_dataWithAckQueue.front().sequenceNumber == sequenceNumber){
		_dataWithAckQueue.pop();
	}
}

void ARCommands::_unpackFrame(uint8_t* data, size_t size){
//	Serial.printf("unpack frame %i : ", *data);
	size--;
	switch (*(data++)){
		case PROJECT_COMMON:
			// frame common
//			Serial.printf("common\n");
			_unpackCommon(data, size);
			break;
		case PROJECT_MINIDRONE:
			// frame minidrone
//			Serial.printf("Minidrone\n");
			_unpackMinidrone(data, size);
			break;
		default:
			// drop frame ?
//			Serial.printf("frame error\n");
			break;
	}
}

void ARCommands::_unpackCommon(uint8_t* data, size_t size){
//	Serial.printf("class : %i ", *data);
	size--;
	switch(*(data++)){
		case CMN_COMMON_STATE:
			_processCommonCommonState(data, size);
			break;
		case CMN_HEADLIGHTS_STATE:
			_processCommonHeadlightsState(data, size);
			break;
		default:
//			Serial.printf("unused class\n");
			_processUnused(data, size);
			break;
	}

//	Serial.println();
}

void ARCommands::_unpackMinidrone(uint8_t* data, size_t size){
 //	Serial.printf("class : %i ", *data);
	size--;
	switch(*(data++)){
		case MD_PILOTING_STATE:
			_processMinidronePilotingState(data, size);
			break;
		case MD_PILOTING_SETTINGS_STATE:
			_processMinidronePilotingSettingsState(data, size);
			break;
		case MD_SPEED_SETTINGS_STATE:
			_processMinidroneSpeedSettingsState(data, size);
			break;
		default:
//			Serial.printf("unused class\n");
			_processUnused(data, size);
			break;
	}
}

void ARCommands::_processCommonCommonState(uint8_t* data, size_t size){
	size -= 2;
	int16_t command = _bufferToUint16t(data);
	data += 2;
	switch(command){
		case 1:
			_mdState.battery = *data;
			Serial.printf("battery : %i\n", _mdState.battery);
			break;
		case 7:
			_mdState.rssi = _bufferToInt16t(data);
//			Serial.printf("RSSI : %i\n", _mdState.rssi);
			break;
		case 16:																			// Not sure if this one is implemented on BLE drones.
			_mdState.linkSignalQuality = *data;
//			Serial.printf("link signal quality : %i\n", _mdState.linkSignalQuality);
			break;
		default:
			break;
	}
}

void ARCommands::_processCommonHeadlightsState(uint8_t* data, size_t size){
	size -= 2;
	int16_t command = _bufferToUint16t(data);
	data += 2;
	switch(command){
		case 0:
			_mdState.headlightLeft = *(++data);
			_mdState.headlightRight = *data;
			break;
		default:
			break;
	}
}

void ARCommands::_processMinidronePilotingState(uint8_t* data, size_t size){
	size -= 2;
	int16_t command = _bufferToUint16t(data);
	data += 2;
	switch(command){
		case 1:
			_mdState.flyingState = (flyingState_t)_bufferToInt32t(data);
//			Serial.printf("flying state : %i\n", _mdState.flyingState);
			break;
		case 2:
			_mdState.alertState = (alertState_t)(_bufferToInt32t(data));
			break;
		case 3:
			_mdState.autoTakeOffMode = (bool)(*data);
			break;
		case 6:
			_mdState.pilotingMode = (pilotingMode_t)_bufferToInt32t(data);
//			Serial.printf("piloting mode : %i\n", _mdState.pilotingMode);
			break;
		default:
			break;
	}
}

void ARCommands::_processMinidronePilotingSettingsState(uint8_t* data, size_t size){
	size -= 2;
	int16_t command = _bufferToUint16t(data);
	data += 2;
	switch(command){
		case 0:
			_mdState.maxAltitude = _bufferToFloat(data);
//			_bufferToFloat(data, true);
//			_bufferToFloat(data, true);
			break;
		case 1:
			_mdState.maxTilt = _bufferToFloat(data);
			
/*			Serial.printf("max tilt : %f (min : %f, max %f)\n", _mdState.maxTilt,
												_bufferToFloat(data),
												_bufferToFloat(data);
*/			break;
		case 2:
			_mdState.bankedTurn = *data;
//			Serial.printf("banked turn : %i\n", _mdState.bankedTurn);
			break;
		case 3:
			_mdState.maxThrottle = _bufferToFloat(data);
			break;
		case 4:
			_mdState.preferredPilotingMode = (pilotingMode_t)_bufferToInt32t(data);
			break;
		default:
			break;
	}
}

void ARCommands::_processMinidroneSpeedSettingsState(uint8_t* data, size_t size){
	size -= 2;
	int16_t command = _bufferToUint16t(data);
	data += 2;
	switch(command){
		case 0:
		// There are three settings : current vertical speed, min and max.
		// For now only current is saved.
		// the two others could be used to map the user input setting (controller potentiometer) to setting range.
			_mdState.maxVerticalSpeed = _bufferToFloat(data);
/*			Serial.printf("max vertical speed : %f (min : %f, max %f)\n", _mdState.maxVerticalSpeed,
																			_bufferToFloat(data),
																			_bufferToFloat(data));

*/			break;
		case 1:
		// Same here.
			_mdState.maxRotationSpeed = _bufferToFloat(data);
/*			Serial.printf("max rotation speed : %f (min : %f, max %f)\n", _mdState.maxRotationSpeed,
																			_bufferToFloat(data),
																			_bufferToFloat(data));

*/			break;
		case 2:
		// Wheels. Unused
			break;
		case 3:
			_mdState.maxHorizontalSpeed = _bufferToFloat(data);
//			_bufferToFloat(data);
//			_bufferToFloat(data);
			break;
		default:
			break;
	}
}

void ARCommands::_processUnused(uint8_t* data, size_t size){
//	Serial.printf("data size : %i\n", size);
	uint16_t command = _bufferToUint16t(data);
	data += 2;
	size -= 2;
//	Serial.printf("command : %i, unused data : ", command);

	for(size_t i = 0; i < size; ++i){
//		Serial.printf("%i ", data[i]);
	}

//	Serial.println();
}

void ARCommands::_int16tToBuffer(int16_t value, uint8_t* buffer){
	const uint8_t size = 2;
	memcpy(buffer, &value, size);
}

void ARCommands::_uint16tToBuffer(uint16_t value, uint8_t* buffer){
	const uint8_t size = 2;
	memcpy(buffer, &value, size);
}

void ARCommands::_int32tToBuffer(int32_t value, uint8_t* buffer){
	const uint8_t size = 4;
	memcpy(buffer, &value, size);
}

void ARCommands::_uint32tToBuffer(uint32_t value, uint8_t* buffer){
	const uint8_t size = 4;
	memcpy(buffer, &value, size);
}

void ARCommands::_int64tToBuffer(int64_t value, uint8_t* buffer){
	const uint8_t size = 8;
	memcpy(buffer, &value, size);
}

void ARCommands::_uint64tToBuffer(uint64_t value, uint8_t* buffer){
	const uint8_t size = 8;
	memcpy(buffer, &value, size);
}

void ARCommands::_floatToBuffer(float value, uint8_t* buffer){
	const uint8_t size = 4;
	memcpy(buffer, &value, size);
}

void ARCommands::_doubleToBuffer(double value, uint8_t* buffer){
	const uint8_t size = 8;
	memcpy(buffer, &value, size);
}

int16_t ARCommands::_bufferToInt16t(uint8_t* buffer){
	const uint8_t size = 2;
	int16_t value = 0;
	memcpy(&value, buffer, size);
//	if(increment) buffer += size;
	return value;
}

uint16_t ARCommands::_bufferToUint16t(uint8_t* buffer){
	const uint8_t size = 2;
	uint16_t value = 0;
	memcpy(&value, buffer, size);
//	if(increment) buffer += size;
	return value;
}

int32_t ARCommands::_bufferToInt32t(uint8_t* buffer){
	const uint8_t size = 4;
	int32_t value = 0;
	memcpy(&value, buffer, size);
//	if(increment) buffer += size;
	return value;
}

uint32_t ARCommands::_bufferToUint32t(uint8_t* buffer){
	const uint8_t size = 4;
	uint32_t value = 0;
	memcpy(&value, buffer, size);
//	if(increment) buffer += size;
	return value;
}

int64_t ARCommands::_bufferToInt64t(uint8_t* buffer){
	const uint8_t size = 8;
	int64_t value = 0;
	memcpy(&value, buffer, size);
//	if(increment) buffer += size;
	return value;
}

uint64_t ARCommands::_bufferToUint64t(uint8_t* buffer){
	const uint8_t size = 8;
	uint64_t value = 0;
	memcpy(&value, buffer, size);
//	if(increment) buffer += size;
	return value;
}

float ARCommands::_bufferToFloat(uint8_t* buffer){
	const uint8_t size = 4;
	float value = 0;
	memcpy(&value, buffer, size);
//	if(increment) buffer += size;
	return value;
}

double ARCommands::_bufferToDouble(uint8_t* buffer){
	const uint8_t size = 8;
	double value = 0;
	memcpy(&value, buffer, size);
//	if(increment) buffer += size;
	return value;
}

