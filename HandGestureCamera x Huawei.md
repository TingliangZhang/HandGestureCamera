# HandGestureCamera
手势识别相机手环



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
