
#include "SHProjection.h"

namespace Aqsis {

// not without danger, but it works on MSVC...
template< typename TReal >
const TReal SHProjection< TReal >::PI = static_cast< TReal >( acos(-1.0) );


template< typename TReal >
template< class TFunction >
inline
SHProjection< TReal >::SHProjection(
	const TFunction& function,
	int numBands,
	int numSamples,
	IntegrationMethods integrationMethod )
	:
	m_numBands( numBands )
{
	project( function, numBands, numSamples, integrationMethod );
}

template< typename TReal >
inline
SHProjection< TReal >::SHProjection( const SHCoefficients& coeffs )
{
	// init m_numBands
	TReal temp = ceil( sqrt( static_cast< TReal >( coeffs.size() ) ) );
	m_numBands = static_cast<int>( temp );

	// copy coefficients over (empty places in the highest band will be set to zero)
	m_coefficients.resize( m_numBands * m_numBands, 0.0 );
	std::copy( coeffs.begin(), coeffs.end(), m_coefficients.begin() );
}

template< typename TReal >
inline
SHProjection< TReal >::SHProjection( const SHProjection& other )
	: m_numBands( other.m_numBands )
	, m_coefficients( other.m_coefficients )
{
}

template< typename TReal >
inline
const SHProjection< TReal >& SHProjection< TReal >::operator = ( const SHProjection& other )
{
	m_numBands = other.m_numBands;
	m_coefficients = other.m_coefficients;

	return *this;
}

template< typename TReal >
TReal SHProjection< TReal >::Evaluate( TReal theta, TReal phi ) const
{
	TReal result = 0.0;

	for( int l=0; l<m_numBands; ++l )
		for( int m=-l; m<=l; ++m )
			result += GetCoefficient(l,m) * Y(l,m,theta,phi);

	return result;
}

template< typename TReal >
TReal SHProjection< TReal >::Evaluate( TReal x, TReal y, TReal z ) const
{
	// first get the spherical coordinates ...
	TReal theta = acos(z);
	TReal t = x / sqrt(x*x + y*y);
	TReal phi = y >= 0 ? acos(t) : 2*PI - acos(t);

	// ... then call the spherical evaluate
	return Evaluate( theta, phi );
}

template< typename TReal >
inline
const SHProjection< TReal >& SHProjection< TReal >::operator += ( const SHProjection& other )
{
	// resize vector of coefficients if other has a higher order.
	if( other.m_coefficients.size() > m_coefficients.size() )
		m_coefficients.resize( other.m_coefficients.size(), 0.0 );

	// add coefficients
	const std::size_t n = std::min( m_coefficients.size(), other.m_coefficients.size() );
	for( std::size_t i = 0; i<n; ++i )
		m_coefficients[i] += other.m_coefficients[i];

	return *this;
}

template< typename TReal >
inline
const SHProjection< TReal >& SHProjection< TReal >::operator *= ( const SHProjection& other )
{
	// resize vector of coefficients if other has a higher order.
	if( other.m_coefficients.size() > m_coefficients.size() )
		m_coefficients.resize( other.m_coefficients.size(), 0.0 );

	// multiply coefficients
	const std::size_t n = std::min( m_coefficients.size(), other.m_coefficients.size() );
	for( std::size_t i = 0; i<n; ++i )
		m_coefficients[i] *= other.m_coefficients[i];

	return *this;
}

template< typename TReal >
inline
const SHProjection< TReal >& SHProjection< TReal >::operator *= ( TReal factor )
{
	std::transform( m_coefficients.begin(), m_coefficients.end(), m_coefficients.begin(),
		std::bind2nd( std::multiplies<TReal>(), factor ) );

//	const std::size_t n = m_coefficients.size();
// 	for( std::size_t i=0; i<n; ++i )
// 		m_coefficients[i] *= factor;

	return *this;
}

template< typename TReal>
inline
const SHProjection< TReal > operator + ( const SHProjection< TReal >& lhs, const SHProjection< TReal >& rhs )
{
	SHProjection< TReal > ret( lhs );
	ret += rhs;
	return ret;
}

template< typename TReal>
inline
const SHProjection< TReal > operator * ( const SHProjection< TReal >& lhs, const SHProjection< TReal >& rhs )
{
	SHProjection< TReal > ret( lhs );
	ret *= rhs;
	return ret;
}

template< typename TReal, typename TScalar >
inline
const SHProjection< TReal > operator * ( TScalar lhs, const SHProjection< TReal >& rhs )
{
	SHProjection< TReal > ret( rhs );
	ret *= static_cast< TReal >( lhs );
	return ret;
}

template< typename TReal, typename TScalar >
inline
const SHProjection< TReal > operator * ( const SHProjection< TReal >& lhs, TScalar rhs )
{
	SHProjection< TReal > ret( lhs );
	ret *= static_cast< TReal >( rhs );
	return ret;
}

template< typename TReal>
inline
TReal SHProjection< TReal >::InnerProduct( const SHProjection< TReal >& lhs, const SHProjection< TReal >& rhs )
{
	const std::size_t n = std::min( lhs.m_coefficients.size(), rhs.m_coefficients.size() );
	return std::inner_product(
		lhs.m_coefficients.begin(), lhs.m_coefficients.begin() + n,
		rhs.m_coefficients.begin(),
		TReal() );
}

template< typename TReal>
inline
void SHProjection< TReal >::Convolve( const SHProjection< TReal >& kernelProjection )
{
	const int numBands = std::min( m_numBands, kernelProjection.m_numBands );
	for( int l=0; l<numBands; ++l )
	{
		const TReal alpha = sqrt( 4*PI / (2*l + 1) );
		for( int m=-l; m<=l; ++m )
		{
			m_coefficients[ l*(l+1) + m ] *= alpha * kernelProjection.GetCoefficient(l, 0);
		}
	}
}

template< typename TReal>
inline
const typename SHProjection< TReal >::SHCoefficients& SHProjection< TReal >::GetProjection() const
{
	return m_coefficients;
}

template< typename TReal >
inline
const TReal SHProjection< TReal >::GetCoefficient( int l, int m ) const
{
	return m_coefficients[ l*(l+1) + m ];
}

template< typename TReal >
inline
void SHProjection< TReal >::SetCoefficient( int l, int m, TReal value )
{
	m_coefficients[ l*(l+1) + m ] = value;
}

template< typename TReal >
inline
const TReal SHProjection< TReal >::GetCoefficient( int i ) const
{
	return m_coefficients[i];
}

template< typename TReal >
inline
void SHProjection< TReal >::SetCoefficient( int i, TReal value )
{
	m_coefficients[i] = value;
}

template< typename TReal >
inline
const TReal& SHProjection< TReal >::operator [] (int i) const
{
	return m_coefficients[i];
}

template< typename TReal >
inline
TReal& SHProjection< TReal >::operator [] (int i)
{
	return m_coefficients[i];
}

template< typename TReal >
inline
const int SHProjection< TReal >::GetNumBands() const
{
	return m_numBands;
}

template< typename TReal >
template< class TFunction >
void SHProjection< TReal >::project( const TFunction& function, int numBands, int numSamples, IntegrationMethods integrationMethod )
{
	if( integrationMethod == IntegrationMethod_JitteredMC )
		project_JitteredMC( function, numBands, numSamples );
	else if( integrationMethod == IntegrationMethod_Simpson )
		project_Simpson( function, numBands, numSamples );
	else
		project_MC( function, numBands, numSamples );
}

template< typename TReal >
template< class TFunction >
void SHProjection< TReal >::project_MC( const TFunction& function, int numBands, int numSamples )
{
	const int numCoeffs = numBands*numBands;

	// prepare the coefficients vector
	m_coefficients.clear();
	m_coefficients.resize( numCoeffs, 0 );

	for( int i=0; i<numSamples; ++i )
	{
		TReal xi_1 = frand();	// get two independent, uniformly distributed
		TReal xi_2 = frand();	// random numbers in [0,1) ...

		TReal theta = 2 * acos( sqrt( xi_1 ) );		// ... and generate points in spherical coordinates
		TReal phi = 2 * PI * xi_2;					// uniformly distributed over the sphere's surface.

		for( int l=0; l<numBands; ++l )
		{
			for( int m=-l; m<=l; ++m )
			{
				int index = l*(l+1) + m;
				m_coefficients[index] += Y(l,m,theta,phi) * function(theta, phi);	// take a sample and add it to the sum
			}
		}
	}

	// divide by the number of samples and multiply by the area
	// of the integration region to get the final result
	const TReal normalizationFactor = 4*PI / numSamples;
	for( int k=0; k<numCoeffs; ++k )
		m_coefficients[k] = m_coefficients[k] * normalizationFactor;

}

template< typename TReal >
template< class TFunction >
void SHProjection< TReal >::project_JitteredMC( const TFunction& function, int numBands, int numSamples )
{
	const int numCoeffs = numBands*numBands;

	// prepare the coefficients vector
	m_coefficients.clear();
	m_coefficients.resize( numCoeffs, 0 );

	// we want to split the sphere's surface into 2n x n strata
	// and take one random sample in each stratum
	const int n = static_cast< int >( ceil( sqrt(numSamples*0.5) ) );
	const TReal h = static_cast< TReal >(1.0)/n;

	for( int j=0; j<2*n; ++j )
	{
		for( int i=0; i<n; ++i )
		{
			// get a random position in the current stratum
			TReal zeta_1 = (i + frand()) * h;
			TReal zeta_2 = (j + frand()) * h;

			TReal theta = 2 * acos( sqrt( 1 - zeta_1) );
			TReal phi = 2 * PI * zeta_2;


			for( int l=0; l<numBands; ++l )
			{
				for( int m=-l; m<=l; ++m )
				{
					int index = l*(l+1) + m;
					m_coefficients[index] += Y(l,m,theta,phi) * function(theta, phi);
				}
			}
		}
	}

	// scale the result
	const TReal normalizationFactor = (4*PI) / (2*n*n);
	for( int k=0; k<numCoeffs; ++k )
		m_coefficients[k] = m_coefficients[k] * normalizationFactor;

}

template< typename TReal >
template< class TFunction >
void SHProjection< TReal >::project_Simpson( const TFunction& function, int numBands, int numSamples )
{
	const int numCoeffs = numBands*numBands;

	m_coefficients.clear();
	m_coefficients.resize( numCoeffs, 0 );

	int n = static_cast< int >( ceil( sqrt(numSamples * 0.5) ) );	// n is number of samples we will take
	n = (n % 2) == 0 ? n : n+1;	// make sure n is even
	int i,j;
	const TReal h = PI/n;		// h is the step size
	TReal theta = 0.0;
	TReal phi = 0.0;
	
	// evaluate interior samples with weight 16
	TReal weight = 16;
	for( j=1; j<=2*n-1; j+=2 )
	{
		for( i=1; i<=n-1; i+=2 )
		{
			theta = i*h;
			phi = j*h;

			for( int l=0; l<numBands; ++l )
			{
				for( int m=-l; m<=l; ++m )
				{
					int index = l*(l+1) + m;

					m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
				}
			}

		}
	}

	// evaluate interior samples with weight 4
	weight = 4;
	for( j=2; j<=2*n-2; j+=2 )
	{
		for( i=2; i<=n-2; i+=2 )
		{
			theta = i*h;
			phi = j*h;

			for( int l=0; l<numBands; ++l )
			{
				for( int m=-l; m<=l; ++m )
				{
					int index = l*(l+1) + m;

					m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
				}
			}
		}
	}

	// evaluate interior samples with weight 4
	weight = 8;
	for( j=1; j<=2*n-1; ++j )
	{
		int mod = j % 2;
		for( i=1+mod; i<=n-1-mod; i+=2 )
		{
			theta = i*h;
			phi = j*h;

			for( int l=0; l<numBands; ++l )
			{
				for( int m=-l; m<=l; ++m )
				{
					int index = l*(l+1) + m;

					m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
				}
			}

		}
	}

	// evaluate boundary samples with weight 1 (just the corners)
	weight = 1;
	for( int l=0; l<numBands; ++l )
	{
		for( int m=-l; m<=l; ++m )
		{
			int index = l*(l+1) + m;

			theta = 0;
			phi = 0;
			m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
			theta = PI;
			phi = 0;
			m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
			theta = PI;
			phi = 2*PI;
			m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
			theta = 0;
			phi = 2*PI;
			m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
		}
	}

	// evaluate boundary samples with weight 4 (on "vertical" boundaries)
	weight = 4;
	for( j=1; j<=2*n-1; j+=2 )
	{
		phi = j*h;

		for( int l=0; l<numBands; ++l )
		{
			for( int m=-l; m<=l; ++m )
			{
				int index = l*(l+1) + m;

				theta = 0.0;
				m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
				theta = PI;
				m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
			}
		}

	}

	// evaluate boundary samples with weight 4 (on "horizontal" boundaries)
	weight = 4;
	for( i=1; i<=n-1; i+=2 )
	{
		theta = i*h;

		for( int l=0; l<numBands; ++l )
		{
			for( int m=-l; m<=l; ++m )
			{
				int index = l*(l+1) + m;

				phi = 0;
				m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
				phi = 2*PI;
				m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
			}
		}

	}

	// evaluate boundary samples with weight 2 (on "vertical" boundaries)
	weight = 2;
	for( j=2; j<=2*n-2; j+=2 )
	{
		phi = j*h;

		for( int l=0; l<numBands; ++l )
		{
			for( int m=-l; m<=l; ++m )
			{
				int index = l*(l+1) + m;

				theta = 0;
				m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
				theta = PI;
				m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
			}
		}

	}

	// evaluate boundary samples with weight 2 (on "horizontal" boundaries)
	weight = 2;
	for( i=2; i<=n-2; i+=2 )
	{
		theta = i*h;

		for( int l=0; l<numBands; ++l )
		{
			for( int m=-l; m<=l; ++m )
			{
				int index = l*(l+1) + m;

				phi = 0;
				m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
				phi = 2*PI;
				m_coefficients[index] += weight * Y(l,m,theta,phi) * function(theta, phi) * sin(theta);
			}
		}

	}

	// normalization
	const TReal normalizationFactor = (4*2*PI*PI) / (36*2*n*n);
	for( int k=0; k<numCoeffs; ++k )
		m_coefficients[k] = m_coefficients[k] * normalizationFactor;

}

template< typename TReal >
TReal SHProjection< TReal >::P( int l, int m, TReal x )
{
	// Code taken from 'Robin Green - Spherical Harmonic Lighting'.
	TReal pmm = 1;
	if( m > 0 )
	{
		TReal somx2 = sqrt( (1-x)*(1+x) );
		TReal fact = 1;
		for( int i=1; i<=m; ++i )
		{
			pmm *= -fact * somx2;
			fact += 2;
		}
	}

	if( l == m )
		return pmm;

	TReal pmmp1 = x * (2*m+1) * pmm;
	if( l == m+1 )
		return pmmp1;

	TReal pll = 0.0;
	for( int ll=m+2; ll<=l; ++ll )
	{
		pll = ( (2*ll-1) * x * pmmp1 - (ll+m-1) * pmm ) / (ll-m);
		pmm = pmmp1;
		pmmp1 = pll;
	}

	return pll;
}

template< typename TReal >
TReal SHProjection< TReal >::K( int l, int m )
{
	return sqrt( ( (2*l+1) * factorial(l-m) ) / ( 4*PI * factorial(l+m) ) );
}

template< typename TReal >
TReal SHProjection< TReal >::Y( int l, int m, TReal theta, TReal phi )
{
	const TReal sqrt2 = sqrt( static_cast< TReal >(2.0) );
	if( m == 0 )
		return K(l,0) * P(l,m,cos(theta));
	else if( m > 0 )
		return sqrt2 * K(l,m) * cos(m*phi) * P(l,m,cos(theta));
	else
		return sqrt2 * K(l,-m) * sin(-m*phi) * P(l,-m,cos(theta));
}

template< typename TReal >
std::string SHProjection< TReal >::PrettyPrint() const
{
	std::stringstream ss;
	for( int l=0; l<m_numBands; ++l )
		for( int m=-l; m<=l; ++m )
			ss << GetCoefficient(l, m) << ( m == l ? "\n" : ", " );

	return ss.str();
}

template< typename TReal > 
inline
TReal SHProjection< TReal >::frand()
{
	return static_cast< TReal >( rand() ) / RAND_MAX;
}

template< typename TReal >
inline
int SHProjection< TReal >::factorial( int n )
{
	int result = 1;

	for( int i=2; i<=n; ++i )
		result *= i;

	return result;
}

}
