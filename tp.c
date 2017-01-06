#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int ORDRE_MAX = 0;

typedef struct {
	size_t width;
	size_t height;
	unsigned char nbColor;
	unsigned char backGroundColor;
	unsigned char *data;
} Image;

typedef struct {
	size_t ordre;
	unsigned char dr;
	size_t x;
	size_t y;
	size_t radius;
	size_t width;
	size_t height;
	unsigned char color;
} Shap;

static Image * image_new (size_t width,size_t height,unsigned char nbColor,unsigned char backGroundColor){
  	Image *image;

  	image = malloc (sizeof *image);
  	image->width = width;
  	image->height = height;
	image->nbColor = nbColor;
	image->backGroundColor = backGroundColor;
  	image->data = malloc (width * height*sizeof(char));

  return image;
}

static Shap * shap_new (size_t ordre,unsigned char dr,size_t x,size_t y,size_t radius,size_t width,size_t height,unsigned char color){
  	Shap *shap;

  	shap = malloc (sizeof *shap);
	shap->ordre = ordre;
	shap->dr = dr;
	shap->x = x;
	shap->y = y;
	shap->radius = radius; 
  	shap->width = width;
  	shap->height = height;
	shap->color = color;

	if(ORDRE_MAX < ordre) ORDRE_MAX = ordre;

  return shap;
}

static void image_free (Image *image){
	free (image->data);
  	free (image);
}

static void shap_free (Shap *shap){
  	free (shap);
}

static void image_fill (Image *image, unsigned char  value) {
  	memset (image->data, value, image->width * image->height);
}

static void image_save (const Image *image, const char  *filename){
  	FILE *out;

 	out = fopen (filename, "wb");
  	if (!out)
    	return;

  	fprintf (out, "P5\n");
  	fprintf (out, "%zu %zu\n", image->width, image->height);
  	fprintf (out, "255\n");

  	fwrite (image->data, 1, image->width * image->height, out);

  	fclose (out);
}

static void image_set_pixel (Image *image, ssize_t cx, ssize_t cy, ssize_t x, ssize_t y,ssize_t radius, unsigned char  value){
  	size_t tx, ty;
  	unsigned char *p;


  		tx = (radius) + x + cx;
  		ty = (radius) + y + cy;

  		p = image->data + (ty * image->width) + tx;
		*p = value;
}


static void draw_circle (Image *image, int cx, int cy, int radius,unsigned char  value){
	int x, y;
	
	if(cx >= (image->width - radius)) cx = (image->width - 2*radius) - 1; 
	if(cy >= (image->height - radius)) cy = (image->height - 2*radius) - 1;

  	for (y = -radius; y <= radius; y++)
    		for (x = -radius; x <= radius; x++)
      			if ((x * x) + (y * y) <= (radius * radius) )
        			image_set_pixel (image,cx,cy, x, y,radius, value);
}

/*** Ajout d'une figure **/
static void remplir_tab_Shap (Shap *tabShap,Shap *shap, int *nbShap){
	tabShap->ordre = shap->ordre;
	tabShap->dr = shap->dr;
	tabShap->x = shap->x;
	tabShap->y = shap->y;
	tabShap->radius = shap->radius; 
  	tabShap->width = shap->width;
  	tabShap->height = shap->height;
	tabShap->color = shap->color;
	(*nbShap)++;
}

/** Affichage de la liste des figures ***/ 
static void afficher_tab_Shap (Image *image,Shap tabShap[],size_t nbShap){

	int i = 0;

	printf(" %d %d %d %d \n",image->width,image->height,image->nbColor,image->backGroundColor);

	for(i = 0; i < nbShap; i++){
		if(tabShap[i].dr == 'd'){
			printf("%d) %c %d %d %d %d\n",tabShap[i].ordre,tabShap[i].dr,tabShap[i].x,tabShap[i].y,tabShap[i].radius,tabShap[i].color);
		}else if(tabShap[i].dr == 'r'){
			printf("%d) %c %d %d %d %d %d\n",tabShap[i].ordre,tabShap[i].dr,tabShap[i].x,tabShap[i].y,tabShap[i].width,tabShap[i].height,tabShap[i].color);
		}
	}
}

