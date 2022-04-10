//22.1.9
#include "../FractalTemplate.cpp"
#include <complex>
#include <omp.h> 

FractalParameter C_Real("C-Real Part",-0.615,-1,1),
				C_Imag("C-Imaginary Part",-0.43,-1,1),
				EscapeRadius("EscapeRaius",2,0.05,10);

const inline int IterationColorCount()
{return 256;}

inline RGBA IterationColor(int t)
{
	return RGBA(255-t,255-t,255,255);
}

void Fractal(const Posize &ps)
{
	RGBA *Buffer=new RGBA[ps.w*ps.h];
	#pragma omp parallel for
	for (int i=0;i<ps.h;++i)
		for (int j=0;j<ps.w;++j)
		{
			complex<double> z(j*2.0/ps.w-1,i*2.0/ps.h-1);
			int t=0;
			while (t<IterationColorCount()&&norm(z)<=EscapeRadius*EscapeRadius)
				z=z*z+complex<double>(C_Real,C_Imag),++t;
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
	SetFractalName(PUIT("JuliaSet EscapeTimeAlgorithm"));
	C_Real.InitWidgets();
	C_Imag.InitWidgets();
	EscapeRadius.InitWidgets();
}
