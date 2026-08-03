#ifndef markList_h
#define markList_h

#include <lilOS.h>
#include <fontLabel.h>


// *****************************************************
// ******************   markListObj   ******************
// *****************************************************


class markListObj :	public drawGroup {

	public:
				markListObj(void);
	virtual	~markListObj(void);
	
	virtual	void	setup(void);
				void	setValues(const char* inName,double inLat,double inLon);
	virtual	void	drawSelf(void);
	
				fontLabel*	markName;
				fontLabel*	markDist;
				fontLabel*	markBearing;
				fontLabel*	markLat;
				fontLabel*	markLon;
};
	

	
// *****************************************************
// *****************    markListList   *****************
// *****************************************************


class markListList :	public drawList {

	public:
				markListList(void);
	virtual	~markListList(void);
	
	virtual	void	setup(void);
				void	addItem(const char* inName,double inLat,double inLon);
	virtual	void	drawSelf(void);
};
	
	
	
// *****************************************************
// ******************     markList    ******************
// *****************************************************


class markList :	public panel {

	public:
				markList(int ourAppID);
	virtual	~markList(void);
		
				void	setup(void);
				void	loop(void);
	virtual	void	drawSelf(void);
	
				markListList*	ourList;
};


#endif