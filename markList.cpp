
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
	markID		= 0;
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
	
	
void markListObj::setMarkID(uint32_t ID) { markID = ID; }


uint32_t markListObj::getMarkID(void) { return markID; }
			
			
				
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


void markListList::addItem(markListObj* newObj) { if (newObj) addObj(newObj); }


void markListList::setup(void) {

}
	
	
void markListList::drawSelf(void) { 

	//screen->drawRect(this,&white);
}	
	
	
	
// **********************************************************************
// IDList
// **********************************************************************



IDList::IDList(blockFile* aBLockfile) {

	IDBuff = NULL;
	ourBlockFile = aBLockfile;
}


IDList::~IDList(void) { resizeBuff(0,(void**)&IDBuff); }


// Finds and returns the size of the Root block, in bytes.
unsigned long IDList::getNumRootBytes(void) {

	unsigned long	rootID;
	
	rootID = ourBlockFile->readRootBlockID();			// Grab root ID.
	if (rootID) {												// We got one?
		return ourBlockFile->getBlockSize(rootID);	// return the num bytes.
	}																//
	return 0;													// Guess we didn't have one.
}


// Calculates and returns the number of items we're storing.	
int IDList::getNumItems(void) {
	
	unsigned long	numBytes;
	
	numBytes = getNumRootBytes();						// get the number of bytes..
	if (numBytes) {										// If we have some..
		return numBytes/sizeof(unsigned long);		// Calculate and return the number of items.
	}															//
	return 0;												// Or none..
}


// Allocates and populates our IDBuff of item IDs.
bool IDList::getList(bool plusOne) {
	
	unsigned long	rootID;
	unsigned long	numBytes;
	unsigned long	allocate;
	bool				success;
	
	success = false;																		// We ain't a success yet.
	numBytes = getNumRootBytes();														// Get the number of items we have.
	allocate = numBytes;																	// Allocate set to numBytes. We need both.
	if (plusOne) {																			// If plus one..
		allocate = allocate + sizeof(unsigned long);								// Bump up allocate. Bigger buffer than needed.
	}																							//
	if (resizeBuff(allocate,(void**)&IDBuff)) {									// Resize the IDBuff to the allocation byes value.
		rootID = ourBlockFile->readRootBlockID();									// Grab root ID.
		success = ourBlockFile->getBlock(rootID,(uint8_t*)IDBuff,numBytes);	// Have a go at grabbing the buffer.
		success = success || plusOne;													// If first one? Then it's ok.
		if (!success) {																	//	If we failed..
			resizeBuff(0,(void**)&IDBuff);											// Recycle the buffer we allocated.
		}																						//
	}																							//
	return success;																		// Return our result.
}


// Allocates and ID number, adds this new value to the rootID block
// nd saves the block to file.
unsigned long IDList::addItem(void) {
	
	unsigned long 	newID;
	unsigned long 	rootID;
	unsigned long	numBytes;
	int				numItems;
	unsigned long 	result;
	
	result = 0;																				// A new ID can not be zero.
	if (ourBlockFile->isEmpty()) {													// If it's an empty file..
		rootID = ourBlockFile->getNewBlockID();									// First value is rootID.
	} else {																					// Oh, we got a file with stuff?
		rootID = ourBlockFile->readRootBlockID();									// Just in case..
	}																							//
	numItems = 	getNumItems();															// Get an item count.
	if (getList(true)) {																	// If we can get our list + 1..
		newID = ourBlockFile->getNewBlockID();										// Create the newID.
		IDBuff[numItems] = newID;														// Poke the new ID into the end.
		numItems++;																			// Bump up the number of items.
		numBytes = numItems * sizeof(unsigned long);								// Update the numbBytes value.
		if (ourBlockFile->writeBlock(rootID,(uint8_t*)IDBuff,numBytes)) {		// If we can updating the file.
			result = newID;																// Everything worked? Set result.
		}																						//
		resizeBuff(0,(void**)&IDBuff);												// Recycle the buffer we allocated.
	}																							//
	return result;																			// Return our result.
}


