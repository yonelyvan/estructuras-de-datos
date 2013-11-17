#include <iostream>
#include <fstream>
#include <allegro.h>  
#define filas 256
#define columnas 256

using namespace std;

template<class N>
class nodo{
        public:
        N color;//colores (0,1, ... ,255)
        int x1, y1, x2, y2;//coordenadas de inicio  y final
        nodo<N> *primero; 
        nodo<N> *segundo;
        nodo<N> *tercero;
        nodo<N> *cuarto;
        
        public:
        nodo<N>(int x_1, int y_1, int x_2, int y_2){//por ahora solo potencia de 2 
                color = (-1);
                x1=x_1;    y1=y_1;
        x2=x_2;    y2 =y_2;
                primero = segundo = tercero = cuarto = NULL;
                }
        
        };
        
template<class T>
class cuaternario{
        public:
                        nodo<T> *root;
                        int matrix[filas][columnas];
                        int fils,cols,profundidad;///datos de l imgen 
                        int buffer[filas][columnas];
                        
        public:
                        cuaternario(int dimencion){root= new nodo<T>(0,0,dimencion,dimencion); }
                        void cargar_imagen();
                        void mostrar_imagen();
                        
                        void add(nodo<T> *&n);
                        bool igualdad(int x1, int y1, int x2, int y2);
                        void print_archivo(nodo<T> *&n);
                        void print_imagen(nodo<T> *&n);
        };

template<class T>
void cuaternario<T>::cargar_imagen(){
        ifstream file("avril.pgm"); ///nombre de la imagen
        char basura[100];
                file.getline(basura,100); /// descartamos informacion basura
                file.getline(basura,100);
                file>>cols>>fils;
                file>>profundidad;
                
        for(int i=0;i<filas;i++){    ///leemos la matriz
                for(int j=0;j<columnas;j++){
                        file>>matrix[j][i];
                        }
                }
          file.close();
        }        

template<class T>
bool cuaternario<T> :: igualdad(int x1, int y1, int x2, int y2){
        int m=matrix[x1][y1],c=x2-x1;
        for(int i=0;i<c;i++){
                for(int j=0;j<c;j++){
                  if(matrix[x1+i][y1+j] != m){return false;}
             }                
        }
        return true;
}

template <class T>
void cuaternario<T> :: add(nodo<T> *&n){
         nodo<T> *tem = n;
         if(igualdad(tem->x1, tem->y1, tem->x2, tem->y2)){tem->color=matrix[tem->x1][tem->y1];return;}
     
         int x = tem->x1 + ((tem->x2 - tem->x1)/2);
         int y = tem->y1 + ((tem->y2 - tem->y1)/2);
     
         nodo<T> *a =new nodo<T>(tem->x1, tem->y1, x, y);//(0,0,2^n,2^n)
         nodo<T> *b =new nodo<T>(x, tem->y1, tem->x2, y);
         nodo<T> *c =new nodo<T>(tem->x1, y, x, tem->y2);
         nodo<T> *d =new nodo<T>(x, y, tem->x2, tem->y2);
         
         tem->primero =a; 
         tem->segundo =b; 
         tem->tercero =c; 
         tem->cuarto  =d; 
          
         add(tem->primero);
         add(tem->segundo);
         add(tem->tercero); 
         add(tem->cuarto);
        }

template <class T>        
void cuaternario<T>::print_archivo(nodo<T> *&n){
        if(!n){return;}
        if(n->color !=(-1)){//cout<<n->color<<" coordenadas : "<<n->x1<<","<<n->y1<<","<<n->x2<<","<<n->y2<<endl;
        ofstream archivo;
        archivo.open("cuaternario.txt",ios::app);
        archivo<<n->color<<" "<<n->x1<<" "<<n->y1<<" "<<n->x2<<" "<<n->y2<<endl;
        archivo.close();
        }
        
        print_archivo(n->primero);
        print_archivo(n->segundo);
        print_archivo(n->tercero);
        print_archivo(n->cuarto);
        }

template<class T>
void cuaternario<T>::print_imagen(nodo<T> *&n){
        if(!n){return;}
        if(n->color !=(-1)){
             for(int i=n->x1; i<n->x2; i++){
                    for(int j=n->y1; j<n->y2; j++){
                        buffer[i][j] = n->color;
                     }
             }
        }
        
        print_imagen(n->primero);
        print_imagen(n->segundo);
        print_imagen(n->tercero);
        print_imagen(n->cuarto );
}
      
template<class T>
void cuaternario<T>::mostrar_imagen(){
        BITMAP *mapa = create_bitmap(400, 400);  
        clear(mapa); 
        for(int i=0;i<filas;i++){
                for(int j=0;j<columnas;j++){
                putpixel(mapa,i,j,makecol(buffer[i][j],buffer[i][j],buffer[i][j]));
                }
        }
        blit(mapa, screen, 0, 0, 0, 0, 400, 400);
        destroy_bitmap(mapa);
        
        }


        
int main(){
  allegro_init();  
  install_keyboard();  
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 400, 400, 0, 0); 

  cuaternario<int> *avril=new cuaternario<int>(256);
  avril->cargar_imagen();   

  avril->add(avril->root);            /// se crea el arbol
  avril->print_archivo(avril->root); ///imprime arbol en un archivo

  avril->print_imagen(avril->root);  ///imprime arbol en un buffer
  avril->mostrar_imagen();           ///muestra buffer

  readkey(); 
  return 0;
}
END_OF_MAIN();
