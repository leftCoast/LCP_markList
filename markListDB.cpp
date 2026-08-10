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
// **********************   addItemDBox    **********************
// **************************************************************
	
						
addItemDBox::addItemDBox(listener* inListener,screenTypes inType)
	: alertObj(ADD_ITEM_MSG,inListener,noteAlert,false,false),
	kbdUser(this,inType) {
	
	rect		editRect;
	frame*	editFrame;
	
	setRect(60,100,200,146);									// Move our alertBox down a bit.
	theMsg->y = 30;												// Kick the default message over a bit.
	editRect.setRect(10,70,width-20,12);					// Setup location for the edit box.																				//
	
	nameField = new editLabel(&editRect,"");				// Create it.
	nameField->setLocation(3,3);
	nameField->setColors(&black,&white);					// Set colors
	nameField->setEventSet(fullClick);
	nameD = new datafield(&editRect);
	nameD->begin(getKbd(),nameField);
	addObj(nameD);													// Hook it up.
	editFrame = new frame(&editRect);						// Using the edit field rect create a frame.
	editFrame->insetRect(-3);									// Expand it by 3 pixels per side.
	nameD->setRect(editFrame);
	addObj(editFrame);											// Hook it up.	
	
	editRect.y = editRect.y+22;								// Bump down, next line.
	latField = new editLabel(&editRect,"");				// Create it.
	latField->setLocation(3,3);
	latField->setColors(&black,&white);						// Set colors
	latField->setEventSet(fullClick);
	latD = new datafield(&editRect);
	latD->begin(getKbd(),latField);
	addObj(latD);													// Add it to the list.
	editFrame = new frame(&editRect);							// Using the edit field rect create a frame.
	editFrame->insetRect(-3);									// Expand it by 3 pixels per side.
	latD->setRect(editFrame);
	addObj(editFrame);											// In the pot it goes.
	
	editRect.y = editRect.y+22;								// Bump down, next line.
	lonField = new editLabel(&editRect,"");				// Create it.
	lonField->setLocation(3,3);
	lonField->setColors(&black,&white);						// Set colors
	lonField->setEventSet(fullClick);
	lonD = new datafield(&editRect);
	lonD->begin(getKbd(),lonField);
	addObj(lonD);							
	editFrame = new frame(&editRect);						// Using the edit field rect create a frame.
	editFrame->insetRect(-3);									// Expand it by 3 pixels per side.
	lonD->setRect(editFrame);
	addObj(editFrame);											// And add it in.
	
	if (ourGPS->valid) {
		latField->setValue(ourGPS->latLon.showLatStr());
		lonField->setValue(ourGPS->latLon.showLonStr());
	}
	hookup();
	nameField->setValue("Home");
	latField->setValue("48.49033 n");
	lonField->setValue("122.66190 W");
	
															// Hook into the idler queue.
}
	
	
addItemDBox::~addItemDBox(void) {

}


const char*	addItemDBox::getName(void) { return nameField->getTextBuff(); }


gPosPack addItemDBox::getPos(void) {

	posParser	aParser;

	return aParser.parsePos(latField->getTextBuff(),lonField->getTextBuff());
}

/*
void addItemDBox::handleCom(stdComs comID) {

	if (comID==okCmd) {
		Serial.println("got OkCmd");
		Serial.println(nameField->getTextBuff());
		Serial.println(latField->getTextBuff());
		Serial.println(lonField->getTextBuff());
	}
	alertObj::handleCom(comID);
	//if (comID==newItemCmd) {
	//	nameField->clearStr();
	//}
}


void addItemDBox::idle(void) {
	
	alertObj::idle();
	
}
*/

// **************************************************************
// **********************   editItemDBox    **********************
// **************************************************************

						
editItemDBox::editItemDBox(listener* inListener,const char* inName,screenTypes inType)
	: alertObj(EDIT_ITEM_MSG,inListener,noteAlert,false,false),
	kbdUser(this,inType) {
	
	rect	editRect;
	
	theMsg->x += 4;															// Kick the default message over a bit.
	y += 50;																		// Move our alertBox down a bit.
	editRect.setRect(52,40,75,10);										// Setup location for the edit box.
	nameField = new editLabel(&editRect,inName);						// Create it.
	nameField->setColors(&black,&white);								// Set colors
	setEditField(nameField);												// We also manage the keyboard. So, edit this.
	addObj(nameField);														// Attach the edit field to our list.
	
	frame*	editFrame = new frame(nameField);						// Using the edit field rect create a frame.
	editFrame->insetRect(-3);												// Expand it by 3 pixels per side.
	addObj(editFrame);														// And add that to our list.
	
	hookup();																	// Hook into the idler queue.
}
	
	
editItemDBox::~editItemDBox(void) {  }


//void	editItemDBox::setName(const char* itemName) { nameField->setValue(itemName); }


const char*	editItemDBox::getName(void) { return nameField->editBuff; }


gPosPack editItemDBox::getPos(void) {

	posParser	aParser;
		
	//return aParser.parsePos(latField->editBuff,lonField->editBuff);
}
	