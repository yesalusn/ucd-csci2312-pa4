//
// Created by Nikki on 4/2/2016.
//

#include <iostream>

#include "Exceptions.h"

using std::endl;

namespace Gaming
{
	//__name setter for Gaming Exceptions
	void GamingException::setName(std::string name)
	{
		__name = name;
	}

	// insertion operator for Gaming Exceptions
	std::ostream &operator<<(std::ostream &os, const GamingException &ex)
	{
		ex.__print_args(os);
		return os;
	}

	/*
	 * Dimension Exceptions Class Constructor
	 * --Derived class of Gaming Exceptions
	 */
	DimensionEx::DimensionEx(unsigned expWidth, unsigned expHeight, unsigned width, unsigned height):
			__exp_width(expWidth), __exp_height(expHeight), __width(width), __height(height){}

	//getters
	unsigned DimensionEx::getExpWidth() const
	{
		return __exp_width;
	}

	unsigned DimensionEx::getExpHeight() const
	{
		return __exp_height;
	}

	//returns the message about the dimensions exception for width
	unsigned DimensionEx::getWidth() const
	{
		return __width;
	}

	//returns the message about the dimensions exception for height
	unsigned DimensionEx::getHeight() const
	{
		return __height;
	}

	/*
	 * Insufficient Dimensions Exception Class Constructor
	 * --Derived class of Dimension Exception
	 */
	InsufficientDimensionsEx::InsufficientDimensionsEx(unsigned minWidth, unsigned minHeight,
	                                                   unsigned width, unsigned height): DimensionEx(minWidth, minHeight, width, height)
	{
		this->setName("InsufficientDimensionsEx");
	}

	//Insufficient Dimensions Exception Class print arguments
	void InsufficientDimensionsEx::__print_args(std::ostream &os) const
	{
		os << InsufficientDimensionsEx::getName() << "(minWidth=" << getExpWidth() << ", minHeight=" << getExpHeight()
		<< ", width=" << getWidth() << ", height=" << getHeight() << ")";
	}

	/*
	 * Out Of Bounds Exception Class Constructor
	 * --Derived class of Dimension Exception
	 */
	OutOfBoundsEx::OutOfBoundsEx(unsigned maxWidth, unsigned maxHeight, unsigned width
			, unsigned height): DimensionEx(maxWidth, maxHeight, width, height)
	{
		this->setName("OutOfBoundsEx");
	}

	//Out of Bounds Exception Class print arguments
	void OutOfBoundsEx::__print_args(std::ostream &os) const
	{
		os << OutOfBoundsEx::getName() << "(maxWidth=" << getExpWidth() << ", maxHeight=" << getExpHeight()
		<< ", width=" << getWidth() << ", height=" << getHeight() << ")";
	}

	void PositionEx::__print_args(std::ostream &os) const
	{
		os << PositionEx::getName() << " ," << endl;
	}

	PositionEx::PositionEx(unsigned x, unsigned y):
	__x(x), __y(y)
	{}

	PositionNonemptyEx::PositionNonemptyEx(unsigned x, unsigned y):
	PositionEx(x, y)
	{
		this->setName("PositionNonemptyEx");
	}

	PositionEmptyEx::PositionEmptyEx(unsigned x, unsigned y):
	PositionEx(x, y)
	{
		this->setName("PositionEmptyEx");
	}

	void PosVectorEmptyEx::__print_args(std::ostream &os) const
	{
		os << PosVectorEmptyEx::getName() << endl;
	}

	PosVectorEmptyEx::PosVectorEmptyEx()
	{
		this->setName("PosVectorEmptyEx");
	}
}