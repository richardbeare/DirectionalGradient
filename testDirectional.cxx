#include "ioutils.h"
#include "itkDirectionalGradientImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkSimpleFilterWatcher.h"

int main(int argc, char * argv[])
{
  const unsigned dim = 2;

  typedef itk::Image<unsigned char, dim> ImageType;
  typedef itk::Image<float, dim> FloatImageType;
  typedef itk::Image<unsigned char, dim> MaskType;

  ImageType::Pointer raw = readIm<ImageType>(argv[1]);
  MaskType::Pointer mask = readIm<MaskType>(argv[2]);

  typedef itk::DirectionalGradientImageFilter<ImageType, MaskType, FloatImageType> DGType;

  DGType::Pointer dg = DGType::New();

  dg->SetInput(raw);
  dg->SetMaskImage(mask);
  dg->SetOutsideValue(1);

  itk::SimpleFilterWatcher watcher(dg, "dirgrad");


  typedef itk::ThresholdImageFilter <FloatImageType>  ThreshType;

  ThreshType::Pointer thresh = ThreshType::New();

  // save the dark to light transition
  thresh->SetInput(dg->GetOutput());
  thresh->ThresholdBelow(0);
  thresh->SetLower(0);
  writeIm<FloatImageType>(thresh->GetOutput(), argv[3]);

  // save light to dark transition
  dg->SetScale(-1);
  thresh->SetInput(dg->GetOutput());
  thresh->ThresholdBelow(0);
  thresh->SetLower(0);
  writeIm<FloatImageType>(thresh->GetOutput(), argv[4]);


  return EXIT_SUCCESS;
}
