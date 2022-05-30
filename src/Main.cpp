/**
 * @file Main.cpp
 *
 * @brief Entrypoint for the program.
 *
 * Martin Johnson
 * Billy-Joe Lacasse
 * William Lebel
 */

#include "MainWindow.h"

int main()
{
	MainWindow MainWindow;
	int init_value = MainWindow.initialisation();
	if (init_value != 0) {
		return init_value;
	}

	return MainWindow.renderLoop();
}