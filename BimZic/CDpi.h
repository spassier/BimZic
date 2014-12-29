#pragma once


#define MAXSUPPORTDPI 200

// Definition: relative pixel = 1 pixel at 96 DPI and scaled based on actual DPI.
class CDpi
{
public:
	CDpi() : _fInitialized(false), _dpiX(96), _dpiY(96) { }

	// Get screen DPI.
	int GetDPIX() { _Init(); return _dpiX; }
	int GetDPIY() { _Init(); return _dpiY; }
	int GetDPIXO(){ _Init(); return _dpiXO;}
	int GetDPIYO(){ _Init(); return _dpiYO;}
	// Convert between raw pixels and relative pixels.
	int ScaleX(int x) { if (bScale) { _Init(); return MulDiv(x, _dpiX, 96); } else return x; }
	int ScaleY(int y) { if (bScale) { _Init(); return MulDiv(y, _dpiY, 96); } else return y; }
	float ScaleX(float x) { if (bScale) { _Init(); return ((x*_dpiX)/96); } else return x; }
	float ScaleY(float y) { if (bScale) { _Init(); return ((y*_dpiY)/96); } else return y; }
	int UnscaleX(int x) { _Init(); return MulDiv(x, 96, _dpiX); }
	int UnscaleY(int y) { _Init(); return MulDiv(y, 96, _dpiY); }

	// Determine the screen dimensions in relative pixels.
	int ScaledScreenWidth() { return _ScaledSystemMetricX(SM_CXSCREEN); }
	int ScaledScreenHeight() { return _ScaledSystemMetricY(SM_CYSCREEN); }

	// Scale rectangle from raw pixels to relavice pixels.
	void ScaleRect(__inout RECT *pRect)
	{
		pRect->left = ScaleX(pRect->left);
		pRect->right = ScaleX(pRect->left);
		pRect->top = ScaleY(pRect->top);
		pRect->bottom = ScaleY(pRect->bottom);
	}
	void ScaleRect(__inout RectF *pRect)
	{
		pRect->X = ScaleX(pRect->X);
		pRect->Y = ScaleY(pRect->Y);
		pRect->Width = ScaleX(pRect->Width);
		pRect->Height = ScaleY(pRect->Height);
	}

	// Determine if screen resolution meets minimum requirements in relative pixels.
	bool IsResolutionAtLeast(int cxMin, int cyMin)
	{
		return (ScaledScreenWidth() >= cxMin) && (ScaledScreenHeight() >= cyMin);
	}

	// Convert a point size (1/72 of an inch) to raw pixels.
	//int PointsToPixels(int pt) { return MulDiv(pt, _dpiY, 72); }
		//int PointsToPixels(int pt) { return MulDiv(pt, _dpiY, 96); }
	float PointsToPixels(float pt) { if (bScale) { return pt; } else { _Init(); return pt * (96.0F/_dpiX);} }
	
	// Invalidate any cached metrics.
	void Invalidate() { _fInitialized = false; }

public:
	~CDpi(void) { };

private:
	void _Init()
	{
		if (!_fInitialized)
		{
			HDC hdc = GetDC(NULL);
			if (hdc)
			{
				_dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
				_dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
				ReleaseDC(NULL, hdc);
			}
			_fInitialized = true;
			if(_dpiX>MAXSUPPORTDPI)
			{
				_dpiXO=_dpiX;
				_dpiX=MAXSUPPORTDPI;
			}
			if(_dpiY>MAXSUPPORTDPI)
			{
				_dpiYO=_dpiY;
				_dpiY=MAXSUPPORTDPI;
			}
		}
	}

	int _ScaledSystemMetricX(int nIndex)
	{
		_Init();
		return MulDiv(GetSystemMetrics(nIndex), 96, _dpiX);
	}

	int _ScaledSystemMetricY(int nIndex)
	{
		_Init();
		return MulDiv(GetSystemMetrics(nIndex), 96, _dpiY);
	}

private:
	bool _fInitialized;
	int _dpiXO;
	int _dpiYO;
	int _dpiX;
	int _dpiY;

	bool bScale;

public:
	void SetScale(bool inScale) { bScale = inScale; }
	bool GetScale() { return bScale; }
};
