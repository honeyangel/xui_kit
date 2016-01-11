#ifndef __xui_action_data_h__
#define __xui_action_data_h__

#if 0
#include "xui_vector.h"
#include "xui_colour.h"

class xui_action_data
{
public:
	/*
	//member
	*/
	bool					soft;
	bool					loop;
	std::vector<s32>		framevec;
	std::vector<s32>		movexvec;
	std::vector<s32>		moveyvec;
	std::vector<s32>		sizewvec;
	std::vector<s32>		sizehvec;
	std::vector<f32>		alphavec;
	std::vector<xui_colour>	colorvec;

	/*
	//constrctor
	*/
	xui_action_data( void )
	{
		soft = false;
		loop = false;
	}

	/*
	//operator
	*/
	xui_action_data& operator = ( const xui_action_data& other )
	{
		soft     = other.soft;
		loop     = other.loop;
		framevec = other.framevec;
		movexvec = other.movexvec;
		moveyvec = other.moveyvec;
		sizewvec = other.sizewvec;
		sizehvec = other.sizehvec;
		alphavec = other.alphavec;
		colorvec = other.colorvec;

		return (*this);
	}
};

#endif
#endif//__xui_action_data_h__