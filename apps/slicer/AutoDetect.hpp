/* 
 * File:   AutoDetect.h
 * Author: hsaturn
 *
 * Created on 4 avril 2019, 20:49
 */

#pragma once
#include "usb/UsbHotplugObserver.h"

namespace slicer
{
   
class AutoDetect : public core::UsbHotplugObserver
{
   public:
      AutoDetect();
      AutoDetect(const AutoDetect& orig) = delete;
      
      ~AutoDetect() override = default;

      void onPlugged() override;
      void onUnplugged() override;
      void onTerminate() override;

      
   private:
      
};

}