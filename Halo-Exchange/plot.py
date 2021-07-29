#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np
import matplotlib.pyplot as plt
import statistics


# In[2]:


def function(vec):
    lst1 = []
    lst2 = []
    lst3 = []
    lst4 = []
    lst5 = []
    lst6 = []
    lst7 = []
    ex = 0
    for i in vec:
        if ex % 7 == 0:
            lst1.append(i)
        elif ex % 7 == 1:
            lst2.append(i)
        elif ex % 7 == 2:
            lst3.append(i)
        elif ex % 7 == 3:
            lst4.append(i)
        elif ex % 7 == 4:
            lst5.append(i)
        elif ex % 7 == 5:
            lst6.append(i)
        elif ex % 7 == 6:
            lst7.append(i)
        ex=ex+1

    data = [np.log10(lst1),np.log10(lst2),np.log10(lst3),np.log10(lst4),np.log10(lst5),np.log10(lst6),np.log10(lst7)]
    return data


# In[3]:


file1 = open("data_16","r+")
sendrecv = []
packunpack = []
derived = []
c = 1
for i in file1:
    if c % 3 == 0:
        derived.append(float(i[5:13]))
    elif c % 3 == 1:
        sendrecv.append(float(i[5:13]))
    elif c % 3 == 2:
        packunpack.append(float(i[5:13]))
    c = c + 1
    
data1 = function(sendrecv)
data2 = function(packunpack)
data3 = function(derived)

red_square = dict(markerfacecolor='r')
green_square = dict(markerfacecolor='g')
blue_square = dict(markerfacecolor='b')

fig1, ax1 = plt.subplots()
ax1.set_title('Time Plot for Process 16')
plt.xlabel("Matrix Size")
plt.ylabel("Time(s)")
xx = ["16^2", "32^2", "64^2", "128^2", "256^2", "512^2", "1024^2"]

pmid = []
cmid = []
tmid = []
for i in range(7):
    pmid.append(float(statistics.median(data1[i])))
    cmid.append(float(statistics.median(data2[i])))
    tmid.append(float(statistics.median(data3[i])))

ax1.boxplot(data1 , flierprops=red_square, labels = xx)
ax1.plot(range(1,8), pmid, label = "MPI_Send/Recv")
plt.legend()


ax1.boxplot(data2, flierprops=green_square, labels = xx)
ax1.plot(range(1,8), cmid, label = "MPI_Pack/Unpack")
plt.legend()


ax1.boxplot(data3, flierprops=blue_square, labels = xx)
ax1.plot(range(1,8), tmid, label = "Derived_MPI_Type_Vector")
plt.legend()

plt.savefig('plot16.jpg')
plt.plot()


# In[4]:


file2 = open("data_36","r+")
sendrecv = []
packunpack = []
derived = []
c = 1
for i in file2:
    if c % 3 == 0:
        derived.append(float(i[5:13]))
    elif c % 3 == 1:
        sendrecv.append(float(i[5:13]))
    elif c % 3 == 2:
        packunpack.append(float(i[5:13]))
    c = c + 1
      
data1 = function(sendrecv)
data2 = function(packunpack)
data3 = function(derived)

red_square = dict(markerfacecolor='r')
green_square = dict(markerfacecolor='g')
blue_square = dict(markerfacecolor='b')

fig1, ax1 = plt.subplots()
ax1.set_title('Time Plot for Process 36')
plt.xlabel("Matrix Size")
plt.ylabel("Time(s)")
xx = ["16^2", "32^2", "64^2", "128^2", "256^2", "512^2", "1024^2"]

pmid = []
cmid = []
tmid = []
for i in range(7):
    pmid.append(float(statistics.median(data1[i])))
    cmid.append(float(statistics.median(data2[i])))
    tmid.append(float(statistics.median(data3[i])))

ax1.boxplot(data1 , flierprops=red_square, labels = xx)
ax1.plot(range(1,8), pmid, label = "MPI_Send/Recv")
plt.legend()


