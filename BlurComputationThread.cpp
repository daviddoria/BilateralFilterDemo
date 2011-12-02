#include "BlurComputationThread.h"

#include <iostream>

#include "Helpers.h"

BlurComputationThreadClass::BlurComputationThreadClass() : ComputationThreadClass()
{

}

void BlurComputationThreadClass::Compute()
{
  emit StartProgressBarSignal();

  // Start the procedure
  this->Stop = false;


  std::cout << "Generating..." << std::endl;

  for(unsigned int domainSigmaId = 0; domainSigmaId < DomainSigmas.size(); ++domainSigmaId)
    {
    for(unsigned int rangeSigmaId = 0; rangeSigmaId < RangeSigmas.size(); ++rangeSigmaId)
      {
      if(this->Stop)
        {
        exit();
        }
      std::cout << "Generating (" << domainSigmaId << ", " << rangeSigmaId
                << ") with domainSigma " << DomainSigmas[domainSigmaId]
                << " and rangeSigma " << RangeSigmas[rangeSigmaId] << std::endl;
      ImageType::Pointer output = ImageType::New();

      Helpers::AnisotropicBlurAllChannels<ImageType>(this->Image, DomainSigmas[domainSigmaId], RangeSigmas[rangeSigmaId], output);

      QImage qimage = Helpers::GetQImageRGB<ImageType>(output);

      emit IterationCompleteSignal(domainSigmaId, rangeSigmaId, qimage);
      }
    }


  // When the function is finished, end the thread
  exit();
}
