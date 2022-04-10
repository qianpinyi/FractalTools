//22.4.8
#include "../FractalTemplate.cpp"
#include <complex>
#include <omp.h> 

FractalParameter EscapeRadius("EscapeRaius",2,0.05,10);

const inline int IterationColorCount()
{
	return 20;
//	return 256;
}

inline RGBA IterationColor(int t)
{
	static RGBA co[20]{RGBA_NONE,RGBA_NONE,RGBA_NONE,RGBA_BLACK,RGBA_RED,
					  {255,200,100,255},{255,150,100,255},{255,150,50,255},{255,100,50,255},{255,100,0,255},
					  {221,138,153,255},{247,142,148,255},{237,102,99,255},{239,92,72,255},{177,41,53,255},
					  {64,126,175,255},{97,167,227,255},{0,142,225,255},{0,104,183,255},{0,9,153,255}};
	return co[t];
//	return RGBA(255-t,255-t,255,255);
}

void Fractal(const Posize &ps)
{
	RGBA *Buffer=new RGBA[ps.w*ps.h];
	#pragma omp parallel for
	for (int i=0;i<ps.h;++i)
		for (int j=0;j<ps.w;++j)
		{
			complex<double> z(0,0),c(j*2.0/ps.w-1-0.5,i*2.0/ps.h-1);
			int t=0;
			while (t<IterationColorCount()&&norm(z)<=EscapeRadius*EscapeRadius)
				z=z*z+c,++t;
			if (t>=IterationColorCount())
				t=0;
			RGBA co=IterationColor(t);
			Buffer[i*ps.w+j]=co;
		}
	SDL_Surface *sur=SDL_CreateRGBSurfaceWithFormatFrom((void*)Buffer,ps.w,ps.h,32,ps.w*4,SDL_PIXELFORMAT_RGBA32);
	SDL_SetSurfaceBlendMode(sur,SDL_BLENDMODE_BLEND);
	CurrentWindow->RenderCopy(CreateTextureFromSurfaceAndDelete(sur),ps.ToOrigin(),ps);
	DELETEtoNULL(Buffer);
}

void FractalInit()
{
	SetFractalName(PUIT("Mandelbrot EscapeTimeAlgorithm"));
	EscapeRadius.InitWidgets();
}
