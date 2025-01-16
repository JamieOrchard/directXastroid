#ifndef FONT_CPP
#define FONT_CPP

//THIS IS MOSTLY for DEV AND DEBUG more fitting font system to be added later
//This is also all written for purpose and will need to be abstracted later...

class Text
{
public:
	std::string content;
	D2D1_RECT_F pos;
	ID2D1SolidColorBrush* colour;

	bool is_loaded = false;

	void Create(D2D1_RECT_F _pos, std::string _colour);
};

void Text::Create(D2D1_RECT_F _pos, std::string _colour)
{
	pos = _pos;
	colour = COLOURS::palette[_colour];
	is_loaded = true;
}

namespace Font
{
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

	bool font_initalised = false;

	void Create();
	void Update();
	void Render(Text* _text);
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

void Font::Render(Text* _text)
{
	if(!font_initalised){return;}
	if(!_text->is_loaded){return;}

	D2D1_SIZE_F renderTargetSize = renderTarget->GetSize();

	//Do silly string conversion
	std::wstring wide_string = std::wstring(_text->content.begin(), _text->content.end());
	const wchar_t* result = wide_string.c_str();

	
	renderTarget->DrawText(
		result,
		wide_string.size(),
		m_pTextFormat,
		_text->pos,
		_text->colour
		);
}

#endif