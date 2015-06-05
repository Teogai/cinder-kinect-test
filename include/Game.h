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

class Game{
public:
	void	setup();
	void	update(Kinect2::Frame mFrame);
	void	draw();

private:
	std::vector<Ball>	vBall;
	Kinect2::Frame		mFrame;

};