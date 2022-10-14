# RayTracingInOneWeekend

## 8. Diffuse Materials
![Image](https://github.com/Richbabe/RayTracingInOneWeekend/blob/main/Image/DiffuseModelsCompare.png?raw=true)

第八章三种代码分别代表了三种不同的Diffuse光照模型：
* 作者原方法
* Lambertian漫反射
* Hemisphere scatter漫反射

现在业界都采用Lambertian漫反射模型：
* 作者原方法的漫反射模型是错的，不符合物理
* Hemisphere scatter漫反射是Lambertian漫反射没提出来之前许多raytracing papers都在用，与法线跟入射光线夹角无关，上半表面的所有角度反射概率一样大

关于作者原方法与Lambertian漫反射的对比：

作者原方法是在以HitPos + Normal为原点的单位球内任取一点，这使得反射点在法线方向的概率更大，而掠射角（90°）方向概率更小，这种分布趋近于$ cos^3θ $的关系（θ为入射光线与法线的夹角）。这样当光线从切线方向入射时也会散射到很大片区域, 对最终颜色值的影响也会更低。

而Lambertian漫反射是符合兰伯特定律，即从不同方向照射到p点的光在该点的贡献是不同的，贡献度为$ cosθ $（θ为入射光线与法线的夹角）。从法线方向来的入射光线贡献最大，从接近切线方向（与法线方向垂直）来的入射光线贡献最小（趋近于0）。

![Image](https://github.com/Richbabe/RayTracingInOneWeekend/blob/main/Image/LamberCompare.png?raw=true)

例如上图，作者原方法的采样点S'会位于球心到图中S点的连线上，此时从交点出发连接PS‘会比Lambertian模型的反射光线PS更加靠近法线N，分布更不均匀。

关于作者原方法与Lambertian漫反射渲染出来的图片的对比：
1. 阴影部分变少了
2. 大球和小球都变亮了

这些变化都是由散射光线分布更均匀引起的, 现在更少的光线会朝着法线方向散射。对于漫发射的物体来说, 他们会变得更亮。因为更多光线朝着摄像机反射。对于阴影部分来说, 更少的光线朝上反射, 所以小球下方的大球区域会变得更加明亮。
