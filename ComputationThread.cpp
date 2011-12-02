#include "ComputationThread.h"

ComputationThreadClass::ComputationThreadClass() : Stop(false)
{

}

void ComputationThreadClass::StopComputation()
{
  this->Stop = true;
}

void ComputationThreadClass::run()
{
  Compute();
}

void ComputationThreadClass::exit()
{
  // When the thread is stopped, emit the signal to stop the marquee progress bar
  emit StopProgressBarSignal();
}
