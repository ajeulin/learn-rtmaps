// **********************************************************
//      2024 : Arthur JEULIN
//      Copyright (c) Coast Autonomous 
//      Created by Arthur JEULIN on 11/11/2024.
// **********************************************************
//
// Module : Learn RTMaps Chapter 1 multiplier
//

#include "chapter_1/maps_multiplier_1.h"
/**
 * Chapter 1 - Numeric data handling
 * This chapter ocntains some simple samples that show how to program
 * components that deal with numeric data (how to get data an an input,
 * how to access properties, how to provide data on an output).
 * 
 * maps_multiplier_1 :
 *  It reads integer scalar data on its input, mutiplies 
 *  the incoming integer by a factor specified in a property, and output the result as scalar integer.
 *  This component demonstrates how to : 
 *    - define an input
 *    - define an output
 *    - define a property
 *    - get scalar data from its input
 *    - output scalar data
 */

// Declaration of a input called "input", which accepts connections with
// outputs of type Integer, which has a Fifo Reader behavior by default

MAPS_BEGIN_INPUTS_DEFINITION(MAPS_MULTIPLIER_1)
  MAPS_INPUT("input",MAPS::FilterInteger32,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Declaration of an output called "output" of type Integer, which will
// handle vector of size 1 max (which means ... scalars)
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPS_MULTIPLIER_1)
  MAPS_OUTPUT("output",MAPS::Integer32,nullptr,nullptr,1)
MAPS_END_OUTPUTS_DEFINITION

// Declaration of property called "factor" of type integer (due to tye type
// of the default value), with default value = 2, and which is allowed to be
// modified by the user during the diagram execution
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPS_MULTIPLIER_1)
  MAPS_PROPERTY("factor",2,false, true)
MAPS_END_PROPERTIES_DEFINITION

// no actio here yet
MAPS_BEGIN_ACTIONS_DEFINITION(MAPS_MULTIPLIER_1)
MAPS_END_ACTIONS_DEFINITION

// Declaration of the component attributes: names "multiplier_1" with
// version 1.0 default priority of 128 (meduim) if the component is threaded
// the component accepts threaded or sequential behavior, it threaded by default
// and has 1 input, 1 output, and 1 property (taken from the above declarations)
MAPS_COMPONENT_DEFINITION(MAPS_MULTIPLIER_1,"multiplier_1","1.0.0",128,
    MAPS::Threaded|MAPS::Sequential, MAPS::Threaded,
    1, // Nb of inputs
    1, // Nb of outputs
    1, // Nv of properties
    0)
// This fuction is called once by the RTMaps engine when the diagram executionstarts
// Initialization code shoud be put here
void MAPS_MULTIPLIER_1::Birth()
{
  // Create a new input reader using the "Reactive" policy.
  // The "Reactive" policy calls a user-provided callback as soon as data samples
  // are available on the input that are handles by this policy

  m_input_reader = MAPS::MakeInputReader::Reactive(
    this, // Pointer ot the component that owns the inputs
    Input("input"), // The list of inputs to reader.
    // This callback will be called by the input reader when new data is available on the input
    // In this, example, we use a C++11 lambda expression, but you could use any callable "object" such as
    // * free function
    // * Member functions of the component class 
    // * Lambda expression
    // * Functor 
    [this] (MAPSTimestamp ts, MAPS::InputElt<int32_t> in_elt)
    {
      // Reader and process the input
      // Get the integer contained in the MAPSIOElt object that has just received
      const auto value_int = in_elt.Data(); // int32_t value
      // Get the current value of "factor" property (chose type is integer)
      const auto factor = static_cast<int32_t>(GetIntegerProperty("factor"));
      // compute the result
      const auto result = value_int * factor;

      // output the result
      // First make an OutputGuard object in which we can write the resulta to be output
      MAPS::OutputGuard<int32_t> outGuard{this,Output("output")};
      // Write the resulta in the Output Guard object ("OutputGuard::Data() returns a C++ reference to the contained integer")
      outGuard.Data() = result;

      // Important : Transfert the timestamp
      outGuard.Timestamp() = ts;
    }
  );
}

// This function is called in a loop durinng diagram execution
// Alwaus check you use a blocing function somewhere( here InputReader::Read() using a Reactive policy)
// otherwise this component would run into an inifiite loop, hence consuming 100 % CPU.
void MAPS_MULTIPLIER_1::Core() { m_input_reader->Read();}

// This function is called once by the RTMaps engine when the diagram execution stops.
// Cleanup code should go in there
void MAPS_MULTIPLIER_1::Death()
{
  // Important: Destroy the input reader
  // m_input_reader is an std::unique_tr, so we call its reset() method to destroy it
  // and free the resource that have been allocated for it
  m_input_reader.reset();
}