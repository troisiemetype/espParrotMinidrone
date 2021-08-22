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

#ifndef ESP_PARROT_AR_COMMANDS_H
#define ESP_PARROT_AR_COMMANDS_H

#include <Arduino.h>

#include <queue>

//using namespace std;

class ARCommands{
public:

	enum frameType_t{
		FRAME_TYPE_ACK = 1,
		FRAME_TYPE_DATA,
		FRAME_TYPE_LOW_LATENCY,
		FRAME_TYPE_DATA_WITH_ACK,
	};

	enum frameStatus_t{
		FRAME_STATUS_WAIT_SEND = 0,
		FRAME_STATUS_WAIT_ACK,
	};

	enum{
		PROJECT_COMMON 					= 0,
		PROJECT_MINIDRONE				= 2,
	} ARProjects;

	enum{
		CMN_NETWORK 					= 0,
		CMN_NETWORK_EVENT 				= 1,
		CMN_SETTINGS 					= 2,
		CMN_SETTINGS_STATE 				= 3,
		CMN_COMMON 						= 4,
		CMN_COMMON_STATE 				= 5,
		CMN_OVERHEAT 					= 6,
		CMN_OVERHEAT_STATE 				= 7,
		CMN_CONTROLLER 					= 8,
		CMN_WIFI_SETTINGS 				= 9,
		CMN_WIFI_SETTINGS_STATE 		= 10,
		CMN_MAVLINK 					= 11,
		CMN_MAVLINK_STATE 				= 12,
		CMN_CALIBRATION 				= 13,
		CMN_CALIBRATION_STATE 			= 14,
		CMN_CAMERA_SETTINGS_STATE 		= 15,
		CMN_GPS 						= 16,
		CMN_FLIGHT_PLAN_EVENT 			= 17,
		CMN_FLIGHT_PLAN_STATE 			= 18,
		CMN_AR_LIBS_VERSION_STATE 		= 19,
		CMN_AUDIO 						= 20,
		CMN_AUDIO_STATE 				= 21,
		CMN_HEADLIGHTS 					= 22,
		CMN_HEADLIGHTS_STATE 			= 23,
		CMN_ANIMATIONS 					= 24,
		CMN_ANIMATIONS_STATE 			= 25,
		CMN_ACCESSORY 					= 26,
		CMN_ACCESSORY_STATE 			= 27,
		CMN_CHARGER 					= 28,
		CMN_CHARGER_STATE 				= 29,
		CMN_RUN_STATE 					= 30,
		CMN_FACTORY 					= 31,
		CMN_FLIGHT_PLAN_SETTINGS 		= 32,
		CMN_FLIGHT_PLAN_SETTINGS_STATE 	= 33,
		CMN_UPDATE_STATE 				= 34,
	} ARCommonClasses;

	enum{
		MD_PILOTING 					= 0,
		MD_SPEED_SETTINGS 				= 1,
		MD_MEDIA_RECORD_EVENT 			= 2,
		MD_PILOTING_STATE 				= 3,
		MD_ANIMATIONS 					= 4,
		MD_SPEED_SETTINGS_STATE 		= 5,
		MD_MEDIA_RECORD 				= 6,
		MD_MEDIA_RECORD_STATE 			= 7,
		MD_PILOTING_SETTINGS 			= 8,
		MD_PILOTING_SETTINGS_STATE 		= 9,
		MD_SETTINGS 					= 10,
		MD_SETTINGS_STATE 				= 11,
		MD_FLOOD_CONTROL_STATE 			= 12,
		MD_GPS 							= 13,
		MD_CONFIGURATION 				= 14,
		MD_USB_ACCESSORY_STATE 			= 15,
		MD_USB_ACCESSORY 				= 16,
		MD_REMOTE_CONTROLLER 			= 17,
		MD_NAVIGATION_DATA_STATE 		= 18,
		MD_MINICAM_STATE 				= 19,
		MD_VIDEO_SETTINGS 				= 20,
		MD_VIDEO_SETTINGS_STATE 		= 21,
		MD_MINICAM 						= 24,
		MD_REMOTE_CONTROLLER_STATE 		= 25,
	} ARMinidroneClasses;

	struct buffer_t{
		frameType_t frameType;
		uint8_t sequenceNumber;
		uint8_t data[18];
		uint8_t length;
		uint32_t timestamp;
		int8_t retry;
		frameStatus_t frameStatus;
	};

	enum flyingState_t{
		FLYING_STATE_LANDED,
		FLYING_STATE_TAKINGOFF,
		FLYING_STATE_HOVERING,
		FLYING_STATE_FLYING,
		FLYING_STATE_LANDING,
		FLYING_STATE_EMERGENCY,
		FLYING_STATE_ROLLING,
		FLYING_STATE_INIT,
	};

	enum alertState_t{
		ALERT_STATE_NONE,
		ALERT_STATE_USER,
		ALERT_STATE_CUTOUT,
		ALERT_STATE_CRITICAL_BATTERY,
		ALERT_STATE_LOW_BATTERY,
	};

	enum pilotingMode_t{
		PILOTING_MODE_EASY,
		PILOTING_MODE_MEDIUM,
		PILOTING_MODE_DIFFICULT,
	};

	struct minidroneState_t{
		uint8_t battery;									// 0/5/1 		common/CommonState/batteryStateChanged

		int16_t rssi;										// 0/5/7		common/CommonState/wifiSignalChanged
		uint8_t linkSignalQuality;							// 0/5/15		common/CommonState/linkSignalQuality

