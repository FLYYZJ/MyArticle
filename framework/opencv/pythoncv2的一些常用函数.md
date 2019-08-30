# 亮度和对比度调整
```python
def Contrast_and_Brightness(self, alpha, beta, img):
    blank = np.zeros(img.shape, img.dtype)  # 掩模
    # dst = alpha * img + beta * blank
    dst = cv2.addWeighted(img, alpha, blank, 1 - alpha, beta)  # 将掩模赋值，通过赋权的方式加到img上
    return dst
```

按照一定的范围给图片进行亮度提升，其原理是将低于某像素值的像素点直接设置颜色为255（变成白色），从而提高整体的亮度
```python
def increase_brightness(img, value=30):
    hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    h, s, v = cv2.split(hsv)

    lim = 255 - value
    v[v > lim] = 255
    v[v <= lim] += value

    final_hsv = cv2.merge((h, s, v))
    img = cv2.cvtColor(final_hsv, cv2.COLOR_HSV2BGR)
    return img
```
直方图均衡化提高对比度
```python
cv.equalizeHist() # 全局均衡化
cv.createCLAHE([, clipLimit[, tileGridSize]]) # 局部均衡化，clipLimit表示均衡化,tileGridSize表示局部范围大小，clipLimit表示对比度大小

def equalHist_demo(image):
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
    dst = cv.equalizeHist(gray)
    cv.imshow("equal", dst)


def localizeEqual(image):
    gray = cv.cvtColor(image, cv.COLOR_BGR2GRAY)
    clahe = cv.createCLAHE(clipLimit=5.0, tileGridSize=(8, 8))
    dst = clahe.apply(gray)
    cv.imshow("myequal", dst)

```
![结合可视化工具进行图像的亮度校正](https://www.life2coding.com/change-brightness-and-contrast-of-images-using-opencv-python/)
```python
import cv2


def funcBrightContrast(bright=0):
    bright = cv2.getTrackbarPos('bright', 'Life2Coding')  # 调节条，bright为trackname，Life2Coding为要投放到的window的名称
    contrast = cv2.getTrackbarPos('contrast', 'Life2Coding')

    effect = apply_brightness_contrast(img, bright, contrast)
    cv2.imshow('Effect', effect)


def apply_brightness_contrast(input_img, brightness=255, contrast=127):
    brightness = map(brightness, 0, 510, -255, 255)
    contrast = map(contrast, 0, 254, -127, 127)

    if brightness != 0:
        if brightness > 0:
            shadow = brightness
            highlight = 255
        else:
            shadow = 0
            highlight = 255 + brightness
        alpha_b = (highlight - shadow) / 255
        gamma_b = shadow

        buf = cv2.addWeighted(input_img, alpha_b, input_img, 0, gamma_b)  # addWeighted函数是两个array 的加权和，alpha_b为src1的权重，beta=0为src2的权重，gamma为每个加权和结果还要加的一个scale
    else:
        buf = input_img.copy()

    if contrast != 0:
        f = float(131 * (contrast + 127)) / (127 * (131 - contrast))
        alpha_c = f
        gamma_c = 127 * (1 - f)

        buf = cv2.addWeighted(buf, alpha_c, buf, 0, gamma_c)

    cv2.putText(buf, 'B:{},C:{}'.format(brightness, contrast), (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
    return buf


def map(x, in_min, in_max, out_min, out_max):
    return int((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)  # 将亮度按照比例进行缩减


if __name__ == '__main__':
    original = cv2.imread(r"E:\EmpTools\OCR\error3\CSC033468725-2_1.bmp", 1)
    img = original.copy()

    cv2.namedWindow('Life2Coding', 1)

    bright = 255
    contrast = 127

    # Brightness value range -255 to 255
    # Contrast value range -127 to 127

    cv2.createTrackbar('bright', 'Life2Coding', bright, 2 * 255, funcBrightContrast)
    cv2.createTrackbar('contrast', 'Life2Coding', contrast, 2 * 127, funcBrightContrast)
    funcBrightContrast(0)
    cv2.imshow('Life2Coding', original)

cv2.waitKey(0)
```