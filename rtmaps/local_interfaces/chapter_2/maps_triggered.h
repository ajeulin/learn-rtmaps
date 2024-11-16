// **********************************************************
//      2024 : Arthur JEULIN
//      Copyright (c) Coast Autonomous 
//      Created by Arthur JEULIN on 11/16/2024.
// **********************************************************
//
// Module : Learn RTMaps Chapter 2 triggered
//

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#ifndef MAPS_TRIGGERED_H
#define MAPS_TRIGGERED_H

// Includes maps  sdk library header
#include <maps.hpp>
// Includes the MAPS::InputReader class and its dependencies
#include <maps/input_reader/maps_input_reader.hpp>

class MAPS_TRIGGERED : public MAPSComponent
{
  // Use standard header definition macro
  MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPS_TRIGGERED)
  private:
    // Declare an input reader 
    std::unique_ptr<MAPS::InputReader> _input_reader;
    // Place here your specific methods and attributes

    void ProcessData(MAPSTimestamp ts, MAPS::InputElt<int32_t> in_elt_1, MAPS::InputElt<int32_t> in_elt_2);
};

#endif /* MAPS_TRIGGERED */