# notebook

##### yolov3的使用

##### 图像的旋转

##### 图像的角度计算

##### 高斯模糊

cv2.GaussianBlur

##### 图像的二值化算法：

包括OSTU算法（大津阈值法），

> cv2.threshold(img, 127, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)

自适应高斯阈值，自适应平均阈值。后两种是局部阈值方法。

> cv2.adaptiveThreshold(img, 255,cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 11, 2)

大津阈值法对于光照不同的图片处理效果不好。所以采用自适应的方法来进行操作。