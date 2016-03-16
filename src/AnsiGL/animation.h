#ifndef __ANSIGL_ANIMATION_H__
#define __ANSIGL_ANIMATION_H__

/*
 * animation.h
 *
 *
 */

#include "image.h"


namespace AnsiGL
{
	// Just in case Frames need to be expanded into something more
	typedef Image	Frame;


	class Animation : public Content
	{
	public:
		ANSIGL_POINTERS( Animation )

		Animation()
		{
			m_CurFrame = m_Frames.end();
		}

		Image::Ptr CurFrame() const;

		//
		// Note: If certain frames need to be offset for the animation to look
		//	 correct...  Remember that all Content-based objects have access
		//	 to Content::Offset.  Try setting the frame's offset before
		//	 adding it to the Animation.
		//
		void AddFrame( Frame::Ptr frame );
		void RemoveFrame( Frame::Ptr frame );
		void RemoveFrame( unsigned int frameNum );
		void Clear()
		{
			m_Frames.clear();
		}

		void Reset()
		{
			m_CurFrame = m_Frames.begin();
		}
		void Advance( unsigned int numFrames = 1 );
		void Rewind( unsigned int numFrames = 1 );

		// The render functions simply render the current frame
		virtual std::string str();
		virtual std::string Render() const;
		virtual void RenderToSurface( Surface::Ptr dest, Point2D pos = Point2D() ) const;

	protected:
		typedef std::list< Frame::Ptr >::iterator		iterator;
		typedef std::list< Frame::Ptr >::const_iterator		const_iterator;
		typedef std::list< Frame::Ptr >::reverse_iterator	reverse_iterator;
		typedef std::list< Frame::Ptr >::const_reverse_iterator	const_reverse_iterator;

		std::list< Frame::Ptr >	m_Frames;
		iterator		m_CurFrame;
	};
}


#endif // __ANSIGL_ANIMATION_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


