/*
 * File:   UsbManager.h
 * Author: hsaturn
 *
 * Created on 4 avril 2019, 21:43
 */

#pragma once
#include <libusb-1.0/libusb.h>

namespace core
{
   class UsbManager
   {
      public:
         UsbManager(const UsbManager& orig) = delete;
         virtual ~UsbManager();

         libusb_device ** getDevices();
         static UsbManager& getInstance();

      private:
         UsbManager();

         static libusb_device **devs;
         static bool mbInit;
   };

}