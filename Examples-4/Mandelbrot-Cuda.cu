//22.4.8
/*
	C:\"Program Files\NVIDIA GPU Computing Toolkit"\CUDA\v10.2\bin\nvcc.exe .\Mandelbrot-Cuda.cu -o .\Mandelbrot-Cuda.exe -ccbin "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.24.28314\bin\Hostx64\x64"  -std=c++14 -w -I"I:\ProgramProjects\DEV64-VC\include" -I"I:\ProgramProjects\PAL\PAL_Library"  -L"I:\ProgramProjects\DEV64-VC\lib" -lSDL2main -lSDL2 -lSDL2_ttf -lUser32 -Xcompiler /w34819
*/
#include "../FractalTemplate.cpp"
#include <complex>

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#define IterationColorCount() 256
#define IterationColor(t) (255-(t)|255-(t)<<8|255<<16|255<<24)

__global__ void GetFractalInKernel(Uint32 *data,const int W,const int H,const int Group,const double EscapeRadius)
{
	int y=threadIdx.x+Group*1024;
	int x=blockIdx.x;
	
	double Cx=x*2.0/W-1-0.5,
		   Cy=y*2.0/H-1,
		   Zx=0,
		   Zy=0;
	int t=0;
	while (t<IterationColorCount()&&Zx*Zx+Zy*Zy<=EscapeRadius*EscapeRadius)
	{
		double nx=Zx*Zx-Zy*Zy+Cx,
			   ny=2*Zx*Zy+Cy;
		Zx=nx;
		Zy=ny;
		++t;
	}
	if (t>=IterationColorCount())
		t=0;
	data[y*W+x]=IterationColor(t);
}

FractalParameter EscapeRadius("EscapeRaius",2,0.05,10);

void Fractal(const Posize &ps)
{
	Uint32 *Buffer=new Uint32[ps.w*ps.h];
	
	Uint32 *DeviceData=nullptr;
	cudaMalloc((void**)&DeviceData,ps.Size()*sizeof(Uint32));
	for (int i=0;i<(ps.h+1023)/1024;++i)
	{
		dim3 grid(ps.w);
		dim3 threads(min(ps.h-i*1024,1024));
		GetFractalInKernel << <grid, threads >> >(DeviceData,ps.w,ps.h,i,EscapeRadius);
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
	EscapeRadius.InitWidgets();
}
