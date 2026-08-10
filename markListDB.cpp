#include <markListDB.h>
#include <markList.h>
#include <GPSReader.h>
#include <globalPos.h>
#include <modalKbd.h>

//#include <debug.h>


#define ADD_ITEM_MSG		"Add a new navigation mark."
#define ADD_NAME_MSG		"Untitled"
#define ADD_LAT_MSG		"Latitude"
#define ADD_LON_MSG		"Longitude"
#define EDIT_ITEM_MSG	"Have a go at changing an item."
#define DELETE_OK_MSG	"You want to delete that? Just checkin'.."
#define README_MSG		"Add items with green +.\n\nEdit names using yellow pencil.\n\nDelete Items with red trash."




frame::frame(rect* inRect)
	:drawObj(inRect) {  }


frame::~frame(void) {  }

				
void frame::drawSelf(void) { screen->drawRect(this,&black); }
	
	
	
// **************************************************************
// **********************   readmeDBox   **********************
// **************************************************************


readmeDBox::readmeDBox(listener* inListener)
	: alertObj("default",inListener,noteAlert,true,false) {
	
		int	addedY;
		
		addedY = height * 2;
		height = height + addedY;
		okBtn->y = okBtn->y + addedY;
		theMsg->height = theMsg->height + addedY;
		theMsg->setText(README_MSG);
		theMsg->calculate();
	}
	
	
readmeDBox::~readmeDBox(void) {  }		

	
// **************************************************************
// *********************   deleteOkAlert   **********************
// **************************************************************


deleteOkAlert::deleteOkAlert(listener* inListener)
	: alertObj(DELETE_OK_MSG,inListener,warnAlert,true,true) {  }
	
	
deleteOkAlert::~deleteOkAlert(void) {  }
	

	
// **************************************************************
// ********************   addEditDboxBase    ********************
// **************************************************************


addEditDboxBase::addEditDboxBase(const char* alertMsg,listener* inListener,screenTypes inType)
	: alertObj(alertMsg,inListener,noteAlert,false,false),
	kbdUser(this,inType) {
	
	rect		editRect;
	frame*	editFrame;
	
	setRect(60,100,200,146);										// Set alertBox size and shape.
	theMsg->y = 30;													// Kick the default message over a bit.
	editRect.setRect(10,70,width-20,12);						// Setup location for initial edit box.																				//
	
	nameField = new editLabel(&editRect,"");					// Create it.
	nameField->setLocation(3,3);									// It's position inside it's datafield.
	nameField->setColors(&black,&white);						// Set colors
	nameField->setEventSet(fullClick);							// Events when editing.
	nameD = new datafield(&editRect);							// Create it's datafield.
	nameD->begin(getKbd(),nameField);							// Tell the datafield where the keyboard and it's editLabel are.
	addObj(nameD);														// Hook it up.
	editFrame = new frame(&editRect);							// Using the edit field rect create a frame.
	editFrame->insetRect(-3);										// Expand it by 3 pixels per side.
	nameD->setRect(editFrame);										// Use this to reset the datafield's frame.
	addObj(editFrame);												// Hook it up.	
	
	editRect.y = editRect.y+22;									// Bump down, next line.
	latField = new editLabel(&editRect,"");					// Go through the exact same dance with these next two..
	latField->setLocation(3,3);									// Place it.
	latField->setColors(&black,&white);							// Colors.
	latField->setEventSet(fullClick);							// Events.
	latD = new datafield(&editRect);								// Create.
	latD->begin(getKbd(),latField);								// Assemble.
	addObj(latD);														// Add it to the list.
	editFrame = new frame(&editRect);							// Make a frame.
	editFrame->insetRect(-3);										// Expand it.
	latD->setRect(editFrame);										// Use it.
	addObj(editFrame);												// Add it.
	
	editRect.y = editRect.y+22;									// Next line.
	lonField = new editLabel(&editRect,"");					// Create.
	lonField->setLocation(3,3);									// locate.
	lonField->setColors(&black,&white);							// Colors
	lonField->setEventSet(fullClick);							// Events.
	lonD = new datafield(&editRect);								// Create.
	lonD->begin(getKbd(),lonField);								// Tab A into slot B.
	addObj(lonD);														// Install.
	editFrame = new frame(&editRect);							// Another.
	editFrame->insetRect(-3);										// Expand.
	lonD->setRect(editFrame);										// Use it.
	addObj(editFrame);												// And add it in.
	
	hookup();															// Hook into the idler queue.
}


addEditDboxBase::~addEditDboxBase(void) {  }

	
const char*	addEditDboxBase::getName(void) { return nameField->getTextBuff(); }


gPosPack addEditDboxBase::getPos(void) {

	posParser	aParser;

	return aParser.parsePos(latField->getTextBuff(),lonField->getTextBuff());
}


					
// **************************************************************
// **********************   addItemDBox    **********************
// **************************************************************
	
						
addItemDBox::addItemDBox(listener* inListener,screenTypes inType)
	: addEditDboxBase(ADD_ITEM_MSG,inListener,inType) {
	
	if (ourGPS->valid) {												// If we have a current location..
		latField->setValue(ourGPS->latLon.showLatStr());	// Use it to set.
		lonField->setValue(ourGPS->latLon.showLonStr());	// Default values.
	}																		//
}
	
	
addItemDBox::~addItemDBox(void) {

}



// **************************************************************
// **********************   editItemDBox    **********************
// **************************************************************

						
editItemDBox::editItemDBox(listener* inListener,gPosPack* inPos,const char* inName,screenTypes inType)
	: addEditDboxBase(EDIT_ITEM_MSG,inListener,inType) {
	
	globalPos	aGpos;
	
	if (inPos && inName) {
		aGpos.setPos(inPos);
		nameField->setValue(inName);
		latField->setValue(aGpos.showLatStr());	// Use it to set.
		lonField->setValue(aGpos.showLonStr());	// Default values.
	}
}
	
	
editItemDBox::~editItemDBox(void) {  }

	