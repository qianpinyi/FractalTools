//22.4.8
/*
	C:\"Program Files\NVIDIA GPU Computing Toolkit"\CUDA\v10.2\bin\nvcc.exe .\Mandelbrot-Cuda.cu -o .\Mandelbrot-Cuda.exe -ccbin "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx64\x64"  -std=c++14 -w -I"I:\ProgramProjects\DEV64-VC\include" -I"I:\ProgramProjects\PAL\PAL_Library"  -L"I:\ProgramProjects\DEV64-VC\lib" -lSDL2main -lSDL2 -lSDL2_ttf -lUser32 -Xcompiler /w34819
*/
#include "../FractalTemplate.cpp"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#define IterationColor(t) \
	~(((t)>>0)&0xff|((t)>>1&0xff)<<8|((t)>>2&0xff)<<16|0)
//	((255-(t)%256)%256|(255-(t)*2%256)%256<<8|(255)%256<<16|255<<24)

__global__ void GetFractalInKernel(Uint32 *data,const int W,const int H,const int Group,const double EscapeRadius,const double Scale,const double ShiftX,const double ShiftY,const int IterationColorCount)
{
	int y=threadIdx.x+Group*1024;
	int x=blockIdx.x;
	
	double Cx=(x*2.0/W-1)/Scale+ShiftX,
		   Cy=(y*2.0/H-1)/Scale+ShiftY,
		   Zx=0,
		   Zy=0;
	int t=0;
	while (t<IterationColorCount&&Zx*Zx+Zy*Zy<=EscapeRadius*EscapeRadius)
	{
		double nx=Zx*Zx-Zy*Zy+Cx,
			   ny=2*Zx*Zy+Cy;
		Zx=nx;
		Zy=ny;
		++t;
	}
	if (t>=IterationColorCount)
		t=0;
	data[y*W+x]=IterationColor(t);
}

FractalParameter EscapeRadius("EscapeRaius",2,0.05,10),
				 Scale("Scale",1,0.1,1e100),
				 ShiftX("ShiftX",-0.5,-1,1),
				 ShiftY("ShiftY",0,-1,1),
				 IterationColorCount("MaxIteration",256,16,2048);

void Fractal(const Posize &ps)
{
	Uint32 *Buffer=new Uint32[ps.w*ps.h];
	
	Uint32 *DeviceData=nullptr;
	cudaMalloc((void**)&DeviceData,ps.Size()*sizeof(Uint32));
	for (int i=0;i<(ps.h+1023)/1024;++i)
	{
		dim3 grid(ps.w);
		dim3 threads(min(ps.h-i*1024,1024));
		GetFractalInKernel << <grid, threads >> >(DeviceData,ps.w,ps.h,i,EscapeRadius,Scale,ShiftX,ShiftY,IterationColorCount);
		cudaDeviceSynchronize();
	}
	cudaMemcpy(Buffer,DeviceData,ps.Size()*sizeof(Uint32),cudaMemcpyDeviceToHost);
    cudaFree(DeviceData);
	
	SDL_Surface *sur=SDL_CreateRGBSurfaceWithFormatFrom((void*)Buffer,ps.w,ps.h,32,ps.w*4,SDL_PIXELFORMAT_RGBA32);
	SDL_SetSurfaceBlendMode(sur,SDL_BLENDMODE_BLEND);
	CurrentWindow->RenderCopy(CreateTextureFromSurfaceAndDelete(sur),ps.ToOrigin(),ps);
	DELETEtoNULL(Buffer);
}

void FractalInit()
{
	SetFractalName(PUIT("Mandelbrot EscapeTimeAlgorithm"));
	new PosEventLayerI(0,PUI_FA_MAINWINDOW,new PosizeEX_Fa6_Full,
		[](int&,const PUI_PosEvent *event,int mode)->int
		{
			if (event->type==PUI_Event::Event_MouseEvent)
				if (event->MouseEvent()->state&PUI_MouseEvent::Mouse_Left)
				{
					Posize WH=GetFractalDisplayPosize();
					ShiftX.SetX(ShiftX-event->delta.x*2.0/WH.w/Scale),ShiftY.SetX(ShiftY-event->delta.y*2.0/WH.h/Scale);
					return 1;
				}
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
	EscapeRadius.InitWidgets();
	Scale.InitWidgets();
	ShiftX.InitWidgets();
	ShiftY.InitWidgets();
	IterationColorCount.InitWidgets();
}
