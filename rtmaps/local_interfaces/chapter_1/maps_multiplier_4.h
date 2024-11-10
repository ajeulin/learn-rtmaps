// **********************************************************
//      2024 : Arthur JEULIN
//      Copyright (c) Coast Autonomous 
//      Created by Arthur JEULIN on 11/11/2024.
// **********************************************************
//
// Module : Learn RTMaps Chapter 4 multiplier
//

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#ifndef MAPS_MULTIPLIER_4_H
#define MAPS_MULTIPLIER_4_H

// Includes maps sdk library header
#include <maps.hpp>
// Includes the MAPS::InputReader class and its dependencies
#include <maps/input_reader/maps_input_reader.hpp>

class MAPS_MULTIPLIER_4 : public MAPSComponent
{
  // Use the standard header definition macro
  MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPS_MULTIPLIER_4)
  // Overload the Set functions (virtual) enable to hook the change of property values
  // Here we hook the changes in properties of integer type
  void Set(MAPSProperty& p, MAPSInt64 value);
  private:
    std::unique_ptr<MAPS::InputReader> m_input_reader;
    // This member variable will remain synchronized with the "factor" property value
    int m_factor;
};



#endif /* MAPS_MULTIPLIER_4_H */