#include "HUDBase.h"

using namespace OUAN;

HUDBase::HUDBase() : mOverlay(NULL)
{

}

HUDBase::~HUDBase()
{
	destroy();
}

void HUDBase::init(const std::string& overlayName)
{
	mOverlayName=overlayName;
	mOverlay=Ogre::OverlayManager::getSingleton().getByName(overlayName);

	if (!mOverlay)
	{
		Logger::getInstance()->log("[HUDBase::showOverlay] Error loading "+mOverlayName+" Overlay!");
	}
}

void HUDBase::destroy()
{
	//Ogre will handle the overlay's deletion, 
	//so we'll just set the pointer to NULL
	mOverlay=NULL; 
	mOverlayName.clear();
}

void HUDBase::show()
{
	mOverlay->show();
}

void HUDBase::hide()
{
	mOverlay->hide();
}

void HUDBase::showElement(const std::string& elementName)
{	
	Ogre::OverlayElement* element=Ogre::OverlayManager::getSingletonPtr()->getOverlayElement(elementName);
	if (element)
	{
		element->show();
	}
	else
	{
		Logger::getInstance()->log("[HUDBase::showElement] Error loading "+elementName+" Overlay element!");
	}
}

void HUDBase::hideElement(const std::string& elementName)
{
	Ogre::OverlayElement* element=Ogre::OverlayManager::getSingletonPtr()->getOverlayElement(elementName);
	if (element)
	{
		element->hide();
	}
	else
	{
		Logger::getInstance()->log("[HUDBase::hideElement] Error loading "+elementName+" Overlay element!");
	}
}