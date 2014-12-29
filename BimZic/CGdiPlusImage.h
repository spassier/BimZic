#pragma once


class CGdiPlusImage
{
public:
	Gdiplus::Image* pImage;

public:
	CGdiPlusImage()						{pImage = NULL;}
	virtual ~CGdiPlusImage()	{Empty();}

	void Empty()
	{ 
		if(pImage)
		{
			delete(pImage);
			(pImage) = NULL;
		}
	}

	bool Load(LPCWSTR pFile)
	{
		Empty();
		pImage = Image::FromFile(pFile);
		return pImage->GetLastStatus() == Gdiplus::Ok;
	}

	int GetWidth()
	{
		return pImage->GetWidth();
	}

	int GetHeight()
	{
		return pImage->GetHeight();
	}

	operator Gdiplus::Image*() const			{return pImage;}
};