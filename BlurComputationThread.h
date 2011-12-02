#ifndef BLURCOMPUTATIONTHREAD_H
#define BLURCOMPUTATIONTHREAD_H

// Qt
#include <QThread>
#include <QImage>

// Custom
#include "ComputationThread.h"
#include "Types.h"

// ITK
#include "itkImage.h"

// This class is named 'TestComputationThreadClass' instead of just 'TestComputationThread'
// because we often want to name a member variable 'ComputationThread'
class BlurComputationThreadClass : public ComputationThreadClass
{
Q_OBJECT

signals:
  void IterationCompleteSignal(const unsigned int domainSigmaId, const unsigned int rangeSigmaId, const QImage&);
  
public:
  BlurComputationThreadClass();

  void Compute();
  
  ImageType::Pointer Image;

  std::vector<float> DomainSigmas;
  std::vector<float> RangeSigmas;
};

#endif
