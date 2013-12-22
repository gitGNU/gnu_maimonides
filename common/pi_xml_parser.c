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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>
#include <libxml/tree.h>


#include"pi_xml_parser.h"
#include"globales.h"
//#if defined(LIBXML_WRITER_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)


static void
processNode(xmlTextReaderPtr reader, int *section, 
	    int *subsection,int *parameter,struct imagen **image_index, 
	    struct pista **audios_index,struct pista **videos_index,
	    struct glob_settings** settings) 
{
    xmlChar *name, *value;
    int index;
    int k;
    struct imagen *image_index2 = NULL;
    struct pista *audios_index2 = NULL,*videos_index2 = NULL;
    struct glob_settings* settings2 = NULL;

    image_index2 = *image_index;
    audios_index2 = *audios_index;
    videos_index2 = *videos_index;
    settings2  =*settings;

    //printf("principio de secuencia %d \n",*subsection); 
    name = xmlTextReaderConstName(reader);
    if (name == NULL)
	name = BAD_CAST "--";

    value = xmlTextReaderConstValue(reader);
    //DEBUG
    /*    printf("resultado: %d %d %s %d %d ", */
    xmlTextReaderDepth(reader);
    xmlTextReaderNodeType(reader);
    xmlTextReaderIsEmptyElement(reader);
    xmlTextReaderHasValue(reader);
    /*    if (value != NULL)
      printf("%s\n",value);
    else
      printf("\n",value);
    */
    //  printf("entramos en la seccion : %d subseccion %d parametro %d\n",*section,*subsection,*parameter);
   //Seccion
    if(!strcmp(name,"imágenes") &&  xmlTextReaderNodeType(reader)==1 && xmlTextReaderDepth(reader)==1)
      {
	//printf("entrando en imagenes \n");
	*section = 1;
	*subsection = 0;
      }
    else if (!strcmp(name,"videos") &&  xmlTextReaderNodeType(reader)==1 && xmlTextReaderDepth(reader)==1 )
      {
	//printf("entrando en videos \n");
	*section = 2;
	*subsection = 0;
      }
    else if (!strcmp(name,"audios") &&  xmlTextReaderNodeType(reader)==1 && xmlTextReaderDepth(reader)==1)
      {
	//printf("entrando en audios \n");
	*section = 3;
	*subsection = 0;
      }
    else if (!strcmp(name,"no_audios") &&  xmlTextReaderNodeType(reader)==1)
      {
	//printf("entrando en nº audios \n");
	*section = 6;
      }
    else if (!strcmp(name,"no_videos") &&  xmlTextReaderNodeType(reader)==1)
      {
	//printf("entrando en nº videos \n");
	*section = 5;
      }
    else if (!strcmp(name,"no_images") &&  xmlTextReaderNodeType(reader)==1)
      {
	*section = 4;
      }
    else if (!strcmp(name,"tipo_de_presentacion") &&  xmlTextReaderNodeType(reader)==1)
      {
	*section = 7;
      }
    //subseccion
    if(*section == 1)
      {
	//nueva imagen
	if(xmlTextReaderDepth(reader)==2 && xmlTextReaderNodeType(reader)==1)
	  {
	    //    printf("añadimos 1 a subsection\n");
	    *subsection = *subsection+1;
	  }
	else if(xmlTextReaderDepth(reader)==3 && xmlTextReaderNodeType(reader)==1 && !strcmp(name,"nombre")) //estamos capturando nombre
	  {
	    *parameter = 1;
	  }
	else if(xmlTextReaderDepth(reader)==3 && xmlTextReaderNodeType(reader)==15 && !strcmp(name,"nombre")) //dejamos de capturar nombre
	  {
	    *parameter = 0;
	  }
	else if(xmlTextReaderDepth(reader)==3 && xmlTextReaderNodeType(reader)==1 && !strcmp(name,"tiempo")) //estamos capturando tiempo
	  {
	    *parameter = 2;
	  }
	else if(xmlTextReaderDepth(reader)==3 && xmlTextReaderNodeType(reader)==15 && !strcmp(name,"tiempo")) //estamos capturando nombre
	  {
	    *parameter = 0;
	  }
	index = *subsection;	    	
	if(*parameter == 1 && xmlTextReaderDepth(reader)==4)
	  {
	    printf("INFO: capturado un archivo de imagen! \n");
	    printf("INFO: imágen nº %d nombre %s\n",*subsection,value);
	    strcpy(image_index2[index-1].name,value); //captura nombre
	  }
	else if(*parameter == 2 && xmlTextReaderDepth(reader)==4)
	  {
	    printf("INFO: capturado un archivo de imagen! \n");
	    printf("INFO: imágen nº %d tiempo %s\n",*subsection,value);
	    image_index2[index-1].seconds=atoi(value); //captura tiempo
	  }
      }
    //VIDEO
    else if(*section == 2)
      {
	
	//nuevo video
	if(xmlTextReaderDepth(reader)==2 && xmlTextReaderNodeType(reader)==1)
	  {
	    // printf("añadimos 1 a subsection\n");
	    *subsection=*subsection+1;
	  }
	if(xmlTextReaderDepth(reader)==3 && xmlTextReaderNodeType(reader)==1 && !strcmp(name,"nombre")) //estamos capturando nombre
	  {
	    *parameter = 1;
	  }
	else if(xmlTextReaderDepth(reader)==3 && xmlTextReaderNodeType(reader)==15 && !strcmp(name,"nombre")) //dejamos de capturar nombre
	  {
	    *parameter = 0;
	  }
	if(*parameter == 1 && xmlTextReaderDepth(reader)==4)
	  {
	    printf("INFO: capturado un archivo de video! \n");
	    printf("INFO: Nombre = %s \n",value);
	    strcpy(videos_index2[*subsection-1].name,value); //captura nombre
	  }
      }
    //AUDIO
    else if(*section == 3)
      {
	//nueva pista
	if(xmlTextReaderDepth(reader)==2 && xmlTextReaderNodeType(reader)==1)
	  {
	    // printf("añadimos 1 a subsection\n");
	    *subsection = *subsection+1;
	  }
	if(xmlTextReaderDepth(reader)==3 && xmlTextReaderNodeType(reader)==1 && !strcmp(name,"nombre")) //estamos capturando nombre
	  {
	    *parameter = 1;
	  }
	else if(xmlTextReaderDepth(reader)==3 && xmlTextReaderNodeType(reader)==15 && !strcmp(name,"nombre")) //dejamos de capturar nombre
	  {
	    *parameter = 0;
	  }

	if(*parameter == 1 && xmlTextReaderDepth(reader)==4)
	  {
	    printf("INFO: capturado un archivo de audio! \n");
	    printf("INFO: Nombre = %s \n",value);
	    strcpy(audios_index2[*subsection-1].name,value); //captura nombre
	  }
      }
    else if(*section == 4)
      {
	if ( xmlTextReaderDepth(reader)==2)
	  {
	    printf("reservando %s imagenes (%d) \n",value,atoi(value));
	    if (atoi(value) >= 0)
	      {
		no_images = atoi(value);//RETRIVIYNG IMAGES NUMBER
		image_index2 = malloc(no_images*sizeof(struct imagen));
	      }
	    else
	      printf("WARNING: NO IMAGES!!\n");
	  }
      }
    else if(*section == 5)
      {
	if ( xmlTextReaderDepth(reader)==2)
	  {
	    printf("reservando %s videos (%d) \n",value,atoi(value));	    
	    if (atoi(value) >= 0)
	      {
		no_videos = atoi(value);//RETRIVIYNG IMAGES NUMBER
		videos_index2  = malloc(no_videos*sizeof(struct pista));
	      }
	    else
	          printf("WARNING: NO VIDEOS!!\n");
	  }
      }
    else if(*section == 6)
      {
	if ( xmlTextReaderDepth(reader)==2)
	  {
	    printf("reservando %s audios (%d) \n",value,atoi(value));
	    if (atoi(value) >= 0)
	      {
		no_pistas = atoi(value);//RETRIVIYNG IMAGES NUMBER
		audios_index2  = malloc(no_pistas*sizeof(struct pista));
	      }
	    else
	         printf("WARNING: NO VIDEOS!!\n");
	  }
      }
    //TIPO DE PRESENTACION
    else if(*section == 7)
      {
	if ( xmlTextReaderDepth(reader)==2)
	  {
	    switch(atoi(value))
	      {
	      case(IMAGES_SOUND):
		printf("INFO: presentación de IMÁGENES CON SONIDO  (%d) \n",atoi(value));
		break;
	      case(IMAGES_NOSOUND):
		printf("INFO: presentación de tipo IMÁGENES SIN SONIDO (%d) \n",atoi(value));
		break;
	      case(VIDEOS_SOUND):
		printf("INFO: presentación de tipo VIDEO (%d) \n",atoi(value));
		break;
	      default:
		printf("AVISO: presentación de tipo DESCONOCIDO (%d) \n",atoi(value));
		break;
	      }
		if (atoi(value) >= 0)
	      {
		settings2 = malloc(sizeof(struct glob_settings));
		settings2->prestype = atoi(value);
	      }
	    else
	      {
		printf("ERROR: no se ha podido obtener un valor válido de tipo de presentación\n");
		printf("INFO: se procede con el valor por defecto\n");
		settings2 = malloc(sizeof(struct glob_settings));
		settings2->prestype = IMAGES_SOUND;
	         
	      }
	  }
      }
    //RELLENADO
    *videos_index=videos_index2;
    *audios_index=audios_index2;
    *image_index=image_index2;
    *settings = settings2;
    //printf("fin secuencia %d \n",*subsection); 
}

