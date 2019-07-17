/*
 * File:   UsbManager.cpp
 * Author: hsaturn
 *
 * Created on 4 avril 2019, 21:43
 */

#include "UsbManager.h"
#include <iostream>

using namespace std;

namespace core
{
   bool UsbManager::mbInit=false;
   libusb_device **UsbManager::devs;

   UsbManager::UsbManager()
   {
      cout << "UsbManager initialization..." << endl;
      int r = libusb_init(NULL);
      if (r < 0)
      {
         cerr << "ERROR: UsbManager, unable to init usb." << endl;   // TODO
         return;
      }

      cout << "UsbManager init ok." << endl;
   }

	libusb_device ** UsbManager::getDevices()
	{
		if (mbInit)
      {
         libusb_free_device_list(devs, 1);
		}
      auto cnt = libusb_get_device_list(NULL, &devs);
      if (cnt < 0)
      {
         cerr << "Error: UsbManager, unable to enumerate usb devices (" << cnt << ")" << endl;
         libusb_exit(NULL);
         return nullptr;
      }
      mbInit=true;
		return devs;
	}

   UsbManager::~UsbManager()
   {
      if (mbInit)
      {
         libusb_free_device_list(devs, 1);
         libusb_exit(NULL);
      }
   }

   UsbManager& UsbManager::getInstance()
   {
      static UsbManager instance;
      return instance;
   }

}
