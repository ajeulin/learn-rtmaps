// **********************************************************
//      2024 : Arthur JEULIN
//      Copyright (c) Coast Autonomous 
//      Created by Arthur JEULIN on 11/11/2024.
// **********************************************************
//
// Module : Learn RTMaps Chapter 2 multiplier
//

#include "chapter_1/maps_multiplier_2.h"
#include <algorithm>

/**
 * Chapter 1 - Numeric data handling
 * It read integer vectors data on its input, multiplies the incoming integers
 * by a factor specified in a property, and outputs the results as a vector of integers.
 * 
 * This component demonstrates how to : 
 *  - declare an output able to handle vectors of limited size (see the MAPS_OUTPUT macro)
 *  - use of the VectorSize() method
 * This component will be set to handle vectors of integers with a limited maximum size (set to 32 
 *  in this example).
 */

const int MAX_ELEMENTS_OUT = 32;

// Declaration of an input called "input", which accepts connections with
// outputs of type Integer, and which has a FifoReader behavior by default.
MAPS_BEGIN_INPUTS_DEFINITION(MAPS_MULTIPLIER_2)
  MAPS_INPUT("input",MAPS::FilterInteger32,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Declaration of an output called "outpout" of type Integer, which will 
// handle vectors of size 32 max
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPS_MULTIPLIER_2)
  MAPS_OUTPUT("output",MAPS::Integer32,nullptr,nullptr,MAX_ELEMENTS_OUT)
MAPS_END_OUTPUTS_DEFINITION

// Declaration of property called "factor" of type integer (tue to the type
// of the default value), with default value = 2, and which is allowed to be
// modified by the user during the diagram execution
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPS_MULTIPLIER_2)
  MAPS_PROPERTY("factor",2,false,true)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPS_MULTIPLIER_2)
MAPS_END_ACTIONS_DEFINITION

// Declaration of the component attributes: named "multiplier_2", with
// version 1.0, default priority of 128 (meduim) if the component is threaded,
// the components accepts threaded or sequential behavior, is threaded by defaults
// and has 1 input, 1 output and 1 property (taken from the above declarations)
MAPS_COMPONENT_DEFINITION(MAPS_MULTIPLIER_2,"multiplier_2","2.0.0",128,
    MAPS::Threaded|MAPS::Sequential,MAPS::Threaded,
    1, // Nb of inputs
    1, // Nb of outputs
    1, // Nb of properties
    0) // Nb of actions

void MAPS_MULTIPLIER_2::Birth()
{
  m_input_reader = MAPS::MakeInputReader::Reactive(
    this, // Pointer of the component that owns the inputs
    Input("input"), 
    [this](MAPSTimestamp ts, MAPS::InputElt<int32_t> in_elt)
    {
      // First make an OutputGuard object in which we can write the result to be output
      MAPS::OutputGuard<int32_t> outGuard{this,Output("output")};
      // Bound the vector size to MAX_ELEMENT_OUT if the input vector is larger thatn MAX_ELEMENT_OUT elements
      // we write "(std::min)" to prevent the "min"
      const auto elementCount = (std::min)(in_elt.VectorSize(), static_cast<size_t>(MAX_ELEMENTS_OUT));
      // Get the current value of the "factor" property (chose type is integer)
      const auto factor = static_cast<int32_t>(GetIntegerProperty("factor"));

      // For each element in the input up to MAX_ELEMENTS_OUT if the input vector is larger),
      // compute the result and write it to the output
      for(size_t i = 0; i < elementCount; ++i) {outGuard.Data(i) = factor * in_elt.Data(i);}

      // Important : Specify the number of valid element in the output vector (less or equal to MAX_ELEMNTS_OUT)
      outGuard.VectorSize() = elementCount;
      // Important : Transfer the timestamp
      outGuard.Timestamp() = ts;  
    }
  );
}

void MAPS_MULTIPLIER_2::Core(){ m_input_reader->Read();}

void MAPS_MULTIPLIER_2::Death(){ m_input_reader.reset();}