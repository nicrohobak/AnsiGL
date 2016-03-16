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

	protected:
		tPointType	_X;
		tPointType	_Y;

	public:
		Point2D();
		Point2D( tPointType x, tPointType y );

		virtual ~Point2D();

		const tPointType &X() const		// Give the compiler the option to in-line
		{
			return _X;
		}
		void X( tPointType x );
		inline tPointType IncX()
		{
			return ++_X;
		};
		inline tPointType DecX()
		{
			return --_X;
		};

		const tPointType &Y() const		// Give the compiler the option to in-line
		{
			return _Y;
		}
		void Y( tPointType y );
		inline tPointType IncY()
		{
			return ++_Y;
		};
		inline tPointType DecY()
		{
			return --_Y;
		};

		void Set( tPointType x, tPointType y );

		Point2D &operator+=( const Point2D &right );
		Point2D &operator-=( const Point2D &right );

		bool operator==( const Point2D &right ) const;
		bool operator!=( const Point2D &right ) const;

		friend Point2D operator+( const Point2D &left, const Point2D &right );
		friend Point2D operator-( const Point2D &left, const Point2D &right );
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
		Point3D();
		Point3D( tPointType x, tPointType y, tPointType z );
		Point3D( const Point2D &point );
		Point3D( tPointType x, tPointType y );

		const tPointType &Z() const		// Give the compiler the option to in-line
		{
			return _Z;
		}
		void Z( tPointType Z );
		inline tPointType IncZ()
		{
			return ++_Z;
		};
		inline tPointType DecZ()
		{
			return --_Z;
		};

		void Set( tPointType x, tPointType y, tPointType z );

		Point3D &operator+=( const Point3D &right );
		Point3D &operator-=( const Point3D &right );

		bool operator==( const Point3D &right ) const;
		bool operator!=( const Point3D &right ) const;

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
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


