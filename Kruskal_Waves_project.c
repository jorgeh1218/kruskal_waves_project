/* Proyecto de Elementos Discretos realizado por:
 * Jorge Hidalgo y Victor Tarkanyi Seccion 4*/
#include <stdio.h>
#include <string.h>
 #define CC 100 // Constante Numero de cuerpos celestes CC = 100 o CC= ? 
 ///////////////////////////////////////////////////////////////////
typedef struct
{
	char Nombre_Astro[40];
	char Tipo_Astro;
}Registro_astro;
typedef struct 
{
	float distanc;
	float rend;
}PesoMatAdy;
typedef struct
{
	float peso;
	int vertice1;
	int vertice2;
}Registro_Peso_Matriz	;
////////////////////////////////////////////////////////////////
typedef Registro_astro Vect_astros[CC];
typedef PesoMatAdy Matriz_Ady[CC][CC];
typedef int Matriz_Circuitos_Satelitales [2*CC][CC];
typedef float MatrizArbol[CC][CC];
typedef Registro_Peso_Matriz Vector_de_Satelites[(CC* (CC-1))/2];
typedef int Matriz_Ondas[CC][CC];
typedef int VectorVertices[CC];
typedef int Vertice;
typedef int MatrizCaminos[2*CC][CC]; // Puede tener problemas si hay muchas posibilidades de caminos se desbordara la matriz , solo aumentar el [2*CC] a un valor mas grande
//////////////////////////////////////////////////////////////////
int llenar_vectoreg(Vector_de_Satelites vectorsatelite,Matriz_Ady Matrizsatelites,int k);
void ordenar_vector(int k,Vector_de_Satelites vectorsatelite);
void extraer_etiqueta (int k,Vector_de_Satelites vectorsatelite,Matriz_Circuitos_Satelitales doble,MatrizArbol mat_result);
int busqueda_arcos(Matriz_Ady Matrizsatelites);
void AlgoritmoKruskal(Vect_astros vcelestes,Matriz_Ady Matrizsatelites,int cc,FILE *salida);
void AlgoritmoOndas(Matriz_Ady ady, Vect_astros vcelestes,int x,int y,int cc,float combust,FILE *salida);
int rodarfila(MatrizCaminos matriz_result, int filactual,int *filavacia,int columnact,int rueda_n);
void encontrar_menor_costo(Matriz_Ady ady,Vect_astros vcelestes ,MatrizCaminos matriz_result,int destino,int posdestino,float  combust,FILE *salida);
int busqueda_hacia_adelante(Vertice pos,Vertice destino,Matriz_Ady Adyac,Matriz_Ondas Onda,VectorVertices marcar, int *existcamino,int n_vertices);
int conjunto_ady_nom(int buscar,Matriz_Ady adyac, Matriz_Ondas Onda, int ind1,int ind2,int cantvertices,VectorVertices marcar);
void buscar_para_vert (Matriz_Ondas Onda, Matriz_Ady Ady, int vertice,int nvertices,int *filavacia,int columnavacia,MatrizCaminos matriz_result,int fila_actual,int columna_d_onda);
void construccion_hacia_atras(Vertice x,Vertice destino,Matriz_Ady Adyac,Matriz_Ondas Onda,int columna,MatrizCaminos matriz_result,int n_vertices,int posdestino);
///////////////////////////////*****//////////////////////******************///////////////////////////***********/////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 int main ( )
 {
	   FILE *entrada,*salida;
	 Vect_astros vcelestes; 
	 char cuerpo1[40],cuerpo2[40];
	 float d,r,combust;
	 int cc,i,m,j,celest1=-1,celest2=-1,h,auxa=0,auxa2=0;
	 Matriz_Ady adyac;
	 for(i=0;i<CC;i++)
	 {
			for(j=0;j<CC;j++)
			{
					adyac[i][j].distanc=-2;
					adyac[i][j].rend=-2;
			}
	}
	printf("\n*************************************");
	 printf("\n*       |BIENVENIDO(S) A ISA|       *");
	 printf("\n*          |WELCOME TO ISA|         *");
	 printf("\n*   |INTERNATIONAL SPACE AGENCY|    *");
	 printf("\n*   |AGENCIA ESPACIAL INTERNACIONAL|*");
	 printf("\n**************************************");
	 entrada= fopen("enterprise.in","r");
	 if(entrada==NULL)
	 printf("\n*|No hay  comunicacion con el archivo enterprise.in|* ");
	 salida= fopen("enterprise.out","w");
	 fscanf(entrada,"%d",&cc);// Numero de cuerpos celestes
	 for(i=0;i<cc;i++)
	 {
		 fscanf(entrada,"%s%*c%c",vcelestes[i].Nombre_Astro,&vcelestes[i].Tipo_Astro);
	}
	 fscanf(entrada,"%d",&m);
		 for(i=0;i<m;i++) // Lectura de rutas
		 {
				 fscanf(entrada,"%s%*c%s",cuerpo1,cuerpo2);
				 for(j=0;j<cc && celest1==-1;j++)
				{
						if(strcmp(cuerpo1,vcelestes[j].Nombre_Astro)==0)
						{
							celest1=j;
						}
				}
				if(celest1==-1)
				{
						printf("\nMala Comunicacion Espacial no existe un astro llamado %s en los Cuerpos Celestes conocidos",cuerpo1);
						getchar();
						getchar();
				}
				else
				{
					for(j=0;j<cc && celest2==-1;j++)
					{
							if(strcmp(cuerpo2,vcelestes[j].Nombre_Astro)==0)
							{
								celest2=j;
							}
					}
						if(celest2==-1)
					{
							printf("\nMala Comunicacion Espacial, Enterprise , no existe un astro llamado %s en los Cuerpos Celestes conocidos, presione una tecla",cuerpo2);
							getchar();
							getchar();
					}
				}
					if(celest2!=-1 && celest1!=-1)
					{
						fscanf(entrada,"%f",&d);
						fscanf(entrada,"%f",&r);
						if(d<0 || r<0)
						{
								printf("\nMala Comunicacion Espacial Enterprise señala que los datos de %.2f (distancia) y %.2f (rendimiento  de combustible) para Años Luz deben ser postivos, presione una tecla",d,r);
								getchar();
								getchar();					
						}
								adyac[celest1][celest2].distanc=d;
								adyac[celest1][celest2].rend=r;
								adyac[celest2][celest1].distanc= d;
								adyac[celest2][celest1].rend= r;
					}
					celest1=-1;
					celest2=-1;
					auxa=0;
					auxa2=0;
		  }
		  fscanf(entrada,"%d",&h);
		  for(i=0;i<h;i++)// Lectura de hypersaltos
		  {
				fscanf(entrada,"%s%*c%s",cuerpo1,cuerpo2);	  
				for(j=0;j<cc && celest1==-1;j++)
				{
					if(vcelestes[j].Tipo_Astro=='A')
					{
						if(strcmp(cuerpo1,vcelestes[j].Nombre_Astro)==0)
						{
							
							celest1=j;
						}
					}
				}
				if(celest1==-1)
				{
						printf("\nMala Comunicacion Espacial no existe un astro llamado %s en los Cuerpos Celestes conocidos",cuerpo1);
						getchar();
						getchar();
				}
				for(j=0;j<cc && celest2==-1;j++)
				{
					if(vcelestes[j].Tipo_Astro=='A')
					{
						if(strcmp(cuerpo2,vcelestes[j].Nombre_Astro)==0)
						{
							celest2=j;
						}
					}
				}
					if(celest2==-1)
				{
						printf("\nMala Comunicacion Espacial Enterprise señala que no existe un Agujero Negro llamado %s en los Cuerpos Celestes conocidos, presione una tecla",cuerpo2);
						getchar();
						getchar();
				}
					else
					{
						adyac[celest1][celest2].distanc= 0;
						adyac[celest2][celest1].distanc= 0;
						adyac[celest1][celest2].rend= 0;
						adyac[celest2][celest1].rend= 0;						
					}
		  }
		fprintf(salida,"Mapa de Interconexion satelital\n");
		AlgoritmoKruskal(vcelestes,adyac,cc,salida);								
		fscanf(entrada,"%d",&h);
		celest1=-1;
		celest2=-1;
		for(i=0;i<h;i++)
		{
			fscanf(entrada,"%s%*c%s%*c%f",cuerpo1,cuerpo2,&combust);	  // Lectura de Cuerpos para hallar ondas
			fprintf(salida,"\n Plan de viaje entre cuerpos celestes: %s  %s\n",cuerpo1,cuerpo2);
				 for(j=0;j<cc && celest1==-1;j++)
				{
						if(strcmp(cuerpo1,vcelestes[j].Nombre_Astro)==0)
						{
							celest1=j;
						}
						
				}
				for(j=0;j<cc && celest2==-1;j++)
				{
						if(strcmp(cuerpo2,vcelestes[j].Nombre_Astro)==0)
						{
							celest2=j;

						}
				}
			AlgoritmoOndas(adyac,vcelestes,celest1,celest2,cc,combust,salida);
			celest1=-1;
			celest2=-1;
		}
		printf("\n\n***||ISA: El Enterprise se encuentra en orbita, su historial de misiones esta en el archivo enterprise.out \n\n");
		fclose(salida);
		fclose(entrada);
		return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
void AlgoritmoKruskal(Vect_astros vcelestes,Matriz_Ady Matrizsatelites,int cc,FILE *salida)
{	
	Matriz_Circuitos_Satelitales doble;
	Vector_de_Satelites vectorsatelite;
	MatrizArbol mat_result;
	int i, n_arcos,k=0,j;
	for(i=0;i<CC;i++)// Asignacion
	{
			for(j=0;j<cc;j++)
			{
					if(i==0)
					{
							if(vcelestes[j].Tipo_Astro!= 'A')
						{
							doble[i][j]=j;
						}
						else
						{
							doble[i][j]=-2;
						}
					}
					else
					{
						doble[i][j]=-2;
					}
				}
	}
		for(i=0;i<cc;i++)
	{
		fprintf(salida,"%d %s\n",i,vcelestes[i].Nombre_Astro);
	}
	for(i=0;i<CC;i++)
	{
		for(j=0;j<CC;j++)
		{
			mat_result[i][j]=-2;
		}
	}
	n_arcos= busqueda_arcos(Matrizsatelites);
	k=llenar_vectoreg(vectorsatelite,Matrizsatelites,k);// k sera hasta donde está lleno el vector
	ordenar_vector(k,vectorsatelite);
	extraer_etiqueta(k,vectorsatelite,doble,mat_result);
	for(i=0;i<cc;i++)
	{
		for(j=0;j<cc;j++)
		{	
			if(mat_result[i][j]!=-2 && mat_result[i][j]!=0)
			{
				fprintf(salida," |*%.2f| ",mat_result[i][j]);
			}
			else
			{
				fprintf(salida," | -2 | ");
			}
		}
		fprintf(salida,"\n");
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AlgoritmoOndas(Matriz_Ady ady, Vect_astros vcelestes,int x,int y,int cc,float combust, FILE *salida)
{
	MatrizCaminos matriz_result;
    Matriz_Ondas ondas;
    VectorVertices marcas_vertices;
	 int n,i,posdestino=-2,j,n_ondas=0;
	for(i=0;i<CC;i++)
	{
			for(j=0;j<CC;j++)
			{
							ondas[i][j]=-2;
			}
	}
	ondas[0][0]= x;
	for(i=0;i<cc;i++)
	{
		marcas_vertices[i]=-2;
	}
	marcas_vertices[x] = 1;

	n_ondas=busqueda_hacia_adelante(x,y,ady,ondas,marcas_vertices,&posdestino,cc);
	if(posdestino!=-2)
	{
				for(i=0;i<2*CC;i++) // Llenando con destino la matriz de resultados asi se ahorra trabajo
		{
				for(j=0;j<cc;j++)
				{
					if(j==0)
					{
						matriz_result[i][j]=y;
					}
					else
					{
						matriz_result[i][j]=-2;
					}

				}
		}	
		construccion_hacia_atras(x,y,ady,ondas,posdestino-1,matriz_result,n,posdestino);

		encontrar_menor_costo(ady,vcelestes,matriz_result,x,posdestino,combust,salida);

	}
	else
	{
		fprintf(salida,"*|No existe camino seguro|*\n");
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int rodarfila(MatrizCaminos matriz_result, int filactual,int *filavacia,int columnact,int rueda_n)
{
	int i,j=1,encontrado;
	encontrado=1;
	*filavacia= *filavacia + rueda_n;
	for(i=*filavacia-1; i>=filactual; i--)
	{
		for(j=1;j<CC && encontrado!=0;j++)
		{
				if(matriz_result[i][j]!=-2)
				{
						if(i==filactual)
						{
							matriz_result[i+rueda_n][j]=matriz_result[i][j];
						}
						else
						{
							matriz_result[i+rueda_n][j]=matriz_result[i][j];
							matriz_result[i][j]=-2;
						}
				}
				else
				{
					encontrado=0;
				}	
		}
			encontrado=1;	
	}
	i=filactual;
	j=1;
	while(rueda_n > 0)
	{
			if(matriz_result[i][j]!=-2)
			{
				matriz_result[rueda_n+i][j]=matriz_result[i][j];
				j++;
			}
			else
			{
					j=1;
					rueda_n--;
			}
			
	}
	return *filavacia;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void encontrar_menor_costo(Matriz_Ady ady,Vect_astros vcelestes ,MatrizCaminos matriz_result,int destino,int posdestino,float  combust,FILE *salida)
{
	int i=0,j,fila_dcamin=0,extra;
	float aux=0,menor=9999999;
	while(matriz_result[i][posdestino]!=-2)
	{
		for(j=posdestino;j>0;j--)
		{
			
			aux= aux + (ady[matriz_result[i][j]][matriz_result[i][j-1]].rend);		
		}
				if(aux<menor)
				{
						menor= aux;
						fila_dcamin=i;
				}
					aux= 0;
					i++;
	}	
/*			for(j=posdestino;j>0;j--)
		{	
			if(ady[matriz_result[fila_dcamin][j]][matriz_result[fila_dcamin][j-1]].rend!=0 || ady[matriz_result[fila_dcamin][j]][matriz_result[fila_dcamin][j-1]].distanc!=0 )
			{
				comb_viaje=comb_viaje + ady[matriz_result[fila_dcamin][j]][matriz_result[fila_dcamin][j-1]].distanc/ ady[matriz_result[fila_dcamin][j]][matriz_result[fila_dcamin][j-1]].rend;
			}
		}*/
	if(menor>combust)// No se puede hacer el viaje
	{
			fprintf(salida,"*|El combustible dado no es suficiente|*\n");
	}
		for(j=posdestino;j>=0;j--)
		{
			extra=matriz_result[fila_dcamin][j];
			if(j==0)
			{	
				fprintf(salida," %s \n",vcelestes[extra].Nombre_Astro);
			}
				else
				{
					fprintf(salida," %s -",vcelestes[extra].Nombre_Astro);
				}
		}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void buscar_para_vert (Matriz_Ondas Onda, Matriz_Ady Ady, int vertice,int nvertices,int *filavacia,int columnavacia,MatrizCaminos matriz_result,int fila_actual,int columna_d_onda)
{
		int i=0,cont=1,encontrado=0,adyanum=0,controlcopy=0,begin=fila_actual,j,numondas=columna_d_onda+2,aux,auxvert=vertice;// porque se quitan el destino y los adyacentes por eso se sumo 2 para tener el numero de ondas
	while(Onda[i][columna_d_onda]!=-2)
		{
				if(Ady[vertice][Onda[i][columna_d_onda]].distanc!=-2)
					{
						adyanum++;
					}
					i++;
		}
		i=0;
		if(adyanum>1)
		{
			*filavacia=rodarfila(matriz_result,fila_actual,filavacia,columnavacia-1,adyanum-1);
		}
			while(Onda[i][columna_d_onda]!=-2 && columna_d_onda>=0 && cont <= nvertices)
			{
						if(Ady[vertice][Onda[i][columna_d_onda]].distanc!=-2)
					{					
						controlcopy++;		
									if(controlcopy>1)
									{
										fila_actual++;
									}
							while(matriz_result[fila_actual][columnavacia]==-2 && encontrado==0)
							{	
								if(matriz_result[fila_actual][columnavacia]==-2)
								{
									encontrado=1;
									matriz_result[fila_actual][columnavacia]= Onda[i][columna_d_onda];
									i++;			
								}				
								else
								{
										fila_actual= fila_actual + 1;
								}
								
							}
							
							encontrado=0;
					}
					else
					{
						i++;
					}	

					if(Onda[i][columna_d_onda]==-2 && columna_d_onda>=0) 
					{
						columna_d_onda--;
							if(columna_d_onda<0)
							{
								adyanum=0;
								begin++;
								fila_actual=begin;
								j=0;
								aux=numondas+1;
								while(matriz_result[begin][j]!=-2)
								{
									aux--;
										j++;
								}
								aux--; // por calculo de numero de espacios en matriz se  resto 
								columna_d_onda=aux;
								if(columna_d_onda <0)
								{
										columna_d_onda= columna_d_onda * -1;
								}
								columnavacia=j;
								vertice=matriz_result[begin][columnavacia-1];
								i=0;
								if(matriz_result[fila_actual][1]!=auxvert && cont <= nvertices)
								{
											auxvert=matriz_result[fila_actual][1];
											cont++;
								}
								while(Onda[i][columna_d_onda]!=-2 && cont<=nvertices)
								{
									if(Ady[vertice][Onda[i][columna_d_onda]].distanc!=-2)
									{
										adyanum++;
									}
									i++;
								}
								if(adyanum>1)
								{
									*filavacia=rodarfila(matriz_result,begin,filavacia,columnavacia-1,adyanum-1);
								}
								i=0;
								controlcopy=0;								
							}
							else
							{
								
								adyanum=0;
								controlcopy=0;
								i=0;

								vertice=matriz_result[begin][columnavacia];
								
									if(matriz_result[fila_actual][1]!=auxvert && cont <= nvertices)
								{
									auxvert=matriz_result[fila_actual][1];
									cont++;
								}
								while(Onda[i][columna_d_onda]!=-2)
								{
									if(Ady[vertice][Onda[i][columna_d_onda]].distanc!=-2 && cont<=nvertices)
									{
										adyanum++;
									}
									i++;
								}
								if(adyanum>1)
								{
									*filavacia=rodarfila(matriz_result,begin,filavacia,columnavacia-1,adyanum-1);
								}
								i=0;
								fila_actual= begin;
								columnavacia++;
							}
					}
			}		
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void construccion_hacia_atras(Vertice x,Vertice destino,Matriz_Ady Adyac,Matriz_Ondas Onda,int columna,MatrizCaminos matriz_result,int n_vertices,int posdestino)
{
	int i=0,k=0,t=1,auxt=0,aux2=0;

			while(Onda[i][columna]!=-2)
			{

				if(Adyac[destino][Onda[i][columna]].distanc!=-2) // Primera ronda de llenado solo la posicion anterior de ondas al destino 
				{
					matriz_result[k][t]=Onda[i][columna];
					k++;
				}
				i++;
			}
			columna--;
			i=0;
			aux2=k;
			auxt=t;	
			buscar_para_vert(Onda,Adyac,matriz_result[i][t],k,&aux2,2,matriz_result,0,columna);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int conjunto_ady_nom(int buscar,Matriz_Ady adyac, Matriz_Ondas Onda, int ind1,int ind2,int cantvertices,VectorVertices marcar)
{
	int aux=0,j,i;
	i=ind1;
	while(Onda[i][ind2]!= -2)
	{
		for(j=0;j<cantvertices;j++)
		{
			if((adyac[buscar][j].distanc!=-2)&&(marcar[j]==-2))
			{
				Onda[ind1][ind2+1]= j;
				marcar[j]=1;
				ind1++;
				aux++;
			}
		}

	if((j==cantvertices)&&(Onda[i+1][ind2]!=-2))
		{
			buscar= (Onda[i+1][ind2]);
			i++;
		}
		else
		{
			break;
		}
	}
	return aux;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int busqueda_hacia_adelante(Vertice pos,Vertice destino,Matriz_Ady Adyac,Matriz_Ondas Onda,VectorVertices marcar, int *existcamino,int n_vertices)
{
	int j,k,i,aux;
	j=0;
	i=0;
	aux=1;
	aux= aux  + conjunto_ady_nom(pos, Adyac,Onda,i,j,n_vertices,marcar);
	j++;	
	if(Onda[i][j] != -2)
	{
			while((Onda[i][j]!=-2 &&Onda[i][j]!=-1 )&&(marcar[destino]==-2))
			{
				j++;
				Onda[i][j]=-1;
				k= Onda[i][j-1];
				aux= aux * conjunto_ady_nom(k ,Adyac,Onda,i,j-1,n_vertices,marcar); 
				if(marcar[destino]!=-2)
				{
					*existcamino= j;
				}
			}
	}
	return aux;
}
/////////////////////////////////////////////////////////////////////////////////////////////
int llenar_vectoreg(Vector_de_Satelites vectorsatelite,Matriz_Ady Matrizsatelites,int k)
{
	int i,j;
	for(i=0;i<CC-1;i++)
	{	
			for(j=0;j<CC-1;j++)
			{
				if(i==j)
				{	
					while(j!=CC)
					{
							if(Matrizsatelites[i][j].distanc!=-2)
							{
								vectorsatelite[k].peso=Matrizsatelites[i][j].distanc;
								vectorsatelite[k].vertice1= i;
								vectorsatelite[k].vertice2= j;
								k++;
							}
							j++;
					}
				}
			}
	}
	return k;	
}
////////////////////////////////////////////////////////////////////////
void ordenar_vector(int k,Vector_de_Satelites vectorsatelite)
{
	int w,z,aux;
	for(w=0;w<k-1;w++)
	{
		for(z=w+1;z<k;z++)
		{
			if(vectorsatelite[w].peso>vectorsatelite[z].peso)
			{
				aux= vectorsatelite[w].peso;
				vectorsatelite[w].peso=vectorsatelite[z].peso;
				vectorsatelite[z].peso=aux;
				aux= vectorsatelite[w].vertice1;
				vectorsatelite[w].vertice1= vectorsatelite[z].vertice1;
				vectorsatelite[z].vertice1= aux;
				aux= vectorsatelite[w].vertice2;
				vectorsatelite[w].vertice2= vectorsatelite[z].vertice2;
				vectorsatelite[z].vertice2= aux;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void extraer_etiqueta (int k,Vector_de_Satelites vectorsatelite,Matriz_Circuitos_Satelitales doble,MatrizArbol mat_result)
{
	int i=0,j,encontrado,encontrado2,w=0,aux1,aux2,z=0;
	float aux3;
	i=0;
	while(i<k)// k es el num of vertices
	{
		encontrado=-1;
				aux1= vectorsatelite[i].vertice1;
				aux2= vectorsatelite[i].vertice2;
				aux3= vectorsatelite[i].peso;
									w=0;
									z=0;
									while(doble[w][aux1]!=-2 && encontrado!=-2)
									{
										j=z;
										while(doble[j][aux2]!=-2 && encontrado!=-2)
										{
											if(doble[j][aux2]==doble[w][aux1])
											{
												encontrado=-2;
											}
											j++;
										}
										w++;
									}
									if(doble[0][aux2]!=-2 && doble[0][aux1]!=-2 && encontrado!=-2)
									{
										mat_result[aux1][aux2]= aux3;
										mat_result[aux2][aux1]= aux3;
										z=1;
										encontrado2=-1;
										encontrado=-1;
										while(z<CC && (encontrado2==-1|| encontrado==-1))
										{
											if(doble[z][aux1]==-2 && encontrado==-1)
											{
												encontrado=z; // espacio vacio 
											}
												if(doble[z][aux2]==-2 && encontrado2==-1)
											{
												encontrado2=z; // espacio vacio de columna vert 2
											}
											z++;
										}
											z=encontrado;
											j=0;
											while(z<(encontrado + encontrado2+1))
											{
												doble[z][aux1]=doble[j][aux2];
												j++;
												z++;
											}
											j=0;
											z=encontrado2;
											while(z<(encontrado + encontrado2+1))
											{
												doble[z][aux2]=doble[j][aux1];
												j++;
												z++;
											}
									}
				 i++;								
	 }

}
//////////////////////////////////////////////////////////////
int busqueda_arcos(Matriz_Ady Matrizsatelites)
{
	int cant=0,i,j;
	for(i=0;i<CC-1;i++)
	{
		for(j=0;j<CC-1;j++)
		{
			if(i==j)
			{
				while(j!=CC-1)
				{
						if(Matrizsatelites[i][j].distanc!=-2 &&Matrizsatelites[i][j].distanc!=0)
						{
							cant ++;
						}
						j++;
				}
			}
		}
	}
	return cant;
}
