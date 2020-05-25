/*
obs-midi

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include "device-manager.h"

DeviceManager::DeviceManager()
{
	rtMidi = new rtmidi::midi_in();
	MO = new rtmidi::midi_out();
}

DeviceManager::~DeviceManager()
{
	rtMidi->~midi_in();
	MO->~midi_out();
}

/* Load the Device Manager from saved Config Store data.
 * This method is called from Config. Should only be called once on runtime
 */
void DeviceManager::Load(obs_data_t* data)
{
	vector <string> portsList = GetPortsList();
	obs_data_array_t* devicesData = obs_data_get_array(data, "devices");
	size_t deviceCount = obs_data_array_count(devicesData);
	for (size_t i = 0; i < deviceCount; i++)
	{
		obs_data_t* deviceData = obs_data_array_item(devicesData, i);
		MidiAgent* device = new MidiAgent();
		device->Load(deviceData);
		midiAgents.push_back(device);

		if (device->isEnabled())
		{
			int portNumber = GetPortNumberByDeviceName(device->GetName().c_str());
			if (portNumber != -1)
			{
				device->OpenPort(portNumber);
			}
		}
	}
}


/* Returns vector list of Port Names 
 */
vector <string> DeviceManager::GetPortsList()
{
	vector <string> ports;
	int portCount = rtMidi->get_port_count();
	for (int i = 0; i < portCount; i++)
	{
		ports.push_back(rtMidi->get_port_name(i));
	}
	return ports;
}

/* Returns the port number of the specified device.
 * If the device isn't found (possibly due to being disconnected), returns -1
 */
int DeviceManager::GetPortNumberByDeviceName(const char* deviceName)
{
	vector <string> portsList = GetPortsList();
	auto it = find(portsList.begin(), portsList.end(), deviceName);
	if (it != portsList.end()) {
		return distance(portsList.begin(), it);
	}
	else {
		return -1;
	}
}


vector<MidiAgent*> DeviceManager::GetActiveMidiDevices()
{
	return midiAgents;
}

MidiAgent* DeviceManager::GetMidiDeviceByName(const char* deviceName)
{
	for (int i = 0; i < midiAgents.size(); i++)
	{
		if (midiAgents.at(i)->GetName() == deviceName)
		{
			return midiAgents.at(i);
		}
	}
	return NULL;
}

vector <MidiHook*> DeviceManager::GetMidiHooksByDeviceName(const char* deviceName)
{
	auto device = GetMidiDeviceByName(deviceName);
	if (device != NULL)
	{
		return device->GetMidiHooks();
	}
	throw ("no midi hooks for this device");
}

/* Registers a midi device.
 * Will create, store and enable a midi device.
*/
void DeviceManager::RegisterMidiDevice(int port)
{
	MidiAgent* midiIn = new MidiAgent();
	midiIn->OpenPort(port);

	midiAgents.push_back(midiIn);
}


/* Get this Device Manager state as OBS Data. (includes devices and their midi hooks)
* This is needed to Serialize the state in the config.
* https://obsproject.com/docs/reference-settings.html
*/
obs_data_t* DeviceManager::GetData()
{
	obs_data_t* data = obs_data_create();

	obs_data_array_t* deviceData = obs_data_array_create();
	for (int i = 0; i < midiAgents.size(); i++)
	{
		obs_data_array_push_back(deviceData, midiAgents.at(i)->GetData());
	}

	obs_data_set_array(data, "devices", deviceData);
	return data;
}

void DeviceManager::SendMidi(QString mtype, int channel, int norc, int value)

{
	rtmidi::message hello = new rtmidi::message();
	if (mtype == "control_change"){
		hello.control_change(channel, norc, value);
	} else if (mtype == "note_on") {
		hello.note_on(channel, norc, value);
	} else if (mtype == "note_off") {
		hello.note_off(channel, norc, value);
	}
	if (hello.size() != 0) {
		MO->send_message(hello);
	}
	//***Need to add message Deletion here***//

}
