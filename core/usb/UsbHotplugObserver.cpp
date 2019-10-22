/*
 * File:   UsbHotplugObserver.cpp
 * Author: hsaturn
 *
 * Created on 4 avril 2019, 22:31
 */

#include "UsbHotplugObserver.h"
#include "UsbManager.h"
#include <iostream>

using namespace std;

namespace core
{
   UsbHotplugObserver::UsbHotplugObserver(const VendorProduct& vp)
      : mVendorProduct(vp)
   {
      auto& usb=core::UsbManager::getInstance();

      if (usb.isOk())
      {
         UsbManager::registerHotplug(this);
      }
   }

   UsbHotplugObserver::~UsbHotplugObserver()
   {
      auto& usb=core::UsbManager::getInstance();

      if (usb.isOk())
      {
         UsbManager::unregisterHotplug(this);
      }
   }

}

