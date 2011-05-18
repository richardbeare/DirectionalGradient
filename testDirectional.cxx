#include "ioutils.h"
#include "itkDirectionalGradientImageFilter.h"

int main(int argc, char * argv[])
{
  const unsigned dim = 2;

  typedef itk::Image<unsigned char, dim> ImageType;
  typedef itk::Image<unsigned char, dim> MaskType;

  ImageType::Pointer raw = readIm<ImageType>(argv[1]);
  MaskType::Pointer mask = readIm<MaskType>(argv[2]);

  typedef itk::DirectionalGradientImageFilter<ImageType, MaskType, ImageType> DGType;

  DGType::Pointer dg = DGType::New();

  dg->SetInput(raw);
  dg->SetMaskImage(mask);


  return EXIT_SUCCESS
}
