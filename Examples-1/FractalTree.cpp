//21.11.5
#include "../FractalTemplate.cpp"

FractalParameter alpha("alpha",15,-90,90),
				 maxDepth("maxDepth",10,0,20),
				 Ratio("ratio",0.7,0.1,0.9),
				 mainHeight("mainHeigth",0.3,0.1,0.9);
				 
void FractalTree1(Point2D a,double len,PUI_Turtle tur,int dep=0)
{
	tur.DrawForward(len);
	if (len<5||dep>maxDepth)
		return;
	tur.Turn(-alpha);
	FractalTree1(tur.CurrentPoint(),len*Ratio,tur,dep+1);
	tur.Turn(alpha*2);
	FractalTree1(tur.CurrentPoint(),len*Ratio,tur,dep+1);
}

void FractalTree2(Point2D a,double len,PUI_Turtle tur,int dep=0)
{
	for (int i=0;i<=1;++i)
	{
		tur.DrawForward(len/2);
		if (len<5||dep>maxDepth)
			return;
		tur.Turn(-alpha);
		FractalTree2(tur.CurrentPoint(),len*Ratio,tur,dep+1);
		tur.Turn(alpha*2);
		FractalTree2(tur.CurrentPoint(),len*Ratio,tur,dep+1);
		tur.Turn(-alpha);
	}
}	

void FractalTree3(Point2D a,double len,PUI_Turtle tur,int dep=0)
{
	tur.DrawForward(len/2);
	if (len<5||dep>maxDepth)
		return;
	tur.Turn(-15);
	FractalTree3(tur.CurrentPoint(),len*Ratio,tur,dep+1);
	tur.Turn(15);
	tur.DrawForward(len/2);
	tur.Turn(alpha);
	FractalTree3(tur.CurrentPoint(),len*Ratio,tur,dep+1);
}

void FractalTree4(Point2D a,double len,PUI_Turtle tur,int dep=0)
{
	tur.DrawForward(len*(1-Ratio));
	if (len<5||dep>maxDepth)
		return;
	tur.Turn(-alpha);
	FractalTree4(tur.CurrentPoint(),len*Ratio,tur,dep+1);
	tur.Turn(alpha);
	FractalTree4(tur.CurrentPoint(),len*Ratio,tur,dep+1);
	tur.Turn(alpha);
	FractalTree4(tur.CurrentPoint(),len*Ratio,tur,dep+1);
}

int CurrentTreeType=0;
void Fractal(const Posize &ps)
{
	int W=(min(ps.w,ps.h)-10)*mainHeight;
	Point2D p=Point2D(ps.midX(),ps.y2()-10);
	PUI_Turtle tur;
	tur.MoveTo(p).SetDirection(-90);
	switch (CurrentTreeType)
	{
		case 0:	FractalTree1(p,W,tur);	break;
		case 1:	FractalTree2(p,W,tur);	break;
		case 2:	FractalTree3(p,W,tur);	break;
		case 3:	FractalTree4(p,W,tur);	break;
	}
}

SingleChoiceButtonI *SCB=NULL;
void FractalInit()
{
	new TinyText(0,PUI_FA_MAINWINDOW,{0,0,150,30},PUIT("选择分形递归树类型:"),-1);
	SCB=new SingleChoiceButtonI(0,PUI_FA_MAINWINDOW,{0,30,160,120},
		[](int&,const string &name,int pos)
		{
			SetFractalName(name);
			CurrentTreeType=pos;
			PUI_Window::SetNeedFreshScreenAll();
		},0);
	SCB->SetAccentData(30,15,18);
	SCB->AddChoice(PUIT("Y型"));
	SCB->AddChoice(PUIT("双Y型"));
	SCB->AddChoice(PUIT("侧偏型"));
	SCB->AddChoice(PUIT("三叉型"));
	SCB->ChooseChoice(CurrentTreeType,1);
	alpha.InitWidgets();
	maxDepth.InitWidgets();
	Ratio.InitWidgets();
	mainHeight.InitWidgets();
}
