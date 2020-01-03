#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Dec 26 16:20:03 2019

@author: ziyan
"""

import sklearn.metrics as evl
import regulation
import cv2
from cn import get
from sklearn.metrics import classification_report
from tqdm import tqdm
def recall(path,ant=1,th=0.7):
    predict = []
    real = []
    for i in tqdm(range(len(path))):
        d,draw = get(cv2.imread(path[i]))
        if ant == 1:
            ss = regulation.oneDirection(d,th)
        if ant ==2:
            ss = regulation.directionNum(d,th)
        if ant ==3:
            ss = regulation.linesLength(d)
        if ant ==4:
            ss = regulation.straightAngle(d,th)
        predict.append(ss)
        if "POS" in path[i]:
            real.append(1)
        if "NEG" in path[i]:
            real.append(0)
#    target_names = ['neg','pos']
#    print(real)
#    print(predict)
    print('\n')
    print(classification_report(real,predict))
#    recall = evl.recall_score(real,predict)
#    acc = evl.accuracy_score(real,predict)
#    return  recall,acc