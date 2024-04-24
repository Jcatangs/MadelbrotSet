#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) //: m_thread(&ComplexPlane::updateRender, this)
{
	m_pixel_size = { pixelWidth, pixelHeight };
	m_aspectRatio = pixelHeight / (double)pixelWidth;
	m_plane_center = { 0, 0 };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_zoomCount = 0;
	m_State = State::CALCULATING;

	m_vArray.setPrimitiveType(Points);
	m_vArray.resize((size_t)(pixelWidth * pixelHeight));

}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
	int pixelHeight = m_pixel_size.y;
	int pixelWidth = m_pixel_size.x;

	//Thread thread(&ComplexPlane::updateRender, this); //DANGEROUSSSSSSS
	//thread.launch();
	//thread.wait();
	//Thread thread1(&ComplexPlane::updateRender, this);
	//Thread thread2(&ComplexPlane::updateRender, this);
	//Thread thread3(&ComplexPlane::updateRender, this);
	//thread1.launch();
	//thread2.launch();
	//thread3.launch();
	//thread1.wait();
	//thread2.wait();
	//thread3.wait();

	if (m_State == CALCULATING)
	{

		for (int i = 0; i < pixelHeight; i++)
		{
			for (int j = 0; j < pixelWidth; j++)
			{
				m_vArray[j + i * m_pixel_size.x].position = { (float)j,(float)i };

				Vector2f complex_coords = mapPixelToCoords(Vector2i(j,i)); //Find Vector2f MAP TO (j,i) MAYBE NEEDS FIXIN
				int iterations = countIterations(complex_coords);
				Uint8 r, g, b;

				iterationsToRGB(iterations, r, g, b);
				m_vArray[j + i * m_pixel_size.x].color = Color( r,g,b );
			}
		}
	}
	m_State = DISPLAYING;
}

void ComplexPlane::zoomIn()
{
	m_zoomCount++;

	float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x, y };
	m_State = CALCULATING;
}
void ComplexPlane::zoomOut()
{
	if (m_zoomCount > 0) m_zoomCount--;

	float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x, y }; 
	m_State = CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
	m_plane_center = mapPixelToCoords(mousePixel);
	m_State = CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
	double a = m_plane_center.x;
	double b = m_plane_center.y;

	double x = m_mouseLocation.x;
	double y = m_mouseLocation.y;

	complex<double> c(a, b);
	complex<double> mouse(x, y);

	stringstream text_box;
	text_box << "Mandelbrot Set" <<
		"\nCenter: (" << c.real() << ',' << c.imag()<< ')' <<
		"\nCursor: (" << mouse.real() << ',' << mouse.imag() << ')' <<
		"\nLeft-click to Zoom in" <<
		"\nRight-click to Zoom out" << endl;

	text.setString(text_box.str());
}


int ComplexPlane::countIterations(Vector2f coords)
{
	complex<double> c(coords.x, coords.y);
	complex<double> z(0, 0);

	for (int i = 0; i < MAX_ITER; i++)
	{
		z = z * z + c;
		if (abs(z) > 2.0) 
		{
			return i;
		}
	}
	return MAX_ITER; 
}


void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	if (count == MAX_ITER)
	{
		r = 225;
		g = 222;
		b = 222;
	}
	else
	{
		if (count >= 0 && count < 17) // Purple / blue for low iteration counts
		{
			r = 0;
			g = 102;
			b = 255;
		}
		else if (count >= 17 && count < 33) // Turquoise
		{
			r = 0;
			g = 221;
			b = 255;
		}
		else if (count >= 33 && count < 49) // Green
		{
			r = 0;
			g = 255;
			b = 128;
		}
		else if (count >= 49 && count < 64) // turq
		{
			r = 0;
			g = 255;
			b = 229;
		}
		else // Red for high iteration counts
		{
			r = 51;
			g = 220;
			b = 220;
		}
	}
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) 
{
	float nX = mousePixel.x;  
	float nY = mousePixel.y; 
	float a = 0;               
	float bX = m_pixel_size.x; 
	float bY = m_pixel_size.y; 

	float x = ((nX - a) / (bX - a)) * m_plane_size.x + (m_plane_center.x - m_plane_size.x * 0.5);
	float y = ((nY - a) / (bY - a)) * m_plane_size.y + (m_plane_center.y - m_plane_size.y * 0.5);

	return Vector2f(x, y);
}

