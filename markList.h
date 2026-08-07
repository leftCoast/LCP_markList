#ifndef markList_h
#define markList_h

#include <lilOS.h>
#include <fontLabel.h>
#include <scrollingList.h>
#include <globalPos.h>


extern navMark selectedMark;
extern bool		haveMarkSet;


// *****************************************************
// ******************   markListObj   ******************
// *****************************************************


class markListObj :	public navMark,
							public drawGroup {

	public:
				markListObj(scrollingList* inList);
	virtual	~markListObj(void);
	
	virtual	void	setName(const char* inName);
	virtual	void	setPos(gPosPack* inPos);
				void	setLblColors(void);
	virtual	void  setThisFocus(bool setLoose);
				void	setMark(void);
	virtual	void	draw(void);
	virtual	void	drawSelf(void);
	
	protected:
	virtual	void	doAction(event* inEvent,point* localPt);
				void	setup(void);
				void	setLatLonLbls(void);
				
				scrollingList*	ourlist;
				bool				scrolling;
				bool				init;
				erasableText*	nameLbl;
				erasableText*	distLbl;
				erasableText*	bearingLbl;
				erasableText*	latLbl;
				erasableText*	lonLbl;
};
	

	
// *****************************************************
// *****************    markListList   *****************
// *****************************************************


class markListList :	public scrollingList {

	public:
				markListList(rect* inRect);
	virtual	~markListList(void);
	
	virtual	void	setup(void);
				void	addItem(const char* inName,gPosPack* inGPos);
	virtual	void	drawSelf(void);
};
	
	
	
// *****************************************************
// ******************     markList    ******************
// *****************************************************

class selectBtn;

class markList :	public panel {

	public:
				markList(int ourAppID);
	virtual	~markList(void);
		
				void	setup(void);
	virtual	void	drawSelf(void);
				void	selected(markListObj* itemPtr);
				void	setMark(void);
				void	loop(void);

	
				markListList*	ourList;
				selectBtn* 		setMarkBtn;
				markListObj*	selectedItem;
};


// *****************************************************
// ******************    selectBtn    ******************
// *****************************************************

class selectBtn :	public iconButton {

	public:
				selectBtn(int xLoc,int yLoc,const char* iconPath,markList* inApp);
	virtual	~selectBtn(void);
	
	virtual	void	doAction(event* inEvent,point* localPt);			
				
				markList*	ourLink;
};



#endif