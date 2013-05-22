#include "..\include\pen.h"

namespace kami2dlib
{


    Color::Color()
        : r(0)
        , g(0)
        , b(0)
        , a(0)
    {}

    Color::~Color()
    {}


    DWORD Color::colorref()
    {
        return RGB(r,g,b);
    }

    // -------------------------------------------------------------------------



    Pen::Pen()
        : _width(1.0f)
        , _linestyle(0xFFFF)
    {
    }

    Pen::Pen(DWORD color)
        : _width(1.0f)
        , _linestyle(0xFFFF)
    {
        this->setColor(color);
    }

    Pen::Pen(BYTE r, BYTE g, BYTE b, BYTE alpha)
        : _width(1.0f)
        , _linestyle(0xFFFF)
    {
        this->setColor(r,g,b,alpha);
    }

    Pen::~Pen()
    {}


    FLOAT&	Pen::width()
    {
        return this->_width;
    }

    DWORD&	Pen::linestyle()
    {
    	return this->_linestyle;
    }

    Color&	Pen::color()
    {
        return this->_color;
    }

     void Pen::setColor(BYTE r, BYTE g, BYTE b, BYTE alpha)
    {
        this->_color.r = r;
        this->_color.g = g;
        this->_color.b = b;
        this->_color.a = alpha;
    }

    void Pen::setColor(DWORD color)
    {
        this->_color.r = GetRValue(color);
        this->_color.g = GetGValue(color);
        this->_color.b = GetBValue(color);
        this->_color.a = (LOBYTE((color)>>24));
    }

}