int recoverDoc(char *docname,struct imagen **image_index,struct pista **audios_index,struct pista **videos_index,struct glob_settings** settings)
{

  xmlTextReaderPtr reader;
  int ret,i,ret2;
  int section=0,subsection=0,parameter=0;
  struct imagen *image_index2;
  struct pista *audios_index2= NULL,*videos_index2 = NULL;
  struct glob_settings* settings2 = NULL;



   LIBXML_TEST_VERSION;
   /*
    * Pass some special parsing options to activate DTD attribute defaulting,
    * entities substitution and DTD validation
    */
   reader = xmlReaderForFile(docname, "UTF-8",
			     XML_PARSE_DTDATTR |  /* default DTD attributes */
			     XML_PARSE_NOENT/* |     substitute entities */
			     /*  XML_PARSE_DTDVALID*/); /* validate with the DTD */
   if (reader != NULL) {
     ret = xmlTextReaderRead(reader);
     while (ret == 1) {
       processNode(reader,&section,&subsection,&parameter,&image_index2,&audios_index2,&videos_index2,&settings2);
       ret = xmlTextReaderRead(reader);
       ret2 = 0;
     }    
     /*
      * Once the document has been fully parsed check the validation results
      */
     if (xmlTextReaderIsValid(reader) != 1) {
       fprintf(stderr, "Document %s does not validate\n", docname);
       ret2 = 10;
     }
     xmlFreeTextReader(reader);
     if (ret != 0) {
       fprintf(stderr, "%s : failed to parse\n", docname);
       ret2 = 10;
     }
   } 
   else 
     {
       fprintf(stderr, "ERROR: Unable to open %s\n", docname);
       fprintf(stderr, "INFO: Creating new one %s\n", docname);
       settings2 = malloc(sizeof(struct glob_settings));
       settings2->prestype = IMAGES_SOUND;
       populateDoc(docname,&image_index2,&audios_index2,&videos_index2,&settings2);
       ret2 = 10;
     }
   *settings = settings2;
   *image_index = image_index2;
   *videos_index = videos_index2;
   *audios_index = audios_index2;

    /*
     * Cleanup function for the XML library.
     */
    //xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    //xmlMemoryDump();
   return ret2;
}

