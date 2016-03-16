/*
 * point.cpp
 *
 * A Point is a single place within a space...in this case, an X,Y coordinate.
 */


#include "AnsiGL/point.h"


namespace AnsiGL
{
	Point2D::Point2D():
		_X(0),
		_Y(0)
	{
	}

	Point2D::Point2D( tPointType x, tPointType y )
	{
		X(x);
		Y(y);
	}

	Point2D::~Point2D()
	{
	}

	void Point2D::X( tPointType x )
	{
		_X = x;
	}

	void Point2D::Y( tPointType y )
	{
		_Y = y;
	}

	void Point2D::Set( tPointType x, tPointType y )
	{
		X( x );
		Y( y );
	}

	Point2D &Point2D::operator+=( const Point2D &right )
	{
		_X = _X + right._X;
		_Y = _Y + right._Y;
		return (*this);
	}

	Point2D &Point2D::operator-=( const Point2D &right )
	{
		_X = _X - right._X;
		_Y = _Y - right._Y;
		return (*this);
	}

	bool Point2D::operator==( const Point2D &right ) const
	{
		return (_X == right._X && _Y == right._Y);
	}

	bool Point2D::operator!=( const Point2D &right ) const
	{
		return !((*this) == right);
	}


	Point2D operator+( const Point2D &left, const Point2D &right )
	{
		Point2D RetVal( left._X + right._X, left._Y + right._Y );
		return RetVal;
	}

	Point2D operator-( const Point2D &left, const Point2D &right )
	{
		Point2D RetVal( left._X - right._X, left._Y - right._Y );
		return RetVal;
	}


	Point3D::Point3D():
		_Z(0)
	{
	}

	Point3D::Point3D( tPointType x, tPointType y, tPointType z ):
		_Z(z)
	{
		_X = x;
		_Y = y;
	}

	Point3D::Point3D( const Point2D &point ):
		_Z(0)
	{
		_X = point.X();
		_Y = point.Y();
	}

	Point3D::Point3D( tPointType x, tPointType y ):
		_Z(0)
	{
		_X = x;
		_Y = y;
	}

	void Point3D::Z( tPointType z )
	{
		_Z = z;
	}

	void Point3D::Set( tPointType x, tPointType y, tPointType z )
	{
		Point2D::Set( x, y );
		Z( z );
	}

	Point3D &Point3D::operator+=( const Point3D &right )
	{
		_X = _X + right._X;
		_Y = _Y + right._Y;
		_Z = _Z + right._Z;
		return (*this);
	}

	Point3D &Point3D::operator-=( const Point3D &right )
	{
		_X = _X - right._X;
		_Y = _Y - right._Y;
		_Z = _Z - right._Z;
		return (*this);
	}

	bool Point3D::operator==( const Point3D &right ) const
	{
		return (_X == right._X && _Y == right._Y && _Z == right._Z);
	}

	bool Point3D::operator!=( const Point3D &right ) const
	{
		return !((*this) == right);
	}


	Point3D operator+( const Point3D &left, const Point3D &right )
	{
		Point3D RetVal( left._X + right._X, left._Y + right._Y, left._Z + right._Z );
		return RetVal;
	}

	Point3D operator-( const Point3D &left, const Point3D &right )
	{
		Point3D RetVal( left._X - right._X, left._Y - right._Y, left._Z - right._Z );
		return RetVal;
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


