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
ID_state=[]
time = []
x=[]
y=[]
head = []

#导入数据##########################################################################
# 读取head文件
with open("state.csv", 'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    header_row = next(plots)
    for row in plots:
    	ID_state.append(float(row[0])) 
        time.append(float(row[1]))  
        x.append(float(row[2]))  
        y.append(float(row[3])) 
        head.append(float(row[4]))   


#画图###################################################################################
#figure 1 首向角相关内容
plt.figure(num=1, figsize=(10, 6))
plt.plot(ID_state, head, label='head', color='b')#画首向角
#plt.plot(time_target_head, target_head, label='target_head', color='r')#画目标航向角
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

# #############################################################################################
# #figure 2 目标力与实现力
# fig=plt.figure(num=2, figsize=(10, 6))
# style_list = ["b-", "r-", "b.-", "yo-"]
# # style_list = ["g+-", "r*-", "b.-", "yo-"]
# #使用subplots 画图
# ax1=fig.add_subplot(311) #2*2的图形 在第一个位置
# ax1.plot(time_tau, tau_x, style_list[0],label='target force')
# ax1.plot(time_BalphaU, BalphaU_x, style_list[1],label='achieved force')
# ax1.set_xlabel('Time')
# ax1.set_ylabel('force\N')
# plt.legend()
# ax1.set_title('desire force & achieved force')

# ax2=fig.add_subplot(312) #2*2的图形 在第一个位置
# ax2.plot(time_tau,tau_y, style_list[0],label='target force')
# ax2.plot(time_BalphaU,BalphaU_y, style_list[1],label='achieved force')
# ax2.set_xlabel('Time')
# ax2.set_ylabel('force\N')
# plt.legend()

# ax3=fig.add_subplot(313) #2*2的图形 在第一个位置
# ax3.plot(time_tau, tau_z, style_list[0],label='target force')
# ax3.plot(time_BalphaU, BalphaU_z, style_list[1],label='achieved force')
# ax3.set_xlabel('Time')
# ax3.set_ylabel('force\N*m')
# plt.legend()
# plt.savefig('image/tau_BalphaU.png', format='png',dpi=300)

# ###################################################################################################
# #figure 3 三自由度误差与控制力
# fig2=plt.figure(num=3, figsize=(12, 10))
# style_list = ["b-", "r-", "b.-", "yo-"]
# # style_list = ["g+-", "r*-", "b.-", "yo-"]
# #使用subplots 画图
# ax4=fig2.add_subplot(311) #2*2的图形 在第一个位置
# ax4.plot(time_xr, x_r, style_list[0],label='x_error')
# ax4.set_xlabel('Time')
# ax4.set_ylabel('X error \ m')
# ax4.set_title('position error & control force')
# plt.grid(axis='y')
# plt.legend()
# plt.legend(loc='upper left', bbox_to_anchor=(0.03, 0.95))

# ax5 = ax4.twinx()
# ax5.plot(time_controler_fx, controler_fx, style_list[1],label='controller fx')
# ax5.set_xlabel('Time')
# ax5.set_ylabel('X Ctrl force \ N')
# plt.legend()
# plt.legend(loc='upper left', bbox_to_anchor=(0.16, 0.95))



# ax6=fig2.add_subplot(312) #2*2的图形 在第一个位置
# ax6.plot(time_yr, y_r, style_list[0],label='y_error')
# ax6.set_xlabel('Time')
# ax6.set_ylabel('Y error \ m')
# # ax6.set_title('position error & control force')
# plt.grid(axis='y')
# plt.legend()
# plt.legend(loc='upper left', bbox_to_anchor=(0.03, 0.95))

# ax7 = ax6.twinx()
# ax7.plot(time_controler_fy, controler_fy, style_list[1],label='controller fy')
# ax7.set_xlabel('Time')
# ax7.set_ylabel('Y Ctrl force \ N')
# plt.legend()
# plt.legend(loc='upper left', bbox_to_anchor=(0.16, 0.95))



# ax8=fig2.add_subplot(313) #2*2的图形 在第一个位置
# ax8.plot(time_head, head, style_list[0],label='head')
# ax8.set_xlabel('Time')
# ax8.set_ylabel('theta error \ m')
# # ax8.set_title('position error & control force')
# plt.grid(axis='y')
# plt.legend()
# plt.legend(loc='upper left', bbox_to_anchor=(0.03, 0.95))

# ax9 = ax8.twinx()
# ax9.plot(time_controler_mz, controler_mz, style_list[1],label='controller mz')
# ax9.set_xlabel('Time')
# ax9.set_ylabel('Z Ctrl force \ N*m')
# plt.legend()
# plt.legend(loc='upper left', bbox_to_anchor=(0.16, 0.95))
# plt.savefig('image/error_controllerForce.png', format='png',dpi=300)

###################################################################################################
#figure 4 轨迹
plt.figure(num=4, figsize=(10, 6))
plt.plot(x, y, color='b',label='raw')#
#plt.plot(target_UTMx, target_UTMy, color='r',label='target',marker='*')#
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
# #figure 5 轨迹误差
# fig=plt.figure(num=5, figsize=(10, 6))
# style_list = ["b-", "r-", "b.-", "yo-"]
# # style_list = ["g+-", "r*-", "b.-", "yo-"]
# #使用subplots 画图
# ax1=fig.add_subplot(211) #2*2的图形 在第一个位置
# ax1.plot(time_UTM, UTM_X, style_list[0],label='raw UTM X')
# ax1.plot(time_target_UTM, target_UTMx, style_list[1],label='target_UTMx')
# ax1.set_xlabel('Time')
# ax1.set_ylabel('X \ m')
# plt.legend()
# ax1.set_title('desire UTM & achieved UTM')

# ax2=fig.add_subplot(212) #2*2的图形 在第一个位置
# ax2.plot(time_UTM, UTM_Y, style_list[0],label='raw UTM Y')
# ax2.plot(time_target_UTM, target_UTMy, style_list[1],label='target_UTMy')
# ax2.set_xlabel('Time')
# ax2.set_ylabel('Y \ m')
# plt.legend()
# plt.savefig('image/trajectory_error.png', format='png',dpi=300)

# ###################################################################################################
# #figure 6 三自由度速度
# fig=plt.figure(num=6, figsize=(10, 6))
# style_list = ["b-", "r-", "b.-", "yo-"]
# # style_list = ["g+-", "r*-", "b.-", "yo-"]
# #使用subplots 画图
# ax1=fig.add_subplot(311) #2*2的图形 在第一个位置
# ax1.plot(time_vel_x, vel_x, style_list[0],label='velocity surge')
# ax1.set_xlabel('Time')
# ax1.set_ylabel('Surge \ m/s')
# plt.legend()
# ax1.set_title('Velocity')

# ax2=fig.add_subplot(312) #2*2的图形 在第一个位置
# ax2.plot(time_vel_y, vel_y, style_list[0],label='velocity sway')
# ax2.set_xlabel('Time')
# ax2.set_ylabel('Sway \ m/s')
# plt.legend()

# ax2=fig.add_subplot(313) #2*2的图形 在第一个位置
# ax2.plot(time_vel_head, vel_head, style_list[0],label='velocity yaw')
# ax2.set_xlabel('Time')
# ax2.set_ylabel('Yaw \ m/s')
# plt.legend()


# plt.savefig('image/Velocity_3DOF.png', format='png',dpi=300)