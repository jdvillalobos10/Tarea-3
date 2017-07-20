#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define G 4*pow(3.14159265359,2)
#define t 252
#define deltat 0.1

void almacenar(float *cifras, int columna, double Matriz[10][7]);
void Dezplazamiento(float *x, float *y, float *z, float *vx, float *vy, float *vz, float *masas, double t, double deltat);
void acelera(float *x, float *y, float *z, double *ax, double *ay, double *az, float *masas);

int main(void)
{    
    FILE *file; 

    file = fopen("coordinates.csv", "r");
    int len=300;
    char line_buffer[len];
    char *split_buffer;
    const char *delimiter;
    delimiter = ",";
    int i=0,j=0;
    double Matriz[10][7];
    while(fgets(line_buffer,len,file))
    {   j=0;
        //printf("LINE IS:%s", line_buffer );
        split_buffer = strtok(line_buffer,delimiter);
        while(split_buffer != NULL)
        {
            
            if(j!=0){
                Matriz[i][j-1] = atof(split_buffer);
                //printf("ITEM IN LINE: %s",split_buffer);
                split_buffer = strtok(NULL,delimiter);
            }
            j+=1;
        }
        
        i+=1;
    }
    
    
    //printf("%f\n", Matriz[1][1]);
    
double masa=Matriz[0][1];
    for(i=0;i<10;i++){
        
        //printf("%f\n",Matriz[i][1]);
        Matriz[i][1]=Matriz[i][1]/masa;
        //printf("%e\n\n", Matriz[i][1]);
        }

    
    int n=1000;
    float *masas=malloc((10*n)*sizeof(double));
    float *x=malloc((10*n)*sizeof(double));
    float *y=malloc((10*n)*sizeof(double));
    float *z=malloc((10*n)*sizeof(double));
    
    float *vx=malloc((10*n)*sizeof(double));
    float *vy=malloc((10*n)*sizeof(double));
    float *vz=malloc((10*n)*sizeof(double));
    
    float *ax=malloc(n*sizeof(double));
    float *ay=malloc(n*sizeof(double));
    float *az=malloc(n*sizeof(double));

    
    almacenar(masas, 1, Matriz);
    almacenar(x,2,Matriz);
    almacenar(y,3,Matriz);
    almacenar(z,4,Matriz);
    almacenar(vx,5,Matriz);
    almacenar(vy,6,Matriz);
    almacenar(vz,7,Matriz);
    //printf("%e\n", masas[2]);
    //printf("%e\n", x[0]);
    
    Dezplazamiento(x,y,z,vx,vy,vz,masas,1,0.1);
    
 return 0;
    
    
}

//Genero localmente la aceleración y posición para posteriormente ir cambiando y almacenando dichos datos
void acelera(float *x, float *y, float *z, double *ax, double *ay, double *az, float *masas){
    double matriz1[10][3];
    double magnitud;    
    double *distancia=malloc(3*sizeof(double));
    
    //con un doble for relleno lo que serán mis valores de distancia
    for (int j=0;j<3;j++){
        for(int i=0;i<10;i++){
            distancia[0]=x[j]-x[i];
            distancia[1]=y[j]-y[i];
            distancia[2]=z[j]-z[i];
            
            if(i!=j){
                //La magnitud es la raiz a la 3/2 de la suma de las distancias al cuadrado
                magnitud=pow(pow(distancia[0],2.0)+pow(distancia[1],2.0)+pow(distancia[2],2.0),0.5);
                magnitud=pow(magnitud,3.0);
            }
            //almaceno mis valores en x,y,z de acuerdo a la fórmula de aceleración gravitacional
            ax[i]+=distancia[0]*masas[j]*G/magnitud;
            ay[i]+=distancia[1]*masas[j]*G/magnitud;
            az[i]+=distancia[2]*masas[j]*G/magnitud;
        }
    }
        
    
    
}

//creo la funcion para almacenar correctamente los datos correspondientes en las matrices
void almacenar(float *cifras, int columna, double Matriz[10][7]){
    int i;
    for (i=0;i<10;i++){
        cifras[i]=Matriz[i][columna];
        
    }
}

//Este método de desplazamiento emplea el método de leapfrog como se utiliza en el ipynb del sistema solar
void desplazamiento(float *x, float *y, float *z, float *vx, float *vy, float *vz, float *masas, double t, double deltat){

    //defino mis variables temporales para calcular el desplaazmiento
    int puntos;
    puntos = (int)((t)/(deltat));
    double *uno=malloc((10)*sizeof(double));
    double *dos=malloc((10)*sizeof(double));
    double *tres=malloc((10)*sizeof(double));
    double *cuatro=malloc((10)*sizeof(double));
    double *cinco=malloc((10)*sizeof(double));
    double *seis=malloc((10)*sizeof(double));
    
  //creo una especie de .copy() en C
    int a;
    a=0;
    while(a<10){
        
        uno[a]=x[a];
        dos[a]=y[a];
        tres[a]=z[a];
        cuatro[a]=vx[a];
        cinco[a]=vy[a];
        seis[a]=vz[a];
        
        a++;
    }
   //creo los mallocs para ir reescribiendo las velocidades medias y aceleraciones 
    double *vx_media=malloc((10)*sizeof(double));
    double *vy_media=malloc((10)*sizeof(double));
    double *vz_media=malloc((10)*sizeof(double));
    double *ax_temporal=malloc((10)*sizeof(double));
    double *ay_temporal=malloc((10)*sizeof(double));
    double *az_temporal=malloc((10)*sizeof(double));
    
    int b;
    b=0;
    while(b<(puntos-1)){
        int c;
        c=0;
        acelera(x,y,z,ax_temporal,ay_temporal,az_temporal,masas);
        while(c<(10)){
            vx_media[c]=vx[c]*ax_temporal[c]*0.5*deltat; 
            vy_media[c]=vy[c]*ay_temporal[c]*0.5*deltat;
            vz_media[c]=vz[c]*az_temporal[c]*0.5*deltat;
            
        uno[c]+=deltat*vx[c];
        dos[c]+=deltat*vy[c];
        tres[c]+=deltat*vz[c];
            
            c++;
        }
        
        acelera(x,y,z,ax_temporal,ay_temporal,az_temporal,masas);
        int d;
        d=0;
        while(d<10){
            uno[d]=vx_media[d]*ax_temporal[c]*0.5*deltat;
            dos[d]=vy_media[d]*ay_temporal[c]*0.5*deltat;
            tres[d]=vz_media[d]*az_temporal[c]*0.5*deltat;
            
        }
        int e;
        e=0;
        while(e<10){
            x[e]=uno[e];
            y[e]=dos[e];
            z[e]=tres[e];
            vx[e]=cuatro[e];
            vy[e]=cinco[e];
            vz[e]=seis[e];
            
            e++;
        }
        b++;
    
}
    
}