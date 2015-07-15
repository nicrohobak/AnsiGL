/*
 * point.cpp
 *
 * A Point is a single place within a space...in this case, an X,Y coordinate.
 */


#include "AnsiGL/point.h"


namespace AnsiGL
{
    Point2D::Point2D():
	m_X(0),
	m_Y(0)
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
	m_X = x;
    }

    void Point2D::Y( tPointType y )
    {
	m_Y = y;
    }

    void Point2D::Set( tPointType x, tPointType y )
    {
	X( x );
	Y( y );
    }

    Point2D &Point2D::operator+=( const Point2D &right )
    {
	m_X = m_X + right.m_X;
	m_Y = m_Y + right.m_Y;
	return (*this);
    }

    Point2D &Point2D::operator-=( const Point2D &right )
    {
	m_X = m_X - right.m_X;
	m_Y = m_Y - right.m_Y;
	return (*this);
    }

    bool Point2D::operator==( const Point2D &right ) const
    {
	return (m_X == right.m_X && m_Y == right.m_Y);
    }

    bool Point2D::operator!=( const Point2D &right ) const
    {
	return !((*this) == right);
    }


    Point2D operator+( const Point2D &left, const Point2D &right )
    {
	Point2D RetVal( left.m_X + right.m_X, left.m_Y + right.m_Y );
	return RetVal;
    }

    Point2D operator-( const Point2D &left, const Point2D &right )
    {
	Point2D RetVal( left.m_X - right.m_X, left.m_Y - right.m_Y );
	return RetVal;
    }


    Point3D::Point3D():
	m_Z(0)
    {
    }

    Point3D::Point3D( tPointType x, tPointType y, tPointType z ):
	m_Z(z)
    {
	m_X = x;
	m_Y = y;
    }

    Point3D::Point3D( const Point2D &point ):
	m_Z(0)
    {
	m_X = point.X();
	m_Y = point.Y();
    }

    Point3D::Point3D( tPointType x, tPointType y ):
	m_Z(0)
    {
	m_X = x;
	m_Y = y;
    }

    void Point3D::Z( tPointType z )
    {
	m_Z = z;
    }

    void Point3D::Set( tPointType x, tPointType y, tPointType z )
    {
	Point2D::Set( x, y );
	Z( z );
    }

    Point3D &Point3D::operator+=( const Point3D &right )
    {
	m_X = m_X + right.m_X;
	m_Y = m_Y + right.m_Y;
	m_Z = m_Z + right.m_Z;
	return (*this);
    }

    Point3D &Point3D::operator-=( const Point3D &right )
    {
	m_X = m_X - right.m_X;
	m_Y = m_Y - right.m_Y;
	m_Z = m_Z - right.m_Z;
	return (*this);
    }

    bool Point3D::operator==( const Point3D &right ) const
    {
	return (m_X == right.m_X && m_Y == right.m_Y && m_Z == right.m_Z);
    }

    bool Point3D::operator!=( const Point3D &right ) const
    {
	return !((*this) == right);
    }


    Point3D operator+( const Point3D &left, const Point3D &right )
    {
	Point3D RetVal( left.m_X + right.m_X, left.m_Y + right.m_Y, left.m_Z + right.m_Z );
	return RetVal;
    }

    Point3D operator-( const Point3D &left, const Point3D &right )
    {
	Point3D RetVal( left.m_X - right.m_X, left.m_Y - right.m_Y, left.m_Z - right.m_Z );
	return RetVal;
    }
}




