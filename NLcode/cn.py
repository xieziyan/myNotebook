#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Dec 26 10:11:29 2019

@author: ziyan
"""

import cv2
import os
import copy
import numpy as np
import math
import pandas as pd
import regulation
from tqdm import tqdm

root_path = './dm/'
subfile = [] 
for item in os.listdir(root_path):
    path = os.path.join(root_path,item)
    subfile.append(path)

pos1_path = []
for item in os.listdir(subfile[0]):
    path = os.path.join(subfile[0],item)
    pos1_path.append(path)
    
testpos1_path = []
for item in os.listdir(subfile[1]):
    path = os.path.join(subfile[1],item)
    testpos1_path.append(path)
    
neg1_path = []
for item in os.listdir(subfile[2]):
    path = os.path.join(subfile[2],item)
    neg1_path.append(path)
    
testneg1_path = []
for item in os.listdir(subfile[3]):
    path = os.path.join(subfile[3],item)
    testneg1_path.append(path)
    
def binary(img):
    if len(img.shape)!=2:
        img = cv2.cvtColor(img,cv2.COLOR_RGB2GRAY)
    img = cv2.GaussianBlur(img, (5, 5), 0)
#    img, th2 = cv2.threshol,(img, 127, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    img = cv2.adaptiveThreshold(img, 255,cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 11, 2)
    img = cv2.fastNlMeansDenoising(img,None,10,7,11)
    return img
    
def stat(angle,weight): 
#    th=4
    st = np.zeros(18)
    for i in range(len(angle)):
#        if weight[i]-th>0:
#            w = weight[i]-th
#        else:
#            w = 0
        key = int(angle[i]//10)
        if key==18:
            key=0
        st[key]=st[key]+weight[i]
    return st

def houghtran(src):
    th=1.5
    angle = []
    weight = []
    draw = copy.deepcopy(src)
    bina = binary(src)
    img = cv2.Canny(bina,20,300,3)
    lines = cv2.HoughLinesP(img,1.0,np.pi/180,33,20,1)
    if isinstance(lines,np.ndarray):
        for x1, y1, x2, y2 in lines.reshape(-1,4):
            w =math.sqrt((x1-x2)**2+(y1-y2)**2)
            if w>=th:
                cv2.line(draw, (x1, y1), (x2, y2), (125, 0, 255), 1)
                agg = (math.atan2(y2-y1,x2-x1))/math.pi*180
                weight.append(w)
                angle.append(int(agg)+90)
#    cv2.imshow('bin',bina)
#    cv2.imshow('src',draw)
#    cv2.waitKey(0)
#        
    return angle,weight,draw

def get(img):
    angle,w,dr = houghtran(img)

    d = stat(angle,w)
    sumation = d.sum()
    d = d/sumation
    d = np.append(d,sumation).reshape(1,-1)
    
    return d,dr

def write_dic(path,names):
    item = path[0]
    img = cv2.imread(item)
    data = get(img)
    data = pd.DataFrame(data)
    data['name'] = item.split('/')[-1]
    for i in tqdm(range(1,len(path))):
        item = path[i]
        img = cv2.imread(item)
        da = get(img)
        da = pd.DataFrame(da)
        da['name'] = item.split('/')[-1]
        data = pd.concat([data,da],ignore_index=True)	
        
    data.to_csv('./{}.csv'.format(names),float_format='%.2f')


	
if __name__=='__main__':
    np.seterr(divide='ignore',invalid='ignore')
    testPath = neg1_path+pos1_path
    real = []
    predict = []
    for i in range(len(testPath)):
        if "POS" in testPath[i]:
            real.append(1)
        if "NEG" in testPath[i]:
            real.append(0)
        data,draw = get(cv2.imread(testPath[i]))
        result = regulation.classfication(data)
#        if(result==1):
#            print(testPath[i])
        
     