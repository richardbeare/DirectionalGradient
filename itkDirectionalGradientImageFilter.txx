#ifndef __itkDirectionalGradientImageFilter_txx
#define __itkDirectionalGradientImageFilter_txx

#include "itkDirectionalGradientImageFilter.h"
#include "itkProgressAccumulator.h"
#include "itkCropImageFilter.h"
#include "itkConstantPadImageFilter.h"

namespace itk
{
template<class TInputImage, class TMaskImage, class TOutputImage, class TDTImage>
DirectionalGradientImageFilter<TInputImage, TMaskImage, TOutputImage, TDTImage>
::DirectionalGradientImageFilter()
{
  m_DT = DistTransType::New();
  m_GradDT = GradFiltType::New();
  m_Innerprod = InnerProductType::New();
  m_ThreshInnerprod = ThresholdInnerProductType::New();
  m_RawGrad = GaussGradFiltType::New();
  m_Padder = PadType::New();
  m_Cropper = CropType::New();

  m_Sigma=1.0;
  m_Scale=1.0;
  m_OutsideValue = 0;
  m_Pad = false;

  m_ApplyAngleThreshold = false;
  m_AngleThreshold = M_PI;

}


template<class TInputImage, class TMaskImage, class TOutputImage, class TDTImage>
void
DirectionalGradientImageFilter<TInputImage, TMaskImage, TOutputImage, TDTImage>
::GenerateData()
{

  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  typename InputImageType::ConstPointer input  = this->GetInput();
  typename MaskImageType::ConstPointer mask = this->GetMaskImage();
  typename DTImageType::ConstPointer dt = this->GetDTImage();
  // construct mini pipeline
  if (m_Pad)
    {
      typename InputImageType::SizeType Pad;
      Pad.Fill(1);
      m_Padder->SetPadLowerBound( Pad);
      m_Padder->SetPadUpperBound( Pad );
      m_Padder->SetConstant( 0 );
      m_Padder->SetInput( mask );

      m_DT->SetInput(m_Padder->GetOutput());
      m_DT->SetOutsideValue(m_OutsideValue);

      m_Cropper->SetInput( m_DT->GetOutput() );
      m_Cropper->SetUpperBoundaryCropSize( Pad );
      m_Cropper->SetLowerBoundaryCropSize( Pad );

      m_GradDT->SetInput(m_Cropper->GetOutput());

    }
  else
    {
    if (dt)
      {
      m_GradDT->SetInput(dt);
      }
    else
      {
      m_DT->SetInput(mask);
      m_DT->SetOutsideValue(m_OutsideValue);
      m_GradDT->SetInput(m_DT->GetOutput());
      }
    }

  m_RawGrad->SetInput(input);
  m_RawGrad->SetSigma(m_Sigma);

  if (m_ApplyAngleThreshold)
    {
    m_ThreshInnerprod->SetInput(m_GradDT->GetOutput());
    m_ThreshInnerprod->SetInput2(m_RawGrad->GetOutput());
    m_ThreshInnerprod->SetScale(m_Scale);
    m_ThreshInnerprod->SetAngleThresh(m_AngleThreshold);
    
    progress->RegisterInternalFilter(m_DT, 0.2f);
    progress->RegisterInternalFilter(m_GradDT, 0.2f);
    progress->RegisterInternalFilter(m_RawGrad, 0.4f);
    progress->RegisterInternalFilter(m_Innerprod, 0.2f);
    
    m_ThreshInnerprod->GraftOutput(this->GetOutput());
    m_ThreshInnerprod->Update();
    this->GraftOutput(m_ThreshInnerprod->GetOutput());

    }
  else
    {
    m_Innerprod->SetInput(m_GradDT->GetOutput());
    m_Innerprod->SetInput2(m_RawGrad->GetOutput());
    m_Innerprod->SetScale(m_Scale);
    
    progress->RegisterInternalFilter(m_DT, 0.2f);
    progress->RegisterInternalFilter(m_GradDT, 0.2f);
    progress->RegisterInternalFilter(m_RawGrad, 0.4f);
    progress->RegisterInternalFilter(m_Innerprod, 0.2f);
    
    m_Innerprod->GraftOutput(this->GetOutput());
    m_Innerprod->Update();
    this->GraftOutput(m_Innerprod->GetOutput());
    }
}


}

#endif