int populateDoc(char *docname, struct imagen** image_index,struct pista** audios_index,struct pista** videos_index,struct glob_settings** settings)
{
  xmlDocPtr doc = NULL;       /* document pointer */
  xmlNodePtr root_node = NULL,node = NULL, node1 = NULL;/* node pointers */
  xmlDtdPtr dtd = NULL;       /* DTD pointer */
  char buff[256];
  int i, j;

  struct imagen *image_index2 = NULL;
  struct pista *audios_index2 = NULL,*videos_index2 = NULL;
  struct glob_settings* settings2 = NULL;

  image_index2 = *image_index;
  audios_index2 = *audios_index;
  videos_index2 = *videos_index;
  settings2 = *settings;
  
  LIBXML_TEST_VERSION;

  doc = xmlNewDoc(BAD_CAST "1.0");
  root_node = xmlNewNode(NULL, BAD_CAST "settings");
  xmlDocSetRootElement(doc, root_node);
  sprintf(buff,"%d",settings2->prestype);
  xmlNewChild(root_node, NULL, BAD_CAST "tipo_de_presentacion", BAD_CAST buff);
  sprintf(buff,"%d",no_images);
  xmlNewChild(root_node, NULL, BAD_CAST "no_images", BAD_CAST buff);
  sprintf(buff,"%d",no_videos);
  xmlNewChild(root_node, NULL, BAD_CAST "no_videos", BAD_CAST buff);
  sprintf(buff,"%d",no_pistas);
  xmlNewChild(root_node, NULL, BAD_CAST "no_audios", BAD_CAST buff);

  //imagenes
  node = xmlNewChild(root_node,NULL, BAD_CAST "imágenes",NULL);
  if(no_images > 0) //si tenemos imágenes rellenamos la configuración
    {
      for(i = 0;i<no_images;i++)
	{
	  sprintf(buff,"n%d",i+1);
	  node1 = xmlNewChild(node, NULL, BAD_CAST buff,NULL);
	  xmlNewChild(node1, NULL, BAD_CAST "nombre",BAD_CAST image_index2[i].name);
	  sprintf(buff,"%d",image_index2[i].seconds);
	  xmlNewChild(node1, NULL, BAD_CAST "tiempo",BAD_CAST buff);
	  //xmlNewChild(node, NULL, BAD_CAST "transicion",BAD_CAST image_index->transition);
	}
    }
  //videos
  node = xmlNewChild(root_node,NULL, BAD_CAST "videos",NULL);
  if(no_videos > 0) //si tenemos imágenes rellenamos la configuración
    {
      for(i = 0;i<no_videos;i++)
	{
	  sprintf(buff,"n%d",i+1);
	  node1 = xmlNewChild(node, NULL, BAD_CAST buff,NULL);
	  xmlNewChild(node1, NULL, BAD_CAST "nombre",BAD_CAST videos_index2[i].name);
	}
    }
  //audio
  node = xmlNewChild(root_node,NULL, BAD_CAST "audios",NULL);
  if(no_pistas > 0) //si tenemos imágenes rellenamos la configuración
    {
      for(i = 0;i<no_pistas;i++)
	{
	  sprintf(buff,"n%d",i+1);
	  node1= xmlNewChild(node, NULL, BAD_CAST buff,NULL);
	  xmlNewChild(node1, NULL, BAD_CAST "nombre",BAD_CAST audios_index2[i].name);
	}
    }
  /*
   *      * Dumping document to stdio or file
   *           */
  xmlSaveFormatFileEnc(docname, doc, "UTF-8", 1);

  xmlFreeDoc(doc);
  //xmlCleanupParser();
  //xmlMemoryDump();
  return 0;
}
