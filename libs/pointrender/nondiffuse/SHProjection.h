#ifndef __SHProjection_h__
#define __SHProjection_h__

#include <vector>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <numeric>
#include <sstream>
#include <algorithm>


namespace Aqsis {

// Forward declaration needed by operator declarations below. Also defines the default template parameter.
template< typename TReal = double >
class SHProjection;

// Forward declarations of some template friend operators of class SHProjection.
template< typename TReal >
const SHProjection< TReal > operator + ( const SHProjection< TReal >& lhs, const SHProjection< TReal >& rhs );

template< typename TReal, typename TScalar >
const SHProjection< TReal > operator * ( TScalar lhs, const SHProjection< TReal >& rhs );

template< typename TReal, typename TScalar >
const SHProjection< TReal > operator * ( const SHProjection< TReal >& lhs, TScalar rhs );

template< typename TReal >
const SHProjection< TReal > operator * ( const SHProjection< TReal >& lhs, const SHProjection< TReal >& rhs );


/**
Represents a SH approximation of a function over the unit sphere.

To project a function in spherical coordinates into a real SH basis, simply pass the function
or functor as an argument to the appropriate constructor. The approximation is calculated
numerically with one of the available #IntegrationMethods (which may take a while; depending
on the desired order of approximation and numerical accuracy).

There's also support for a few basic operations like scaling, addition, convolution, etc.
*/
template< typename TReal >
class SHProjection
{
public:
	/// Enumeration of available numerical integration methods
	enum IntegrationMethods
	{
		IntegrationMethod_MC,			///< plain Monte Carlo integration over the sphere's surface
		IntegrationMethod_JitteredMC,	///< Monte Carlo integration using jittered sampling
		IntegrationMethod_Simpson,		///< integration using Simpson's rule

		IntegrationMethod_NumOf
	};

	static const TReal PI;

	/// Vector of TReals representing the SH-projection of a function
	typedef std::vector<TReal> SHCoefficients;

public:
	/// Construct from function/functor. You can specify the order of approximation (numBands),
	/// the numerical integration method to be used, and the number of samples.
	/// The template parameter TFunction should be a type that takes two TReals as arguments
	/// (spherical coordinates) and returns a TReal (function value) in order to avoid warnings
	/// regarding precision loss from implicit conversions.
	template < class TFunction >
	explicit SHProjection(
		const TFunction& function,
		int numBands = 3,
		int numSamples = 1000,
		IntegrationMethods integrationMethod = IntegrationMethod_Simpson );

	/// Construct from a vector of coefficients.
	/// Useful for functions that have a known projection or can be projected analytically.
	explicit SHProjection( const SHCoefficients& coefficients );

	/// Copy constructor
	SHProjection( const SHProjection& other );

	/// Copy assignment operator
	const SHProjection& operator = ( const SHProjection& other );

	/// Evaluates this projection at the spherical coordinates (theta, phi, 1).
	TReal Evaluate( TReal theta, TReal phi ) const;

	/// Evaluates this projection at the Cartesian coordinates (x,y,z).
	TReal Evaluate( TReal x, TReal y, TReal z ) const;

	/// Adds another SHProjection to this one (component-wise).
	/// If other has a higher order this SHProjection's order is increased.
	/// This is equivalent to a addition of the represented functions.
	const SHProjection& operator += ( const SHProjection& other );

	/// Multiplies this SHProjection by another one (component-wise).
	/// If other has a higher order this SHProjection's order is increased.
	const SHProjection& operator *= ( const SHProjection& other );

	/// Multiplies this SHProjection by a constant factor (component-wise).
	/// This is equivalent to a scaling of the represented function.
	const SHProjection& operator *= ( TReal factor );

	/// Performs component-wise addition of two SHProjections.
	/// The order of the result is the maximum order of the arguments.
	/// This is equivalent to an addition of the represented functions.
	friend const SHProjection operator + <> ( const SHProjection& lhs, const SHProjection& rhs );

	/// Performs component-wise multiplication of two SHProjections.
	/// The order of the result is the maximum order of the arguments.
	friend const SHProjection operator * <> ( const SHProjection& lhs, const SHProjection& rhs );

	/// Multiplies a SHProjection by a constant factor.
	/// This is equivalent to a scaling of the represented function.
//	template < typename TReal, class TFunction, typename TScalar >
	friend const SHProjection operator * <> ( TReal lhs, const SHProjection& rhs );

	/// Multiplies a SHProjection by a constant factor.
	/// This is equivalent to a scaling of the represented function.
//	template < typename TReal, class TFunction, typename TScalar >
	friend const SHProjection operator * <> ( const SHProjection& lhs, TReal rhs );

	/// Computes the inner product of two SHProjections (aka the dot product).
	/// This is equivalent to the integral of the product of the represented functions.
	static TReal InnerProduct( const SHProjection& lhs, const SHProjection& rhs );

	/// Convolves this SHProjection with a symmetric kernel function (left-convolution).
	/// kernelProjection has to be symmetric with respect to the z-axis (it must not
	/// depend on phi).
	void Convolve( const SHProjection& kernelProjection );

	/// Returns the SH coefficients of this projection.
	const SHCoefficients& GetProjection() const;

	/// Returns the SH coefficient belonging to Y(l,m).
	const TReal GetCoefficient( int l, int m ) const;

	/// Sets the SH coefficient belonging to Y(l,m).
	void SetCoefficient( int l, int m, TReal value );

	/// Returns the i-th SH coefficient.
	const TReal GetCoefficient( int i ) const;

	/// Sets the i-th SH coefficient.
	void SetCoefficient( int i, TReal value );

	/// Const access to the i-th SH coefficient.
	const TReal& operator [] ( int i ) const;

	/// Non-const access to the i-th SH coefficient.
	TReal& operator [] ( int i );

	/// Returns the number of bands used for this projection (ie the order of approximation).
	const int GetNumBands() const;

	/// Returns a formated string representation of this SHProjection.
	/// @todo take precision as parameter.
	std::string PrettyPrint() const;

private:
	int m_numBands;					///< number of bands used for this projection (order of approximation)
	SHCoefficients m_coefficients;	///< the SH coefficients of this projection

	/// Projects function into SH coefficients
	template < class TFunction >
	void project( const TFunction& function, int numBands, int numSamples, IntegrationMethods integrationMethod );

	/// Projects function using plain Monte Carlo integration.
	template < class TFunction >
	void project_MC( const TFunction& function, int numBands, int numSamples );

	/// Projects function using Jittered Monte Carlo integration.
	template < class TFunction >
	void project_JitteredMC( const TFunction& function, int numBands, int numSamples );

	/// Projects function using Simpson's rule.
	template < class TFunction >
	void project_Simpson( const TFunction& function, int numBands, int numSamples );

	/// Evaluates the Associated Legendre Polynomial with parameters (l,m) at x.
	static TReal P( int l, int m, TReal x );

	/// Returns the normalization constant for the SH basis function with parameters (l,m).
	static TReal K( int l, int m );

	/// Evaluates the real SH basis function with parameters (l,m) at (theta,phi).
	static TReal Y( int l, int m, TReal theta, TReal phi );

	/// Simple incremental factorial helper function.
	static int factorial( int n );

	/// Simple helper to get a sequence of independent uniformly distributed pseudo-random variables in [0,1).
	static TReal frand();

};

}

#endif
