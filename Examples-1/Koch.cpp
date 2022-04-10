//21.11.4
#include "../FractalTemplate.cpp"

void Koch(Point2D p1,Point2D p2,PUI_Turtle tur)
{
	if (Dist(p1,p2)<10)
		tur.DrawLine(p1,p2);
	else
	{
		Point2D a,b,c;
		Koch(p1,a=p1+(p2-p1)*(1.0/3),tur);
		Koch(a,b=a+((p2-p1)*(1.0/3)).TurnRight(M_PI/3),tur);
		Koch(b,c=p1+(p2-p1)*(2.0/3),tur);
		Koch(c,p2,tur);
	}
}

void Fractal(const Posize &ps)
{
	int W=min(ps.w,ps.h)-10;
	Point2D c=ps.GetCentre();
	Koch(c+Point2D(0,-W/2).TurnRight(M_PI/3),c+Point2D(0,-W/2).TurnLeft(M_PI/3),PUI_Turtle());
	Koch(c+Point2D(0,-W/2).TurnLeft(M_PI/3),c+Point2D(0,W/2),PUI_Turtle());
	Koch(c+Point2D(0,W/2),c+Point2D(0,-W/2).TurnRight(M_PI/3),PUI_Turtle());
}

void FractalInit()
{
	SetFractalName(PUIT("KochÑ©»¨"));
}
