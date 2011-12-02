#ifndef HELPERS_H
#define HELPERS_H

// ITK
#include "itkImageRegion.h"

// QT
#include <QImage>

namespace Helpers
{

template <typename TImage>
QImage GetQImageRGB(const TImage* image);

template<typename TVectorImage>
void AnisotropicBlurAllChannels(const TVectorImage* image, const float domainSigma, const float rangeSigma, TVectorImage* output);

template<typename TImage>
void DeepCopy(const TImage* input, TImage* output);

template<typename TImage>
void DeepCopyInRegion(const TImage* input, const itk::ImageRegion<2>& region, TImage* output);

template<typename TImage>
void WriteImage(const TImage* image, const std::string& filename);

}

#include "Helpers.hxx"

#endif
