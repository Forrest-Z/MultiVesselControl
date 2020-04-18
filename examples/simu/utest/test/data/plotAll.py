#!/usr/bin/python
# -*- coding: UTF-8 -*-


import os
import numpy as np
import matplotlib.pyplot as plt
import csv

# 用来正常显示负号
plt.rcParams['axes.unicode_minus'] = False
# # 用来正常显示中文标签
# plt.rcParams['font.sans-serif'] = ['SimHei']

#创建一个文件夹放置图片
curPath=os.getcwd()
tempPath='image'
targetPath=curPath+os.path.sep+tempPath
if not os.path.exists(targetPath):
    os.makedirs(targetPath)
else:
    print('路径已经存在！')

#创建数据存储空间
time_head = []
head = []

time_target_head = []
target_head = []

time_controler_mz=[]
controler_mz=[]

time_controler_fx=[]
controler_fx=[]

time_controler_fy=[]
controler_fy=[]

time_tau=[]
tau_x=[]
tau_y=[]
tau_z=[]

time_BalphaU=[]
BalphaU_x=[]
BalphaU_y=[]
BalphaU_z=[]

time_xr=[]
x_r=[]

time_yr=[]
y_r=[]

time_UTM=[]
UTM_X=[]
UTM_Y=[]

time_target_UTM=[]
target_UTMx=[]
target_UTMy=[]

time_vel_x=[]
vel_x=[]

time_vel_y=[]
vel_y=[]

time_vel_head=[]
vel_head=[]


#导入数据##########################################################################
# 读取head文件
with open("kf_head_c.csv", 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    header_row = next(plots)
    for row in plots:
        time_head.append(float(row[0]))  
        head.append(float(row[1]))    
# 读取target head文件
with open("target_head.csv", 'r')as csvfile:
    plots_target = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_target)
    for row in plots_target:
        time_target_head.append(float(row[0]))
        target_head.append(float(row[1]))
# 读取controler_mz文件
with open("controller_mz.csv", 'r')as csvfile:
    plots_mz = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_mz)
    for row in plots_mz:
        time_controler_mz.append(float(row[0]))
        controler_mz.append(float(row[1]))
#读取目标力与实现力
#tau
with open("target_rotation_tau.csv", 'r')as csvfile:
    plots_tau = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_tau)
    for row in plots_tau:
        time_tau.append(float(row[0]))
        tau_x.append(float(row[1]))
        tau_y.append(float(row[2]))
        tau_z.append(float(row[3]))
#BalphaU
with open("target_rotation_BalphaU.csv", 'r')as csvfile:
    plots_BalphaU = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_BalphaU)
    for row in plots_BalphaU:
        time_BalphaU.append(float(row[0]))
        BalphaU_x.append(float(row[1]))
        BalphaU_y.append(float(row[2]))
        BalphaU_z.append(float(row[3]))

#读取x误差
with open("x_r.csv", 'r')as csvfile:
    plots_xr = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_xr)
    for row in plots_xr:
        time_xr.append(float(row[0]))
        x_r.append(float(row[1]))
#读取controller_x
with open("controller_fx.csv", 'r')as csvfile:
    plots_fx = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_fx)
    for row in plots_fx:
        time_controler_fx.append(float(row[0]))
        controler_fx.append(float(row[1]))
#读取y误差
with open("y_r.csv", 'r')as csvfile:
    plots_yr = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_yr)
    for row in plots_yr:
        time_yr.append(float(row[0]))
        y_r.append(float(row[1]))
#读取controller_y
with open("controller_fy.csv", 'r')as csvfile:
    plots_fy = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_fy)
    for row in plots_fy:
        time_controler_fy.append(float(row[0]))
        controler_fy.append(float(row[1]))
#读取UTMx
with open("UTMx.csv", 'r')as csvfile:
    plots_UTMx = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_UTMx)
    for row in plots_UTMx:
        time_UTM.append(float(row[0]))
        UTM_X.append(float(row[1]))
#读取UTMy
with open("UTMy.csv", 'r')as csvfile:
    plots_UTMy = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_UTMy)
    for row in plots_UTMy:
        # time_UTM.append(float(row[0]))
        UTM_Y.append(float(row[1]))
#读取target UTMx
with open("target_UTMx.csv", 'r')as csvfile:
    plots_target_UTMx = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_target_UTMx)
    for row in plots_target_UTMx:
        time_target_UTM.append(float(row[0]))
        target_UTMx.append(float(row[1]))
