/* 
 * File:   UsbManager.h
 * Author: hsaturn
 *
 * Created on 4 avril 2019, 21:43
 */

#pragma once
#include <libusb-1.0/libusb.h>
#include <map>
#include "VendorProduct.h"
#include "UsbHotplugObserver.h"

using namespace std;
        
namespace core
{

   class UsbManager
   {
      public:
         UsbManager(const UsbManager& orig) = delete;
         virtual ~UsbManager();
         
         libusb_device ** getDevices();
         bool isOk() const { return mbInit; }
         
         static UsbManager& getInstance();
         static void update();
         static const VendorProduct& getLastDeviceUnOrPlugged() { return mLastDeviceEvent; }
         
      private:
         static bool registerHotplug(UsbHotplugObserver*);
         static void unregisterHotplug(UsbHotplugObserver*);
         
         friend class UsbHotplugObserver;
         static int LIBUSB_CALL CallBack(
               libusb_context *ctx,
               libusb_device *dev,
               libusb_hotplug_event event,
               void *user_data);

         static void initHotplug();
         
         UsbManager();
         
      private:
         using Container = map<VendorProduct, UsbHotplugObserver*>;
         
         static libusb_device **devs;
         static bool mbInit;
         libusb_hotplug_callback_handle handles[2];
         static Container mapObservers;
         static VendorProduct mLastDeviceEvent;
   };

}