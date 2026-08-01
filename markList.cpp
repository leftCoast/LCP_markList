#include <markList.h>
#include <fontLabel.h>

markList::markList(int ourAppID)
	: panel(ourAppID) { }


markList::~markList(void) {  }
		


void markList::setup(void) {

	fontLabel myText(10,40,300,40);
	
	screen->fillScreen(&black);
	myText.setFont(AFF_SANS_9_OB);
	myText.setColors(&green);
	myText.setValue("markList!");
	myText.draw();
}
				
				
void markList::loop(void) {


}

void markList::drawSelf(void) {  }
