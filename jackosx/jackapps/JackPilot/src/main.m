//
//  main.m
//  JackPilot1.0
//
//  Created by Johnny Petrantoni on Thu Jul 10 2003.
//  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

int s_debug = FALSE;

int main(int argc, const char *argv[])
{
	if(argv[1]) {
		if(argv[1][0]=='d') s_debug = TRUE;
	}
    return NSApplicationMain(argc, argv);
}