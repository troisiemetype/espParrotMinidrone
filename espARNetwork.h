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

#ifndef ESP_PARROT_AR_NETWORK_H
#define ESP_PARROT_AR_NETWORK_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#include "espARCommands.h"

class ARNetwork;

class ARDeviceCallbacks : public BLEAdvertisedDeviceCallbacks{
public:
	ARDeviceCallbacks(ARNetwork* network);
	void onResult(BLEAdvertisedDevice advertisedDevice);
private:
	ARNetwork *_network;
};

class ARClientCallbacks : public BLEClientCallbacks{
public:
	ARClientCallbacks(ARNetwork* network);
	void onConnect(BLEClient* client);
	void onDisconnect(BLEClient* client);
private:
	ARNetwork *_network;
};

class ARNetwork{
public:

	friend class ARDeviceCallbacks;
	friend class ARClientCallbacks;

	static BLEUUID ARCOMMAND_SENDING_SERVICE_UUID;

	static BLEUUID SEND_DATA_UUID;
	static BLEUUID SEND_DATA_WITH_ACK_UUID;
	static BLEUUID SEND_HIGH_PRIORITY_UUID;
	static BLEUUID SEND_ACK_UUID;

	static BLEUUID ARCOMMAND_RECEIVING_SERVICE_UUID;

	static BLEUUID RECEIVE_DATA_UUID;
	static BLEUUID RECEIVE_DATA_WITH_ACK_UUID;
	static BLEUUID RECEIVE_ACK_DATA_UUID;
	static BLEUUID RECEIVE_ACK_HIGH_PRIORITY_UUID;

	static BLEUUID NORMAL_BLE_FTP_SERVICE_UUID;

	static BLEUUID NORMAL_FTP_TRANSFERRING_UUID;
	static BLEUUID NORMAL_FTP_GETTING_UUID;
	static BLEUUID NORMAL_FTP_HANDLING_UUID;

	static BLEUUID UPDATE_BLE_FTP_UUID;

	static BLEUUID UPDATE_FTP_TRANSFERRING_UUID;
	static BLEUUID UPDATE_FTP_GETTING_UUID;
	static BLEUUID UPDATE_FTP_HANDLING_UUID;
/*
	static BLEUUID ARCOMMAND_SENDING_SERVICE_UUID;		// Send service

	static BLEUUID SEND_DATA_UUID;						// Send command without ack
	static BLEUUID SEND_DATA_WITH_ACK_UUID;				// Send command with ack
	static BLEUUID SEND_HIGH_PRIORITY_UUID;				// Send low latency (with ack) (send before other commands)
	static BLEUUID SEND_ACK_UUID;						// Send ack for received command

	static BLEUUID ARCOMMAND_RECEIVING_SERVICE_UUID;	// receive service

	static BLEUUID RECEIVE_DATA_UUID;					// Receive command without ack
	static BLEUUID RECEIVE_DATA_WITH_ACK_UUID;			// Receive command with ack
	static BLEUUID RECEIVE_ACK_DATA_UUID;				// ack for sent command
	static BLEUUID RECEIVE_ACK_HIGH_PRIORITY_UUID;		// ack for sent low latency

	// Handshake UUID. Not needed with my rolling spider
	static BLEUUID NORMAL_BLE_FTP_SERVICE_UUID;

	static BLEUUID NORMAL_FTP_TRANSFERRING_UUID;
	static BLEUUID NORMAL_FTP_GETTING_UUID;
	static BLEUUID NORMAL_FTP_HANDLING_UUID;

	static BLEUUID UPDATE_BLE_FTP_UUID;

	static BLEUUID UPDATE_FTP_TRANSFERRING_UUID;
	static BLEUUID UPDATE_FTP_GETTING_UUID;
	static BLEUUID UPDATE_FTP_HANDLING_UUID;
*/
	ARNetwork(ARCommands* arCommands);
	~ARNetwork();

	void init();

	bool checkConnection();
	void shakeHands();

	void update();

	void enumerateServices();

	int getRSSI();

	static void onReceiveStatic(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* data, size_t length, bool isNotify);
	void onReceive(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* data, size_t length, bool isNotify);

protected:
	void _onReceiveCommand(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* data, size_t length, bool isNotify);
	void _onReceiveCommandAck(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* data, size_t length, bool isNotify);
	void _onReceiveAckCommand(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* data, size_t length, bool isNotify);
	void _onReceiveAckLowLatency(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* data, size_t length, bool isNotify);

	bool _scan();
	void _connect();
	void _firstConnect();

	void _setSendCharacteristics();
	void _setReceiveCharacteristics();
	void _setHandshakeCharacteristics();

	void _sendFrame();

	static ARNetwork *_pInstance;
	ARCommands *_arCommands;

	BLEScan *_pBLEScan = NULL;
	BLEClient *_pClient = NULL;

	BLERemoteCharacteristic *_chrSendCommand = NULL;
	BLERemoteCharacteristic *_chrSendCommandAck = NULL;
	BLERemoteCharacteristic *_chrSendLowLatency = NULL;
	BLERemoteCharacteristic *_chrSendAck = NULL;

	BLERemoteCharacteristic *_chrReceiveCommand = NULL;
	BLERemoteCharacteristic *_chrReceiveCommandAck = NULL;
	BLERemoteCharacteristic *_chrReceiveAckCommand = NULL;
	BLERemoteCharacteristic *_chrReceiveAckLowLatency = NULL;

	// Those seems to be needed in some cases, but rolling spider goes well without
	BLERemoteCharacteristic *_chrNormalFTPTransferring = NULL;
	BLERemoteCharacteristic *_chrNormalFTPGetting = NULL;
	BLERemoteCharacteristic *_chrNormalFTPHandling = NULL;

	BLERemoteCharacteristic *_chrUpdateFTPTransferring = NULL;
	BLERemoteCharacteristic *_chrUpdateFTPGetting = NULL;
	BLERemoteCharacteristic *_chrUpdateFTPHandling = NULL;


	BLEAdvertisedDevice *_minidrone = NULL;
	const uint8_t BLE_SCAN_TIME = 5;
	bool _connected;

};

#endif