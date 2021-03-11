#pragma once

#ifndef INTERFACE_BUTTON_MAP_H
#define INTERFACE_BUTTON_MAP_H 1

#include <SFML/Graphics.hpp>
#include "Button.h"


class ButtonMap
{
public:
	static std::map<std::string, Button*>& GetMap();

private:
	static std::map<std::string, Button*> buttons;
};

#endif /* !INTERFACE_BUTTON_MAP_H */

