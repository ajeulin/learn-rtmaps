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

#ifndef MAPS_REACTIVE_H
#define MAPS_REACTIVE_H

// Include maps sdk library header
#include <maps.hpp>
// Includes the MAPS::InputReader class and its dependencies
#include <maps/input_reader/maps_input_reader.hpp>

class MAPS_REACTIVE : public MAPSComponent
{
  // Use standard header define macro
  MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPS_REACTIVE)
  private:
    std::unique_ptr<MAPS::InputReader> _input_reader;
    // Place here your specific methods and attributes

    void ProcessData(MAPSTimestamp ts, size_t /* inputThatAnswered*/,MAPS::ArrayView<MAPS::InputElt<int32_t>> in_elts);
};

#endif /* MAPS_REACTIVE */