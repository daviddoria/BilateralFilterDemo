#ifndef COMPUTATIONTHREAD_H
#define COMPUTATIONTHREAD_H

#include <QThread>


// This class is named 'ComputationThreadClass' instead of just 'ComputationThread'
// because we often want to name a member variable 'ComputationThread'
class ComputationThreadClass : public QThread
{
Q_OBJECT

signals:
  // This signal is emitted to start the progress bar
  void StartProgressBarSignal();

  // This signal is emitted to stop the progress bar
  void StopProgressBarSignal();

  void RefreshSignal();
  
  void IterationCompleteSignal();

public:
  ComputationThreadClass();

  // This function is called when the thread is started.
  void run();

  virtual void Compute() = 0;

  // This function is called when the thread is stopped.
  void exit();

  void StopComputation();

protected:
  
  // This flag can be set from another thread (by calling StopInpainting()) to indicate that we want to stop the computation at the next possible opportunity.
  bool Stop;
};

#endif