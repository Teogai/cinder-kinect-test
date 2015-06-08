#include "Game.h"
#include "cinder/gl/Texture.h"

using namespace Kinect2;
using namespace ci;
using namespace ci::app;
using namespace std;

void Game::setup(){
	Rand::randSeed(time(NULL));
	for (int i = 0; i < 10; i++)
		vBall.push_back(Ball());
}

void Game::update(vector<Hand> mHands){
	vHands = mHands;
	for (const Hand& hand : vHands){
		for (vector<Ball>::iterator it = vBall.begin(); it != vBall.end(); ++it){
			float dist = distVec2f(hand.pos, (*it).pos);
			float threshold = 40.0f / hand.depth + (*it).size;
			if (dist < threshold){
				vBall.erase(it);
				vBall.push_back(Ball());
				break;
			}
		}
	}
}

void Game::draw(){
	gl::pushMatrices();
	gl::lineWidth(4.0f);
		
	for (const Ball& ball : vBall){
		gl::color(ball.color);
		gl::drawSolidCircle(ball.pos, ball.size, 32);
	
	}
	gl::popMatrices();
}

float Game::distVec2f(Vec2f a, Vec2f b){
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}