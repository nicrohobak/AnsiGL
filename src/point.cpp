/*
 * point.cpp
 *
 * A Point is a single place within a space...in this case, an X,Y coordinate.
 */


#include "AnsiGL/point.h"


namespace AnsiGL
{
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
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


