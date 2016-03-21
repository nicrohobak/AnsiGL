#ifndef __ANSIGL_POINT_H__
#define __ANSIGL_POINT_H__

/*
 * point.h
 *
 * A Point is a single place within a space...in this case, an X,Y coordinate.
 */


#include "defines.h"


namespace AnsiGL
{
	// A single data type for corrdinate systems based on AnsiGL::Point
	typedef int		tPointType;


	class Point2D
	{
	public:
		ANSIGL_POINTERS( Point2D )

	public:
		Point2D():
			_X(0),
			_Y(0)
		{
		}

		Point2D( tPointType x, tPointType y )
		{
			X(x);
			Y(y);
		}

		virtual ~Point2D()
		{
		}

		const tPointType &X() const
		{
			return _X;
		}

		void X( tPointType x )
		{
			_X = x;
		}

		tPointType IncX()
		{
			return ++_X;
		}

		tPointType DecX()
		{
			return --_X;
		}

		const tPointType &Y() const
		{
			return _Y;
		}

		void Y( tPointType y )
		{
			_Y = y;
		}

		tPointType IncY()
		{
			return ++_Y;
		}

		tPointType DecY()
		{
			return --_Y;
		}

		void Set( tPointType x, tPointType y )
		{
			X( x );
			Y( y );
		}

		Point2D &operator+=( const Point2D &right )
		{
			_X = _X + right._X;
			_Y = _Y + right._Y;
			return (*this);
		}

		Point2D &operator-=( const Point2D &right )
		{
			_X = _X - right._X;
			_Y = _Y - right._Y;
			return (*this);
		}

		bool operator==( const Point2D &right ) const
		{
			return (_X == right._X && _Y == right._Y);
		}

		bool operator!=( const Point2D &right ) const
		{
			return !((*this) == right);
		}

		friend Point2D operator+( const Point2D &left, const Point2D &right );
		friend Point2D operator-( const Point2D &left, const Point2D &right );

	protected:
		tPointType	_X;
		tPointType	_Y;
	};


	//
	// Point2D arithmetic operators
	//
	Point2D operator+( const Point2D &left, const Point2D &right );
	Point2D operator-( const Point2D &left, const Point2D &right );


	// Since we now have a distinction...let the simple "Point" just refer to a Point2D by default
	typedef Point2D	Point;


	class Point3D : public Point2D
	{
	public:
		ANSIGL_POINTERS( Point3D )

	protected:
		tPointType	_Z;

	public:
		Point3D():
			_Z(0)
		{
		}

		Point3D( tPointType x, tPointType y, tPointType z ):
			_Z(z)
		{
			_X = x;
			_Y = y;
		}

		Point3D( const Point2D &point ):
			_Z(0)
		{
			_X = point.X();
			_Y = point.Y();
		}

		Point3D( tPointType x, tPointType y ):
			_Z(0)
		{
			_X = x;
			_Y = y;
		}

		const tPointType &Z() const
		{
			return _Z;
		}

		void Z( tPointType z )
		{
			_Z = z;
		}

		tPointType IncZ()
		{
			return ++_Z;
		}

		tPointType DecZ()
		{
			return --_Z;
		}

		void Set( tPointType x, tPointType y, tPointType z )
		{
			X( x );
			Y( y );
			Z( z );
		}

		Point3D &operator+=( const Point3D &right )
		{
			_X = _X + right._X;
			_Y = _Y + right._Y;
			_Z = _Z + right._Z;
			return (*this);
		}

		Point3D &operator-=( const Point3D &right )
		{
			_X = _X - right._X;
			_Y = _Y - right._Y;
			_Z = _Z - right._Z;
			return (*this);
		}

		bool operator==( const Point3D &right ) const
		{
			return (_X == right._X && _Y == right._Y && _Z == right._Z);
		}

		bool operator!=( const Point3D &right ) const
		{
			return !((*this) == right);
		}

		friend Point3D operator+( const Point3D &left, const Point3D &right );
		friend Point3D operator-( const Point3D &left, const Point3D &right );
	};


	//
	// Point3D arithmetic operators
	//
	Point3D operator+( const Point3D &left, const Point3D &right );
	Point3D operator-( const Point3D &left, const Point3D &right );
}


#endif // __ANSIGL_POINT_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


