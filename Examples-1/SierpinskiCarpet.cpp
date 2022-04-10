//21.11.4
#include "../FractalTemplate.cpp"

void SierpinskiCarpet(const Posize &ps)
{
	if (ps.w<=5||ps.h<=5)
		MainWindow->RenderFillRect(ps,ThemeColor[4]);
	else
		for (int i=0;i<=2;++i)
			for (int j=0;j<=2;++j)
				if (i!=1||j!=1)
					SierpinskiCarpet(Posize(ps.x+ps.w/3*i,ps.y+ps.h/3*j,ps.w/3+1,ps.h/3+1));
}

void Fractal(const Posize &ps)
{
	SierpinskiCarpet(ps.Shrink(20));
}

void FractalInit()
{SetFractalName(PUIT("SierpinskiµØÌº"));}