// If this can locate the delivered ID, it will delete it from our
// rootID block and save the change. Make sure to delette the actual
// data block in question first. The does NOT do that.
bool IDList::removeItem(unsigned long oldID) {
	
	unsigned long	rootID;
	int				numItems;
	unsigned long	numBytes;
	bool				foundIt;
	
	foundIt = false;																	// Ain't found anything yet.
	numItems = getNumItems();														// Grab number of items.
	if (numItems) {																	// If we got items.
		if (getList()) {																// And we got the list.
			for(int i=0;i<numItems;i++) {											// For every item..
				if (IDBuff[i]==oldID) {												// If we found it..
					foundIt = true;													// Note it.
				}																			// pop
				if (foundIt) {															// Again, if we found it..
					IDBuff[i] = IDBuff[i+1];										// copy the next item into this one.
				}																			// The last will get gibberish, but don't worry.
			}																				// It's going to be chopped off.
			if (foundIt) {																// If we found it..
				rootID = ourBlockFile->readRootBlockID();						// Grab root ID.
				numBytes = (numItems-1) * sizeof(unsigned long);			// Update the numbBytes value.
				ourBlockFile->writeBlock(rootID,(uint8_t*)IDBuff,numBytes);	// Stuff back the buffer.
			}																				// pop
		}																					// pop
	}																						// pop
	return foundIt;																	// Let 'em know if we found one.
}



// **********************************************************************
// itemMgr - The interface for the App.
// **********************************************************************



itemMgr::itemMgr(blockFile* aBlockFile)
	: IDList(aBlockFile) { ourBlockFile = aBlockFile; }
	
	
itemMgr::~itemMgr(void) { }


// Not yet.
void itemMgr::sortList(void) {  }



// We first read out how many items we should have data for. Meaning, how many data IDs we
// have. Then we loop through each one. Creating a markListObj, assigning an ID to it from
// the list. Then seeing if we can read that IDs data into that object. If so, we add it to
// the list on the screen. If not we recycle the object and move on.
void itemMgr::populateList(void) {
	
	markList*		ourApp;
	int				numMarks;
	markListObj*	newMark;
	
	ourApp = (markList*)ourPanel;								// Need pointer to our application.
	numMarks = getNumItems();									// How many items we got here?
	if (numMarks) {												// We got items?
		for(int i=0;i<numMarks;i++) {							// For each one
			newMark = new markListObj(ourApp->ourList);	// Create a mark.
			if (newMark) {											// Got one?
				newMark->setMarkID(IDBuff[i]);				// Set in this new ID.
				if (readItem(newMark)) {						// Go see if we can read it out.
					ourApp->addMark(newMark);					// Got it? Add it to the list.
				} else {												// Else we can't read out that one?
					delete(newMark);								// Didn't work, we scrap it.
				}														//
			}															//
		}																//
		sortList();													// All filled. Let's get them sorted.
	}																	//
}
				


// We are given a name and position. With this we create a new markListObj. Stuff it with
// it's name and position. Then create a file ID for it and using this, save it to disk.
// Finally we add it to the list it will appear on.
void itemMgr::addNewItem(const char* inName,gPosPack* inPos) {
	
	markList*		ourApp;
	unsigned long	newID;
	markListObj*	newItem;
	
	if (inName && inPos) {									// We got a name and position?
		ourApp = (markList*)ourPanel;						// Need pointer to our application.
		newItem = new markListObj(ourApp->ourList);	// Create the item..
		if (newItem) {											// Got it?
			newItem->setName(inName);						// Here's the name.
			newItem->setPos(inPos);							// And your position.
			newID = addItem();								// Add item to root list.
			if (newID) {										// Non zero ID means success.
				newItem->setMarkID(newID);					// Set in it's new ID.
				saveItem(newItem);							// Save this to disk.
				ourApp->addMark(newItem);					// Time to send it onto the list.
				sortList();										// If sorting, good time for that?
			}														// *pop*
		}															// *pop*
	}																// *pop*
}


