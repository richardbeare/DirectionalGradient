/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCovariantThresholdInnerProductImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-04-01 14:36:19 $
  Version:   $Revision: 1.23 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCovariantThresholdInnerProductImageFilter_h
#define __itkCovariantThresholdInnerProductImageFilter_h

#include "itkBinaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace itk
{
  
/** \class CovariantThresholdInnerProductImageFilter
 *
 * \brief Take a pair of images of vectors as input and produce an image with the
 *  dot product of those vectors. Output is zero if angle between
 *  vectors is above a threshold.
 *
 * The filter expects the input image pixel type to be a vector and 
 * the output image pixel type to be a scalar.
 *
 * This filter assumes that the PixelType of the input image is a VectorType
 * that provides a * method.
 *
 * \ingroup IntensityImageFilters  Multithreaded
 */

namespace Functor {  
  
template< class TInput1, class TInput2, class TOutput>
class ThresholdInnerProduct
{
public:
  ThresholdInnerProduct() {}
  ~ThresholdInnerProduct() {}

  bool operator!=( const ThresholdInnerProduct & ) const
    {
    return false;
    }
  bool operator==( const ThresholdInnerProduct & other ) const
    {
    return !(*this != other);
    }

  void SetScale(double i){m_Scale = i;}
  void SetAngleThresh(double i)
  {
    m_AngleThresh = i;
    m_BigAngleThresh = M_PI - i;
    
  }

  inline TOutput operator()( const TInput1 & A, const TInput2 &B ) const
    {
      double AdotB = A * B;
      double normA = A.GetNorm();
      double normB = B.GetNorm();
      double Angle = vcl_acos(AdotB/(normA*normB));

      if ((Angle < m_AngleThresh) || (Angle > m_BigAngleThresh) )
	{
	return static_cast<TOutput>(m_Scale * AdotB);
	}
      else
	{
	return(0);
	}
    }
private:
  double m_Scale;
  double m_AngleThresh;
  double m_BigAngleThresh;
}; 
}
 
template <class TInputImage1, class TInputImage2, class TOutputImage>
class ITK_EXPORT CovariantThresholdInnerProductImageFilter :
    public
BinaryFunctorImageFilter<TInputImage1, TInputImage2, TOutputImage, 
                        Functor::ThresholdInnerProduct< typename TInputImage1::PixelType, 
					       typename TInputImage2::PixelType,
					       typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef CovariantThresholdInnerProductImageFilter Self;
  typedef BinaryFunctorImageFilter<
  TInputImage1, TInputImage2, TOutputImage, 
      Functor::ThresholdInnerProduct< typename TInputImage1::PixelType, typename TInputImage2::PixelType,
    typename TOutputImage::PixelType> >
                                         Superclass;
  typedef SmartPointer<Self>             Pointer;
  typedef SmartPointer<const Self>       ConstPointer;
    
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(CovariantThresholdInnerProductImageFilter, 
               BinaryFunctorImageFilter);

  // mostly for making the gradient negative, so we don't need a
  // separate filter to do it.
  void SetScale(double scale)
  {
    this->GetFunctor().SetScale(scale);
    this->Modified();
  }
  void SetAngleThresh(double angle)
  {
    this->GetFunctor().SetAngleThresh(angle);
    this->Modified();
  }

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  /** End concept checking */
#endif

protected:
  CovariantThresholdInnerProductImageFilter() 
    {
    this->SetScale(1.0);
    this->SetAngleThresh(M_PI);
    }
  virtual ~CovariantThresholdInnerProductImageFilter() {}
    
private:
  CovariantThresholdInnerProductImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
 
} // end namespace itk


#endif
