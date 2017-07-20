import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

n=302
x=30.0
y=30.0
c=1.0
dt=0.05
t1=1200
t2=600

inicial=np.zeros((n,n))
pasado=np.zeros((n,n))
presente=np.zeros((n,n))
futuro=np.zeros((n,n))

inicial[100,151]=-0.5

dx=x/299.0
dy=y/299.0
g1=(2*(c**2)*dt/dx)
g2=((c**2)*(dt**2)/(dx**2))
cte=((c*dt)/dx)
nueva=[]

def funcion(t):
    
    mask=np.ones((302,302))
    for i in range(1,n):
        for j in range(1,n):
            if(i==200):
                mask[i][j]=0
                for j in range(110,170):
                    mask[i][j] = 1
                    
    for i in range(299):
        for j in range(299):
            futuro[i,j]=g1*(inicial[i+1,j]-inicial[i-1,j])+g1*(inicial[i,j+1]-inicial[i,j-1])
    pasado=inicial.copy()
    presente=futuro.copy()*mask 
    nueva.append(presente)
    
    for k in range(1,t):
        for i in range(1,n-1):
            for j in range(1,n-1):
                futuro[i,j]=g2*(presente[i+1,j]-2*presente[i,j]+presente[i-1,j])+g2*(presente[i,j+1]-2*presente[i,j]+presente[i,j-1])+2*presente[i,j]-pasado[i,j]
                
        pasado=presente.copy()
        presente=futuro.copy()*mask 
        nueva.append(presente)
    return(presente,nueva)

plt.imshow(funcion(t1)[0],cmap="hot")
plt.title('Onda en 60s')
plt.savefig("tiempo=60s.png")
plt.close()

plt.imshow(funcion(t2)[0],cmap="hot")
plt.title('Onda en 30s')
plt.savefig("tiempo=30s.png")
plt.close()

a=funcion(t1)[1]
imagen=plt.figure()
b=plt.imshow(np.abs(a[0]),extent=(30+dx,30-dx,30+dy,30-dy))
plt.colorbar()

def animacion(tiempo):
    b.set_array(abs(a[tiempo]))
    return b

anima=animation.FuncAnimation(imagen,animacion, np.arange(1,len(nueva)), interval=20,blit=False)
anima.save("Onda.mp4")
    


