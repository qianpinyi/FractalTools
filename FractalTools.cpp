#ifndef FRACTALTOOLS_CPP
#define FRACTALTOOLS_CPP 1
#include <PAL_GUI/PAL_GUI_0.cpp>
#include <PAL_Algorithm/PAL_ComputationalGeometry.cpp>
using namespace PAL_GUI;
using namespace PAL_Algorithm;
#ifndef M_PI
#define M_PI 3.1415926535898
#endif

class PUI_Turtle//21.11.4
{
	protected:
		PUI_Window *win=NULL;
		double x=0,y=0,direction=0;//direction in degree
		RGBA Color=ThemeColorM[5];
		
	public:
		inline PUI_Turtle& MoveTo(double newX,double newY,bool keepDirection=0)
		{
			if (x==newX&&y==newY) return *this;
			if (!keepDirection)
				if (newX==x)
					direction=newY>y?90:-90;
				else if (newX>x)
					direction=atan((newY-y)/(newX-x))/M_PI*180;
				else if (newY>=y)
					direction=atan((newY-y)/(newX-x))/M_PI*180+180;
				else direction=atan((newY-y)/(newX-x))/M_PI*180-180;
			x=newX;y=newY;
			return *this;
		}
		
		inline PUI_Turtle& MoveTo(const Point2D &p,bool keepDirection=0)
		{return MoveTo(p.x,p.y,keepDirection);}
		
		inline PUI_Turtle& DrawTo(double newX,double newY,bool keepDirection=0)
		{
			win->RenderDrawLine(Point(x+0.5,y+0.5),Point(newX+0.5,newY+0.5),ThemeColor(Color));
			return MoveTo(newX,newY,keepDirection);
		}
		
		inline PUI_Turtle& DrawTo(const Point2D &p,bool keepDirection=0)
		{return DrawTo(p.x,p.y,keepDirection);}
		
		inline PUI_Turtle& DrawLine(double xs,double ys,double xt,double yt,bool keepDirection=0)
		{
			MoveTo(xs,ys,1);
			return DrawTo(xt,yt,keepDirection);
		}
		
		inline PUI_Turtle& DrawLine(const Point2D &p1,const Point2D &p2,bool keepDirection=0)
		{return DrawLine(p1.x,p1.y,p2.x,p2.y,keepDirection);}
		
		inline PUI_Turtle& MoveForward(double d=1.0)
		{return MoveTo(x+d*cos(direction/180*M_PI),y+d*sin(direction/180*M_PI),1);}
		
		inline PUI_Turtle& DrawForward(double d=1.0)
		{return DrawTo(x+d*cos(direction/180*M_PI),y+d*sin(direction/180*M_PI),1);}
		
		inline PUI_Turtle& Turn(double delta)
		{
			direction+=delta;
			while (direction>180)
				direction-=360;
			while (direction<-180)
				direction+=360;
			return *this;
		}
		
		inline PUI_Turtle& SetDirection(double _direction)
		{
			direction=_direction;
			return *this;
		}
		
		inline PUI_Turtle& SetColor(const RGBA &co)
		{
			Color=co;
			return *this;
		}
		
		inline Point2D CurrentPoint() const
		{return Point2D(x,y);}
		
		inline double CurrentX() const
		{return x;}
		
		inline double CurrentY() const
		{return y;}
		
		inline double Direction() const
		{return direction;}
		
		PUI_Turtle(double _x,double _y,double _direction,PUI_Window *_win=CurrentWindow)
		:x(_x),y(_y),direction(_direction),win(_win) {}
		
		PUI_Turtle(const Point2D &p,double _direction,PUI_Window *_win=CurrentWindow)
		:x(p.x),y(p.y),direction(_direction),win(_win) {}
		
		PUI_Turtle():x(0),y(0),direction(0),win(CurrentWindow) {}
};

struct FractalParameter
{
	static int FractalParameterStackCnt;
	static int FractalParameterWidgetWidth;
	string name;
	double x,DefaultX,MinX,MaxX;
	bool Changed=0;
	Layer *Lay=NULL;
	FullFillSlider <FractalParameter*> *FFS=NULL;
	TinyText *TT=NULL;
	Button <FractalParameter*> *Bu=NULL;
	
	operator double () const
	{return x;}
	
	void SetX(double _x)
	{
		if (FFS!=NULL)
			FFS->SetPercent((EnsureInRange(_x,MinX,MaxX)-MinX)/(MaxX-MinX),1);
		else x=EnsureInRange(_x,MinX,MaxX);
	}
	
	string GetXStr()
	{
		stringstream ss;
		ss<<x;
		return ss.str();
	}
	
	void InitWidgets(PosizeEX *psex=NULL)
	{
		if (psex==NULL)
			psex=new PosizeEX_Fa6(1,3,FractalParameterWidgetWidth,0,FractalParameterStackCnt++*30,30);
		Lay=new Layer(0,PUI_FA_MAINWINDOW,psex);
		TT=new TinyText(0,Lay,new PosizeEX_Fa6(3,2,0,140,0,0),name+"("+GetXStr()+"): ");
		FFS=new FullFillSlider<FractalParameter*>(0,Lay,new PosizeEX_Fa6(2,2,140,60,1,1),0,
			[](FractalParameter *&This,double per,bool)
			{
				This->x=per*(This->MaxX-This->MinX)+This->MinX;
				This->TT->SetText(This->name+"("+This->GetXStr()+"): ");
				This->Changed=1;
				PUI_Window::SetNeedFreshScreenAll();
			},this);
		Bu=new Button<FractalParameter*>(0,Lay,new PosizeEX_Fa6(1,2,60,0,1,1),PUIT("ÖØÖÃ"),
			[](FractalParameter *&This)
			{
				This->FFS->SetPercent((This->DefaultX-This->MinX)/(This->MaxX-This->MinX),1);
			},this);
		FFS->SetPercent((DefaultX-MinX)/(MaxX-MinX),0);
	}
	
	static void SetWidgetWidth(int w)
	{FractalParameterWidgetWidth=w;}
	
	FractalParameter(const string &_name,double defaultX,double minX,double maxX):name(_name),x(defaultX),DefaultX(defaultX),MinX(minX),MaxX(maxX) {}
};
int FractalParameter::FractalParameterStackCnt=0;
int FractalParameter::FractalParameterWidgetWidth=500;

struct Affine2D
{
	double a,b,c,d,e,f;
	
	inline Point2D operator * (const Point2D &p) const
	{return Point2D(a*p.x+b*p.y+e,c*p.x+d*p.y+f);}

	Affine2D(double _a,double _b,double _c,double _d,double _e,double _f)
	:a(_a),b(_b),c(_c),d(_d),e(_e),f(_f) {}
	
	Affine2D() {}
};

#endif
