/* 
 * File:   UsbHotplugObserver.h
 * Author: hsaturn
 *
 * Created on 4 avril 2019, 22:31
 */

#pragma once

#include "VendorProduct.h"

namespace core
{

   class UsbHotplugObserver
   {
      public:
         UsbHotplugObserver(const VendorProduct& vp);
         UsbHotplugObserver(const UsbHotplugObserver& orig) = delete;
         
         virtual ~UsbHotplugObserver();

         // Overidable callbacks
         virtual void onPlugged()   {};   // Called when an usb devices is attached
         virtual void onUnplugged() {};   // Called whenever an usb device 
         virtual void onTerminate() {};
         
         const VendorProduct& getVendorProduct() const { return mVendorProduct; }

      private:
         VendorProduct mVendorProduct;
   };

}