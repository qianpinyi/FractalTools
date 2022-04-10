//21.11.4
#include "../FractalTemplate.cpp"

void Arboresent(Point2D a,Point2D b,Point2D c,PUI_Turtle tur)
{
	if (Dist(b,c)<20)
		tur.DrawLine(a,b).DrawTo(c).DrawTo(a);
	else
	{
		Arboresent(Intersection(Line2D(MidPoint(a,b),MidPoint(a,b)+(b-a).TurnL90()),Line2D(b,c)),a,b,tur);
		Arboresent(Intersection(Line2D(MidPoint(a,c),MidPoint(a,c)+(c-a).TurnL90()),Line2D(b,c)),a,c,tur);
	}
}

double maxEpsilon=60;
double epsilon=15;
void Fractal(const Posize &ps)
{
	int W=min(ps.w,ps.h)-10;
	Point2D p=Point2D(ps.GetCentre())+Point2D(0,W/2);
	Arboresent(p+Point2D(0,-W/2/cos((45-epsilon/2)/180*M_PI)).TurnRight((45+epsilon/2)/180*M_PI),
				p,p+Point2D(0,-W).TurnRight(epsilon/180*M_PI),PUI_Turtle());
	Arboresent(p+Point2D(0,-W/2/cos((45-epsilon/2)/180*M_PI)).TurnLeft((45+epsilon/2)/180*M_PI),
				p,p+Point2D(0,-W).TurnLeft(epsilon/180*M_PI),PUI_Turtle());
}

FullFillSliderI *FFS_Epsilon=NULL;
TinyText *TT_Epsilon=NULL;
void FractalInit()
{
	SetFractalName(PUIT("Arboresent·Î"));
	TT_Epsilon=new TinyText(0,PUI_FA_MAINWINDOW,new PosizeEX_Fa6(1,3,100,300,0,30),"epsilon("+llTOstr(epsilon)+"): ",1);
	FFS_Epsilon=new FullFillSliderI(0,PUI_FA_MAINWINDOW,new PosizeEX_Fa6(1,3,300,0,0,30),0,
		[](int&,double per,bool loose)
		{
			epsilon=per*maxEpsilon;
			TT_Epsilon->SetText("epsilon("+llTOstr(epsilon)+"): ");
			PUI_Window::SetNeedFreshScreenAll();
		},0);
	FFS_Epsilon->SetPercent(epsilon/maxEpsilon,0);
}
