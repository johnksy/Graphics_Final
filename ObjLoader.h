#include <stdio.h>
#include <string.h>
#include <assert.h>
//#include <gl/GLAUX.H>
#include <cfloat>
#include "bmploader.cpp"
//#pragma comment( lib, "glaux.lib" ) 

#define Debug(x){MessageBox(NULL, x, "Debug:", MB_OK);}


struct vertex
{
	float x, y, z;
	unsigned int index;	
};
typedef vertex normal;
struct texcoord
{
	float u, v;
	unsigned int index;
};

struct face
{
	float x[3], y[3], z[3];		//vertices
	float u[3], v[3];			//text coords
	float a[3], b[3], c[3];		//normals
};


struct material
{	
	char name[32];
	int illum;
	char map_Kd[32];
	float Ni;
	float Kd[3];
	float Ka[3];
	float Tf[3];
	unsigned int texture;
};

class model
{
private:
	int faces;
	int coords;
	int normals;
	bool loaded;
	char mtllib[256];
	char filename[256];
	char directory[256];
	float min_v[3];
	float max_v[3]; 
	float mid[3];
	unsigned int vindex;
	unsigned int tindex;
	unsigned int nindex;
	struct vnode
	{
		vertex data;
		vnode * next;
	};
	struct tnode
	{
		texcoord data;
		tnode * next;
	};

	struct mnode
	{
		material data;
		mnode * next;
	};
	struct fnode
	{
		face data;
		mnode * mat;
		fnode * next;
	};
	
	vnode * vfirst;
	vnode * vcurrent;
	tnode * tfirst;
	tnode * tcurrent;
	vnode * nfirst;
	vnode * ncurrent;
	fnode * ffirst;
	fnode * fcurrent;
	mnode * mfirst;
	mnode * mcurrent;
	void skipComment(FILE *);

	bool loadVertex(FILE *);
	bool loadTexCoord(FILE *);
	bool loadNormal(FILE *);
	bool loadFace(FILE *);
	bool loadMaterialLib(FILE *);
	void loadMaterials(FILE *);
	void loadBmpTexture(char* fileName, GLuint* texture);
	void useMaterial(FILE *);

	int width, height;
	uchar4 *dst;
	int shape;	//triangle

public:
	bool Load(char * objfile, char * mtlname, int n);
	model()
	{
		vfirst=vcurrent= NULL;
		tfirst=tcurrent= NULL;
		nfirst=ncurrent= NULL;
		ffirst=fcurrent= NULL;
		mfirst=mcurrent= NULL;
		vindex=tindex=nindex= 1;
	}
	void draw();
	float Y(float, float){return 0.0;}
	bool Collide(float, float, float,float, float, float){return false;}
	float midx(){return mid[0];};
	float midy(){return mid[1];};
	float midz(){return mid[2];};
};

void model::loadBmpTexture(char* fileName, GLuint* texture )
{
	
	LoadBMPFile( &dst, &width, &height, fileName ) ;
		
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dst);

}

bool model::Load(char * objfile, char * mtlname, int n = 3)
{
	shape = n;
	char buffer[256];
	strcpy(filename, objfile);
	FILE * file = fopen(filename, "r");

	strcpy(mtllib, mtlname);

	if(file == NULL)
	{
		MessageBox(NULL, objfile, "Model file not found:", MB_OK);
		return false;
	}
	//int count = 0;
	min_v[0] = min_v[1] = min_v[2] = FLT_MAX;
	max_v[0] = max_v[1] = max_v[2] = -FLT_MAX;
	while(fscanf(file, "%s", buffer) != EOF)
	{
		//printf("%s ",buffer);
		if(!strcmp("#", buffer))skipComment(file);
		if(!strcmp("mtllib", buffer))loadMaterialLib(file);
		if(!strcmp("v", buffer))loadVertex(file);
		if(!strcmp("vt", buffer))loadTexCoord(file);
		if(!strcmp("vn", buffer))loadNormal(file);
		if(!strcmp("f", buffer))loadFace(file);
		if(!strcmp("s", buffer))fscanf(file, "%s", buffer); 
		if(!strcmp("usemtl", buffer))useMaterial(file);
		//count++;
		//printf(" %d\n",count);
		
		
	}

	for(int i=0; i<3; i++)
		mid[i] = (min_v[i]+max_v[i])/2;
	printf("diff:	%f, %f, %f\n",max_v[0] - min_v[0],max_v[1]-min_v[1],max_v[2]-min_v[2]);
	printf("mid:	%f, %f, %f\n", mid[0],mid[1],mid[2]);
	fclose(file);
	loaded = true;
	return true;
}

