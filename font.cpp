#ifndef FONT_CPP
#define FONT_CPP

//THIS IS MOSTLY for DEV AND DEBUG more fitting font system to be added later
//This is also all written for purpose and will need to be abstracted later...

namespace Font
{
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

	bool font_initalised = false;

	void Create();
	void Update();
	void Render(ID2D1HwndRenderTarget* _renderTarget, std::string _text);
}

void Font::Create()
{
	HRESULT hr;
	//Create a DriectWrite Factory
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, 
		__uuidof(m_pDWriteFactory),
		reinterpret_cast<IUnknown **>(&m_pDWriteFactory));

	//Create DirectWrite text object
	hr = m_pDWriteFactory->CreateTextFormat(
		L"Verdana",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		12.0f,
		L"",
		&m_pTextFormat
		);
	
	m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	font_initalised = true;
}

void Font::Render(ID2D1HwndRenderTarget* _renderTarget, std::string _text)
{
	if(!font_initalised){return;}

	D2D1_SIZE_F renderTargetSize = _renderTarget->GetSize();

	//Do silly string conversion
	std::wstring wide_string = std::wstring(_text.begin(), _text.end());
	const wchar_t* result = wide_string.c_str();

	
	_renderTarget->DrawText(
		result,
		wide_string.size(),
		m_pTextFormat,
		D2D1::RectF(0, 0, 100, 200),
		COLOURS::palette["GREEN"]
		);
}

#endif