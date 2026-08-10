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
// ********************   addEditDboxBase    ********************
// **************************************************************


class addEditDboxBase :	public alertObj,
								public kbdUser {

public:
			addEditDboxBase(const char* alertMsg,listener* inListener,screenTypes inType=sType240x320);
virtual	~addEditDboxBase(void);
				
			const char*	getName(void);
			gPosPack 	getPos(void);

			editLabel*	nameField;
			editLabel*	latField;
			editLabel*	lonField;
			datafield*	nameD;
			datafield*	latD;
			datafield*	lonD;
};



// **************************************************************
// **********************   addItemDBox    **********************
// **************************************************************


class addItemDBox :	public addEditDboxBase {

	public:
			addItemDBox(listener* inListener,screenTypes inType=sType240x320);
virtual	~addItemDBox(void);

};



// **************************************************************
// **********************   editItemDBox    *********************
// **************************************************************


class editItemDBox :	public addEditDboxBase {

	public:
			editItemDBox(listener* inListener,gPosPack* inPos,const char* inName,screenTypes inType=sType240x320);
virtual	~editItemDBox(void);

};
			
#endif