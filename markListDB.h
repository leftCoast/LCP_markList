#ifndef markListDB_h
#define markListDB_h

#include <editLabel.h>
#include <datafield.h>
#include <modalKbd.h>
#include <alertObj.h>
#include <globalPos.h>



// **************************************************************
// *************************   frame   **************************
// **************************************************************


class frame : public drawObj {

	public:
				frame(rect* inRect);
				~frame(void);
				
	virtual	void	drawSelf(void);
};


// **************************************************************
// ***********************   readmeDBox   ***********************
// **************************************************************


class readmeDBox :	public alertObj {

	public:
				readmeDBox(listener* inListener);
				~readmeDBox(void);			
};



// **************************************************************
// **********************  deleteOkAlert   **********************
// **************************************************************


class deleteOkAlert :	public alertObj {

	public:
				deleteOkAlert(listener* inListener);
				~deleteOkAlert(void);			
};



// **************************************************************
// **********************   clearOkAlert   **********************
// **************************************************************


class clearOkAlert :	public alertObj {

	public:
				clearOkAlert(listener* inListener);
				~clearOkAlert(void);			
};


// **************************************************************
// **********************   addItemDBox    **********************
// **************************************************************


class addItemDBox :	public alertObj,
							public kbdUser {

	public:
				addItemDBox(listener* inListener,screenTypes inType=sType240x320);
				~addItemDBox(void);
				
				const char*	getName(void);
				gPosPack 	getPos(void);
	virtual	void  		handleCom(stdComs comID);
	virtual	void			idle(void);
			
				editLabel*	nameField;
				editLabel*	latField;
				editLabel*	lonField;
				datafield*	nameD;
				datafield*	latD;
				datafield*	lonD;
				gPosPack		ourPos;	
};



// **************************************************************
// **********************   editItemDBox    *********************
// **************************************************************


class editItemDBox :	public alertObj,
							public kbdUser {

	public:
				editItemDBox(listener* inListener,const char* inName=NULL,screenTypes inType=sType240x320);
				~editItemDBox(void);
				
				const char*	getName(void);
				gPosPack 	getPos(void);
				
				editLabel*	nameField;
				editLabel*	latField;
				editLabel*	lonField;
				gPosPack		ourPos;
};
			
#endif