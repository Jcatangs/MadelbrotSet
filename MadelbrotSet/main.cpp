#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <complex>
#include <cmath>
#include "ComplexPlane.h"

using namespace sf;
using namespace std;

int main()
{
	Clock clock;
	int window_width = VideoMode::getDesktopMode().width * 0.5;
	int window_height = VideoMode::getDesktopMode().height * 0.5;

	VideoMode vm(window_width, window_height);
	RenderWindow window(vm, "Mandelbrot Set", Style::Default);
	
	ComplexPlane Mandelbrot_set(window_width, window_height);

	Font font; //Refer for coordinate presentation LOAD TEXTS //LOAD FONTS
	font.loadFromFile("tuffy.ttf");

	Text text("", font);
	text.setCharacterSize(20);
	text.setStyle(Text::Regular);
	text.setFillColor(Color::White);

	while (window.isOpen())
	{
		Event event;
		clock.restart();
	
		//INPUT SEGMENT
		while (window.pollEvent(event))
		{ 
			if (event.type == Event::Closed) window.close();
			
			else if (event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == Mouse::Right)
				{
					//zoomOut
					Mandelbrot_set.zoomOut();
					//call setCenter on ComplexPlane with the (x,y) pixel on the mouse click	
					Mandelbrot_set.setCenter(Mouse::getPosition(window));
				}

				else if (event.mouseButton.button == Mouse::Left)
				{
					//zoomIn
					Mandelbrot_set.zoomIn();
					clock.restart();
					//call setCenter on ComplexPlane with (x,y) of mouse click
					//this will be used to display mouse coordinates
					Mandelbrot_set.setCenter(Mouse::getPosition(window));
				}
			}
			if (event.type == Event::MouseMoved) Mandelbrot_set.setMouseLocation(Mouse::getPosition(window));
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();

		//UPDATE SEGMENT
			/* 
				::: updateRender on ComplexPlane :::
				::: loadText on ComplexPlane :::
			*/
		//Mandelbrot_set.updateRender();
		//Mandelbrot_set.updateRenderMain();
		Mandelbrot_set.multiThread();
		Mandelbrot_set.loadText(text);

		//DRAW SCENE SEGMENT
		window.clear();
		window.draw(Mandelbrot_set);
		window.draw(text);
		window.display();

	}
	
	return 0;
}