void model::useMaterial(FILE * file)
{
	char buffer[256];
	mnode * cursor = mfirst;
	fscanf(file, "%s", buffer);
	printf("%s ",buffer);
	while(strcmp(buffer, cursor->data.name)){
		cursor= cursor->next;
	}
	printf(" %s", cursor->data.name);
	mcurrent = cursor;
	
}

void model::skipComment(FILE * file)
{
	char buffer[256];
	fgets(buffer, 256, file);
}

bool model::loadMaterialLib(FILE * file)
{
	/*
	char * wd = strtok(filename, "/");
	char buffer[256];
	fscanf(file, "%s", buffer);
	sprintf(mtllib, "%s/%s", wd, buffer);
	strcpy(directory, wd);
	*/

	FILE * lib = fopen(mtllib, "r");
	//printf("mtl: %s", mtllib);
	if(lib == NULL)
	{
		MessageBox(NULL, mtllib, "Material library not found:", MB_OK);
		return false;
	}
	else loadMaterials(lib);
	fclose(lib);
	return true;
}

void model::loadMaterials(FILE * file)
{

	char parameter[32];
	mnode * temp;
	while(fscanf(file, "%s", parameter) != EOF)
	{

		if(!strcmp("newmtl", parameter))
		{
			temp = new mnode();
			fscanf(file, "%s", temp->data.name);
		}
		if(!strcmp("illum", parameter))
			fscanf(file, "%i", &temp->data.illum);
		if(!strcmp("map_Kd", parameter))
		{
			fscanf(file, "%s", temp->data.map_Kd);

			if(strstr(temp->data.map_Kd, ".bmp") != NULL){
				loadBmpTexture( temp->data.map_Kd, & (temp->data.texture ) );
			}
			else
				assert("Texture format should be .bmp!");

		}
		if(!strcmp("Ni", parameter))
			fscanf(file, "%f", &temp->data.Ni);
		if(!strcmp("Kd", parameter))
			fscanf(file, "%f %f %f", &temp->data.Kd[0],&temp->data.Kd[1],&temp->data.Kd[2]);
		if(!strcmp("Ka", parameter))
			fscanf(file, "%f %f %f", &temp->data.Ka[0],&temp->data.Ka[1],&temp->data.Ka[2]);
		if(!strcmp("Tf", parameter))
		{
			fscanf(file, "%f %f %f", &temp->data.Tf[0],&temp->data.Tf[1],&temp->data.Tf[2]);
			if(mfirst == NULL)
			{
				mfirst = temp;
				mcurrent = temp;
				mfirst->next = NULL;
			}
			else
			{
				mcurrent->next = temp;
				mcurrent = mcurrent->next;
				mcurrent->next = NULL;
			}

		}
	}

}

bool model::loadVertex(FILE * file)
{
	vnode * temp = new vnode();
	fscanf(file, "%f %f %f", &temp->data.x, &temp->data.y, &temp->data.z);
	if(temp->data.x < min_v[0]){min_v[0] = temp->data.x;}
	if(temp->data.y < min_v[1]){min_v[1] = temp->data.y;}
	if(temp->data.z < min_v[2]){min_v[2] = temp->data.z;}
	if(temp->data.x > max_v[0]){max_v[0] = temp->data.x;}
	if(temp->data.y > max_v[1]){max_v[1] = temp->data.y;}
	if(temp->data.z > max_v[2]){max_v[2] = temp->data.z;}

	//printf("%f %f %f", temp->data.x, temp->data.y, temp->data.z);
	temp->data.index = vindex;
	if(vfirst == NULL)
	{
		vfirst = temp;
		vcurrent = temp;
		vfirst->next = NULL;
	}
	else
	{
		vcurrent->next = temp;
		vcurrent = vcurrent->next;
		vcurrent->next = NULL;
	}
	vindex++;
	return true;
}

