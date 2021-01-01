#include <iostream>

#include "Mouse.h"

int main( int argc, char* argv[] ) {
	Mouse mouse;
    mouse.Initialize();

	while( true )
	{
        mouse.Update();
	}

	return 0;
}
