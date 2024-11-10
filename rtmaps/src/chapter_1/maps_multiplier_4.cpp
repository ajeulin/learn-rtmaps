// **********************************************************
//      2024 : Arthur JEULIN
//      Copyright (c) Coast Autonomous 
//      Created by Arthur JEULIN on 11/11/2024.
// **********************************************************
//
// Module : Learn RTMaps Chapter 3 multiplier
//

#include "chapter_1/maps_multiplier_4.h"

/**
 * This module has the same behavior has multiplier4, but introduces some slight
 * optimization taht can be useful sometime for high frequency components.
 * This component demonstrates how to :
 * - access inputs and outpus by their index and not their names
 * - maintain a member variable which remains synchronized with the value of a given property.
 */

// define index of "input"
#define IDX_I_INPUT 0
#define IDX_O_INPUT 0
MAPS_BEGIN_INPUTS_DEFINITION(MAPS_MULTIPLIER_4)
  MAPS_INPUT("input",MAPS::FilterInteger32,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

MAPS_BEGIN_OUTPUTS_DEFINITION(MAPS_MULTIPLIER_4)
  MAPS_OUTPUT("output",MAPS::Integer32,nullptr,nullptr,0)
MAPS_END_OUTPUTS_DEFINITION

MAPS_BEGIN_PROPERTIES_DEFINITION(MAPS_MULTIPLIER_4)
  MAPS_PROPERTY("factor",2,false,true)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPS_MULTIPLIER_4)
MAPS_END_ACTIONS_DEFINITION

MAPS_COMPONENT_DEFINITION(MAPS_MULTIPLIER_4,"multiplier_4","2.0.0",128,
  MAPS::Threaded|MAPS::Sequential,MAPS::Threaded,
    1, // Nb of inputs
    1, // Nb of outputs
    1, // Nb of properties
    0) // Nb of actions

void MAPS_MULTIPLIER_4::Set(MAPSProperty& p, MAPSInt64 value)
{
  if(&p == &Property("factor"))
  {
    m_factor = static_cast<int32_t>(value);
  }
  MAPSComponent::Set(p,value);
}

void MAPS_MULTIPLIER_4::Birth()
{
  // Initialize the m_factor member variable when starting the  diagram execution
  m_factor = static_cast<int32_t>(GetIntegerProperty("factor"));
  m_input_reader = MAPS::MakeInputReader::Reactive(
    this,
    Input(IDX_I_INPUT), // Access to the input via index (this is more efficient than using its name)
    // This callback will be called when data is received for the first time only
    [this](MAPSTimestamp /*ts*/, MAPS::InputElt<int32_t> in_elt)
    {
      // Access to the output via its index (this is more efficient thatn using its name)
      Output(IDX_O_INPUT).AllocOutputBuffer(static_cast<int>(in_elt.BufferSize()));
    },
    // This callback will be called when data is received for the first time and the followng times when
    // new data is received
    [this](MAPSTimestamp ts, MAPS::InputElt<int32_t> in_elt)
    {
      // First make an OutputGuard object in which we can write the result to be output
      // Access to the output via its index (this is more efficient thatn using its name)
      MAPS::OutputGuard<int32_t> outGuard{this,Output(IDX_O_INPUT)};

      // For each element in the input, compute the result and write it to the output
      const auto elementCount = in_elt.VectorSize();
      // Paranoid (and somewhat dangerous even when know what you are doing) optimization here
      const int32_t* src_ptr = in_elt.DataPointer();
      int32_t* dst_ptr = outGuard.DataPointer();
      for(size_t i = 0; i < elementCount; ++i)
      {
        *(dst_ptr++) = *(src_ptr++) * m_factor;
      }
      // Important: Specify the number of valid element in the output vector
      outGuard.VectorSize() = in_elt.VectorSize();

      // Important : Transfert the timestam
      outGuard.Timestamp() = ts;
    }
  );
}

void MAPS_MULTIPLIER_4::Core() { m_input_reader->Read();}

void MAPS_MULTIPLIER_4::Death() { m_input_reader.reset();}