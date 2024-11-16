// **********************************************************
//      2024 : Arthur JEULIN
//      Copyright (c) Coast Autonomous 
//      Created by Arthur JEULIN on 11/16/2024.
// **********************************************************
//
// Module : Learn RTMaps Chapter 2 tiggered
//


#include "chapter_2/maps_triggered.h"

/**
 * maps_triggered demonstrated how to write RTMaps components that have multiples inputs
 * These samples show how to get integer data on several inputs, make a vector,
 * and output it
 * 
 * Calling succesive StartReading functions on FifoReader, LastOrNextReader, Wait4NextReader or
 * NeverskippingReader inputs.
 * 
 * StartReading is blocking when called on those type of inputs,so successive StartReading function
 * synchronize the compnonent on the input that receives data at the lowest frequency thus generating FIFO
 * overflow and latency on the other inputs.
 * 
 * This component generates a new vector, each time a new piece of data arrives on the
 * first input. ("triggered by first input")
 * The way to achieve this is to declared the first input as FifoReader,
 * the second input as SamplingReader and call 2 sucessive StartReading functions.
 * 
 */

MAPS_BEGIN_INPUTS_DEFINITION(MAPS_TRIGGERED)
  MAPS_INPUT("tiggered",MAPS::FilterInteger32,MAPS::FifoReader)
  MAPS_INPUT("sampling",MAPS::FilterInteger32,MAPS::SamplingReader)
MAPS_END_INPUTS_DEFINITION

MAPS_BEGIN_OUTPUTS_DEFINITION(MAPS_TRIGGERED)
  MAPS_OUTPUT("output",MAPS::Integer32,nullptr,nullptr,2)
MAPS_END_OUTPUTS_DEFINITION

#define IDX_O_OUTPUT 0

MAPS_BEGIN_PROPERTIES_DEFINITION(MAPS_TRIGGERED)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPS_TRIGGERED)
MAPS_END_ACTIONS_DEFINITION

MAPS_COMPONENT_DEFINITION(MAPS_TRIGGERED,"maps_trigger","1.0.0",128,
  MAPS::Threaded,MAPS::Threaded,
    2, // Nb Inputs
    1, // Nb Outputs
    0, // Nb properties
    0) // Nb actions

void MAPS_TRIGGERED::Birth()
{
  /**
   * Create a enx input reader using the "Triggered" policy.
   * The "Triggered" policy waits for new data samples tobe available
   * on its "trigger" input (which is the "triggered" input in this sample)
   * then, it reads the data samples of the other inputs ("sampling" in this sample)
   * and calls the user-provided callback
   */
  _input_reader = MAPS::MakeInputReader::Triggered(
    this,
    // The input reader will first wait for data on this input
    Input("triggered"),
    // Triggered::DataInput means that we want to acces the "value" of the data of the trigger
    // input in the callback. In this case, the trigger input MUST be added to the list of data inputs
    MAPS::InputReaderOption::Triggered::TriggerKind::DataInput,
    // SamplingBehavior::WaitForAllInputs means that the data callback will be called
    // only if there are data samples on all the inputs
    MAPS::InputReaderOption::Triggered::SamplingBehavior::WaitForAllInputs,
    /**
     * The list of inputs to read.
     * You can pass in any contiguous sequence of "MAPSInput*" elements
     * that can be converted to a "MAPS::ArrayView<MAPSInput*>"
     * Examples of such sequences are: "std::vector<MAPSInput*>, std::array<MAPSIput*,N>, MAPSInput* someArray[N]"
     * 
     * Here, we pass a temporary std::array<MAPSInput*,2>
     */
    MAPS::MakeArray(&Input("triggered"),&Input("sampling")), // The data samples received on these inputs will be passed to the callback
    // This callback will be called when data was read fro the trigger and the data inputs.
    // Here, we demonstrated, the use of a member function pointer as data callback
    &MAPS_TRIGGERED::ProcessData
  );
}

void MAPS_TRIGGERED::Core(){ _input_reader->Read();}

void MAPS_TRIGGERED::Death() { _input_reader.reset();}

void MAPS_TRIGGERED::ProcessData(MAPSTimestamp ts, MAPS::InputElt<int32_t> in_elt_1, MAPS::InputElt<int32_t> in_elt_2)
{
  // First make an OutputGuard object in which we can write the result to be output
  MAPS::OutputGuard<int32_t> outGuard{this,Output(IDX_O_OUTPUT)};

  // Generate the output vector
  outGuard.Data(0) = in_elt_1.Data();
  outGuard.Data(1) = in_elt_2.Data();

  // Important: Specify the number of valid elements in the output vector
  outGuard.VectorSize() = 2;

  // Important: Transfer the timestamp
  outGuard.Timestamp() = ts;

}