#读取target UTMy
with open("target_UTMy.csv", 'r')as csvfile:
    plots_target_UTMy = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_target_UTMy)
    for row in plots_target_UTMy:
        # time_target_UTM.append(float(row[0]))
        target_UTMy.append(float(row[1]))
#读取vel_x
with open("vel_x.csv", 'r')as csvfile:
    plots_vel_x = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_vel_x)
    for row in plots_vel_x:
        time_vel_x.append(float(row[0]))
        vel_x.append(float(row[1]))
#读取vel_y
with open("vel_y.csv", 'r')as csvfile:
    plots_vel_y = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_vel_y)
    for row in plots_vel_y:
        time_vel_y.append(float(row[0]))
        vel_y.append(float(row[1]))
#读取vel_head
with open("vel_head.csv", 'r')as csvfile:
    plots_vel_head = csv.reader(csvfile, delimiter=',')
    header_row = next(plots_vel_head)
    for row in plots_vel_head:
        time_vel_head.append(float(row[0]))
        vel_head.append(float(row[1]))

#画图###################################################################################
#figure 1 首向角相关内容
plt.figure(num=1, figsize=(10, 6))
plt.plot(time_head, head, label='head', color='b')#画首向角
plt.plot(time_target_head, target_head, label='target_head', color='r')#画目标航向角
# plt.plot(time_controler_mz, controler_mz, label='controller_mz', color='g')#画力矩
plt.xlabel('Time')
plt.ylabel('Head')
plt.title('Heading')
plt.legend()
# plt.xlim((0, 18))
# plt.ylim((0, 18))
# 坐标轴刻度
# my_x_ticks = np.arange(0, 15000, 2000)
# my_y_ticks = np.arange(2, 9, 2)
# plt.xticks(my_x_ticks)
# plt.yticks(my_y_ticks)
# plt.show()
plt.savefig('image/head.png', format='png',dpi=300)

#############################################################################################
#figure 2 目标力与实现力
fig=plt.figure(num=2, figsize=(10, 6))
style_list = ["b-", "r-", "b.-", "yo-"]
# style_list = ["g+-", "r*-", "b.-", "yo-"]
#使用subplots 画图
ax1=fig.add_subplot(311) #2*2的图形 在第一个位置
ax1.plot(time_tau, tau_x, style_list[0],label='target force')
ax1.plot(time_BalphaU, BalphaU_x, style_list[1],label='achieved force')
ax1.set_xlabel('Time')
ax1.set_ylabel('force\N')
plt.legend()
ax1.set_title('desire force & achieved force')

ax2=fig.add_subplot(312) #2*2的图形 在第一个位置
ax2.plot(time_tau,tau_y, style_list[0],label='target force')
ax2.plot(time_BalphaU,BalphaU_y, style_list[1],label='achieved force')
ax2.set_xlabel('Time')
ax2.set_ylabel('force\N')
plt.legend()

ax3=fig.add_subplot(313) #2*2的图形 在第一个位置
ax3.plot(time_tau, tau_z, style_list[0],label='target force')
ax3.plot(time_BalphaU, BalphaU_z, style_list[1],label='achieved force')
ax3.set_xlabel('Time')
ax3.set_ylabel('force\N*m')
plt.legend()
plt.savefig('image/tau_BalphaU.png', format='png',dpi=300)

###################################################################################################
#figure 3 三自由度误差与控制力
fig2=plt.figure(num=3, figsize=(12, 10))
style_list = ["b-", "r-", "b.-", "yo-"]
# style_list = ["g+-", "r*-", "b.-", "yo-"]
#使用subplots 画图
ax4=fig2.add_subplot(311) #2*2的图形 在第一个位置
ax4.plot(time_xr, x_r, style_list[0],label='x_error')
ax4.set_xlabel('Time')
ax4.set_ylabel('X error \ m')
ax4.set_title('position error & control force')
plt.grid(axis='y')
plt.legend()
plt.legend(loc='upper left', bbox_to_anchor=(0.03, 0.95))

ax5 = ax4.twinx()
ax5.plot(time_controler_fx, controler_fx, style_list[1],label='controller fx')
ax5.set_xlabel('Time')
ax5.set_ylabel('X Ctrl force \ N')
plt.legend()
plt.legend(loc='upper left', bbox_to_anchor=(0.16, 0.95))



ax6=fig2.add_subplot(312) #2*2的图形 在第一个位置
ax6.plot(time_yr, y_r, style_list[0],label='y_error')
ax6.set_xlabel('Time')
ax6.set_ylabel('Y error \ m')
# ax6.set_title('position error & control force')
plt.grid(axis='y')
plt.legend()
plt.legend(loc='upper left', bbox_to_anchor=(0.03, 0.95))

