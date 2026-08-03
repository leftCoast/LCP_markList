
#include <markList.h>
#include <fontLabel.h>
#include <globalPos.h>


#define LIST_X		10								// Little border?
#define LIST_Y		40								// Give it some room
#define LIST_W 	320 - LIST_X - LIST_X	// Should center it.

#define LOBJ_H		18
#define NUM_ITEMS	24
#define LIST_H		LOBJ_H * NUM_ITEMS


rect markObjRect(0,0,LIST_W,LOBJ_H);
rect markListRect(LIST_X,LIST_Y,LIST_W,LIST_H);


// *****************************************************
// ******************   markListObj   ******************
// *****************************************************


markListObj::markListObj(void)
	: drawGroup(&markListRect) {
	
	markName		= NULL;
	markDist		= NULL;
	markBearing	= NULL;
	markLat		= NULL;
	markLon		= NULL;
	setup();
}


markListObj::~markListObj(void) {  }


void markListObj::setup(void) {

	markName = new fontLabel(0,0,LIST_W,17);
	if (markName) {
		markName->setFont(AFF_MONO_9);
		markName->setColors(&yellow);
		addObj(markName);
	}
	
	/*
	markDist = new fontLabel();
	if (markDist) {
		markDist->setFont(AFF_MONO_9);
	}
	
	markBearing = new fontLabel();
	if (markBearing) {
		markBearing->setFont(AFF_MONO_9);
	}
	*/
	
	markLat = new fontLabel(0,18,LIST_W,17);
	if (markLat) {
		markLat->setFont(AFF_MONO_9);
		markLat->setColors(&yellow);
		addObj(markLat);
	}
	
	markLon = new fontLabel(0,36,LIST_W,17);
	if (markLon) {
		markLon->setFont(AFF_MONO_9);
		markLon->setColors(&yellow);
		addObj(markLon);
	}
}


void markListObj::setValues(const char* inName,double inLat,double inLon) {

	globalPos aMark;
	
	aMark.setLat(inLat);
	aMark.setLon(inLon);
	if (markName) {
		markName->setValue(inName);
	}
	if (markLat) {
		markLat->setValue(aMark.showLatStr());
	}
	if (markLon) {
		markLon->setValue(aMark.showLonStr());
	}
}


void markListObj::drawSelf(void) {

	//screen->drawRect(this,&green);
}
	
	
				
// *****************************************************
// *****************    markListList   *****************
// *****************************************************


markListList::markListList(void)
	: drawList(&markListRect) {  }


markListList::~markListList(void) {  }


void markListList::addItem(const char* inName,double inLat,double inLon) {
	
	markListObj*	newObj;
	
	newObj = new markListObj;
	if (newObj) {
		newObj->setValues(inName,inLat,inLon);
		addObj(newObj);
	}
}


void markListList::setup(void) {

}
	
	
void markListList::drawSelf(void) {  }	
	
	
	
// *****************************************************
// ******************     markList    ******************
// *****************************************************


markList::markList(int ourAppID)
	: panel(ourAppID) { }


markList::~markList(void) {  }
		


void markList::setup(void) {

	globalPos aMark;
	
	markListList*	ourList = new markListList;
	if (ourList) {
		aMark.setPosValues("48.46597° N","122.67584° W");
		Serial.println(aMark.lonDeg);
		ourList->addItem("Skyline entrance",aMark.getLatAsDbl(),aMark.getLonAsDbl());
		addObj(ourList);
	}
}
				
				
void markList::loop(void) {


}

void markList::drawSelf(void) { screen->fillScreen(&black); }
