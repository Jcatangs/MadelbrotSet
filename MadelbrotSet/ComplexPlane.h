#pragma once
#ifndef COMPLEX_PLANE_H
#define COMPLEX_PLANE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <vector>
#include <complex>
#include <cmath>

using namespace std;
using namespace sf;

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum State { CALCULATING, DISPLAYING };

class ComplexPlane : public Drawable
{
	public:
		ComplexPlane(int pixelWidth, int pixelHeight);
		void draw(RenderTarget& target, RenderStates states) const override;
		void zoomIn();
		void zoomOut();
		void setCenter(Vector2i mousePixel);
		void setMouseLocation(Vector2i mousePixel);
		void loadText(Text& text);
		void updateRender();
		
	private:
		VertexArray m_vArray; 
		State m_State; 
		Vector2f m_mouseLocation; 
		Vector2i m_pixel_size; 
		Vector2f m_plane_center; 
		Vector2f m_plane_size;
		int m_zoomCount; 
		float m_aspectRatio;

		int countIterations(Vector2f coords);
		void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);
		Vector2f mapPixelToCoords(Vector2i mousePixel);

};

#endif