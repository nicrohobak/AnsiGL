#ifndef __ANSIGL_POINT_H__
#define __ANSIGL_POINT_H__

/*
 * point.h
 *
 * A Point is a single place within a space...in this case, an X,Y coordinate.
 */


#include <boost/shared_ptr.hpp>


namespace AnsiGL
{
    // A single data type for corrdinate systems based on AnsiGL::Point
    typedef int		tPointType;


    class Point2D
    {
    public:
	typedef boost::shared_ptr< Point2D >	Ptr;

    protected:
	tPointType	m_X;
	tPointType	m_Y;

    public:
	Point2D();
	Point2D( tPointType x, tPointType y );

	virtual ~Point2D();

	const tPointType &X() const		// Give the compiler the option to in-line
	{
	    return m_X;
	}
	void X( tPointType x );
	inline tPointType IncX() { return ++m_X; };
	inline tPointType DecX() { return --m_X; };

	const tPointType &Y() const		// Give the compiler the option to in-line
	{
	    return m_Y;
	}
	void Y( tPointType y );
	inline tPointType IncY() { return ++m_Y; };
	inline tPointType DecY() { return --m_Y; };

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
	typedef boost::shared_ptr< Point3D >	Ptr;

    protected:
	tPointType	m_Z;

    public:
	Point3D();
	Point3D( tPointType x, tPointType y, tPointType z );
	Point3D( const Point2D &point );
	Point3D( tPointType x, tPointType y );

	const tPointType &Z() const		// Give the compiler the option to in-line
	{
	    return m_Z;
	}
	void Z( tPointType Z );
	inline tPointType IncZ() { return ++m_Z; };
	inline tPointType DecZ() { return --m_Z; };

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


