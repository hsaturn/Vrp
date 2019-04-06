/* 
 * File:   AutoDetect.cpp
 * Author: hsaturn
 * 
 * Created on 4 avril 2019, 20:49
 */
#include "usb/UsbManager.h"
#include "AutoDetect.hpp"
#include <iostream>                       
#include <iomanip>                        
using namespace std; 

namespace slicer
{

   AutoDetect::AutoDetect() : UsbHotplugObserver({0x403, 0x6001})
   {
      
   }
   
   void AutoDetect::onPlugged()
   {
      cout << "Printer plugged" << endl;
      cout << "Last event " << hex << core::UsbManager::getLastDeviceUnOrPlugged() << dec << endl;
   }
   
   void AutoDetect::onUnplugged()
   {
      cout << "Printer unplugged" << endl;
   }
   
   void AutoDetect::onTerminate()
   {
      cout << "Terminate :-(" << endl;
   }

}