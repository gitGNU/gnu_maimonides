/*
 * Maimonides Suite v1.0.11 - advertisment/information player via embedded Raspberry Pi
 *
 *  Copyright 2012-2013 by it's authors. 
 *
 *  Some rights reserved. See COPYING, AUTHORS.
 *  This file may be used under the terms of the GNU General Public
 *  License version 3.0 as published by the Free Software Foundation
 *  and appearing in the file COPYING included in the packaging of
 *  this file.
 *
 *  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 *  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <gtk/gtk.h>
#include <cairo.h>
#include <wand/magick_wand.h>
#ifdef linux
#include <sys/types.h>
#include <dirent.h>
#endif

#include "carga_imagenes.h"

int isjpeg(char* name)
{
 int length;
  char end1[] = ".jpeg";
  char end2[] = ".JPEG";
  char* end[5];
 
  length = strlen(name);
  
  strcpy(end,&name[length-5]);

  //printf("comparando : %s con %s y %s \n",end,end1,end2);
  if(!strncmp(end,end1,5) || !strncmp(end,end2,5))
    return 1;
  else
    return 0;
}
int topng_3(char* name)
{
  MagickWand *mw = NULL;
 int length;

  MagickWandGenesis();
length = strlen(name);
  /* Create a wand */
  mw = NewMagickWand();

  /* Read the input image */
  MagickReadImage(mw,name);
  //setformat
  MagickSetImageFormat(mw,"png"); //capturamos algún tipo de error?¿?

  strncpy(&name[length-4],".png",4);
  //printf("writting image: %s \n",name);
  /* write it */
  MagickWriteImage(mw,name);

  /* Tidy up */
  if(mw) mw = DestroyMagickWand(mw);

  MagickWandTerminus();
}
int topng_4(char* name)
{
  MagickWand *mw = NULL;
 int length;

  MagickWandGenesis();

  /* Create a wand */
  mw = NewMagickWand();
  length = strlen(name);
  /* Read the input image */
  MagickReadImage(mw,name);
  //setformat
  MagickSetImageFormat(mw,"png"); //capturamos algún tipo de error?¿?

  strncpy(&name[length-5],".png\0",5);
  /* write it */
  MagickWriteImage(mw,name);

  /* Tidy up */
  if(mw) mw = DestroyMagickWand(mw);

  MagickWandTerminus();
}
int isjpg(char* name)
{
 int length;
  char end1[] = ".jpg";
  char end2[] = ".JPG";
  char* end[4];
 
  length = strlen(name);
  
  strcpy(end,&name[length-4]);

  // printf("compararlo con JPG me devuelve el valor %d \n",strncmp(end,end2,4));
  if(!strncmp(end,end1,4) || !strncmp(end,end2,4))
    return 1;
  else
    return 0;
}
int isgif(char* name)
{
 int length;
  char end1[] = ".gif";
  char end2[] = ".GIF";
  char* end[4];
 
  length = strlen(name);
  
  strcpy(end,&name[length-4]);

  //printf("comparando : %s con %s y %s \n",end,end1,end2);
  if(!strncmp(end,end1,4) || !strncmp(end,end2,4))
    return 1;
  else
    return 0;
}

int ispng(char* name)
{
 int length;
  char end1[] = ".png";
  char end2[] = ".PNG";
  char* end[4];
 
  length = strlen(name);
  
  strcpy(end,&name[length-4]);

  //printf("comparando : %s con %s y %s \n",end,end1,end2);
  if(!strncmp(end,end1,4) || !strncmp(end,end2,4))
    return 1;
  else
    return 0;
}

