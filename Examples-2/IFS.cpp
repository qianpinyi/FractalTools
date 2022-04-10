//21.11.5
#include "../FractalTemplate.cpp"

struct IFSData
{
	bool Loaded=0;
	vector <Doublet <Affine2D,double> > IFSCode;
	string name;
	
	int Load(const string &path)
	{
		ifstream fin(path);
		if (!fin.is_open())
			return 1;
		Loaded=0;
		string str;
		getline(fin,str);
		if (str!="IFSVersion1")
			return 2;
		getline(fin,name);
		int cnt=0;
		fin>>cnt;
		IFSCode=vector <Doublet <Affine2D,double> > (cnt,{Affine2D(),1.0/cnt});
		for (int i=0;i<cnt;++i)
			fin>>IFSCode[i].a.a>>IFSCode[i].a.b>>IFSCode[i].a.c
			   >>IFSCode[i].a.d>>IFSCode[i].a.e>>IFSCode[i].a.f
			   >>IFSCode[i].b;
		if (fin.fail())
			return 3;
		Loaded=1;
		return 0;
	}
	
	int Save(const string &path)
	{
		ofstream fout(path);
		if (!fout.is_open())
			return 1;
		fout<<"IFSVersion1"<<endl
			<<name<<endl
			<<IFSCode.size()<<endl;
		for (auto &vp:IFSCode)
			fout<<vp.a.a<<vp.a.b<<vp.a.c
				<<vp.a.d<<vp.a.e<<vp.a.f
				<<vp.b;
		return 0;
	}
}CurIFS;

double mX,MX,mY,MY;
void IFS(int n,const Posize &ps,bool MeasureMode=0)
{
	Point2D pt(0,0);
	for (int i=0;i<n;++i)
	{
		double P=rand()*1.0/RAND_MAX,sumProb=0;
		for (auto &vp:CurIFS.IFSCode)
			if (P<=sumProb+vp.b)
			{
				pt=vp.a*pt;
				if (MeasureMode)
				{
					mX=min(mX,pt.x);
					MX=max(MX,pt.x);
					mY=min(mY,pt.y);
					MY=max(MY,pt.y);
				}
				else MainWindow->RenderDrawPoint(Point2D(pt.x*ps.w+ps.x,(1-pt.y)*ps.h+ps.y).ToPosizePoint(),ThemeColor[5]);
				break;
			}
			else sumProb+=vp.b;
	}
}

FractalParameter Scale("Scale",1.0,0,5),
				 ShiftX("ShiftX",0,-1000,1000),
				 ShiftY("ShiftY",0,-1000,1000),
				 Count("Count",100000,1000,500000);
int BaseWidth=100;

void Measure()
{
	mX=mY=1e9;
	MX=MY=-1e9;
	IFS(10000,ZERO_POSIZE,1);
}

void BestMatch()
{
	Posize ps=MainWindow->GetWinPS().Shrink(30);
	Scale.SetX(min(ps.w*1.0/BaseWidth/(MX-mX),ps.h*1.0/BaseWidth/(MY-mY)));
	ShiftX.SetX(-(MX+mX-1)*0.5*Scale*BaseWidth);
	ShiftY.SetX((MY+mY-1)*0.5*Scale*BaseWidth);
}

void Fractal(const Posize &ps)
{
	BaseWidth=min(ps.w,ps.h)-10;
	if (CurIFS.Loaded)
		IFS(Count,Posize(ps.GetCentre(),0,0).Expand(BaseWidth*Scale/2)+Point(ShiftX,ShiftY));
}

ButtonI *Bu_Load=NULL;
string FileSelectPath;
void FractalInit()
{
	SetFractalName(PUIT("IFS 迭代函数系统算法"));
	FileSelectPath=ProgramPath+"\\IFSCodes";
	new PosEventLayerI(0,PUI_FA_MAINWINDOW,new PosizeEX_Fa6_Full,
		[](int&,const PUI_PosEvent *event,int mode)->int
		{
			if (event->type==PUI_Event::Event_MouseEvent)
				if (event->MouseEvent()->state&PUI_MouseEvent::Mouse_Left)
					return ShiftX.SetX(ShiftX+event->delta.x),ShiftY.SetX(ShiftY+event->delta.y),1;
			return 0;
		},0);
	new EventLayerI(0,PUI_FA_MAINWINDOW,
		[](int&,const PUI_Event *event)->int
		{
			if (event->type==PUI_Event::Event_WheelEvent)
				if (event->WheelEvent()->dy!=0)
					return Scale.SetX(Scale*(event->WheelEvent()->dy<0?0.8:1.2)),1;
			return 0;
		},0);
	Bu_Load=new ButtonI(0,PUI_FA_MAINWINDOW,{0,0,60,30},PUIT("加载"),
		[](int&)
		{
			new SimpleFileSelectBox(0,[](const string &path)->int
			{
				FileSelectPath=GetPreviousBeforeBackSlash(path);
				if (CurIFS.Load(path))
					SetSystemBeep(SetSystemBeep_Error),
					(new MessageBoxButtonI(0,PUIT("错误"),PUIT("无法载入数据！")))->AddButton(PUIT("确定"));
				else
				{
					SetFractalName(PUIT("IFS分形：")+CurIFS.name);
					Measure();
					BestMatch();
				}
				PUI_Window::SetNeedFreshScreenAll();
				return 0;
			},FileSelectPath,{".txt",".ifs"});
		},0);
	Count.InitWidgets();
	Scale.InitWidgets();
	ShiftX.InitWidgets();
	ShiftY.InitWidgets();
	new ButtonI(0,PUI_FA_MAINWINDOW,new PosizeEX_Fa6(1,3,100,0,FractalParameter::FractalParameterStackCnt*30+1,28),PUIT("最佳尺寸"),[](int&){BestMatch();},0);
}
