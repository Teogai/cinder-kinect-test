
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Texture.h"
#include "cinder/params/Params.h"

#include "Kinect2.h"

using namespace Kinect2;
class BasicApp : public ci::app::AppBasic 
{
public:
	void						draw();
	void						prepareSettings( ci::app::AppBasic::Settings* settings );
	void						setup();
	void						update();
	void						getHands();
	void						drawHands();

private:
	Kinect2::DeviceRef			mDevice;
	Kinect2::Frame				mFrame;

	float						mFrameRate;
	bool						mFullScreen;
	ci::params::InterfaceGlRef	mParams;

	Kinect2::Body::Joint handRight, thumbRight, handLeft, thumbLeft;

};

#include "cinder/Font.h"

using namespace ci;
using namespace ci::app;
using namespace std;

void BasicApp::draw()
{
	gl::setViewport( getWindowBounds() );
	gl::clear();
	gl::setMatricesWindow( getWindowSize() );
	gl::enableAlphaBlending();
	/*
	if (mFrame.getColor()) {
		gl::TextureRef tex = gl::Texture::create(mFrame.getColor());
		gl::draw(tex, tex->getBounds(), Rectf(Vec2f::zero(), getWindowSize()));
		drawHands();
	}
	*/
	if (mFrame.getBodyIndex()) {
		gl::TextureRef tex = gl::Texture::create(Kinect2::colorizeBodyIndex(mFrame.getBodyIndex()));
		gl::draw(tex, tex->getBounds(), Rectf(Vec2f::zero(), Vec2f(getWindowSize())));
	}
	
	//if (){
		drawHands();
	//}
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

void BasicApp::prepareSettings( Settings* settings )
{
	settings->prepareWindow( Window::Format().size( 1280, 720 ).title( "Basic App" ) );
	settings->setFrameRate( 60.0f );
}

void BasicApp::setup()
{	
	gl::enable( GL_TEXTURE_2D );
	
	mFrameRate	= 0.0f;
	mFullScreen	= false;

	mDevice = Kinect2::Device::create();
	mDevice->start( Kinect2::DeviceOptions().enableInfrared().enableBodyIndex() );

	mParams = params::InterfaceGl::create( "Params", Vec2i( 200, 100 ) );
	mParams->addParam( "Frame rate",	&mFrameRate,				"", true );
	mParams->addParam( "Full screen",	&mFullScreen,				"key=f" );
	mParams->addButton( "Quit", bind(	&BasicApp::quit, this ),	"key=q" );

}

void BasicApp::update()
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

CINDER_APP_BASIC( BasicApp, RendererGl )
	
void BasicApp::getHands(){
	if (mFrame.getBodyIndex())
	{
		vector<Body> vBodies = mFrame.getBodies();
		console() << vBodies.size() << std::endl;
		for (size_t i = 0; i < vBodies.size();i++){
			Body body = vBodies[i];
			if (body.isTracked())
			{
				// Find the joints
				handRight = body.getJointMap().at(JointType_HandRight);
				thumbRight = body.getJointMap().at(JointType_ThumbRight);

				handLeft = body.getJointMap().at(JointType_HandLeft);
				thumbLeft = body.getJointMap().at(JointType_ThumbLeft);
			}
		}
	}
}


void BasicApp::drawHands(){
	gl::color(255, 255, 255);
	gl::drawSolidCircle(Vec2f(handRight.getPosition().x, handRight.getPosition().y), 10);
}

