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
					// on déssine le rectangle
				}
				stop = 1;
			}
			j++;
		}
		stop = 0;
		ordre++;
	}
  	image_save (image, "circle.pgm");
}
/****************************************************************/


int main(int argc, char *argv[]) {

	Image *image;
	Shap tabShap[1000];
	int nbShap = 0;	
	Shap *circle1;
	int i = 0;
	

  	image = image_new (600, 600, 255,0);
	if(image->nbColor > image->backGroundColor)
  		image_fill (image, image->backGroundColor);

	for(i = 0; i < 4; i++){
		circle1 = shap_new (i,'d',100*i,100*i,100,0,0,250 - i*50);
		remplir_tab_Shap (&tabShap[nbShap],circle1,&nbShap);
	}


	//afficher_tab_Shap (image,tabShap,nbShap);

	//suppr_Shap (tabShap,&nbShap,3);

	//afficher_tab_Shap (image,tabShap,nbShap);

	//changer_ordre_figure (tabShap,nbShap,0,2);

	//afficher_tab_Shap (image,tabShap,nbShap);

	//modifier_figure (tabShap,nbShap,1);

	afficher_tab_Shap (image,tabShap,nbShap);

	genererImage(image,tabShap,nbShap);


  	image_free (image);

  return 0;

}













































