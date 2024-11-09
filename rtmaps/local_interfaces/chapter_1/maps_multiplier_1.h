// **********************************************************
//      2024 : Arthur JEULIN
//      Copyright (c) Coast Autonomous 
//      Created by Arthur JEULIN on 11/11/2024.
// **********************************************************
//
// Module : Learn RTMaps Chapter 1 multiplier
//


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#ifndef MAPS_MULTIPLIER_1_H
#define MAPS_MULTIPLIER_1_H

// Includes maps sdk library header
#include <maps.hpp>
// Includes the MAPS::InputReader class and its dependencies
#include <maps/input_reader/maps_input_reader.hpp>

class MAPS_MULTIPLIER_1 : public MAPSComponent
{
  // Use standard header definition macro
  MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPS_MULTIPLIER_1)
private:
  // Declare input reader
  std::unique_ptr<MAPS::InputReader> m_input_reader;
};

#endif /* MAPS_MULTIPLIER_1_H */