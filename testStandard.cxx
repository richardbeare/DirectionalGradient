#include "ioutils.h"
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>


int main(int argc, char * argv[])
{
  const unsigned dim = 2;

  typedef itk::Image<unsigned char, dim> ImageType;
  typedef itk::Image<float, dim> FloatImageType;
  typedef itk::Image<unsigned char, dim> MaskType;

  ImageType::Pointer raw = readIm<ImageType>(argv[1]);

  typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<ImageType, FloatImageType> GType;

  GType::Pointer gr = GType::New();

  gr->SetInput(raw);

  writeIm<FloatImageType>(gr->GetOutput(), argv[2]);


  return EXIT_SUCCESS;
}
