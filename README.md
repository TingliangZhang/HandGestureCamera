# HandGestureCamera


## 肌电技术方案

测试仪器：[LCR/阻抗分析仪](https://www.hioki.cn/html/methods/power.html)

LCR是单一频率下的阻抗分析仪



## 应用方向

AR手势识别

手语识别



健康管理：

Use [Skulpt](https://www.skulpt.me/) to measure your body fat percentage and get a personalized workout plan to burn fat and build muscle.



## 手势识别相机手环



使用两个位于手表位置的前视摄像头实时识别手势

用于AR设备的手势识别辅助



技术难点：

受限于手腕视角，只能看到骨骼突起和肌肉拉伸，以及偶尔出现的手指，做模式识别可能会比较困难，不同人可能要单独标定。

动态手势复杂度高，手指关节很多

手势数据集较少，我们这一特定的场景还需要单独采集数据集

手环所在位置一定程度上会影响识别的结果



技术方案：

数据集获取使用Leapmotion提取手势关节数据，结合两个摄像头采集到的信息作为训练集。

1.多模态输入

将若干帧光流图像连接在单张彩色图像的通道维度上，将其作为深度网络的输入，使得输入数据中既包含空间信息又携带时序信息，以此来实现动态手势识别

一个序列不同的模态携带不同的信息，RGB图片包含丰富的颜色和纹理信息，深度图片包含物体的轮廓和深度信息，光流分别描述了像素在x方向与y方向的运动信息。这些数据在信息描述上具有互补性，将它们融合起来，能够有效弥补单一模态数据的局限性。

基于RGB 序列和Depth序列和Flow序列的融合特征进行动态手势识别

2.手部检测器预处理

融合彩色和深度数据的双通道Faster R-CNN手部检测框架

3.基于人工特征的机器学习算法的动态手势建模

经典的方法有规整动态时间(Dynamic Time Warping, DTW)算法、隐马尔科夫模型(Hidden Markov Model, HMM)

这部分可以视能否直接得到手部数据来决定做不做



参考

https://image.hanspub.org/Html/19-1541767_35738.htm

https://www.hindawi.com/journals/tswj/2014/267872/



## 新的肌电方法

Tomo: Wearable, Low-Cost Electrical Impedance Tomography for Hand Gesture Recognition

https://dl.acm.org/doi/10.1145/2807442.2807480

金属电极+双选择器+网络分析仪芯片+ESP32+小锂电

https://www.youtube.com/watch?v=FqgwBc7cGZ8

能不能做连续的

下垂和举起手是否能够鲁棒识别

其他身体部分，比如袜子



这一作老哥后来还做了手表周围的另一只手的触摸识别

https://dl.acm.org/doi/10.1145/2984511.2984568

以及手指是否摸到皮肤的识别

https://dl.acm.org/doi/10.1145/3332165.3347869

和一些导电物体周围贴上电极的识别

https://dl.acm.org/doi/10.1145/3025453.3025842



## 新的想法-手语识别

目标是识别中文手语，关于中文手语的科普参考 [回形针Vol.154 手语的秘密](https://www.youtube.com/watch?v=vpkHTLhuDqg)

发现手语包括手部动作、肢体语言、面部表情

目前ASL图像识别方面做的比较好的是国外的[SignAll](https://www.signall.us/)，看图片是一个Kinect2和俩Realsense D435i，还有一个铁架子打光+架设摄像头，使用者要戴上一个五彩斑斓的手套。国内则是[腾讯优图发布的AI手语翻译机](https://cloud.tencent.com/developer/article/1430241)通过单摄像头识别，但是看起来需要很高的算力，DEMO用的是外星人的笔记本。

其实可以先做ABC的识别：这个是一个很傻的一个一个字母翻译的网页[English to Sign Language (ASL) Translator](https://wecapable.com/tools/text-to-sign-language-converter/)

[Hand Talk Translator](https://play.google.com/store/apps/details?id=br.com.handtalk)是逆向的转换，从文本到动漫人物动作的转换，国内类似产品是[搜狗AI开发平台手语数字人](https://ai.sogou.com/solution/signlanguage_digitalman/)

[手语也能机器翻译了！机器学习手势识别功能了解一下](https://blog.51cto.com/u_14815220/3451551) 华为机器学习（ML Kit）提供手部关键点识别服务，可用于手语识别。手部关键点识别服务能识别手部21个关键点，通过每个手指的方向和手语规则作比较去找手语字母表。

后期可以做根据国家通用手语词典来做8k+的词组识别。



申专利、发顶会、打比赛



## 竞品

[Tenzr: A Fun and Efficient Way to Regain Mobility and Function](https://tenzrhealth.com/) 主打手腕康复，就离谱，感觉是智商税，技术方案是IMU

[TENZR, the next generation gesture recognition wrist-band for VR AR, to be presented at CES 2018](https://www.prnewswire.com/news-releases/tenzr-the-next-generation-gesture-recognition-wrist-band-for-vr-ar-to-be-presented-at-ces-2018-300572362.html)

这个报告里面的BioInteractive Technologies Inc.和上一个是同一家智商税公司，所以这个只是个想法，没做。



[Myo Gesture Control Armband review](https://www.digitaltrends.com/pc-accessory-reviews/myo-gesture-control-armband-review/) EMG原理，很多年前就比较完善了，但是已经停产了xs，估计无了