ax7 = ax6.twinx()
ax7.plot(time_controler_fy, controler_fy, style_list[1],label='controller fy')
ax7.set_xlabel('Time')
ax7.set_ylabel('Y Ctrl force \ N')
plt.legend()
plt.legend(loc='upper left', bbox_to_anchor=(0.16, 0.95))



ax8=fig2.add_subplot(313) #2*2的图形 在第一个位置
ax8.plot(time_head, head, style_list[0],label='head')
ax8.set_xlabel('Time')
ax8.set_ylabel('theta error \ m')
# ax8.set_title('position error & control force')
plt.grid(axis='y')
plt.legend()
plt.legend(loc='upper left', bbox_to_anchor=(0.03, 0.95))

ax9 = ax8.twinx()
ax9.plot(time_controler_mz, controler_mz, style_list[1],label='controller mz')
ax9.set_xlabel('Time')
ax9.set_ylabel('Z Ctrl force \ N*m')
plt.legend()
plt.legend(loc='upper left', bbox_to_anchor=(0.16, 0.95))
plt.savefig('image/error_controllerForce.png', format='png',dpi=300)

###################################################################################################
#figure 4 轨迹
plt.figure(num=4, figsize=(10, 6))
plt.plot(UTM_X, UTM_Y, color='b',label='raw')#
plt.plot(target_UTMx, target_UTMy, color='r',label='target',marker='*')#
# plt.plot(time_target_head, target_head, label='target_head', color='r')#画目标航向角
# plt.plot(time_controler_mz, controler_mz, label='controller_mz', color='g')#画力矩
plt.xlabel('UTM X')
plt.ylabel('UTM Y')
plt.title('trajectory')
plt.legend()
# plt.xlim((0, 18))
# plt.ylim((0, 18))
# 坐标轴刻度
# my_x_ticks = np.arange(0, 15000, 2000)
# my_y_ticks = np.arange(2, 9, 2)
# plt.xticks(my_x_ticks)
# plt.yticks(my_y_ticks)
# plt.show()
plt.savefig('image/trajectory.png', format='png',dpi=300)

###################################################################################################
#figure 5 轨迹误差
fig=plt.figure(num=5, figsize=(10, 6))
style_list = ["b-", "r-", "b.-", "yo-"]
# style_list = ["g+-", "r*-", "b.-", "yo-"]
#使用subplots 画图
ax1=fig.add_subplot(211) #2*2的图形 在第一个位置
ax1.plot(time_UTM, UTM_X, style_list[0],label='raw UTM X')
ax1.plot(time_target_UTM, target_UTMx, style_list[1],label='target_UTMx')
ax1.set_xlabel('Time')
ax1.set_ylabel('X \ m')
plt.legend()
ax1.set_title('desire UTM & achieved UTM')

ax2=fig.add_subplot(212) #2*2的图形 在第一个位置
ax2.plot(time_UTM, UTM_Y, style_list[0],label='raw UTM Y')
ax2.plot(time_target_UTM, target_UTMy, style_list[1],label='target_UTMy')
ax2.set_xlabel('Time')
ax2.set_ylabel('Y \ m')
plt.legend()
plt.savefig('image/trajectory_error.png', format='png',dpi=300)

###################################################################################################
#figure 6 三自由度速度
fig=plt.figure(num=6, figsize=(10, 6))
style_list = ["b-", "r-", "b.-", "yo-"]
# style_list = ["g+-", "r*-", "b.-", "yo-"]
#使用subplots 画图
ax1=fig.add_subplot(311) #2*2的图形 在第一个位置
ax1.plot(time_vel_x, vel_x, style_list[0],label='velocity surge')
ax1.set_xlabel('Time')
ax1.set_ylabel('Surge \ m/s')
plt.legend()
ax1.set_title('Velocity')

ax2=fig.add_subplot(312) #2*2的图形 在第一个位置
ax2.plot(time_vel_y, vel_y, style_list[0],label='velocity sway')
ax2.set_xlabel('Time')
ax2.set_ylabel('Sway \ m/s')
plt.legend()

ax2=fig.add_subplot(313) #2*2的图形 在第一个位置
ax2.plot(time_vel_head, vel_head, style_list[0],label='velocity yaw')
ax2.set_xlabel('Time')
ax2.set_ylabel('Yaw \ m/s')
plt.legend()


plt.savefig('image/Velocity_3DOF.png', format='png',dpi=300)