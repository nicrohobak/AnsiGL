#ifndef __ANSIGL_CLONE_H__
#define __ANSIGL_CLONE_H__

/*
 * clone.h
 *
 * A content object that simply copies some other piece of content; effectively allowing multiple placements of the same content, without creating a full copy.
 */


#include "content.h"


namespace AnsiGL
{
	class Clone : public Content
	{
	public:
		ANSIGL_POINTERS( Clone )

	protected:
		Content::wPtr		_Target;

	public:
		Clone();
		Clone( Content::Ptr target );
		~Clone();

		virtual Content::Ptr Target() const;
		virtual void Target( Content::Ptr target );

		virtual std::string str();
		virtual std::string Render() const;
		virtual void RenderToSurface( Surface::Ptr dest, const Point2D &pos = Point2D() ) const;
	};
}



#endif // __ANSIGL_CLONE_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