// This pulls the item out of the list it was displayed in. Patches the list. Then pulls
// it out of the file it was saved it. Finally it deletes the object itself.
void itemMgr::deleteItem(markListObj* oldView) {

	markList*	ourApp;
	
	if (oldView) {														// Sanity, make sure we have one.
		ourApp = (markList*)ourPanel;								// Need pointer to our application.
		setFocusPtr(NULL);											// Remove it from focus.
		oldView->unhook();											// Pull the poor thing from it's list.
		ourApp->ourList->setPositions();							// Close up the gap.
		ourApp->ourList->setNeedRefresh();						// Redraw everything without it.
		removeItem(oldView->getMarkID());						// Pull it's ID from the root list.
		ourBlockFile->deleteBlock(oldView->getMarkID());	// Pull it's data block.
		delete(oldView);												// Recycle it.
	}																		// Man! Talk about "Erased from existence!"
}


// We have a markListObj with an item ID already set. This attempts to read that item's
// data into it. It returns if it was successful or not.
bool itemMgr::readItem(markListObj* aMark) {

	uint8_t*	buff;
	uint32_t	numBytes;
	bool		success;
	
	success = false;																					// Well, not yet..
	if (aMark) {																						// Behave, no writing to NULL.
		if (aMark->getMarkID()) {																		// Got an ID?
			numBytes = ourBlockFile->getBlockSize(aMark->getMarkID());						// get the size buffer we'll need.
			if (resizeBuff(numBytes,&buff)) {													// If we can get the bytes																	// We have an id?
				success = ourBlockFile->getBlock(aMark->getMarkID(),buff,numBytes);		// grab the buffer.
				if (success) {																			// We got it?
					aMark->readBuff(buff);															// Stuff the data into the mark.
				}																							//
				resizeBuff(0,&buff);																	// Recycle the buffer.
			}																								//
		}																									//
	}																										//
	return success;																					// return our result.
}
				

// We have a complete markListObj with name, position and ID. This will save this item to
// the file. It doen't tell us if this worked or not.		
void itemMgr::saveItem(markListObj* aMark) {
	
	uint8_t*	buff;
	uint32_t	numBytes;
	
	if (aMark) {																				// We got an item?
		if (aMark->getMarkID() && aMark->numBytes()) {							// It has bytes & an ID?
			buff = NULL;																		// Ok NULL out the buff ptr.
			numBytes = aMark->numBytes();												// Grab the size we need.
			if (resizeBuff(numBytes,&buff)) {											// If we can allocate the buffer..
				aMark->fillBuff(buff);														// Fill it with the data.
				ourBlockFile->writeBlock(aMark->getMarkID(),buff,numBytes);	// Save it off.
				resizeBuff(0,&buff);															// Cleanup our mess.
			}																						//
		}																							//
	}																								//
}


// Kinda' a typing aid? I copied all this stuff from shopList app. They were so close.
void itemMgr::saveSelected(markListObj* selected) { saveItem(selected); }
		


	
// *****************************************************
// ******************     markList    ******************
// *****************************************************


markList::markList(int ourAppID)
	: panel(ourAppID) {
	
	bool	success;
	
	success = false;											// We're girding on our sword.
	ourAddItemDBox		= NULL;								// Auto delete.
	ourEditItemDBox	= NULL;								// Auto delete.
	readmeBtn			= NULL;								// Auto delete.
	checkClear			= NULL;								// Auto delete.
	checkDelete			= NULL;								// Auto delete.
	ourBlockFile		= NULL;								// We delete.
	ourItemMgr			= NULL;								// We delete.
	selectedItem		= NULL;								// No allocation.
	if (setFilePath("markFile")) {						// We want this name in our folder.
		ourBlockFile = new blockFile(mFilePath);		// Have a go at creating the block file.
		if (ourBlockFile) {									// Got the block file?
			ourItemMgr = new itemMgr(ourBlockFile);	// Create the item manager.
			if (ourItemMgr) {									// Got that last item?
				success = true;								// We'll call this good.
			}														//
		}															//
	}																//
	if (!success) close();									// If we can't assemble? Close shop now.
}


