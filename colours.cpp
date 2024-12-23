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

	ID2D1SolidColorBrush* m_BlackBrush;
	ID2D1SolidColorBrush* m_WhiteBrush;
	ID2D1SolidColorBrush* m_GreyBrush;
	ID2D1SolidColorBrush* m_RedBrush;
	ID2D1SolidColorBrush* m_YellowBrush;
	ID2D1SolidColorBrush* m_GreenBrush;
	ID2D1SolidColorBrush* m_CyanBrush;
	ID2D1SolidColorBrush* m_BlueBrush;

	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0, 1.0f), &m_BlackBrush);
	palette["BLACK"] = m_BlackBrush;

	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &m_WhiteBrush);
	palette["WHITE"] = m_WhiteBrush;
	
	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(0.75f, 0.75f, 0.75f, 1.0f), &m_GreyBrush);
	palette["GREY"] = m_GreyBrush;

	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0, 0, 1.0f), &m_RedBrush);
	palette["RED"] = m_RedBrush;

	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0, 1.0f), &m_YellowBrush);
	palette["YELLOW"] = m_YellowBrush;

	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 1.0f, 0, 1.0f), &m_GreenBrush);
	palette["GREEN"] = m_GreenBrush;

	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 1.0f, 1.0f, 1.0f), &m_CyanBrush);
	palette["CYAN"] = m_CyanBrush;
	
	hr = _renderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 1.0f, 1.0f), &m_BlueBrush);
	palette["BLUE"] = m_BlueBrush;
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