		uint8_t headlightLeft;								// 0/23/0		common/HeadlightsState/intensityChanged
		uint8_t headlightRight;

		flyingState_t flyingState;							// 2/3/1 		minidrone/PilotingState/flyingStateChanged
		alertState_t alertState;							// 2/3/2 		minidrone/PilotingState/AlertStateChanged
		bool autoTakeOffMode;								// 2/3/3 		minidrone/PilotingState/AutoTakeOffModeChanged
		pilotingMode_t pilotingMode;						// 2/3/6 		minidrone/PilotingState/pilotingModeChanged		// See also PreferredPilotingMode

		float maxAltitude; 									// 2/8/0 		minidrone/PilotingSettings/MaxAltitude 		min : 2m, max : 10m
		float maxTilt; 										// 2/8/1 		minidrone/PilotingSettings/MaxTilt 			Default : 15° min : 5°, max : 25°
		bool bankedTurn;									// 2/8/2 		minidrone/PilotingSettings/BankedTurn
		float maxThrottle;									// 2/8/3 		minidrone/PilotingSettings/MaxThrottle 		between 0 and 1
		pilotingMode_t preferredPilotingMode;				// 2/8/4 		minidrone/PilotingSettings/PreferredPilotingMode

		float maxVerticalSpeed; 							// 2/1/0 		minidrone/SpeedSettings/MaxVerticalSpeed 	Default : 0.7m/s, min : 0.5, max : 2.0
		float maxRotationSpeed; 							// 2/1/1 		minidrone/SpeedSettings/MaxRotationSpeed 	Default : 185°/s, min : 50, max : 360
		float maxHorizontalSpeed; 							// 2/1/3 		minidrone/SpeedSettings/MaxHorizontalSpeed  ony used when maxTilt is not used.
		// todo : use Navigation data state ?				// 2/18/x
		// piloting settings state
	};

	uint16_t FRAME_TIMEOUT = 200;

	friend class ARNetwork;

	ARCommands();
	~ARCommands();

	void update();

	void sendAllSettings();

	void sendFlatTrim();
	void sendTakeOff();
	void sendPCMD(int8_t roll, int8_t pitch, int8_t yaw, int8_t gaz, bool rollPitchFlag = true);
	void sendLanding();
	void sendEmergency();
	void sendAutoTakeOffMode(bool autoTakeOffMode);
	void sendTogglePilotingMode();

	void sendMaxAltitude(float maxAltitude);
	void sendMaxTilt(float maxTilt);
	void sendBankedTurn(bool bankedTurn);
	void sendMaxThrottle(float maxThrottle);
	void sendPreferredPilotingMode(uint8_t preferredPilotingMode);

	void sendMaxVerticalSpeed(float maxVerticalSpeed);
	void sendMaxRotationSpeed(float maxRotationSpeed);
	void sendWheels(bool wheels);
	void sendMaxHorizontalSpeed(float maxHorizontalSpeed);

	uint8_t getBattery();
	int16_t getRSSI();

protected:
	void _populateReceiveQueue(frameType_t frameType, uint8_t* data, uint8_t length);
	void _populateSendQueue(frameType_t frameType, uint8_t* data, uint8_t length, uint8_t retry = 5);

	bool _updateSend(buffer_t* bf);
	bool _updateSendData(buffer_t* bf);
	bool _updateSendLowLatency(buffer_t* bf);
	bool _updateSendDataWithAck(buffer_t* bf);

	void _processAck(uint8_t sequenceNumber);

	void _unpackFrame(uint8_t* data, size_t size);
	void _unpackCommon(uint8_t* data, size_t size);
	void _unpackMinidrone(uint8_t* data, size_t size);

	void _processCommonCommonState(uint8_t* data, size_t size);
	void _processCommonHeadlightsState(uint8_t* data, size_t size);

	void _processMinidronePilotingState(uint8_t* data, size_t size);
	void _processMinidronePilotingSettingsState(uint8_t* data, size_t size);
	void _processMinidroneSpeedSettingsState(uint8_t* data, size_t size);

	void _processUnused(uint8_t* data, size_t size);


	void _int16tToBuffer(int16_t value, uint8_t* buffer);
	void _uint16tToBuffer(uint16_t value, uint8_t* buffer);

	void _int32tToBuffer(int32_t value, uint8_t* buffer);
	void _uint32tToBuffer(uint32_t value, uint8_t* buffer);

	void _int64tToBuffer(int64_t value, uint8_t* buffer);
	void _uint64tToBuffer(uint64_t value, uint8_t* buffer);

	void _floatToBuffer(float value, uint8_t* buffer);
	void _doubleToBuffer(double value, uint8_t* buffer);


	int16_t _bufferToInt16t(uint8_t* buffer);
	uint16_t _bufferToUint16t(uint8_t* buffer);

	int32_t _bufferToInt32t(uint8_t* buffer);
	uint32_t _bufferToUint32t(uint8_t* buffer);

	int64_t _bufferToInt64t(uint8_t* buffer);
	uint64_t _bufferToUint64t(uint8_t* buffer);

	float _bufferToFloat(uint8_t* buffer);
	double _bufferToDouble(uint8_t* buffer);

	std::queue<buffer_t> _dataQueue;
	std::queue<buffer_t> _lowLatencyQueue;
	std::queue<buffer_t> _dataWithAckQueue;
	std::queue<buffer_t> _receiveQueue;

	uint8_t _sequenceNumberAck;
	uint8_t _sequenceNumberData;
	uint8_t _sequenceNumberLowLatency;
	uint8_t _sequenceNumberDataWithAck;

	minidroneState_t _mdState;

};

#endif