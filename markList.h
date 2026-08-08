#ifndef markList_h
#define markList_h

#include <lilOS.h>
#include <fontLabel.h>
#include <scrollingList.h>
#include <globalPos.h>
#include <blockFile.h>
#include <markListDB.h>

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
	
	virtual	void		setName(const char* inName);
	virtual	void		setPos(gPosPack* inPos);
				void		setLblColors(void);
	virtual	void  	setThisFocus(bool setLoose);
				void		setMark(void);
	virtual	void		draw(void);
	virtual	void		drawSelf(void);
				void		setMarkID(uint32_t ID);
				uint32_t	getMarkID(void);
				
	protected:
	virtual	void	doAction(event* inEvent,point* localPt);
				void	setup(void);
				void	setLatLonLbls(void);
				
				scrollingList*	ourlist;
				bool				scrolling;
				erasableText*	nameLbl;
				erasableText*	distLbl;
				erasableText*	bearingLbl;
				erasableText*	latLbl;
				erasableText*	lonLbl;
				uint32_t			markID;
				
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
				void	addItem(markListObj* newItem);
	virtual	void	drawSelf(void);
};
	
	
// **********************************************************************
// IDList
// **********************************************************************

class IDList {

	public:
				IDList(blockFile* aBlockFile);
	virtual	~IDList(void);
	
				unsigned long	getNumRootBytes(void);
				int				getNumItems(void);
				bool				getList(bool plusOne=false);
				unsigned long	addItem(void);
				bool				removeItem(unsigned long oldID);

				unsigned long*	IDBuff;
				blockFile*		ourBlockFile;
};



// **********************************************************************
// itemMgr
// **********************************************************************


class itemMgr :	public IDList {

	public:
				itemMgr(blockFile* aBlockFile);
	virtual	~itemMgr(void);
	
				void	populateList(void);
				void	addNewItem(const char* inName,gPosPack* inPos);
				void	deleteItem(markListObj* oldView);
				void	saveItem(markListObj* aMark);
				void	saveSelected(markListObj* selected);
				void	sortList(void);
				
	protected:
				bool	readItem(markListObj* aMark);
				
				blockFile*	ourBlockFile;
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
				void	setItemIcons(bool selectActive,bool addActive,bool editActive,bool delActive,bool readmeActive);
	virtual	void  handleCom(stdComs comID);								// Watching commands come in.
				void	selected(markListObj* itemPtr);
				void	setMark(void);
				void	addMark(markListObj* newObj);
				void	loop(void);

				markListList*	ourList;
				markListObj*	selectedItem;
				selectBtn* 		setMarkBtn;
				stdComBtn*		addItem;
				stdComBtn*		delItem;
				stdComBtn*		editItem;
				stdComBtn*		readmeBtn;
				readmeDBox*		ourReadmeDBox;
				addItemDBox*	ourAddItemDBox;
				editItemDBox*	ourEditItemDBox;
				clearOkAlert* 	checkClear;
				deleteOkAlert*	checkDelete;
				blockFile*		ourBlockFile;
				itemMgr*			ourItemMgr;	
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