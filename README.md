个人兴趣为了方便画分形开发的分形工具包，对PAL_GUI做了简单的封装

一方面是封装了PUI_Turtle，和大部分的绘图Turtle类似。

另一方面写了FractalTemplate简化窗口这方面的操作，FractalParameter用于简化参数与控件的绑定。

FractalTemplate用户只需要实现FractalInit()和Fractal(const Posize &ps)这两个函数的函数体即可。

可以看代码了解大致用法。

PAL_GUI等PAL_Library的子库需要到我的其他仓库下载（也可能还没更新最新版）

ppt是22.4.10南京航空航天大学计算机科学与技术学院科协创新训练营讲座使用的，可以用于学习参考。