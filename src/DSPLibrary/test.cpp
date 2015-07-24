#include "DSPDLL.h"
#include <iostream.h>

int main(int arg, char **input) {
	int MAXFRAMES = 1500;
	int BEGIN = 0;
	//int numberofchannels = 7;
	//int channeldef[7] = {128,64,64,64,64,64,64};
	int numberofchannels = 21;
	int channeldef[21] = {80,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20};

	if(arg < 2) {
		cout << "You need to pass a binary file as an argument." << endl;
		return (-1);
	}
	cout << "loading file" <<endl;
	if(DSPDLL(512,input[1] ,BEGIN,BEGIN+MAXFRAMES) < 0)
		return(-2);

	cleanup();
	return(0);
}