bool model::loadTexCoord(FILE * file)
{
	tnode * temp = new tnode();
	fscanf(file, "%f %f", &temp->data.u, &temp->data.v);
	//printf("%f %f", temp->data.u, temp->data.v);
	temp->data.index = tindex;
	temp->next = NULL;
	if(tfirst == NULL)
	{
		tfirst = temp;
		tcurrent = temp;
	}
	else
	{
		tcurrent->next = temp;
		tcurrent = tcurrent->next;
	}
	tindex++;
	return true;
}

bool model::loadNormal(FILE * file)
{
	vnode * temp = new vnode();
	fscanf(file, "%f %f %f", &temp->data.x, &temp->data.y, &temp->data.z);
	//printf("%f %f %f", temp->data.x, temp->data.y, temp->data.z);
	temp->data.index = nindex;
	temp->next = NULL;
	if(nfirst == NULL)
	{
		nfirst = temp;
		ncurrent = temp;
	}
	else
	{
		ncurrent->next = temp;
		ncurrent = ncurrent->next;
	}
	nindex++;
	return true;
}

bool model::loadFace(FILE * file)
{
	fnode * temp = new fnode();
	temp->mat = mcurrent;
	vnode * vcursor = vfirst;
	tnode * tcursor = tfirst;
	vnode * ncursor = nfirst;
	unsigned int *v_index, *t_index, *n_index;
	v_index = new unsigned int[shape];
	t_index = new unsigned int[shape];
	n_index = new unsigned int[shape];
	for(int i = 0; i < shape; i++)
	{
		vcursor = vfirst;
		tcursor = tfirst;
		ncursor = nfirst;
		fscanf(file, "%i/%i/%i", &v_index[i], &t_index[i], &n_index[i]);
		//printf("%i/%i/%i ", v_index[i], t_index[i], n_index[i]);
		for(int v = 1; v != v_index[i]; v++)
			vcursor = vcursor->next;
		temp->data.x[i] = vcursor->data.x;
		temp->data.y[i] = vcursor->data.y;
		temp->data.z[i] = vcursor->data.z;

		for(int k = 1; k != t_index[i]; k++)
			tcursor = tcursor->next;
		temp->data.u[i] = tcursor->data.u;
		temp->data.v[i] = tcursor->data.v;

		for(int j = 1; j != n_index[i]; j++)
			ncursor = ncursor->next;
		temp->data.a[i] = ncursor->data.x;
		temp->data.b[i] = ncursor->data.y;
		temp->data.c[i] = ncursor->data.z;
	}
	//printf("\n");

	temp->next = NULL;
	if(ffirst == NULL)
	{
		ffirst = temp;
		fcurrent = temp;
		ffirst->next = NULL;
	}
	else
	{
		fcurrent->next = temp;
		fcurrent = fcurrent->next;
		fcurrent->next = NULL;
	}
	faces++;
	return true;
}

void model::draw()
{
#ifdef __GL_H__
	if(loaded)
	{
		fnode * fcursor = ffirst;
		if( ffirst->mat->data.texture != NULL){
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_2D, ffirst->mat->data.texture);
		}
		int nCnt = 0;

		glBegin(GL_TRIANGLES);
		while(fcursor != NULL)
		{

			glBindTexture(GL_TEXTURE_2D, fcursor->mat->data.texture );
			nCnt++;
			
			glColor3f(1.0f, 1.0f, 1.0f);

			for(int j=0; j<shape; j++){
				glTexCoord2f(fcursor->data.u[j], fcursor->data.v[j]);
				glNormal3f(fcursor->data.a[j], fcursor->data.b[j], fcursor->data.c[j]);
				glVertex3f(fcursor->data.x[j], fcursor->data.y[j], fcursor->data.z[j]);
			}

			fcursor = fcursor->next;
			
		}
		
		glEnd();
	}
#endif
}
