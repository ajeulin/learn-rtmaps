// **********************************************************
//      2024 : Arthur JEULIN
//      Copyright (c) Coast Autonomous 
//      Created by Arthur JEULIN on 11/16/2024.
// **********************************************************
//
// Module : Learn RTMaps Chapter 2 reative
//

#include "chapter_2/maps_reactive.h"

/**
 * This component generates a new vector, each time a new piece of data
 * arrives on either the first or the second input. ("reactive on 2 inputs")
 * The way to achieve this is to declare 2 inputs a FifoReader, but use a single
 * StartReading function for the 2 inputs.
 */

MAPS_BEGIN_INPUTS_DEFINITION(MAPS_REACTIVE)
  MAPS_INPUT("input_1",MAPS::FilterInteger32,MAPS::FifoReader)
  MAPS_INPUT("input_2",MAPS::FilterInteger32,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

MAPS_BEGIN_OUTPUTS_DEFINITION(MAPS_REACTIVE)
  MAPS_OUTPUT("output",MAPS::Integer32,nullptr,nullptr,2)
MAPS_END_OUTPUTS_DEFINITION

#define IDX_O_OUTPUT 0

MAPS_BEGIN_PROPERTIES_DEFINITION(MAPS_REACTIVE)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPS_REACTIVE)
MAPS_END_ACTIONS_DEFINITION

// Multiple inpus components have to be threaded. Don't allow sequential behavior
MAPS_COMPONENT_DEFINITION(MAPS_REACTIVE,"maps_reactive","1.0.0",128,
  MAPS::Threaded,MAPS::Threaded,
    2, // Nb of Inputs
    1, // Nb of Outputs
    0, // Nb of Properties
    0) // Nb of Actions

void MAPS_REACTIVE::Birth()
{
  /**
   * Create a new input reader using the "Reactive" policy
   * The "Reactive" policy calls a user-provided callback as soon as data samples
   * are available on the inputs that are handled by this policy
   */
  _input_reader = MAPS::MakeInputReader::Reactive(
    this,
    /**
     * When the input reader reads data for the first time, it has a choice:
     *  * "Wait For all Inputs": Reads data from the remaining inputs and then call the callback
     *        This means that all the InputsElt parameters of the callback will be valid
     *    In this case, "inputThatAnswered" would point to the most recently-read input
     *  * "Immediate": Call the callback immediately
     *        This means that the only InputElt that would be valid for use by the callback
     *        is the on which index is "inputThatAnwsered" (see the prototype of the callback)
     *  
     * In this example, we will use the "Immediate" behavior to illustrate how to check for an InputElt's validity
     */
    MAPS::InputReaderOption::Reactive::FirstTimeBehavior::Immediate,
    /**
     * If the number of inputs is > 1 (as is the case here), then internal buffer can be used for keep track
     * of the most recent data sample on each input.
     * In this example, we want to use an internal buffer because we want to output the mot
     * recent data value that is received on each input.
     */
    MAPS::InputReaderOption::Reactive::Buffering::Enabled,
    /**
     * The list of input reader
     * You can pass in any contiguous sequence of "MAPSInputs*" elements
     * That can be converted to a "MAPS::ArrayView<MAPSInputs*>"
     * Examples of such sequences are: std::vector<MAPSInputs*>, std::array<MAPSInputs*,N>, MAPSInputs* someArray[N]
     * Here, we pass in a temporary std::array<MAPSInputs*,2>
     */
    MAPS::MakeArray(&Input("input_1"),&Input("input_2")),
    /**
     * In this sample, since all inputs have the same type which is known at compile time
     * we will use the following callback
     */
    &MAPS_REACTIVE::ProcessData
  );
}

void MAPS_REACTIVE::Core(){ _input_reader->Read();}

void MAPS_REACTIVE::Death(){ _input_reader.reset();}

void MAPS_REACTIVE::ProcessData(MAPSTimestamp ts, size_t /* inputThatAnswered*/,MAPS::ArrayView<MAPS::InputElt<int32_t>> in_elts)
{
  // First make an OutputGuard object in which we can write the result to be output
  MAPS::OutputGuard<int32_t> outGuard{this,Output(IDX_O_OUTPUT)};
  /**
   * in_elts[inputThatAnswered] is valid
   * but for the sake of the example, we demonstrate
   * how to check the validity in a generic way
   */
  const size_t input_count = in_elts.size(); // == 2 in this example
  for(size_t i = 0; i < input_count ; ++i)
  {
    /**
     * Notive that we iterate over InputsElt object
     * (as opposed to iterating over the data elements of a single InputElt's buffer)
     * then we assign their data to elements of the output buffer
     */
    const auto& in_elt = in_elts[i];

    outGuard.Data(i) = (in_elt.IsValid()) ?  in_elt.Data() : 0 ;
  }

  // Important: Specify the number of valid elements in the output vector
  outGuard.VectorSize() = input_count;

  // Important: Transfert the timestam
  outGuard.Timestamp() = ts;
}