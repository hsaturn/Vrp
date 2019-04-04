/* 
 * File:   AutoDetect.cpp
 * Author: hsaturn
 * 
 * Created on 4 avril 2019, 20:49
 */
#include "usb/UsbManager.h"
#include "AutoDetect.hpp"
#include <libusb-1.0/libusb.h>                          
#include <iostream>                       
#include <iomanip>                        
using namespace std; 

namespace slicer
{

   AutoDetect::AutoDetect()
   {
      auto& usb=core::UsbManager::getInstance();
      
      libusb_device **devs = usb.getDevices();
      libusb_device *dev;
      int i = 0, j = 0;
      uint8_t path[8]; 

      while ((dev = devs[i++]) != NULL)
      {
         struct libusb_device_descriptor desc;
         int r = libusb_get_device_descriptor(dev, &desc);
         if (r < 0)
         {
            cerr << "Error: AutoDetect, failed to get device descriptor" << endl;
            break;
         }   
         if (desc.idVendor == 0x403 && desc.idProduct == 0x6001)
         {
            cout << "Printer detected, bus " << (int)libusb_get_bus_number(dev) << ", device " << (int)libusb_get_device_address(dev) << endl;
            cout << " VENDOR  " << hex << desc.idVendor << endl;
            cout << " PRODUCT " << hex << desc.idProduct << endl;

            r = libusb_get_port_numbers(dev, path, sizeof(path));
            if (r > 0)
            {
               cout << "  Path: " << path[0];
               for (j = 1; j < r; j++)
                  cout << '.' << path[j];
               cout << endl;
            }
         }
      }
      cout << dec;
   }
}