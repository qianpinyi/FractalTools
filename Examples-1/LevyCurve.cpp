//21.11.4
#include "../FractalTemplate.cpp"

double minAlpha=0,maxAlpha=60;
double alpha=45;
void LevyCurve(Point2D a,Point2D b,Point2D c,PUI_Turtle tur,int dep=0)
{
	if (dep>=15) return;
	tur.MoveTo(b).DrawTo(a).DrawTo(c);
	if (Dist(b,c)>=5)
	{
		LevyCurve(MidPoint(a,b)+((a-b)*0.5*tan(alpha/180*M_PI)).TurnL90(),b,a,tur,dep+1);
		LevyCurve(MidPoint(a,c)+((a-c)*0.5*tan(alpha/180*M_PI)).TurnR90(),a,c,tur,dep+1);
	}
}

void Fractal(const Posize &ps)
{
	int W=min(ps.w,ps.h)-10,A=0.3*W;
	Point2D c=Point2D(ps.GetCentre())-Point2D(0,W/5);
	LevyCurve(c+Point2D(0,A*tan(alpha/180*M_PI)),c+Point2D(-A,0),c+Point2D(A,0),PUI_Turtle().DrawLine(c+Point2D(A,0),c+Point2D(-A,0)));
}

FullFillSliderI *FFS_Alpha=NULL;
TinyText *TT_Alpha=NULL;
void FractalInit()
{
	SetFractalName(PUIT("C(Levy)ÇúÏß"));
	TT_Alpha=new TinyText(0,PUI_FA_MAINWINDOW,new PosizeEX_Fa6(1,3,100,300,0,30),"alpha("+llTOstr(alpha)+"): ",1);
	FFS_Alpha=new FullFillSliderI(0,PUI_FA_MAINWINDOW,new PosizeEX_Fa6(1,3,300,0,0,30),0,
		[](int&,double per,bool loose)
		{
			alpha=per*(maxAlpha-minAlpha)+minAlpha;
			TT_Alpha->SetText("alpha("+llTOstr(alpha)+"): ");
			PUI_Window::SetNeedFreshScreenAll();
		},0);
	FFS_Alpha->SetPercent((alpha-minAlpha)/(maxAlpha-minAlpha),0);
}
