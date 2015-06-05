#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"

#include "Kinect2.h"

struct Ball{
	ci::Vec2i	pos;
	ci::Color	color;
	float		size;

	Ball(){
		pos = ci::Vec2i(ci::Rand::randInt(ci::app::getWindowWidth()), ci::Rand::randInt(ci::app::getWindowHeight()));
		color = ci::Color(ci::Rand::randFloat(), ci::Rand::randFloat(), ci::Rand::randFloat());
		size = ci::Rand::randFloat(20.0f, 50.0f);
	}
};

struct Hand{
	ci::Vec2f	pos;
	float		depth;
	HandState	state;

	Hand(ci::Vec2f mPos, float mDepth, HandState mState){
		pos = mPos;
		depth = mDepth;
		state = mState;
	}
};

class Game{
public:
	void	setup();
	void	update(std::vector<Hand> mHands);
	void	draw();
	float	distVec2f(ci::Vec2f a, ci::Vec2f b);

private:
	std::vector<Ball>	vBall;
	std::vector<Hand>		vHands;
};