markList::~markList(void) {

	if (ourBlockFile) delete ourBlockFile;
	if (ourItemMgr) delete ourItemMgr;
}
		


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
		
		aGPos = ourParser.parsePos("48.48867° N,","122.67742° W");
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
	
	addItem = newStdBtn(defX++,defY++,icon32,newItemCmd,this);
	addObj(addItem);
	spreader.addRect(addItem);
	
	editItem = newStdBtn(defX++,defY++,icon32,editCmd,this);
	editItem->setActive(false);
	addObj(editItem);
	spreader.addRect(editItem);
	
	delItem = newStdBtn(defX++,defY++,icon32,deleteItemCmd,this);
	delItem->setActive(false);
	addObj(delItem);
	spreader.addRect(delItem);
	
	readmeBtn = newStdBtn(defX++,defY++,icon32,readmeCmd,this);
   addObj(readmeBtn);
   spreader.addRect(readmeBtn);
   
   setItemIcons(true,true,true,true,true);
}


void markList::setItemIcons(bool selectActive,bool addActive,bool editActive,bool delActive,bool readmeActive) {

		setMarkBtn->setActive(selectActive);
		addItem->setActive(addActive);
		editItem->setActive(editActive);
		delItem->setActive(delActive);
		readmeBtn->setActive(readmeActive);
}

	
void markList::setMark(void) {

	if (selectedItem) {
		selectedItem->setMark();
		close();
	}
}

			
void markList::selected(markListObj* itemPtr) {

	selectedItem = itemPtr;
	if (selectedItem) {
		setItemIcons(true,true,true,true,true);
	} else {
		setItemIcons(false,true,false,false,true);
	}
};


void markList::drawSelf(void) { screen->fillScreen(&black); }
		
		
void markList::addMark(markListObj* newObj) { ourList->addItem(newObj); }


void markList::handleCom(stdComs comID) {
	
	gPosPack	aPos;
	
	switch(comID) {
		case newItemCmd		:
			if (ourAddItemDBox) {
				aPos = ourAddItemDBox->getPos();
				ourItemMgr->addNewItem(ourAddItemDBox->getName(),&aPos);
			} else {
				ourAddItemDBox = new addItemDBox(this,sType320x480);
				setItemIcons(false,false,false,false,false);
			}
		break;
		case deleteItemCmd	:
			checkDelete = new deleteOkAlert(this);
		break;
		case editCmd			:
			if (selectedItem) {
				ourEditItemDBox = new editItemDBox(this,selectedItem->getName(),sType320x480);
				if (ourEditItemDBox) {
					setItemIcons(false,false,false,false,false);
				}
			}
		break;	
		case okCmd 				:
			if (ourAddItemDBox) {
				aPos = ourAddItemDBox->getPos();
				ourItemMgr->addNewItem(ourAddItemDBox->getName(),&aPos);
				setItemIcons(false,true,false,false,false);
				ourAddItemDBox = NULL;
			} else if (ourEditItemDBox){
				selectedItem->setName(ourEditItemDBox->getName());
				aPos = ourEditItemDBox->getPos();
				selectedItem->setPos(&aPos);
				ourItemMgr->saveSelected(selectedItem);
				setItemIcons(true,true,true,true,true);
				ourEditItemDBox = NULL;
			} else if (checkDelete) {
				checkDelete = NULL;
				ourItemMgr->deleteItem(selectedItem);
				setItemIcons(false,true,false,false,true);
			} else if (ourReadmeDBox) {	
				ourReadmeDBox = NULL;
			 	setItemIcons(false,true,false,false,true);
			}
		break;
		case cancelCmd			:
			if (ourAddItemDBox) {
				setItemIcons(false,true,false,false,true);
				ourAddItemDBox = NULL;
			} else if (ourEditItemDBox){
				setItemIcons(true,true,true,true,true);
				ourEditItemDBox = NULL;							
			} else if (checkDelete) {
				setItemIcons(true,true,true,true,true);
				checkDelete = NULL;
			}
		break;
		case readmeCmd			:
			setFocusPtr(NULL);
			setItemIcons(false,false,false,false,false);
			ourReadmeDBox = new readmeDBox(this);
		break;
		default					:
			//Serial.print("Seeing comID ");
			//Serial.println((int)comID);
			panel::handleCom(comID);
		break;
	}
}

				
void markList::loop(void) { }



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
				

	
	
	
	

