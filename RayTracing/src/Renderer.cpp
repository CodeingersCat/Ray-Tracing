#include "Renderer.h"
#include "Walnut/Random.h"
#include<glm/glm.hpp>
#include<math.h>
#include<iostream>

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height) return;
		m_FinalImage->Resize(width, height);

	} else {
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render()
{
	//Render every pixel

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++) {
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++) {
			{
				glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
				coord = coord * 2.0f - 1.0f;
				m_ImageData[x + y*m_FinalImage->GetWidth()] = PerPixel(coord);
				
			}
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);

	float radius = 0.5f;
	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	glm::vec3 rayOrigin(0.0f, 0.0f, 1.0f);

	//Ray intersection equation
	// (bx^2 + by^2)t^2 + 2*(axbx + ayby)t + (ax^2 + ay^2 - r^2) = 0
	// where
	// a = ray origin
	// b = ray direction
	// r = radius of sphere/circle
	// t = hit distance

	//float a = pow(rayDirection.x, 2.0) + pow(rayDirection.y, 2.0) + pow(rayDirection.z, 2.0);
	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - pow(radius, 2.0);
	
	//Quadratic discriminant
	//b^2 - 4*a*c
	float discmnt = b * b - 4.0f * a * c;

	if (discmnt >= 0.0f) return 0xffff00ff;
	// else 
		
	return 0x00000000;
	//return 0xff000000 | (g << 8) | r;
}