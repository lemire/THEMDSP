///////////////////////////////////////////////////////////////////
//
// (c) 1999, 2000 Daniel Lemire, Ph.D. (Daniel.Lemire@Videotron.ca)
// for THEM Geophysics. Please call if you have any problem
// with this code (514) 524-3517.
//
// http://www.techelements.com/
// http://www.ondelette.com/
//
// This code shouldn't not be used for any other purpose but
// to process THEM Geophysics signals without my consent.
// 
////////////////////////////////////////////////////////////////////

#include "DSPDLL.h"
#include <iostream.h>

int main(int arg, char **input) {
	int MAXFRAMES = 1500;
	int BEGIN = 0;
	if(arg < 2) {
		cout << "You need to pass a binary file as an argument." << endl;
		return (-1);
	}
	cout << "loading file (input file must be an extracted signal EM1, EM2 or EM3)" <<endl;
	int ERRORCODE = 0;
	if((ERRORCODE = DSPDLL(512,input[1] ,BEGIN,BEGIN+MAXFRAMES)) < 0) {
		cout << "problem "<<ERRORCODE << endl;
		cout << "most likely cause is that the software cannot find the file"<< endl;
		return(-2);
	}
	textWrite("signalbefore.txt", BEGIN,BEGIN+MAXFRAMES);
	applyDespike();
	applySIMn();
	applyStacking(6);
	textWrite("signalafter.txt", BEGIN,BEGIN+MAXFRAMES);
	int numberofchannels = 21;
	int channeldef[21] = {80,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};
	computeChannels(numberofchannels, channeldef ,1000);
	textWriteChannels("channels.txt");
	cleanup();
	return(0);
}