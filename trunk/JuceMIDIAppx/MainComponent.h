#ifndef _MAINCOMPONENT_H_
#define _MAINCOMPONENT_H_

#include <juce/juce.h>
#include "ButtonArray.h"

// uint32 seems to be defined in multiple places, this is a hack for now..
#define uint32 JUCE_NAMESPACE::uint32

class MainComponent  :	public Component,
						public ButtonListener,
						public ComboBoxListener,
						public Thread
	{
	private:
		StringArray midiDevices;
		MidiOutput* midiOutput;
		ComboBox* midiOutputSelector;
		TextButton* play;
		TextButton* stop;
		Label* text;
		Array<int> noteSeq;
		int	index;
		int rate;
		Array<ButtonArray*> buttonMatrix; 
		
	public:
		//==============================================================================
		juce_UseDebuggingNewOperator // fixes some window compiler issue
		
		/** Create our main component which does all of the app's work.
		 */
		MainComponent () 
			:	Thread(T("Sequencer")),
				midiOutput(0),
				index(0),
				rate(125)
		{	
					
			for(int i = 0; i < 4; i++)
			{
				ButtonArray* buttons;
				buttons = new ButtonArray(16);
				addAndMakeVisible(buttons);
				buttonMatrix.add(buttons);
			//	buttonMatrix[i]->getToggleState(20);
			}
			
			// simple sequencing example,  using a thread
			
			// get a list of our MIDI output devices
			midiDevices = MidiOutput::getDevices();
			
			// create a combo box (menu) component
			addAndMakeVisible(midiOutputSelector = new ComboBox("MIDI Output Selector"));
			midiOutputSelector->setBounds(10, 10, 270, 20);
			midiOutputSelector->addListener(this);
			
			// populate the menu with our MIDI device names
			for(int i = 0; i < midiDevices.size(); i++)
			{
				midiOutputSelector->addItem(midiDevices[i], i+1);
			}
			
			// choose the first item 1 (note that the first item is not 0 in this case for combo boxes)
			midiOutputSelector->setSelectedId(1, false);
			
			// play button
			addAndMakeVisible(play = new TextButton(T("Play")));
			play->setBounds(10, 40, 270, 20);
			play->addButtonListener(this);
			
			// stop button
			addAndMakeVisible(stop = new TextButton(T("Stop")));
			stop->setBounds(10, 70, 270, 20);
			stop->addButtonListener(this);
			
			// some text for our status
			addAndMakeVisible(text = new Label(T("Text"),T("Ready...")));
			text->setBounds(10, 100, 270, 20);
			
			// add some notes to the sequence, noteSeq is an Array<int>
			noteSeq.add(60);
			noteSeq.add(62);
			noteSeq.add(64);
			noteSeq.add(65);
			noteSeq.add(67);
			noteSeq.add(65);
			noteSeq.add(64);
			noteSeq.add(62);
		}
		
		~MainComponent ()
		{
			deleteAllChildren();
		}
		
		//==============================================================================
		
		void resized ()
		{
			// This is called whenever this component's size changes. We could respond
			// to this in a number of ways, but the most obvious thing to do is reposition
			// all our widgets, using their 'setBounds()' function.
			// It's nice to position them relative to the size of this Component. That means
			// making use of the 'getWidth()' and 'getHeight()' functions to determine where
			// to put them and how big they should be.
			
			for(int i = 0; i < buttonMatrix.size(); i++)
			{
				buttonMatrix[i]->setBounds (0, i*20+130, getWidth(), getHeight()); 
			}
		}
		
		
		void buttonClicked(Button* button)
		{
			if(button == play)
			{
				text->setText(T("Playing"), false);
				
				// startThread starts our thread which ultimately causes our run() method to be called
				// 10 = highest priority
				startThread(10); 
			}
			else if(button == stop)
			{
				text->setText(T("Stopped"), false);
				
				// stop (and timeout after 3 secs, if this fails and then force if necessary)
				stopThread(3000); 
			}
				
		}
		
		void comboBoxChanged(ComboBox* comboBox)
		{
			if(comboBox == midiOutputSelector)
			{
				// open the midi output selected via the menu
				midiOutput = MidiOutput::openDevice(midiOutputSelector->getSelectedItemIndex());
				
				// this would be called near the start of the program since we do..
				//  midiOutputSelector->setSelectedId(1, false);
				// ..shortly after populating our combo box menu in the constructor
			}
		}
		
		
		void run()
		{
			// this is where our Thread runs after startThread() is called when the play button is clicked
			
			while( ! threadShouldExit() )
			{
				// get the current time in ms, do this first so it is as accurate as possible
				const uint32 now = Time::getMillisecondCounter();
				
				// .. even bfeore checking our midi output is valid (i.e., not 0)
				if(midiOutput != 0)
				{
					// the '%' is the modulo operator, it gives the remainder after a division
					// so although index keeps getting larger.. doing index % noteSeq.size() always keeps
					// the index into the array betwen 0 and (noteSeq.size()-1)
					// e.g. if noteSeq.size() is 16 the return of index % noteSeq.size()
					// will always be between 0...15
					// Therefor (index - 1) % noteSeq.size() should get the previous note in the sequence
					
					// note off for the previous note..
				//	int previousNoteIndex = (index - 1) % noteSeq.size();
				//	midiOutput->sendMessageNow(MidiMessage::noteOff(1, noteSeq[previousNoteIndex]));
				//	midiOutput->sendMessageNow(MidiMessage::noteOff(2, noteSeq[previousNoteIndex] + 4));
				//	midiOutput->sendMessageNow(MidiMessage::noteOff(3, 35));
					
					// note on for new note..
				//	int currentNoteIndex = index % noteSeq.size();
				//	midiOutput->sendMessageNow(MidiMessage::noteOn(1, noteSeq[currentNoteIndex], 0.9f));
				//	midiOutput->sendMessageNow(MidiMessage::noteOn(2, noteSeq[currentNoteIndex] + 4, 0.9f));
				//	midiOutput->sendMessageNow(MidiMessage::noteOn(3, 35,0.9f));
					
					for(int i = 0; i < 16; i++)
					{
						
						if (buttonMatrix[0]->getToggleState(i)==true)
						{
							midiOutput->sendMessageNow(MidiMessage::noteOff(3, 35));
							midiOutput->sendMessageNow(MidiMessage::noteOn(3, 35,0.9f));
						
						}
				
						if (buttonMatrix[1]->getToggleState(i)==true)
						{
							midiOutput->sendMessageNow(MidiMessage::noteOff(3, 40));
							midiOutput->sendMessageNow(MidiMessage::noteOn(3, 40,0.9f));
						} 
						
						if (buttonMatrix[2]->getToggleState(i)==true)
						{
							midiOutput->sendMessageNow(MidiMessage::noteOff(3, 42));
							midiOutput->sendMessageNow(MidiMessage::noteOn(3, 42,0.9f));
						} 
						
						if (buttonMatrix[3]->getToggleState(i)==true)
						{
							midiOutput->sendMessageNow(MidiMessage::noteOff(3, 46));
							midiOutput->sendMessageNow(MidiMessage::noteOn(3, 46,0.9f));
						} 
					}
					
					
				}
			
				index++;
				
				// pause until the next event is due i.e. the time it was at the top of the loop
				// plus 'rate' which is the time we want between events (defaulted to 125ms)
				Time::waitForMillisecondCounter(now + rate);
			}
			
			// we break out of the while loop when the Thread is told to stop via stopThread()
			// here we send an all notes off message to prevent hangin notes (which haven't had a real note off)
			if(midiOutput != 0) 
				midiOutput->sendMessageNow(MidiMessage::allNotesOff(1));
		}
		
	};

#endif//_MAINCOMPONENT_H_ 