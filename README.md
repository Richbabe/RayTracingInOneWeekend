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

## 10. Dielectrics
折射法则由Snell's Law定义：

$$ \eta · sin\theta = \eta' · sin\theta' $$

其中$ \theta $和$ \theta' $分别为入射光线和折射光线与法线方向的夹角，$ \eta $和$ \eta' $ (pronounced “eta” and “eta prime”) 为折射率（通常情况下，空气折射率为1.0，玻璃为1.3 - 1.7，钻石为2.4），如图：

![Image](https://github.com/Richbabe/RayTracingInOneWeekend/blob/main/Image/RayRefraction.png?raw=true)

为了计算出折射光线的方向，我们需要求解$ \theta' $：

$$  sin\theta’ = \frac{\eta}{\eta'} · sin\theta $$

在折射表面的一侧，假设折射光线为$ \pmb{R}' $，法线为$ \pmb{n}' $，且其夹角为$ \theta' $。那么我们可以把$ \pmb{R}' $分解成与$ \pmb{n}' $垂直和平行两部分:

$$ \pmb{R}' = \pmb{R}'_\bot + \pmb{R}'_{||} $$

然后我们需要求解$ \pmb{R}'_\bot $ 和 $ \pmb{R}'_{||} $：

$$ \pmb{R}'_\bot = \frac{\eta}{\eta'}(\pmb{R} + cos\theta \pmb{n}) $$

$$ \pmb{R}'_{||} = -\sqrt{1 - |\pmb{R}'_\bot|^2}\pmb{n} $$

然后我们要求解$ cos\theta $，已知两个向量点乘公式为:

$$ \pmb{a} · \pmb{b} = |\pmb{a}||\pmb{b}|cos\theta $$ 

我们把$ \pmb{a} $ 和 $ \pmb{b} $归一化成单位向量，即：

$$ \pmb{a} · \pmb{b} = cos\theta $$ 

于是$ \pmb{R}'_\bot $可表示为：

$$ \pmb{R}'_\bot = \frac{\eta}{\eta'}(\pmb{R} + (-\pmb{R} · \pmb{n}) \pmb{n}) $$

所以求$ \pmb{R}' $的代码如下：

![Image](https://github.com/Richbabe/RayTracingInOneWeekend/blob/main/Image/RefractionCode.png?raw=true)

当光线从高折射率介质向低折射率介质照射的时候，如果入射角度过大将不会产生折射，全部能量发生了反射，因为这一般发生在物体内部，也称为全内反射(total internal reflection)。

回顾Snell's law中求解$ Sin\theta' $:

$$  sin\theta’ = \frac{\eta}{\eta'} · sin\theta $$

如果Ray入射为玻璃，出射为空气时($ \eta = 1.5 $和$ \eta' = 1.0 $ )，那么有：

$$  sin\theta’ = \frac{1.5}{1.0} · sin\theta $$

又因为$ Sin\theta' <= 1.0 $，所以当：

$$ \frac{1.5}{1.0} · sin\theta > 1.0 $$

时（即入射角度$ theta $大于一定程度），方程无解，所以我们需要在计算折射光线之前进行全内反射的判定。

然而，现实中的玻璃在接近平行观察的时候，很难再通过玻璃表面观察到任何场景，此时玻璃表面完全呈现为一个镜面（菲涅尔现象）。这是因为真实界面处的透射率和反射率会随入射角的不同而产生巨大变化，该值的具体变化可以由Fresnel方程描述。

我们可以是用Schlick近似算法来判断观察点是否位于这个死角，如果是的话需要用反射替换折射。

考虑了全内反射和菲涅尔现象后的代码如下：

![Image](https://github.com/Richbabe/RayTracingInOneWeekend/blob/main/Image/RefractionWithReflection.png?raw=true)
