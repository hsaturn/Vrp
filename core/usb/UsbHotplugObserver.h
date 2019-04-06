/* 
 * File:   UsbHotplugObserver.h
 * Author: hsaturn
 *
 * Created on 4 avril 2019, 22:31
 */

#pragma once

#include <cstdint>


namespace core
{
   
class UsbHotplugObserver
{
   public:
      UsbHotplugObserver(uint16_t idVendor, uint16_t idProduct);
      UsbHotplugObserver(const UsbHotplugObserver& orig);
      virtual ~UsbHotplugObserver();
      
      virtual void onPlugged()=0;
      virtual void onUnplugged()=0;
      
   private:
};

}