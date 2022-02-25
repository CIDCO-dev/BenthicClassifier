#ifndef SOUNDINGPOINT_HPP
#define SOUNDINGPOINT_HPP

struct SoundingPoint
{
  SoundingPoint(double intensity):intensity(intensity){

  }

  double intensity;  //backscatter intensity

  double sum;                 //  λ1 + λ2 + λ3
  double omnivariance;        // (λ1 · λ2 · λ3) ^ (1/3)

  double eigenentropy;        // - SIGMA( λi · ln(λi) )

  double anisotropy;          // (λ1 − λ3)/λ1
  double planarity;           // (λ2 − λ3)/λ1
  double linearity;           // (λ1 − λ2)/λ1
  double surfaceVariation;    // λ3/(λ1 + λ2 + λ3)
  double sphericity;          // λ3/λ1

  // Verticality features
  double verticality;         // 1 − | < [0 0 1], e3i >|      where <u,v> is the dot product

  // Moment features
  double momentOrder1Axis1;   // SIGMA(  < pi - p, e1 > )        p is centroid of cell
  double momentOrder1Axis2;   // SIGMA(  < pi - p, e2 > )        p is centroid of cell
  double momentOrder2Axis1;   // SIGMA(  < pi - p, e1 >^2 )      p is centroid of cell
  double momentOrder2Axis2;   // SIGMA(  < pi - p, e2 >^2 )      p is centroid of cell

  // Height features
  double verticalRange;
  double heightBelow;

  double heightAbove;

  //DBSCAN stuff
  int classId = -1;
  int octave = 0;

} ;

#endif
