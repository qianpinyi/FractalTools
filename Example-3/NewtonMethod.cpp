//22.4.8
#include "../FractalTemplate.cpp"
#include <complex>
#include <omp.h> 

FractalParameter EscapeRadius("EscapeRaius",10,0.05,100);

const inline int IterationColorCount()
{return 20;}

inline RGBA IterationColor(int t)
{return RGBA(255-t*25,255-t*25,255,255);}

void Fractal(const Posize &ps)
{
	RGBA *Buffer=new RGBA[ps.w*ps.h];
	#pragma omp parallel for
	for (int i=0;i<ps.h;++i)
		for (int j=0;j<ps.w;++j)
		{
			complex<double> z(j*8.0/ps.w-4,i*8.0/ps.h-4);
			int t=0;
			while (t<IterationColorCount()&&norm(z)<=EscapeRadius*EscapeRadius)
				z=z-(z*z*z*z*z-1.0)/(4.0*z*z*z*z),++t;
			if (t>=IterationColorCount())
				Buffer[i*ps.w+j]=RGBA_BLACK;
			else
			{
				RGBA co=IterationColor(t);
				Buffer[i*ps.w+j]=co;
			}
		}
	SDL_Surface *sur=SDL_CreateRGBSurfaceWithFormatFrom((void*)Buffer,ps.w,ps.h,32,ps.w*4,SDL_PIXELFORMAT_RGBA32);
	SDL_SetSurfaceBlendMode(sur,SDL_BLENDMODE_BLEND);
	CurrentWindow->RenderCopy(CreateTextureFromSurfaceAndDelete(sur),ps.ToOrigin(),ps);
	DELETEtoNULL(Buffer);
}

void FractalInit()
{
	SetFractalName(PUIT("Newton's Method EscapeTimeAlgorithm"));
	EscapeRadius.InitWidgets();
}
