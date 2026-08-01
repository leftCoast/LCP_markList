#ifndef markList_h
#define markList_h

#include <lilOS.h>


class markList :	public panel {

	public:
				markList(int ourAppID);
	virtual	~markList(void);
		
				void	setup(void);
				void	loop(void);
	virtual	void	drawSelf(void);
};


#endif