/* 
 * File:   VendorProduct.h
 * Author: hsaturn
 *
 * Created on 6 avril 2019, 11:48
 */

#pragma once
#include <libusb-1.0/libusb.h>

namespace core
{
   class VendorProduct
   {
      public:
         VendorProduct(int iVendor, int iProduct) : vendor(iVendor), product(iProduct) {}
         VendorProduct(libusb_device_descriptor desc) : VendorProduct(desc.idVendor, desc.idProduct){}

         operator int() const { return (vendor << 16) | product; }
         
         bool operator <(const VendorProduct& vp) const { return operator int() < vp.operator int(); }
      private:
         int vendor;
         int product;
   };
}