/** Supprimer une figure **/
static void suppr_Shap (Shap tabShap[],int *nbShap, size_t ordreFigure){
	int i = 0, j = 0;
	
	while( i <= *nbShap){
		if(ordreFigure == tabShap[i].ordre){
			for(j = i; j < *nbShap; j++){
				tabShap[i] = tabShap[i+1];
			}
			printf("\nLa figure est supprimée correctement.\n");
			(*nbShap)--;
			break;
		}
		i++;
	}
	if(i > *nbShap)
	printf("\nLa figure que vous voulez supprimer n'existe pas.\n");
	
}

static int existeOrdre(Shap tabShap[],size_t nbShap, int ordre){
	int i = 0;
	while( i < nbShap){
		if(ordre == tabShap[i].ordre){
			return i;
		}
		i++;
	}
	return -1;
}

/** Changer l'ordre **/
static void changer_ordre_figure ( Shap tabShap[],size_t nbShap, int ancienOrdre,int nvOrdre){

	int ind1, ind2;
	
	ind1 = existeOrdre(tabShap,nbShap,ancienOrdre);
	ind2 = existeOrdre(tabShap,nbShap,nvOrdre);

	if(ind1 != -1 && ind2 != -1){
		tabShap[ind1].ordre =  nvOrdre;
		tabShap[ind2].ordre =  ancienOrdre;
		printf("\nLa figure est reordonnée correctement.\n");
	}else printf("\nErreur dans la reordonnation !!!\n");

}
void dessiner_rect (char img[],int L,int H,int x , int y ,int l, int h,char c ){

	int ind =0;
	int yy=y*L;
	int debut= x+yy;
	int len = l*h;
	int j=0;
	while(1){
		for (ind=debut;ind<debut+l;ind++){
			img[ind]=c;	//dessin
		}
		j++;
		debut+=L;
		if(j==h) break;
	}

}
/*** mofidication d'une figure ******/
 static void modifier_figure ( Shap tabShap[],size_t nbShap, int Ordre){

	int ind1;
	int y = 0, r = 0, l = 0, h = 0, x = 0;
	unsigned char c;
	ind1 = existeOrdre(tabShap,nbShap,Ordre);


	if(ind1 != -1){
		if(tabShap[ind1].dr == 'd'){
			printf("Disque ancienne information : \t");
			printf("%d) %c %d %d %d %d\n",tabShap[ind1].ordre,tabShap[ind1].dr,tabShap[ind1].x,tabShap[ind1].y,tabShap[ind1].radius,tabShap[ind1].color);

			printf("Entree les nouveaux coordonnées du centre : \n\n");  
			printf("\tx = ");scanf("%d",&x);tabShap[ind1].x = x;
			printf("\ty = ");scanf("%d",&y);tabShap[ind1].y = y;

			printf("\n\nEntree le nouveau rayon : \n\n");
			printf("\tr = ");scanf("%d",&r);tabShap[ind1].radius = r;

			printf("\n\nEntree la nouvelle couleur : \n\n");
			printf("\tc = ");scanf("%d",&c);tabShap[ind1].color = c;
			printf("\n\n\n%d) %c %d %d %d %d\n",tabShap[ind1].ordre,tabShap[ind1].dr,tabShap[ind1].x,tabShap[ind1].y,tabShap[ind1].radius,tabShap[ind1].color);
		}else if(tabShap[ind1].dr == 'r'){
			printf("Rectangle ancienne information : \t");
			printf("%d) %c %d %d %d %d %d\n",tabShap[ind1].ordre,tabShap[ind1].dr,tabShap[ind1].x,tabShap[ind1].y,tabShap[ind1].width,tabShap[ind1].height,tabShap[ind1].color);
			printf("Entree les nouveaux coordonnées du sommet : \n\n");  
			printf("\tx = ");scanf("%d",&x);tabShap[ind1].x = x;
			printf("\ty = ");scanf("%d",&y);tabShap[ind1].y = y;

			printf("\n\nEntree la nouvelle hauteur : \n\n");
			printf("\th = ");scanf("%d",&h);tabShap[ind1].height = h;

			printf("\n\nEntree la nouvelle largeur : \n\n");
			printf("\tl = ");scanf("%d",&l);tabShap[ind1].width = l;

			printf("\n\nEntree la nouvelle couleur : \n\n");
			printf("\tc = ");scanf("%d",&c);tabShap[ind1].color = c;
			printf("%d) %c %d %d %d %d %d\n",tabShap[ind1].ordre,tabShap[ind1].dr,tabShap[ind1].x,tabShap[ind1].y,tabShap[ind1].width,tabShap[ind1].height,tabShap[ind1].color);
		}
		printf("\nLa figure est reordonnée correctement.\n");
	}else printf("\nErreur dans la reordonnation !!!\n");

}

