// **********************************************************
//      2024 : Arthur JEULIN
//      Copyright (c) Coast Autonomous 
//      Created by Arthur JEULIN on 11/17/2024.
// **********************************************************
//
// Module : Learn RTMaps Chapter 2 maps_periodic_sampling
//

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#ifndef MAPS_PERIODIC_SAMPLING_H
#define MAPS_PERIODIC_SAMPLING_H

// Include maps sdk library header
#include <maps.hpp>
// Includes the MAPS::InputReader class and its dependencies
#include <maps/input_reader/maps_input_reader.hpp>

class MAPS_PERIODIC_SAMPLING : public MAPSComponent
{
  MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPS_PERIODIC_SAMPLING)
  private:
    std::unique_ptr<MAPS::InputReader> _input_reader;
    // Place here your specific methods and attributes

    void ProcessData(MAPSTimestamp ts, MAPS::ArrayView<MAPS::InputElt<int32_t>> in_elts);
};
#endif /* MAPS_PERIODIC_SAMPLING_H */