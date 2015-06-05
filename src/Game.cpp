#include "Game.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"

using namespace Kinect2;
using namespace ci;
using namespace ci::app;
using namespace std;

void Game::setup(){
	Rand::randSeed(time(NULL));
	vBall.push_back(Ball());
	vBall.push_back(Ball());
	vBall.push_back(Ball());
}

void Game::update(Frame frame){
	mFrame = frame;
}

void Game::draw(){
	console() << ci::Rand::randFloat() << endl;
	if (mFrame.getColor()) {
		gl::pushMatrices();
		gl::lineWidth(4.0f);
		
		for (const Ball& ball : vBall){
			gl::color(ball.color);
			gl::drawSolidCircle(ball.pos, ball.size, 32);
			
		}
		gl::popMatrices();
	}
}