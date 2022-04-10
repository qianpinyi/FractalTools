//21.11.4
#include "../FractalTemplate.cpp"

void Cantor(int ax,int ay,int bx,int by,PUI_Turtle tur)
{
	tur.MoveTo(ax,ay);
	tur.DrawTo(bx,by);
	if ((bx-ax)<10)
		return;
	int cx,cy,dx,dy;
	cx=ax+(bx-ax)/3;
	cy=ay+50;
	dx=bx-(bx-ax)/3;
	dy=by+50;
	ay+=50;
	by+=50;
	Cantor(ax,ay,cx,cy,tur);
	Cantor(dx,dy,bx,by,tur);
}

void Fractal(const Posize &ps)
{
	Point A=ps.Shrink(10).GetLU(),
		  B=ps.Shrink(10).GetRU();
	Cantor(A.x,A.y,B.x,B.y,PUI_Turtle());
}

void FractalInit()
{
	SetFractalName(PUIT("CantorÈý·Ö¼¯"));
}
