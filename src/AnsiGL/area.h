#ifndef __ANSIGL_AREA_H__
#define __ANSIGL_AREA_H__

/*
 * area.h
 *
 * A simple definition of a square area.i
 */


#include "point.h"


namespace AnsiGL
{
    // A single data type for all sizes based on AnsiGL::Area
    typedef int			tSizeType;

    //
    // A square area
    //
    class Area2D
    {
    public:
	ANSIGL_POINTERS( Area2D )

    protected:
	tSizeType	m_Width;
	tSizeType	m_Height;

    public:
	Area2D();
	Area2D( tSizeType width, tSizeType height );

	virtual ~Area2D();

	inline const tSizeType &Width() const
	{
	    return m_Width;
	}
	void Width( tSizeType width );

	inline const tSizeType &Height() const
	{
	    return m_Height;
	}
	void Height( tSizeType height );

	bool operator==( const Area2D &right ) const;
	bool operator!=( const Area2D &right ) const;

	bool operator>( const Area2D &right ) const;
	bool operator<( const Area2D &right ) const;

	bool operator>=( const Area2D &right ) const;
	bool operator<=( const Area2D &right ) const;

	void ResizeTo( const Area2D &size );
	void ResizeTo( tSizeType width, tSizeType height );
    };


    // For most things, an area is simply 2D anyway...
    typedef Area2D	Area;


    //
    // A cubic area
    //
    class Area3D : public Area2D
    {
    public:
	ANSIGL_POINTERS( Area3D )

    protected:
	tSizeType	m_Depth;

    public:
	Area3D();
	Area3D( tSizeType width, tSizeType height, tSizeType depth = 0 );

	inline const tSizeType &Depth() const
	{
	    return m_Depth;
	}
	void Depth( tSizeType depth );

	bool operator==( const Area3D &right ) const;
	bool operator!=( const Area3D &right ) const;

	bool operator>( const Area3D &right ) const;
	bool operator<( const Area3D &right ) const;

	bool operator>=( const Area3D &right ) const;
	bool operator<=( const Area3D &right ) const;

	void ResizeTo( const Area3D &size );
	void ResizeTo( tSizeType width, tSizeType height, tSizeType depth );
    };


    //
    // An area, fixed to a specific point
    //
    class FixedArea2D : public Area2D
    {
    public:
	ANSIGL_POINTERS( FixedArea2D )

    protected:
	Point2D		m_Point;	// The point to which this area is fixed

    public:
	FixedArea2D();
	FixedArea2D( const Area2D &size, const Point2D &pos = Point2D() );

	inline const Point2D &Point() const
	{
	    return m_Point;
	}
	void Point( const Point2D &point );

	inline const tPointType &X() const
	{
	    return m_Point.X();
	}
	void X( tPointType x );

	inline const tPointType &Y() const
	{
	    return m_Point.Y();
	}
	void Y( tPointType y );

	// NOTE: Since the ==/!= operators have not been defined here, any use of those operators will not compare the position itself, only the size of the fixed areas!
    };


    // For most things, an area is simply 2D anyway...
    typedef FixedArea2D		FixedArea;


    //
    // A 3D area, fixed to a specific point
    //
    class FixedArea3D : public Area3D
    {
    public:
	ANSIGL_POINTERS( FixedArea3D )

    protected:
	Point3D		m_Point;			// The position to which this area is fixed

    public:
	FixedArea3D();
	FixedArea3D( const Area3D &size, const Point3D &point );
	FixedArea3D( const Area3D &size, const Point2D &point );
	FixedArea3D( const Area2D &size, const Point3D &point );
	FixedArea3D( const Area2D &size, const Point2D &point );
	FixedArea3D( const Area3D &size );
	FixedArea3D( const Area2D &size );

	inline const Point3D &Point() const
	{
	    return m_Point;
	}
	void Point( const Point3D &point );

	inline const tPointType &X() const
	{
	    return m_Point.X();
	}
	void X( tPointType x );

	inline const tPointType &Y() const
	{
	    return m_Point.Y();
	}
	void Y( tPointType y );

	inline const tPointType &Z() const
	{
	    return m_Point.Z();
	}
	void Z( tPointType z );

	// NOTE: Since the ==/!= operators have not been defined here, any use of those operators will not compare the position itself, only the size of the fixed areas!
    };
}




#endif // __ANSIGL_AREA_H__