ax1.boxplot(data2, flierprops=green_square, labels = xx)
ax1.plot(range(1,8), cmid, label = "MPI_Pack/Unpack")
plt.legend()


ax1.boxplot(data3, flierprops=blue_square, labels = xx)
ax1.plot(range(1,8), tmid, label = "Derived_MPI_Type_Vector")
plt.legend()

plt.savefig('plot36.jpg')
plt.show()


# In[5]:


file3 = open("data_49","r+")
sendrecv = []
packunpack = []
derived = []
c = 1
for i in file3:
    if c % 3 == 0:
        derived.append(float(i[5:13]))
    elif c % 3 == 1:
        sendrecv.append(float(i[5:13]))
    elif c % 3 == 2:
        packunpack.append(float(i[5:13]))
    c = c + 1
    
data1 = function(sendrecv)
data2 = function(packunpack)
data3 = function(derived)

red_square = dict(markerfacecolor='r')
green_square = dict(markerfacecolor='g')
blue_square = dict(markerfacecolor='b')

fig1, ax1 = plt.subplots()
ax1.set_title('Time Plot for Process 49')
plt.xlabel("Matrix Size")
plt.ylabel("Time(s)")
xx = ["16^2", "32^2", "64^2", "128^2", "256^2", "512^2", "1024^2"]

pmid = []
cmid = []
tmid = []
for i in range(7):
    pmid.append(float(statistics.median(data1[i])))
    cmid.append(float(statistics.median(data2[i])))
    tmid.append(float(statistics.median(data3[i])))

ax1.boxplot(data1 , flierprops=red_square, labels = xx)
ax1.plot(range(1,8), pmid, label = "MPI_Send/Recv")
plt.legend()


ax1.boxplot(data2, flierprops=green_square, labels = xx)
ax1.plot(range(1,8), cmid, label = "MPI_Pack/Unpack")
plt.legend()


ax1.boxplot(data3, flierprops=blue_square, labels = xx)
ax1.plot(range(1,8), tmid, label = "Derived_MPI_Type_Vector")
plt.legend()

plt.savefig('plot49.jpg')
plt.show()


# In[6]:


file4 = open("data_64","r+")
sendrecv = []
packunpack = []
derived = []
c = 1
for i in file4:
    if c % 3 == 0:
        derived.append(float(i[5:13]))
    elif c % 3 == 1:
        sendrecv.append(float(i[5:13]))
    elif c % 3 == 2:
        packunpack.append(float(i[5:13]))
    c = c + 1
    
     
data1 = function(sendrecv)
data2 = function(packunpack)
data3 = function(derived)

red_square = dict(markerfacecolor='r')
green_square = dict(markerfacecolor='g')
blue_square = dict(markerfacecolor='b')

fig1, ax1 = plt.subplots()
ax1.set_title('Time Plot for Process 64')
plt.xlabel("Matrix Size")
plt.ylabel("Time(s)")
xx = ["16^2", "32^2", "64^2", "128^2", "256^2", "512^2", "1024^2"]

pmid = []
cmid = []
tmid = []
for i in range(7):
    pmid.append(float(statistics.median(data1[i])))
    cmid.append(float(statistics.median(data2[i])))
    tmid.append(float(statistics.median(data3[i])))

ax1.boxplot(data1 , flierprops=red_square, labels = xx)
ax1.plot(range(1,8), pmid, label = "MPI_Send/Recv")
plt.legend()


ax1.boxplot(data2, flierprops=green_square, labels = xx)
ax1.plot(range(1,8), cmid, label = "MPI_Pack/Unpack")
plt.legend()


ax1.boxplot(data3, flierprops=blue_square, labels = xx)
ax1.plot(range(1,8), tmid, label = "Derived_MPI_Type_Vector")
plt.legend()

plt.savefig('plot64.jpg')
plt.show()