/*** generer une image avec tout les figures ****/
static void genererImage(Image *image,Shap tabShap[],size_t nbShap){
	int ordre = 0,j = 0, stop = 0;
	
					
	while(ordre <= ORDRE_MAX){
		while(j  < nbShap && stop == 0){
			if(tabShap[j].ordre == ordre){
				if(tabShap[j].dr == 'd'){
					///on déssine le cercle 
					draw_circle (image,tabShap[j].x, tabShap[j].y, tabShap[j].radius,tabShap[j].color);
				}else{
					dessiner_rect (image->data,1000,1000,tabShap[j].x,tabShap[j].y,tabShap[j].width,tabShap[j].height,tabShap[j].color); 
				}
				stop = 1;
			}
			j++;
		}
		stop = 0;
		ordre++;
	}
  	image_save (image, "img.pgm");
}
/****************************************************************/
void pause(){
printf("Appuyer sur Entree pour continuer ");
getchar();
getchar();
system("clear");
}
void save_img(char datafile [],Image img,Shap tabShap [] ,int nbShap){
	FILE *fd;
	fd = fopen(datafile,"w");
		if(fd == NULL){
			printf("erreur lors de l'ouverture");
		}

int ordre = 0,j = 0, stop = 0;
	
	fprintf(fd,"%d %d %d %d \n", img.width,img.height,img.nbColor,img.backGroundColor);				
	while(ordre <= ORDRE_MAX){
		while(j  < nbShap && stop == 0){
			if(tabShap[j].ordre == ordre){
				if(tabShap[j].dr == 'd'){
					///on déssine le cercle 
					fprintf(fd,"d %d %d %d %d\n",tabShap[j].x,tabShap[j].y,tabShap[j].radius,tabShap[j].color);
				}else{
					fprintf(fd,"r %d %d %d %d %d\n",tabShap[j].x,tabShap[j].y,tabShap[j].width,tabShap[j].height,tabShap[j].color);
				}
				stop = 1;
			}
			j++;
		}
		stop = 0;
		ordre++;
	}
	fclose(fd);
}
	





