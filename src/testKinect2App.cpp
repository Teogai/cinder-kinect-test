
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/params/Params.h"

#include "Kinect2.h"
#include "Game.h"

using namespace Kinect2;


struct Hand{
	Body::Joint joint;
	HandState state;

	Hand(Body::Joint mJoint, HandState mState){
		joint = mJoint;
		state = mState;
	}
};

class testKinect2App : public ci::app::AppBasic 
{
public:
	void						draw();
	void						prepareSettings( ci::app::AppBasic::Settings* settings );
	void						setup();
	void						update();
	void						updateHands();
	void						drawHands();


private:
	Kinect2::DeviceRef			mDevice;
	Kinect2::Frame				mFrame;

	float						mFrameRate;
	bool						mFullScreen;
	ci::params::InterfaceGlRef	mParams;

	Game						game;

	std::vector<Hand>			vHands;

	//std::vector<Body::Joint> 	handRight;
	//std::vector<Body::Joint> 	thumbRight;
	//std::vector<Body::Joint> 	handLeft;
	//std::vector<Body::Joint> 	thumbLeft;
};

#include "cinder/Font.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void testKinect2App::draw()
{
	gl::setViewport( getWindowBounds() );
	gl::clear(Colorf::black());
	gl::enableAlphaBlending();
	gl::color(ColorAf::white());

	if (mFrame.getColor()) {
		gl::TextureRef tex = gl::Texture::create(mFrame.getColor());
		gl::draw(tex, tex->getBounds(), Rectf(getWindowBounds()));
	}

	drawHands();
	game.draw();
	//Draw 4 Frames
	/*
	if ( mFrame.getColor() ) {
		gl::TextureRef tex = gl::Texture::create( mFrame.getColor() );
		gl::draw( tex, tex->getBounds(), Rectf( Vec2f::zero(), getWindowCenter() ) );
	}
	if ( mFrame.getDepth() ) {
		gl::TextureRef tex = gl::Texture::create( Kinect2::channel16To8( mFrame.getDepth() ) );
		gl::draw( tex, tex->getBounds(), Rectf( getWindowCenter().x, 0.0f, (float)getWindowWidth(), getWindowCenter().y ) );
	}
	if ( mFrame.getInfrared() ) {
		gl::TextureRef tex = gl::Texture::create( mFrame.getInfrared() );
		gl::draw( tex, tex->getBounds(), Rectf( 0.0f, getWindowCenter().y, getWindowCenter().x, (float)getWindowHeight() ) );
	}
	if ( mFrame.getBodyIndex() ) {
		gl::TextureRef tex = gl::Texture::create( Kinect2::colorizeBodyIndex( mFrame.getBodyIndex() ) );
		gl::draw( tex, tex->getBounds(), Rectf( getWindowCenter(), Vec2f( getWindowSize() ) ) );
	}
	*/

	mParams->draw();
}

void testKinect2App::prepareSettings(Settings* settings)
{
	settings->prepareWindow( Window::Format().size( 1280, 720 ).title( "Basic App" ) );
	settings->setFrameRate( 60.0f );
}

void testKinect2App::setup()
{	
	gl::enable( GL_TEXTURE_2D );
	
	mFrameRate	= 0.0f;
	mFullScreen	= false;

	mDevice = Kinect2::Device::create();
	mDevice->start( Kinect2::DeviceOptions().enableInfrared().enableBodyIndex().enableBody() );

	mParams = params::InterfaceGl::create( "Params", Vec2i( 200, 100 ) );
	mParams->addParam( "Frame rate",	&mFrameRate,				"", true );
	mParams->addParam( "Full screen",	&mFullScreen,				"key=f" );
	mParams->addButton("Quit", bind(&testKinect2App::quit, this), "key=q");

	game.setup();
}

void testKinect2App::update()
{
	mFrameRate = getAverageFps();
	
	if ( mFullScreen != isFullScreen() ) {
		setFullScreen( mFullScreen );
		mFullScreen = isFullScreen();
	}

	if ( mDevice && mDevice->getFrame().getTimeStamp() > mFrame.getTimeStamp() ) {
		mFrame = mDevice->getFrame();
	}

	updateHands();
	game.update(mFrame);
}

void testKinect2App::updateHands(){
	vHands.clear();
	for (const Kinect2::Body& body : mDevice->getFrame().getBodies()) {
		vHands.push_back(Hand(body.getJointMap().at(JointType_HandLeft), body.getHandLeftState()));
		vHands.push_back(Hand(body.getJointMap().at(JointType_HandRight), body.getHandRightState()));
	}
}

void testKinect2App::drawHands(){
	if (mFrame.getColor() && mDevice) {
		gl::pushMatrices();
		gl::scale(Vec2f(getWindowSize()) / Vec2f(mFrame.getColor().getSize()));
		gl::lineWidth(4.0f);

		for (const Hand& hand : vHands){
			switch (hand.state){
				case HandState_Closed:
					gl::color(1, 0, 0);	 break;
				case HandState_Lasso:
					gl::color(1, 0.7f, 0);	 break;
				case HandState_Open:
					gl::color(0, 1, 0);	 break;
				default: 
					gl::color(ColorAf::white());
			}
			Vec3f handPos = hand.joint.getPosition();
			Vec2f pos = Kinect2::mapBodyCoordToColor(handPos, mDevice->getCoordinateMapper());
			gl::drawStrokedCircle(pos, 40.0f / handPos.z, 32);
		}

		/*
		for (const auto& joint : body.getJointMap()) {
			Vec2f pos = Kinect2::mapBodyCoordToDepth(joint.second.getPosition(), mDevice->getCoordinateMapper());
			if (joint.first == JointType_HandLeft || joint.first == JointType_HandTipLeft || joint.first == JointType_ThumbLeft){
				if (body.getHandLeftState() == HandState_Closed) gl::color(255, 0, 0);
				else if (body.getHandLeftState() == HandState_Lasso) gl::color(255, 255, 0);
				else if (body.getHandLeftState() == HandState_Open) gl::color(0, 255, 0);
				else  gl::color(ColorAf::white());
			}
			else if (joint.first == JointType_HandRight || joint.first == JointType_HandTipRight || joint.first == JointType_ThumbRight){
				if (body.getHandRightState() == HandState_Closed) gl::color(255, 0, 0);
				else if (body.getHandRightState() == HandState_Lasso) gl::color(255, 255, 0);
				else if (body.getHandRightState() == HandState_Open) gl::color(0, 255, 0);
				else  gl::color(ColorAf::white());
			}
			else  gl::color(ColorAf::white());
			gl::drawSolidCircle(pos, 7.0f, 32);
			gl::color(Kinect2::getBodyColor(body.getIndex()));
			gl::drawSolidCircle(pos, 5.0f, 32);
		}
		*/
		gl::popMatrices();
	}
}

CINDER_APP_BASIC(testKinect2App, RendererGl)
