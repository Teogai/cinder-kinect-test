#pragma once
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"

#include "Kinect2.h"

struct Ball{
	ci::Vec2f	pos;
	ci::Color	color;
	ci::Rand	rand;

	Ball(){
		pos = ci::Vec2f(rand.nextFloat(), rand.nextFloat());
	}
};

class Game{
public:
	void	setup();
	void	update();
	void	draw();

private:
	std::vector<Ball>	vBall;

};