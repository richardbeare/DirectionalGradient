
#include "itkCommand.h"
using namespace itk;

#include "ioutils.h"
#include "itkSimpleFilterWatcher.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkLabelOverlayImageFilter.h"


int main(int arglen, char * argv[])
{
  const int dim = 2;
  
  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef float FType;
  typedef itk::Image< FType, dim > FlIType;

  typedef itk::RGBPixel<unsigned char>   RGBPixelType;
  typedef itk::Image<RGBPixelType, dim>    RGBImageType;
  
  IType::Pointer raw = readIm<IType>(argv[1]);
  FlIType::Pointer gradient = readIm<FlIType>(argv[2]);

  typedef itk::BinaryThresholdImageFilter<FlIType, IType> ThreshType;
  ThreshType::Pointer thresh = ThreshType::New();
  thresh->SetInput(gradient);
  thresh->SetUpperThreshold(atof(argv[3]));
  thresh->SetInsideValue(0);
  thresh->SetOutsideValue(atof(argv[4]));
  writeIm<IType>(thresh->GetOutput(), "thresh.nii.gz");

  typedef itk::LabelOverlayImageFilter<IType, IType, RGBImageType> OverlayType;
  OverlayType::Pointer overlay = OverlayType::New();
  overlay->SetInput( raw );
  overlay->SetLabelImage( thresh->GetOutput());

  typedef itk::ImageFileWriter< RGBImageType > RGBWriterType;
  RGBWriterType::Pointer rgbwriter = RGBWriterType::New();
  rgbwriter->SetInput( overlay->GetOutput() );
  rgbwriter->SetFileName( argv[5] );
  rgbwriter->Update();
  overlay->SetBackgroundValue( 0);
//   if( arglen > 3 )
//     {
//     overlay->SetOpacity( atof( argv[3] ) );
//     }

//   if( arglen > 4 )
//     {
//     //overlay->SetUseBackground( true );
//     overlay->SetBackgroundValue( atoi( argv[4] ) );
//     }

    rgbwriter->Update();

  return 0;
}

