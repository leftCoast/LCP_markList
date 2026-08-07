
#include <markList.h>
#include <fontLabel.h>
#include <globalPos.h>
#include <strTools.h>
#include <rectArrange.h>


#define LIST_X		10								// Little border?
#define LIST_Y		40								// Give it some room
#define LIST_W 	320 - LIST_X - LIST_X	// Should center it.

#define LOBJ_H		60
#define NUM_ITEMS	6
#define LIST_H		LOBJ_H * NUM_ITEMS
#define MAX_CHARS	27								// Any more and it overwrites.

rect markObjRect(0,0,LIST_W,LOBJ_H);
rect markListRect(LIST_X,LIST_Y,LIST_W,LIST_H);

colorObj*	focusBackColor;
colorObj*	dispBackColor;
colorObj*	mainTextColor;
colorObj*	subTextColor;	

#define SELECT_ICON	"select.bmp"
		
#define APP_ICON_H	40
#define APP_ICON_Y	4

navMark	selectedMark;
bool		haveMarkSet = false;

// *****************************************************
// ******************   markListObj   ******************
// *****************************************************


markListObj::markListObj(scrollingList* inList)
	: navMark(),
	drawGroup(&markObjRect) {
	
	nameLbl		= NULL;
	distLbl		= NULL;
	bearingLbl	= NULL;
	latLbl		= NULL;
	lonLbl		= NULL;
	ourlist		= inList;
	setEventSet(touchNDrag);		// We want the fancy event set.
	scrolling = false;				// Ain't scrolling yet.
	setup();
}	
	

markListObj::~markListObj(void) {  }


void markListObj::setup(void) {
	

	/*
	distLbl = new erasableText();
	if (distLbl) {
		distLbl->setFont(AFF_MONO_9);
		addObj(lonLbl);
	}
	
	bearingLbl = new erasableText();
	if (bearingLbl) {
		bearingLbl->setFont(AFF_MONO_9);
		addObj(lonLbl);
	}
	*/
	nameLbl = new erasableText(0,0,LIST_W,17);
	if (nameLbl) {
		nameLbl->setFont(AFF_MONO_9);
		addObj(nameLbl);
	}
	
	latLbl = new erasableText(0,18,LIST_W,17);
	if (latLbl) {
		latLbl->setFont(AFF_MONO_9);
		addObj(latLbl);
	}
	
	lonLbl = new erasableText(0,36,LIST_W,17);
	if (lonLbl) {
		lonLbl->setFont(AFF_MONO_9);
		addObj(lonLbl);
	}
	setLblColors();
}


void markListObj::setName(const char* inName) {
	
	char*	shortName;
	
	if (inName) {
		navMark::setName(inName);
		if (strlen(inName)>MAX_CHARS) {
			shortName = NULL;
			heapStr(&shortName,inName);
			shortName[MAX_CHARS]='\0';
			nameLbl->setValue(shortName);
			Serial.println(nameLbl->width);
			freeStr(&shortName);
		} else {
			nameLbl->setValue(inName);
		}
	}
}


void markListObj::setPos(gPosPack* inPos) {
	
	navMark::setPos(inPos);
	setLatLonLbls();
}
	
	
// Set the labels to show what we're holding.
void markListObj::setLatLonLbls(void) {

	char			outStr[40];
	char			qStr[4];
	double		value;
	
	strcpy(qStr," N");
	if (latLon.latQuad==south) {
		strcpy(qStr," S");
	}
	value = latLon.latDeg + (latLon.latMin/60.0);
	sprintf (outStr,"%s%10f%s","Lat: ",value,qStr);
	latLbl->setValue(outStr);
	
	strcpy(qStr," W");
	if (latLon.lonQuad==east) {
		strcpy(qStr," E");
	}
	value = latLon.lonDeg + (latLon.lonMin/60.0);
	sprintf (outStr,"%s%10f%s","Lon: ",value,qStr);
	lonLbl->setValue(outStr);
}


