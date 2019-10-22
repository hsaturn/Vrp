/*
 * File:   UsbManager.cpp
 * Author: hsaturn
 *
 * Created on 4 avril 2019, 21:43
 */

#include "UsbManager.h"
#include "UsbHotplugObserver.h"
#include <iostream>

using namespace std;

namespace core
{
   bool UsbManager::mbInit=false;
   libusb_device** UsbManager::devs=nullptr;
   UsbManager::Container UsbManager::mapObservers;
   VendorProduct UsbManager::mLastDeviceEvent(0,0);

   UsbManager::UsbManager()
   {
      cout << "UsbManager initialization..." << endl;
      int r = libusb_init(NULL);
      if (r < 0)
      {
         cerr << "ERROR: UsbManager, unable to init usb." << endl;   // TODO
      }
      mbInit=true;

      initHotplug();

      cout << "UsbManager init ok." << endl;
   }

   libusb_device** UsbManager::getDevices()
   {
      if (!mbInit) return nullptr;

      if (devs == nullptr)
      {
         auto cnt = libusb_get_device_list(NULL, &devs);
         if (cnt < 0)
         {
            cerr << "Error: UsbManager, unable to enumerate usb devices (" << cnt << ")" << endl;
         }
      }
      return devs;
   }

   void UsbManager::update()
   {
      if (!mbInit) return;

      static struct timeval tv= {0,0};
      int r = libusb_handle_events_timeout(NULL, &tv);
      if (r < 0)
      {
         static int last_r = 0;
         if (last_r != r)
         {
            cerr << "Error: usbManager update failed (" << r << ')' << endl;
            last_r = r;
         }
      }
   }

   UsbManager::~UsbManager()
   {
      if (mbInit)
      {
         for(auto& [vp, pHotPlug] : mapObservers)
         {
            pHotPlug->onTerminate();
         }
         mapObservers.clear();
         if (devs) libusb_free_device_list(devs, 1);
         libusb_exit(NULL);
         cout << "UsbManager shutdown." << endl;
      }
   }

   UsbManager& UsbManager::getInstance()
   {
      static UsbManager instance;
      return instance;
   }

   void UsbManager::initHotplug()
   {
      if (!mbInit) return;

      if (libusb_has_capability (LIBUSB_CAP_HAS_HOTPLUG))
      {
         libusb_hotplug_register_callback(NULL,
                 (libusb_hotplug_event) (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT),
                 LIBUSB_HOTPLUG_NO_FLAGS,
                 LIBUSB_HOTPLUG_MATCH_ANY,      // VENDOR
                 LIBUSB_HOTPLUG_MATCH_ANY,      // PRODUCT
                 LIBUSB_HOTPLUG_MATCH_ANY, CallBack, NULL, NULL);

      }
      else
      {
         cerr << "Usb: Hotplug capabilites are not supported on this platform" << endl;
         return;
      }
   }

   bool UsbManager::registerHotplug(UsbHotplugObserver* hp)
   {
      if (!mbInit) return false;

      const auto& vp = hp->getVendorProduct();
      auto it = mapObservers.find(vp);
      if (it != mapObservers.end())
      {
         cerr << "Usb: cannot listen twice to the same vendor/product" << endl;
         return false;
      }
      cout << "registering " << hp->getVendorProduct() << endl;
      mapObservers[vp] = hp;
      return true;
   }

   void UsbManager::unregisterHotplug(UsbHotplugObserver* hp)
   {
      if (!mbInit) return;

      for(Container::iterator it = mapObservers.begin(); it != mapObservers.end(); it++)
      {
         if (it->second == hp)
         {
            cout << "Unregistering hotplug " << hp->getVendorProduct() << endl;
            mapObservers.erase(it);
            return;
         }
      }
   }

   int LIBUSB_CALL UsbManager::CallBack(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)
   {
      struct libusb_device_descriptor desc;
      auto rc = libusb_get_device_descriptor(dev, &desc);
      if (LIBUSB_SUCCESS != rc)
      {
         cerr << "UsbManager: unable to get device description " << rc << endl;
         return 0;
      }
      mLastDeviceEvent = desc;
      const auto& it=mapObservers.find(mLastDeviceEvent);

      cout << "Usb event (" << event << ") Vendor / Product: " << hex << "0x" << desc.idVendor << ' ' << "0x" << desc.idProduct << dec;
      if (it == mapObservers.end())
      {
         cout << " (no listener)" << endl;
         return 0;
      }

      if (event & LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED)
      {
         cout << " plugged" << endl;
         it->second->onPlugged();
      }
      else if (event & LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT)
      {
         cout << "  unplugged" << endl;
         it->second->onUnplugged();
      }
      return 0;
   }
}
