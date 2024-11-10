// **********************************************************
//      2024 : Arthur JEULIN
//      Copyright (c) Coast Autonomous 
//      Created by Arthur JEULIN on 11/11/2024.
// **********************************************************
//
// Module : Learn RTMaps Chapter 3 multiplier
//

#include "chapter_1/maps_multiplier_3.h"

/**
 * Chapter 1 - Numeric data handling
 * It will be able to handle vectors of number of any size.
 * This component demonstrates how to : 
 *  - declare an output able without the automatic output buffer allcation (see the MAPS_OUTPUT macro)
 *  - "manual" allocation of the output buffers when receiving an input for the first time (see MAPSOutput::AllocOutputBuffer)
 *  - use of the VectorSize() method
 *  - use of BufferSize() method
 */

MAPS_BEGIN_INPUTS_DEFINITION(MAPS_MULTIPLIER_3)
  MAPS_INPUT("input",MAPS::FilterInteger32,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

MAPS_BEGIN_OUTPUTS_DEFINITION(MAPS_MULTIPLIER_3)
  MAPS_OUTPUT("output",MAPS::Integer32,nullptr,nullptr,0)
MAPS_END_OUTPUTS_DEFINITION

MAPS_BEGIN_PROPERTIES_DEFINITION(MAPS_MULTIPLIER_3)
  MAPS_PROPERTY("factor",2,false,true)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPS_MULTIPLIER_3)
MAPS_END_ACTIONS_DEFINITION

MAPS_COMPONENT_DEFINITION(MAPS_MULTIPLIER_3,"multiplier_3","2.0.0",128,
  MAPS::Threaded|MAPS::Sequential,MAPS::Threaded,
  1, // Nb of inputs
  1, // Nb of outputs
  1, // Nb of properties
  0) // Nb of actions

void MAPS_MULTIPLIER_3::Birth()
{
  m_input_reader = MAPS::MakeInputReader::Reactive(
    this,
    Input("input"),
    // this callback will be called when data is received for the first time only
    [this](MAPSTimestamp /*ts*/, MAPS::InputElt<int32_t> in_elt)
    {
      // Allocate the output buffers so that they can contain vectors up to the 
      // maximum size handled by the output that we are connectect to. 
      // InputElt::BufferSize() specifies the allocated sie in the InputElt.
      // Therefore, the maxium length of vectors that can be handled.
      // Note that, for a given InputElt object, VectorSize() is always less thatn or equal to BufferSize()
      Output("output").AllocOutputBuffer(static_cast<int>(in_elt.BufferSize()));
    },
    // This callback will be called when data is received for the first time AND the following times when new data is received.
    [this](MAPSTimestamp ts, MAPS::InputElt<int32_t> in_elt)
    {
      // First make an OutputGuard object in which we can write the result to be output
      MAPS::OutputGuard<int32_t> outGuard{this,Output("output")};

      // Get the current value of the "factor" property (whose type is integer)
      const auto factor = static_cast<int32_t>(GetIntegerProperty("factor"));

      // For each element in the input, compute the result and write it to the output
      const auto elementCount = in_elt.VectorSize();
      for(size_t i = 0 ; i < elementCount ; ++i) { outGuard.Data(i) = factor * in_elt.Data(i);}
      // Important: specify the number of valid elements in the output vector
      outGuard.VectorSize() = in_elt.VectorSize();
      // Important Transfer the timestamp
      outGuard.Timestamp() = ts;
    }
  );
}

void MAPS_MULTIPLIER_3::Core() { m_input_reader->Read();}

void MAPS_MULTIPLIER_3::Death(){ m_input_reader.reset();}