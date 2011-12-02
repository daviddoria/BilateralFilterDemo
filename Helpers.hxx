// ITK
#include "itkBilateralImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkVectorIndexSelectionCastImageFilter.h"
#include "itkComposeImageFilter.h"
#include "itkImageToVectorImageFilter.h"
#include "itkImageFileWriter.h"

// Qt
#include <QColor>

namespace Helpers
{

template <typename TImage>
QImage GetQImageRGB(const TImage* image)
{
  itk::ImageRegion<2> region = image->GetLargestPossibleRegion();
  QImage qimage(region.GetSize()[0], region.GetSize()[1], QImage::Format_RGB888);

  itk::ImageRegionConstIterator<TImage> imageIterator(image, region);

  while(!imageIterator.IsAtEnd())
    {
    typename TImage::PixelType pixel = imageIterator.Get();

    itk::Index<2> index = imageIterator.GetIndex();
    int r = static_cast<int>(pixel[0]);
    int g = static_cast<int>(pixel[1]);
    int b = static_cast<int>(pixel[2]);
    QColor pixelColor(r,g,b);
    if(r > 255 || r < 0 || g > 255 || g < 0 || b > 255 || b < 0)
      {
      std::cout << "Can't set r,g,b to " << r << " " << g << " " << b << std::endl;
      }
    qimage.setPixel(index[0], index[1], pixelColor.rgb());

    ++imageIterator;
    }

  return qimage; // The actual image
  //return qimage.mirrored(false, true); // The flipped image
}


template<typename TVectorImage>
void AnisotropicBlurAllChannels(const TVectorImage* image, const float domainSigma, const float rangeSigma, TVectorImage* output)
{
  std::cout << "Blurring with domainSigma " << domainSigma
          << " and rangeSigma " << rangeSigma << std::endl;

  //typedef itk::Image<typename TVectorImage::InternalPixelType, 2> ScalarImageType;
  typedef itk::Image<float, 2> ScalarImageType;

  // Disassembler
  typedef itk::VectorIndexSelectionCastImageFilter<TVectorImage, ScalarImageType> IndexSelectionType;
  typename IndexSelectionType::Pointer indexSelectionFilter = IndexSelectionType::New();
  indexSelectionFilter->SetInput(image);

  // Reassembler
  typedef itk::ComposeImageFilter<ScalarImageType, TVectorImage> ImageToVectorImageFilterType;
  typename ImageToVectorImageFilterType::Pointer imageToVectorImageFilter = ImageToVectorImageFilterType::New();

  std::vector< typename ScalarImageType::Pointer > filteredImages;

  for(unsigned int i = 0; i < image->GetNumberOfComponentsPerPixel(); ++i)
    {
    indexSelectionFilter->SetIndex(i);
    indexSelectionFilter->Update();

    typename ScalarImageType::Pointer imageChannel = ScalarImageType::New();
    DeepCopy<ScalarImageType>(indexSelectionFilter->GetOutput(), imageChannel);

    typedef itk::BilateralImageFilter<ScalarImageType, ScalarImageType>  BilateralFilterType;
    typename BilateralFilterType::Pointer bilateralFilter = BilateralFilterType::New();
    bilateralFilter->SetInput(imageChannel);
    bilateralFilter->SetDomainSigma(domainSigma);
    bilateralFilter->SetRangeSigma(rangeSigma);
    bilateralFilter->Update();

    std::stringstream ss;
    ss << "Filtered_" << domainSigma << "_" << rangeSigma << ".mha";
    WriteImage<ScalarImageType>(bilateralFilter->GetOutput(), ss.str());
    typename ScalarImageType::Pointer blurred = ScalarImageType::New();
    DeepCopy<ScalarImageType>(bilateralFilter->GetOutput(), blurred);

    filteredImages.push_back(blurred);
    imageToVectorImageFilter->SetInput(i, filteredImages[i]);
    }

  imageToVectorImageFilter->Update();

  DeepCopy<TVectorImage>(imageToVectorImageFilter->GetOutput(), output);
}

template<typename TImage>
void DeepCopy(const TImage* input, TImage* output)
{
  //std::cout << "DeepCopy()" << std::endl;
  if(output->GetLargestPossibleRegion() != input->GetLargestPossibleRegion())
    {
    output->SetRegions(input->GetLargestPossibleRegion());
    output->Allocate();
    }
  DeepCopyInRegion<TImage>(input, input->GetLargestPossibleRegion(), output);
}

template<typename TImage>
void DeepCopyInRegion(const TImage* input, const itk::ImageRegion<2>& region, TImage* output)
{
  // This function assumes that the size of input and output are the same.

  itk::ImageRegionConstIterator<TImage> inputIterator(input, region);
  itk::ImageRegionIterator<TImage> outputIterator(output, region);

  while(!inputIterator.IsAtEnd())
    {
    outputIterator.Set(inputIterator.Get());
    ++inputIterator;
    ++outputIterator;
    }
}


template<typename TImage>
void WriteImage(const TImage* image, const std::string& filename)
{
  // This is a convenience function so that images can be written in 1 line instead of 4.
  typename itk::ImageFileWriter<TImage>::Pointer writer = itk::ImageFileWriter<TImage>::New();
  writer->SetFileName(filename);
  writer->SetInput(image);
  writer->Update();
}

} // end namespace
