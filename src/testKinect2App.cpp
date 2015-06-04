
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/params/Params.h"

#include "Kinect2.h"

using namespace Kinect2;

class testKinect2App : public ci::app::AppBasic 
{
public:
	void						draw();
	void						prepareSettings( ci::app::AppBasic::Settings* settings );
	void						setup();
	void						update();
	void						getHands();
	void						drawHands();

	std::vector<Kinect2::Body::Joint> 			handRight;
	std::vector<Kinect2::Body::Joint> 			thumbRight;
	std::vector<Kinect2::Body::Joint> 			handLeft;
	std::vector<Kinect2::Body::Joint> 			thumbLeft;

private:
	Kinect2::DeviceRef			mDevice;
	Kinect2::Frame				mFrame;

	float						mFrameRate;
	bool						mFullScreen;
	ci::params::InterfaceGlRef	mParams;
};

#include "cinder/Font.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void testKinect2App::draw()
{
	gl::setViewport( getWindowBounds() );
	gl::clear();
	gl::setMatricesWindow( getWindowSize() );
	gl::enableAlphaBlending();

	if (mFrame.getColor()) {
		gl::TextureRef tex = gl::Texture::create(mFrame.getColor());
		gl::draw(tex, tex->getBounds(), Rectf(Vec2f::zero(), getWindowSize()));
	}
	drawHands();

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

	getHands();
}
	
void testKinect2App::getHands(){
	handRight.clear();
	thumbRight.clear();
	handLeft.clear();
	thumbLeft.clear();

	mFrame = mDevice->getFrame();
	vector<Body> vBodies = mFrame.getBodies();
	for (size_t i = 0; i < vBodies.size();i++){
		Body body = vBodies[i];
		if (body.isTracked())
		{
			// Find the joints
			handRight.push_back(body.getJointMap().at(JointType_HandRight));
			thumbRight.push_back(body.getJointMap().at(JointType_ThumbRight));

			handLeft.push_back(body.getJointMap().at(JointType_HandLeft));
			thumbLeft.push_back(body.getJointMap().at(JointType_ThumbLeft));
		}
	}
}


void testKinect2App::drawHands(){
	gl::color(255, 255, 255);
	for (size_t i = 0; i < handRight.size(); i++){
		Vec3f pos = handRight[i].getPosition();
		Vec2f out = Vec2f(pos.x, -pos.y) * Vec2f(getWindowSize()) + Vec2f(getWindowCenter());
		gl::drawStrokedCircle(out, 20);
	}
	for (size_t i = 0; i < thumbRight.size(); i++){
		Vec3f pos = thumbRight[i].getPosition();
		Vec2f out = Vec2f(pos.x, -pos.y) * Vec2f(getWindowSize()) + Vec2f(getWindowCenter());
		gl::drawStrokedCircle(out, 20);
	}
	for (size_t i = 0; i < handLeft.size(); i++){
		Vec3f pos = handLeft[i].getPosition();
		Vec2f out = Vec2f(pos.x, -pos.y) * Vec2f(getWindowSize()) + Vec2f(getWindowCenter());
		gl::drawStrokedCircle(out, 20);
	}
	for (size_t i = 0; i < thumbLeft.size(); i++){
		Vec3f pos = thumbLeft[i].getPosition();
		Vec2f out = Vec2f(pos.x, -pos.y) * Vec2f(getWindowSize()) + Vec2f(getWindowCenter());
		gl::drawStrokedCircle(out, 20);
	}
}

CINDER_APP_BASIC(testKinect2App, RendererGl)
