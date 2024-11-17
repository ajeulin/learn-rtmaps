// **********************************************************
//      2024 : Arthur JEULIN
//      Copyright (c) Coast Autonomous 
//      Created by Arthur JEULIN on 11/17/2024.
// **********************************************************
//
// Module : Learn RTMaps Chapter 2 resampling
//

/**
 * This component generates a new vector at a constant frequency 
 * independently of the frequency of the data received on the inputs.
 * This is achived with 2 inputs of type SamplingReader
 */

#include "chapter_2/maps_periodic_sampling.h"

MAPS_BEGIN_INPUTS_DEFINITION(MAPS_PERIODIC_SAMPLING)
  MAPS_INPUT("sampling_1",MAPS::FilterInteger32,MAPS::SamplingReader)
  MAPS_INPUT("sampling_2",MAPS::FilterInteger32,MAPS::SamplingReader)
MAPS_END_INPUTS_DEFINITION

MAPS_BEGIN_OUTPUTS_DEFINITION(MAPS_PERIODIC_SAMPLING)
  MAPS_OUTPUT("output",MAPS::Integer32,nullptr,nullptr,2)
MAPS_END_OUTPUTS_DEFINITION

#define IDX_O_OUTPUT 0

MAPS_BEGIN_PROPERTIES_DEFINITION(MAPS_PERIODIC_SAMPLING)
  MAPS_PROPERTY("sampling_period",100'000,false,false)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPS_PERIODIC_SAMPLING)
MAPS_END_ACTIONS_DEFINITION

MAPS_COMPONENT_DEFINITION(MAPS_PERIODIC_SAMPLING,"maps_periodic_sampling","1.0.0",128,
  MAPS::Threaded,MAPS::Threaded,
    2, // Nb of Inputs
    1, // Nb of Ouputs
    1, // Nb of Propeties
    0) // Nb of Actions

void MAPS_PERIODIC_SAMPLING::Birth()
{
  /**
   * Create a new input reader using "PeriodicSampling" policy
   * The "PeriodicSampling" policy calls a user-provided callback at
   * a fixed rate and provides it with the most recent data samples on 
   * all inputs
   */
  _input_reader = MAPS::MakeInputReader::PeriodicSampling(
      this,
      GetIntegerProperty("sampling_period"),
      MAPS::InputReaderOption::PeriodicSampling::SamplingBehavior::WaitForAllInputs,
      MAPS::MakeArray(&Input("sampling_1"),&Input("sampling_2")),
      &MAPS_PERIODIC_SAMPLING::ProcessData
  );
}

void MAPS_PERIODIC_SAMPLING::Core(){_input_reader->Read();}

void MAPS_PERIODIC_SAMPLING::Death(){_input_reader.reset();}

void MAPS_PERIODIC_SAMPLING::ProcessData(MAPSTimestamp ts, MAPS::ArrayView<MAPS::InputElt<int32_t>> in_elts)
{
  // First make an OutputGuard object in which we can write the result to be result to be output
  MAPS::OutputGuard<int32_t> outGuard{this, Output(IDX_O_OUTPUT)};
  const size_t input_count = in_elts.size(); // == 2 in this example
  for(size_t i = 0; i < input_count ; ++i)
  {
    auto& in_elt = in_elts[i]; // get a reference to the i'th InputElt
    outGuard.Data(i) = in_elt.Data(); // Assign the first data element of the i'th input to the i'th element of output
  }

  // Important: Specify the number of valid elements in the output vector
  outGuard.VectorSize() = input_count;

  // Important: Transfert the timestamp
  outGuard.Timestamp() = ts;
}