FILE* load_img(char datafile [],Image **img,Shap tabShap [] ,int* nbShap){
	FILE *fd;
	int i=0,j=0;
	char line[100];
	int value[4];
	int val[6];
	Shap *rect ,*circle1;
int type = 0;
	fd = fopen(datafile,"r");
	if(fd == NULL){
		printf("erreur lors de l'ouverture");
		return fd;
	}
	fgets(line, 200, fd);
	
	char *token = strtok(line, " ");
        while(token != NULL) {
		value[i] = atoi(token);
		i++;
		token = strtok(NULL, " ");
	}
	*img = image_new (value[0], value[1], value[2],value[3]);
	if((*img)->nbColor > (*img)->backGroundColor)
  		image_fill ((*img), (*img)->backGroundColor);
	while(fgets(line,sizeof(line),fd)){
	    token = strtok(line, " ");
		j=0;
            while(token != NULL) {
	    	if(strcmp(token,"d")==0){
			type=1;
						
		}else if(strcmp(token,"r")==0) {
			type=2;
			
		}else{
			val[j]=atoi(token);
			j++;
		}
		token = strtok(NULL, " ");
	    }
            if(type==1){
		circle1 = shap_new (*nbShap,'d',val[0],val[1],val[2],0,0,val[3]);
remplir_tab_Shap (&tabShap[*nbShap],circle1,nbShap);
		}else{
		rect =shap_new (*nbShap,'r',val[0],val[1],0,val[2],val[3],val[4]);
		remplir_tab_Shap (&tabShap[*nbShap],rect,nbShap);	
	}
	    }	
	
	
}
int main(int argc, char *argv[]) {
	
	int tmp = 0,tmp2=0;
	char *t="";
	Image *image;
	Shap tabShap[1000];
	int nbShap = 0;	
	Shap *circle1;
	Shap *rect ;
	int i = 0;
	char nom[100];
	int imgH=1000,imgL=1000,nbC=255,backC=0;
	int y = 0, r = 0, l = 0, h = 0, x = 0;
	unsigned char c;
	
	load_img("test.txt",&image,tabShap,&nbShap);	
	
	
	
	//afficher_tab_Shap (image,tabShap,nbShap);

	//suppr_Shap (tabShap,&nbShap,3);

	//afficher_tab_Shap (image,tabShap,nbShap);

	//changer_ordre_figure (tabShap,nbShap,0,2);

	//afficher_tab_Shap (image,tabShap,nbShap);

	//modifier_figure (tabShap,nbShap,1);

	


	//genererImage(image,tabShap,nbShap);


  	//image_free (image);
/***************************************/
	int option=0;
	int op=0;
    	i=0;
 
    
 
    while(1){
	printf("****** Bienvenue ******\n\n"
           "Veuillez choisir une option :\n"
           "1: Affichage de la liste des figures\n"
           "2: Ajouter une figure \n"
           "3: Supprimer une figure\n"
           "4: Modifier une figure\n"
           "5: Changer l'ordre d'une figure\n"
           "6: Charger a partir d'un fichier \n"
	   "7: Sauvgarder sous forme victorielle \n"
	   "8: Sauvgarder sous forme matricielle \n"
           "9: Quitter\n");

    printf("\n\nEntrez votre choix: ");
        scanf("%1d", &option);           
        switch(option){
            case 1:                     
		printf("Affichage de la liste des figures\n");
		afficher_tab_Shap (image,tabShap,nbShap);
		printf("\n\n\n");

		pause();
            	break;
            case 2:
                printf("Ajout d'une figure\n");
		printf("1 : disque \n");
		printf("2: rectangle\n");

		printf("\n\nEntrez votre choix: ");
       		scanf("%d", &op);
		switch(op){
                	case 1: 
				printf("Entree les coordonnées du centre : \n\n");  

			printf("\tx = ");scanf("%d",&x);
			printf("\ty = ");scanf("%d",&y);

			printf("\n\nEntree le  rayon : \n\n");
			printf("\tr = ");scanf("%d",&r);

			printf("\n\nEntree la couleur : \n\n");
			printf("\tc = ");scanf("%d",&tmp);
			t=&tmp;
			c=*t;

			circle1 =shap_new (nbShap,'d',x,y,r,0,0,c);

		remplir_tab_Shap (&tabShap[nbShap],circle1,&nbShap);		

				break;
			
			case 2: 
				printf("Entree les coordonnées du sommet : \n\n");  
			printf("\tx = ");scanf("%d",&x);
			printf("\ty = ");scanf("%d",&y);
			printf("\n\nEntree la hauteur : \n\n");
			printf("\th = ");scanf("%d",&h);

			printf("\n\nEntree la largeur : \n\n");
			printf("\tl = ");scanf("%d",&l);

			printf("\n\nEntree la couleur : \n\n");
			printf("\tc = ");printf("\tc = ");scanf("%d",&tmp);
			t=&tmp;
			c=*t;
			rect =shap_new (nbShap,'r',x,y,0,l,h,c);
			remplir_tab_Shap (&tabShap[nbShap],rect,&nbShap);
				break;
		} 
	pause(); 
	   break;
	case 3 :
		printf("Suppression d'une figure\n");
		printf("Donner l'ordre de la figure a supprimer dans la liste des figures\n");
	scanf("%d",&tmp);
	suppr_Shap (tabShap,&nbShap,tmp);
pause();
	break;
	case 4 :
		printf("Modification d'une figure\n");
		printf("Donner l'ordre de la figure a modifier dans la liste des figures\n");
	scanf("%d",&tmp);
	modifier_figure (tabShap,nbShap,tmp);
	pause();
	break;
case 5 :
		printf("Changer l'ordre d'une figure\n");
		printf("Donner l'ordre actuel de la figures\n");
	scanf("%d",&tmp);
printf("Donner le nouveau ordre  de la figures\n");
	scanf("%d",&tmp2);
changer_ordre_figure (tabShap,nbShap,tmp,tmp2);
	pause();
	break;
	case 6 :
		printf ("Entrer le nom du fichier :  "); 
		scanf("%s",nom);
		nbShap=0;
		load_img(nom,&image,tabShap,&nbShap);
		break;
	case 7 :
		printf ("Entrer le nom du fichier :  "); 
		scanf("%s",nom);
		save_img(nom,*image,tabShap,nbShap);
		break; 
	case 8 :
		genererImage(image,tabShap,nbShap);
		break; 
	case 9 :
		image_free (image); return 0;
	break; 
        }
 
    }

  return 0;

}













































