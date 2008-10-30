/*
 *  ButtonArray.h
 *  JuceMIDIAppx
 *
 *  Created by Matthew Pott on 29/10/2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _BUTTONARRAY_H_
#define _BUTTONARRAY_H_

class ButtonArray : public Component,
					public ButtonListener

{
public:
	Array<TextButton*> arrayOfButtons;
	
	ButtonArray(int numButtons)
	{
		for(int i = 0; i < numButtons; i++)
		{
			arrayOfButtons.add(new TextButton (String(i+1)));
			
			addAndMakeVisible(arrayOfButtons[i]);
			
			arrayOfButtons[i]->addButtonListener(this);
			
			arrayOfButtons[i]->setClickingTogglesState(true);
			
			arrayOfButtons[i]->setConnectedEdges(15);
		}	
	}
	
	~ButtonArray()
	{
		deleteAllChildren();
	}
	
	bool getToggleState(int which)
	{
		return arrayOfButtons[which]->getToggleState();
	}
	
	void buttonClicked (Button* button)
	{

	}
	
	void resized()
	{
		for(int i = 0; i < arrayOfButtons.size(); i++)
		{
			arrayOfButtons[i]->setBounds ((25*i), 0, 20, 20);
		}	
		
	}	
	
};	



#endif //_BUTTONARRAY_H_
