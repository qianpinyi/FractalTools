//21.11.4
#include "FractalTools.cpp"
using namespace Charset;
TinyText *TT_Name=NULL;
string FractalName,ProgramPath;

void SetFractalName(const string &name)
{
	if (FractalName==name) return;
	MainWindow->SetWindowTitle("Fractal: "+DeleteEndBlank(name)+" By:qianpinyi");
	TT_Name->SetText("Fractal: "+name);
	FractalName=name;
}

inline Posize GetFractalDisplayPosize()
{return Widgets::GetWidgetsByName("FractalShowLayer")->GetgPS();}

void Fractal(const Posize&);
void FractalInit(); 

int main(int argc,char **argv)
{
	SetCmdUTF8AndPrintInfo("Fractal","1.0","qianpinyi");
	ProgramPath=GetPreviousBeforeBackSlash(argv[0]);
	PUI_SetPreferredRenderer(PUI_PreferredRenderer_OpenGL);
	PAL_GUI_Init(PUI_WINPS_DEFAULT,"Fractal");
	new ShowLayerI("FractalShowLayer",PUI_FA_MAINWINDOW,new PosizeEX_Fa6(2,2,0,0,30,0),
		[](int&,const Posize &gPS,const Posize &lmt)
		{
			Fractal(gPS);
		},0);
	TT_Name=new TinyText(0,PUI_FA_MAINWINDOW,new PosizeEX_Fa6(2,3,30,30,0,30),"Fractal",0,ThemeColorM[7]);
	FractalInit();
	PUI_EasyEventLoop();
	return 0;
}
