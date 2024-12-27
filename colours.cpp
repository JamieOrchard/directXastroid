#ifndef COLOURS_H
#define COLOURS_H

namespace COLOURS
{
	std::map<std::string, ID2D1SolidColorBrush*> palette;

	void Init(ID2D1HwndRenderTarget* _renderTarget);
	void Deinit();
}

void COLOURS::Init(ID2D1HwndRenderTarget* _renderTarget)
{
	HRESULT hr;

	ID2D1SolidColorBrush* m_Brush;

	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 1.0f), &m_Brush);
	palette["BLACK"] = m_Brush;

	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &m_Brush);
	palette["WHITE"] = m_Brush;
	
	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(0.75f, 0.75f, 0.75f, 1.0f), &m_Brush);
	palette["GREY"] = m_Brush;

	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0, 0, 1.0f), &m_Brush);
	palette["RED"] = m_Brush;

	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0, 1.0f), &m_Brush);
	palette["YELLOW"] = m_Brush;

	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 1.0f, 0, 1.0f), &m_Brush);
	palette["GREEN"] = m_Brush;

	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 1.0f, 1.0f, 1.0f), &m_Brush);
	palette["CYAN"] = m_Brush;
	
	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 1.0f, 1.0f), &m_Brush);
	palette["BLUE"] = m_Brush;

	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.49f, 0.15f), &m_Brush);
	palette["ORANGE"] = m_Brush;
}

void COLOURS::Deinit()
{
	/*
	SafeRelease(COLOURS::palette["BLACK"]);
	SafeRelease(COLOURS::palette["WHITE"]);
	SafeRelease(COLOURS::palette["GREY"]);
	SafeRelease(COLOURS::palette["RED"]);
	SafeRelease(COLOURS::palette["YELLOW"]);
	SafeRelease(COLOURS::palette["GREEN"]);
	SafeRelease(COLOURS::palette["CYAN"]);
	SafeRelease(COLOURS::palette["BLUE"]);
	*/
}

#endif