int	set_image_resolution(char* image_name,int width,int height)
{
 //imagemagick sollution
  MagickBooleanType status; 
  MagickWand *magick_wand;
  ////////////////////////
  //imagemagick_convertion
  MagickWandGenesis();
  magick_wand=NewMagickWand(); 
  status=MagickReadImage(magick_wand,image_name); 
  if (status == MagickFalse) 
    return -1;
  /* Turn the images into a thumbnail sequence. */ 
  MagickResetIterator(magick_wand); 
  while (MagickNextImage(magick_wand) != MagickFalse) 
    MagickResizeImage(magick_wand,width,height,LanczosFilter,1.0); 
  /* Write the image then destroy it. */ 
  status=MagickWriteImages(magick_wand,image_name,MagickTrue); 
  if (status == MagickFalse) 
    return -1;  //TEMPORAL
  magick_wand=DestroyMagickWand(magick_wand); 
  MagickWandTerminus();

  return 0;
}
#ifdef linux
int filtro_imagenes(strImagen** images_to_analyze, gchar* ruta)
{
  int images=0;
  int i = 0,is = 0;
  struct dirent *de;
  strImagen * imagenes_temp;
  DIR *pd;
  char nombre_completo[128];
  //abrimos y comprobamos el directorio suministrado
  pd = opendir(ruta);
  if(pd == NULL)
  {
    g_printf("ERROR: Couldn't open %s directory \n",ruta); //ERROR
    return(-1);
  }
  //validacion, y conversión y contabilización de los archivos
  //copiamos los nombres de los archivos de dentro del directorio
  while(de = readdir(pd))
    {
      if(strcmp(de->d_name,".") && strcmp(de->d_name,".."))
	{
	  strcpy(nombre_completo,ruta);
	  strcat(nombre_completo,"/");
	  strcat(nombre_completo,de->d_name);

	  printf("analizando %s \n",nombre_completo);
	  if(isjpg(nombre_completo))
	    {
	       printf("Es jpg!\n");
	       topng_3(nombre_completo);
	    }
	  if(isjpeg(nombre_completo))
	    {
	       printf("Es jpeg!\n");
	      topng_4(nombre_completo);
	    }
	  if(isgif(nombre_completo))
	    {
	       printf("Es gif!\n");
	      topng_3(nombre_completo);
	    }
	  printf("final %s \n",nombre_completo);
	  if(ispng(nombre_completo)) //is valid
	    {
	      printf("Es válida!\n");
	      images++;
	    }
	}
    }
  //construccion en emmoria
  printf("tantas imágenes %d \n",images);
  imagenes_temp  = malloc(images*sizeof(strImagen));
  closedir(pd);
  free(de);
  pd = opendir(ruta);
  is=0;
  while(de = readdir(pd))
    {
      if(strcmp(de->d_name,".") && strcmp(de->d_name,".."))
	{
	  strcpy(nombre_completo,ruta);
	  strcat(nombre_completo,"/");
	  strcat(nombre_completo,de->d_name);

	  if(ispng(nombre_completo)) //is valid
	    {
	      strcpy( imagenes_temp[i].name,nombre_completo);
	      printf("image loaded %s \n", imagenes_temp[i].name);
	      i++;
	    }
	  else
	    printf("no válido!!\n");	 
	}
    }
  *images_to_analyze = imagenes_temp;
  closedir(pd);
  return images;
}
//carga las imágenes del sistema de archivos local
//simplemente coge las imágenes del sistema de archivos local y las adapta a nuestras necesidades
int carga_imagenes_from_local (struct imagen** image_index)
{
  struct imagen * pt_imagenes;

  //adaptamos imágenes y las salvamos
  no_images = filtro_imagenes(&pt_imagenes,"./imagenes");
  *image_index = pt_imagenes;
  if(no_images >0)
    return 1;
  else
    {
      no_images = 0;
      return 0;
    }

}
int carga_imagenes_from_XML (struct imagen** image_index)
{
  int ret;
  struct imagen *image_index2;
  struct pista *audios_index2, *videos_index2;

  image_index2 = *image_index;
  //audios_index2 = *audios_index; 
  //videos_index2 = *videos_index;
  
  ret = cargar_XML_biblioteca (XML_MODE, image_index2,audios_index2,videos_index2);
  

  no_images = filtro_imagenes(&image_index,"./imagenes"); //retorna -1 si algo va mal
  printf("Hay %d numero de imágenes\n",no_images);

 //borrando
 if((system("rm  ./imagenes/*.gif")))
   printf("error borrando gifs o no hay gifs que covnertir! \n");
 if((system("rm  ./imagenes/*.GIF")))
   printf("error borrando gifs o no hay gifs que covnertir! \n");
 if((system("rm ./imagenes/*.jpg")))
   printf("error borrando jpgs o no hay jpgs que covnertir! \n");
 if((system("rm ./imagenes/*.JPG")))
   printf("error borrando jpgs o no hay jpgs que covnertir! \n");
 if((system("rm ./imagenes/*.jpeg")))
   printf("error borrando jpegs o no hay jpegs que borrar! \n");
 if((system("rm ./imagenes/*.JPEG")))
   printf("error borrando jpegs o no hay jpegs que borrar! \n");
  /////
 
  //cambiando resolución
  for(i=0;i<no_images;i++)
    {
      //NEW IMAGEMAGICK method
      //return -1 si hay error
      printf("cambiando resolucion a %s \n",image_index2[i].name);
      set_image_resolution(image_index2[i].name,1024,768);
      //////////////////////////////////////////////////
      image_index2[i].seconds = 2;
    }
  *image_index=image_index2;

}
int carga_imagenes (int MODE,struct imagen** image_index)
{
  int i,ret;

  struct imagen *image_index2;
 

  image_index2 = *image_index;

  switch(MODE)
    {
    case(FILES_MODE):
      ret = carga_imagenes_from_local(&image_index2);
      //borrando 
#ifdef linux
      //SOLO LINUX
      if((system("rm  ./imagenes/*.gif")))
	printf("error borrando gifs o no hay gifs que covnertir! \n");
      if((system("rm  ./imagenes/*.GIF")))
	printf("error borrando gifs o no hay gifs que covnertir! \n");
      if((system("rm ./imagenes/*.jpg")))
	printf("error borrando jpgs o no hay jpgs que covnertir! \n");
      if((system("rm ./imagenes/*.JPG")))
	printf("error borrando jpgs o no hay jpgs que covnertir! \n");
      if((system("rm ./imagenes/*.jpeg")))
	printf("error borrando jpegs o no hay jpegs que borrar! \n");
      if((system("rm ./imagenes/*.JPEG")))
	printf("error borrando jpegs o no hay jpegs que borrar! \n");
      /////
#endif
      //cambiando resolución
      for(i=0;i<no_images;i++)
	{
	  //NEW IMAGEMAGICK method
	  //return -1 si hay error
	  printf("cambiando resolucion a %s \n",image_index2[i].name);
	  set_image_resolution(image_index2[i].name,1024,768);
	  //////////////////////////////////////////////////
	  //un segundo, por defecto
	  image_index2[i].seconds = 1;
	}

      break;
    case(XML_MODE):
      ret = carga_imagenes_from_XML(&image_index2);
      break;
    default:
      return -1;
      break;
    }
  *image_index = image_index2;
  return ret;
  /*
  cargar_XML_biblioteca (XML_MODE, image_index,audios_index,videos_index);
  numero = 0;

  no_images = filtro_imagenes(&image_index,ruta); //retorna -1 si algo va mal
  printf("Hay %d numero de imágenes\n",no_images);

 //borrando
 if((system("rm  ./imagenes/*.gif")))
   printf("error borrando gifs o no hay gifs que covnertir! \n");
 if((system("rm  ./imagenes/*.GIF")))
   printf("error borrando gifs o no hay gifs que covnertir! \n");
 if((system("rm ./imagenes/*.jpg")))
   printf("error borrando jpgs o no hay jpgs que covnertir! \n");
 if((system("rm ./imagenes/*.JPG")))
   printf("error borrando jpgs o no hay jpgs que covnertir! \n");
 if((system("rm ./imagenes/*.jpeg")))
   printf("error borrando jpegs o no hay jpegs que borrar! \n");
 if((system("rm ./imagenes/*.JPEG")))
   printf("error borrando jpegs o no hay jpegs que borrar! \n");
  /////
 
  //cambiando resolución
  for(i=0;i<no_images;i++)
    {
      //NEW IMAGEMAGICK method
      //return -1 si hay error
      printf("cambiando resolucion a %s \n",image_index[i].name);
      set_image_resolution(image_index[i].name,1024,768);
      //////////////////////////////////////////////////
      image_index[i].seconds = 2;
    }
  if(audios_index != NULL)
    free(audios_index);
  if(image_index != NULL)
    free(image_index);
  if(videos_index != NULL)
    free(videos_index);
	//*/
  return 0;

}
#endif