// selecting and dragging things.
void markListObj::doAction(event* inEvent,point* localPt) {

	point		listPt;
	
	if (inEvent->mType==touchEvent) {
		if (haveFocus()) {
			setFocusPtr(NULL);
		} else {
			setFocusPtr(this);
		}
	} else if (inEvent->mType==dragBegin) {
		setFocusPtr(NULL);
		scrolling = true;
		listPt.x = localPt->x - x;
		listPt.y = localPt->y - y;
		ourlist->doAction(inEvent,&listPt);
	}  else if (inEvent->mType==dragOn) {							// Or else, we're actually dragging..
		if (scrolling) {
			listPt.x = localPt->x - x;
			listPt.y = localPt->y - y;
			ourlist->doAction(inEvent,&listPt);
		}
	} else if (inEvent->mType==liftEvent) {							// Or else, we're finished dragging..
	 	if (scrolling) {
	 		listPt.x = localPt->x - x;
			listPt.y = localPt->y - y;
			ourlist->doAction(inEvent,&listPt);
			scrolling = false;
		}
	}
} 	


// Focus is changing, make these look right.
void markListObj::setLblColors(void) {

	if (haveFocus()) {
		nameLbl->setColors(mainTextColor,focusBackColor);
		//distLbl->setColors(subTextColor,focusBackColor);
		//bearing->setColors(subTextColor,focusBackColor);
		latLbl->setColors(subTextColor,focusBackColor);
		lonLbl->setColors(subTextColor,focusBackColor);
	} else {
		nameLbl->setColors(mainTextColor,dispBackColor);
		//distLbl->setColors(subTextColor,dispBackColor);
		//bearing->setColors(subTextColor,dispBackColor);
		latLbl->setColors(subTextColor,dispBackColor);
		lonLbl->setColors(subTextColor,dispBackColor);
	}
}


// This is called when OUR focus changes. Usually used to cause a redraw to show the
// change. But in this case, ourApp needs to know who's selected, because it holds the
// function icons. How to set them, who to act on when clicked. It needs to track this.
void  markListObj::setThisFocus(bool setLoose) {
	
	markList*	ourApp;
	
	ourApp = (markList*)ourPanel;		// We are a panel, therefor ourApp is our panel.
	drawObj::setThisFocus(setLoose);	// Do the normal stuff.. ('Causes subsequent redraw)
	if (setLoose) {						// 
		ourApp->selected(this);
	} else {
		ourApp->selected(NULL); 
	}
	setLblColors();
}


void  markListObj::setMark(void) {
	
	selectedMark = *this;
	haveMarkSet = true;
}


// We don't want to draw stuff where it ain't wanted.
void markListObj::draw(void) {

	if (ourlist->isVisible(this)) {
		drawGroup::draw();
		
	}
	needRefresh = false;					  // But in all cases. We no longer need to be drawn.		
}


void markListObj::drawSelf(void) {

	rect			aFrame(this);
	
	aFrame.insetRect(1);
	if (haveFocus()) {
		screen->fillRect(&aFrame,focusBackColor);
	} else {
		screen->fillRect(&aFrame,dispBackColor);
	}
	//screen->drawRect(this,&red);
}
	
	
				
// *****************************************************
// *****************    markListList   *****************
// *****************************************************


markListList::markListList(rect* inRect)
	: scrollingList(inRect) {
	
	focusBackColor = new colorObj(LC_CHARCOAL);
	dispBackColor = new colorObj(LC_BLACK);
	mainTextColor = new colorObj(LC_YELLOW);
	subTextColor = new colorObj(LC_YELLOW);
	subTextColor->blend(&black,50);
}


markListList::~markListList(void) {

	if (focusBackColor) {
		delete(focusBackColor);
		focusBackColor = NULL;
	}
	if (dispBackColor) {
		delete(dispBackColor);
		dispBackColor = NULL;
	}
	if (mainTextColor) {
		delete(mainTextColor);
		mainTextColor = NULL;
	}
	if (subTextColor) {
		delete(subTextColor);
		subTextColor = NULL;
	} 
}


void markListList::addItem(const char* inName,gPosPack* inGPos) {
	
	markListObj*	newObj;
	
	newObj = new markListObj(this);
	if (newObj) {
		newObj->setName(inName);
		newObj->setPos(inGPos);
		addObj(newObj);
	}
}


void markListList::setup(void) {

}
	
	
void markListList::drawSelf(void) { 

	//screen->drawRect(this,&white);
}	
	
	
	
// *****************************************************
// ******************     markList    ******************
// *****************************************************


markList::markList(int ourAppID)
	: panel(ourAppID) {
	
	selectedItem = NULL;
}


markList::~markList(void) {  }
		


void markList::setup(void) {

	gPosPack		aGPos;
	posParser	ourParser;
	rect			iconBar;
	iconArrange	spreader;
	int			defX;
	int			defY;
	rect			aFrame(0,0,32,32);
	
	iconBar.x = 0;											// Set at the left edge.
	iconBar.y = screen->height() - APP_ICON_H;	// Set to display bottom - height of icon.
	iconBar.width = screen->width();					// Set to width of the display.
	iconBar.height = APP_ICON_H;						// Set to height of icon.
	spreader.settings(&iconBar,10,20);				// Setup the spreader's limits.
	
	defX = 0;	// It no longer really matters where we initially put the icons.
	defY = 0;	// The spreader will arrange them for us.
	
	rect			listRect(LIST_X,LIST_Y,LIST_W,LIST_H);
	ourList = new markListList(&listRect);
	if (ourList) {
		addObj(ourList);
		
		aGPos = ourParser.parsePos("48.46597° N","122.67584° W");
		ourList->addItem("Skyline entrance",&aGPos);
		
		aGPos = ourParser.parsePos("48.48909° N","122.71192° W");
		ourList->addItem("Outside Burrows pass",&aGPos);
		
		aGPos = ourParser.parsePos("48.52678° N,","122.78071° W");
		ourList->addItem("Outside thatcher pass",&aGPos);
		
		aGPos = ourParser.parsePos("48.52938° N","122.81805° W");
		ourList->addItem("Inside thatcher pass",&aGPos);
		
		aGPos = ourParser.parsePos("48.57866° N","122.88653° W");
		ourList->addItem("Top of Upright channel",&aGPos);
		
		aGPos = ourParser.parsePos("48.54209° N","122.93782° W");
		ourList->addItem("Bottom of Upright channel",&aGPos);
		
		aGPos = ourParser.parsePos("48.54408° N","123.00436° W");
		ourList->addItem("Outside Friday harbor",&aGPos);
		
		aGPos = ourParser.parsePos("48.54395° N", "122.84553° W");
		ourList->addItem("Northern tip of frost island",&aGPos);
	}
	
	aFrame.setRect(defX++,defY++,32,32);
	setFilePath(SELECT_ICON);
	setMarkBtn = new selectBtn(defX++,defY++,mFilePath,this);
	addObj(setMarkBtn);
	spreader.addRect(setMarkBtn);
}

	
void markList::setMark(void) {

	if (selectedItem) {
		selectedItem->setMark();
	}
}

			
void markList::selected(markListObj* itemPtr) { selectedItem = itemPtr; };


void markList::drawSelf(void) { screen->fillScreen(&black); }
		
				
void markList::loop(void) {


}



// *****************************************************
// ******************    selectBtn    ******************
// *****************************************************


selectBtn::selectBtn(int xLoc,int yLoc,const char* iconPath,markList* inApp)
	: iconButton(xLoc,yLoc,iconPath) { ourLink = inApp; }
	
selectBtn::~selectBtn(void) {  }
	
	
void selectBtn::doAction(event* inEvent,point* localPt) { 

	if (ourLink) {
		ourLink->setMark();
	}		
}
				

	
	
	
	

